/* 
 * File:   DebugOut.h
 * Author: hans
 *
 * Created on 8 de Setembro de 2010, 15:22
 */

#ifndef _DEBUGOUT_H
#define	_DEBUGOUT_H
#include <iostream>
#include <ostream>
#include <fstream>

#define DEFAULT_THRESHOLD 1 //-1 - All messages off
#define DEFAULT_MSG_LEVEL 1

extern const std::string DBG_STR;

#define DBG_SEP ">"
#define DBG_END ": "

#define DBG_LINE to_string(__LINE__)        
#define DBG_AT (DBG_STR + __FILE__ + DBG_SEP + __FUNCTION__  + DBG_SEP  + DBG_LINE)
#define dbgOutAt0() dbgOut() << DBG_AT << DBG_END
#define dbgOutAt1(level) dbgOut(level) << DBG_AT << DBG_END
#define dbgOutAt2(tag, level) dbgOut(level) << tag << DBG_SEP << DBG_AT << DBG_END
#define _ARG2(_0, _1, _2, ...) _2
#define NARG2(...) _ARG2(__VA_ARGS__, 2, 1, 0)
#define _ONE_OR_TWO_ARGS_1(a) dbgOutAt1(a)
#define _ONE_OR_TWO_ARGS_2(a, b) dbgOutAt2(a,b)
#define __ONE_OR_TWO_ARGS(N, ...) _ONE_OR_TWO_ARGS_ ## N (__VA_ARGS__)
#define _ONE_OR_TWO_ARGS(N, ...) __ONE_OR_TWO_ARGS(N, __VA_ARGS__)
#define dbgOutAt(...) _ONE_OR_TWO_ARGS(NARG2(__VA_ARGS__), __VA_ARGS__)

using std::endl;

extern int debugLevelThreshold;

class DebugOut
{
    int msgLevel;
protected:
    static std::ofstream logfile;
public:
    static const std::ofstream::openmode erase;
    static const std::ofstream::openmode append;

    DebugOut();

    DebugOut& level(int level);
    DebugOut& threshlold(int level);
    static void threshold(int level);

    template <typename T>
    DebugOut& operator << (const T &stream)
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

    DebugOut& operator<<(std::ostream& (*stream)(std::ostream&));
    friend DebugOut& dbgToLogFile(const std::string &file_name, std::ofstream::openmode mode);
    friend DebugOut& dbgToStdOut();
    DebugOut& clearLog();
};

extern DebugOut debugOut;

DebugOut& dbgOut(int level=DEFAULT_THRESHOLD);
DebugOut& dbgOut(const std::string &tag, int level=DEFAULT_THRESHOLD);
DebugOut& dbgThreshold(int level);
DebugOut& dbgToLogFile(const std::string &file_name, std::ofstream::openmode mode = DebugOut::append);
DebugOut& dbgToStdOut();
#endif	/* _DEBUGOUT_H */

std::string to_string(int i);