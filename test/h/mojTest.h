#ifndef MOJTEST_H_
#define MOJTEST_H_

#include "kernel.h"


extern int x, y, z;
extern Semaphore xySem;
extern Semaphore zSem;
extern Semaphore fSem;

extern int userMain00(int argc, char **args);
extern int userMain01(int argc, char **args);
extern int userMain02(int argc, char **args);
extern int userMain03(int argc, char **args);
extern int userMain04(int argc, char **args);
extern int userMain05(int argc, char **args);

#endif /* MOJTEST_H_ */
