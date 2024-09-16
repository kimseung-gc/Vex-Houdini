// +-----------------------------------------------------------------------------+
// | Author: Seunghyeon (Hyeon) Kim                                              |
// | This header file contains code for Vex Houdini.                             |
// | It has a required function for the normal selection process.                |
// +-----------------------------------------------------------------------------+

// +-----------------------------------------------------------------------------+
// | Import required modules                                                     |
// +-----------------------------------------------------------------------------+
#include "NormalSelectionUtilOptimized.h"

// +-----------------------------------------------------------------------------+
// | Function declarations                                                       |
// +-----------------------------------------------------------------------------+

#ifndef __NORMALSELECTIONOPTIMIZED_h__
#define __NORMALSELECTIONOPTIMIZED_h__

/* final_call (geometry, ptnum, associates)
 * Pre-condition: Properly initialized geometry
 * Post-condition: Returns the resorted list of connection to mids
 */
// int[] final_call (int geometry, ptnum; dict associates) {
//     int exclusions[] = array();
//     int norms[] = callNorms(geometry, ptnum);
//     if (inpointgroup(geometry, "mid", ptnum)) {
//         return array();
//     } //if
//     int floor = floornum(geometry);
//     dict dictionary = callCombinedDict(floor);
//     int connectedMids[] = associates[itoa(ptnum)];
//     foreach (int mid ; connectedMids) {
//         if (find(keys(dictionary), mid) > 0) {
//             append(exclusions, selectedSurface);
//         } // if
//     } // foreach
//     for (int i = -1; i < 2; i++) {
//         float cummulativeCoin = 0.0;
//         foreach (int mid ; connectedMids) {
//             int prev = point(geometry, "prev", mid);
//             string selectedSurfaceInd = (prev == ptnum) ? "ss1":"ss2";
//             int selectedSurface = point(geometry, selectedSurfaceInd, mid);
//             if (find(keys(dictionary), mid) > 0) {
//                 continue;
//             } // if
//             int tempss = selectedSurface+i;
//             if (find(exclusions, tempss) > 0) {
//                 continue;
//             } // if
//         } // foreach
//     } // for
// } // final_call ()

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
        dict prevOcc = associates[itoa(prevPtInd)];
        dict nextOcc = associates[itoa(nextPtInd)];
        // Finalize selection
        int temp[] = selection_finalization (geometry, floor, ind, associates, dictionary, 1, option);
        // Save occupied points into the associates dictionary
        prevOcc[itoa(temp[0])] = i;
        nextOcc[itoa(temp[1])] = i;
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
        dict prevOcc = associates[itoa(prevPtInd)];
        dict nextOcc = associates[itoa(nextPtInd)];

        int temp[] = selection_finalization (geometry, floor, i, associates, dictionary, 0, option);

        // Save occupied points into the associates dictionary
        prevOcc[itoa(temp[0])] = i;
        nextOcc[itoa(temp[1])] = i;
        associates[itoa(prevPtInd)] = prevOcc;
        associates[itoa(nextPtInd)] = nextOcc;
    } // for
    setdetailattrib(geometry, "associates", associates);//
} // function

#endif