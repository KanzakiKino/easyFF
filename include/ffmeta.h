// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino

#ifndef EASYFF_FFMETA_H
#define EASYFF_FFMETA_H

#include "fferror.h"

typedef struct AVDictionary AVDictionary;
typedef struct {
    const char* title;
    const char* author;
    const char* albumArtist;
    const char* album;
    const char* grouping;
    const char* composer;
    const char* year;
    const char* track;
    const char* comment;
    const char* genre;
    const char* copyright;
    const char* description;
} FFMeta;

#define FFMETA_INIT {\
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,\
    NULL,NULL,NULL\
}

#ifdef EASYFF_PROTECTED
    FFError FFMeta_assignAVDic ( FFMeta* meta, const AVDictionary* );
#endif

#endif
