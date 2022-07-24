//
// Created by pwlqr on 21/07/2022.
//

#include "Cube.h"

int Cube::cubeOperation(char *state,const char *operation) {
    for (int i = 0; i < strlen(operation); ++i) {
        rotateCube(state, move(operation[i]));
    }
    return 0;
}

const char Cube::DEFAULT_CUBE[54] = "yyyyyyyy.rrrrrrrr.bbbbbbbb.oooooooo.gggggggg.wwwwwwww";

int Cube::edgeMovement[][4] = {
        {9,  18, 27, 36},
        {4,  42, 45, 20},
        {6,  15, 51, 29},
        {0,  24, 49, 38},
        {2,  33, 47, 11},
        {13, 40, 31, 22}};


int Cube::rotateSide(char *state, const int &side) {
    if (side < 0 || side > 6) return 1;

    int initialIndex = side * 9;

    char last[2] = {state[initialIndex + 6], state[initialIndex + 7]};

    for (int i = 4; i > 0; --i) {
        state[i * 2 - 1 + initialIndex] = state[(i - 1) * 2 - 1 + initialIndex];
        state[i * 2 - 1 - 1 + initialIndex] = state[(i - 1) * 2 - 1 - 1 + initialIndex];
    }

    state[initialIndex] = last[0];
    state[initialIndex + 1] = last[1];

    return 0;
}


int Cube::rotateEdge(char *state, const int &side) {

    if (side < 0 || side > 6) return 1;

    char temp[3];
    copyTreeChars(temp, 0, state, edgeMovement[side][3]);

    for (int i = 0; i < 3; ++i) {
        //saves for later use the last side coordinates.
        copyTreeChars(state, edgeMovement[side][3 - i], state, edgeMovement[side][3 - i - 1]);
    }

    copyTreeChars(state, edgeMovement[side][0], temp, 0);

    return 0;
}

void Cube::copyTreeChars(char *dst, const int &dstIndex, const char *src, const int &srcIndex) {
    int dstFinal, srcFinal;
    for (int i = 0; i < 3; ++i) {
        dstFinal = (dstIndex + i) % 9 != 8 ? dstIndex + i : dstIndex + i - 8;
        srcFinal = (srcIndex + i) % 9 != 8 ? srcIndex + i : srcIndex + i - 8;

        //cout << dstFinal <<"   " << srcFinal << endl;

        dst[dstFinal] = src[srcFinal];
    }
}

int Cube::move(const char &move) {
    switch (move) {
        case 'r':
            return 4;
        case 'u' :
            return 0;
        case 'l':
            return 2;
        case 'f':
            return 1;
        case 'b':
            return 3;
        case 'd':
            return 5;
        default:
            return -1;
    }
}

