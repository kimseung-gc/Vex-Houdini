// +-----------------------------------------------------------------------------+
// | Author: Seunghyeon (Hyeon) Kim                                              |
// | This header file contains code for Vex Houdini.                             |
// | It has a required function for the normal selection process.                |
// +-----------------------------------------------------------------------------+

// +-----------------------------------------------------------------------------+
// | Import required modules                                                     |
// +-----------------------------------------------------------------------------+
#include "NormalSelectionUtil.h"

// +-----------------------------------------------------------------------------+
// | Function declarations                                                       |
// +-----------------------------------------------------------------------------+

#ifndef __NORMALSELECTION_h__
#define __NORMALSELECTION_h__

/* callPoints(geometry)
 * Pre-condition: The input geometry must be pre-processed with the required attributes
 * Post-condition: It will return nothing
 * Comments: It will create attributes required for the next steps of the process on the geometry.
 */
void callPoints (int geometry, option) {
    int n = npoints(geometry);
    int mids[] = extractMids(geometry);
    int floor = floornum(geometry);
    dict dictionary = callCombinedDict(floor);
    dict associates = dict();
    //dict config = detail(geometry, "outterConfig");
    foreach (string i ; keys(dictionary)) {
        int ind = atoi(i);
        // Declaration of necessary variables
        int nextPtInd = point(geometry, "next", ind);
        int prevPtInd = point(geometry, "prev", ind);
        int prevOcc[] = associates[itoa(prevPtInd)];
        int nextOcc[] = associates[itoa(nextPtInd)];
        // Finalize selection
        int temp[] = selection_finalization (geometry, floor, ind, associates, dictionary, 1, option);
        // Save occupied points into the associates dictionary
        append(prevOcc, temp[0]);
        append(nextOcc, temp[1]);
        associates[itoa(prevPtInd)] = prevOcc;
        associates[itoa(nextPtInd)] = nextOcc;
    } // foreach
    foreach (int i; mids) {
        if (!inpointgroup(geometry, "mid", i)) {
            continue;
        } else if (isvalidindex(dictionary, itoa(i))) {
            continue;
        }// if/else if
        
        // Declaration of necessary variables
        int nextPtInd = point(geometry, "next", i);
        int prevPtInd = point(geometry, "prev", i);
        int prevOcc[] = associates[itoa(prevPtInd)];
        int nextOcc[] = associates[itoa(nextPtInd)];

        int temp[] = selection_finalization (geometry, floor, i, associates, dictionary, 0, option);

        // Save occupied points into the associates dictionary
        append(prevOcc, temp[0]);
        append(nextOcc, temp[1]);
        associates[itoa(prevPtInd)] = prevOcc;
        associates[itoa(nextPtInd)] = nextOcc;
    } // for
    setdetailattrib(geometry, "associates", associates);
} // function

#endif