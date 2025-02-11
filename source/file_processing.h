#ifndef FILE_PROCESSING_H
#define FILE_PROCESSING_H

struct entire_file {
    u8 *Data;
    u64 Size;
};

struct stream {
    u8 *BufferPos;
    u8 *BufferEnd;
};

//~ File reader
enum file_token_type {
    FileTokenType_None, 
    
    FileTokenType_String, 
    FileTokenType_Integer, 
    FileTokenType_Float, 
    
    FileTokenType_BeginCommand, 
    FileTokenType_Identifier,
    
    FileTokenType_BeginArguments,
    FileTokenType_EndArguments,
    
    FileTokenType_BeginSpecial,
    FileTokenType_EndSpecial,
    
    FileTokenType_Invalid, 
    FileTokenType_EndFile, 
};

enum file_reader_error {
    FileReaderError_None,
    FileReaderError_InvalidToken,
};

struct file_token {
    b8 IsValid;
    file_token_type Type;
    u32 Line;
    
    union {
        const char *Identifier;
        const char *String;
        s32 Integer;
        f32 Float;
        char Char;
    };
};

struct asset_system;
struct file_reader {
    u8 *FileStart;
    u8 *FilePos;
    u8 *FileEnd;
    u32    Line;
    asset_system *System;
    
    file_reader_error LastError;
    file_token LastToken;
    
    file_token ExpectToken(file_token_type Type);
    
    char *ConsumeTextIdentifier();
    char *ConsumeTextString();
    inline u32 ConsumeTextHexNumber();
    
    file_token NextToken();
    file_token PeekToken(u32 N=1);
};


#endif //FILE_PROCESSING_H
