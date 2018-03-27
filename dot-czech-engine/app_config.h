/*
* app_config.h
* app_config header file
*
* Author: Tony Hong
*
* Copyright (c) Dot Incorporation.
*/
#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include <stdint.h>
#define MAX_LANGUAGE 2

typedef struct
{
    uint8_t code, rule, grade;
} language_t;

typedef struct
{
    uint8_t 	support;
    uint8_t 	major;
    language_t 	item[MAX_LANGUAGE];
} language_config_t;

typedef struct
{
    language_config_t	lang;
} app_config_t;

extern app_config_t* m_app_config;

#endif