// +-----------------------------------------------------------------------------+
// | Author: Seunghyeon (Hyeon) Kim                                              |
// | This header file contains code for Vex Houdini.                             |
// | It has the required functions for the Columns and Base Calculation          |
// +-----------------------------------------------------------------------------+

// +-----------------------------------------------------------------------------+
// | Import required modules                                                     |
// +-----------------------------------------------------------------------------+
#include "BasicUtil.h"

// +-----------------------------------------------------------------------------+
// | Main declarations                                                           |
// +-----------------------------------------------------------------------------+

#ifndef __COLUMNBASE_H__
#define __COLUMNBASE_H__

// +-----------------------------------------------------------------------------+
// | Function declarations                                                           |
// +-----------------------------------------------------------------------------+

/* create_norms(geometry, ptnum)
 * Pre-condition: valid geometry with required channels and a valid point number
 * Post-condition: The function will return the vector list with the normals of the point number calculated.
 * Comments: It will create an attribute called the radius.
 */
vector[] create_norms (int geometry, ptnum) {
    int numE = point(geometry, "numE", ptnum);
    vector norms[] = array();
    float radius = chf("../pillXZ_" + itoa(detail(geometry, "floor")) + "x")/2.0;
    addpointattrib(geometry, "radius", radius);
    float z_dir = chf("../pillXZ_" + itoa(detail(geometry, "floor")) + "y")/2.0;
    for (int i = 0; i < numE; i++) {
        vector norm = set(z_dir*cos(2*PI*i/numE), 0, radius*sin(2*PI*i/numE));
        append(norms, unitfy(norm));
    } // for
    return norms;
} // function

/* create_norms(geometry, ptnum)
 * Pre-condition: valid geometry with required channels and a valid point number
 * Post-condition: The function will return the vector list with vectors pointing from origin
 * to the midpoint of the edges of the potential surface.
 * Comments: It will create an attribute called the radius.
 */
vector[] create_otms (int geometry, ptnum) {
    int numE = point(geometry, "numE", ptnum);
    vector otms[] = array();
    float radius = chf("../pillXZ_" + itoa(detail(geometry, "floor")) + "x")/2.0;
    addpointattrib(geometry, "radius", radius);
    float z_dir = chf("../pillXZ_" + itoa(detail(geometry, "floor")) + "y")/2.0;
    for (int i = 0; i < numE; i++) {
        vector otm = set(radius*cos(2*PI*i/numE), 0, z_dir*sin(2*PI*i/numE));
        append(otms, otm);
    } // for
    return otms;
} // function

#endif