// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino

#ifndef EASYFF_FFOPTION_H
#define EASYFF_FFOPTION_H

#include "fferror.h"

typedef struct FFOption FFOption;

#ifdef EASYFF_PROTECTED
    typedef struct AVDictionary AVDictionary;
    AVDictionary** FFOption_getAVDic ( FFOption* );
#endif

FFOption* FFOption_new    ();
void      FFOption_delete ( FFOption** );

FFError FFOption_set ( FFOption*, const char*, const char* );

#endif
