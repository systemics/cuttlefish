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

#include "ctl_bcm.h"
#include "ctl_timer.h"  

#include <lo/lo.h>   

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

struct MyWindow : BCM, Timer, public Window {

	 	Scene scene;
		ShaderProgram program;
		Pipe pipe;
		
		Pose viewpose;
		float width, height, aspect, scale; //Total Width, Height of all Screens Combined
		 
		MBO * circle; 
		MBO * grid;
		MBO * line;
		
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
	             pipe.bindAttributes();
	        program.unbind();
			
			initView();  
			initBufferObjects(); 
			//program.bind();
		}	
		
		void initBufferObjects(){
			circle 	= new MBO ( Mesh::Disc(4).color(1,1,0), GL::DYNAMIC  );
			line 	= new MBO ( Mesh::Rect( width *4 , 1.0 ).color(0,1,1), GL::DYNAMIC );
			grid 	= new MBO ( Mesh::Grid( width * 4, height, 1 ) );    
		}
		
		void initView(){
	         

			//use 16 GL units x 9 GL units   
			//pass in width and height in inches
			width =  14. + 3./8.;//21.5;
			height = 10. + 11./16.;//12.5;     

  			aspect = width / height;   
  		                     

			int numscreens = 4;
			
			Vec3f viewer(0,0,1);  //Position in gl units
			
			//GET SCREEN ID
			int screenID = -1;//0;//??
	
			Pose p(-width/2.0,-height/2.0, 0);
			
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
			
			scene.camera.view() = View( viewer, p, aspect, height );
			
       }
        
        virtual void onDraw(){
				             
			                  
			static float time = 0; time +=.03;   
			float x = sin(time);    
			
			
			circle -> mesh.moveTo( x * width * 2.0, 0, 0 );   
			circle  -> update();
			
			line  -> mesh.moveTo( 0, x * height/2.0, 0 );       
			line  -> update();
			
		    program.bind();
		
		         // program.uniform("lightPosition", 2.0, 2.0, 2.0);
		         program.uniform("projection",  scene.xf.proj);

		         program.uniform("normalMatrix", scene.xf.normal);
				 program.uniform("modelView", scene.xf.modelView );//app.scene().xf.modelView);        
				
			  	 pipe.line( *grid );  
				 pipe.line( *circle );
                 pipe.line( *line ); 
		   program.unbind();

        }
        
    virtual void onTimer(){   
		cout << "timer func\n";
      onFrame();
    }
        virtual void onFrame(){
			
			scene.onFrame();
	
            glClearColor(0,0,0,1);
            // clear screen
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
             
            onDraw();
			swapBuffers(); 
 			// to update we need to swap the buffers
 			
        }

};

bool running = true;
void quit(int) {
  running = false;
}                                                                                        

int onMulti1(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) { 
 
	cout << path << types << endl; 
	float x = argv[0] -> f;
	float y = argv[1] -> f;
	
	cout << x << " " << y << endl; 
	
	MyWindow& w = *((MyWindow*)user_data); 
	Vec3f tv = w.scene.camera.pos();
	tv[0] = - w.width * 2.0 + x * w.width * 4.0;  
	w.scene.camera.pos() = tv;
}


int main() {

  map<string, int> idOf;

  int n = 1;
  idOf["b8:27:eb:84:b7:37"] = n++;
  idOf["b8:27:eb:a8:87:0b"] = n++;
  idOf["b8:27:eb:7d:63:d7"] = n++;
  idOf["b8:27:eb:a6:9f:1a"] = n++;

  ifstream foo("/sys/class/net/eth0/address");
  char mac[256];
  foo.getline(mac, 256);
  identifier = idOf[mac];

  MyWindow win;    

  //win.start(1 / 15.0);
  while (running) {
    win.onFrame();
    usleep(16666);
    //usleep(16666);
  }

  return 0;
}
