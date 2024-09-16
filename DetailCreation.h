// +-----------------------------------------------------------------------------+
// | Author: Seunghyeon (Hyeon) Kim                                              |
// | This header file contains code for Vex Houdini.                             |
// | It has the required functions for the detail creation for the procedural    |
// | building generator.                                                         |
// +-----------------------------------------------------------------------------+

// +-----------------------------------------------------------------------------+
// | Import required modules                                                     |
// +-----------------------------------------------------------------------------+
#include "BasicUtil.h"

// +-----------------------------------------------------------------------------+
// | Function declarations                                                       |
// +-----------------------------------------------------------------------------+

#ifndef __DETAILCREATION_h__
#define __DETAILCREATION_h__

/* createDetails (geometry)
 * Pre-condition: valid geometry
 * Post-condition: Does not return anything
 * Comments: it creates the required attribute for the geometry for processing.
 */
void createDetails (int geometry) {
    // Read in floor details
    int floorn = floornum(geometry);
    string floor = itoa(floorn);

    // set Beam Size for easier Reference
    float height = ch("../beam_size_" + floor + "y");
    setdetailattrib(geometry, "height", height);

    // set Scaling Factors for easier reference
    vector2 scalingFactor;
    scalingFactor[0] =ch("../jscalexz_"+floor+"x");
    scalingFactor[1] =ch("../jscalexz_"+floor+"y");
    setdetailattrib(geometry, "scalingFactor", scalingFactor);

    // make the skeleton scale into an attribute 
    // for easier calling of the variable
    float temp = ch("../skel_scale_"+floor);
    setdetailattrib(geometry, "skelScale", temp);
} // function

#endif