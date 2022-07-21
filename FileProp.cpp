//
// Created by pwlqr on 21/07/2022.
//

#include "FileProp.h"

ostream &operator<<(ostream &os, const FileProp &f) {
    os << "name : " <<  f.fileName << ", owner: " <<  f.fileOwner <<", content: "<< f.fileContent << endl;
}