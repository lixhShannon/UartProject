#ifndef __SL_SYSTEM_H__
#define __SL_SYSTEM_H__


#ifdef MAINC
#define EX
#else 
#define EX extern 
#endif
/****************************************************************************
                            macro definition
****************************************************************************/
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef signed char        int_least8_t;
typedef short              int_least16_t;
typedef int                int_least32_t;
typedef long long          int_least64_t;
typedef unsigned char      uint_least8_t;
typedef unsigned short     uint_least16_t;
typedef unsigned int       uint_least32_t;
typedef unsigned long long uint_least64_t;

typedef signed char        int_fast8_t;
typedef int                int_fast16_t;
typedef int                int_fast32_t;
typedef long long          int_fast64_t;
typedef unsigned char      uint_fast8_t;
typedef unsigned int       uint_fast16_t;
typedef unsigned int       uint_fast32_t;
typedef unsigned long long uint_fast64_t;

typedef long long          intmax_t;
typedef unsigned long long uintmax_t;

typedef union{
  uint8_t byte;
  struct{
    unsigned Low:4;
    unsigned High:4;
  };
}BYTETOHALF;

#include "stdio.h"
#include "uart_send.h"
#include "stdafx.h"
#include "stdlib.h"
#include <time.h>
#include <sys/timeb.h>
#include <windows.h>
#include <iostream> 
#include "SerialPort.h"

#endif
