//
// Created by pwlqr on 21/07/2022.
//

#ifndef DBSERVERCPP_FILEPROP_H
#define DBSERVERCPP_FILEPROP_H

# include <iostream>
using namespace std;


const int NAME_SIZE = 50;
const int OWNERR_SIZE = 50;
const int CONTENT_SIZE = 100;

class FileProp {

public:

    char fileName[NAME_SIZE];
    char fileOwner[OWNERR_SIZE];
    char fileContent[CONTENT_SIZE];

    friend ostream& operator<<(ostream& os , const FileProp& f);

};


#endif //DBSERVERCPP_FILEPROP_H
