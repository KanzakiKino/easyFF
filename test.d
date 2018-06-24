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
    FFStream_setupAudioEncoder( audio, 2, 44100, FFRational(1,30) );
    assert( !FFStream_checkError(audio) );

    FFWriter_writeHeader( writer );
    assert( !FFWriter_checkError(writer) );

    FFWriter_delete( &writer );
}
