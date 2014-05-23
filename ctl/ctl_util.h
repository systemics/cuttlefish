

#ifndef CTL_UTIL_INCLUDED
#define CTL_UTIL_INCLUDED

//#include "Rot.h"
#include "gfx/gfx_matrix.h"

namespace vsr {
	
  template<typename T>
	inline Quat Rot2Quat(const T& r){
        return Quat(r[0], -r[3], r[2], r[1]);
    }   

	/* inline vsr::Vec Vec2Vec(const Vec3f& v){ */
	/* 	return vsr::Vec(v[0], v[1], v[2]); */
	/* } */
}

#endif
