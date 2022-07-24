//
// Created by pwlqr on 21/07/2022.
//

#ifndef DBSERVERCPP_FILEPROP_H
#define DBSERVERCPP_FILEPROP_H

# include <iostream>

using namespace std;

const static int NAME_SIZE = 50;
const static int OWNER_SIZE = 50;
const static int CONTENT_SIZE = 100;

class FileProp {

    char fileName[NAME_SIZE] = {0};
    char fileOwner[OWNER_SIZE] = {0};
    char fileContent[CONTENT_SIZE] = {0};

public:
    const char *getFileContent() const;

    FileProp* setFileName(const char* name);

    FileProp* setFileOwner(const char* owner);

    FileProp* setFileContent(const char* content);

    const char *getFileName() const;

    const char *getFileOwner() const;

    char *getFileContentNonConst();

    friend ostream &operator<<(ostream &os, const FileProp &f);

};


#endif //DBSERVERCPP_FILEPROP_H
