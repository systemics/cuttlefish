                                   //
//  cpu_benchmark.cpp
//  Versor on the Raspberry PI
//
//  Created by Pablo Colapinto on 1/24/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "vsr.h"
#include "vsr_op.h"
#include "vsr_xf.h"
#include "vsr_stat.h"  

#include "ctl_egl.h"
#include "ctl_gl.h"

#include "ctl_glsl.h"
#include "ctl_pipe.h"
#include "ctl_scene.h"

#include "ctl_render.h"  
#include "ctl_bcm.h"
#include "ctl_timer.h" 

#include "ctl_util.h"

#include "vsr_field.h"

#include <iostream>

using namespace std;

using namespace ctl;
using namespace ctl::GL;
using namespace ctl::EGL;
using namespace ctl::GLSL;
using namespace vsr;     

int identifier;   


struct MyWindow : BCM,  public Window {    
	
	 	Scene scene;
		ShaderProgram program;
		
		Field<Pnt> f; 
	   // Field<Pnt> f2;
		
	    float * s;   
	   
		int numDipoles;
		Vec * dp; 
		Dls * dls;
		Par * par; 
		float *  dfl;
		
		Pipe pipe;
		
		Pose viewpose;
		float width, height, aspect, scale; //Width, Height of each screen in inches

        MyWindow(float w, float h) : Window (),
			f(40,20,1,.5)//, f2(20,10,1,1)
		{     
			
			Rand::Seed(1);
			s = new float[f.num()];
			for (int i = 0; i < f.num(); ++i){
				s[i] = 2. + Rand::Num();
			}   
			
			initView(w,h);
            initGL();
        }
        
        ~MyWindow(){}
        
        void initGL(){
             glClearColor(1,1,1,1);
             srand( time(NULL) );

			string Vert = AVertex + Varying + UMatrix  + NTransform + VLighting + VCalc + MVert;
			string Frag = MFrag;

	        program.source(Vert,Frag);
			program.bind();
	             pipe.bindAttributes();
	        program.unbind();  
	
			pipe.program = &program;
			
			initBufferObjects();
 
       }              

		void initBufferObjects(){
		   //	field = new Field<Point> (30,30,1,.1);  	
			numDipoles = 4;
			dp = new Vec[numDipoles]; 
			dls = new Dls[numDipoles];
			par = new Par[numDipoles]; 
			dfl = new float[numDipoles];
		}

		void initView(float w, float h){
	         
			width =  w;
			height = h;     

  			aspect = width / height;   
  		                     

			int numscreens = 4;
			
			vsr::Vec3f viewer(0,0,20);  //Position in inches
			
	
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
	         
			static float time = 0.0; time += .05;  
			

			//Slow method
			
			// for (int i = 0; i < f.num(); ++i){   
			// 	Pnt tp = f[i]; Pnt np;  
			//                   for (int j = 0; j < numDipoles; ++j ){  
			// 		double dist = 1.0 / ( Ro::sqd( f[i], dls[j] ) +.01 );
			// 		np +=  Ro::loc( tp.sp( Gen::bst( par[j] * dist) ) );//.sp(bst) );
			// 	   }
			// 	f[i] = np; 
			// }  
			        
			
			//OR (faster) . . .  [this is not an accurate EM field, but looks like one . . .]
			Vec tally;
			
			for (int i = 0; i < f.num(); ++i){   
				  Par tpar;
                  for (int j = 0; j < numDipoles; ++j ){ 
					float sqd =  ( Ro::sqd( f[i], dls[j] ) +1. );
					float dist = 1.0 / sqd;  
					tpar += par[j] * dist;
				   }
				f[i] = Ro::loc( f[i].sp(  Gen::bst( tpar  * s[i] )  )  );  
				tally += f[i];  
    		}
		   
		 	// Average particle position  is used as Camera Target
			tally /= f.num(); 

			Vec tp( scene.camera.pos()[0], scene.camera.pos()[1], scene.camera.pos()[2] );  
			Vec goal = ( tally - tp ).unit();    
			Rot rot  = Gen::ratio( -Vec::z, goal ); 

			scene.camera.quat() = Rot2Quat(rot); 		
			scene.camera.orient(); 
			
			
			// Oscillation of the Dipoles Position  		   
			for (int j = 0; j < numDipoles; ++j ){     
				float t = 1.0 * j/numDipoles;
				dp[j] = tally + ( Vec::x * width * sin(time) * (j&1? -1 : 1) );
				dls[j] = Ro::dls( dp[j], .2 + .3 * t).trs (0, t * cos(time) * height, 0);
				par[j] = Ro::par( dls[j], Vec::x * (j&1?-1:1) ) * .1; 
		   }
			   
			Mat4f mvm = scene.xf.modelViewMatrixf();
			
		    program.bind();
						
		         program.uniform("projection",  scene.xf.proj);
		         program.uniform("lightPosition", 2.0, 2.0, 2.0);
		         program.uniform("normalMatrix", scene.xf.normal);  
				 program.uniform("modelView", scene.xf.modelView );//app.scene().xf.modelView);        
				
					Render( f, mvm, pipe ); 
				
					for (int j = 0; j < numDipoles; ++j){  
						Render( dls[j], mvm, pipe );
					}   
				
	       program.unbind();       

        }
        
    virtual void onTimer(){   
		cout << "timer func\n";
      onFrame();
    }         

     virtual void onFrame(){
         
		scene.camera.pos() = Vec3f(0,0,40);
		
		scene.onFrame();

         glClearColor(0,0,0,1);
         glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         onDraw();
		 swapBuffers(); 

     }  

};  
       


bool running = true;
void quit(int) {
  running = false;
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

	cout << "IDENTIFIER" << identifier << endl; 

   MyWindow win(14. + 3./8., 10. + 11./16.);    
 
// MyWindow win(21.5, 12.5);   

  //win.start(1 / 15.0);
  while (running) {
    win.onFrame();
    usleep(16666);
    //usleep(16666);
  }

  return 0;
}