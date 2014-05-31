/*
 * =====================================================================================
 *
 *       Filename:  xDisplacementMap.cpp
 *
 *    Description:  render to a texture then use texture to displace vertices
 *
 *        Version:  1.0
 *        Created:  05/28/2014 17:14:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ctl_app.h"
#include "gfx/gfx_mbo.h"
#include "gfx/gfx_process.h"

/* #include "vsr/vsr_cga3D_op.h" */
/* #include "vsr/vsr_render.h" */

using namespace std;
using namespace ctl; 
using namespace gfx;
/* using namespace vsr; */
/* using namespace vsr::cga3D; */

struct Displacement : public Process {

    MBO * grid;

    float spacing, amt;

    //pass in num points
    Displacement(int w, int h, float s, Renderer * r) : 
    Process(w,h,r), spacing(s) { init(); }

    virtual void init(){
     Renderer& r = *renderer;
     initShader();
     grid = new MBO( Mesh::HexaGrid(width,height,spacing).moveTo( 
            r.scene.camera.view.topleft + Vec3f(r.layout.screenWidth/2.0, -r.layout.screenHeight/2.0,0 )) );

     Vec3f bl = r.scene.camera.view.bottomleft;
     float offw = ( bl[0] + r.layout.totalWidth()/2.0) / r.layout.totalWidth();
     float offh = ( bl[1] + r.layout.totalHeight()/2.0) / r.layout.totalHeight();

      for (int i = 0; i<grid->mesh.num(); ++i){
          Vec3f vp = grid -> mesh[i].Pos;
          float uv = (vp[0] - bl[0])/r.layout.screenWidth;//(float)i/width;
          float uw = (vp[1] - bl[1])/r.layout.screenHeight;//offh;//(vp[1] - tl[1]-r.layout.screenHeight)/r.layout.screenHeight;//(float)j/height;

          grid -> mesh[i].Col = Vec4f(uv*offw,uw*offh,1-uv*offw,1);//Vec4f(uv * offw, uw * offh, 1-(uv*offw), 1);
      }
     

     grid -> mode = GL::LS;
     grid -> update();
     amt = 5;
   }

   virtual void initShader(){
    this->program = new ShaderProgram( DisplacementVertES, DefaultFragES, 0);
    this->bindAll();
   } 

   virtual void update(){
      this->program->uniform("amt",amt);
   }


  virtual void operator()(){
      this->bind();
       this->bind( renderer -> scene.xf );  
       update();     
       this->line(*grid); 
      this->unbind();    
  }

  virtual void draw(){
    this->line(*grid);
  }

};


struct MyProcess : public Process {

  R2T r2t;
  RP2T rp2t;

  Slab slab;
  Blur blur;

  Displacement dispmap;
  bool bDrawDispMap;

  MyProcess(int w, int h, Renderer * r) : 
  Process(w,h,r), r2t(w,h,r), rp2t(w,h), dispmap(215,145,.1,r), 
  blur(w,h), slab(w,h), bDrawDispMap(false)
  { init(); } 

  virtual void init(){

    blur.initRect();                // Initialze a rectangle
    blur.texture = r2t.textureA;

    slab.rect = blur.rect;          // Share resources
    slab.texture = rp2t.textureA;  

    rp2t.process = &blur;


  }


  void operator()(){



    // Render to Offscreen Texture!
    r2t();        

    // Now Blur it and Capture that!
    rp2t();

    //switch to original viewport
    glViewport(0, 0, renderer -> contextWidth, renderer -> contextHeight ); 
    //renderer -> clear();

   // slab();

    //blur();

     slab.texture->bind();
        glLineWidth(5);
        dispmap();
     slab.texture->unbind();

    r2t.swap();   // Swap textures
    rp2t.swap();  
  }

};



struct MyApp : public App {

  float time = 0;

  MBO * mbo;
  
  MyProcess * process;

  MyApp() : App( Layout(4,4) ) {init();}// App(21,14) { init(); }

  void init(){
    mbo = new MBO( Mesh::Circle( 20 ) );
    process = new MyProcess( surface.width/2, surface.height/2, this );

    glLineWidth(1);
  }

  virtual void update(){
    time += .1;
    process -> blur.ux = sin(time) * .02;
    process -> blur.uy = cos(time) * .02;
    process -> blur.amt = fabs(sin(time)) * .5;
    process -> dispmap.amt = sin(time*.2) * .5;

    // mbo -> mesh.scaleA( sin(time*.1) );
    //mbo -> mesh.moveTo( sin(time) * layout.screenWidth/2.0, 0, 0 );   
    // mbo -> update();
  }
  
  virtual void onDraw(){
    glLineWidth(10);
    pipe.line(*mbo);  
  }

  virtual void onFrame() { 
     update();          
     Renderer::clear();
    
     (*process)();
     //Renderer::render(); 
    
     Window::swapBuffers(); 
  }    
};

STARTANDRUN()
