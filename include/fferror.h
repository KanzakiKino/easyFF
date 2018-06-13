// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino

#ifndef EASYFF_ERROR_H
#define EASYFF_ERROR_H

typedef unsigned char FFError;

#define EASYFF_NOERROR              0x00
#define EASYFF_ERROR_NULL_POINTER   0x01
#define EASYFF_ERROR_ILLEGAL_OBJECT 0x02
#define EASYFF_ERROR_CREATE_CONTEXT 0x03
#define EASYFF_ERROR_STREAM         0x04
#define EASYFF_ERROR_NO_CODEC       0x05
#define EASYFF_ERROR_PACKET_LOST    0x06

#endif
