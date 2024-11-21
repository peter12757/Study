#ifndef COMMON_H
#define COMMON_H
// extern "C"
// {
// #include <libavcodec/avcodec.h>
// }

#include <iostream>
using namespace std;
static int typePlatform =0;
#ifdef _WIN32
    #ifdef WIN32
        typePlatform =1;    //win32
    #else
typePlatform =2;    //win64
    #endif
#else
typePlatform =3;    //linux
#endif


#endif // COMMON_H
