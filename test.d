// Written without License in the D programming language.
// Copyright 2018 KanzakiKino

import std.stdio;
pragma(lib,"avutil");
pragma(lib,"avcodec");
pragma(lib,"avformat");
pragma(lib,"easyff");
pragma(lib,"swscale");

extern(C)
{
    alias FFError = ubyte;

    struct FFImage;
    int     FFImage_getWidth  ( FFImage* );
    int     FFImage_getHeight ( FFImage* );
    FFError FFImage_checkError ( FFImage* );

    struct FFReader;
    FFReader* FFReader_new                 ( const(char)* );
    void      FFReader_delete              ( FFReader** );
    FFError   FFReader_checkError          ( FFReader* );
    FFStream* FFReader_findVideoStream     ( FFReader* );
    FFStream* FFReader_findAudioStream     ( FFReader* );
    byte      FFReader_decode              ( FFReader*, FFStream* );
    FFImage*  FFReader_convertFrameToImage ( FFReader* );

    struct FFStream;
    FFError FFStream_checkError ( FFStream* );
}

void main ()
{
    FFReader* reader = FFReader_new( "/home/kinok/pics/test.jpg" );
    assert( reader );

    auto video = FFReader_findVideoStream( reader );
    assert( video );
    "VideoStream Status: %d".writefln( FFStream_checkError( video ) );

    while ( FFReader_decode( reader, video ) ) {
        auto image = FFReader_convertFrameToImage( reader );
        FFImage_checkError(image).writeln;
        "%d,%d".writefln( FFImage_getWidth(image), FFImage_getHeight(image) );
    }
    "Reader Status: %d".writefln( FFReader_checkError( reader ) );

    FFReader_delete( &reader );
}
