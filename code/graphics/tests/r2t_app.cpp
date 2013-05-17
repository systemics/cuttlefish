 

#include "ctl_app.h"
#include "ctl_gl_mbo.h" 
#include "ctl_gl_fbo.h"

using namespace std;
using namespace ctl;

struct MyApp : App {
        
    //THIS IS THE APP SPECIFIC STUFF, 
	MBO * circle;   
	
	//ALL THIS IS THE PIPELINE STUFF.  MANDATORY
	MBO * rect;
	   
	FBO fboA;
	Texture * textureA;    
	Texture * textureB;     
	
	Pipe texpipe; 
	Pipe texalpha;
    
	MyApp() : App(14. + 3./8., 10. + 11./16.) {
	
		init();
	}  
	
	void swapTextures(){
		Texture * tmp = textureA; textureA = textureB; textureB = tmp;
	}  
	
	void initObjects(){
		cout << "init buffer objects" << endl;   	
		circle = new MBO( Mesh::Circle().color(1,0,0,1) );     	
		rect = new MBO( Mesh::Rect( 2.0, 2.0 ).color(0,0,0,.9) );
	}   
	
	void initPipeline(){    
		
		//Make Two Textures the size of the screen (as big as possible anyway: there are memeory limitations )
		textureA = new Texture( surface.width/2.0, surface.height/2.0 );   
		textureB = new Texture( surface.width/2.0, surface.height/2.0 );  		                    
		
		//This is the shader used to draw the Rendered Texture to screen 
		//It doesn't use any transformation matrices, just clip space and the texture values
		string Vert = ClipSpaceVert;
		string Frag = TFrag;
		texpipe.program = new ShaderProgram( Vert, Frag, 0 );
		texpipe.bindAll();  
		
		//This Shader Will Be the "Feedback Shader" that adds a bit of the previously drawn Frame on top
		string FragB = TFragAlpha;
		texalpha.program = new ShaderProgram( Vert, FragB, 0 );
		texalpha.bindAll();		
	}

	virtual void init(){ 
		
		initPipeline();  //<-- Initialize the Graphics Pipeline Shaders, Textures, etc.
		initObjects();   //<-- Create and bind all the VBO's
	
	}
	
	void updateMeshes(){
		
		static float time = 0; time +=.03;   
		float x = sin(time);    

		circle -> mesh.moveTo( x * width /2.0, 0, 0 );   
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
			textureA -> bind();
				texpipe.line( *rect );
			textureA -> unbind();
	    texpipe.unbind();

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