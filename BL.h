//
// Created by pwlqr on 23/07/2022.
//

#ifndef DBSERVERCPP_BL_H
#define DBSERVERCPP_BL_H


#include "DataBase.h"
#include "Command.h"

class BL {
    DataBase *dataBase;
public:

    BL();

    ~BL();

    int operateCommand(const char *commandBuff ,char*& response, int& responseSize);

private:

};


#endif //DBSERVERCPP_BL_H
