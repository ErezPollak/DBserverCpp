//
// Created by pwlqr on 16/07/2022.
//

#include "DataBase.h"

DataBase::DataBase() {
    ifstream infoFile(DB_PATH + DB_infoFile, ios::binary);
    if (!infoFile)throw "cant open info file!";
    FileInfo fileInfo;
    while (infoFile.read((char *) &fileInfo, sizeof(FileInfo))) {
        try {
            char fileContent[fileInfo.length];
            readFromFile(fileInfo.name, fileContent, fileInfo.length);
            //the file is now brought into the process it wasn't changed yet.
            fileInfo.isChanged = false;
            this->dataBase.insert(pair<FileInfo, string>(fileInfo, fileContent));
        } catch (const char *str) {
            cout << str << "  the file:  " << fileInfo.name << endl;
        }
    }
    infoFile.close();
    //if (!infoFile.good())throw "problem during reading info file!";
}

int DataBase::readFromFile(const char *name, char *content, int &length) const {
    ifstream file(DB_PATH + name, ios::binary);
    if (!file)throw "cant open content file!";
    file.read(content, length);
    if (!file.good())throw "problem during reading from content file!";
    file.close();
    return 0;
}


DataBase::~DataBase() {
    clearDB();
}


void DataBase::clearDB() {
    ofstream infoFile(DB_PATH + DB_infoFile, ios::binary);
    if (!infoFile)throw "cant open info file!";
    FileInfo fileInfo;
    for_each(this->dataBase.begin(), this->dataBase.end(),
             [&](pair<FileInfo, string> p) {
                 fileInfo = p.first;
                 infoFile.write((char *) &fileInfo, sizeof(FileInfo));
                 if (p.first.isChanged)
                     writeToFile(fileInfo.name, &(p.second)[0]);
             });
    if (!infoFile.good())throw "problem during writing!";
    infoFile.close();

    cout << "updated files" << endl;
}


int DataBase::writeToFile(const char *name, const char *content) const {
    ofstream file(DB_PATH + name, ios::binary);
    if (!file)throw "cant open content file!";
    file.write(content, strlen(content) + 1);
    if (!file.good())throw "problem during writing to content file!";
    file.close();
    return 0;
}


void DataBase::printDB() {
    int i = 0;
    for_each(this->dataBase.begin(), this->dataBase.end(),
             [i](pair<FileInfo, string> p) mutable -> void {
                 cout << "#" << ++i <<
                      " name: " << p.first.name <<
                      " ,owner: " << p.first.owner <<
                      " ,length: " << p.first.length <<
                      " ,changed: " << p.first.isChanged << endl <<
                      "\tcontent: " << p.second << endl;
             });
    cout << endl;
}

DataBase::FileInfo *DataBase::propToInfo(const FileProp &fp, const bool &isCreated) const {
    FileInfo *fileInfo = new FileInfo();
    strncpy(fileInfo->name, fp.fileName, NAME_SIZE);
    strncpy(fileInfo->owner, fp.fileOwner, OWNERR_SIZE);
    fileInfo->length = strlen(fp.fileContent) + 1;
    fileInfo->isChanged = isCreated;

    return fileInfo;

}

int DataBase::creat(const FileProp &fp) {
    if (any_of(this->dataBase.begin(), this->dataBase.end(),
               [&fp](pair<FileInfo, string> p) { return strcmp(p.first.name, fp.fileName) == 0; }))
        throw "file already exits in the database! abort adding!!";
    this->dataBase.insert(pair<FileInfo, string>(*propToInfo(fp, true), fp.fileContent));
    return 0;
}


int DataBase::read(FileProp &fp) const {
    auto it = find_if(this->dataBase.begin(), this->dataBase.end(),
                      [=](const pair<FileInfo, string> &p) -> bool {
                          return *propToInfo(fp, 0) == p.first;
                      });
    if (it == this->dataBase.end())throw "ether the file doesn't exist or you are not allowed to read it!";
    strncpy(&fp.fileContent[0], &it->second[0], CONTENT_SIZE);
    return 0;
}

int DataBase::write(const FileProp &fp) {
    //finds the iterator contains the needed information, and makes sure the owner is
    auto it = find_if(this->dataBase.begin(), this->dataBase.end(),
                      [=](const pair<FileInfo, string> &p) -> bool {
                          return *propToInfo(fp, 0) == p.first;
                      });
    if (it == this->dataBase.end())throw "ether the file doesn't exist or you are not allowed to read it!";
    this->dataBase.erase(it);
    creat(fp);
    return 0;
}

int DataBase::deleteObj(const FileProp &fp) {
    //finds the iterator contains the needed information, and makes sure the owner is
    auto it = find_if(this->dataBase.begin(), this->dataBase.end(),
                      [=](const pair<FileInfo, string> &p) -> bool {
                          return *propToInfo(fp, 0) == p.first;
                      });
    if (it == this->dataBase.end())throw "ether the file doesn't exist or you are not allowed to read it!";
    this->dataBase.erase(it);
    return 0;
}

