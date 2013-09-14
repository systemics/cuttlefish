/*
An App class for the raspberry pi built on the cuttlefish framework

pablo colapinto and karl yerkes hell yeah!

*/    

#ifndef CF_APP_H_INCLUDED
#define CF_APP_H_INCLUDED

#ifndef __raspberry__
#define __raspberry__
#endif

#include "ctl_bcm.h"
#include "ctl_timer.h"
#include "ctl_host.h"

#include "gfx/gfx_scene.h" 
#include "gfx/gfx_pipe.h"
#include "gfx/gfx_gl.h"  

#include "ctl_egl.h"
#include "ctl_osc.h"
 
using namespace gfx;

namespace ctl {
	
	using namespace EGL;
    using namespace GLSL; 

	struct App :  BCM, Host, Window, OSCPacketHandler { 
	        
		Scene scene;   		///<-- Transformation Matrices   	
		Pipe pipe;	   		///<-- Graphics Pipeline    
		 
		Vec4f background;
		
		Pose viewpose;
		float width, height; ///<-- Width, Height of each screen in inches  
		
		Mat4f mvm; 			 //transformation matrix of scene
		
		
	     App (float w, float h, float z=30.0) : Window(), background(0,0,0,1), OSCPacketHandler() 
		{        		
			initView(w,h,z);
            initGL();

	    addListener(GetTouch, "/touch", "iiii", this);  

        }

      static int GetTouch( const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) { 
        App& app = *(App*)user_data;
        app.onTouch(argv[0]->i, argv[1]->i, argv[2]->i, argv[3]->i);
      }

      virtual void onTouch(int n, int x, int y, int t) {
        cout << n << endl;
      }

	    ~App(){}     
	 
		void initGL(){
             
			glClearColor(1,1,1,1);
	        srand( time(NULL) );  

		    string Vert = AVertex + Varying + UMatrix  + NTransform + VLighting + VCalc + MVert;
			string Frag = MFrag;//TFragMix; //mFrag
            
			pipe.program = new ShaderProgram(Vert,Frag,0);
			pipe.bindAll();		
			
		}
	     
		virtual void init() = 0; 
		virtual void update() = 0;
		virtual void onDraw() = 0;   
		
		//calculation view from row, col and width height
		Pose calcView(int row, int col, int trow, int tcol, float w, float h){
			
		}
		      
		void initView(float w, float h, float z){

			width =  w;
			height = h;     

  		    float aspect = width / height;   
             
			scene.fit(w,h);

			int numscreens = 4;

			Vec3f viewer(0,0,z);  


			Pose p(-width/2.0,-height/2.0, 0);

			switch( identifier ){

				case 2:
					p = Pose( - width * 2, - height / 2.0, 0 );
					break;
				case 3:
					p = Pose( - width , - height / 2.0, 0 );
					break;
				case 4:
					p = Pose( 0, - height / 2.0, 0 );
					break;
				case 5:
					p = Pose( width, - height / 2.0, 0 );
					break;

		        default:  
					cout << "USING DEFAULT SCREEN VIEW POSE" << endl;
		          break;  
			}

			scene.camera.view = View( viewer, p, aspect, height );
			scene.camera.pos() = Vec3f( 0, 0, z);
       }  
   

	
		virtual void onFrame(){
   	  
			 glViewport(0,0,surface.width,surface.height); 
	         glClearColor(background[0],background[1],background[2],background[3]);
	         glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
             
             scene.onFrame();
              
             update();

		   	 mvm = scene.xf.modelViewMatrixf();
	         pipe.bind( scene.xf );

	         	onDraw();
			 
		     pipe.unbind();
			 swapBuffers();
			
		}  
		
		virtual void onTimer(){   
			cout << "timer func\n";
	      onFrame();
	    }
	  
	}; 
	
	#define STARTANDRUN()  \
	bool running = true; \
	void quit(int) {     \
	  running = false;   \
	}                    \
                         \
	int main(){          \
                         \
		MyApp app;       \
                         \
		while(running){  \
		   app.onFrame();\
		   usleep(166);  \
		}                \
                         \
		  return 0;      \
	}
	
} 

#endif
