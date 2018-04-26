/* 
 * File:   DebugOut.cpp
 * Author: hans
 * 
 * Created on 8 de Setembro de 2010, 15:22
 */

#include "DebugOut.h"
#include <sstream>

DebugOut debugOut;
std::ofstream DebugOut::logfile;
const std::ofstream::openmode DebugOut::erase = std::ofstream::out;
const std::ofstream::openmode DebugOut::append = std::ofstream::out | std::ofstream::app;

int debugLevelThreshold = DEFAULT_THRESHOLD;
const std::string DBG_STR = "";


DebugOut::DebugOut() {
    msgLevel = DEFAULT_MSG_LEVEL;
}

DebugOut& DebugOut::level(int level) {
    msgLevel = level;
    return *this;
}

DebugOut& DebugOut::threshlold(int level) {
    debugLevelThreshold = level;
    return *this;
}

void DebugOut::threshold(int level) {
    debugLevelThreshold = level;
}

DebugOut& DebugOut::operator<<(std::ostream& (*stream)(std::ostream&))
{
   if (msgLevel<=debugLevelThreshold)
   {
       if (logfile.is_open())
           logfile << stream;
       else
           std::cout << stream;
   }
   
   return *this;
}

DebugOut& dbgThreshold(int level) {
    debugOut.threshlold(level);
    return debugOut;
}

DebugOut& dbgOut(int level) {
    debugOut.level(level);
    return debugOut;
}

DebugOut& dbgOut(const std::string &tag, int level) {
    debugOut.level(level);
    dbgOut(level) << tag << DBG_END;
    return debugOut;
}

std::string to_string(int i) {
    std::stringstream ss;
    ss << i;
    std::string str = ss.str();
    
    return str;
}

DebugOut& dbgToLogFile(const std::string &file_name, std::ofstream::openmode mode) {
    if (DebugOut::logfile.is_open())
        DebugOut::logfile.close();
    
     DebugOut::logfile.open(file_name.c_str(), mode);
     
     return debugOut;
}

DebugOut& dbgToStdOut() {
    
    if (DebugOut::logfile.is_open())
        DebugOut::logfile.close();
    
    return debugOut;
}
