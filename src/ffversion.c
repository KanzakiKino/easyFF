// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino
#define EASYFF_PROTECTED

#include <ffversion.h>

#include <libavutil/avutil.h>
#include <libavutil/version.h>
#include <libavcodec/avcodec.h>
#include <libavcodec/version.h>
#include <libavformat/avformat.h>
#include <libavformat/version.h>
#include <libswresample/swresample.h>
#include <libswresample/version.h>
#include <libswscale/swscale.h>
#include <libswscale/version.h>

#if EASYFF_AVUTIL_VERSION != LIBAVUTIL_VERSION_MAJOR
#warning "libavutil version is incompatible."
#endif

#if EASYFF_AVCODEC_VERSION != LIBAVCODEC_VERSION_MAJOR
#warning "libavcodec version is incompatible."
#endif

#if EASYFF_AVFORMAT_VERSION != LIBAVFORMAT_VERSION_MAJOR
#warning "libavformat version is incompatible."
#endif

#if EASYFF_SWRESAMPLE_VERSION != LIBSWRESAMPLE_VERSION_MAJOR
#warning "libswresample version is incompatible."
#endif

#if EASYFF_SWSCALE_VERSION != LIBSWSCALE_VERSION_MAJOR
#warning "libswscale version is incompatible."
#endif

unsigned int FF_getMajorVersion ()
{
    return EASYFF_VERSION_MAJOR;
}
unsigned int FF_getMinorVersion ()
{
    return EASYFF_VERSION_MINOR;
}
unsigned int FF_getMicroVersion ()
{
    return EASYFF_VERSION_MICRO;
}

char FF_hasVersionIntegrity ()
{
    if ( EASYFF_VERSION_MAJOR != FF_getMajorVersion() ) {
        return 0;
    }
    if ( EASYFF_VERSION_MINOR != FF_getMinorVersion() ) {
        return 0;
    }
    if ( EASYFF_VERSION_MICRO != FF_getMicroVersion() ) {
        return 0;
    }

    if ( EASYFF_AVUTIL_VERSION != AV_VERSION_MAJOR(avutil_version()) ) {
        return 0;
    }
    if ( EASYFF_AVFORMAT_VERSION != AV_VERSION_MAJOR(avformat_version()) ) {
        return 0;
    }
    if ( EASYFF_AVCODEC_VERSION != AV_VERSION_MAJOR(avcodec_version()) ) {
        return 0;
    }
    if ( EASYFF_SWRESAMPLE_VERSION != AV_VERSION_MAJOR(swresample_version()) ) {
        return 0;
    }
    if ( EASYFF_SWSCALE_VERSION != AV_VERSION_MAJOR(swscale_version()) ) {
        return 0;
    }
    return 1;
}
