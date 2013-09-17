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
#include "ctl_sound.h"

#include "gfx/gfx_scene.h" 
#include "gfx/gfx_pipe.h"
#include "gfx/gfx_gl.h"    

#include "ctl_egl.h" 
#include "ctl_osc.h"
 
using namespace gfx;

namespace ctl {
	
	using namespace EGL;
    using namespace GLSL; 

 	//rowcolumn layout, default is 1 x 1 (single screen). . .
    struct Layout{  
	
		Layout( int M, int N, float w = 21.5, float h = 14.5, float wb = 1.0, float hb = 1.0) :
		numRow(M), numCol(N), screenWidth(w), screenHeight(h), wPitch(wb), hPitch(hb) {}
	
		int numRow; int numCol;                 /// Number of Rows and Columns in Multidisplay
		
		float screenWidth; float screenHeight; 	/// Width of individual screens in inches
		float wPitch; float hPitch;  			/// Bezel
	     
	
	 	//Total Width of MxN Display
		float totalWidth(){
			return numCol * screenWidth + (numCol-1) * wPitch;
		}  
		
		//Total Height of MxN Display
		float totalHeight(){
			return numRow * screenHeight + (numRow-1) * hPitch;
		} 
		
		//gets Pose (bottom left corner) of Mth Row and Nth Col screen
		Pose poseOf( int M, int N){
			return Pose( 
				left(M,N),
				bottom(M,N), 
				0 );
		}  
		
		//Absolute (GL coord) position of left speaker   
		float left(int M, int N){
			return - totalWidth() / 2.0 + N * screenWidth + N * wPitch;
		}  
		   
		//absolute (GL coord) position of right speaker
		float bottom(int M, int N){
			return - totalHeight() / 2.0 + M * screenHeight + M * hPitch;
		}
		
	};

	struct App :  BCM, Host, Window, OSCPacketHandler, Sound { 
	    
		Layout layout; 		///<--- Screen Layout
	    
		Scene scene;   		///<-- ModelViewProjection Transformation Matrices  
		 	
		Pipe pipe;	   		///<-- Graphics Pipeline    
		 
		Vec4f background;  	///<--- Background Color
		
		Mat4f mvm; 			 ///<-- temporary save of scene's transformation matrix    
		
		Vec3f speakerL, speakerR;     ///<-- left and right speaker positions   
	    
         
	     App (float w, float h, float z=30.0) : Window(), background(0,0,0,1), OSCPacketHandler(),
			layout(1,1, w, h, 0, 0)
		{   
			
			//CALCULATE CAMERA POSE     		
			initView(z, false); 
			
			//INITIALIZE GRAPHICS
            initGL();     

			//INITIALIZE AUDIO
			Sound::init();

	    	addListener(GetTouch, "/touch", "iiii", this);  

        }  

		//or pass in an MxN layout . . .
		 App( const Layout& l, float z = 30.0 ) : Window(), background(0,0,0,1), OSCPacketHandler(), layout(l) {
			initView(z, true);
			initGL();
			addListener(GetTouch, "/touch", "iiii", this); 
		} 
		
	 virtual void onSound( SoundData& io ){
		
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
       
		void initView(float z, bool isGrid){
            
			float w = layout.screenWidth;
			float h = layout.screenHeight;   

  		    float aspect = w / h;   
             
			scene.fit(w,h);

			Pose p( -w/2.0,-h/2.0, 0);
			
			speakerL = Vec3f( -w/2.0, 0, 0);
			speakerR = Vec3f( w/2.0, 0, 0);
			              
			//If we're in multi-screen mode, RE-DO pose and speaker positions based on grid layout . . .
			if (isGrid) {
				p = layout.poseOf( identifier.row, identifier.col );   
			
				//only works on a grid . . .
				speakerL = Vec3f( 
					layout.left( identifier.row, identifier.col ), 
					layout.bottom( identifier.row, identifier.col ) + layout.screenHeight / 2.0, 0);

				speakerR = Vec3f(
				 	layout.left( identifier.row, identifier.col ) + layout.screenWidth, 
					layout.bottom( identifier.row, identifier.col ) + layout.screenHeight / 2.0, 0);
			 }
			

			scene.camera.pos() = Vec3f( 0, 0, z); 
			scene.camera.view = View( scene.camera.pos(), p, aspect, h );

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
		   // cout << "timer func\n";
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
