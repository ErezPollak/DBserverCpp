//
// Created by pwlqr on 16/07/2022.
//

#ifndef DBSERVERCPP_DATABASE_H
#define DBSERVERCPP_DATABASE_H

#include <fstream>
#include <string.h>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include "FileProp.h"

using namespace std;

class DataBase {

    struct FileInfo {
        char name[NAME_SIZE];
        char owner[OWNERR_SIZE];
        int length;
        bool isChanged;

        bool operator==(const FileInfo &f) const {
            return strcmp(f.name, this->name) == 0 &&
                   strcmp(this->owner, f.owner) == 0;
        }

        bool operator<(const FileInfo &f) const {
            return this->name < f.name;
        }
    };

private:
    const string DB_PATH = "";
    const string DB_infoFile = "DBinfo.bin";
    map<FileInfo, string> dataBase;
public:

    DataBase();

    ~DataBase();

    int read(FileProp& fp)const;

    int write(const FileProp& fp);

    int creat(const FileProp& fp);

    void printDB();

    void clearDB();

private:

    FileInfo* propToInfo(const FileProp& fp, const bool& isCreated)const;

    int writeToFile(const char *name, const char *content) const;

    int readFromFile(const char *name, char *content, int &length) const;
};


#endif //DBSERVERCPP_DATABASE_H
