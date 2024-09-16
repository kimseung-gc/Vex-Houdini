// +-----------------------------------------------------------------------------+
// | Author: Seunghyeon (Hyeon) Kim                                              |
// | This header file contains code for Vex Houdini. It has the required utility |
// | functions for the normal selection process.                                 |
// +-----------------------------------------------------------------------------+

// +-----------------------------------------------------------------------------+
// | Import required modules                                                     |
// +-----------------------------------------------------------------------------+
#include "BasicUtil.h"

#ifndef __NORMALSELECTIONUTILOPTIMIZED_h__
#define __NORMALSELECTIONUTILOPTIMIZED_h__

// +-----------------------------------------------------------------------------+
// | Structs                                                                     |
// +-----------------------------------------------------------------------------+

/* pillar
 * Data type for each columns of the building
 */
struct pillar {
    int geometry, ptId;
    vector[] otms () {
        return callotms(geometry, ptId);
    } // callOtms ()
    vector[] norms () {
        return callNorms(geometry, ptId);
    } // callNorms ()
    float[] normLens () {
        return callNormLens(geometry, ptId);
    } // normLens ()
    int numE () {
        return point(geometry, "numE", ptId);
    } // numE ()
};

/* midPt
 * midpoints of each beams
 */
struct midPt {
    // Fields
    int geometry, ptInd;

    //methods
    vector currentPtVec () {
        return callPointVector(geometry, ptInd);
    } // currentPtVec ()
    vector prevPtVec () {
        int prevPt = point(geometry, "prev", ptInd);
        return callPointVector(geometry, prevPt);
    } // prevPtVec ()
    vector nextPtVec () {
        int nextPt = point(geometry, "next", ptInd);
        return callPointVector(geometry, nextPt);
    } // prevPtVec ()
    pillar prevPillar () {
        int prevPt = point(geometry, "prev", ptInd);
        return pillar(geometry, prevPt);
    } // prevPillar ()
    pillar nextPillar () {
        int nextPt = point(geometry, "next", ptInd);
        return pillar(geometry, nextPt);
    } // nextPillar ()
    vector[] callPrevNorms () {
        return this->prevPillar()->norms();
    } // callPrevNorms()
    vector[] callNextNorms () {
        return this->nextPillar()->norms();
    } // callPrevNorms()
    vector TMFP() {
        vector curp = this->currentPtVec();
        vector p1 = this->prevPtVec();
        return curp-p1;
    } // TMFP ()
    vector TMFN() {
        vector curp = this->currentPtVec();
        vector p1 = this->nextPtVec();
        return curp-p1;
    } // TMFN ()
};

// +-----------------------------------------------------------------------------+
// | Function declarations                                                       |
// +-----------------------------------------------------------------------------+

/* force_selection (geoemtry, currentPt, forceSets)
 * Pre-condition: currentPt must be a valid index of forceSets
 * Post-condition: It will force the selection of the beam as the forceSets mention.
 */
int[] force_selection (midPt mid; dict forceSets) {
    int ss1, ss2;
    string selectionInts = forceSets[itoa(mid.ptInd)];
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
int[] selection (midPt mid; dict prevOcc, nextOcc) {
    int ss1, ss2;
    float coins1 = -1, prevCoins1 = -1;
    float coins2 = -1, prevCoins2 = -1;
    for (int i = 0; i < mid->prevPillar()->numE(); i++) {
        coins1 = coincidence(mid->callPrevNorms()[i], mid->TMFP());
        if (!isvalidindex(prevOcc, itoa(i)) && (coins1 >= prevCoins1)) {
            ss1 = i;
            prevCoins1 = coins1;
        } /*else if (isvalidindex(prevOcc, itoa(i)) && (coins1 >= prevCoins1)) {
            ss1 = -prevOcc[itoa(i)];
            prevCoins1 = coins1;
        } // if / else if*/
    } // foreach
    for (int i = 0; i < mid->nextPillar()->numE(); i++) {
        coins2 = coincidence(mid->callNextNorms()[i], mid->TMFN());
        if (!isvalidindex(nextOcc, itoa(i)) && (coins2 >= prevCoins2)) {
            ss2 = i;
            prevCoins2 = coins2;
        } /*else if (isvalidindex(nextOcc, itoa(i)) && (coins2 >= prevCoins2)) {
            ss2 = -nextOcc[itoa(i)];
            prevCoins2 = coins2;
        }// if*/
    } // foreach
    return array(ss1,ss2);
} // function

/* selection_finalization(geometry, floor, i, associates, forceSets, mod)
 * Pre-condition: The geometry must be pre-processed
 * Post-condition: It will finalize ss1 and ss2 and return them as array(ss1,ss2)
 */
int[] selection_finalization (int geometry, floor, i; dict associates, forceSets; int mod, option) {
    midPt mid = midPt(geometry, i);
    pillar prevPillar = mid->prevPillar();
    pillar nextPillar = mid->nextPillar();
    // Declaration of basic variables
    vector UNIT_X = callUnits()[0];
    vector UNIT_Y = callUnits()[1];
    
    // Declaration of necessary variables
    int nextPtInd = point(geometry, "next", i);
    int prevPtInd = point(geometry, "prev", i);
    dict prevOcc = associates[itoa(prevPtInd)];
    dict nextOcc = associates[itoa(nextPtInd)];
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
        selections = selection(mid,prevOcc,nextOcc);
    } else if (mod == 1) {
        selections = force_selection(mid, forceSets);
    } // if/else if
    ss1 = selections[0];    
    ss2 = selections[1];
    vector towardsMidFP, towardsMidFN;

    if (ss1 < 0) {
        int ind = -ss1;
        // Save variables as attributes
        ssv1 = point(geometry, "N", ind);
        ssl1 = 0;
        prevPt = callPointVector(geometry, ind);
        towardsMidFP = 0;
    } else {
        // Finalization of selected vectors and lengths
        ssv1 = selectionArr1[ss1];
        ssl1 = selectionLenArr1[ss1];
        // Vectors towards the new mid point from previous point and next point
        towardsMidFP = ssl1/2.0*unitfy(ssv1);
    } // if/else
    if (ss2 < 0) {
        int ind = -ss2;
        // Save variables as attributes
        ssv2 = point(geometry, "N", ind);
        ssl2 = 0;
        nextPt = callPointVector(geometry, ind);
        towardsMidFN = 0;
    } else {
        ssv2 = selectionArr2[ss2];
        ssl2 = selectionLenArr2[ss2];
        towardsMidFN = ssl2/2.0*unitfy(ssv2);
    }// if/else
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