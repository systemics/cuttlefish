#ifndef CTL_SCENE_H
#define CTL_SCENE_H

#include "ctl_gfxmatrix.h"

#include <iostream>

using namespace std;

namespace ctl {
	
	struct Pose {
		Pose(Vec3f p, Quat q = Quat(1,0,0,0)) : mPos(p), mQuat(q) { orient(); }
		Pose(float x, float y, float z) : mPos(x,y,z), mQuat(1,0,0,0) { orient(); }
		Pose() : mX(1,0,0), mY(0,1,0), mZ(0,0,1), mPos(0,0,0), mQuat(1,0,0,0) {}
		Vec3f mX, mY, mZ, mPos;
		Quat mQuat;
		
		Quat quat() const { return mQuat; }
		Quat & quat() { return mQuat; }
		
		Pose& quat( Quat q ) { mQuat = q; return orient(); }

		Pose& orient() {
		    mX = Quat::spin( Vec3f(1,0,0), mQuat);
		    mY = Quat::spin( Vec3f(0,1,0), mQuat);
		    mZ = Quat::spin( Vec3f(0,0,1), mQuat);			
			return *this;
		}

		Vec3f px()  const{ return mPos + mX; }
		Vec3f py()  const{ return mPos + mY; }
		Vec3f pz()  const{ return mPos + mZ; }
		
		Vec3f x()  const{ return mX; }
		Vec3f y()  const{ return mY; }
		Vec3f z()  const{ return mZ; }
		
 		Vec3f& x() { return mX; }
		Vec3f& y() { return mY; }
		Vec3f& z() { return mZ; }
		// Vec3f x()  const{ mX = Quat::spin( Vec3f(1,0,0), mQuat); return mX; }
		// Vec3f y()  const{ mY = Quat::spin( Vec3f(0,1,0), mQuat); return mY; }
		// Vec3f z()  const{ mZ = Quat::spin( Vec3f(0,0,1), mQuat); return mZ; }
 
		


		Vec3f pos()  const{ return mPos; }
		Vec3f& pos() { return mPos; }
	};



	//Distances of Viewport from Eye Position
struct View {

	View(double _l, double _t, double _r, double _b) : l(_l), t(_t), r(_r), b(_b) {}

	// View (Vec3f ta, Vec3f tb, Vec3f tc ) {
	// 	Vec3f x = tb - ta;
	// 	Vec3f u = tc - ta;
	// 	l = x.dot(ta);  //*n/d;
	// 	r = x.dot(tb);  //*n/d;
	// 	b = u.dot(ta);
	// 	t = u.dot(tc);
	// }
	
	View (){}
	
	//Views for multiscreen environments will typically have same eye
	View( Vec3f eye, const Pose& p, float aspect, float height = 1.0  ){
		Vec3f br = p.pos() + p.x() * height * aspect; 
		Vec3f tl = p.pos() + p.y() * height ;
		set( eye, p.pos(), br, tl, p.z() );	
	}
	
	//Views for multiscreen environments will typically have same eye -- center
	// View( int w, int h, Vec3f eye, float screenAspect, float screenHeight  ){
	// 	Vec3f xoff = c.x() * screenHeight/2.0 * aspect;
	// 	Vec3f yoff = c.y() * height/2.0;
	// 	Vec3f bl = c.pos() - xoff - yoff; 
	// 	Vec3f br = c.pos() + xoff - yoff;
	// 	Vec3f tl = c.pos() + c.y() * height ;
	// 	set( eye, bl, br, tl, c.z() );	
	// }
	
	View ( Vec3f eye, Vec3f bl, Vec3f br, Vec3f tl, Vec3f normal  = Vec3f(0,0,1) ) {
		set( eye,bl,br,tl,normal );
	}	

	View& set ( Vec3f eye, Vec3f bl, Vec3f br, Vec3f tl, Vec3f normal = Vec3f(0,0,1) ) {

		Vec3f ta = bl - eye; Vec3f tb = (br - eye); Vec3f tc = (tl -eye);
		float d =  -(normal.dot(ta));
//		cout << "DIST: "<< d << endl; 
		Vec3f x = (tb - ta).unit();
		Vec3f u = (tc - ta).unit();
		// cout << ta << tb << tc << endl; 
		l = x.dot(ta) / d;  //*n/d;
		r = x.dot(tb) / d;  //*n/d;
		b = u.dot(ta) / d;
		t = u.dot(tc) / d;


		cout << "View INIT\n DIST: " << d << "\n" << ta << tb << tc << endl; 

		return *this;
	}			

	float l, t, r, b;
//	Vec3f eye, va, vb, vc;
};

struct Lens {
	Lens() : mFocal(60.), mWidth(1600), mHeight(1200), mNear(0.1), mFar(100.0) {
		
		
	}
	double mFocal, mWidth, mHeight, mNear, mFar;
	double aspect() { return mWidth / mHeight; }
//	View mView;
};

struct Camera {
		
		Vec3f mX, mY, mZ, mPos;
		
		View mView;
		Lens mLens; // Lens Data, Aspect Ratio, Etc.

		Camera() : mX(1,0,0), mY(0,1,0), mZ(0,0,1), mPos(0,0,1) {
            
			//Pose pose(-4, -3, 0);
			//mView = View( mPos, pose, mLens.aspect(), 6.0 );

		}


		Vec3f x() { return mX; }
		Vec3f y() { return mY; }
		Vec3f z() { return mZ; }
		Vec3f pos() const { return mPos; }    
		
		Vec3f& pos() { return mPos; }
		Lens lens() const { return mLens; }
		Lens& lens() { return mLens; }
		View view() const { return mView; }
		View& view() { return mView; }

};

	
   struct Scene {
            
            Scene() {}
            
            XformMat xf;  
			Pose model;
			

 			Camera camera;
		
            
            // float height() const { return camera.height(); }
            // float width() const { return camera.width(); }
            
            // Rot cat() { return XMat::identity(); }//camera.rot() * model.rot(); }//camera.rot() * model.rot(); }
            
            Mat4f mod() { return XMat::rot( model.quat() ); }

		 
		//	Mat4f mvm() { return XMat::trans(0,0,-2); }// XMat::identity() * mod(); }
			Mat4f mvm() { 
			//	cout << camera.pos() << endl; 
				return XMat::lookAt( camera.x(), camera.y(), camera.z(), camera.pos() ) * mod(); 
			}

			 // Mat4f proj() { return XMat::identity(); }
            
			Mat4f proj() {    
				Lens& tl = camera.lens();
                return XMat::fovy( tl.mFocal * PI/180.0, tl.mWidth/tl.mHeight, tl.mNear, tl.mFar ); 
	         }
	
			Mat4f frust() {
				View& v = camera.view();
				Lens& tl = camera.lens();
                // return XMat::frustum2( v.l, v.r, v.b, v.t, tl.mNear, tl.mFar ); 
                return XMat::frustum2( v.l * tl.mNear, v.r*tl.mNear, v.b*tl.mNear, v.t*tl.mNear, tl.mNear, tl.mFar ); 
  	         }
            
			Mat4f norm(){
                return (!(mvm().transpose()) );
            }
            
            //ADVANCED MODE -> Update Shader Uniforms
            void updateMatrices(){
            
                // Mat4f tmod = mod();
                // Mat4f tview = XMat::lookAt( camera.x(), camera.y(), camera.z() * -1, camera.pos());
				Mat4f tmvm =  mvm();     //     XMat::identity();//
                // Mat4f tproj = proj();
                Mat4f tproj = frust();     //XMat::identity();   // 
                Mat4f tnorm = norm();
                 
			  //  cout << tmvm << endl; 
                // copy(tmod.val(), tmod.val() + 16, xf.model);
                // copy(tview.val(), tview.val() + 16, xf.view);

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