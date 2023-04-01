#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

#define ERROR(_function, _condition, _message, _return)                                 \
if(_condition)                                                                          \
{                                                                                       \
    printf("%s:%d: " #_function "() => error " _message "\n", __FILE__, __LINE__);      \
    _return;                                                                            \
}

#endif