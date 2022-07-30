//
// Created by pwlqr on 16/07/2022.
//

#include "DataBase.h"

#pragma region Singelton


DataBase *DataBase::dataBaseInstance = nullptr;

/**
 * Static methods should be defined outside the class.
 */
DataBase *DataBase::GetInstance() {
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangerous in case two instance threads wants to access at the same time
     */
    if (dataBaseInstance == nullptr) {
        dataBaseInstance = new DataBase();
    }
    return dataBaseInstance;
}

#pragma endregion

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


void DataBase::printDB() const {
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


FileProp *DataBase::returnDBState(const string &owner, int &size) const {
    FileProp *fileProps = new FileProp[this->dataBase.size() * sizeof(FileProp)];
    size = 0;
    for_each(this->dataBase.begin(), this->dataBase.end(),
             [&size, &owner, fileProps](pair<FileInfo, string> p) mutable -> void {
                if (strcmp(&owner[0] , p.first.owner) == 0) {
                     fileProps[size].setFileName(p.first.name);
                     fileProps[size].setFileOwner(p.first.owner);
                     fileProps[size].setFileContent(&p.second[0]);
                     ++size;
                 }
             }

    );
    return fileProps;
}

int DataBase::creat(const FileProp *fp) {
    if (doesFileExist(fp)) throw "ERROR: DB: file already exits in the database! abort adding!!";
    FileInfo fi(fp, true);
    this->dataBase.insert(pair<FileInfo, string>(fi, fp->getFileContent()));
    return 0;
}


int DataBase::read(FileProp *fp) const {
    auto it = find_if(this->dataBase.begin(), this->dataBase.end(),
                      [&fp](pair<FileInfo, string> p) -> bool {
                          return strcmp(p.first.name, fp->getFileName()) == 0 &&
                                 strcmp(p.first.owner, fp->getFileOwner()) == 0;
                      });
    if (it == this->dataBase.end())throw "ERROR: DB: ether the file doesn't exist or you are not allowed to read it!";
    strncpy(fp->getFileContentNonConst(), &it->second[0], CONTENT_SIZE);
    return 0;
}

int DataBase::write(const FileProp *fp) {
    //finds the iterator contains the needed information, and makes sure the owner is
    auto it = find_if(this->dataBase.begin(), this->dataBase.end(),
                      [&fp](pair<FileInfo, string> p) -> bool {
                          return strcmp(p.first.name, fp->getFileName()) == 0 &&
                                 strcmp(p.first.owner, fp->getFileOwner()) == 0;
                      });
    if (it == this->dataBase.end())throw "ERROR: DB: ether the file doesn't exist or you are not allowed to read it!";
    this->dataBase.erase(it);
    creat(fp);
    return 0;
}

int DataBase::deleteObj(const FileProp *fp) {
    //finds the iterator contains the needed information, and makes sure the owner is
    auto it = find_if(this->dataBase.begin(), this->dataBase.end(),
                      [&fp](pair<FileInfo, string> p) -> bool {
                          return strcmp(p.first.name, fp->getFileName()) == 0 &&
                                 strcmp(p.first.owner, fp->getFileOwner()) == 0;
                      });
    if (it == this->dataBase.end())
        throw "ERROR: DB: ether the file doesn't exist or you are not allowed to read it!";
    this->dataBase.erase(it);
    return 0;
}

bool DataBase::doesFileExist(const FileProp *fp) const {
    return any_of(this->dataBase.begin(), this->dataBase.end(),
                  [&fp](pair<FileInfo, string> p) -> bool {
                      return strcmp(p.first.name, fp->getFileName()) == 0 &&
                             strcmp(p.first.owner, fp->getFileOwner()) == 0;
                  });
}




