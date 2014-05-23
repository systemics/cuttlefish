
#include "ctl_app.h"

#include "vsr/vsr_cga3D_op.h"
#include "vsr/vsr_field.h"
#include "vsr/vsr_render.h"


using namespace std;
using namespace ctl; 
using namespace vsr;

struct MyApp : public App {     
   
  Field<Vec> vf;        ///< THE VECTOR FIELD
  Field<Vec> orth;      ///< THE ORTHOGONAL VECTOR FIELD

  MBO vfmesh;           ///< Field Mesh Buffer Object
  MBO orthmesh;         ///< Field Mesh Buffer Object 
  
  int numDipoles;       ///< NUMBER OF DIPOLES TO CALCULATE
  Point * dipole;       ///< AN ARRAY OF DIPOLE LOCATIONS 
  Pair * par;           ///< THE DIPOLES THEMSELVES    
  
  int numPotentials;    ///< NUMBER OF CONTOUR LINES
  int pRes;             ///< RESOLUTION OF CONTOUR LINES

  MBO * potentialA;     ///< Pointer to Contour Line Meshes (clockwise)
  MBO * potentialB;     ///< Pointer to Contour Line Meshes (counterclockwise)

  float time;           ///< A Time Counter
  
  MyApp() : App( 21.5, 14.5 ),
    time(0),
    vf(20,10,1),
    orth(20,10,1),
    numDipoles(2),
    numPotentials(5),
    pRes(20)
  
  { 

    dipole = new Pnt[numDipoles]; 
    par = new Par[numDipoles];  
    
    potentialA  =  new MBO[numPotentials];     
    potentialB  =  new MBO[numPotentials];
    
    for (int i = 0; i < numPotentials; ++i){
      float col= (float)i/numPotentials;
      potentialA[i] = Mesh::Contour(pRes).color(1-col,1,col).mode(GL::LS);     
      potentialB[i] = Mesh::Contour(pRes).color(col,1,1-col).mode(GL::LS);  
    }

    vfmesh = MakeMeshBuffer( vf );
    orthmesh = MakeMeshBuffer( orth );

  }   

  
  //Update the Geometry
  void update(){   
                        
    time += .05; 
    
    calcDipoles();
    
    calcVecFields();  
    
    calcEquipotentials();

  }  
  
  //get field info
  void pollData(){}
  
  void calcDipoles(){
    for (int i = 0; i < numDipoles; ++i){
      float t = 1.0 * i/numDipoles;
      dipole[i] = Ro::point( sin(time+t*2) * layout.screenWidth/3.0, cos(time + t *2 ) * layout.screenHeight/3.0, 0 );
      par[i] = Par(Tnv(0,1,0)).translate( dipole[i] ); 
    }
  }  
  
  void calcVecFields(){
     for (int i = 0; i < vf.num(); ++i){
        static Par tpar; 
        tpar.reset();
        
        //sum in respective dipoles
        for (int j = 0; j < numDipoles; ++j ){ 
          float sqd =  ( Ro::sqd( vf.grid(i), dipole[j] ) + .1 );
          float dist = 1. / sqd;  
          tpar += par[j] * dist;
         }   
        
         // Vec np =  vf.grid(i).bst ( tpar );//Ro::loc( vf.grid(i).bst ( tpar ) );
        Vec np = Ro::loc( vf.grid(i).bst ( tpar * .6 ) );
        vf[i] = Vec( np - Vec(vf.grid(i)) ); 
        orth[i] = vf[i].rot( Biv::xy * PIOVERFOUR );
      }
  }  
  
  void calcEquipotentials(){
    
    for (int i = 0; i < numPotentials; ++i){    
      
      float it = (float)i / numPotentials;
      float theta = (TWOPI * it);
      float ta = cos( theta ) * orth.tw()/2.0;  
      float tb = sin( theta ) * orth.th()/2.0;

      //cout << ta << " " << tb << " " <<  orth.tw() <<  endl; 

      Vec nvA(ta, tb, 0);   //starting point clockwise  
      Vec nvB(ta, tb, 0);   //starting point counterclockwise
      
      for (int j = 0; j < pRes; ++j ){
        potentialA[i].mesh[j].Pos.set( nvA[0], nvA[1], nvA[2] );
        potentialB[i].mesh[j].Pos.set( nvB[0], nvB[1], nvB[2] );
        nvB -= orth.euler2d( nvB ) ;
        nvA += orth.euler2d( nvA ) ; 
      }        
      
      potentialA -> update(); 
      potentialB -> update();
    }
  }
  
   //DRAW GEOMETRY TO SCREEN  
   virtual void onDraw(){  
 
     for (int i = 0 ; i<numDipoles; ++i){
      Render( dipole[i], this );
     }

      Render( vf, vfmesh, this );
          
      for (int i = 0; i < numPotentials; ++i){
        pipe.line( potentialA[i] );
        pipe.line( potentialB[i] );
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
