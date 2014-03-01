#include "ctl_appR2T.h"
#include "vsr/vsr_cga3D_op.h"
#include "vsr/vsr_field.h"
#include "gfx/gfx_mbo.h"

#include "Gamma/Gamma.h"    // core functions
#include "Gamma/Filter.h"
#include "Gamma/Noise.h"
#include "Gamma/Oscillator.h"
#include "Gamma/SamplePlayer.h"

using namespace std;
using namespace ctl;
using namespace vsr;

struct MyApp : public AppR2T {
	
	Field<Vec2D> vf;  	/// THE VECTOR FIELD
	Field<Vec2D> orth; 	/// THE ORTHOGONAL VECTOR FIELD
	
	Field< Sca > rf;    /// Div and Curl of the Field
	Field< Sca > gf;    /// Div and Curl of the Field
	Field< Sca > bf;    /// Div and Curl of the Field
	
	int numDipoles;  	/// NUMBER OF DIPOLES TO CALCULATE
	Vec2f * touch; 	    /// AN ARRAY OF TOUCHES
	Pnt * dipole;
	Par * par;   		/// DIPOLES THEMSELVES

  gam::NoisePink<> noise;
  gam::Biquad<> biquad;
  gam::SineD<> bonk;
	
	int numPotentials;
	int pRes;
	float step;

	MBO * potentials;  
	MBO * potentialsB;  
	
	int numAgents;
	Field< Vec > agent;
	
    //a time element
	float time;
	float gain, res, cutoff;
	
	MyApp() : AppR2T( Layout(1,4) ),  //( 21.5,14.5),//
		time(0),
		vf(44,7,1,2),
		orth(44,7,1,2),
		rf(44,7,1,2),
		gf(44,7,1,2),
		bf(44,7,1,2),
		numDipoles(3),
		numPotentials(10),
		pRes(40),
		step(2.0),
		numAgents(100),
		agent(numAgents,1,1,1)
	
	{
		AppR2T::init();

    gam::Sync::master().spu(48000); // XXX!!!
		Sound::init();

		bDoTrace 		= true;
		traceAmt 		= .2; 


		touch 		= new Vec2f[numDipoles];
		dipole 		= new Pnt[numDipoles];
		par 		= new Par[numDipoles];  
		
		potentials 	= new MBO[numPotentials];
		potentialsB = new MBO[numPotentials];
				
		for (int i = 0; i < numPotentials; ++i){
			 potentials[i]   = MBO( Mesh::Contour(pRes).color(0,1,0).mode(GL::LS) );
			 potentialsB[i]  = MBO( Mesh::Contour(pRes).color(0,1,0).mode(GL::LS) );
		}
		
    	gain = 0.5f;
    	cutoff = 500.0f;
    	res = 1.0f;
	}
	
  void onTouch(int finger, int xPosition, int yPosition, int orientation) {

		if (finger < numDipoles){
			
			float tx = xPosition / 3600.0;
		    float ty = yPosition / -2600.0;
		
			touch[finger] = Vec2f(tx,ty);
			
			dipole[finger] = Point( layout.totalWidth()/2.0 * tx, layout.totalHeight()/2.0 * ty, 0 );
		}

    if (finger == 0) {
      gain = (touch[0].y * 0.5f) + 0.5f;
      cutoff = ((touch[0].x * 0.5f) + 0.5f) * 1100.0f;
      res = 2;
    }
	}

  virtual void onSound( SoundData& io ){
    for (int i = 0; i < io.n; ++i) {
      biquad.freq(cutoff);
      biquad.res(res);
      float s = biquad(noise()) * gain;
      for (int j = 0; j < 2; j++) {
        *io.outputBuffer++ = (short)(s * 32767.0);
      }
    }   
  }
	
	//Initialize
	void initMeshes(){}
	
	//Update the Geometry
	void update(){
		
		time += .05;
		
//		calcDipoles();
//		
//		calcVecFields();
//		
//		calcEquipotentials();
//		
//		calcDivCurl();  
//		
//		calcAgents();   
		
	}      
	
	void calcAgents(){
		for (int i = 0; i < agent.num(); ++i ){  
			
			// for (int )
			// agent.grid(i).bst( par)   
		}
	}
	
	void calcDivCurl(){
		
		for (int i = 0; i<vf.num(); ++i){  
			auto a = Vec2D(1,1)* vf[i];
			rf[i] = a[0];
			gf[i] = a[1];
		}
		
		for (int i = 0 ; i < gf.num(); ++i){
			bf[i] = gf.sumNbrs(i) / 4.0;
		}
	}
	
	void calcDipoles(){
		for (int i = 0; i < numDipoles; ++i){
			//float t =  1. + 1.0 * i/numDipoles;
			//touch[i] = Point( sin(time*t) * layout.totalWidth()/2.0, 0, 0 );
			par[i] = Par(Tnv(0, 1.0 / numDipoles  ,0)).trs( dipole[i]  );
		}
	}
	
	void calcVecFields(){
		 for (int i = 0; i < vf.num(); ++i){
				static Par tpar;
				tpar.reset();
				
				//sum in respective dipoles
				for (int j = 0; j < numDipoles; ++j ){
					float sqd =  ( Ro::sqd( vf.grid(i), dipole[j] ) + .1 );
					float dist = 1.0 / sqd;
					tpar += par[j] * dist;
				 }
				
			   // Vec np =  vf.grid(i).bst ( tpar );//Ro::loc( vf.grid(i).bst ( tpar ) );
				Vec np = Ro::loc( vf.grid(i).bst ( tpar ) );
				vf[i] = Vec( np - Vec(vf.grid(i)) );
			    orth[i] = vf[i].rot( Biv::xy * PIOVERFOUR );
			}
	}
	
	void calcEquipotentials(){
		
		for (int i = 0; i < numPotentials; ++i){
			float theta = (TWOPI * i/numPotentials);
			float ta = cos( theta ) * orth.tw()/2.0;
			float tb = sin( theta ) * orth.th()/2.0;
			
			// Vec nv(ta, tb, 0);  //starting point
			// Vec nvB(ta, tb, 0);  //starting point
			Vec2D nv(ta, tb);  //starting point
			Vec2D nvB(ta, tb);  //starting point
			
			for (int j = 0; j < potentials[i].mesh.num(); ++j ){
				//float tt = 1.0 * j/potentials[i].mesh.num();
				potentials[i].mesh[j].Pos.set( nv[0], nv[1], 0 );     //2D
			    //potentials[i].mesh[j].Col.set( 0,  1,  ta * .3, .9);
				potentialsB[i].mesh[j].Pos.set( nvB[0], nvB[1], 0 );  //2D
			    //potentialsB[i].mesh[j].Col.set( 0,  1,  ta * .3, .9);
				nvB -= orth.euler2d( nvB ) * step;
				nv += orth.euler2d( nv ) * step;
			}
			
			potentials[i].update();
			potentialsB[i].update();
		}
	}
	

	
    //DRAW GEOMETRY TO SCREEN
	 virtual void drawScene(){	
//
//		// 
//			    // DRAWCOLOR( vf, 1,1,0 );
//		Render( vf, mvm, pipe, rf, gf, bf );
//		//DRAWCOLOR( orth, 0,1,0);
//		
//		for (int i = 0; i < numPotentials; ++i){
//			pipe.line( potentials[i] );
//			pipe.line( potentialsB[i] );
//		}     
//		
//		
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
 		//usleep(1666);
 	}
	
	  return 0;
}
