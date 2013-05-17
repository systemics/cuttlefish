 

#include "ctl_app.h"
#include "ctl_gl_mbo.h" 
#include "ctl_gl_fbo.h"

using namespace std;
using namespace ctl;

struct MyApp : App {
        

	MBO * circle; 
	MBO * rect;
	MBO * rectTrans;  
	   
	FBO fboA;//, fboB;
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

	virtual void init(){ 
		
		cout << "init buffer objects" << endl;
		
		circle = new MBO( Mesh::Circle().color(1,0,0,1) ); 
		rect = new MBO( Mesh::Rect( 2.0, 2.0 ).color(0,0,0,.9) );
	   // rectTrans = new MBO( Mesh::Rect(  2.0, 2.0  ).color(0,0,0,.9) ); 
		  		
		//Make a Texture the size of the screen
		textureA = new Texture( surface.width/2.0, surface.height/2.0 );   
		textureB = new Texture( surface.width/2.0, surface.height/2.0 );   
		//Attach it to Framebuffer  ColorAttachment
		fboA.attach(*textureA, GL::COLOR);  
		//fboB.attach(*textureB, GL::COLOR); 
				
		//create new shader on secondary pipeline  
		string Vert = ClipSpaceVert;//AVertex + Varying + UMatrix  + NTransform + VLighting + VCalc + MVert;
		string Frag = TFrag;
		texpipe.program = new ShaderProgram( Vert, Frag, 0 );
		texpipe.bindAll();  
		
		//create new shader on tertiary pipeline  
//		string VertB = AVertex + Varying + UMatrix  + NTransform + VLighting + VCalc + MVert;
		string FragB = TFragAlpha;
		texalpha.program = new ShaderProgram( Vert, FragB, 0 );
		texalpha.bindAll();		
		
		//glLineWidth(10);
		
	}                  
	
	void updateMeshes(){
		
		static float time = 0; time +=.03;   
		float x = sin(time);    

		circle -> mesh.moveTo( x * width /2.0, 0, 0 );   
		circle  -> update();
		
	}
	
	virtual void onDraw(){     
        
		updateMeshes();
        
		swapTextures();
		//Write into textureA using textureB 
		fboA.attach(*textureA, GL::COLOR);  
		fboA.bind();               
		
		    glViewport(0,0,textureA->width(),textureA->height() ); 
			glClearColor(0,0,0,1);
           	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			texalpha.bind();
			
			    textureB -> bind();      
					texalpha.line( *rect );
				textureB -> unbind();  
				    
			 texalpha.unbind();   
			
			pipe.bind( scene.xf ); 

				pipe.line( *circle );  

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