#include "ctl_app.h"
#include "gfx/gfx_mbo.h"

#include "Gamma/Access.h"
#include "Gamma/AudioIO.h"
#include "Gamma/Delay.h"
#include "Gamma/DFT.h"
#include "Gamma/Noise.h"
#include "Gamma/Oscillator.h"
#include "Gamma/Print.h"
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
  
  float time;    
  
  float mix[2];       ///<-- Left/Right Sound Mix  
              
  //INSTANTIATE THE SINGLE MONITOR APPLICATION WITH WIDTH AND HEIGHT IN INCHES

