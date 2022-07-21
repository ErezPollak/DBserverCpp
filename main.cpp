//
// Created by pwlqr on 16/07/2022.
//
#include <fstream>
#include <iostream>
#include "DataBase.h"
#include "Cube.h"

using namespace std;


int main() {

    try {

        DataBase dataBase;
        cout << "created db " << endl;
        dataBase.printDB();

        ///loop:
        ///..waiting for a command.
        FileProp fp;
        strcpy(fp.fileName , "erezCube.cube");
        strcpy(fp.fileOwner , "erez_polak");
        strcpy(fp.fileContent , Cube::DEFAULT_CUBE);

        try {
            dataBase.read(fp);

            Cube::cubeOperation(fp.fileContent , "rruurr");

            dataBase.write(fp);
            cout << " the fp is:  " << endl << fp << endl;
        }catch (const char* s){
            cout << s << endl;
        }
        cout << endl;
        dataBase.printDB();
        ///..executing the command.
        ///..sending response.

        ///..writing changes to the file by detractor.

    } catch (const char *str) {
        cout << str << endl;
    }
    return 0;
}




