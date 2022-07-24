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


FileProp * DataBase::returnDBState(int &size) const {
    size = dataBase.size();
    FileProp * fileProps = new FileProp[(this->dataBase.size() + 1) * sizeof(FileProp)];
    int i = 0;
    for_each(this->dataBase.begin(), this->dataBase.end(),
             [i, fileProps](pair<FileInfo, string> p) mutable -> void {
                 fileProps[i].setFileName(p.first.name);
                 fileProps[i].setFileOwner(p.first.owner);
                 fileProps[i].setFileContent(&p.second[0]);
                 ++i;
             }

    );
    return fileProps;
}

int DataBase::creat(const FileProp *fp) {
    if (doesFileExist(fp)) throw "file already exits in the database! abort adding!!";
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
    if (it == this->dataBase.end())throw "ether the file doesn't exist or you are not allowed to read it!";
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
    if (it == this->dataBase.end())throw "ether the file doesn't exist or you are not allowed to read it!";
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
        throw "ether the file doesn't exist or you are not allowed to read it!";
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




