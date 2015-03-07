/*
 * =====================================================================================
 *
 *       Filename:  xWaves.cpp
 *
 *    Description:  waves
 *
 *        Version:  1.0
 *        Created:  06/03/2014 16:52:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ctl_app.h"
#include "vsr/vsr_cga2D_types.h"

using namespace vsr;
using namespace vsr::cga2D;
using namespace gfx;
using namespace std;
using namespace ctl; 

#define MAXWAVES 10

struct Wavefront {

    Point cir;
    Point pnt;

    bool isRef[4]   = {0,0,0,0};
    bool hasRef[4]  = {0,0,0,0};
    bool calcRef[4] = {1,1,1,1};

    Wavefront( const Point& p = Ro::null(0,0), float rad = .2) : 
    pnt(p), cir( Ro::dls(p, rad) )
    {}

    Wavefront * reflection[4];

    ~Wavefront(){
        for (int i =0; i<4; ++i){
          delete[] reflection[i];
        }
      }
    
    void propagate(float amt = 1.8){
      float rad = Ro::size(cir,true);
      cir = cir.dilate(pnt, log(amt)/rad);
    }

    bool sideOf( const Dll& dll ){
       auto p = Ro::cen( cir );
       auto s = p <= dll;
       return ( s[0] > 0 );
    }
};


struct Boundary {

    Line top, bottom, left, right;
    Pair ptop, pbottom, pleft, pright;
    Point stop, sbottom, sleft, sright;

    vector<DualLine> vline; 
    vector<Pair> vpair;
    vector<Point> vpoint;

    
    Boundary( float w, float h) {
      Point tl = Vec(-w/2.0,-h/2.0).null(); 
      Point tr = Vec(w/2.0,-h/2.0).null(); 
      Point bl = Vec(-w/2.0,h/2.0).null(); 
      Point br = Vec(w/2.0,h/2.0).null(); 

      init(tl,tr,bl,br);
    }

    void init( const Point& tl, const Point& tr, const Point& bl, const Point& br){
        top = tl^tr^Inf(1);
        right = tr^br^Inf(1);
        bottom = br^bl^Inf(1);
        left = bl^tl^Inf(1);

        ptop = tl ^ tr;
        pright = tr ^ br;
        pbottom = br ^ bl;
        pleft = bl ^ tl;

        stop = Ro::sur( ptop );
        sright = Ro::sur( pright);
        sbottom = Ro::sur( pbottom);
        sleft = Ro::sur( pleft );

        vline.push_back(top.dual());
        vline.push_back(right.dual());
        vline.push_back(bottom.dual());
        vline.push_back(left.dual());

        vpair.push_back(ptop);
        vpair.push_back(pright);
        vpair.push_back(pbottom);
        vpair.push_back(pleft);

        vpoint.push_back(stop);
        vpoint.push_back(sright);
        vpoint.push_back(sbottom);
        vpoint.push_back(sleft);

    }
        
};

struct WaveField {

  vector <Wavefront*> waves;
  Boundary box;

  WaveField(float w, float h) : box(w,h) {}

  virtual void update(){         
          
    if (!waves.empty()) waves[0] -> propagate();

    int numWaves = waves.size();

   // cout << numWaves << endl;

    for (int i=0;i<numWaves;++i){

      Wavefront& w = *waves[i];
      
      for (int j=0;j<4;++j){
                
        if ( waves.size() < MAXWAVES ){
          if (w.calcRef[j]){
            
            auto meet = (w.cir ^ box.vline[j]).dual(); 
            
            if ( Ro::size(meet,true) < .02 ) {

              auto split = Ro::split(meet);
              auto tc = split[0];
              auto sa = tc <= box.vpoint[j];  
              auto sb = split[1] <= box.vpoint[j];   

              if (sa[0] <= 0){

                //NEW WAVE
                Wavefront * nw = new Wavefront(tc);
                for (auto& dll : box.vline){
                  nw -> calcRef[j] = nw->sideOf(dll);
                }
                nw -> calcRef[j] = 0;

                
                w.calcRef[j] = 0;
                w.hasRef[j] = 1;
                waves.push_back( nw );
                w.reflection[j] = nw;             
             }
            }
          }
        }
     
        if (w.hasRef[j]) {
            w.reflection[j] -> cir = w.cir.re( box.vline[j] );
        }
      }
    }
  }


};




struct MyApp : public App {
        
   WaveField wavefield;

   MBO * circle;
  
  //INSTANTIATE THE APPLICATION WITH WIDTH AND HEIGHT
  MyApp() : App( Layout(4,4) ),
  wavefield( layout.screenWidth, layout.screenHeight)
  { 
    init();
  }                                  
  
  //INIITIALIZE ELEMENTS OF THE SCENE 
  virtual void init(){

     wavefield.waves.push_back( new Wavefront() );

     circle = new MBO( Mesh::Circle() );

  }  
  
  //UPDATE() CHANGES THE POSITION OF THE CIRCLE
  virtual void update(){
     
    static float time = 0; time +=.01;
    wavefield.update();
     // for (int i=0;i<wavefield.waves.size();++i){
     //   state.wave[i].cir = wavefield.waves[i] -> cir;
     //   state.wave[i].exists = true;
     // }
  }

  //THIS DRAWS THE CIRCLE TO THE SCREEN
  virtual void onDraw(){ 
    pipe.begin(*circle);    
   // for (int i=0;i<MAXWAVES;++i){
     for (int i=0;i<wavefield.waves.size();++i){

            Point& w = wavefield.waves[i] -> cir;
          //if (w.exists){         
            float scale = Ro::rad( w );
            Vec pos = Ro::cen( w );
            modelview( gfx::XMat::scale(scale) );
            pipe.draw(*circle); 
         // }
    }
    pipe.end(*circle);
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


