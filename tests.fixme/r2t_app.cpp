 

#include "ctl_app.h"
#include "ctl_gl_mbo.h" 
#include "ctl_gl_fbo.h"   

#include "ctl_osc.h"

using namespace std;
using namespace ctl;

struct MyApp : App, OSCPacketHandler {
        
    //THIS IS THE APP SPECIFIC STUFF, 
	MBO * circle;   
	
	//ALL THIS IS THE PIPELINE STUFF.  MANDATORY
	MBO * rect;   
	   
	FBO fboA;
	Texture * textureA;    
	Texture * textureB;     
	
	Pipe texpipe; 
	Pipe texalpha; 
	
	Pipe blurpipe;
	
	float traceAmt;
	Vec3f pos; 
	float ux, uy, bluramt;
    
	MyApp() : App(14. + 3./8., 10. + 11./16.), OSCPacketHandler() {
	
		init();
	}  
	
	void swapTextures(){
		Texture * tmp = textureA; textureA = textureB; textureB = tmp;
	}  
	

	
	void initPipeline(){    
		
		//Make Two Textures the size of the screen (as big as possible anyway: there are memeory limitations )   
		int tw = surface.width/2.0; int th =  surface.width/2.0;
		textureA = new Texture( tw,th);//surface.width/2.0, surface.height/2.0 );   
		textureB = new Texture( tw,th);//surface.width/2.0, surface.height/2.0 );  		                    
		                                                                             
		//This is the shader used to draw the Rendered Texture to screen 
		//It doesn't use any transformation matrices, just clip space and the texture values
		string FragA = TFragBlur;//TFrag;
		texpipe.program = new ShaderProgram( ClipSpaceVert, FragA, 0 );
		texpipe.bindAll();  
		
		//This Shader Will Be the "Feedback Shader" that adds a bit of the previously drawn Frame on top
		string FragB = TFragAlpha;
		texalpha.program = new ShaderProgram( ClipSpaceVert, FragB, 0 );
		texalpha.bindAll();		
	}
 
	void initObjects(){
		cout << "init buffer objects" << endl;   	
		circle = new MBO( Mesh::Circle().moveTo(0,.5,0).color(1,0,0,1) );     	
		rect = new MBO( Mesh::Rect( 2.0, 2.0 ).color(0,0,0,1.0) );
	}
	
	void initOSC(){
		addListener( GetTrace, "/trace", "f", this);
	   	addListener( GetPosition,"/xy","ff", this); 
	 	addListener( GetUX,"/ux","f", this);  
	 	addListener( GetUY,"/uy","f", this);  
	   	addListener( GetBlur,"/bluramt","f", this);
	}

	virtual void init(){ 
		
		initPipeline();  //<-- Initialize the Graphics Pipeline Shaders, Textures, etc.
		initObjects();   //<-- Create and bind all the VBO's   
		initOSC();
	}
	
	void updateMeshes(){
		
		static float time = 0; time +=.03;   
		float x = sin(time);    

		circle -> mesh.moveTo( pos );//x * width /2.0, .5, 0 ); 
		//cout << pos << endl;  
		circle  -> update();
		
	}      
	
	
	//This is where the actual objects get rendered
	void drawScene(){
 		updateMeshes();   			//<-- CPU Calculations happen in updateMeshes()   
        pipe.line( *circle );  		//<-- Then we Render all our Shit
	}   
	     
	
   //This is the texture feedback loop 
	virtual void onDraw(){     
        
 		swapTextures();
		//Write into textureA using textureB 
		fboA.attach(*textureA, GL::COLOR);  
		fboA.bind();               
		
		    glViewport(0,0,textureA->width(),textureA->height() ); 
			glClearColor(0,0,0,1);
           	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			//We add a bit of the previously drawn frame
			texalpha.bind();
			    texalpha.program -> uniform("alpha", traceAmt);
			
			    textureB -> bind();      
					texalpha.line( *rect );
				textureB -> unbind();  
				    
			texalpha.unbind(); 
			 
			//And add a new frame on top
			 pipe.bind( scene.xf );
				
				drawScene();  
		
			pipe.unbind( );  
			
		fboA.unbind();  

		//Switch to full Viewport and Bind Texture to Rect 
	   glViewport(0,0,surface.width,surface.height);		
		texpipe.bind ();
		texpipe.program -> uniform("ux" ,ux);
		texpipe.program -> uniform("uy" ,uy); 
		texpipe.program -> uniform("bluramt", bluramt);
			textureA -> bind();
				texpipe.line( *rect );
			textureA -> unbind();
	    texpipe.unbind();

	}  
	
	
	static int GetTrace(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {

		MyApp& app = *(MyApp*)user_data;  
		
		app.traceAmt =  argv[0]->f;  
		cout << app.traceAmt << endl;
		
	    return 0;
	}  
	
	static int GetUX(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {

		MyApp& app = *(MyApp*)user_data;  
		
		app.ux =  argv[0]->f;  
		cout << app.ux << endl;
		
	    return 0;
	}
	
	static int GetUY(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {

		MyApp& app = *(MyApp*)user_data;  
		
		app.uy =  argv[0]->f;  
		cout << app.uy << endl;
		
	    return 0;
	} 
	
 	static int GetBlur(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {

		MyApp& app = *(MyApp*)user_data;  
		
		app.bluramt =  argv[0]->f;  
		cout << app.bluramt << endl;
		
	    return 0;
	}      	
	
	static int GetPosition(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {

		MyApp& app = *(MyApp*)user_data;  
		//cout << "/xy " << argv[0]->f << " " <<  argv[1]->f << endl; 
		
		float l = - app.width / 2.0 ;
		float b = - app.height / 2.0; 
		app.pos =  Vec3f( l + app.width * argv[0]->f ,  b + app.height * ( 1- argv[1] -> f ) , 0.0 );  
	   // cout << app.pos << endl;
		
	    return 0;
	}	
	
};
    
bool running = true;
void quit(int) {
  running = false;
}


int main(){
	
	MyApp app; 
  
	while(running){
		app.onFrame();
		usleep(166);
	}   
	
	  return 0; 
}