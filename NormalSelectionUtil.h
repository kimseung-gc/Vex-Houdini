// +-----------------------------------------------------------------------------+
// | Author: Seunghyeon (Hyeon) Kim                                              |
// | This header file contains code for Vex Houdini. It has the required utility |
// | functions for the normal selection process.                                 |
// +-----------------------------------------------------------------------------+

// +-----------------------------------------------------------------------------+
// | Import required modules                                                     |
// +-----------------------------------------------------------------------------+
#include "BasicUtil.h"

// +-----------------------------------------------------------------------------+
// | Function declarations                                                       |
// +-----------------------------------------------------------------------------+

#ifndef __NORMALSELECTIONUTIL_h__
#define __NORMALSELECTIONUTIL_h__

/* force_selection (geoemtry, currentPt, forceSets)
 * Pre-condition: currentPt must be a valid index of forceSets
 * Post-condition: It will force the selection of the beam as the forceSets mention.
 */
int[] force_selection (int geometry, currentPt; dict forceSets) {
    int ss1, ss2;
    string selectionInts = forceSets[itoa(currentPt)];
    int tempSel[] = processStr(selectionInts);
    if (len(tempSel) < 2) {
        ss1 = 0;
        ss2 = 0;
        return array(ss1, ss2);
    } // if
    ss1 = tempSel[0];
    ss2 = tempSel[1];
    // If not, the algorithm automatically calculates the selected surfaces.
    return array(ss1,ss2);
}

/* selection (norms,corner1,corner2,forceSets,forceSetsBackup)
 * Pre-condition: The geometry should be pre-processed.
 * Post-condition: It will return the best oriented normal indices.
 */
int[] selection (int geometry; int currentPt, corner1, corner2; int prevOcc[], nextOcc[]) {
    float resolution = 0.1;
    int ss1, ss2;
    vector normsc1[] = callNorms(geometry, corner1);
    vector normsc2[] = callNorms(geometry, corner2);
    vector towardsMidFP = callPointVector(geometry, currentPt)-callPointVector(geometry, corner1);
    vector towardsMidFN = callPointVector(geometry, currentPt)-callPointVector(geometry, corner2);
    float coins1 = -1, prevCoins1 = -1;
    float coins2 = -1, prevCoins2 = -1;
    int prevNorms1[];
    int prevNorms2[];
    for (int i = 0; i < len(normsc1); i++) {
        int ind1 = find(prevOcc, i);
        coins1 = coincidence(normsc1[i], towardsMidFP);
        if ((coins1 > prevCoins1) && (ind1 < 0)) {
            append(prevNorms1, i);
            ss1 = i;
            prevCoins1 = coins1;
        } // if
    } // foreach
    for (int i = 0; i < len(normsc2); i++) {
        int ind2 = find(nextOcc, i);
        coins2 = coincidence(normsc2[i], towardsMidFN);
        if ((coins2 > prevCoins2) && (ind2 < 0)) {
            append(prevNorms2, i);
            ss2 = i;
            prevCoins2 = coins2;
        } // if
    } // foreach
    return array(ss1,ss2);
} // function

/* selection_finalization(geometry, floor, i, associates, forceSets, mod)
 * Pre-condition: The geometry must be pre-processed
 * Post-condition: It will finalize ss1 and ss2 and return them as array(ss1,ss2)
 */
int[] selection_finalization (int geometry, floor, i; dict associates, forceSets; int mod, option) {
    // Declaration of basic variables
    vector UNIT_X = callUnits()[0];
    vector UNIT_Y = callUnits()[1];
    
    // Declaration of necessary variables
    int nextPtInd = point(geometry, "next", i);
    int prevPtInd = point(geometry, "prev", i);
    int prevOcc[] = associates[itoa(prevPtInd)];
    int nextOcc[] = associates[itoa(nextPtInd)];
    vector prevPt = callPointVector(geometry, prevPtInd);
    vector nextPt = callPointVector(geometry, nextPtInd);
    vector residue = prevPt-nextPt;
    vector ptLoc = callPointVector(geometry, i);
    // vector2 scaler = detail(0,"scalingFactor");
    vector selectionArr1[] = callotms(geometry, prevPtInd);
    vector selectionArr2[] = callotms(geometry, nextPtInd);
    float selectionLenArr1[] = callNormLens(geometry, prevPtInd);
    float selectionLenArr2[] = callNormLens(geometry, nextPtInd);
        
    // Uninitialized Declaration of Selection variables
    int ss1, ss2;
    vector ssv1, ssv2;
    float ssl1, ssl2;
    int selections[] = array();
    if (mod == 0) {
        selections = selection(geometry,i,prevPtInd,nextPtInd,prevOcc,nextOcc);
    } else if (mod == 1) {
        selections = force_selection(geometry, i, forceSets);
    } // if/else if
    ss1 = selections[0];    
    ss2 = selections[1];
    
    // Finalization of selected vectors and lengths
    ssv1 = selectionArr1[ss1];
    ssv2 = selectionArr2[ss2];
    ssl1 = selectionLenArr1[ss1];
    ssl2 = selectionLenArr2[ss2];

    // Vectors towards the new mid point from previous point and next point
    vector towardsMidFP = ssl1/2.0*unitfy(ssv1);
    vector towardsMidFN = ssl2/2.0*unitfy(ssv2);
    
    // Calculation of new mid point and previous and next point
    vector p1 = prevPt+towardsMidFP;
    vector p2 = nextPt + towardsMidFN;
    p1 += option*set(0,(ch("../dim_"+itoa(floor) +"y"))-detail(geometry, "height")/2.0,0);
    p2 += option*set(0,(ch("../dim_"+itoa(floor) +"y"))-detail(geometry, "height")/2.0,0);
    ptLoc = (p1+p2)/2;
    residue = p1-p2; // Vector from next to previous

    // Normal Calculation
    vector norm = cross(UNIT_Y, residue);
    
    // Normal modification towards the centre(0,0)
    float dotPN = dot(ptLoc,norm);
    if (dotPN >= 0) {
        norm = -norm;
    } // if

    // Save variables as attributes
    setpointattrib(geometry, "prevPtLoc", i, p1);
    setpointattrib(geometry, "nextPtLoc", i, p2);
    setpointattrib(geometry, "N", i, norm);
    setpointattrib(geometry, "P", i, ptLoc);
    setpointattrib(geometry, "ssl1", i, ssl1);
    setpointattrib(geometry, "ssl2", i, ssl2);
    setpointattrib(geometry, "ss1", i, ss1);
    setpointattrib(geometry, "ss2", i, ss2);

    return array(ss1,ss2);
} // function

#endif