#ifndef __LOGGER_CLASS_H__
#define __LOGGER_CLASS_H__

#include <string>
#include <fstream>

#define LOG(X) myLogger.log(X)

class Logger {
    public:
        Logger(std::string fName);
        ~Logger();

        void log( std::string line );

    private:
        std::ofstream *myFile;
};

extern Logger myLogger;

#endif
