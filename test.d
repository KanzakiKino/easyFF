// Written without License in the D programming language.
// Copyright 2018 KanzakiKino

import std.conv,
       std.random,
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

    struct FFRational {
        int num, den;
    }

    struct FFMeta {
        const(char)* title;
        const(char)* author;
        const(char)* albumArtist;
        const(char)* album;
        const(char)* grouping;
        const(char)* composer;
        const(char)* year;
        const(char)* track;
        const(char)* comment;
        const(char)* genre;
        const(char)* copyright;
        const(char)* description;
    }

    struct FFImage;
    FFImage* FFImage_new        ( int, int, long );
    void     FFImage_delete     ( FFImage** );
    FFError  FFImage_checkError ( FFImage* );
    char*    FFImage_getBuffer  ( FFImage* );
    void     FFImage_setPts     ( FFImage*, long );

    struct FFSound;
    FFSound* FFSound_new    ( int, int, int, long );
    void     FFSound_delete ( FFSound** );
    FFError  FFSound_checkError ( FFSound* );
    float*   FFSound_getBuffer ( FFSound* );
    void     FFSound_setPts     ( FFSound*, long );

    struct FFStream;
    FFError FFStream_checkError        ( FFStream* );
    FFError FFStream_setupVideoEncoder ( FFStream*, int, int, FFRational, FFOption* );
    FFError FFStream_setupAudioEncoder ( FFStream*, int, int, FFOption* );
    FFError FFStream_getFrameSize      ( FFStream* );
    FFRational FFStream_getTimebase      ( FFStream* );

    struct FFWriter;
    FFWriter* FFWriter_new               ( const(char)* );
    void      FFWriter_delete            ( FFWriter** );
    FFError   FFWriter_checkError        ( FFWriter* );
    FFStream* FFWriter_createVideoStream ( FFWriter* );
    FFStream* FFWriter_createAudioStream ( FFWriter* );
    FFError   FFWriter_setMeta           ( FFWriter*, FFMeta* );
    FFError   FFWriter_writeHeader       ( FFWriter* );
    FFError   FFWriter_encodeImage       ( FFWriter*, FFImage* );
    FFError   FFWriter_encodeSound       ( FFWriter*, FFSound* );
    FFError   FFWriter_flush             ( FFWriter* );

    struct FFOption;
    FFOption* FFOption_new    ();
    void      FFOption_delete ( FFOption** );
    FFError   FFOption_set    ( FFOption*, const(char)*, const(char)* );

    char FF_hasVersionIntegrity ();
}

void main ()
{
    assert( FF_hasVersionIntegrity() );

    enum OutputPath = "test.mp4";
    enum VideoW = 1024, VideoH = 768;
    enum FPS = 30, DurationSec = 10;
    enum SampleRate = 48000, Channels = 2;
    enum SamplesPerFrame = 1024/Channels;

    auto writer = FFWriter_new( OutputPath );
    assert( writer && !FFWriter_checkError(writer) );

    auto option = FFOption_new();
    FFOption_set( option, "b", "50k" );

    auto video = FFWriter_createVideoStream( writer );
    assert( video && !FFStream_checkError(video) );
    FFStream_setupVideoEncoder( video, VideoW, VideoH, FFRational(1,FPS), option );
    assert( !FFStream_checkError(video) );
    FFOption_delete( &option );

    auto audio = FFWriter_createAudioStream( writer );
    assert( audio && !FFStream_checkError(audio) );
    FFStream_setupAudioEncoder( audio, Channels, SampleRate, null );
    assert( !FFStream_checkError(audio) );

    FFMeta meta;
    meta.album = "test";
    FFWriter_setMeta( writer, &meta );

    FFWriter_writeHeader( writer );
    assert( !FFWriter_checkError(writer) );

    FFImage* img = FFImage_new( VideoW, VideoH, 0 );
    assert( img && !FFImage_checkError(img) );
    auto imgBuffer = FFImage_getBuffer( img );
    for ( size_t i = 0; i < VideoW*VideoH*4; i++ ) {
        imgBuffer[i] = uniform(byte.min,byte.max);
    }
    for ( long i = 0; i < DurationSec*FPS; i++ ) {
        FFImage_setPts( img, i );
        FFWriter_encodeImage( writer, img );
        assert( !FFWriter_checkError(writer) );
    }
    FFImage_delete( &img );

    FFSound* snd = FFSound_new( SamplesPerFrame, Channels, SampleRate, 0 );
    assert( snd && !FFSound_checkError(snd) );
    auto sndBuffer = FFSound_getBuffer( snd );
    for ( size_t i = 0; i < SamplesPerFrame*Channels; i++ ) {
        sndBuffer[i] = uniform(-1f,1f);
    }
    for ( long i = 0; i < DurationSec*SampleRate; i += SamplesPerFrame ) {
        FFSound_setPts( snd, i );
        FFWriter_encodeSound( writer, snd );
        assert( !FFWriter_checkError(writer) );
    }
    FFSound_delete( &snd );

    FFWriter_flush( writer );
    assert( !FFWriter_checkError(writer) );
    FFWriter_delete( &writer );
}
