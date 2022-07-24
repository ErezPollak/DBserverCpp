//
// Created by pwlqr on 21/07/2022.
//

#include <cstring>
#include "FileProp.h"

ostream &operator<<(ostream &os, const FileProp &f) {
    os << "name : " <<  f.fileName << ", owner: " <<  f.fileOwner <<", content: "<< f.fileContent << endl;
}

const char *FileProp::getFileName() const {
    return fileName;
}

const char *FileProp::getFileOwner() const {
    return fileOwner;
}

char *FileProp::getFileContentNonConst(){
    return fileContent;
}

FileProp *FileProp::setFileName(const char *name) {
    strncpy(this->fileName , name , NAME_SIZE);
    return this;
}

FileProp *FileProp::setFileOwner(const char *owner) {
    strncpy(this->fileOwner , owner , OWNER_SIZE);
    return this;
}

FileProp *FileProp::setFileContent(const char *content) {
    strncpy(this->fileContent , content , CONTENT_SIZE);
    return this;
}

const char *FileProp::getFileContent() const {
    return fileContent;
}
