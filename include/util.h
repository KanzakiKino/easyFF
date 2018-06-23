// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino

#ifndef EASYFF_UTIL_H
#define EASYFF_UTIL_H

#define NULL_GUARD(T) \
    if ( !T ) return

#define ILLEGAL_GUARD(T) \
    if ( T->error != EASYFF_NOERROR ) \
        return

#define THROW(T) \
    this->error = T; \
    return T

#endif
