//
// Created by pwlqr on 23/07/2022.
//

#ifndef DBSERVERCPP_COMMAND_H
#define DBSERVERCPP_COMMAND_H

#include <string>
#include <cstring>
#include "FileProp.h"
#include "BL.h"

const int RECEIVE_MESSAGE_LENGTH = 1024;
const int COMMAND_SIZE = 20;

enum CommandType {
    CREATE, READ, WRITE, DELETE, ALL, SIZES, ERROR
};


class Command {

    string name = "";
    string owner = "";
    string payload = "";
    string command = "";

    CommandType type;


public:

    Command(const char *commandBuff);

    const string &getName() const;

    const string &getOwner() const;

    const string &getState() const;

    const string &getCommand() const;

    CommandType getType() const;

    CommandType recognizeType(const bool &doesFileExist);

private:
    bool isState(const char *str);
};

#endif //DBSERVERCPP_COMMAND_H
