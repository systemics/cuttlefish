//
//  berryrender.cpp
//  Versor on the Raspberry PI
//
//  Created by Pablo Colapinto on 1/24/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//


#include "ctl_egl.h"
#include "ctl_gl.h"

#include "ctl_gl_data.h"

#include <iostream>


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
        
            glClearColor(.2,.2,.2,1);
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
    
    MyWindow win;
    
    while(1){
        win.onFrame();
        sleep(1);
    }
    
    return 0;
    
}