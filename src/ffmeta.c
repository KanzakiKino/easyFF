// Written under LGPL-3.0 in the C language.
// Copyright 2018 KanzakiKino
#define EASYFF_PROTECTED

#include <ffmeta.h>
#include <util.h>

#include <libavutil/dict.h>

#include <stdlib.h>

#define DICT_FLAG AV_DICT_MATCH_CASE

const char* FFMeta_getValue ( const AVDictionary* dic, const char* key )
{
    AVDictionaryEntry* entry = av_dict_get( dic, key, NULL, DICT_FLAG );
    if ( entry ) {
        return entry->value;
    } else {
        return NULL;
    }
}

FFError FFMeta_assignAVDic ( FFMeta* meta, const AVDictionary* dic )
{
    NULL_GUARD( meta ) EASYFF_ERROR_NULL_POINTER;

    meta->title       = FFMeta_getValue( dic, "title" );
    meta->author      = FFMeta_getValue( dic, "author" );
    meta->albumArtist = FFMeta_getValue( dic, "album_artist" );
    meta->album       = FFMeta_getValue( dic, "album" );
    meta->grouping    = FFMeta_getValue( dic, "grouping" );
    meta->composer    = FFMeta_getValue( dic, "composer" );
    meta->year        = FFMeta_getValue( dic, "year" );
    meta->track       = FFMeta_getValue( dic, "track" );
    meta->comment     = FFMeta_getValue( dic, "comment" );
    meta->genre       = FFMeta_getValue( dic, "genre" );
    meta->copyright   = FFMeta_getValue( dic, "copyright" );
    meta->description = FFMeta_getValue( dic, "description" );
    return EASYFF_NOERROR;
}
