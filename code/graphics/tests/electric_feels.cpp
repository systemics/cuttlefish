#include "vsr.h"
#include "vsr_op.h"
#include "vsr_xf.h" 
#include "vsr_stat.h"   
#include "vsr_field.h"

#include "ctl_util.h"
#include "ctl_render.h"
#include "ctl_appR2T.h"  

using namespace std;
using namespace ctl;
using namespace vsr;

struct MyApp : AppR2T {
   
	MBO * field;
 
    Field<Pnt> f;
	Field<Vec> vf;

	float * s;   
	   
	int numDipoles, numParticles; 
	
	float vel;

	vsr::Vec * dp;   
    vsr::Dls * dls;  
	vsr::Par * par; 
	
	bool bReset; 
    
	float ow, oh, tw, th;

	MyApp() : 
	AppR2T( 14. + 3./8., 10. + 11./16. ),
    f(40,7,1,2), vf(20,5,1,3)    
	{


		numDipoles = 4;
		dp = new Vec[numDipoles]; 
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
		addListener(GetTouch, "/touch", "iii", this);
		
		 ow = - ( width * 2)/ 2.0; 
		 tw = width * 4;    
		
		 oh = - height/ 2.0; 
		 th = height;  
		
		field = new MBO( Mesh::Points2( &( vf.grid(0) ), vf.dataPtr(), vf.num() ).mode(GL::L), GL::DYNAMIC );
		for (int i = 0; i < vf.num(); ++i){ 
			field -> mesh[i*2].Col.set(1,0,0,1); 
			field -> mesh[i*2+1].Col.set(1,0,1,.9);  
		 } 
	}   
	
	~MyApp(){}  
	
	void updateMeshes(){ 
		for (int i = 0; i < vf.num(); ++i){  
			field -> mesh[i*2+1].Pos = Vec3f( vf.grid(i) ) + Vec3f( vf[i] ) ; 
		 } 
		field -> update();  
		
		if (bReset){
			for (int i = 0 ;i < f.num(); ++i){
				f[i] = ( Vec( f.grid(i) ) + Vec(Rand::Num(), Rand::Num(), 0 ) ).null();
			}
		}
	}   
	
	virtual void drawScene(){  
 
   		updateMeshes();
		
		static float time = 0.0; time += .05;  
			
			Vec tally;
			
			for (int i = 0; i < f.num(); ++i){   
				  Par tpar;
			   	for (int j = 0; j < numDipoles; ++j ){ 
					float sqd =  ( Ro::sqd( f[i], dls[j] ) +1. );
					float dist = 1.0 / sqd;  
					tpar += par[j] * dist;
				 }
				f[i] = Ro::loc( f[i].sp(  Gen::bst( tpar  * s[i] * vel)  )  );  
				tally += f[i];  
			 } 
			
			for (int i = 0; i < vf.num(); ++i){
				Par tpar;
				for (int j = 0; j < numDipoles; ++j ){ 
					float sqd =  ( Ro::sqd( vf.grid(i), dls[j] ) +1. );
					float dist = 1.0 / sqd;  
					tpar += par[j] * dist;
				 }
				Pnt np = Ro::loc( vf.grid(i).sp ( Gen::bst( tpar * vel )  ) );
				vf[i] = np - vf.grid(i);
			}
					   
			// Average particle position is used as Camera Target
		    tally /= f.num(); 
			//cout << tally << endl;  
			// Vec tp( scene.camera.pos()[0], scene.camera.pos()[1], scene.camera.pos()[2] );  
			// Vec goal = ( tally - tp ).unit();    
			// Rot rot  = Gen::ratio( -Vec::z, goal ); 
			// 
			// scene.camera.quat() = Rot2Quat(rot); 		
			// scene.camera.orient();   

			// Oscillation of the Dipoles Position  		   
			// for (int j = 0; j < numDipoles; ++j ){     
			// 	float t = 1.0 * j/numDipoles;
			// 	dp[j] = tally + ( Vec::x * width * sin(time) * (j&1? -1 : 1) );
			// 	dls[j] = Ro::dls( dp[j], .2 + .3 * t).trs (0, t * cos(time) * height, 0);
			// 	par[j] = Ro::par( dls[j], Vec::x * (j&1?-1:1) ) * .1; 
			// 		   } 
			
			for (int j = 0; j < numDipoles; ++j ){ 
				float t = 1.0 * j/numDipoles;
				dls[j] = Ro::dls( dp[j], .2 + .3 * t);
				par[j] = Ro::par( dls[j], Vec::y );// * (j&1?-1:1) ) * .1; 
		   } 	            
		 
		
		Mat4f mvm = scene.xf.modelViewMatrixf(); 
		
		Render( f, mvm, pipe ); 
		
		//Flow Field Lines
		pipe.line( *field );
		//Render( vf, mvm, pipe );
			
		for (int j = 0; j < numDipoles; ++j){  
			Render( dls[j], mvm, pipe );
		}  
	} 
	
	static int GetPositions(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) {
	
		MyApp& app = *(MyApp*)user_data;  
		
		for (int i = 0; i < app.numDipoles; ++i){ 
			float x =  argv[i*2]->f; 
			float y =  argv[i*2+1]->f; 
			app.dp[i] = Vec( app.ow + app.tw*x, app.oh + app.th*(1-y),0);
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
	
	static int GetTouch( const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data) { 
	  
		MyApp& app = *(MyApp*)user_data;
		for (int i = 0; i < 4; ++i){ 
			int idx = argv[0] -> i; 
			float x =  ( argv[1] -> i ) / 3000.0; 
			float y =  ( argv[2] -> i ) / 2500.0; 
			
			app.dp[idx] = Vec( (app.tw / 2.0 )*x, ( app.th /2.0) * (-y),0); 
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