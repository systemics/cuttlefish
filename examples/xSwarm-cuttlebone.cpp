/*
 * =====================================================================================
 *
 *       Filename:  xSwarm-cuttlebone.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/04/2014 14:59:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#define SERVER_PATH "/home/ky/"
//#define SERVER_PATH "/Users/ky/code/cuttlefish/"
#define CLIENT_PATH "/home/pi/"

#include "ctl_bone.h"
#include "gfx/gfx_matrix.h"
#include "vsr/vsr_cga3D_frame.h"
#include "vsr/vsr_stat.h"

#define NUMAGENTS 30

struct Foo {
  float value;
  gfx::Vec3f pos[NUMAGENTS];
  gfx::Vec4f rot[NUMAGENTS];
};

#if SIM

using namespace vsr;
using namespace gfx;

struct MyApp : ctl::Simulator<Foo> {

  vector<Frame> frame;
  //vector<Frame> nearest;

  float range,thresh,min;

  MyApp() : Simulator<Foo>("192.168.7.255", 1 / 30.f) {
    //
    // shouldLog = true;
  }

  virtual void setup(Foo& state) { 
    state.value = 0; 

    frame = vector<Frame>(NUMAGENTS);

    range = 8;
    thresh = 2.5;
    min = .75;

    Rand::Seed();
    for (auto& f : frame ){
      Vec v( Rand::Num(-1,1) , Rand::Num(-1,1), Rand::Num(-1,1));
      f.pos() = Ro::null( v * range );
      f.rot() = Gen::rot( Biv(  Rand::Num(), Rand::Num(), Rand::Num() ) );
      f.scale() = .5;
    }   
  }
  
  virtual void update(float dt, Foo& state) {

     
      state.value += 0.01f;

      Point force = Ro::null( Vec( cos(state.value), sin(state.value), 0 ) * 30 );


      float acc = .05;
      float rotAcc = .02; 

      int numNeighbors = 3;
      for (auto& fa : frame){

        float famt = 1.0 / (.01 + Ro::sqd( fa.pos(), force ) );

        vector<Frame> nearest;
        vector<Frame> toonear;

        for (auto& fb : frame){
          float halfplane = (fb.pos() <= fa.dxy())[0];
          if ( halfplane > 0 ){
            float dist = Ro::sqd( fa.bound(), fb.bound() );
            if (dist < thresh) nearest.push_back(fb);
            if (dist < min) toonear.push_back(fb);
            if (nearest.size() == numNeighbors) break;
         }
        }
        
        Biv db; // Amount to orient
        Vec dx; // Amount to move

        if (!toonear.empty()){
          fa.db() = fa.xz() * .01; 
        } else {

         for (auto& neigh : nearest){
           db += Gen::log( neigh.rot() ) / nearest.size();
           dx += Vec( neigh.pos() - fa.pos() ) / nearest.size();
         }

         dx += Vec( fa.pos() - force ) * famt;

         fa.db() = db * rotAcc;
         fa.dx() = dx * acc;

         if (nearest.empty()){
           fa.db() = fa.xz() * .01;
           fa.dx() = fa.z() * acc;
         }
        }

        fa.move(); fa.spin();

      }   

      for (int i=0;i<NUMAGENTS;++i){
        state.pos[i].set( frame[i].pos()[0], frame[i].pos()[1], frame[i].pos()[2] );
        state.rot[i].set( frame[i].rot()[0], frame[i].rot()[1], frame[i].rot()[2], frame[i].rot()[3] );
      }

    //    LOG("update(%f) -> %f", dt, state.value);
    //
    //
    //
  }



};

#else

#include "ctl_app.h"
#include "vsr/vsr_simplex.h"
#include "temp/hyperAmt_glsl.h"
#include "gfx/gfx_process.h"
#include "temp/gfx_hyper.h"

using namespace ctl;
using namespace gfx;
using namespace vsr;
using namespace vsr::cga3D;

struct MyApp : CuttleboneApp<Foo> {

  float v;

  MBO * simplexMBO;
  HyperSimplex * process;

  Simplex<4> simplex;

  //Frame frame[NUMAGENTS];
  Rot rot[NUMAGENTS];
  Vec pos[NUMAGENTS];

  MyApp() : CuttleboneApp<Foo>(Layout(4, 4), 30.0) {
    // Sound::init(1024, 48000);
  }

  virtual void setup() {
    Mesh mesh;
    for (auto& i : simplex.verts ){
      Vertex v(0,0,0);
      v.Norm = Vec3f(i[0],i[1],i[2]);
      v.Col = Vec4f(i[0],i[1],i[2],i[3]);
      mesh.add(v);
    }

    for (auto& i : simplex.edges ){
      mesh.add( i.a).add(i.b);//.add(i.c);
    }

    mesh.mode( GL::LS );
    simplexMBO = new MBO( mesh );

    process = new HyperSimplex(0,0,this);
    
    
  }

  virtual void update(float dt, Foo& state, int popCount) {
    v = state.value - int(state.value);
    for (int i = 0; i<NUMAGENTS; ++i){
      rot[i] = Rot( state.rot[i][0], state.rot[i][1], state.rot[i][2], state.rot[i][3]);
      pos[i] = Vec( state.pos[i][0], state.pos[i][1], state.pos[i][2]);
    }
  }

  //THIS DRAWS
  virtual void onDraw(){ 

    static float val = 0.0;
    val += .1;

    pipe.begin( *simplexMBO );

      for (int i = 0; i<NUMAGENTS; ++i){
        process -> bindModelView( mvm * vsr::Xf::mat(rot[i], pos[i], .5 ) );
        process -> program -> uniform("amt", val );
        pipe.draw(*simplexMBO);
      }

    pipe.end( *simplexMBO );

  } 

  virtual void onFrame(){
    Renderer::clear();

    scene.updateMatrices();
    mvm = scene.xf.modelViewMatrixf();

    process -> bind();   
      process -> bind( scene.xf );      
        onDraw();
    process -> unbind();

    w->swapBuffers();
  }


  virtual void onSound(Sound::SoundData& io) {}
};

#endif

int main() { MyApp().start(); }
