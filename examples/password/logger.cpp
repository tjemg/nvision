#include "logger.h"

Logger myLogger("logging.txt");

Logger::Logger(std::string fName) {
    myFile = new std::ofstream(fName,std::ofstream::binary);
}

Logger::~Logger() {
    myFile->close();
    delete myFile;
}

void Logger::log( std::string line ) {
    myFile->write(line.c_str(),line.length());
    myFile->write("\n",1);
    myFile->flush();
}


