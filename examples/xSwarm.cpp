/*
 * =====================================================================================
 *
 *       Filename:  xSwarm.cpp
 *
 *    Description:  boid-like swarm behavior
 *
 *        Version:  1.0
 *        Created:  06/04/2014 12:14:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "ctl_app.h"
#include "vsr/vsr_cga3D_frame.h"
#include "vsr/vsr_stat.h"
#include "vsr/vsr_simplex.h"
#include "temp/hyperAmt_glsl.h"
#include "gfx/gfx_process.h"
#include "temp/gfx_hyper.h"

using namespace std;
using namespace ctl; 
using namespace vsr;


struct MyApp : public App {
        
  //THIS IS THE APP SPECIFIC STUFF, 
  //
  static const int numAgents = 20;

  float time = 0;

  Simplex<4> simplex;
  HyperSimplex * process;

  vector<Frame> frame;
  MBO *mbo;
  MBO *simplexMBO;
  
  //INSTANTIATE THE APPLICATION WITH WIDTH AND HEIGHT
  MyApp() : App( 21.5,14.5,50 ) { 
    init();
  }                                  
  
  //INIITIALIZE ELEMENTS OF THE SCENE 
  virtual void init(){

    frame = vector<Frame>(numAgents);

    int spacing = 4;

    Rand::Seed();
    for (auto& f : frame ){
      Vec v( Rand::Num(), Rand::Num(), Rand::Num());
      f.pos() = Ro::null(v * spacing );
      f.rot() = Gen::rot( Biv(  Rand::Num(), Rand::Num(), Rand::Num() ) );
      f.scale() = .5;
    }

    mbo = new MBO ( Mesh::Frame(.5) );

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
  
  //UPDATE() CHANGES THE DATA
   virtual void update(){
      time += .01;

      float acc = .02;
      float rotAcc = .02; 

      int numNeighbors = 3;
      for (auto& fa : frame){

        vector<Frame> nearest;
        vector<Frame> toonear;
        float thresh = 1.5;
        float min = .75;
        for (auto& fb : frame){
          float halfplane = (fb.pos() <= fa.dxy())[0];
          if ( halfplane > 0 ){
            float dist = Ro::dist( fa.bound(), fb.bound() );
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

         fa.db() = db * rotAcc;
         fa.dx() = fa.z() * acc;//dx * acc;

         if (nearest.empty()){
           fa.db() = fa.xz() * .01;
         }
        }

        fa.move(); fa.spin();
      }     
  }

  //THIS DRAWS
  virtual void onDraw(){ 

    update();
    
    pipe.begin( *simplexMBO );

      for (auto& f : frame){
        process -> bindModelView( mvm * vsr::Xf::mat(f.rot(), f.vec(), f.scale()) );
        process -> program -> uniform("amt", time );
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

    Window::swapBuffers();
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

