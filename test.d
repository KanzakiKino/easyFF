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

    struct FFRational {
        int num, den;
    }

    struct FFImage;
    FFImage* FFImage_new        ( int, int, long );
    void     FFImage_delete     ( FFImage** );
    FFError  FFImage_checkError ( FFImage* );
    void     FFImage_setPts     ( FFImage*, long );

    struct FFSound;
    FFSound* FFSound_new    ( int, int, int, long );
    void     FFSound_delete ( FFSound** );
    FFError  FFSound_checkError ( FFSound* );
    float*   FFSound_getBuffer ( FFSound* );
    void     FFSound_setPts     ( FFSound*, long );

    struct FFStream;
    FFError FFStream_checkError   ( FFStream* );
    FFError FFStream_setupVideoEncoder ( FFStream*, int, int, FFRational );
    FFError FFStream_setupAudioEncoder ( FFStream*, int, int, FFRational );

    struct FFWriter;
    FFWriter* FFWriter_new               ( const(char)* );
    void      FFWriter_delete            ( FFWriter** );
    FFError   FFWriter_checkError        ( FFWriter* );
    FFStream* FFWriter_createVideoStream ( FFWriter* );
    FFStream* FFWriter_createAudioStream ( FFWriter* );
    FFError   FFWriter_writeHeader       ( FFWriter* );
    FFError   FFWriter_encodeImage       ( FFWriter*, FFImage* );
    FFError   FFWriter_encodeSound       ( FFWriter*, FFSound* );
    FFError   FFWriter_flush             ( FFWriter* );
}

void main ()
{
    auto writer = FFWriter_new( "test.mp4" );
    assert( writer && !FFWriter_checkError(writer) );

    auto video = FFWriter_createVideoStream( writer );
    assert( video && !FFStream_checkError(video) );
    FFStream_setupVideoEncoder( video, 100, 100, FFRational(1,30) );
    assert( !FFStream_checkError(video) );

    auto audio = FFWriter_createAudioStream( writer );
    assert( audio && !FFStream_checkError(audio) );
    FFStream_setupAudioEncoder( audio, 1, 44100, FFRational(1,30*1024) );
    assert( !FFStream_checkError(audio) );

    FFWriter_writeHeader( writer );
    assert( !FFWriter_checkError(writer) );

    FFImage* img = FFImage_new( 100, 100, 0 );
    assert( img && !FFImage_checkError(img) );

    FFSound* snd = FFSound_new( 1024, 1, 44100, 0 );
    assert( snd && !FFSound_checkError(snd) );
    auto sndBuffer = FFSound_getBuffer( snd );
    for ( size_t i = 0; i < 1024; i++ ) {
        import std.random;
        sndBuffer[i] = uniform(-1f,1f);
    }

    for ( long i = 0; i < 100; i++ ) {
        FFImage_setPts( img, i );
        FFWriter_encodeImage( writer, img );
        assert( !FFWriter_checkError(writer) );
        FFSound_setPts( snd, i*1024 );
        FFWriter_encodeSound( writer, snd );
        assert( !FFWriter_checkError(writer) );
    }

    FFImage_delete( &img );
    FFSound_delete( &snd );

    FFWriter_flush( writer );
    assert( !FFWriter_checkError(writer) );
    FFWriter_delete( &writer );
}
