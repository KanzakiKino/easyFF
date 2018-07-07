// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino

#ifndef EASYFF_VERSION_H
#define EASYFF_VERSION_H

#define EASYFF_VERSION_MAJOR 1
#define EASYFF_VERSION_MINOR 0
#define EASYFF_VERSION_MICRO 2

// Supported major versions of libraries.
#define EASYFF_AVUTIL_VERSION     56
#define EASYFF_AVCODEC_VERSION    58
#define EASYFF_AVFORMAT_VERSION   58
#define EASYFF_SWRESAMPLE_VERSION 3
#define EASYFF_SWSCALE_VERSION    5

unsigned int FF_getMajorVersion ();
unsigned int FF_getMinorVersion ();
unsigned int FF_getMicroVersion ();

char FF_hasVersionIntegrity ();

#endif
