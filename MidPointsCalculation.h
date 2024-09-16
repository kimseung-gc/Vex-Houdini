// +-----------------------------------------------------------------------------+
// | Author: Seunghyeon (Hyeon) Kim                                              |
// | This header file contains code for Vex Houdini.                             |
// | It has a required function for the mid point calculations for the procedural|
// | building generator.                                                         |
// +-----------------------------------------------------------------------------+

// +-----------------------------------------------------------------------------+
// | Import required modules                                                     |
// +-----------------------------------------------------------------------------+
#include "BasicUtil.h"

// +-----------------------------------------------------------------------------+
// | Function declarations                                                       |
// +-----------------------------------------------------------------------------+

#ifndef __MIDPOINTSCALCULATION_h__
#define __MIDPOINTSCALCULATION_h__

/* midPtCalculation (geometry)
 * Pre-condition: valid geometry
 * Post-condition: None
 * Comments: It creates the middle points of each of the given points and creates
 * required attributes
 */
void midPtCalculation (int geometry) {
    int mids[] = array();
    dict connectedMids = {};
    // for each of the points
    for (int i = 0; i < npoints(geometry); i++) {
        // for each of the hedges from the point i and iterate until no more hedges from i
        for (int hdg = pointhedge(geometry, i); hdg != -1; hdg = pointhedgenext(geometry, hdg)) {
            // Retrieve the destination point of the hedge
            int destination = hedge_dstpoint(geometry, hdg);
            
            // retrieve the vectors for the destination and source
            vector dstVec = callPointVector(geometry, destination);
            vector startPt = callPointVector(geometry, i);
            
            // average the vectors to obtain their mid point
            vector mid = midPointVector(dstVec, startPt);
            
            // add the mid as a point
            int midPt = addpoint(geometry, mid);
            
            // group mids for futher calculations
            setpointgroup(geometry, "mid", midPt, 1);
            
            // make the src and dst as attributes
            setpointattrib(geometry, "prev", midPt, i);
            setpointattrib(geometry, "next", midPt, destination);
            int midphi[] = connectedMids[itoa(i)];
            int midphd[] = connectedMids[itoa(destination)];
            append(midphi, midPt);
            append(midphd, midPt);
            connectedMids[itoa(i)] = midphi;
            connectedMids[itoa(destination)] = midphd;

            append(mids, midPt);
        } // for
    } // for
    setdetailattrib(geometry, "mids", mids);
    setdetailattrib(geometry, "connectedMids", connectedMids);
} // function

/*
int blastMid (int geometry, midptnum, blasting_plane) {
    int nextPtInd = point(geometry, "next", midptnum);
    int prevPtInd = point(geometry, "prev", midptnum);
    int removables[] = removableTargets(geometry, blasting_plane);
    int ind1 = find(removables, prevPtInd);
    int ind2 = find(removables, nextPtInd);
    if ((ind1<0) && (ind2<0)) {
        return 0;
    } // if
    append(removables, midptnum);
    return 1;
} // function

void blast(int geometry, cblaster[], ptnum) {
    foreach (int primnum ; cblaster) {
        if (blastMid (geoself(), ptnum, primnum)) {
            removepoint(geoself(), ptnum);
        } //if
    } // foreach
} // function
*/
#endif