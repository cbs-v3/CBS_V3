/*******************************************************************
*
*    DESCRIPTION:Logging Support for CBS
*
*    AUTHOR:raidmanji
*
*    HISTORY:
*
*    DATE:2013/4/19
*
*    Copyright (C) 1998-2013 Tencent Inc. All Rights Reserved
*******************************************************************/

#ifndef CBS_DEBUG_H
#define CBS_DEBUG_H

#include <stdio.h>
#include <assert.h>

#define CBS_DEBUG_BASE          0x00000001
#define CBS_DEBUG_ISCSI         0x00000002
#define CBS_DEBUG_TSSD          0x00000004
#define CBS_DEBUG_TFS           0x00000008
#define CBS_DEBUG_CONFIG        0x00000010
#define CBS_DEBUG_HB            0x00000020
#define CBS_DEBUG_TARGET		0x00000040
#define CBS_DEBUG_CBUF          0x00000080
#define CBS_DEBUG_DEV           0x00000100

#define CBS_DEBUG_LEVEL         0x00000000

#ifdef MCP_DEBUG
    #include "tfc_debug_log.h"
    #define LOG_TRACE() DEBUG_P(LOG_TRACE, "entry function:%s\n", __FUNCTION__)
    #define LOG_DEBUG(level, module, fmt, arg...) do { if (level & module) DEBUG_P(LOG_DEBUG, fmt", [%s, %d]\n", ##arg, __FILE__, __LINE__); } while(0)
    #define LOG_NORMAL(fmt, arg...) DEBUG_P(LOG_NORMAL, fmt", [%s, %d]\n", ##arg, __FILE__, __LINE__)
    #define LOG_ERROR(fmt, arg...) DEBUG_P(LOG_ERROR, fmt", [%s, %d]\n", ##arg, __FILE__, __LINE__)
    #define LOG_FATAL(fmt, arg...) DEBUG_P(LOG_FATAL, fmt", [%s, %d]\n", ##arg, __FILE__, __LINE__)
#else
    #define LOG_TRACE() printf("entry function:%s\n", __FUNCTION__)
    #define LOG_DEBUG(level, module, fmt, arg...) do { if (level & module) printf(fmt", [%s, %d]\n", ##arg, __FILE__, __LINE__); } while(0)
    #define LOG_NORMAL(fmt, arg...) printf(fmt",[%s, %d]\n", ##arg, __FILE__, __LINE__)
    #define LOG_ERROR(fmt, arg...) printf(fmt", [%s, %d]\n", ##arg, __FILE__, __LINE__)
    #define LOG_FATAL(fmt, arg...) printf(fmt", [%s, %d]\n", ##arg, __FILE__, __LINE__)
#endif //MCP_DEBUG

#endif

