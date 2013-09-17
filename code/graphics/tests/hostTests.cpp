#include "cf_app.h"
 
using namespace ctl;

struct MyApp : public App {
     
	MyApp() : App( Layout(1,1) ) {}

   virtual void init() {}
   virtual void update() {}
   virtual void onDraw() {}
 
};

STARTANDRUN()