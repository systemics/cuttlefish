#ifndef CTL_APPR2T_INCLUDED
#define CTL_APPR2T_INCLUDED

#include "ctl_app.h"
#include "gfx/gfx_mbo.h" 
#include "gfx/gfx_fbo.h"
#include "ctl_osc.h"

namespace ctl {

/// A Render to Texture Application with OSC calls for blur effects etc.  Subclass ths and render objects in a drawScene() method.
struct AppR2T : App {
	
	//ALL THIS IS THE GL PIPELINE STUFF.
	MBO * rect;   
	   
	FBO fboA;
	Texture * textureA;    
	Texture * textureB;     
	
	Pipe texpipe; 
	Pipe texalpha; 	
	//Pipe blurpipe;
	
	float traceAmt;
	float ux, uy, bluramt;  
	
	bool bUX, bUY, bBLUR, bTRACE; //update bools
    
	AppR2T(float w, float h, float z = 30) : App(w, h, z) {	
		//init();
	}  
	
	void swapTextures(){
		Texture * tmp = textureA; textureA = textureB; textureB = tmp;
	}  
	  
	void initPipeline(){ 
	   
	 	//defaults
		ux = uy = .02;
		traceAmt = .9;
		bluramt = 0.2; 
		
		bUX = bUY = bBLUR = bTRACE = 0;   
		
		//Make Two Textures the size of the screen (as big as possible anyway: there are memeory limitations )   
		int tw = surface.width; int th = surface.height; 
		//int tw = 1280; int th = 853; 
		printf("surface: %d\t%d\n", tw, th);
		textureA = new Texture( tw,th);//surface.width/2.0, surface.height/2.0 );   
		textureB = new Texture( tw,th);//surface.width/2.0, surface.height/2.0 );  		                    
		                                                                             
		//This is the shader used to draw the Rendered Texture to screen 
		//It doesn't use any transformation matrices, just clip space and the texture values
		string FragA = FXAA;//TFragBlur;//TFrag; //FXAA;//
		texpipe.program = new ShaderProgram( ClipSpaceVert, FragA, 0 );
		texpipe.bindAll();  
		
		//This Shader Will Be the "Feedback Shader" that adds a bit of the previously drawn Frame on top
		string FragB = TFragAlpha;
		texalpha.program = new ShaderProgram( ClipSpaceVert, FragB, 0 );
		texalpha.bindAll();	
		
		 fboA.attach(*textureA, GL::COLOR);
 
	}
 
	void initSlab(){    
		
		printf("init buffer objects\n");   	    	
		rect = new MBO( Mesh::Rect( 2.0, 2.0 ).color(0,0,0,1.0) ); 
		 
		glClearColor(0,0,0,1);  
	
	             
	}
	
	void initOSC(){
		addListener( GetTrace, "/trace", "f", this);
	 	addListener( GetUX,"/ux","f", this);  
	 	addListener( GetUY,"/uy","f", this);  
	   	addListener( GetBlur,"/bluramt","f", this);
	}

	virtual void init(){ 
		printf("initializing r2t pipeline\n");
		initPipeline();  //<-- Initialize the Graphics Pipeline Shaders, Textures, etc.
		initSlab();      //<-- Create and bind all the VBO's   
	    initOSC();    	 //<-- Initialize OSC handling for shader settings
	}
	
	 //This is where the actual objects get updated  
	virtual void updateMeshes(){}  
	
	//This is where the actual objects get rendered
	virtual void drawScene() = 0;   
	               
	//We can add a bit of the previously drawn frame  
	void addTrace(){
		texalpha.bind();
	   // float nt = traceAmt;
		    texalpha.program -> uniform("alpha", traceAmt); 

		    textureB -> bind();      
				texalpha.line( *rect );
			textureB -> unbind();  

		texalpha.unbind();  
	}   
	
	void setBlur(){
		texpipe.program -> uniform("ux" ,ux);
		texpipe.program -> uniform("uy" ,uy); 
		texpipe.program -> uniform("bluramt", bluramt);
	}  
	
	void setFrameBuf(){
		texpipe.program -> uniform("frameBufSize", surface.width, surface.height );
	}
	     
   //This is the texture feedback loop 
	virtual void onDraw(){     
        

		//Write into textureA using textureB 
		fboA.attach(*textureA, GL::COLOR);  
		fboA.bind();               
		
		    glViewport(0,0,textureA->width(),textureA->height() ); 
			glClearColor(0,0,0,1);
           	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			//if (bTrace) addTrace()
			 
			//And add a new frame on top
			 mvm = scene.xf.modelViewMatrixf();
			 pipe.bind( scene.xf );
				
				drawScene();  
		
			 pipe.unbind( );  
			
		fboA.unbind();  

		//Switch to full Viewport and Bind Texture to Rect 
	   glViewport(0,0,surface.width,surface.height);	 
	   
		texpipe.bind ();
	
		//if using TFragBlur 
		
 		//setBlur();
		setFrameBuf();
		
		textureA -> bind();
			texpipe.line( *rect );
		textureA -> unbind();     
			
	    texpipe.unbind();  
	
	   swapTextures(); 

	}  
	
	virtual void onFrame(){
		
        glClearColor(background[0],background[1],background[2],background[3]);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        scene.onFrame();
         
        update();

        onDraw();
		 	
		swapBuffers();
		
	}
	
	
	static int GetTrace(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {

		AppR2T& app = *(AppR2T*)user_data;  
		
		app.traceAmt =  argv[0]->f;  
		cout << app.traceAmt << endl;
		
	    return 0;
	}  
	
	static int GetUX(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {

		AppR2T& app = *(AppR2T*)user_data;  
		
		app.ux =  argv[0]->f;  
		cout << app.ux << endl;
		
	    return 0;
	}
	
	static int GetUY(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {

		AppR2T& app = *(AppR2T*)user_data;  
		
		app.uy =  argv[0]->f;  
		cout << app.uy << endl;
		
	    return 0;
	} 
	
 	static int GetBlur(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {

		AppR2T& app = *(AppR2T*)user_data;  
		
		app.bluramt =  argv[0]->f;  
		cout << app.bluramt << endl;
		
	    return 0;
	}      	
	
	// static int GetPosition(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
	// 
	// 	AppR2T& app = *(AppR2T*)user_data;  
	// 	//cout << "/xy " << argv[0]->f << " " <<  argv[1]->f << endl; 
	// 	
	// 	float l = - app.width / 2.0 ;
	// 	float b = - app.height / 2.0; 
	// 	app.pos =  Vec3f( l + app.width * argv[0]->f ,  b + app.height * ( 1- argv[1] -> f ) , 0.0 );  
	//    // cout << app.pos << endl;
	// 	
	//     return 0;
	// }   
	
};  

}    

#endif