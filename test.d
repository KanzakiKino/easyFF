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

    struct FFStream;
    FFError FFStream_checkError ( FFStream* );

    struct FFWriter;
    FFWriter* FFWriter_new               ( const(char)* );
    void      FFWriter_delete            ( FFWriter** );
    FFError   FFWriter_checkError        ( FFWriter* );
    FFStream* FFWriter_createVideoStream ( FFWriter* );
    FFStream* FFWriter_createAudioStream ( FFWriter* );
}

void main ()
{
    auto writer = FFWriter_new( "test.png" );
    assert( writer && !FFWriter_checkError(writer) );

    auto stream = FFWriter_createVideoStream( writer );
    FFStream_checkError(stream).writeln;
    assert( stream && !FFStream_checkError(stream) );

    FFWriter_delete( &writer );
}
