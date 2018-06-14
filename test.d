// Written without License in the D programming language.
// Copyright 2018 KanzakiKino

import std.stdio;
pragma(lib,"avutil");
pragma(lib,"avcodec");
pragma(lib,"avformat");
pragma(lib,"easyff");

extern(C)
{
    alias FFError = ubyte;

    struct FFStream;
    FFError FFStream_checkError ( FFStream* );

    struct FFReader;
    FFReader* FFReader_new                ( const(char)* );
    void      FFReader_delete             ( FFReader** );
    FFError   FFReader_checkError         ( FFReader* );
    FFStream* FFReader_findVideoStream    ( FFReader* );
    FFStream* FFReader_findAudioStream    ( FFReader* );
    byte      FFReader_decode             ( FFReader*, FFStream* );
}

void main ()
{
    FFReader* reader = FFReader_new( "/home/kinok/pics/test.bmp" );
    assert( reader );

    auto video = FFReader_findVideoStream( reader );
    assert( video );
    "VideoStream Status: %d".writefln( FFStream_checkError( video ) );

    while ( FFReader_decode( reader, video ) ) {
        "hoge".writeln;
    }
    "Reader Status: %d".writefln( FFReader_checkError( reader ) );

    FFReader_delete( &reader );
}
