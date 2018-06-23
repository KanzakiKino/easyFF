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

    struct FFWriter;
    FFWriter* FFWriter_new                 ( const(char)* );
    void      FFWriter_delete              ( FFWriter** );
    FFError   FFWriter_checkError          ( FFWriter* );
}

void main ()
{
    auto writer = FFWriter_new( "test.mp3" );
    assert( writer && !FFWriter_checkError(writer) );
    FFWriter_delete( &writer );
}
