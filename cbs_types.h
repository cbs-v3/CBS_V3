/*******************************************************************
*
*    DESCRIPTION:
*
*    AUTHOR:raidmanji
*
*    HISTORY:
*
*    DATE:2013/4/19
*
*    Copyright (C) 1998-2013 Tencent Inc. All Rights Reserved
*******************************************************************/

#ifndef CBS_TYPES_H
#define CBS_TYPES_H

//basic type
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long       u32_64;
typedef unsigned long long  uint64;

typedef signed char     sint8;
typedef signed short    sint16;
typedef signed int      sint32;
typedef signed long     s32_64;
typedef long long       sint64;

//return code
typedef uint32          RT_STATUS;

#define RT_OK              (uint32) 0
#define RT_FAIL            (uint32) 1
#define RT_NO_EVENTS       (uint32) 2
#define RT_BAD_ARG         (uint32) 3
#define RT_BAD_SERV_ID     (uint32) 4
#define RT_BAD_SEQUENCE    (uint32) 5
#define RT_ALLOC_FAILED    (uint32) 6
#define RT_NOT_ALL_OK      (uint32) 7
#define RT_BUSY            (uint32) 8

#define LOU64(x) (uint32)((uint64)(x) & 0x00000000ffffffff)
#define HIU64(x) (uint32)(((uint64)(x) >> 32) & 0x00000000ffffffff)
#define U64(hi,lo) (uint64)(((uint64)(hi) << 32 ) | (uint64)(lo))


#endif

