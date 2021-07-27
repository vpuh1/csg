#ifndef LIMITS_H
#define LIMITS_H

#ifdef __APPLE__
#include <sys/syslimits.h>
#elif __linux__
#include <linux/limits.h>
#elif __FreeBSD__
#include <sys/syslimits.h>
#endif 

#define CSG_PATH_MAX PATH_MAX
#define CSG_NAME_MAX NAME_MAX

#ifdef __linux__
#define CSG_ARG_MAX sysconf(_SC_ARG_MAX)
#else
#define CSG_ARG_MAX ARG_MAX
#endif

#endif /* LIMITS_H */
