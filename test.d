// Written without License in the D programming language.
// Copyright 2018 KanzakiKino

import std.conv,
       std.stdio;
pragma(lib,"avutil");
pragma(lib,"avcodec");
pragma(lib,"avformat");
pragma(lib,"easyff");
pragma(lib,"swresample");
pragma(lib,"swscale");

extern(C)
{
    alias FFError = ubyte;

    struct FFRational
    {
        int num,den;
    }

    struct FFMeta
    {
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
    }

    struct FFReader;
    FFReader* FFReader_new                 ( const(char)* );
    void      FFReader_delete              ( FFReader** );
    FFError   FFReader_checkError          ( FFReader* );
    FFMeta    FFReader_getMeta             ( FFReader* );
    FFStream* FFReader_findAudioStream     ( FFReader* );
    byte      FFReader_decode              ( FFReader*, FFStream* );
    FFSound*  FFReader_convertFrameToSound ( FFReader* );

    struct FFSound;
    void    FFSound_delete        ( FFSound** );
    FFError FFSound_checkError    ( FFSound* );
    int     FFSound_getSamples    ( FFSound* );
    int     FFSound_getChannels   ( FFSound* );
    int     FFSound_getSampleRate ( FFSound* );
    float*  FFSound_getBuffer     ( FFSound* );

    struct FFStream;
    FFError    FFStream_checkError  ( FFStream* );
    long FFStream_getDuration ( FFStream* );
}

void main ()
{
    auto reader = FFReader_new( "/home/kinok/sounds/test.mp3" );
    auto meta = FFReader_getMeta( reader );
    assert( !FFReader_checkError( reader ) );
    "Album: %s".writefln( meta.album.to!string );

    auto audio = FFReader_findAudioStream( reader );
    assert( !FFStream_checkError( audio ) );

    while ( FFReader_decode( reader, audio ) ) {
        auto snd = FFReader_convertFrameToSound( reader );
        assert( !FFSound_checkError( snd ) );
        FFSound_delete( &snd );
    }
    FFReader_delete( &reader );
}
