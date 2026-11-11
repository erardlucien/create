#ifndef IS_CREATED_H
#define IS_CREATED_H

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#include <limits.h>
#endif

#endif
