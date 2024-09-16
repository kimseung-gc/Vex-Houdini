#include "BasicUtil.h"

#ifndef __OPTIMIZEPOINTS_H__
#define __OPTIMIZEPOINTS_H__

dict swapSelections (int ss_s1, ss_s2; dict occupiedDict) {
    dict retDict = occupiedDict;
    retDict[itoa(ss_s1)] = occupiedDict[itoa(ss_s2)];
    retDict[itoa(ss_s2)] = occupiedDict[itoa(ss_s1)];
    return retDict;
} // function

void optimizeSelection (int geometry, nonMidPtnum, targetSurface; float coin) {
    dict occD = detail(geometry, "associates");
    dict occDPt = occD[itoa(nonMidPtnum)];
    int numE = point(geometry, "numE", nonMidPtnum);
    int nums = roundup(numE/2.0);
    vector norms[] = callNorms(geometry, nonMidPtnum);
    float or_coin = 0.0;
    int c_mid = occDPt[itoa(targetSurface)];
    vector the_current_mid = callPointVector(geometry, );
    vector TM1 = callPointVector(geometry, nonMidPtnum);
    for (int i = -1; i <= 1; i+=2) {
        or_coin += coincidence(norms[targetSurface], TMFU);
        int the_other_surface = (targetSurface + i)%numE;
        int the_other_mid = occDPt[]
        vector TM_the_other = callPointVector();
        dict newDict = swapSelections(the_other, targetSurface, occDPt);
    } // for
} // function

#endif