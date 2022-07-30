//
// Created by pwlqr on 23/07/2022.
//

#include "Command.h"

static const map<string , CommandType> DICTIONARY = {
        {"all", CommandType::ALL},
        {"sizes", CommandType::SIZES},
        {"delete",CommandType::DELETE},
        {"read",CommandType::READ},
        {"write",CommandType::WRITE},
        {"create", CommandType::CREATE}
};


Command::Command(const char *commandBuff) {
    char commandCpy[RECEIVE_MESSAGE_LENGTH];
    strncpy(commandCpy, commandBuff, RECEIVE_MESSAGE_LENGTH);
    char *ptr; // declare a ptr pointer
    ptr = strtok(commandCpy, ","); // use strtok() function to separate string using comma (,) delimiter.
    cout << "\nCommand: " << endl;
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
                cout << "payload: " << &ptr[1] << endl;
                this->payload = &ptr[1];
            }
                break;
            default: {
                throw "not a correct payload. problem with one of the headers.";
            }
        }
        ptr = strtok(NULL, " , ");
    }
    cout << endl;
}

const string &Command::getName() const {
    return this->name;
}

const string &Command::getOwner() const {
    return this->owner;
}

const string &Command::getState() const {
    return this->payload;
}

CommandType Command::getType() const {
    return type;
}

CommandType Command::recognizeType(const bool &doesFileExist) {

    return DICTIONARY.at(this->command);

//    if (this->command == "sizes") return SIZES;
//    if (this->command == "all")return ALL;
//    if (this->command == "delete")return DELETE;
//    if (this->command == "reset")return RESET;
//    if (doesFileExist) {
//        return this->payload == "" ? READ :
//               isState(&this->payload[0]) ? WRITE : ERROR;
//    } else {
//        return this->payload == "" ? CREAT_DEFAULT :
//               isState(&this->payload[0]) ? CREATE_INPUT : ERROR;;
//    }
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
