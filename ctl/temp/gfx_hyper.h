
/*
 * =====================================================================================
 *
 *       Filename:  gfx_displacement.h
 *
 *    Description: graphics displacement process 
 *
 *        Version:  1.0
 *        Created:  06/03/2014 16:18:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "gfx/gfx_mbo.h"
#include "gfx/gfx_process.h"
#include "temp/hypershader.h" //similar to one below . . .
#include "temp/hyperAmt_glsl.h"
#include "vsr/vsr_simplex.h"

namespace gfx {

struct HyperSimplex : public Process {

    MBO * mbo;

    float amt;

     vsr::Simplex<4> simplex;
    //pass in num points
    HyperSimplex(int w, int h, Renderer * r) : 
    Process(w,h,r) { init(); }

    virtual void init(){

      Renderer& r = *renderer;
      
      initShader();
    
      /* Mesh mesh; */
      Mesh mesh;
        
      //SIMPLEX
       int iter =0;
       for (auto& i : simplex.verts ){
         float t = (float)iter/simplex.verts.size();
        Vertex v(i[0],i[1],i[2]);
       // Vertex v(0,0,0);
        v.Norm = Vec3f(i[0],i[1],i[2]);
        //v.Col = Vec4f(i[0],i[1],i[2],i[3]);
        v.Col = Vec4f(t, t*t,1-t,1);
        mesh.add(v);
        iter ++;
       }

       for (auto& i : simplex.edges ){
        mesh.add(i.a).add(i.b);//.add(i.c);
       }
    
     mesh.mode( GL::LS );
     mesh.store();
     mbo = new MBO( mesh );

   }

   virtual void initShader(){
    this->program = new ShaderProgram( DefaultVertES, DefaultFragES, 0);//hyperAmt, DefaultFragES, 0);
    this->bindAll();
   } 

   virtual void update(){
     this->program->uniform("amt",amt);
   }

  virtual void operator()(){
   // this->bind();
    this->bind( renderer -> scene.xf ); //binds
    //    update();
        draw();
    this->unbind();   
  }

  virtual void draw(){
    this->line(*mbo);
  }

};  

struct Hyper : public Process {

    MBO * mbo;

    float spacing, amt;

    vsr::Simplex<4> simplex;

    Hyper(int w, int h, float s, Renderer * r) : 
    Process(w,h,r), spacing(s) { init(); }

    virtual void init(){

      Renderer& r = *renderer;
      
      initShader();
    
      /* Mesh mesh; */
      Mesh mesh;
      Mesh meshT;

     for (int j=0; j<=width; ++j){
      for (int k=0;k<=height;++k){
        
        float su = (float)j/width;
        float sv = (float)k/height;
        int idx = (j * height + k);

      //SIMPLEX
       for (auto& i : simplex.verts ){
        Vertex v(-r.layout.screenWidth/2.0 + su * r.layout.screenWidth, -r.layout.screenHeight/2.0 + sv*r.layout.screenHeight,0);
        v.Norm = Vec3f(i[0],i[1],i[2]);
        v.Col = Vec4f(i[0],i[1],i[2],i[3]);
        v.Tex = Vec2f(su,sv);
        mesh.add(v);
       }

       int sidx = idx * simplex.verts.size();
       for (auto& i : simplex.edges ){
        mesh.add(sidx + i.a).add(sidx + i.b);//.add(i.c);
       }
      }
    }
    
     mesh.mode( GL::LS );
     mesh.store();
     mesh.moveTo( r.scene.camera.view.topleft + Vec3f(r.layout.screenWidth/2.0, -r.layout.screenHeight/2.0, 0 ) );
     mbo = new MBO( mesh );

   }

   virtual void initShader(){
    this->program = new ShaderProgram( hyper, TFragES, 0);
    this->bindAll();
   } 

   virtual void update(){
      this->program->uniform("amt",amt);
   }

  virtual void operator()(){
    this->bind();
        this->bind( renderer -> scene.xf );
        update();
        draw();
    this->unbind();   
  }

  virtual void draw(){
    this->line(*mbo);
  }

};

//     


struct HyperProcess : public Process {

  R2T r2t;
  RP2T rp2t;

  Slab slab;
  Blur blur;

  Hyper hypmap;

  HyperProcess(int w, int h, Renderer * r) : 
  Process(w,h,r), r2t(w,h,r), rp2t(w,h), hypmap(50,50,1.0,r), 
  blur(w,h), slab(w,h)
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
    //rp2t();

    //switch to original viewport
    glViewport(0, 0, renderer -> contextWidth, renderer -> contextHeight ); 
    //renderer -> clear();

    //slab();

    blur();

     /* slab.texture->bind(); */
     /*    glLineWidth(1); */
     /*    hypmap(); */
     /* slab.texture->unbind(); */

    r2t.swap();   // Swap textures
  //  rp2t.swap();  
  }

};

} //gfx::



