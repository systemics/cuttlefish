/*
 * =====================================================================================
 *
 *       Filename:  xWaves-cuttlebone.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/02/2014 17:45:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */


#include "vsr/vsr_cga2D_types.h"
#include "gfx/gfx_layout.h"

#include "ctl_bone.h"

using namespace vsr;
using namespace vsr::cga2D;
using namespace gfx;

#define MAXWAVES 10


struct Wave{
  Point cir;
  bool exists;
  Wave() : exists(false){}
};

struct Foo {
  Wave wave[MAXWAVES];  
};

#if SIM

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

    ~Wavefront(){}
    
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


struct MyApp : ctl::Simulator<Foo> {

  WaveField wavefield;
  Layout layout;

  MyApp() : Simulator<Foo>("192.168.7.255", 1 / 30.f),
  layout(4,4),
  wavefield(layout.totalWidth(), layout.totalHeight())
  {
    cout << layout.totalWidth() << " " << layout.totalHeight();
    //
    // shouldLog = true;
  }
  
  //INITIALIZE
  virtual void setup(Foo& state) { 
     wavefield.waves.push_back( new Wavefront() );
  }

  //UPDATE
  virtual void update(float dt, Foo& state) {
      wavefield.update();
      for (int i=0;i<wavefield.waves.size();++i){
        state.wave[i].cir = wavefield.waves[i] -> cir;
        state.wave[i].exists = true;
      }
  }
};

#else

#include "ctl_app.h"

using namespace ctl;
using namespace gfx;

struct MyApp : CuttleboneApp<Foo> {

  float v;
  
  MBO * circle;

  MyApp() : CuttleboneApp<Foo>(Layout(4, 4), 30.0) {
    // Sound::init(1024, 48000);
  }

  virtual void setup() {
      circle = new MBO( Mesh::Circle() );
  }

  virtual void update(float dt, Foo& state, int popCount) {
   // v = state.value - int(state.value);
  }

  virtual void onDraw() { 
    Foo& state = (*renderState);
    pipe.begin(*circle);    
    for (int i=0;i<MAXWAVES;++i){
          Wave& w = state.wave[i];
          if (w.exists){         
            float scale = Ro::rad( w.cir );
            Vec pos = Ro::cen( w.cir );
            modelview( gfx::XMat::translateScale2D( pos, scale) );
            pipe.draw(*circle); 
          }
    }
    pipe.end(*circle);
    
  }

  virtual void onSound(Sound::SoundData& io) {}
};

#endif

int main() { MyApp().start(); }
