/*
 * =====================================================================================
 *
 *       Filename:  swarm.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/27/2015 14:11:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail->wolftype
 *   Organization:  wolftype
 *
 * =====================================================================================
 */

#ifndef  swarm_INC
#define  swarm_INC

#include "state.hpp"

#include "vsr/space/vsr_cga3D_funcs.h"
#include "form/vsr_cga3D_frame.h"
#include "form/vsr_knot.h"
#include "form/vsr_twist.h"
#include "util/vsr_stat.h"
#include "form/vsr_group.h"

#include "ctl_grid.h"

using namespace vsr;
using namespace vsr::cga;

//FOOD CRYSTALS
struct Substrate{

  float time =0;
  float wSpacing = 70;
  float hSpacing = 50;
  ctl::Grid grid = ctl::Grid(NUM_CELLS_WIDTH_SUBSTRATE,NUM_CELLS_HEIGHT_SUBSTRATE); ///< A Read/Write Scalar Field  

  //for rendering:
  Point pnt[NUM_VERTICES_SUBSTRATE];

  Substrate(){

    for (int i=0;i<NUM_CELLS_WIDTH_SUBSTRATE;++i){
      for (int j=0;j<NUM_CELLS_HEIGHT_SUBSTRATE;++j){
          float ti = (float)i/NUM_CELLS_WIDTH_SUBSTRATE;
          float tj = (float)j/NUM_CELLS_HEIGHT_SUBSTRATE;
          auto tmp = circle( -wSpacing/2.0 + ti * wSpacing, -hSpacing/2.0 + tj * hSpacing, 0 );
          for (int k=0;k<NUM_VERTEX_PER_CELL;++k){
           int idx = (i*NUM_CELLS_HEIGHT_SUBSTRATE+j)*NUM_VERTEX_PER_CELL + k;

            auto p = point(tmp, TWOPI*(float)k/NUM_VERTEX_PER_CELL);
            pnt[idx]=p;
          }
      }
    }
  }

  void step(float dt){}
  
};

//    SpaceGroup2D<Vec> sg = SpaceGroup2D<Vec>(3,1,false); // p3m1
//    vector<Point> motif;
//   
//    Substrate() {
//      motif = vector<Point>(NUM_VERTEX_BASE_SUBSTRATE);  
//      for (int i =0;i<NUM_VERTEX_BASE_SUBSTRATE;++i){
//        float t = TWOPI*(float)i/NUM_VERTEX_BASE_SUBSTRATE;
//        motif[i] = vsr::cga::point( CXY(1), t );
//      }
//    }
//  
//    void step(float dt){
//      time += dt;
//      for (int i =0;i<NUM_VERTEX_BASE_SUBSTRATE;++i){
//        float t = TWOPI*(float)i/NUM_VERTEX_BASE_SUBSTRATE;
//        motif[i] = vsr::cga::point( CXY(1), t );
//      }
//  
//      makeMeshData();
//    }
//    
//    void makeMeshData(){
//      //transpose
//      for (int i=0;i<NUM_VERTEX_BASE_SUBSTRATE;++i){
//         auto tmp = sg.apply( motif[i], NUM_CELLS_WIDTH_SUBSTRATE, NUM_CELLS_HEIGHT_SUBSTRATE);
//   
//        for (int k=0;k<NUM_CELLS_WIDTH_SUBSTRATE;++k){
//        for (int m=0;m<NUM_CELLS_HEIGHT_SUBSTRATE;++m){
//          
//          int firstIn = (k*NUM_CELLS_HEIGHT_SUBSTRATE + m) * NUM_REFLECTIONS_PER_CELL;
//          int firstOut = (k*NUM_CELLS_HEIGHT_SUBSTRATE + m) * NUM_VERTEX_PER_CELL + i;
//          
//          for (int j =0;j<NUM_REFLECTIONS_PER_CELL;++j){
//           int idx = j*NUM_VERTEX_BASE_SUBSTRATE + firstOut;
//           pnt[idx] = tmp[firstIn+j];
//          }
//          
//        }}
//         
//      }
//    }



struct Organism;

/*-----------------------------------------------------------------------------
 *  POPULATION
 *-----------------------------------------------------------------------------*/
struct Population{

  Population(int num=NUM_AGENTS) : mNum(num) {}

  ~Population();
    
   Substrate substrate; 
  /*-----------------------------------------------------------------------------
   *  A Distance Relationship between Organisms
   *-----------------------------------------------------------------------------*/
  struct Relationship {
    Organism * partner;
    float dist;
  };

  
  /*-----------------------------------------------------------------------------
   *  A Neighborhood of Relationships
   *-----------------------------------------------------------------------------*/
  struct Neighborhood{
    vector<Relationship> nearest;
    vector<Relationship> toonear;
    Point food;
    void clear(){ nearest.clear(); toonear.clear(); }
  };
  

  /*-----------------------------------------------------------------------------
   *  A List of Members of the Population
   *-----------------------------------------------------------------------------*/
  vector<Organism*> member;

  /*-----------------------------------------------------------------------------
   *  A List of Known Food Sources
   *-----------------------------------------------------------------------------*/
  vector<Point> foodsource;

  /// initialize on a type of organism
  template<typename T>
  void init();
  
  /// reset
  template<typename T>
  void reset();

  /// Add a member
  void add(Organism * org){ member.push_back(org); }

  /// Build a Neighborhood of relationships to other members (and to food)
  void buildNeighborhoods();  

  /// timestep
  void step(float dt);

  float mSpacing=1;               ///<-- Initial Spacing of Population
  float maxDistance = 10;         ///<-- Threshold of influence between Members
  float minDistance = 1;          ///<-- Minimum Spacing Between Members
  int   maxNeighbors = 3;         ///<-- Max number of influencing neighbors
  int   mNum=NUM_AGENTS;          ///<-- Size of Population
  int   maxNum = 100;             ///<-- Max Size of Population


  ///GLOBAL CONTROL OF MEMBERS
  float globalFlockRotVel;
  float globalAvertRotVel;
  float globalSourceRotVel;


  ///Behavior Switching
  void startFollowing();
  void stopFollowing();

};


/*-----------------------------------------------------------------------------
 *  ORGANISM
 *-----------------------------------------------------------------------------*/
struct Organism : public Frame {

  float time=0;
  float energy=100;


  
  Point pnt[NUM_VERTEX_PER_AGENT];      ///<-- for mesh data . . .
  
  Population * mPopulation;
  void population(Population * pop) { mPopulation = pop; }

  enum Behavior{
    Follow = 1, 
    Flee = 1 << 1, 
    Fold = 1 << 2, 
    Unfold = 1 << 3, 
    Feed = 1 << 4, 
    Force = 1 << 5, 
    Flock = 1 << 6
  };

  Organism( Point p = point(0,0,0), Rotor r = Rot(1) ) : Frame(p,r),
  target(NULL), mBehavior( Flock | Force | Feed )
  {}


  void behaviorOn(int mode){ mBehavior |= mode; }
  void behaviorOff(int mode){ mBehavior &= ~mode; }
  void behaviorToggle(int mode) { mBehavior ^= mode; }


  virtual void init(){}

  virtual void onStep(float dt){}       ///<-- subclass's onStep(dt) method

  virtual void step(float dt){

        time+=dt+vVelocity;
        energy -= vVelocity;

        onStep(dt); //subclass's onStep(dt) method
       
        if (mBehavior & Flock)  flock();
        if (mBehavior & Force)  force();
        if (mBehavior & Feed)   feed();
        if (mBehavior & Follow) follow();
        if (mBehavior & Flee)   flee();
        if (mBehavior & Fold)   fold();
        if (mBehavior & Unfold) unfold();
        
         
        this->dVec += this->z() * vVelocity;
        
        this->dBiv *= aBiv; 
        this->dVec *= aVec;
        
        this->spin();
        this->move();


  };

  virtual void feed(){
    //orient towards food
     dBiv += relOrientBiv( foodsource ) * (vSourceRotVel+mPopulation->globalSourceRotVel);
  }

  virtual void flock(){
        
       vVelocity = 0.01;
       //orient towards neighbors
       if (!mNeighborhood.nearest.empty()){
        for (auto& n : mNeighborhood.nearest){
          if (n.dist>FPERROR) {
            dBiv += this->relOrientBiv( n.partner->pos() ) * (vFlockRotVel+mPopulation->globalFlockRotVel);
            vVelocity += vFlockAcc;
          }
        } 
       }
               
        //orient away from neighbors that are too close 
        if (!mNeighborhood.toonear.empty()){
        for (auto& n : mNeighborhood.toonear){
          if (n.dist>FPERROR) dBiv -= this->relOrientBiv( n.partner->pos() ) * (vAvertRotVel+mPopulation->globalAvertRotVel);
         } 
        }


  }

  
  void follow(){
    //if(!target) if (!mNeighborhood.nearest.empty()){ target = mNeighborhood.nearest[0].partner; }
    //if(target) this -> relTwist( *target, vFollowVel );
  }

  void flee(){
    if(target) this -> relTwistAway( *target, vFollowVel );
  }

  virtual void force(){
    //get velocity from world's vector field
    //auto v = world.vecAt( this->pos() );
  }
  
  virtual void fold(){}
  virtual void unfold(){}

  Population::Neighborhood mNeighborhood;
  Population::Neighborhood& neighborhood() { return mNeighborhood; }

  Point foodsource = point(0,0,0);  ///<-- food source position from world
  Organism * target;                ///<-- temporary target to chase?

  int mBehavior;                    ///<-- Behavior Mode
 
  float vVelocity = .00;            ///<-- General Velocity
  float vFollowVel = .01;           ///<-- Velocity to chase
  
  float vSourceRotVel = .01;        ///<-- Rotational Velocity to food source 
  
  float vFlockRotVel =.01;          ///<-- Rotational Velocity during Flocking
  float vAvertRotVel = .01;         ///<-- Rotational Velocity to Avert during Flocking

  float vFlockAcc =.1;  
};


Population::~Population(){
    for (auto& i : member) if(i) delete i;
}

template<typename T> ///<-- T must be an organism
void Population::init(){
  
    member.clear();
    member = vector<Organism*>(mNum);
    float range = mNum * mSpacing;
    Rand::Seed();
    for (auto& i : member ){
      if (!i) i = new T();
      i->population(this);
      i->pos() = point( -range/2.0 + Rand::Num(range), -range/2.0 + Rand::Num(range), -range/2.0 + Rand::Num(range) ); 
      i->rot() = gen::rot( TWOPI*Rand::Num(), -PIOVERTWO + PI * Rand::Num() );  
      i->init(); 
    }

}

template<typename T>
void Population::reset(){
   init<T>();
}


/// Build Neighborhoods to Other Members (and To Food Sources)
void Population::buildNeighborhoods() {
    
    for (auto& ma : member){
       auto& fa = *ma;
       
       fa.neighborhood().clear();

       for (auto& mb : member){
         auto& fb = *mb;
         
         float halfplane = (fb.pos() <= fa.dxy())[0];               ///<-- on which side of fa's half-plane is fb?
         if ( halfplane > 0 ){                                      ///<-- if fb is in front of fa, fa can "see it"
           
           float dist = round::sqd( fa.bound(), fb.bound() );       ///<-- distance between fa and fb
           if (dist < minDistance) fa.neighborhood().toonear.push_back( {mb,dist} );   ///<-- if distance is less than min threshold fb is too near
           else if (dist < maxDistance) fa.neighborhood().nearest.push_back( {mb,dist} );               ///<-- if distance is below max threshhold then add to nearest
           if (fa.neighborhood().nearest.size() > maxNeighbors || fa.neighborhood().toonear.size() > maxNeighbors ) break;       //<-- stop after max # nearest neighbors
        }
       }

       //Find closest Food Source
       for (auto& f : foodsource){

       }

    }
}

void Population::step(float dt){
  buildNeighborhoods();
  for (auto& i : member) i->step(dt);
  substrate.step(dt);
}
  


/*-----------------------------------------------------------------------------
 *  A JELLY
 *-----------------------------------------------------------------------------*/
struct Jelly : Organism {

  Circle midsection;
  TorusKnot tk;
  PointGroup3D<Vec> pg;

  vector<Point> vp;

  Jelly( Point p = point(0,0,0), Rotor r = Rot(1) ) 
  : Organism(p,r),
    pg(3,3) 
   { 
     init();
   }


  virtual void init(){   
    tk = TorusKnot(3,2);
  }


  virtual void onStep(float dt){
    //tk.HF.cir() = this->cxz();
    //auto par = tk.par() * .01;
    
    auto cir = round::produce( round::dls(-1.0,0,0,0).trs(1,1,0), Biv::xz );//this->xz() );
    auto shrink = cir.dilate( PAO, .5 ).trs(.5,0,0);
    
    midsection = shrink.boost( cir.dual() * time  );

    makeMeshData();

  }

  void makeMeshData(){

    vector<Point> base;

    for (int i =0; i<NUM_VERTEX_BASE; ++i){
      float t = TWOPI * (float)i/(NUM_VERTEX_BASE);
       base.push_back( point(midsection, t) );
    }
    
    //transpose . . .
    for (int i=0;i<NUM_VERTEX_BASE;++i){
        auto vp = pg(base[i]);
        for (int j=0;j<MAX_NUM_REFLECTIONS;++j){
          int idx = j*NUM_VERTEX_BASE + i;
          pnt[idx] = vp[j].spin(this->rot()) + this->pos();
        }
    }
    
  } 
  

};  


#endif   /* ----- #ifndef swarm_INC  ----- */

