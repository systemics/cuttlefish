/*
 * =====================================================================================
 *
 *       Filename:  Boundary.h
 *
 *    Description:  Versor Defined 2D Boundaries
 *
 *        Version:  1.0
 *        Created:  06/03/2014 10:56:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

#include "vsr/vsr_cga2D_types.h"

namespace vsr {

namespace cga2D{


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
} //cga2D::
} //vsr::
