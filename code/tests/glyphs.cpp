
#include "ctl_bcm.h"
#include "ctl_timer.h"  
                              
#include "ctl_egl.h"
#include "ctl_gl.h"

#include "ctl_glsl.h"
#include "ctl_mdraw.h"
#include "ctl_scene.h"

// #include "ctl_gl_texture.h" 
// #include "ctl_gl_fbo.h" 
// #include <unistd.h>

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
	Pose viewpose;
	float width, height, aspect, scale; //Total Width, Height of all Screens Combined

	Pipe pipe;
	ShaderProgram program;   
	
	MBO * sphere;

    MyWindow(float w, float h) : Window () {  
        initGL();
		initView(w,h);   
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
		
		pipe.program = &program;
		
		initBufferObjects(); 
	}
   
	void initBufferObjects(){
		sphere = new MBO( Mesh::Sphere() );
		
	}

		void initView(float w, float h){
	         

			//use 16 GL units x 9 GL units   
			//pass in width and height in inches
			width =  w;//21.5;
			height = h;//12.5;     

			aspect = width / height;   
		                     

			int numscreens = 4;
			
			Vec3f viewer(0,0,20);  //Position in inches
			
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
	    program.bind();
	
	         program.uniform("projection",  scene.xf.proj);
	         program.uniform("normalMatrix", scene.xf.normal);
	         program.uniform("modelView", scene.xf.modelView );//app.scene().xf.modelView);        
	         program.uniform("lightPosition", 2.0, 2.0, 2.0);
	 
			pipe.line( *sphere );
	
	  program.unbind();
	
}
   virtual void onTimer(){   
		cout << "timer func\n";
      onFrame();
    }         


        virtual void onFrame(){
			                                             
			static float time = 0; time += .01;
			scene.camera.pos() = Vec3f(0,0,20);
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

  MyWindow win(14. + 3./8., 10. + 11./16.);    

  //win.start(1 / 15.0);
  while (running) {
    win.onFrame();
    usleep(16666);
    //usleep(16666);
  }
      

}


