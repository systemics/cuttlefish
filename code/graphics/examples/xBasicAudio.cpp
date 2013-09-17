#include "ctl_app.h"
#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 

struct MyApp : public App {
        
    //THIS IS THE APP SPECIFIC STUFF, 
	MBO * mbo;  
	
	float time;    
	
	float mix[2]; 			///<-- Left Right Sound Mix  
	            
	//INSTANTIATE THE APPLICATION WITH WIDTH AND HEIGHT
	MyApp() : App(21.5,14.5), time(0){ 
		init();
	}   
	
	virtual void onSound( SoundData& io ){     
		for (int i = 0; i < io.n; ++i) {
			float s = i / (float)io.n;
			for (int j = 0; j < 2; j++) {
			    *io.outputBuffer++ = (short)(s * 32767.0) * mix[j];
			 }
		}  
	}                               
	
	//INIITIALIZE ELEMENTS OF THE SCENE 
	virtual void init(){
		
		//Make a Mesh and add vertices
		Mesh mesh;
		mesh.add ( -1, -1, 0).add();
		mesh.add ( -1, 1, 0).add();
		mesh.add ( 1, 1,  0).add();
		mesh.add ( 1, -1, 0).add();  
		
				
		mbo = new MBO( mesh );
	}  
	
	//THIS CHANGES THE POSITION OF THE CIRCLE
	void update(){
	   
		time +=.01;
		 
		//scene.model.rot() 	= Quat( sin( time) * PI, Vec3f(1,0,0) );
	    //scene.camera.rot()  = Quat( sin( time) * .2, Vec3f(0,1,0) );
		//scene.camera.pos() = Vec3f(0,0,sin(time));
	   // mbo -> mesh.rotate( Quat( .05, Vec3f(0,1,0) ) );
	
		Vec3f v(cos(time) * layout.screenWidth/2.0, 0, 0 );
	    mbo -> mesh.translateA( v.x, 0, 0 );
		mbo -> update(); 
		
		mix[0] = lw( v );
		mix[1] = rw( v );
		
		printf( "mix: %f, %f\n", mix[0], mix[1] );
	}  
	
	float lw( const Vec3f& v ){
		float sqd = (v - speakerL).sq(); //squared distance
		return CLAMP( 1.0 / ( fabs( 1.0 - sqd ) + .001), 0, 1); 
	}                        
	
	float rw( const Vec3f& v ){
		float sqd = (v - speakerR).sq(); //squared distance
		return CLAMP( 1.0 / ( fabs( 1.0 - sqd ) + .001), 0, 1) ; 
	}

	//THIS DRAWS THE CIRCLE TO THE SCREEN
	virtual void onDraw(){ 
		 
		pipe.line( *mbo );
	
	} 
	
}; 

STARTANDRUN()

