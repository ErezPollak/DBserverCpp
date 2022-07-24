//
// Created by pwlqr on 23/07/2022.
//

#include "BL.h"
#include "Cube.h"

FileProp *BL::operateCommand(const char *buff) {
    Command command(buff);

    FileProp *fp = new FileProp;
    fp->setFileName(command.getName().c_str())->setFileOwner(command.getOwner().c_str());

    CommandType type = command.recognizeType(this->dataBase.doesFileExist(fp));

    switch (type) {
        case CREAT_DEFAULT: {
            strncpy(fp->getFileContentNonConst(), Cube::DEFAULT_CUBE, CONTENT_SIZE);
            Cube::cubeOperation(fp->getFileContentNonConst(), &command.getCommand()[0]);
            try {
                this->dataBase.creat(fp);
            } catch (const char *ch) {
                throw ch;
            }
            cout << "created default cube and operated successfully" << endl;
        }
            break;
        case CREATE_INPUT: {
            strncpy(fp->getFileContentNonConst(), command.getState().c_str(), CONTENT_SIZE);
            Cube::cubeOperation(fp->getFileContentNonConst(), &command.getCommand()[0]);
            try {
                this->dataBase.creat(fp);
            } catch (const char *ch) {
                throw ch;
            }
            cout << "created input cube and operated successfully" << endl;

        }
            break;
        case READ: {
            try {
                this->dataBase.read(fp);
            } catch (const char *ch) {
                throw ch;
            }
            cout << "read cube successfully" << endl;
        }
            break;
        case WRITE: {
            strncpy(fp->getFileContentNonConst(), command.getState().c_str(), CONTENT_SIZE);
            Cube::cubeOperation(fp->getFileContentNonConst(), &command.getCommand()[0]);
            try {
                this->dataBase.write(fp);
            } catch (const char *ch) {
                throw ch;
            }
            cout << "write cube successfully" << endl;
        }
            break;
        case RESET: {
            strncpy(fp->getFileContentNonConst(), Cube::DEFAULT_CUBE, CONTENT_SIZE);
            try {
                this->dataBase.write(fp);
            } catch (const char *ch) {
                throw ch;
            }
            cout << "reset cube to default successfully" << endl;
        }
            break;
        case DELETE: {
            try {
                this->dataBase.deleteObj(fp);
            } catch (const char *ch) {
                throw ch;
            }
            cout << "deleted cube successfully" << endl;
        }
            break;
        case ERROR: {
            throw "not a recognisable state!";
        }
    }
    return fp;
}

FileProp * BL::dataBaseState(int &size) const {
    return this->dataBase.returnDBState(size);
}
