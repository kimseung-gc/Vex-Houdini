// +-----------------------------------------------------------------------------+
// | Author: Seunghyeon (Hyeon) Kim                                              |
// | This header file contains code for Vex Houdini.                             |
// | It has the required functions that are considered "useful" in the sense     |
// | that it can be used commonly in a lot of the codes.                         |
// +-----------------------------------------------------------------------------+

// +-----------------------------------------------------------------------------+
// | Function declarations                                                       |
// +-----------------------------------------------------------------------------+

#ifndef __BASICUTIL_h__
#define __BASICUTIL_h__

/*
#pragma opname                      angle
#pragma oplabel                    "angle between a and b"
#pragma label   a                  "vector a"
#pragma label   b                  "vector b"
#pragma hint    a                   vector
#pragma hint    b                   vector
#pragma label   post-condition     "Returns the angle between vector a and b"
#pragma label   comments           "It utilizes dot product to do so. Thus, the 
                                    angle that this function returns is within [0,PI]"
*/
float angle (vector a, b) {
    float dotP = dot(a,b);
    float unitDP = dotP/(length(a)*length(b));
    return acos(unitDP);
} // function


/*
#pragma opname                      unitfy
#pragma oplabel                    "unitfy vector a"
#pragma label   a                  "vector a"
#pragma hint    a                   vector
#pragma label   post-condition     "Returns the unit-vector of a"
*/
vector unitfy (vector a) {
    return a / length(a);
} // function

/*
#pragma opname                      project
#pragma oplabel                    "project of a on b"
#pragma label   a                  "vector a"
#pragma label   b                  "vector b"
#pragma hint    a                   vector
#pragma hint    b                   vector
#pragma label   post-condition     "Returns the projected vector of a on b"
*/
vector project (vector a,b) {
    return dot(a,b)/pow(length(b),2)*b;
} // function

/* coincidence (a,b)
 * Pre-condition: a and b should be 3 dimensional vectors
 * Post-condition: Returns how well-aligned the vectors are
 */
float coincidence (vector a,b) {
    return dot(a,b)/(length(a)*length(b));
} // function

/* midPointVector (a,b)
 * Pre-condition: a and b should be 3 dimensional vectors
 * Post-condition: returns the mid point between a and b
 */
vector midPointVector (vector a, b) {
    vector ret = (a+b)/2.0;
    return ret;
} // function

/* callNorms(geometry, index)
 * Pre-condition: The geometry that contains the point attribute "norms" is required with its valid index
 * Post-condition: It will return an array of normal vectors of a potential surface.
 */
vector[] callNorms (int geometry, index) {
    return point(geometry, "norms", index);
} // function

/* callotms(geometry, index)
 * Pre-condition: The geometry that contains the point attribute "otms" is required with its valid index
 * Post-condition: It will return an array of vectors from origin to mid of a potential surface.
 */
vector[] callotms (int geometry, index) {
    return point(geometry, "otms", index);
} // function

/* callNormLens(geometry, index)
 * Pre-condition: The geometry that contains the point attribute "normLens" is required with its valid index
 * Post-condition: It will return an array of lengths around the normal direction of a potential surface.
 */
float[] callNormLens (int geometry, index) {
    return point(geometry, "normLens", index);
} // function

/* callPointVector(geometry, index)
 * Pre-condition: A geometry and index (e.x. @ptnum)
 * Post-condition: It will return the vector value (coordinate vector) of the point
 */
vector callPointVector (int geometry, index) {
    return point(geometry, "P", index);
} // function

/* callUnits()
 * Pre-condition: None
 * Post-condition: It will return the unit vectors of each axis.
 */
vector[] callUnits () {
    vector UNIT_X = set(1,0,0);
    vector UNIT_Y = set(0,1,0);
    vector UNIT_Z = set(0,0,1);
    return array(UNIT_X,UNIT_Y,UNIT_Z);
} // function

/* floornum(geometry)
 * Pre-condition: geometry with a floor number specified in the detail attributes.
 * Post-condition: It will return the floor number as an integer.
 */
int floornum(int geometry) {
    return detail(geometry, "floor");
} // function

/* retrieveMids(geometry)
 * Pre-condition: pre-processed geometry with the mids
 * Post-condition: returns the mid points as an array.
 */
int[] retrieveMids(int geometry) {
    return detail(geometry, "mids");
} // function

int[] extractMids(int geometry) {
    int ret[] = retrieveMids(geometry);
    removedetailattrib(geometry, "mids");
    return ret;
} // function

/* callCombinedDict(floor)
 * Pre-condition: Valid floor number must be provided
 * Post-condition: It will return the dictionary with all the local declaration of 
 * beam joints overwritten on the overall declaration.
 */
dict callCombinedDict (int floor) {
    string readChan = "None";
    string readChan2 = "None";
    // Checking whether the user has input exception beams
    if (ch("absT") == 0) { 
        readChan = "../bspT";
    } // if
    if (ch("abs_"+itoa(floor)) == 0) {
        readChan2 = "../bsp_"+itoa(floor);
    } // if/else if
    dict dictionary = chdict(readChan);
    dict dictionary2 = chdict(readChan2);
    string keys2[] = keys(dictionary2);
    foreach (string key ; keys2) {
        dictionary[key] = string(dictionary2[key]);
    } // foreach
    return dictionary;
} // function

/* bisection (mid, y_dir, norm, x)
 * Pre-condition: valid mid point and y direction (with some length not 0) with normal and the length x.
 * Post-condition: Returns the coordinates of a bisection (1 edge connecting 2 points) with
 * desired midpoint and y direction with its normal direction.
 */
vector[] bisection (vector mid, y_dir, norm; float x) {
    y_dir = unitfy(y_dir);
    vector allVecs[] = array();
    vector x_dir = unitfy(cross(y_dir, norm));
    for (int i = 0; i < 2; i++) {
        int XP = int(pow(-1, int(i)));
        vector temp = x_dir*x/2.0*XP;
        append(allVecs, mid+temp);
    } // for
    return allVecs;
} // function

/* rectangleFNorm (mid, y_dir, norm, x, y)
 * Pre-condition: The inputs must be well-oriented to its format/<type>
 * Post-condition: It will return the 4 corners of the rectangle of size x times y
 * and the normal of the rectangle will be towards the <norm> input.
 */
vector[] rectangleFNorm (vector mid, y_dir, norm; float x, y) {
    y_dir = unitfy(y_dir);
    vector allVecs[] = array();
    vector x_dir = unitfy(cross(y_dir, norm));
    for (int i = 0; i < 4; i++) {
        int XP = int(pow(-1, int(i/2.0)));
        int YP = int(pow(-1, int((1.0+i)/2.0)));
        vector temp = x_dir*x/2.0*XP + y_dir*y/2.0*YP;
        append(allVecs, mid+temp);
    } // for
    return allVecs;
} // function

/* polygonFNorm (mid, y_dir, norm, x, y, vecLst)
 * Pre-condition: The vector list must be oriented to origin as mid. Other values are similar to rectangleFNorm ().
 * Post-condition: It will return the corners of the desired polygon with desired normal.
 */
vector[] polygonFNorm (vector mid, y_dir, norm; float x, y; vector vecLst[]) {
    y_dir = unitfy(y_dir);
    vector allVecs[] = array();
    vector x_dir = unitfy(cross(y_dir, norm));
    foreach (vector vec; vecLst) {
        vector temp = x_dir*x/2.0*vec[0] + y_dir*y/2.0*vec[1];
        append(allVecs, mid+temp);
    } // for
    return allVecs;
} // function

/* pointVecs(geometry)
 * Pre-condition: Valid geometry is required.
 * Post-condition: Retrieve the points of the geometry as a list of vectors
 */
vector[] pointVecs (int geometry) {
    vector retL[] = array();
    for (int i = 0; i < npoints(geometry); i++) {
        vector temp = callPointVector(geometry, i);
        append(retL, temp);
    } // for
    return retL;
} // function

/* retrieveIBeamVecLst ()
 * Pre-condition: None.
 * Post-condition: Retrieves I beam vectors as list.
 */
vector[] retrieveIBeamVecLst () {
    vector IBEAM[] = array(set(-1,-1, 0), set(1,-1, 0), set(0.1, -0.8), set(0.1, 0.8), set(1,1,0), set(-1,1,0), set(-0.1, 0.8), set(-0.1, -0.8));
    return IBEAM;
} // function

/* retrieveRectangle ()
 * Pre-condition: None.
 * Post-condition: Retrieve the rectangle coordinates as a list.
 */
vector[] retrieveRectangle () {
    vector RECT[] = array(set(-1,-1, 0), set(1,-1, 0), set(1,1,0), set(-1,1,0));
    return RECT;
} // function

/* swap_row (mat, row1, row2)
 * Pre-condition: 3d matrix with rows to swap (0~2)
 * Post-condition: returns the matrix with row1 and row2 swapped.
 */
matrix3 swap_row (matrix3 mat; int row1, row2) {
    vector temp[] = set(mat);
    vector temp2 = temp[row1];
    temp[row1] = temp[row2];
    temp[row2] = temp2;
    return matrix3(temp);
} // function

/* swap_column (mat, c1, c2)
 * Pre-condition: 3d matrix with columns to swap (0~2)
 * Post-condition: It will return the matrix with c1 and c2 swapped.
 */
matrix3 swap_column (matrix3 mat; int c1, c2) {
    matrix3 tmat = transpose(mat);
    tmat = swap_row(tmat, c1, c2);
    return transpose(tmat);
} // function

/* referenceStr(xoz,floornum)
 * Pre-condition: xoz must be a string indicating either "x" or "z"
 * Post-condition: returns the string to refer as a channel inicating the
 * dimension of x or z at floor <floornum>.
 */
string referenceStr (string xoz; int floornum) {
    return "../dim_" + itoa(floornum) + xoz;
} // referenceStr

/* processStr(str)
 * Pre-condition: The input must be a string
 * Post-condition: It will return the integers with separation ','
 * Comments: It is a helper function for the selection function.
 */
int[] processStr (string str) {
    int retArr[] = array();
    string temp[] = split(str, ",");
    foreach (string strI; temp) {
        append (retArr, atoi(strip(strI)));
    } // foreach
    return retArr;
} // function

/* roundup(number)
 * Pre-condition: The input must be a float
 * Post-condition: It will return the number rounded up.
 */
int roundup (float number) {
    int rd = int (number);
    if (number-rd > 0) {
        return rd+1;
    } // if
    return rd;
} // function

/* removableTargets (geometry, prim_num)
 * Pre-condition: valid geometry and primitive number
 * Post-condition: returns a list of points the "removePrim" will delete
 */
int[] removableTargets(int geometry, prim_num) {
    int primpts[] = primpoints(geometry, prim_num);
    int retL[] = array();
    foreach (int i ; primpts) {
        int ptprims[] = pointprims(geometry, i);
        int count = len(ptprims);
        if (count == 1) {
            append(retL, i);
        } // if
    } // foreach
    return retL;
} // function

#endif