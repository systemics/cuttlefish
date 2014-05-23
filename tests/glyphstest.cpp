//
//  circle.cpp
//  Versor on the Raspberry PI
//
//  Created by Pablo Colapinto on 1/24/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "vsr.h"
#include "vsr_op.h"
#include "vsr_xf.h"

#include "ctl_egl.h"
#include "ctl_gl.h"

#include "ctl_glsl.h"
#include "ctl_mdraw.h"
#include "ctl_scene.h"

#include "ctl_render.h"

#include "vsr_field.h"

#include <iostream>

using namespace std;

using namespace ctl;
using namespace ctl::GL;
using namespace ctl::EGL;
using namespace ctl::GLSL;


struct MyWindow : public Window {
	
	 	Scene scene;
		ShaderProgram program;
		
        MyWindow() : Window () {
            initGL();
        }
        
        ~MyWindow(){}
        
        void initGL(){
             glClearColor(1,1,1,1);
             srand( time(NULL) );

			string Vert = AVertex + Varying + UMatrix  + NTransform + VLighting + VCalc + MVert;
			string Frag = USampler + Varying + MFrag;

	        program.source(Vert,Frag);
			program.bind();
	             Pipe::BindAttributes();
	        program.unbind();
 
       }
        
        virtual void onDraw(){
	
			using namespace vsr;
			static double time = 0.0; time += .01;
			
			static Circle c = CXY(1).trs(0,0,-3);
			static Point p = PT(0,0,0);
			static Field<Pnt> f(30,30,1,.1);
			static DualSphere dls = Ro::dls(p, .3);
						
			Circle tc = c.sp( Gen::mot( DLN(1,0,0).trs(0,0,-3) * time ) );		
			Vector tv = Vec(1,0,0).sp( Gen::rot( Biv::xy * time ) );
									
		    program.bind();
		
		         program.uniform("projection",  scene.xf.proj );
		         program.uniform("normalMatrix", scene.xf.normal);
				 program.uniform("modelView", scene.xf.modelView );    
	 			 program.uniform("lightPosition", 2.0, 2.0, 2.0 );
				
				 Render( f, scene.xf.modelViewMatrixf(), program );

		    program.unbind();

        }
        
        virtual void onFrame(){
			
			scene.onFrame();
	
            glClearColor(0,0,0,1);
            // clear screen
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // to update we need to swap the buffers
            onDraw();
 			swapBuffers();
        }

};

bool running = true;
void quit(int) {
  running = false;
}

int main() {
  bcm_host_init();
  signal(SIGABRT, quit);
  signal(SIGTERM, quit);
  signal(SIGINT, quit);

  MyWindow win;

  while (running) {
    win.onFrame();
    usleep(16666);
  }

  bcm_host_deinit();
  return 0;
}