/*
 * =====================================================================================
 *
 *       Filename:  xSimplex.cpp
 *
 *    Description:  grid of simplex
 *
 *        Version:  1.0
 *        Created:  06/03/2014 16:54:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */
#include "vsr/vsr_simplex.h"
#include "vsr/vsr_ncube.h"
#include "vsr/vsr_cga3D_frame.h"
#include "vsr/vsr_stat.h"
#include "vsr/vsr_cga3D_op.h"
#include "vsr/vsr_twist.h"

#include "ctl_app.h"

using namespace std;
using namespace ctl; 
using namespace gfx;
using namespace vsr;


template<int N>
struct SimplexFrame : public Frame {

  MBO * mbo;

  Simplex<N> simplex;

  SimplexFrame() : Frame() { init(); }

  void init(){
    Mesh mesh;
    for (auto& i : simplex.verts ){
      Vertex v(i[0],i[1],i[2]);
      v.Norm = Vec3f(i[0],i[1],i[2]);
      v.Col = Vec4f(0,.5,.5,.8);
      mesh.add(v);
    }

    for (auto& i : simplex.edges ){
      mesh.add( i.a).add(i.b);//.add(i.c);
    }

    mesh.mode( GL::LS );
    mbo = new MBO( mesh, GL::DYNAMIC );
  }
  
  void update(){
   // Vec v = dx();
    
    for (int i=0; i< simplex.verts.size(); ++i){
     auto& v = simplex.verts[i];
     mbo -> mesh[i].Pos = Vec3f(v[0], v[1],v[2]);
    }
    mbo -> update();
  }

  void begin( Renderer * r){
    r -> pipe.begin(*mbo);
  }

  void draw(float dist, Renderer * r){
      update();
      r -> modelview( vsr::Xf::mat( rot(), pos(), scale() ) );
      r -> pipe.draw(*mbo);
  }

  void end( Renderer * r){
    r -> pipe.end(*mbo);
  }

};



template<int N>
struct NCubeFrame : public Frame {

  Mesh mesh;

  NCube<N> cube;

  NCubeFrame() : Frame() { init(); }

  void init() {
    for (auto& i : cube.faces ){
        
    }
  }

};


struct MyApp : public App {
        
  //THIS IS THE APP SPECIFIC STUFF, 
  MBO * simplex;  

  static const int num = 5;
  float time;
  
  //INSTANTIATE THE APPLICATION WITH WIDTH AND HEIGHT
  MyApp() : App( Layout(4,4) ) { 
    init();
  }                                  
  
  //INIITIALIZE ELEMENTS OF THE SCENE 
  virtual void init(){
    Mesh sim;

    simplex = new MBO( sim );
    time = 0;

  }  
  
  //UPDATE() CHANGES THE POSITION OF THE CIRCLE
  virtual void update(){
     
    time +=.01;
    
   
  }

  //THIS DRAWS THE CIRCLE TO THE SCREEN
  virtual void onDraw(){ 
    static const int N = 4;
    glLineWidth(2);

   // time++;
    
    static SimplexFrame<N> sim[num*num];

    sim[0].begin(this);
   
    Rand::Seed();
    for (int i = 0;i<num;++i){
      for (int j = 0;j<num;++j){
        float u = (float)i/num * layout.screenWidth;
        float v = (float)j/num * layout.screenHeight;
        int idx = i*num+j;
        sim[idx].pos() = Ro::null( cos(time) * u,sin(time)*v,0);//.spin( Gen::mot( Twist::Along(DLN(0,1,0), ) ) );
        sim[idx].simplex.spin( Rand::Int(N), .1);
        sim[idx].draw(1.0, this);
      }
    }

    sim[0].end(this);

  } 
  
}; 
        


bool running = true;
void quit(int) {
  running = false;
}

int main(){
  
  MyApp app; 
  
  while(running){
    app.onFrame();
    usleep(166);
  }   
  
    return 0; 
}



