#pragma once

#include "common.h"


int initLog(bool _fileLoggingEnabled, bool _screenLoggingEnabled);
void openLogFile();
void closeLogFile();
void debug(const char *format, ...);
void panic(const char *format, ...);
void shutdown();