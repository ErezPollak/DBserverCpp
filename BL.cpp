//
// Created by pwlqr on 23/07/2022.
//

#include "BL.h"
#include "Cube.h"

BL::BL() : dataBase(DataBase::GetInstance()) {}

BL::~BL() {
    delete this->dataBase;
}

int BL::operateCommand(const char *commandBuff, char *&response, int &responseSize) {
    Command command(commandBuff);

    FileProp *fp = new FileProp;
    fp->setFileName(command.getName().c_str())->setFileOwner(command.getOwner().c_str());

    CommandType type = command.recognizeType(this->dataBase->doesFileExist(fp));

    cout << "type  " << type << endl << endl;

    switch (type) {
        case CREATE: {
            strncpy(fp->getFileContentNonConst(), Cube::DEFAULT_CUBE, CONTENT_SIZE);
            try {
                this->dataBase->creat(fp);
            } catch (const char *ch) {
                throw ch;
            }
            response = (char *) fp;
            responseSize = sizeof(FileProp);
            cout << "created default cube and operated successfully" << endl;
        }
            break;
        case READ: {
            try {
                this->dataBase->read(fp);
            } catch (const char *ch) {
                throw ch;
            }
            response = (char *) fp;
            responseSize = sizeof(FileProp);
            cout << "read cube successfully" << endl;
        }
            break;
        case WRITE: {
            //strncpy(fp->getFileContentNonConst(), command.getState().c_str(), CONTENT_SIZE);
            try {
                this->dataBase->read(fp);
                Cube::cubeOperation(fp->getFileContentNonConst(), &command.getState()[0]);
                this->dataBase->write(fp);
            } catch (const char *ch) {
                throw ch;
            }
            response = (char *) fp;
            responseSize = sizeof(FileProp);
            cout << "write cube successfully" << endl;
        }
            break;

        case DELETE: {
            try {
                this->dataBase->deleteObj(fp);
                fp->setFileContent("deleted!!");
            } catch (const char *ch) {
                throw ch;
            }
            response = (char *) fp;
            responseSize = sizeof(FileProp);
            cout << "deleted cube successfully" << endl;
        }
            break;
        case ALL: {
            int size = 0;
            response = (char *) this->dataBase->returnDBState(command.getOwner(), size);
            responseSize = size * sizeof(FileProp);
        }
            break;

        case SIZES: {
            int *sizes = new int[3]{NAME_SIZE, OWNER_SIZE, CONTENT_SIZE};
            response = (char *) sizes;
            responseSize = 3 * sizeof(int);
        }
            break;

        case ERROR: {
            throw "not a recognisable payload!";
        }
    }
    return 0;
}
