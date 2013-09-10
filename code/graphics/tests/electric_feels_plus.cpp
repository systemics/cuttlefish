#include "vsr.h"
#include "vsr_op.h"
#include "vsr_xf.h" 
#include "vsr_stat.h"   
#include "vsr_field.h"

#include "ctl_util.h"
#include "ctl_render.h"
#include "ctl_appR2T.h"  
#include "ctl_mainloop.h"
#include "ctl_stopwatch.h" 
#include "ctl_sound.h" 

#include <Gamma/Oscillator.h>
#include <Gamma/Delay.h>

using namespace std;
using namespace ctl;
using namespace vsr;  

struct MyApp :  AppR2T, Sound {  //MainLoop,
     
	StopWatch<> stopWatch;  
    
  	gam::SineD<> sineD[5];
  	gam::Delay<> delay;

	MBO * field;
    MBO * potentials;  
	float equiAmt;
	float fieldAmt;
 
    Field<vsr::Pnt> f;
	Field<vsr::Vec> vf;
    Field<vsr::Vec> orth; // orthogonal field to vf;

	float * s;   
	   
	int numDipoles, numParticles, numPotentials;
	
	float vel;
	float cam; // camera z position

	vsr::Vec * dp;   
	vsr::Vec * dpdir; 
    vsr::Dls * dls;  
	vsr::Par * par; 
	
	bool bReset; 
    
	float ow, oh, tw, th;

	MyApp() : 
	AppR2T( 21.5, 14.5 ),
    f(20,7,1,2), vf(25,5,1,3.5), orth(25,5,1,3.5),
    delay(0.211)

	{
    for (int i = 0; i < 5; ++i)
 //     sineD[i] = gam::SineD<>(300.0 + (213.0 * i) , 0.9, 0.3);
      sineD[i] = gam::SineD<>(300.0 + (113.0 * i) , 0.9, 0.3); 
		//defaults
		bReset = 1;
		vel = 3.99; 
		equiAmt = .5;
		fieldAmt = .5;
		
		numDipoles = 5;
		dp = new vsr::Vec[numDipoles]; 
		dpdir = new vsr::Vec[numDipoles];
		dls = new Dls[numDipoles];
		par = new Par[numDipoles]; 
	   
		//INITIALIZE RANDOM WEIGHTS  	
		numParticles = f.num();
		Rand::Seed(10);
		s = new float[ numParticles ];
		for (int i = 0; i < numParticles; ++i){
			s[i] = 2. + Rand::Num();
		}  

		AppR2T :: init();  		
	    addListener(GetPositions, "/xy", "ffffffff", this);  
		addListener(GetVelocity, "/vel", "f", this);
	    addListener(GetReset, "/reset", "f", this);  
		addListener(GetTouch, "/touch", "iiii", this);
		addListener(GetCam, "/cam", "f", this);  
		
		 ow = - ( width * 2)/ 2.0; 
		 tw = width * 4;    
		
		 oh = - height/ 2.0; 
		 th = height; 
		
		//push field back away from screen a bit
		// for (int i = 0; i < vf.num(); ++i){
		// 	vf.grid(i) = vf.grid(i).trs(0,0,-3);
		// } 
		
		field = new MBO( Mesh::Points2( &( vf.grid(0) ), vf.dataPtr(), vf.num() ).mode(GL::L), GL::DYNAMIC );
		for (int i = 0; i < vf.num(); ++i){ 
			field -> mesh[i*2].Col.set(0,0,1,1); 
			field -> mesh[i*2+1].Col.set(1,0,1,.5);  
		 }    
		
		numPotentials = 10;    
		potentials = new MBO[numPotentials];
		for (int i = 0; i < numPotentials; ++i){  
			float t = 1.0 * i/ numPotentials;
			potentials[i]  = MBO( Mesh::Circle(1).color(0,1,0).mode(GL::LS) );
		}  
		
	}   
	
	~MyApp(){}   
	
	virtual void onSound( gam::AudioIOData& io ){
    while (io()) {
      float f = 0;
      for (int i = 0; i < 5; ++i)
        f += sineD[i]();
      f /= 5;
      f += delay(f + delay()*0.4);
      io.out(0) = io.out(1) = f;
    }
	}
	
	// virtual void onLoop(float dt) {
	// 
	//     static Timer frameTimer(1.0f / 30);
	//     if (frameTimer(dt)) {
	//       stopWatch.start();
	//       onFrame();
	//       stopWatch.stop();
	//     }
	// 
	//     static Timer reportTimer(1.0f);
	//     if (reportTimer(dt))
	//       stopWatch.report();  
	//   } 
	
	void updateMeshes(){ 
		for (int i = 0; i < vf.num(); ++i){  
			field -> mesh[i*2+1].Pos = Vec3f( vf.grid(i) ) + Vec3f( vf[i] ) ; 
		 } 
		field -> update();  
		
		if (bReset){
			for (int i = 0 ;i < f.num(); ++i){
				f[i] = ( vsr::Vec( f.grid(i) ) + vsr::Vec(Rand::Num(), Rand::Num(), 0 ) ).null();
			}
		}  
		
		for (int i = 0; i < numPotentials; ++i){
			float t = (-1.0 + 2.0 * i/numPotentials) * 40;
			vsr::Vec nv(t, - height/2.0, 0);  //starting point
			
			for (int j = 0; j < potentials[i].mesh.num(); ++j ){
				float tt = 1.0 * j/potentials[i].mesh.num();
				potentials[i].mesh[j].Pos.set( nv[0], nv[1], nv[2] );
			    potentials[i].mesh[j].Col.set( 0,  1,  t * .3, .9); 
				nv += orth.euler2d( nv ); 
			}  
			potentials[i].update();
		}  
	}   
	
	virtual void drawScene(){  
 
   		updateMeshes();
		
		static float time = 0.0; time += .05;
		static Rot ninetydegrees = Gen::rot( Biv::xy * PIOVERFOUR );  
			
  //  		Vec tally;
			
			for (int i = 0; i < f.num(); ++i){   
				  Par tpar;
			   	for (int j = 0; j < numDipoles; ++j ){ 
					float sqd =  ( Ro::sqd( f[i], dls[j] ) +1. );
					float dist = 1.0 / sqd;  
					tpar += par[j] * dist;
				 }
				f[i] = Ro::loc( f[i].sp(  Gen::bst( tpar  * s[i] * vel)  )  );  
 //   			tally += f[i];  
			 } 
			
			for (int i = 0; i < vf.num(); ++i){
				Par tpar;
				for (int j = 0; j < numDipoles; ++j ){ 
					float sqd =  ( Ro::sqd( vf.grid(i), dls[j] ) +1. );
					float dist = 1.0 / sqd;  
					tpar += par[j] * dist;
				 }
				Pnt np = Ro::loc( vf.grid(i).sp ( Gen::bst( tpar  )  ) );
				vf[i] = vsr::Vec(np - vf.grid(i)) * fieldAmt; 
			    orth[i] = vf[i].sp( ninetydegrees );
			}
					   
//		    tally /= f.num(); 

			
			for (int j = 0; j < numDipoles; ++j ){ 
				float t = 1.0 * j/numDipoles;
				dls[j] = Ro::dls( dp[j], .2 + .3 * t);
				par[j] = Ro::par( dls[j],  dpdir[j] );//vsr::Vec::y );// * (j&1?-1:1) ) * .1; 
		   } 	            
		 
		
		Mat4f mvm = scene.xf.modelViewMatrixf(); 
		
    	Render( f, mvm, pipe ); 
		
		//Flow Field Lines
		pipe.line( *field );   
		
		//equipotentials
		for (int i = 0; i < numPotentials; i++){ 
			pipe.line( potentials[i] );
		 }

		Render( vf, mvm, pipe );
			
		for (int j = 0; j < numDipoles; ++j){  
			Render( dls[j], mvm, pipe );
		}   
	}  
	
	
	
	static int GetPositions(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
	
		MyApp& app = *(MyApp*)user_data;  
		
		for (int i = 0; i < app.numDipoles; ++i){ 
			float x =  argv[i*2]->f; 
			float y =  argv[i*2+1]->f; 
			app.dp[i] = vsr::Vec( app.ow + app.tw*x, app.oh + app.th*(1-y),0);
			//cout << " x " << x << " y " << y << endl;
		 }  
		  
	    
	    return 0;
	} 
	
	static int GetVelocity( const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
	
		MyApp& app = *(MyApp*)user_data;    
		
		app.vel = argv[0] -> f;
		
	}
	
	static int GetReset( const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
	
		MyApp& app = *(MyApp*)user_data;    
		
		app.bReset = argv[0] -> f;
		
	} 

	static int GetCam( const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
	
		MyApp& app = *(MyApp*)user_data;    
		
		app.cam = argv[0] -> f;
		
		app.initView(app.width,app.height,app.cam);
		
	}
	
	static int GetTouch( const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) { 
	  
		MyApp& app = *(MyApp*)user_data;
		//for (int i = 0; i < 4; ++i){ 

    if (argv[0]->i < app.numDipoles) {
			int idx = argv[0]->i; 
			float x =  ( argv[1]->i ) / 3000.0; 
			float y =  ( argv[2]->i ) / 2500.0; 

		    app.dpdir[idx] = vsr::Vec::y.sp( Gen::rot( Biv::xy * ( argv[3]->i ) / 32.0 ) ); // this is the orientation (-32, 32)
			
			app.dp[idx] = vsr::Vec( (app.tw / 2.0 )*x, ( app.th /2.0) * (-y),0); 

      app.sineD[argv[0]->i].set((app.sineD[argv[0]->i].freq() + (argv[3]->i) / 8.0f), 0.9f, 0.3f);
    }
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
 		usleep(1666);
 	}   
	
	  return 0; 
}
