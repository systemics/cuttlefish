#include "ctl_app.h"
#include "gfx/gfx_mbo.h"

#include "Gamma/Gamma.h"    // core functions
#include "Gamma/Access.h"
#include "Gamma/AudioIO.h"
#include "Gamma/Delay.h"
#include "Gamma/DFT.h"
#include "Gamma/Effects.h"
#include "Gamma/Envelope.h"
#include "Gamma/FormantData.h"
#include "Gamma/Noise.h"
#include "Gamma/Oscillator.h"
#include "Gamma/SamplePlayer.h"
#include "Gamma/Print.h"
#include "Gamma/Recorder.h"
#include "Gamma/Scheduler.h"
#include "Gamma/SoundFile.h"
#include "Gamma/Timer.h"
#include "Gamma/Types.h"
#include "Gamma/UnitMaps.h"

using namespace std;
using namespace ctl; 
using namespace gam; 

struct MyApp : public App {
        
    //THIS IS THE APP SPECIFIC STUFF, 
	MBO * mbo;  

  NoisePink<> noise;
  LFO<> lfo;
  Biquad<> biquad;
	 
	Quat model;
	//Frame model; 
	
	float time; 
	
	//INSTANTIATE THE APPLICATION WITH WIDTH AND HEIGHT
	MyApp() : App(21.5,14.5), time(0) { 
		init();

    biquad.set(10, 4, BAND_PASS);
    lfo.set(0.6, 0, 0.5);
	}   
	
	virtual void onSound( SoundData& io ){
		for (int i = 0; i < io.n; ++i) {
            float cutoff = scl::pow3(lfo.triU()) * 10000;
            biquad.freq(cutoff);
			     	float s = biquad(noise()) * 0.5;
			     	for (int j = 0; j < 2; j++) {
			       		*io.outputBuffer++ = (short)(s * 32767.0);
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
		mbo -> mesh.rotate( Quat( .05, Vec3f(0,1,0) ) );
		// mbo -> mesh.translate( .05, 0, 0 );
		mbo -> update(); 
		

		
		

	}

	//THIS DRAWS THE CIRCLE TO THE SCREEN
	virtual void onDraw(){ 
		 
		pipe.line( *mbo );
	
	} 
	
}; 

STARTANDRUN()

