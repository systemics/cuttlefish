#include "ctl_appR2T.h"
#include "vsr/vsr.h" 
#include "vsr/vsr_field.h"
//#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public AppR2T {     
	 
	Field<Vec> vf;  	/// THE VECTOR FIELD
	Field<Vec> orth; 	/// THE ORTHOGONAL VECTOR FIELD
	
	int numDipoles;  	/// NUMBER OF DIPOLES TO CALCULATE
	Pnt * touch; 	    /// AN ARRAY OF TOUCHES 
	Par * par;   		/// DIPOLES THEMSELVES    
	
	int numPotentials;
	MBO * potentials;  
	MBO * potentialsB; 
	
	Vec left, right; /// Left and Right Speaker positions in absolute space  
	
    //a time element
	float time;
	
	MyApp() : AppR2T( Layout(1,4) ), 
		time(0),
		vf(20,10,1),
		orth(20,10,1),
		numDipoles(5),
		numPotentials(10) 
	
	{ 
		AppR2T::init(); 

		touch = new Pnt[numDipoles]; 
		par = new Par[numDipoles];  
		potentials = new MBO[numPotentials]; 
		potentialsB = new MBO[numPotentials];  		
		for (int i = 0; i < numPotentials; ++i){  
			 potentials[i]  = MBO( Mesh::Contour(50).color(0,1,0).mode(GL::LS) );    
			 potentialsB[i]  = MBO( Mesh::Contour(50).color(0,1,0).mode(GL::LS) );    
		}
		
		//vf.spacing( scene.width  / vf.width() )  
		
		Vec left( layout.left( identifier.row, identifier.col )  );  
	}	 
	
	//Initialize
	void initMeshes(){}  
	
	//Update the Geometry
	void update(){   
		                    
		time += .05; 
		
		calcDipoles();
		
		calcVecFields();  
		
		calcEquipotentials();
	}  
	
	//get field info
	void pollData(){
		
	}
	
	void calcDipoles(){
		for (int i = 0; i < numDipoles; ++i){
			float t = 1.0 * i/numDipoles;
			touch[i] = Point( sin(time*t) * layout.screenWidth, t * layout.screenHeight, 0 );
			par[i] = Par(Tnv(0,1,0)).trs( touch[i] ); 
		}
	}  
	
	void calcVecFields(){
		 for (int i = 0; i < vf.num(); ++i){
				static Par tpar; 
				tpar.reset();
				
				//sum in respective dipoles
				for (int j = 0; j < numDipoles; ++j ){ 
					float sqd =  ( Ro::sqd( vf.grid(i), touch[j] ) + .1 );
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
		
		for (int i = 0; i < 10; ++i){  
			float theta = (TWOPI * i/numPotentials);
			float ta = cos( theta ) * orth.tw()/2.0;  
			float tb = sin( theta ) * orth.th()/2.0;
			Vec nv(ta, tb, 0);  //starting point  
			Vec nvB(ta, tb, 0);  //starting point  
			
			for (int j = 0; j < potentials[i].mesh.num(); ++j ){
				//float tt = 1.0 * j/potentials[i].mesh.num();
				potentials[i].mesh[j].Pos.set( nv[0], nv[1], nv[2] );
			    //potentials[i].mesh[j].Col.set( 0,  1,  ta * .3, .9); 
				potentialsB[i].mesh[j].Pos.set( nvB[0], nvB[1], nvB[2] );
			    //potentialsB[i].mesh[j].Col.set( 0,  1,  ta * .3, .9); 
				nvB -= orth.euler2d( nvB ) * .2;
				nv += orth.euler2d( nv ) * .2; 
			}        
			
			potentials[i].update(); 
			potentialsB[i].update();
		}
	}
	
    //DRAW GEOMETRY TO SCREEN  
	 virtual void drawScene(){	
 
          
	   // DRAWCOLOR( vf, 1,1,0 );
		//DRAWCOLOR( orth, 0,1,0);
		
		for (int i = 0; i < 5; ++i){
			pipe.line( potentials[i] );
			pipe.line( potentialsB[i] );
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
 		//usleep(1666);
 	}   
	
	  return 0; 
}