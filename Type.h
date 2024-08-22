/*****************************************************************************
 FILE BEGIN: type.h
 *****************************************************************************/
#ifndef __COMMON_TYPE_H__
#define __COMMON_TYPE_H__

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 /*
  * Symbol Definition
  */
#ifndef NULL
#define NULL    0
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef ERROR
#define ERROR   -1
#endif

#ifndef OK
#define OK      0
#endif

#ifndef true
#define true    1
#endif // !true

#ifndef false
#define false    0
#endif // !true


  /*
   * Data Type Declaration
   */
#ifndef uint64
typedef uint64_t          uint64;
#endif

#ifndef int64
typedef int64_t           int64;
#endif

#ifndef uint32
typedef uint32_t          uint32;
#endif

#ifndef int32
typedef int32_t           int32;
#endif

#ifndef uint16
typedef uint16_t          uint16;
#endif

#ifndef int16
typedef int16_t           int16;
#endif

#ifndef uint8
typedef uint8_t           uint8;
#endif

#ifndef int8
typedef int8_t            int8;
#endif

//#define __DEBUG__

#if 0

#ifdef __DEBUG__
#define DEBUG_LOG(fmt, args...) \
    printf("DEBUG: [%s:%d] DbgMsg:"fmt"\r\n", \
        __FUNCTION__, __LINE__, ##args)
#define DEBUG_ERR(errno,fmt,args...) \
    printf("ERROR: [%s:%d] ErrNum:%d ErrMsg:"fmt"\r\n", \
        __FUNCTION__, __LINE__, errno, ##args)
#else
#define DEBUG_LOG(fmt,args...)
#define DEBUG_ERR(fmt,args...)
#endif

#else

#ifdef __DEBUG__
#define DEBUG_LOG(fmt,...) \
    printf("DEBUG: [%s:%d] DbgMsg:"fmt"\r\n", \
        __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define DEBUG_ERR(errno,fmt,...) \
    printf("ERROR: [%s:%d] ErrNum:%d ErrMsg:"fmt"\r\n", \
        __FUNCTION__, __LINE__, errno, ##__VA_ARGS__)
#else
#define DEBUG_LOG(fmt,...)
#define DEBUG_ERR(errno,fmt,...)
#endif

#endif

#define DEBUG_TEXT_OUT_CHN  1

//��������:���arg==lmt�򷵻�rtn
#define ARG_CHK_EQ(arg, lmt, rtn) \
do{if((arg) == (lmt)){DEBUG_LOG("ARG_CHK_EQ"); return(rtn);}}while(0)

//��������:���arg!=lmt�򷵻�rtn
#define ARG_CHK_NEQ(arg, lmt, rtn) \
do{if((arg) != (lmt)){DEBUG_LOG("ARG_CHK_NEQ"); return(rtn);}}while(0)

//��������:���arg>lmt�򷵻�rtn
#define ARG_CHK_GT(arg, lmt, rtn) \
do{if((arg) > (lmt)){DEBUG_LOG("ARG_CHK_GT"); return(rtn);}}while(0)

//��������:���arg<lmt�򷵻�rtn
#define ARG_CHK_LT(arg, lmt, rtn) \
do{if((arg) < (lmt)){DEBUG_LOG("ARG_CHK_LT"); return(rtn);}}while(0)

//��������:���arg>=lmt�򷵻�rtn
#define ARG_CHK_GEQ(arg, lmt, rtn) \
do{if((arg) >= (lmt)){DEBUG_LOG("ARG_CHK_GEQ"); return(rtn);}}while(0)

//��������:���arg<=lmt�򷵻�rtn
#define ARG_CHK_LEQ(arg, lmt, rtn) \
do{if((arg) <= (lmt)){DEBUG_LOG("ARG_CHK_LEQ"); return(rtn);}}while(0)
#endif /* __COMMON_TYPE_H__ */
/*****************************************************************************
 FILE END: type.h
 *****************************************************************************/
