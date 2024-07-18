#ifndef IS_CREATED_H
#define IS_CREATED_H

#include <sys/stat.h>

#ifdef _WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

#endif