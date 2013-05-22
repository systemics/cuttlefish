/*
An App class for the raspberry pi built on the cuttlebone framework

pablo colapinto and karl yerkes

*/    

#ifndef CTL_APP_H_INCLUDED
#define CTL_APP_H_INCLUDED

#include "ctl_egl.h"
#include "ctl_gl.h"
#include "ctl_bcm.h"
#include "ctl_timer.h"

#include "ctl_glsl.h"
#include "ctl_pipe.h"
#include "ctl_scene.h"  
#include "ctl_host.h"
 


namespace ctl {
	
	using namespace EGL;
    using namespace GLSL; 

	struct App :  BCM, Host, Window { 
	        
		Scene scene;   		///<-- Transformation Matrices
		
		Pipe pipe;	   		///<-- Graphics Pipeline    
		
		Pose viewpose;
		float width, height; ///<-- Width, Height of each screen in inches
	
	     App (float w, float h) : Window()
		{        		
			initView(w,h);
            initGL();
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
		virtual void onDraw() = 0;
		      
		void initView(float w, float h, float z = 20.0){

			width =  w;
			height = h;     

  		    float aspect = width / height;   


			int numscreens = 4;

			vsr::Vec3f viewer(0,0,z);  //Position in inches


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

			scene.camera.view() = View( viewer, p, aspect, height );
			scene.camera.pos() = Vec3f( 0, 0, z);
       }  

		// void viewerAt( float x, float y, float z){ 
		// 	
		// 	width =  w;
		// 	height = h;     
		// 
		//   		    float aspect = width / height;
		// 
		// 	 vsr::Vec3f viewer(0,0,50);  //Position in inches
		// 	 
		// 	Pose p(-width/2.0,-height/2.0, 0);  
		// 	
		// 	switch( identifier ){
		// 
		// 		case 2:
		// 			p = Pose( - width * 2, - height / 2.0, 0 );
		// 			break;
		// 		case 3:
		// 			p = Pose( - width , - height / 2.0, 0 );
		// 			break;
		// 		case 4:
		// 			p = Pose( 0, - height / 2.0, 0 );
		// 			break;
		// 		case 5:
		// 			p = Pose( width, - height / 2.0, 0 );
		// 			break;
		// 
		//         default:  
		// 			cout << "USING DEFAULT SCREEN VIEW POSE" << endl;
		//           break;  
		// 	}
		// 
		// 	scene.camera.view() = View( viewer, p, aspect, height );
		// 	scene.camera.pos() = Vec3f( 0, 0, 50);
		// }    

	
		virtual void onFrame(){

			 scene.onFrame();   
			
	         glClearColor(0,0,0,1);
	         glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
             

	         onDraw();
			 swapBuffers();
			
		}  
		
		virtual void onTimer(){   
			cout << "timer func\n";
	      onFrame();
	    }
	  
	};
	
} 

#endif
