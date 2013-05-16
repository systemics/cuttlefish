/*
 *  CAMERA.h
 *  Versor
 *
 *  Created by x on 5/17/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
 
 #ifndef CTL_CAMERA_H_INCLUDED
 #define CTL_CAMERA_H_INCLUDED

#include "ctl_gl.h"
#include "ctl_gfxmatrix.h"

//#include "vsr/vsr_frame.h"
 
namespace ctl {

    struct Slab{
        Vec blu, bru, tru, tlu;
        Vec& operator[] (int i ) { return (&blu)[i]; }
    };
    
    struct Volume{
        Vec bl, br, tr, tl, blb, brb, trb, tlb; 
        Vec& operator[] (int i ) { return (&bl)[i]; }
    };
    
struct Frustrum{
    float width, height, depth;
    float bwidth, bheight;
    
    Slab dir;
    Volume box;
    
    void calc(){
        dir.blu = (box.blb - box.bl).unit();
        dir.tru = (box.trb - box.tr).unit();
        dir.bru = (box.brb - box.br).unit();
        dir.tlu = (box.tlb - box.tl).unit();
        width = box.br[0] - box.bl[0];
        height = box.tr[1] - box.br[1];
        depth = box.blb[2] - box.bl[2];
        bwidth = box.brb[0] - box.blb[0];
        bheight =box.trb[1] - box.brb[1];
        
    }
    
    Pln left(){ return Ro::null(box.tl) ^ Ro::null(box.bl) ^ Ro::null(box.blb) ^ Inf(1); }
    Pln right(){ return Ro::null(box.tr) ^ Ro::null(box.br) ^ Ro::null(box.brb) ^ Inf(1); }
    Pln top(){ return Ro::null(box.tl) ^ Ro::null(box.tr) ^ Ro::null(box.trb) ^ Inf(1); }
    Pln bottom(){ return Ro::null(box.bl) ^ Ro::null(box.br) ^ Ro::null(box.brb) ^ Inf(1); }
};

struct Lens {

    
    /* Lens Parameters */
    bool bOrtho;
    float mFocal, mNear, mFar, mWidth, mHeight; 

    Lens() :
    mFocal(60.0), 
    mWidth(100), 
    mHeight(100), 
    mNear(0.1), 
    mFar (100.0),
    bOrtho(0)
    {}

    /* Implicit Getters and Setters */
    void width( float w )	{ mWidth = w;}						///< set width
    void height( float h )	{ mHeight = h;}						///< set height
    float ratio() const { return width()/ height(); }

    float width()	const {return mWidth;}							///< get width
    float height()	const {return mHeight;}							///< get height
    float depth()   const { return mFar - mNear; }
    float& width()	 {return mWidth;}							///< get width
    float& height()	 {return mHeight;}							///< get height
    
    float near() const { return mNear; }
    float far() const { return mFar; }
    float& near()  { return mNear; }
    float& far()  { return mFar; }
    
    void	focal(float f)	{ mFocal = f; }						///< set focal length
    float& focal()			{ return mFocal; }					///< get focal length
    float	focal() const	{ return mFocal; }
    
    void  ortho(bool b) { bOrtho = b; }
    bool& ortho()  {return bOrtho; }
    bool  ortho() const {return bOrtho; }
    
    friend ostream& operator << (ostream& os, const Lens& l){
        os << "focal: " << l.focal() << "\n";
        os << "width: " << l.width();
        os << " height: " << l.height() << "\n";
        os << "near: " << l.near();
        os << " far: " << l.far() << "\n";
        
        return os;
    }    

};
    
class Camera : public vsr::Frame {	

		using namespace vsr;
	
        Lens mLens;
        Frustrum mFrustrum;
		Vec mLook;													///< Direction to Look In
				
	public:
    
        Lens& lens() { return mLens; }
        
        Camera() : Frame(), mLook(0,0,0), mLens() {}
        Camera(float x, float y, float z); 
        Camera( Vec v, Drv x); 

		//template <class A> void lookAt( const A& v ) { mLook = v; }
						
		Vec  look() { mLook = Vec(mPos) + forward();  return mLook; }	
        
        //A Direction
        void lookAt(double x, double y, double z){ orientZrel(-x,-y,-z); }
        void lookAt(const Vec& v) { orientZrel(-v); }
        
        void focal( float f ) { mLens.focal( f ); }
        void width( float w )	{ mLens.width( w ) ;}						///< set width
        void height( float h )	{ mLens.height( h );}						///< set height
        float width()	const {return mLens.width();}							///< get width
        float height()	const {return mLens.height();}							///< get height
        float depth()   const { return mLens.mFar - mLens.mNear; }	
				
		void reset() {  pos(0,0,5); rot( Rot(1,0,0,0) ); } 
					
		// void push3D();
		// void pop3D();
		// 
		// //ortho
		// void push2D(int, int);
		// void pop2D();
		
    
    Frustrum frustrum() const { return mFrustrum; }
    
    Frustrum frustrum(){
                 
            mFrustrum.calc();
        
            return mFrustrum;

        }

};

 

    class Scene {
    
        public:
            
            Scene() : camera(0.0,0.0,5.0), model() {}
            
            XformMat xf;            
            Frame model;
            Camera camera;
            
            float height() const { return camera.height(); }
            float width() const { return camera.width(); }
            
            Rot cat() { return camera.rot() * model.rot(); }//camera.rot() * model.rot(); }
            
            Mat4f mod() { return model.image(); }
            Mat4f mvm() { return  XMat::lookAt( camera.x(), camera.y(), camera.z() * -1, camera.pos()) * XMat::rot( model.rot() ) ; }
            Mat4f proj() { 
                Lens& tl = camera.lens();
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
            
            // void pop3D();
            // void push3D();
            // 
            // //IMMEDIATE MODE ONLY (NO IPHONE, etc)
            // void getMatrices(){
            //     glGetDoublev(GL_PROJECTION_MATRIX, xf.projd);	
            //     glGetDoublev(GL_MODELVIEW_MATRIX, xf.modelViewd);
            //     glGetIntegerv(GL_VIEWPORT, xf.viewport);	
            //     
            //     xf.toFloats();
            // }
    
    };

} //vsr::

#endif
