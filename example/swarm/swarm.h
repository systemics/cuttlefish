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

#include "form/vsr_knot.h"
#include "form/vsr_twist.h"
#include "util/vsr_stat.h"
#include "form/vsr_group.h"

using namespace vsr;
using namespace vsr::cga;


struct Organism;

/*-----------------------------------------------------------------------------
 *  POPULATION
 *-----------------------------------------------------------------------------*/
struct Population{

  Population(int num=NUM_AGENTS) : mNum(num) {}

  ~Population();
    
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
    void clear(){ nearest.clear(); toonear.clear(); }
  };
  

  /*-----------------------------------------------------------------------------
   *  A List of Members of the Population
   *-----------------------------------------------------------------------------*/
  vector<Organism*> member;

  /// initialize on a type of organism
  template<typename T>
  void init();
  
  /// reset
  template<typename T>
  void reset();

  /// Add a member
  void add(Organism * org){ member.push_back(org); }

  /// Build a Neighborhood of relationships
  void buildNeighborhoods();  

  /// timestep
  void step(float dt);


  float mSpacing=1;               ///<-- Initial Spacing of Population
  float maxDistance = 10;         ///<-- Threshold of influence between Members
  float minDistance = 1;          ///<-- Minimum Spacing Between Members
  int   maxNeighbors = 3;         ///<-- Max number of influencing neighbors
  int   mNum=NUM_AGENTS;          ///<-- Size of Population
  int   maxNum = 100;             ///<-- Max Size of Population


  float globalFlockRotVel;
  float globalAvertRotVel;
  float globalSourceRotVel;


  bool bDrawNetwork;

//  void draw(GFXSceneNode *re);
//  void draw();
};



/*-----------------------------------------------------------------------------
 *  ORGANISM
 *-----------------------------------------------------------------------------*/
struct Organism : public Frame {


  float time=0;
  Point pnt[NUM_VERTEX_PER_AGENT];      ///for mesh data . . .
  
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


  virtual void init(){}

  virtual void onStep(float dt){}

  virtual void step(float dt){

        time+=dt;

        onStep(dt);
        
       
        if (mBehavior & Flock)  flock();
        if (mBehavior & Force)  force();
        if (mBehavior & Feed)   feed();
        if (mBehavior & Follow) follow();
        if (mBehavior & Flee)   flee();
        if (mBehavior & Fold)   fold();
        if (mBehavior & Unfold) unfold();
        
        //move forward by 
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
    if(target) this -> relTwist( *target, vFollowVel );
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

//  virtual void draw(GFXSceneNode * re){
//      render::pipe( *(Frame*)(this), re );     
//  };
//  virtual void draw(){
//      render::draw( *(Frame*)(this) );     
//  };

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

    }
}

void Population::step(float dt){
  buildNeighborhoods();
  for (auto& i : member) i->step(dt);
}
  
//void Population::draw(GFXSceneNode * re){
//  for(auto& i : member) i->draw(re);
//  if (bDrawNetwork) render::pipe(mNetworkMesh,re);
//}

//void Population::draw(){
//  for(auto& i : member) i->draw();
//
//    glBegin(GL_LINES);
//      for(auto& i : member){
//            glColor3f(0,1,0);
//
//        for (auto& j : i->neighborhood().nearest){
//          GL::vertex( i->pos() );
//          GL::vertex( j.partner->pos() );
//        }
//            glColor3f(0,1,1);
//
//        for (auto& j : i->neighborhood().toonear){
//          GL::vertex( i->pos() );
//          GL::vertex( j.partner->pos() );
//        }
//
//      }
//    glEnd();   
//}





/*-----------------------------------------------------------------------------
 *  A JELLY
 *-----------------------------------------------------------------------------*/
struct Jelly : Organism {

  Circle midsectionR, midsectionL; 
  TorusKnot tk;
  PointGroup3D<Vec> pg;

  vector<Point> vp;

  Jelly( Point p = point(0,0,0), Rotor r = Rot(1) ) 
  : Organism(p,r),
    pg(4,3,false,true) 
   { 
    // init(); 
    }


  virtual void init(){
   
    tk = TorusKnot(3,2);
   // midsectionR = this->cxz().dilate( this->pos(), .5 );//.trs(.2,0,0);
   // mBehavior = 0;
   // midsectionL = this->cxz().dilate( this->pos(), .2 ).trs(-.2,0,0);
  }

//  virtual void draw(GFXSceneNode * re){
//    render::pipe(midsectionR, re);
//    render::pipe(midsectionL, re);
//    render::pipe(this->cxz(), re);
//  }


  virtual void onStep(float dt){
    //tk.HF.cir() = this->cxz();
    //auto par = tk.par() * .01;
    auto cir = round::produce( round::dls(1.0,0,0,0), this->xz() );
    auto shrink = cir.dilate( PAO, .5 );
    midsectionR = shrink.boost( cir.dual() * time  );
   // midsectionL = midsectionL.boost( par );

    makeMeshData();

  }

  void makeMeshData(){
    //pnt.clear();

    vector<Point> base;
    for (int i =0; i<NUM_VERTEX_BASE; ++i){
      float t = TWOPI * (float)i/NUM_VERTEX_BASE;
      base.push_back( point(midsectionR, t) );
    }
    
    auto vp = pg( base );
   // cout << vp.size() << endl;
   // cout << NUM_VERTEX_PER_AGENT << endl;
    for (int i =0; i<NUM_VERTEX_PER_AGENT; ++i){
      pnt[i] = vp[i] + this->pos(); //<-- could optimize as memcopy
    }
  } 
  

};  


#endif   /* ----- #ifndef swarm_INC  ----- */

