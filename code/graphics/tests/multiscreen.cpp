//
//  MULTISCREEN.cpp
//  Versor on the Raspberry PI
//
//  Created by Pablo Colapinto on 1/24/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

// #include "vsr.h"
// #include "vsr_op.h"
// #include "vsr_xf.h"

#include "ctl_egl.h"
#include "ctl_gl.h"

#include "ctl_glsl.h"
#include "ctl_mdraw.h"
#include "ctl_scene.h"
#include "ctl_screen.h"

//#include "ctl_render.h"
//#include "vsr_field.h"

#include <unistd.h>
#include <string>
#include <iostream>
#include <map>

using namespace std;

using namespace ctl;
using namespace ctl::GL;
using namespace ctl::EGL;
using namespace ctl::GLSL;

int identifier;

struct MyWindow : public Window {

	 	Scene scene;
		ShaderProgram program;
		
		Pose viewpose;
		float width, height, aspect, scale; //Total Width, Height of all Screens Combined
		
        MyWindow() : Window () {
            initGL();
        }
        
        ~MyWindow(){}
        
        void initGL(){
	
            glClearColor(1,1,1,1);
            srand( time(NULL) );

			// BUILD AND BIND ATTRIBUTES OF DEFAULT SHADER
			string Vert = AVertex + Varying + UMatrix  + NTransform + VLighting + VCalc + MVert;
			string Frag = USampler + Varying + MFrag;

	        program.source(Vert,Frag);
			program.bind();
	             Pipe::BindAttributes();
	        program.unbind();
			
			initView();
		}	
		
		void initView(){
	
			//use 16 GL units x 9 GL units
			width =  4.;
			height = 3.;
			
			aspect = width / height; 
			int numscreens = 4;
			
			Vec3f viewer(0,0,3);
			
			//GET SCREEN ID
			int screenID = 0;//??
	
			Pose p;
			
			switch( identifier ){
				
				case 1:
					p = Pose( - width * 2, - height / 2.0, 0 );
					break;
				case 2:
					p = Pose( - width , - height / 2.0, 0 );
					break;
				case 3:
					p = Pose( 0, - height / 2.0, 0 );
					break;
				case 4:
					p = Pose( width, - height / 2.0, 0 );
					break;
				
        default:
          break;
			}
			
//			scene.camera.view() = View( viewer, p, aspect, height );
			// scene.camera.view() = View( viewer, Pose(-32.,-4.5, 0), aspect* 4, height );
			scene.camera.view() = View( viewer, Pose(-width/2.0,-height/2.0, 0), aspect, height );
			
 
       }
        
        virtual void onDraw(){
				
			static MBO circle ( Mesh::Disc(.5) );
			static MBO grid ( Mesh::Grid(width, height, 1) );
			
		    program.bind();
		
		         //program.uniform("projection",  scene.xf.proj);
		         program.uniform("lightPosition", 2.0, 2.0, 2.0);

		         program.uniform("normalMatrix", scene.xf.normal);
				 program.uniform("modelView", scene.xf.modelView );//app.scene().xf.modelView);        
				
			  	GL::Pipe::Line( grid );   

		    program.unbind();

        }
        
        virtual void onFrame(){
			
			scene.onFrame();
	
            glClearColor(0,0,0,1);
            // clear screen
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
           
            onDraw();
 			// to update we need to swap the buffers
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

  map<string, int> idOf;

  int n = 1;
  idOf["pi-b"] = n++;
  idOf["pi-c"] = n++;
  idOf["pi-l"] = n++;
  idOf["pi-r"] = n++;

  char hostname[100];
  gethostname(hostname, 100);
  identifier = idOf[hostname];

  MyWindow win;

  while (running) {
    win.onFrame();
    usleep(16666);
  }

  bcm_host_deinit();
  return 0;
}
