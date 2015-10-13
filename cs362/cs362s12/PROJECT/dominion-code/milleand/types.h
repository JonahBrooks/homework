#ifndef TYPES_H
#define TYPES_H

#ifndef __cplusplus
#ifdef WIN32
typedef unsigned char bool;
#define false 0
#define true !false
#else
#include <stdbool.h>
#endif // WIN32
#endif // __cplusplus

#endif // TYPES_H
