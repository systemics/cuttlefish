//
//  berrygraphics.cpp
//  basic egl windowing on the raspberry pi
//  for more advanced use see berryrender
//  Created by Pablo Colapinto on 1/24/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "bcm_host.h"

#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"

#include "ctl_egl.h"

using namespace std;
using namespace ctl::EGL;


struct MyWindow : public Window {

        MyWindow() : Window () {
            initGL();
        }
        
        ~MyWindow(){}
        
        void initGL(){
             glClearColor(1,1,1,1);
             srand( time(NULL) );
        }
        
        virtual void onDraw(){
        
        }
        
        virtual void onFrame(){
        
            float r=(float)rand()/(float)RAND_MAX;
            float g=(float)rand()/(float)RAND_MAX;
            float b=(float)rand()/(float)RAND_MAX;
 
            glClearColor(r,g,b,1);
            // clear screen
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // to update we need to swap the buffers
            swapBuffers();
            onDraw();
        }

};

int main(){
    
    //HARDWARE INITIALIZE
    bcm_host_init();
    
    //Display display;
    //WSurface surface;
    //Config config(&display);
    
    MyWindow win;
    
    while(1){
        win.onFrame();
        sleep(1);
    }
    
    return 0;
    
}