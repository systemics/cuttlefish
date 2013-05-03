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

#include <lo/lo.h>   

//#include "ctl_render.h"
//#include "vsr_field.h"

#include <iostream>

using namespace std;

using namespace ctl;
using namespace ctl::GL;
using namespace ctl::EGL;
using namespace ctl::GLSL;


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
			//pass in width and height in inches
			width =  14. + 3./8.;//21.5;
			height = 10. + 11./16.;//12.5;     

  			aspect = width / height;   
  		                      
			//Set glUnit to half Height
		   // float glUnit = height / 2.0;

			int numscreens = 4;
			
			Vec3f viewer(0,0,1);  //Position in gl units
			
			//GET SCREEN ID
			int screenID = -1;//0;//??
	
			Pose p(-width/2.0,-height/2.0, 0);
			
			switch( screenID ){
				
				case A1:
					p = Pose( - width * 2, - height / 2.0, 0 );
					break;
				case A2:
					p = Pose( - width , - height / 2.0, 0 );
					break;
				case A3:
					p = Pose( 0, - height / 2.0, 0 );
					break;
				case A4:
					p = Pose( width, - height / 2.0, 0 );
					break;
				
			}
			
//			scene.camera.view() = View( viewer, p, aspect, height );
			// scene.camera.view() = View( viewer, Pose(-32.,-4.5, 0), aspect* 4, height );
			scene.camera.view() = View( viewer, p, aspect, height );
			
       }
        
        virtual void onDraw(){
				             
			// scene.camera.pos() = Vec3f(0,0,1);     
				
			static MBO circle ( Mesh::Disc(4).color(1,1,0), GL::DYNAMIC  );
			static MBO grid ( Mesh::Grid(width * 4, height, 1) );
		   //  static MBO line ( Mesh::Line( Vec3f( - width * 2, -height /2.0, 0), Vec3f( width * 2, -height /2.0, 0) ) );   
			static MBO line ( Mesh::Rect( width *4 , 1.0 ).color(0,1,1), GL::DYNAMIC );
			                  
			static float time = 0; time +=.03;   
			float x = sin(time);    
			
			
			circle.mesh.moveTo( x * width * 2.0, 0, 0 );   
			circle.update();
			
			line.mesh.moveTo( 0, x * height/2.0, 0 );       
			line.update();
			
		    program.bind();
		
		         program.uniform("projection",  scene.xf.proj);
		         program.uniform("lightPosition", 2.0, 2.0, 2.0);

		         program.uniform("normalMatrix", scene.xf.normal);
				 program.uniform("modelView", scene.xf.modelView );//app.scene().xf.modelView);        
				
			  	GL::Pipe::Line( grid );  
				GL::Pipe::Line( circle );
                  GL::Pipe::Line( line ); 
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

int onMessage(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) { 
	cout << path << endl; 
}

int main() {
  bcm_host_init();
  signal(SIGABRT, quit);
  signal(SIGTERM, quit);
  signal(SIGINT, quit);

  MyWindow win;    

  lo_server_thread st = lo_server_thread_new("7770", 0);
  lo_server_thread_add_method(st, "/rectangle", "ffffffff", onMessage, 0);
  lo_server_thread_start(st);

  while (running) {
    win.onFrame();
    usleep(16666);
  }

  bcm_host_deinit();
  return 0;
}