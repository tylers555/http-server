#if !defined(SNAIL_JUMPY_USE_PROCESSED_ASSETS)

//~ Initialization
void
asset_loader::Initialize(memory_arena *Arena, asset_system *Assets, 
                         audio_mixer *Mixer_, world_manager *Worlds_,
                         player_data *PlayerData_, enemy_data *EnemyData_){
    InProgress.Initialize(Arena);
    MainAssets = Assets;
    Mixer  = Mixer_;
    Worlds = Worlds_;
    PlayerData = PlayerData_;
    EnemyData  = EnemyData_;
    
    ASCIITable = MakeHashTable<const char *, char>(Arena, 128);
    HashTableAdd(&ASCIITable, "SPACE",                ' ');
    HashTableAdd(&ASCIITable, "EXCLAMATION",          '!');
    HashTableAdd(&ASCIITable, "QUOTATION",            '"');
    HashTableAdd(&ASCIITable, "POUND",                '#');
    HashTableAdd(&ASCIITable, "APOSTROPHE",           '\'');
    HashTableAdd(&ASCIITable, "PARENTHESIS_LEFT",     '(');
    HashTableAdd(&ASCIITable, "PARENTHESIS_RIGHT",    ')');
    HashTableAdd(&ASCIITable, "ASTERISK",             '*');
    HashTableAdd(&ASCIITable, "PLUS",                 '+');
    HashTableAdd(&ASCIITable, "COMMA",                ',');
    HashTableAdd(&ASCIITable, "DASH",                 '-');
    HashTableAdd(&ASCIITable, "PERIOD",               '.');
    HashTableAdd(&ASCIITable, "SLASH",                '/');
    HashTableAdd(&ASCIITable, "ZERO",                 '0');
    HashTableAdd(&ASCIITable, "ONE",                  '1');
    HashTableAdd(&ASCIITable, "TWO",                  '2');
    HashTableAdd(&ASCIITable, "THREE",                '3');
    HashTableAdd(&ASCIITable, "FOUR",                 '4');
    HashTableAdd(&ASCIITable, "FIVE",                 '5');
    HashTableAdd(&ASCIITable, "SIX",                  '6');
    HashTableAdd(&ASCIITable, "SEVEN",                '7');
    HashTableAdd(&ASCIITable, "EIGHT",                '8');
    HashTableAdd(&ASCIITable, "NINE",                 '9');
    HashTableAdd(&ASCIITable, "COLON",                ':');
    HashTableAdd(&ASCIITable, "SEMICOLON",            ';');
    HashTableAdd(&ASCIITable, "ANGLE_BRACKET_LEFT",   '<');
    HashTableAdd(&ASCIITable, "EQUAL",                '=');
    HashTableAdd(&ASCIITable, "ANGLE_BRACKET_RIGHT",  '>');
    HashTableAdd(&ASCIITable, "QUESTION",             '?');
    HashTableAdd(&ASCIITable, "A",                    'A');
    HashTableAdd(&ASCIITable, "B",                    'B');
    HashTableAdd(&ASCIITable, "C",                    'C');
    HashTableAdd(&ASCIITable, "D",                    'D');
    HashTableAdd(&ASCIITable, "E",                    'E');
    HashTableAdd(&ASCIITable, "F",                    'F');
    HashTableAdd(&ASCIITable, "G",                    'G');
    HashTableAdd(&ASCIITable, "H",                    'H');
    HashTableAdd(&ASCIITable, "I",                    'I');
    HashTableAdd(&ASCIITable, "J",                    'J');
    HashTableAdd(&ASCIITable, "K",                    'K');
    HashTableAdd(&ASCIITable, "L",                    'L');
    HashTableAdd(&ASCIITable, "M",                    'M');
    HashTableAdd(&ASCIITable, "N",                    'N');
    HashTableAdd(&ASCIITable, "O",                    'O');
    HashTableAdd(&ASCIITable, "P",                    'P');
    HashTableAdd(&ASCIITable, "Q",                    'Q');
    HashTableAdd(&ASCIITable, "R",                    'R');
    HashTableAdd(&ASCIITable, "S",                    'S');
    HashTableAdd(&ASCIITable, "T",                    'T');
    HashTableAdd(&ASCIITable, "U",                    'U');
    HashTableAdd(&ASCIITable, "V",                    'V');
    HashTableAdd(&ASCIITable, "W",                    'W');
    HashTableAdd(&ASCIITable, "X",                    'X');
    HashTableAdd(&ASCIITable, "Y",                    'Y');
    HashTableAdd(&ASCIITable, "Z",                    'Z');
    HashTableAdd(&ASCIITable, "SQUARE_BRACKET_LEFT",  '[');
    HashTableAdd(&ASCIITable, "BACKSLASH",            '\\');
    HashTableAdd(&ASCIITable, "SQUARE_BRACKET_RIGHT", ']');
    HashTableAdd(&ASCIITable, "CARET",                '^');
    HashTableAdd(&ASCIITable, "BACK_TICK",            '`');
    HashTableAdd(&ASCIITable, "UNDERSCORE",           '_');
    HashTableAdd(&ASCIITable, "CURLY_BRACKET_LEFT",   '{');
    HashTableAdd(&ASCIITable, "PIPE",                  '|');
    HashTableAdd(&ASCIITable, "CURLY_BRACKET_RIGHT",  '}');
    HashTableAdd(&ASCIITable, "TILDE",                '~');
    HashTableAdd(&ASCIITable, "PERCENT",              '%');
    HashTableAdd(&ASCIITable, "DOLLAR_SIGN",          '$');
    HashTableAdd(&ASCIITable, "AMPERSAND",            '&');
    HashTableAdd(&ASCIITable, "AT_SIGN",              '@');
    
    TagTable = MakeHashTable<const char *, asset_tag_id>(Arena, AssetTag_TOTAL);
#define ASSET_TAG(S, N) HashTableAdd(&TagTable, S, AssetTag_##N);
    ASSET_TAGS;
#undef ASSET_TAG
}
//~ Base

#define HandleToken(Token)                   \
if(Token.Type == FileTokenType_BeginCommand) break; \
if(Token.Type == FileTokenType_EndFile)      break; \
if(Token.Type == FileTokenType_Invalid)      break; 

#define SJA_ERROR_BEHAVIOR_FUNCTION(ErrorResult) { \
SeekEndOfFunction(); \
return ErrorResult; }

#define SJA_ERROR_BEHAVIOR_ATTRIBUTE { \
Result = AssetLoadingStatus_Warnings; \
SeekNextAttribute(); \
continue; }

#define SJA_ERROR_BEHAVIOR_COMMAND { \
SeekNextCommand(); \
return AssetLoadingStatus_Warnings; }

#define SJA_HANDLE_ERROR_(Reader, ExpectedType, ...) \
if((Reader)->LastError == FileReaderError_InvalidToken){ \
LogWarning("Expected '%s', instead read: '%s'", TokenTypeName(ExpectedType), TokenToString((Reader)->LastToken)); \
__VA_ARGS__; \
}

#define SJA_EXPECT_TOKEN(Reader, Expected, ErrorResult) { \
file_token Token = (Reader)->ExpectToken(Expected); \
SJA_HANDLE_ERROR_(Reader, Expected, ErrorResult); \
}

#define SJA_EXPECT_UINT(Reader, ...) \
ExpectPositiveInteger_(); \
SJA_HANDLE_ERROR_(Reader, FileTokenType_Integer, __VA_ARGS__);

#define EnsurePositive(Var, ...) \
if(Var < 0){            \
LogWarning("'%d' must be positive!", Var); \
__VA_ARGS__ \
}

#define SJA_EXPECT_INT(Reader, ...) \
(Reader)->ExpectToken(FileTokenType_Integer).Integer; \
SJA_HANDLE_ERROR_(Reader, FileTokenType_Integer, __VA_ARGS__);

#define SJA_EXPECT_IDENTIFIER(Reader, ...) \
(Reader)->ExpectToken(FileTokenType_Identifier).Identifier; \
SJA_HANDLE_ERROR_(Reader, FileTokenType_Identifier, __VA_ARGS__);

#define SJA_EXPECT_STRING(Reader, ...) \
(Reader)->ExpectToken(FileTokenType_String).String; \
SJA_HANDLE_ERROR_(Reader, FileTokenType_String, __VA_ARGS__);

#define SJA_EXPECT_FLOAT(Reader, ...) \
(Reader)->ExpectToken(FileTokenType_Float).Float; \
SJA_HANDLE_ERROR_(Reader, FileTokenType_Float, __VA_ARGS__);

#define SJA_BEGIN_FUNCTION(Reader, Name, ErrorResult) \
const char *Identifier = SJA_EXPECT_IDENTIFIER(Reader, return ErrorResult); \
if(!CStringsEqual(Identifier, Name)){ \
LogWarning("Expected \"%s\" instead read: \"%s\"", Name, Identifier); \
return ErrorResult; \
} \
SJA_EXPECT_TOKEN(Reader, FileTokenType_BeginArguments, SJA_ERROR_BEHAVIOR_FUNCTION(ErrorResult));

#define SJA_END_FUNCTION(Reader, ErrorResult) \
SJA_EXPECT_TOKEN(Reader, FileTokenType_EndArguments, SJA_ERROR_BEHAVIOR_FUNCTION(ErrorResult))

asset_loading_status
asset_loader::ChooseStatus(asset_loading_status Status){
    if(Status > LoadingStatus){
        LoadingStatus = Status;
        return Status;
    }
    return LoadingStatus;
}

void
asset_loader::FailCommand(asset_loading_data *Data, const char *Format, ...){
    va_list VarArgs;
    va_start(VarArgs, Format);
    if(Data) Data->Status = AssetLoadingStatus_Errors;
    VLogWarning(Format, VarArgs);
    SeekNextCommand();
    va_end(VarArgs);
}

void 
asset_loader::BeginCommand(const char *Name){
    CurrentCommand = Name;
    CurrentAttribute = 0;
    CurrentAsset = 0;
}

void
asset_loader::VLogWarning(const char *Format, va_list VarArgs){
    LoadingStatus = AssetLoadingStatus_Warnings;
    string_builder Builder = BeginResizeableStringBuilder(&GlobalTransientMemory, DEFAULT_BUFFER_SIZE);
    BuilderAdd(&Builder, "(Line: %u)", Reader.Line);
    if(CurrentCommand)   BuilderAdd(&Builder, "[%s", CurrentCommand);
    if(CurrentAsset)     BuilderAdd(&Builder, ",\"%s\"", CurrentAsset);
    if(CurrentAttribute) BuilderAdd(&Builder, ",%s", CurrentAttribute);
    if(CurrentCommand)   BuilderAdd(&Builder, "]");
    BuilderAdd(&Builder, ' ');
    VBuilderAdd(&Builder, Format, VarArgs);
    char *Message = EndStringBuilder(&Builder);
    // NOTE(Tyler): Use the asset loader memory, because it will last until the asset system is reset. 
    DEBUG_MESSAGE(DebugMessage_Asset, FinalizeStringBuilder(&InProgress.Memory, &Builder));
}

void 
asset_loader::LogWarning(const char *Format, ...){
    va_list VarArgs;
    va_start(VarArgs, Format);
    VLogWarning(Format, VarArgs);
    va_end(VarArgs);
}

b8
asset_loader::SeekEndOfFunction(){
    file_token Token = Reader.PeekToken();
    if(Token.Type == FileTokenType_BeginArguments) Reader.NextToken();
    u32 ArgumentCount = 1;
    while(ArgumentCount > 0){
        Token = Reader.PeekToken();
        if(Token.Type == FileTokenType_BeginArguments){
            ArgumentCount++;
        }else if(Token.Type == FileTokenType_EndArguments){
            ArgumentCount--;
        }else if((Token.Type == FileTokenType_Invalid) ||
                 (Token.Type == FileTokenType_EndFile)){
            return false;
        }
        Reader.NextToken();
    }
    return true;
}

b8
asset_loader::SeekNextAttribute(){
    while(true){
        file_token Token = Reader.PeekToken();
        switch(Token.Type){
            case FileTokenType_BeginArguments: {
                u32 ArgumentCount = 1;
                while(ArgumentCount){
                    Token = Reader.PeekToken();
                    if(Token.Type == FileTokenType_EndArguments){
                        ArgumentCount--;
                    }else if((Token.Type == FileTokenType_Invalid) ||
                             (Token.Type == FileTokenType_EndFile)){
                        return false;
                    }
                    Reader.NextToken();
                }
            }break;
            case FileTokenType_Identifier: {
                file_token Token = Reader.PeekToken(2);
                if(Token.Type == FileTokenType_BeginArguments){
                    Reader.NextToken();
                    continue;
                }
                return true;
                
            }break;
            case FileTokenType_Invalid:
            case FileTokenType_EndFile: {
                return false;
            }break;
            case FileTokenType_BeginCommand: {
                return true;
            }break;
            default: break;
        }
        Reader.NextToken();
    }
}

b8
asset_loader::SeekNextCommand(){
    while(true){
        file_token Token = Reader.PeekToken();
        switch(Token.Type){
            case FileTokenType_BeginCommand: {
                return true;
            }break;
            case FileTokenType_Invalid:
            case FileTokenType_EndFile: {
                return false;
            }break;
            default: break;
        }
        Reader.NextToken();
    }
}

#define HANDLE_INVALID_ATTRIBUTE(Attribute) \
CurrentAttribute = 0;\
LogWarning("Invalid attribute: \"%s\"", Attribute); \
if(!SeekNextAttribute()) return AssetLoadingStatus_Warnings;

v2
asset_loader::ExpectTypeV2(){
    v2 Result = V2(0);
    
    SJA_BEGIN_FUNCTION(&Reader, "V2", Result);
    
    Result.X = SJA_EXPECT_FLOAT(&Reader, return Result);
    file_token Token = Reader.PeekToken();
    if(Token.Type != FileTokenType_EndArguments){
        Result.Y = SJA_EXPECT_FLOAT(&Reader, return Result);
    }else{
        Result.Y = Result.X;
    }
    
    SJA_END_FUNCTION(&Reader, Result);
    
    return(Result);
}

array<s32>
asset_loader::ExpectTypeArrayS32(){
    array<s32> Result = MakeArray<s32>(&GlobalTransientMemory, SJA_MAX_ARRAY_ITEM_COUNT);
    
    SJA_BEGIN_FUNCTION(&Reader, "Array", Result);
    
    file_token Token = Reader.PeekToken();
    while(Token.Type != FileTokenType_EndArguments){
        s32 Integer = SJA_EXPECT_INT(&Reader, SJA_ERROR_BEHAVIOR_FUNCTION(Result));
        ArrayAdd(&Result, Integer);
        
        Token = Reader.PeekToken();
    }
    
    SJA_END_FUNCTION(&Reader, Result);
    
    return(Result);
}

array<const char *>
asset_loader::ExpectTypeArrayCString(){
    array<const char *> Result = MakeArray<const char *>(&GlobalTransientMemory, SJA_MAX_ARRAY_ITEM_COUNT);
    
    SJA_BEGIN_FUNCTION(&Reader, "Array", Result);
    
    file_token Token = Reader.PeekToken();
    while(Token.Type != FileTokenType_EndArguments){
        const char *String = SJA_EXPECT_STRING(&Reader, SJA_ERROR_BEHAVIOR_FUNCTION(Result));
        ArrayAdd(&Result, String);
        
        Token = Reader.PeekToken();
    }
    
    SJA_END_FUNCTION(&Reader, Result);
    
    return(Result);
}

color
asset_loader::ExpectTypeColor(){
    color Result = {};
    
    SJA_BEGIN_FUNCTION(&Reader, "Color", ERROR_COLOR);
    
    file_token Token = Reader.PeekToken();
    if(Token.Type == FileTokenType_Float){
        for(u32 I=0; I<4; I++){
            Result.E[I] = SJA_EXPECT_FLOAT(&Reader, SJA_ERROR_BEHAVIOR_FUNCTION(ERROR_COLOR));
        }
    }else if(Token.Type == FileTokenType_Integer){
        file_token First = Reader.NextToken();
        Token = Reader.PeekToken();
        if((Token.Type == FileTokenType_Integer) ||
           (Token.Type == FileTokenType_Float)){
            First = MaybeTokenIntegerToFloat(First);
            Assert(First.Type == FileTokenType_Float);
            Result.R = First.Float;
            for(u32 I=1; I<4; I++){
                Result.E[I] = SJA_EXPECT_FLOAT(&Reader, SJA_ERROR_BEHAVIOR_FUNCTION(ERROR_COLOR));
            }
        }else if(Token.Type == FileTokenType_EndArguments){
            Result = MakeColor(First.Integer);
        }else{
            LogWarning("Expected ) or a number, and %s is neither!", TokenToString(Token));
            SJA_ERROR_BEHAVIOR_FUNCTION(ERROR_COLOR);
        }
    }
    
    SJA_END_FUNCTION(&Reader, Result);
    
    return(Result);
}
//~ 

b8 
asset_loader::DoAttribute(const char *String, const char *Attribute){
    b8 Result = CStringsEqual(String, Attribute);
    if(Result) CurrentAttribute = Attribute;
    return(Result);
}

// TODO(Tyler): This should be made into an actual comment type such as /* */ or #if 0 #endif
asset_loading_status
asset_loader::ProcessIgnore(){
    while(true){
        file_token Token = Reader.PeekToken();
        HandleToken(Token);
        Reader.NextToken();
    }
    return AssetLoadingStatus_Okay;
}

asset_loading_status
asset_loader::LoadAssetFile(const char *Path){
    ARENA_FUNCTION_MARKER(&GlobalTransientMemory);
    
    CurrentCommand = 0;
    CurrentAttribute = 0;
    
    os_file *File = OSOpenFile(Path, OpenFile_Read);
    u64 NewFileWriteTime = OSGetLastFileWriteTime(File);
    OSCloseFile(File);
    
    if(LastFileWriteTime < NewFileWriteTime){
        LoadCounter++;
        ArenaClear(&InProgress.Memory);
        
        LoadingStatus = AssetLoadingStatus_Okay;
        
        Reader = MakeFileReader(Path);
        
        while(LoadingStatus != AssetLoadingStatus_Errors){
            file_token Token = Reader.NextToken();
            
            switch(Token.Type){
                case FileTokenType_BeginCommand: {
                    ChooseStatus(ProcessCommand());
                }break;
                case FileTokenType_EndFile: {
                    goto end_loop;
                }break;
                default: {
                    LogWarning("Token: '%s' was not expected!", TokenToString(Token));
                }break;
            }
        }
        end_loop:;
        
        LastFileWriteTime = NewFileWriteTime;
        
        if(LoadingStatus == AssetLoadingStatus_Errors) return LoadingStatus;
        
        Swap(*MainAssets, InProgress); 
    }
    
    return LoadingStatus;
}

#define SJA_COMMAND(Command)                 \
if(CStringsEqual(String, #Command)) { \
BeginCommand(#Command);            \
return Process##Command(); \
}       

asset_loading_status
asset_loader::ProcessCommand(){
    const char *String = SJA_EXPECT_IDENTIFIER(&Reader, SeekNextCommand(); return AssetLoadingStatus_Warnings);
    
    SJA_COMMAND(Ignore);
    SJA_COMMAND(SpecialCommands);
    SJA_COMMAND(Variables);
    
    char *Message = ArenaPushFormatCString(&InProgress.Memory, "(Line: %u) '%s' isn't a valid command!", Reader.Line, String);
    LogMessage(Message);
    DEBUG_MESSAGE(DebugMessage_Asset, Message);
    ProcessIgnore();
    return AssetLoadingStatus_Warnings;
}
#undef SJA_COMMAND

//~ Variables

asset_loading_status 
asset_loader::ProcessSpecialCommands(){
    asset_loading_status Result = AssetLoadingStatus_Okay;
    
    while(true){
        file_token Token = Reader.PeekToken();
        HandleToken(Token);
        const char *Attribute = SJA_EXPECT_IDENTIFIER(&Reader, SJA_ERROR_BEHAVIOR_ATTRIBUTE);
        if(DoAttribute(Attribute, "worlds")){
            WorldsToLoad = ExpectTypeArrayCString();
        }else{ HANDLE_INVALID_ATTRIBUTE(Attribute); }
    }
    
    return ChooseStatus(Result);
}

asset_loading_status
asset_loader::ProcessVariables(){
    asset_loading_status Result = AssetLoadingStatus_Okay;
    
    while(true){
        file_token Token = Reader.PeekToken();
        HandleToken(Token);
        const char *Attribute = SJA_EXPECT_IDENTIFIER(&Reader, SJA_ERROR_BEHAVIOR_ATTRIBUTE);
        if(DoAttribute(Attribute, "var")){
            const char *Name = SJA_EXPECT_IDENTIFIER(&Reader, SJA_ERROR_BEHAVIOR_ATTRIBUTE);
            string_builder Builder = BeginResizeableStringBuilder(&GlobalTransientMemory, DEFAULT_BUFFER_SIZE);
            ExpectDescriptionStrings(&Builder);
            const char *Data = FinalizeStringBuilder(&InProgress.Memory, &Builder);
            asset_variable *Variable = AssetsGet_(&InProgress, Variable, Name);
            Variable->S = Data;
        }else if(DoAttribute(Attribute, "asset")){
            const char *Type = SJA_EXPECT_STRING(&Reader, SJA_ERROR_BEHAVIOR_ATTRIBUTE);
            const char *Name = SJA_EXPECT_STRING(&Reader, SJA_ERROR_BEHAVIOR_ATTRIBUTE);
            const char *Data = SJA_EXPECT_STRING(&Reader, SJA_ERROR_BEHAVIOR_ATTRIBUTE);
            asset_variable *Variable = AssetsGet_(&InProgress, Variable, Name);
            Variable->S = Strings.GetPermanentString(Data);
            Variable->Asset = MakeAssetID(Type, Data);
        }else if(DoAttribute(Attribute, "int")){
            const char *Name = SJA_EXPECT_IDENTIFIER(&Reader, SJA_ERROR_BEHAVIOR_ATTRIBUTE);
            s32 Data = SJA_EXPECT_INT(&Reader, SJA_ERROR_BEHAVIOR_ATTRIBUTE);
            asset_variable *Variable = AssetsGet_(&InProgress, Variable, Name);
            Variable->S32 = Data;
        }else{ HANDLE_INVALID_ATTRIBUTE(Attribute); }
    }
    
    return ChooseStatus(Result);
}
//~ Text adventure descriptions
asset_loading_status
asset_loader::ExpectDescriptionStrings(string_builder *Builder){
    asset_loading_status Result = AssetLoadingStatus_Okay;
    
    while(true){
        file_token Token = Reader.PeekToken();
        if(Token.Type != FileTokenType_String) break;
        const char *S = SJA_EXPECT_STRING(&Reader, SJA_ERROR_BEHAVIOR_ATTRIBUTE);
        
        u32 Length = CStringLength(S);
        for(u32 I=0; S[I]; I++){
            char C = S[I];
            
            if(C == '\\'){
                char Next = S[I+1];
                if(IsANumber(Next)){
                    Next -= '0';
                    BuilderAdd(Builder, '\002');
                    BuilderAdd(Builder, Next+1);
                    I++;
                    continue;
                }
            }
            BuilderAdd(Builder, C);
            
        }
    }
    
    return ChooseStatus(Result);
}

#endif // SNAIL_JUMPY_USE_PROCESSED_ASSETS