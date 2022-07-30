//
// Created by pwlqr on 21/07/2022.
//

#ifndef DBSERVERCPP_CUBE_H
#define DBSERVERCPP_CUBE_H


#include <cstring>
using namespace std;

class Cube {
public:
    /**
     * the main function of this layer and its only endpoint for the api of the data layer.
     * @param state the payload to make the changes on.
     * @param operation  the operation to make.
     * @return 0 if succeeded and 1 if not.
     */
    static int cubeOperation(char *state,const char *operation);
    static const int CUBE_STATE_SIZE = 54;
    static const char DEFAULT_CUBE[CUBE_STATE_SIZE];

private:

    /**
     * make use of both the helper functions and performs a complete move of the cube.
     * @param state the payload to make the move on.
     * @param side the side on the cube to make the move on.
     * @return
     */
    static int rotateCube(char *state, const int &side) {
        return rotateEdge(state, side) + rotateSide(state, side);
    }

    /**
     * rotate only the side of the cube. the side itself.
     * @param state the payload to move.
     * @param side the side move.
     * @return 0 if succeeded, 1 if not.
     */
    static int rotateSide(char *state, const int &side);

    /**
     * moves the edge around the side itself.
     * @param state the payload to make the change on.
     * @param side the side to move its edge.
     * @return
     */
    static int rotateEdge(char *state, const int &side);

    /**
     * the dictionary contains all the relation between side and the letter it is represented by.
     * @param move the letter.
     * @return  the side number.
     */
    static int move(const char &move);

    /**
     * helper function that moves three chars form one string to another.
     * @param dst the dst string.
     * @param srcIndex the index to start picking the letters from.
     * @param src the src string.
     * @param dstIndex the index to start pasting to chars on.
     */
    static void copyTreeChars(char *dst, const int &srcIndex, const char *src, const int &dstIndex);

    /**
     * the array that contains for every side of the cubes what sides we need to move and with what index in them we need to start moving the chars.
     * the information represented by the index on the payload array to start the moving from.
     */
    static int edgeMovement[6][4];
};


#endif //DBSERVERCPP_CUBE_H
