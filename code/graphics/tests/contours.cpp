#include "ctl_appR2T.h"
#include "vsr/vsr.h" 
#include "vsr/vsr_field.h"
#include "gfx/gfx_mbo.h"

using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public AppR2T {     
	 
	Field<Vec> vf;  	/// THE VECTOR FIELD
	Field<Vec> orth; 	/// THE ORTHOGONAL VECTOR FIELD
	
	int numDipoles;  	/// NUMBER OF DIPOLES TO CALCULATE
	Pnt * touch; 	    /// AN ARRAY OF TOUCHES 
	Par * par;   		/// DIPOLES THEMSELVES    
	
	int numPotentials;   	/// NUMBER OF CONTOUR LINES
	int pRes;        	 /// RESOLUTION OF CONTOUR LINES
	MBO * potentialA;  
	MBO * potentialB;
    //a time element
	float time;
	
	MyApp() : AppR2T( 21.5, 14.5 ),//Layout(1,4) ), 
		time(0),
		vf(20,10,1),
		orth(20,10,1),
		numDipoles(5),
		numPotentials(3),
		pRes(50)
	
	{ 
		AppR2T::init(); 

		touch = new Pnt[numDipoles]; 
		par = new Par[numDipoles];  
		
		//ONE BUFFER FOR ALL CONTOURS
		potentialA  =  new MBO( Mesh::Contour(numPotentials * pRes).color(0,1,0).mode(GL::LS) );       
		potentialB  =  new MBO( Mesh::Contour(numPotentials * pRes).color(0,1,0).mode(GL::LS) );  
		
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
		
		for (int i = 0; i < numPotentials; ++i){    
			
			float theta = (TWOPI * i/numPotentials);
			float ta = cos( theta ) * orth.tw()/2.0;  
			float tb = sin( theta ) * orth.th()/2.0;
			Vec nv(ta, tb, 0);  //starting point  
			Vec nvB(ta, tb, 0);  //starting point  
			
			for (int j = 0; j < pRes; ++j ){
				//float tt = 1.0 * j/potentials[i].mesh.num(); 
				int idx = i * pRes + j;
				potentialA -> mesh[idx].Pos.set( nv[0], nv[1], nv[2] );
			    //potentials[i].mesh[j].Col.set( 0,  1,  ta * .3, .9); 
				potentialB -> mesh[idx].Pos.set( nvB[0], nvB[1], nvB[2] );
			    //potentialsB[i].mesh[j].Col.set( 0,  1,  ta * .3, .9); 
				nvB -= orth.euler2d( nvB ) * .2;
				nv += orth.euler2d( nv ) * .2; 
			}        
			
			potentialA -> update(); 
			potentialB -> update();
		}
	}
	
    //DRAW GEOMETRY TO SCREEN  
	 virtual void drawScene(){	
 
          
	   // DRAWCOLOR( vf, 1,1,0 );
		//DRAWCOLOR( orth, 0,1,0);
		
		MBO& mbo = *potentialA;
		
		for (int i = 0; i<2; ++i){
		
			mbo.bind();
			pipe.enable();    
		
				pipe.pointer();
				for (int i = 0; i < numPotentials; ++i){
					mbo.drawElements(pRes, i * numPotentials);
				}   
		       
			pipe.disable(); 
			mbo.unbind();  
		
			mbo = *potentialB;  
		
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