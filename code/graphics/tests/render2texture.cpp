//
//  Render2Texture.cpp
//  Render2Texture on the Raspberry PI
//
//  Created by Pablo Colapinto on 1/24/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "ctl_egl.h"
#include "ctl_gl.h"

#include "ctl_glsl.h"
#include "ctl_mdraw.h"
#include "ctl_scene.h"
#include "ctl_screen.h" 

#include "ctl_gl_texture.h" 
#include "ctl_gl_fbo.h" 

#include "ctl_bcm.h"
#include "ctl_timer.h"  

#include <lo/lo.h>   

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



string TexVert = STRINGIFY( 

    attribute vec3 position;
    attribute vec2 texCoord;
  
    varying lowp vec2 texco;  

	void main(void){  
		texco = texCoord;
		gl_Position = vec4(position,1.0);
    }
); 

string TexFrag = STRINGIFY(     

	uniform sampler2D tex; 
	
	varying lowp vec2 texco;    
	
	void main(void){
        
        vec4 texColor = texture2D(tex, texco);   
		gl_FragColor = texColor;
    }

);         




struct MyWindow : BCM, Timer, public Window {

	 	Scene scene;
		ShaderProgram programA;
 		ShaderProgram programB;  

		Pipe A, B;
   	 
		FBO fbo; 
		Texture * texture;
		
		Pose viewpose;
		float width, height, aspect, scale; //Total Width, Height of all Screens Combined
		 
		MBO * circle; 
		MBO * grid;
		MBO * line; 
		MBO * rect;
		
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

	        programA.source(Vert,Frag);
			programA.bind();
	             A.bindAttributes();
	        programA.unbind();  
	
		    programB.source(TexVert,TexFrag);
			programB.bind();
				 B.bindPosition(); 
			     B.bindTexture(); 
			programB.unbind();   
			
			initView();  
			initBufferObjects(); 
			//program.bind();
		}	
		
		void initBufferObjects(){
			circle 	= new MBO ( Mesh::Disc(4).color(1,1,0), GL::DYNAMIC  );
			line 	= new MBO ( Mesh::Rect( width *4 , 1.0 ).color(0,1,1), GL::DYNAMIC );
			grid 	= new MBO ( Mesh::Grid( width * 4, height, 1 ) );  
			
			rect = new MBO ( Mesh::Rect(2.0,2.0) ); 
			
			int w = scene.camera.lens().mWidth;
			int h = scene.camera.lens().mHeight;
			
		    texture = new Texture( w, h);  
		   
		 	//attach Texture to Framebuffer's color attachment
	      // fbo.attach(*texture, GL::COLOR);
	       //fbo.attach( RBO(w,h, GL::DEPTHCOMP), GL::DEPTH);  
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
             
		void updateMeshes(){
			static float time = 0; time +=.03;   
			float x = sin(time);     
			
			circle -> mesh.moveTo( x * width * 2.0, 0, 0 );   
			circle  -> update();
			
			line  -> mesh.moveTo( 0, x * height/2.0, 0 );       
			line  -> update();  
		}

	   //render to texture and then draw texture to screen            
        virtual void onDraw(){
			 
		    updateMeshes(); 


				// 			   fbo.bind();   
				// 		                  
				// glViewport(0,0,texture->width(),texture->height() );
				// 	                                                                   
				// 	            glClearColor(0,1,0,1);
				// 	           	// clear screen
				// 	           	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      
				//    					
				//  
				//   			 	programA.bind();  
				// 
				// 	 	         	programA.uniform("projection",  scene.xf.proj);
				// 		         	programA.uniform("normalMatrix", scene.xf.normal);
				//  	programA.uniform("modelView", scene.xf.modelView );//app.scene().xf.modelView);    
				// 			
				//    			 			A.line( *grid );  
				//  		A.line( *circle );
				//     	A.line( *line );      
				//    	    		
				// 			   programA.unbind();   
				//    
				// 			   		   fbo.unbind();    
			// // 
			//   glViewport(0,0,scene.camera.lens().mWidth,scene.camera.lens().mHeight); 
			// // 
			// //     
			programB.bind();  
			// 
	 	     texture -> bind();  
		  			B.pos_tex( *rect );   
		     texture -> unbind();    
			// 
		    programB.unbind();      


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
