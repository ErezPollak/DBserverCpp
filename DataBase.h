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
        char owner[OWNER_SIZE];
        int length;
        bool isChanged = 0;

        FileInfo() {}

        FileInfo(const FileProp *fp, const bool isCreated) {
            strncpy(this->name, fp->getFileName(), NAME_SIZE);
            strncpy(this->owner, fp->getFileOwner(), OWNER_SIZE);
            this->length = strlen(fp->getFileContent()) + 1;
            this->isChanged = isCreated;
        }

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

#pragma region Singelton


    /**
     * ctor of the class, first extract all the information from the db info file,
     * then go over the files and extract the information from every file.
     */
    DataBase();

    /**
     * the instance of the database.
     */
    static DataBase *dataBaseInstance;

public:
    /**
     * Database should not be cloneable.
     */
    DataBase(DataBase &db) = delete;

    /**
     * Database should not be assignable.
     */
    void operator=(const DataBase &) = delete;

    /**
     * This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */
    static DataBase *GetInstance();

#pragma endregion

    /**
     * go over the files and write to the disk every changed file.
     */
    ~DataBase();

    /**
     * the read function, updates the FileProp object with the relevant information from the map.
     * @param fp the File Prop object to update.
     * @return 0 if the function succeeded.
     */
    int read(FileProp *fp) const;

    /**
     * the write function, deletes the old information from the map.
     * and fills it with a mew information that was given as a parameter.
     * @param fp the new data.
     * @return 0 if the operation succeeded.
     */
    int write(const FileProp *fp);

    /**
     * creating a new file with the given information.
     * @param fp the information about the file.
     * @return 0 if the operation ended successfully.
     */
    int creat(const FileProp *fp);

    /**
     * deleting an object from the database.
     * @param fp the object to delete.
     * @return 0 if the operation succeeded.
     */
    int deleteObj(const FileProp *fp);

    /**
     * updates the given addresses to be the addresses of the sise and array of all the databese data.
     * @param fileProps the address of the array with all the data.
     * @param size  the size of the array.
     * @return 0 if the operation completed.
     */
    FileProp *returnDBState(const string &owner, int &size) const;

    /**
     * write to the files all the changes that were made from the last clear.
     */
    void clearDB();

    /**
     * prints the database.
     */
    void printDB() const;

    /**
     * return whether or not the file exist.
     * @param fp the file properties for check.
     * @return true or false according to the existence of the file.
     */
    bool doesFileExist(const FileProp *fp) const;

private:

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
