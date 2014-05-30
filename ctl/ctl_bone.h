#ifndef INCLUDE_BONE_HPP
#define INCLUDE_BONE_HPP

#include "Cuttlebone/Broadcaster.hpp"
#include "Cuttlebone/Configuration.hpp"
#include "Cuttlebone/Packet.hpp"
#include "Cuttlebone/Queue.hpp"
#include "Cuttlebone/Receiver.hpp"
#include "Cuttlebone/Selector.hpp"
#include "Cuttlebone/Time.hpp"
#include "Cuttlebone/Timer.hpp"
#include "Cuttlebone/Log.hpp"

#include <thread>
using namespace std;

#define LITTLE_WAIT_TIME_US (100)

#define PORT (63057)

template <typename STATE, unsigned PACKET_SIZE = 1400>
struct Simulator : Timer {

  virtual void setup(STATE& state) = 0;
  virtual void update(float dt, STATE& state) = 0;

  const char* broadcastIp;
  float timerRate;
  bool shouldLog;
  bool done;
  bool waitingToStart;
  STATE* simulationState;

  Queue<STATE> simulateBroadcast;
  thread broadcast;

  Simulator(const char* broadcastIp = "127.0.0.1", float timerRate = 1 / 60.0f)
      : broadcastIp(broadcastIp),
        timerRate(timerRate),
        shouldLog(true),
        done(false),
        waitingToStart(true),
        simulationState(new STATE) {}

  virtual void start() {

    broadcast = thread([&]() {
      Broadcaster broadcaster;
      broadcaster.init(PACKET_SIZE, broadcastIp, PORT, false);
      Packet<PACKET_SIZE> p;
      STATE* state = new STATE;
      int frame = 0;

      while (waitingToStart)
        usleep(LITTLE_WAIT_TIME_US);

      while (!done) {
        int popCount = 0;
        while (simulateBroadcast.pop(*state))
          popCount++;

        if (popCount) {
          if (shouldLog)
            LOG("sending packet %d", frame);
          PacketMaker<STATE, Packet<PACKET_SIZE> > packetMaker(*state, frame);
          while (packetMaker.fill(p))
            broadcaster.send((unsigned char*)&p);
          frame++;
        }
      }

      delete state;
    });

    setup(*simulationState);

    waitingToStart = false;
    Timer::start(1 / 60.0f);

    getchar();

    Timer::stop();
    done = true;
    broadcast.join();
  }

  void onTimer() {
    static float last;
    static float now;
    Timestamp<> ts;
    last = now;
    now = ts.stamp();
    update(now - last, *simulationState);
    simulateBroadcast.push(*simulationState);
  }
};

template <typename STATE, unsigned PACKET_SIZE = 1400>
struct Renderer {

  virtual void gotState(float dt, STATE& state, int popCount) = 0;

  bool shouldLog;
  bool done;
  bool waitingToStart;
  STATE* renderState;

  Queue<STATE> receiveRender;
  thread receive, render;

  Renderer()
      : shouldLog(false),
        done(false),
        waitingToStart(true),
        renderState(new STATE) {}

  virtual void start() {

    receive = thread([&]() {
      Receiver receiver;
      receiver.init(PORT, false);
      Packet<PACKET_SIZE> p;
      STATE* state = new STATE;

      while (waitingToStart)
        usleep(LITTLE_WAIT_TIME_US);

      while (!done) {

        if (!receiver.receive((unsigned char*)&p, PACKET_SIZE, 0.2f))
          continue;

      ABORT_FRAME:
        ;
        // wait until we're at the begining of a frame
        if (p.header.partNumber != 0)
          continue;

        PacketTaker<STATE, Packet<PACKET_SIZE> > packetTaker(
            *state, p.header.frameNumber);

        packetTaker.take(p);

        while (!packetTaker.isComplete()) {
          if (receiver.receive((unsigned char*)&p, PACKET_SIZE, 0.2f)) {
            if (!packetTaker.take(p)) {
              // got a part from an unexpected frame before we finished this
              // frame
              LOG("ABORT FRAME");
              packetTaker.summary();
              goto ABORT_FRAME;
            }
          }
        }

        if (shouldLog)
          LOG("got packet %d", p.header.frameNumber);

        receiveRender.push(*state);
      }

      delete state;
    });

    render = thread([&]() {
      while (waitingToStart)
        usleep(LITTLE_WAIT_TIME_US);

      while (!done) {
        int popCount = 0;
        while (receiveRender.pop(*renderState))
          popCount++;

        if (popCount) {
          static float localLast;
          static float localNow;
          static Timestamp<> ts;
          localLast = localNow;
          localNow = ts.stamp();
          gotState(localNow - localLast, *renderState, popCount);
        }
      }
    });

    waitingToStart = false;

    getchar();

    done = true;
    receive.join();
    render.join();
  }
};

#endif
