//
// Created by pwlqr on 23/07/2022.
//

#include "Command.h"


Command::Command(const char *commandBuff) {
    char commandCpy[RECEIVE_MESSAGE_LENGTH];
    strncpy(commandCpy, commandBuff, RECEIVE_MESSAGE_LENGTH);
    char *ptr; // declare a ptr pointer
    ptr = strtok(commandCpy, ","); // use strtok() function to separate string using comma (,) delimiter.
    cout << "\nSplit string using strtok() function: " << endl;
    // use while loop to check ptr is not null
    while (ptr != NULL) {
        switch (ptr[0]) {
            case 'n': {
                cout << "name: " << &ptr[1] << "   " << endl;
                this->name = &ptr[1];
            }
                break;
            case 'o': {
                cout << "owner: " << &ptr[1] << endl;
                this->owner = &ptr[1];
            }
                break;
            case 'c': {
                cout << "command: " << &ptr[1] << endl;
                this->command = &ptr[1];
            }
                break;
            case 's': {
                cout << "state: " << &ptr[1] << endl;
                this->state = &ptr[1];
            }
                break;
            default: {
                throw "not a correct state. problem with one of the headers.";
            }
        }
        ptr = strtok(NULL, " , ");
    }
}

const string &Command::getName() const {
    return this->name;
}

const string &Command::getOwner() const {
    return this->owner;
}

const string &Command::getState() const {
    return this->state;
}

CommandType Command::getType() const {
    return type;
}

CommandType Command::recognizeType(const bool &doesFileExist) {
    if (this->command == "delete")return DELETE;
    if (this->command == "reset")return RESET;
    if (doesFileExist) {
        return this->state == "" ? READ :
               isState(&this->state[0]) ? WRITE : ERROR;
    } else {
        return this->state == "" ? CREAT_DEFAULT :
               isState(&this->state[0]) ? CREATE_INPUT : ERROR;;
    }
}

bool Command::isState(const char *str) {
    //if the length is not correct the function will return false immediately.
    if (strlen(str) != 53) return false;

    map<char, int> counter;
    counter['y'] = 0;
    counter['g'] = 0;
    counter['r'] = 0;
    counter['w'] = 0;
    counter['o'] = 0;
    counter['b'] = 0;
    counter['.'] = 0;

    for (int i = 0; i < strlen(str); ++i) {
        //checks that the points are in the right positions.
        if (i % 9 == 8 && str[i] != '.') { return false; }
        //counting the number of appearances.
        ++counter[str[i]];
    }

    return counter['y'] == 8 && counter['g'] == 8 && counter['r'] == 8 && counter['w'] == 8 && counter['o'] == 8 &&
           counter['b'] == 8 && counter['.'] == 5;
}

const string &Command::getCommand() const {
    return command;
}
