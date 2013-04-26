/*
 *  MATRIX.h
 *  gelatin
 *
 *  Created by x on 10/22/09.
 *  Copyright 2009 x. All rights reserved.
 *
 */
 
 /*!
	LIGHT-WEIGHT VECTORS AND MATRICES AND DATA STRUCTURES
	FOR PASSING AROUND WITH MEMBERWISE SHALLOW COPYS
 */

#ifndef CTL_RENDER_H_INCLUDED
#define CTL_RENDER_H_INCLUDED

#include "VSR/vsr.h"
#include "vsr_xf.h"
#include "vsr_field.h"

#include "ctl_matrix.h"
#include "ctl_mdraw.h"
#include "ctl_scene.h"
#include "ctl_gl_mbo.h"
#include "ctl_gl_shader.h"

namespace ctl {

	template<class A> void Render ( const A& a, const Mat4f& mvm, ShaderProgram& program ){ cout << "nothing to render this element\n" << endl;  }
	template<class A> void Render (  vsr::Field<A>& a, const Mat4f& mvm, ShaderProgram& program ){ cout << "nothing to render this field\n" << endl; }
	
	template<> void Render(const vsr::Cir& cir, const Mat4f& mvm, ShaderProgram& program )	{

		static MBO circle ( Mesh::Disc(.5) );
		static float mv[16];
		static Mat4f mat;
		static Mat4f tmp;
		
		mat = mvm * tmp.copy( vsr::Xf::mat(cir) );
		mat.fill(mv);
		program.uniform("modelView", mv );    

		GL::Pipe::Line( circle );                    
	}	
	
	template<> void Render(const vsr::Vec& vec, const Mat4f& mvm, ShaderProgram& program){
		static MBO cone ( Mesh::Cone(.1,.3) );
		static MBO line ( Mesh::Line( vsr::Vec(0,0,0),  vec), GL::DYNAMIC );
	
		static float mv[16];
		static Mat4f mat;
		static Mat4f tmp;
		
		Vertex vertex(vec[0], vec[1], vec[2]);
		
		line.update(1, 1, &vertex);
		
		mvm.fill(mv);
		program.uniform("modelView", mv );  
		GL::Pipe::Line( line );
		
		mat = mvm * tmp.copy( vsr::Xf::mat(vec) );
		mat.fill(mv);
		program.uniform("modelView", mv );  
		GL::Pipe::Line( cone );
	
	}
	
	template<> void Render(const vsr::Pnt& pnt, const Mat4f& mvm, ShaderProgram& program){
		
		static float mv[16];
		static Mat4f mat;
		static Mat4f tmp;
		static MBO sphere ( Mesh::Sphere() );
		static MBO point ( Mesh::Point( pnt ) );
		
	    double ta = vsr::Ro::size( pnt, true );

	    //Draw as dual Sphere (if |radius| > 0.000001);
	    if ( fabs(ta) >  FPERROR ) {
						
	        bool real = ta > 0 ? 1 : 0;	
			tmp.copy( vsr::Xf::mat( pnt, sqrt( fabs(ta) ) ) ); 

			mat = mvm * tmp;
			
			mat.fill(mv);
			program.uniform("modelView", mv);
			
			GL::Pipe::Line(sphere);

	        // (real) ? Glyph::SolidSphere(t, 5+ floor(t*30), 5+floor(t*30)) : Glyph::Sphere(t);	

	    } else {


			Vertex v(pnt[0], pnt[1], pnt[2]);

			//point.update(0,1, &v);
			GL::Pipe::Line (point);
	    }
		
	}
	
	// template<> void Render(const vsr::Dls& pnt, const Mat4f& mvm, ShaderProgram& program){
	// 	
	// 	static MBO point ( Mesh::Point( pnt ), GL::DYNAMIC );
	// 	
	// 	Vertex v(pnt[0], pnt[1], pnt[2]);
	// 	
	// 	//point.update(0,1, &v);
	// 	GL::Pipe::Line (point);
	// 	
	// }	
	
	
	template<> void Render(vsr::Field<vsr::Pnt>& f, const Mat4f& mvm, ShaderProgram& program ){
		static float mv[16];
		static Mat4f mat;
		static Mat4f tmp;
		static MBO points ( Mesh::Points( f.dataPtr(), f.num() ), GL::DYNAMIC );
		
		// points.mesh
		for (int i = 0; i < f.num(); ++i){
			points.mesh[i].Pos = Vec3f( f[i] );
		}
		points.update();
		GL::Pipe::Line( points );
		
	}
}

#endif