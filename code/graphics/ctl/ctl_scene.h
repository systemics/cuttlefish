#ifndef CTL_SCENE_H
#define CTL_SCENE_H

#include "ctl_gfxmatrix.h"

namespace ctl {
	
   struct Scene {
            
            Scene() {}
            
            XformMat xf;  

            struct Camera{
				
				struct Lens {
					Lens() : mFocal(60.), mWidth(1920), mHeight(1080), mNear(0), mFar(100) {}
					double mFocal, mWidth, mHeight, mNear, mFar;
				} mLens;
				
				Camera() : mX(1,0,0), mY(0,1,0), mZ(0,0,1), mPos(0,0,5) {}

				Vec3f mX, mY, mZ, mPos;
				
				Vec3f x() { return mX; }
				Vec3f y() { return mY; }
				Vec3f z() { return mZ; }
				Vec3f pos() { return mPos; }
				Lens lens() const { return mLens; }
				Lens& lens() { return mLens; }
				
			} camera;
		
            
            // float height() const { return camera.height(); }
            // float width() const { return camera.width(); }
            
            // Rot cat() { return XMat::identity(); }//camera.rot() * model.rot(); }//camera.rot() * model.rot(); }
            
            Mat4f mod() { return XMat::identity(); }//model.image(); }

			Mat4f mvm() { return XMat::identity(); }//XMat::lookAt( camera.x(), camera.y(), camera.z() * -1, camera.pos() ) * XMat::rot( model.rot() ) ; }
            
			Mat4f proj() { 
                Camera::Lens& tl = camera.lens();
                return XMat::fovy( tl.mFocal * PI/180.0, tl.mWidth/tl.mHeight, tl.mNear, tl.mFar ); 
            }
            
			Mat4f norm(){
                return (!mvm().transpose());
            }
            
            //ADVANCED MODE -> Update Shader Uniforms
            void updateMatrices(){
            
                Mat4f tmod = mod();
                Mat4f tview = XMat::lookAt( camera.x(), camera.y(), camera.z() * -1, camera.pos());
                Mat4f tmvm = mvm();
                Mat4f tproj = proj();
                Mat4f tnorm = norm();
                
                copy(tmod.val(), tmod.val() + 16, xf.model);
                copy(tview.val(), tview.val() + 16, xf.view);
                copy(tmvm.val(), tmvm.val() + 16, xf.modelView);
                copy(tproj.val(), tproj.val() + 16, xf.proj);
                copy(tnorm.val(), tnorm.val() + 16, xf.normal);
             
                xf.toDoubles();
            }

			void onFrame(){
				updateMatrices();
			}	
    
};

} //ctl::

#endif