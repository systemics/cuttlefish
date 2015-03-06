/*
 * =====================================================================================
 *
 *       Filename:  xHyperShader.cpp
 *
 *    Description:  hyper shader??
 *
 *        Version:  1.0
 *        Created:  06/03/2014 20:08:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ctl_app.h"
#include "vsr/vsr_simplex.h"
#include "vsr/vsr_ncube.h"
#include "gfx/gfx_process.h"
#include "temp/hypershader.h"


//#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 
using namespace vsr;



struct MyProcess : Process {

  //  R2T r2t;
  //  Blur blur;
    float amt;//,xpos,ypos;
    
    MyProcess(int w, int h, Renderer *r ) : 
    Process(w,h,r)//, r2t(w,h,r), blur(w,h)
    { init(); }

    virtual void init(){
      initShader();
      amt;// = xpos = ypos = 0;
    }
    
   void initShader(){
    this->program = new ShaderProgram( hyper, DefaultFrag, 0 );
    this->bindAll(); 
   }

  virtual void update(){
     amt += .01;
     this->program->uniform("amt",amt);
  }

   virtual void operator()(){

    
    this->bind();
        update();
        this->bind( renderer -> scene.xf );
        renderer -> onDraw();

    this->unbind();   
  }

};


struct MyApp : public App {
        
  //THIS IS THE APP SPECIFIC STUFF, 
  //
  MBO *mbo;

  int nwidth = 40;
  int nheight = 40;

  MyProcess * process;

  NCube<4> tetra;
  Simplex<4> simplex;

  //INSTANTIATE THE APPLICATION WITH WIDTH AND HEIGHT
  MyApp() : App( 30,20 ) { 
    init();
  }                                  
  
  //INIITIALIZE ELEMENTS OF THE SCENE 
  virtual void init(){
    
    /* Mesh mesh; */
    Mesh mesh;
    Mesh meshT;

    for (int j=0; j<=nwidth; ++j){
      for (int k=0;k<=nheight;++k){
        
        float su = (float)j/nwidth;
        float sv = (float)k/nheight;
        int idx = (j * nheight + k);

      //SIMPLEX
       for (auto& i : simplex.verts ){
        Vertex v(-layout.screenWidth/2.0 + su * layout.screenWidth, -layout.screenHeight/2.0 + sv*layout.screenHeight,0);
        v.Norm = Vec3f(i[0],i[1],i[2]);
        v.Col = Vec4f(i[0],i[1],i[2],i[3]);
        v.Tex = Vec2f(su,sv);
        mesh.add(v);
       }

        int sidx = idx * simplex.verts.size();
       for (auto& i : simplex.edges ){
        mesh.add(sidx + i.a).add(sidx + i.b);//.add(i.c);
       }

       //TET
       int tidx = idx * tetra.roots.size();
       for (auto& i : tetra.roots){
         Vertex v(-layout.screenWidth/2.0 + su * layout.screenWidth, -layout.screenHeight/2.0 + sv*layout.screenHeight,0);
         v.Norm = Vec3f(i[0],i[1],i[2]);
         v.Col = Vec4f(i[0],i[1],i[2],i[3]) * .5;
         v.Tex = Vec2f(su,sv);
         meshT.add(v);
        }
       
        for (auto& i : tetra.edges ){
         meshT.add(tidx + i.a).add(tidx+i.b);
        }
      }
    }

    mesh.mode( GL::LS );
    mbo = new MBO( mesh );

    process = new MyProcess(layout.screenWidth, layout.screenHeight, this);

  }  
  
  virtual void update(){
     
    static float time = 0; time +=.01;
   
  }

  virtual void onDraw(){  
      glLineWidth(1);
      pipe.line( *mbo );
  } 

  virtual void onFrame(){
   
     update(); 
     Renderer::clear();

     scene.updateMatrices();
    
     (*process)();
     //Renderer::render(); 
    
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

