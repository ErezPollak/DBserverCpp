//
// Created by pwlqr on 16/07/2022.
//
#include <fstream>
#include <iostream>
#include "DataBase.h"

using namespace std;


int main() {

    try {

        DataBase dataBase;
        cout << "created db " << endl;
        dataBase.printDB();

        ///loop:
        ///..waiting for a command.
        FileProp fp;
        strcpy(fp.fileName , "eee.cube");
        strcpy(fp.fileOwner , "erez_polak");
        strcpy(fp.fileContent , "aaaaaaaaaaaaaaaaaaaaaaaaaaadsfasadsfasxcvsdf");

        try {
            dataBase.read(fp);
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




