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

    /**
     * file info struct that will contain the information about a file.
     * the name, the owner of the file for authentication peruses,
     * the length of the file, flag that changes when the file changes.
     */
    struct FileInfo {
        char name[NAME_SIZE];
        char owner[OWNERR_SIZE];
        int length;
        bool isChanged;

        //needed for the "find" algorithm.
        bool operator==(const FileInfo &f) const {
            return strcmp(f.name, this->name) == 0 &&
                   strcmp(this->owner, f.owner) == 0;
        }

        //needed for the algorithm functions.
        bool operator<(const FileInfo &f) const {
            return this->name < f.name;
        }
    };

private:
    const string DB_PATH = "";
    const string DB_infoFile = "DBinfo.bin";
    /**
     * the database map, saves the info and the content of every file fot fast and efficient use.
     * the constructor extracts the information about the db from the relevant file.
     * then it extracts the information from all the files indicated in the db data file.
     */
    map<FileInfo, string> dataBase;
public:
    /**
     * ctor of the class, first extract all the information from the db info file,
     * then go over the files and extract the information from every file.
     */
    DataBase();

    /**
     * go over the files and write to the disk every changed file.
     */
    ~DataBase();

    /**
     * the read function, updates the FileProp object with the relevant information from the map.
     * @param fp the File Prop object to update.
     * @return 0 if the function succeeded.
     */
    int read(FileProp &fp) const;

    /**
     * the write function, deletes the old information from the map.
     * and fills it with a mew information that was given as a parameter.
     * @param fp the new data.
     * @return 0 if the operation succeeded.
     */
    int write(const FileProp &fp);

    /**
     * creating a new file with the given information.
     * @param fp the information about the file.
     * @return 0 if the operation ended successfully.
     */
    int creat(const FileProp &fp);

    /**
     * prints the database.
     */
    void printDB();

    /**
     * write to the files all the changes that were made from the last clear.
     */
    void clearDB();

private:

    /**
     * translates the given structure into the database structure.
     * @param fp the given data base from the outer world.
     * @param isCreated the parameter that says if the structure was changed.
     * @return the database datastructures.
     */
    FileInfo *propToInfo(const FileProp &fp, const bool &isCreated) const;

    /**
     * writing the data to a file from the data base.
     * @param name the name of the file to weite to.
     * @param content the content to weite.
     * @return 0 if the operation completed successfully.
     */
    int writeToFile(const char *name, const char *content) const;

    /**
     * reading the information form the file.
     * @param name the name of the file to read from..
     * @param content the pointer to the buffer to put hte data in.
     * @param length the size of the data. needed because it cannot be resolved from the buffer.
     * @return 0 if the operation completed successfully.
     */
    int readFromFile(const char *name, char *content, int &length) const;
};


#endif //DBSERVERCPP_DATABASE_H
