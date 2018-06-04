//
// Created by s17kf on 02.06.18.
//

#ifndef LOGOWANIE_LOG_H
#define LOGOWANIE_LOG_H

#include <string>
#include <cstdarg>

void initLog(std::string &logfileName, int verbo);
void logClose();
//#define log(level, ...) printf("[LOG LEVEL = %d] ", level); printf(__VA_ARGS__); printf("\n"); fflush(stdout)
void log(int level, const char *fmt, ...);

#endif //LOGOWANIE_LOG_H
