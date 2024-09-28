
//~ Smart strings
// TODO(Tyler): These should probably all use the same chunk of memory at some point to make things simpler
// and possible more efficient

/* TODO(Tyler): Functions to implement:
* - Constructors          DONE
 * - Slice                 DONE
 * - To C String           DONE
* - To lowercase          DONE
* - To uppercase          DONE
 * - Concatenate           DONE
 * - First index of char   DONE
* - Duplicate             DONE
* - Split                 DONE
 * - Trim                  DONE
* - Split into lines      DONE
 * - Find substring        DONE
* - From Format           DONE
* - Comparison            DONE
* - Is int                DONE
* - To int                DONE
* - Is float              DONE
* - To float              DONE
* - Alphabetical order    ????
* - Decent hash function  DONE
*/

struct string {
    const char *S;
    u32 Length;
    
    inline const char operator[](s64 Index){
        Assert(Index < Length);
        return(S[Index]);
    }
    
    string(){
    }
    
    string(const char *CS){
        S = CS;
        Length = CStringLength(S);
    }
};

#define FSTR(Str) (Str)->Length, (Str)->S


internal inline string 
String(const char *S, u32 Length){
    string Result = {};
    Result.S = S;
    Result.Length = Length;
    return Result;
}

internal inline string 
String(const char *S){
    return String(S, CStringLength(S));
}

internal inline char *
CString(memory_arena *Memory, string S){
    char *Result = ArenaPushArray(Memory, char, S.Length+1);
    CopyMemory(Result, S.S, S.Length);
    Result[S.Length] = 0;
    return Result;
}

internal inline const char 
StringGet(string *S, s64 Index){
    Assert(Index < S->Length);
    return(S->S[Index]);
}

internal b8
StringEquals(string A, string B){
    if(A.Length != B.Length) return false;
    for(u32 I=0; I<A.Length; I++){
        if(A[I] != B[I]){
            return false;
        }
    }
    
    return true;
}

internal b8
operator==(string A, string B){
    return StringEquals(A, B);
}

internal b8
operator!=(string A, string B){
    return !StringEquals(A, B);
}

internal string
StringSlice(string A, u32 Left, u32 Right){
    Assert(Right >= Left);
    Assert(Right <= A.Length);
    string Result = String(A.S+Left, Right-Left);
    return Result;
}

internal string
StringSliceSize(string A, u32 Left, u32 Size){
    Assert(Left+Size <= A.Length);
    string Result = String(A.S+Left, Size);
    return Result;
}

internal string
StringSliceLeft(string A, u32 Left){
    return StringSlice(A, Left, A.Length);
}

internal void
StringSlice(string *A, u32 Left, u32 Right){
    Assert(Right >= Left);
    Assert(Right <= A->Length);
    A->S += Left; 
    A->Length = Right-Left;
}

internal void
StringSliceLeft(string *A, u32 Left){
    StringSlice(A, Left, A->Length);
}

internal void
StringSliceSize(string *A, u32 Left, u32 Size){
    Assert(Left+Size <= A->Length);
    A->S += Left; 
    A->Length = Size;
}

internal string
StringCopy(memory_arena *Memory, string A){
    string Result = {};
    Result.S = ArenaPushArray(Memory, const char, A.Length);
    Result.Length = A.Length;
    CopyMemory((void *)Result.S, A.S, A.Length);
    return Result;
}

internal inline string
StringMakeLower(memory_arena *Memory, string A){
    string Result = StringCopy(Memory, A);
    for(u32 I=0; I < Result.Length; I++){
        if((('A' <= Result.S[I]) && (Result.S[I] <= 'Z'))) ((char *)Result.S)[I] -= 'A'-'a';
    }
    return Result;
}

internal inline string
StringMakeUpper(memory_arena *Memory, string A){
    string Result = StringCopy(Memory, A);
    for(u32 I=0; I < Result.Length; I++){
        if((('a' <= Result.S[I]) && (Result.S[I] <= 'z'))) ((char *)Result.S)[I] += 'A'-'a';
    }
    return Result;
}

internal inline string
StringConcat(memory_arena *Memory, string A, string B){
    string Result = {};
    Result.Length = A.Length+B.Length;
    Result.S = ArenaPushArray(Memory, char, Result.Length);
    CopyMemory((char *)Result.S,          A.S, A.Length);
    CopyMemory((char *)Result.S+A.Length, B.S, B.Length);
    return Result;
}

internal s32
StringIndexOfFirstChar(string S, char C, u32 Start=0){
    Assert(Start <= S.Length);
    for(u32 I=Start; I<S.Length; I++){
        if(S.S[I] == C) return I;
    }
    return -1;
}

internal s32
StringIndexOfLastChar(string S, char C){
    for(s32 I=S.Length-1; I>=0; I--){
        if(S.S[I] == C) return I;
    }
    return -1;
}

internal array<string>
StringSplit(memory_arena *Memory, string S, char C){
    dynamic_array<string> Result = {};
    InitializeArray(&Result, 8, Memory);
    while(S.Length > 0){
        s32 NextSplit = StringIndexOfFirstChar(S, C);
        if(NextSplit < 0){
            ArrayAdd(&Result, StringSlice(S, 0, S.Length));
            return Result;
        }else{
            ArrayAdd(&Result, StringSlice(S, 0, NextSplit));
            StringSliceLeft(&S, NextSplit+1);
        }
        
    }
    
    return Result;
}

internal string
StringTrim(string S){
    if(S.Length == 0) return S;
    string Result = S;
    while(IsWhiteSpace(*Result.S) && 
          (Result.Length > 0)){
        Result.S++;
        Result.Length--;
    }
    while(IsWhiteSpace(Result.S[Result.Length-1]) && 
          (Result.Length > 0)){
        Result.Length--;
    }
    return Result;
}

internal void
StringTrim(string *S){
    *S = StringTrim(*S);
}

internal string
StringTrimNewLine(string S){
    string Result = S;
    while((Result.Length > 0) &&
          IsNewLine(*Result.S)){
        Result.S++;
        Result.Length--;
    }
    while((Result.Length > 0) &&
          IsNewLine(Result.S[Result.Length-1])){
        Result.Length--;
    }
    return Result;
}

internal void
StringTrimNewLine(string *S){
    *S = StringTrimNewLine(*S);
}

internal string
StringSplitFirstLine(string *S){
    s32 NextSplit = StringIndexOfFirstChar(*S, '\n');
    string Result = {};
    if(NextSplit < 0){
        Result = StringTrimNewLine(StringSlice(*S, 0, S->Length));
        S->Length = 0;
    }else{
        Result = StringTrimNewLine(StringSlice(*S, 0, NextSplit));
        StringSliceLeft(S, NextSplit+1);
    }
    return Result;
}

internal string
StringSplitFirst(string *S, const char C){
    s32 NextSplit = StringIndexOfFirstChar(*S, C);
    string Result = {};
    if(NextSplit < 0){
        Result = StringTrimNewLine(StringSlice(*S, 0, S->Length));
        S->Length = 0;
    }else{
        Result = StringTrimNewLine(StringSlice(*S, 0, NextSplit));
        StringSliceLeft(S, NextSplit+1);
    }
    return Result;
}

internal string
StringSplitLast(string *S, const char C){
    s32 NextSplit = StringIndexOfLastChar(*S, C);
    string Result = {};
    if(NextSplit < 0){
        Result = StringTrimNewLine(StringSliceLeft(*S, 0));
        S->Length = 0;
    }else{
        Result = StringTrimNewLine(StringSliceLeft(*S, NextSplit+1));
        StringSlice(S, 0, NextSplit);
    }
    return Result;
}

internal array<string>
StringSplitIntoLines(memory_arena *Memory, string S){
    dynamic_array<string> Result = {};
    InitializeArray(&Result, 8, Memory);
    while(S.Length > 0){
        s32 NextSplit = StringIndexOfFirstChar(S, '\n');
        
        if(NextSplit < 0){
            ArrayAdd(&Result, StringTrimNewLine(StringSlice(S, 0, S.Length)));
            return Result;
        }else{
            ArrayAdd(&Result, StringTrimNewLine(StringSlice(S, 0, NextSplit)));
            StringSliceLeft(&S, NextSplit+1);
        }
        
    }
    
    return Result;
}

internal s32
StringFindSubstring(string S, string Sub){
    s32 Result = -1;
    for(s32 I=0; 
        (Result < 0) && (I<(s32)S.Length-(s32)Sub.Length); 
        I++){
        Result = I;
        for(u32 J=0; J<Sub.Length; J++){
            if(Sub.S[J] != S.S[I+J]){
                Result = -1;
                break;
            }
        }
    }
    return Result;
}

internal string
VFormatString(memory_arena *Arena, const char *Format, va_list VarArgs1){
    va_list VarArgs2;
    va_copy(VarArgs2, VarArgs1);
    s32 Length = TYLER_VSNPRINTF(0, 0, Format, VarArgs1);
    char *Buffer = ArenaPushArray(Arena, char, Length);
    TYLER_VSNPRINTF(Buffer, Length, Format, VarArgs2);
    return String(Buffer, Length);
}

internal string
FormatString(memory_arena *Arena, const char *Format, ...){
    va_list VarArgs;
    va_start(VarArgs, Format);
    string Result = VFormatString(Arena, Format, VarArgs);
    va_end(VarArgs);
    return Result;
}

internal void
StringAdvance(string *S){
    Assert(S->Length > 0);
    S->S++;
    S->Length--;
}

internal b8
StringIsInt(string S){
    if((S[0] == '-') || 
       (S[0] == '+')){
        StringAdvance(&S);
    }
    for(const char *C=S.S; C-S.S < S.Length; C++){
        if(!IsANumber(*C)) return false;
    }
    return true;
}

internal s32
StringToInt(string S){
    Assert(StringIsInt(S));
    s32 Result = 0;
    
    s32 Sign = 1;
    if(S[0] == '-'){
        Sign = -1;
        StringAdvance(&S);
    }else if(S[0] == '+'){
        Sign = 1;
        StringAdvance(&S);
    }
    
    while(S.Length > 0){
        Assert(IsANumber(S.S[0]));
        s32 Digit = S.S[0]-'0';
        Result *= 10;
        Result += Digit;
        StringAdvance(&S);
    }
    
    Result *= Sign;
    return Result;
}

internal b8
StringIsFloat(string S){
    if((S[0] == '-') || 
       (S[0] == '+')){
        StringAdvance(&S);
    }
    for(const char *C=S.S; C-S.S < S.Length; C++){
        if(!IsANumber(*C) && (*C != '.')) return false;
    }
    return true;
}

internal f32
StringToFloat(string S){
    Assert(StringIsFloat(S));
    s32 Upper = 0;
    s32 Lower = 0;
    s32 LowerPlace = 1;
    
    s32 Sign = 1;
    if(S[0] == '-'){
        Sign = -1;
        StringAdvance(&S);
    }else if(S[0] == '+'){
        Sign = 1;
        StringAdvance(&S);
    }
    
    while(S.Length > 0){
        if(S[0] == '.'){
            StringAdvance(&S);
            break;
        }
        Assert(IsANumber(S[0]));
        s32 Digit = S[0]-'0';
        Upper *= 10;
        Upper += Digit;
        StringAdvance(&S);
    }
    
    while(S.Length > 0){
        Assert(IsANumber(S[0]));
        s32 Digit = S[0]-'0';
        Lower *= 10;
        LowerPlace *= 10;
        Lower += Digit;
        StringAdvance(&S);
    }
    
    f32 Result = Upper;
    Result += (f32)Lower/(f32)LowerPlace;
    Result *= Sign;
    return Result;
}

// NOTE(Tyler): Taken from: http://www.cse.yorku.ca/~oz/hash.html
internal u64
StringHash(string S){
    u64 Hash = 5381;
    char C;
    while ((C = *S.S++)){
        Hash = ((Hash << 5) + Hash) + C; /* hash * 33 + c */
    }
    
    return Hash;
}



//~ Strings
struct string_id {
    u64 ID;
};

global_constant string_id String0 = {0};

internal inline constexpr b8
operator==(string_id A, string_id B){
    b8 Result = (A.ID == B.ID);
    return(Result);
}

internal inline constexpr b8
operator!=(string_id A, string_id B){
    b8 Result = (A.ID != B.ID);
    return(Result);
}

internal inline string_id
MakeStringID(u64 ID){
    string_id Result = {ID};
    return(Result);
}

internal constexpr u64
HashKey(string_id Value) {
    u64 Result = Value.ID;
    return(Result);
}

internal constexpr b32
CompareKeys(string_id A, string_id B){
    b32 Result = (A == B);
    return(Result);
}

union string_buffer_node {
    string_buffer_node *Next;
    char Buffer[DEFAULT_BUFFER_SIZE];
};

// String manager
struct string_manager {
    memory_arena StringMemory;
    hash_table<const char *, const char *> Table;
    
    memory_arena BufferMemory;
    string_buffer_node *NextBuffer;
    
    void Initialize(memory_arena *Arena);
    
    string_id GetString(const char *String);
    const char *GetString(string_id String);
    const char *GetPermanentString(const char *String);
    char *MakeBuffer();
    void  RemoveBuffer(char *Buffer);
    template<typename T> T *HashTableGetPtr(hash_table<string_id, T> *Table, const char *Key);
    template<typename T> T *HashTableFindPtr(hash_table<string_id, T> *Table, const char *Key);
};

void
string_manager::Initialize(memory_arena *Arena){
    StringMemory = MakeArena(Arena, Kilobytes(32));
    Table = MakeHashTable<const char *, const char *>(Arena, 512);
    
    u32 BufferCount = 128;
    //u32 BufferCount = 2;
    BufferMemory = MakeArena(Arena, sizeof(string_buffer_node)*BufferCount);
    
    NextBuffer = 0;
    for(u32 I=0; I<BufferCount; I++){
        string_buffer_node *Node = ArenaPushType(&BufferMemory, string_buffer_node);
        Node->Next = NextBuffer;
        NextBuffer = Node;
    }
}

string_id
string_manager::GetString(const char *String){
    string_id Result = {};
    if(!String) return Result;
    
    const char *ResultString = HashTableFind(&Table, String);
    if(!ResultString){
        ResultString = ArenaPushCString(&StringMemory, String);
        HashTableAdd(&Table, ResultString, ResultString);
    }
    
    Result.ID = (u64)ResultString;
    return(Result);
}

const char *
string_manager::GetString(string_id String){
    const char *Result = (const char *)String.ID;
    
    return(Result);
}

const char *
string_manager::GetPermanentString(const char *String){
    const char *Result = GetString(GetString(String));
    return Result;
}

char *
string_manager::MakeBuffer(){
    Assert(NextBuffer);
    char *Result = NextBuffer->Buffer;
    NextBuffer = NextBuffer->Next;
    ZeroMemory(Result, DEFAULT_BUFFER_SIZE);
    return(Result);
}

void
string_manager::RemoveBuffer(char *Buffer){
    u8 *MemoryMin = BufferMemory.Memory;
    u8 *MemoryMax = BufferMemory.Memory+BufferMemory.Used;
    Assert((MemoryMin <= (u8 *)Buffer) && ((u8 *)Buffer < MemoryMax));
    string_buffer_node *Node = (string_buffer_node *)Buffer;
    Node->Next = NextBuffer;
    NextBuffer = Node;
}

template<typename T> T *
string_manager::HashTableGetPtr(hash_table<string_id, T> *Table, const char *Key){
    string_id String = GetString(Key);
    T *Result = ::HashTableFindPtr(Table, String);
    if(!Result){
        Result = HashTableAlloc(Table, String);
    }
    return(Result);
}

template<typename T> T *
string_manager::HashTableFindPtr(hash_table<string_id, T> *Table, const char *Key){
    string_id String = GetString(Key);
    T *Result = ::HashTableFindPtr(Table, String);
    return(Result);
}


//~ Globals
global string_manager Strings;

//~ String builder
struct string_builder {
    memory_arena *Arena;
    char *Buffer;
    u32 BufferSize;
    u32 BufferCapacity;
};

internal inline string_builder 
BeginStringBuilder(memory_arena *Arena, u32 Capacity){
    string_builder Result = {};
    Result.Buffer = ArenaPushArray(Arena, char, Capacity);
    Result.BufferCapacity = Capacity;
    return Result;
}

internal inline string_builder 
BeginResizeableStringBuilder(memory_arena *Arena, u32 Capacity){
    string_builder Result = BeginStringBuilder(Arena, Capacity);
    Result.Arena = Arena;
    return Result;
}

internal inline char *
FinalizeStringBuilder(memory_arena *Arena, string_builder *Builder){
    u32 Size = Builder->BufferSize+1;
    char *Result = ArenaPushArray(Arena, char, Size);
    CopyMemory(Result, Builder->Buffer, Size);
    Result[Size] = 0;
    return Result;
}

internal inline char *
EndStringBuilder(string_builder *Builder){
    char *Result = Builder->Buffer;
    return Result;
}

internal inline void 
BuilderMaybeResize(string_builder *Builder, u32 NeededSize){
    NeededSize += 1; // NOTE(Tyler): Leave an extra byte for a possible null terminator
    if(!Builder->Arena) return;
    if(NeededSize < Builder->BufferCapacity-Builder->BufferSize) return;
    
    u32 Capacity = Builder->BufferCapacity;
    while(Capacity-Builder->BufferSize <= NeededSize){
        Capacity *= 2;
    }
    
    ArenaResizeMemory(Builder->Arena, Builder->Buffer, Builder->BufferCapacity, Capacity);
    Builder->BufferCapacity = Capacity;
}

internal inline void
BuilderToFile(string_builder *Builder, os_file *File, u64 Offset=0){
    OSWriteToFile(File, Offset, Builder->Buffer, Builder->BufferSize);
}

internal inline void
BuilderAdd(string_builder *Builder, char C){
    BuilderMaybeResize(Builder, 1);
    
    Builder->Buffer[Builder->BufferSize++] = C;
    Builder->Buffer[Builder->BufferSize] = 0;
}

internal inline void
BuilderAddData(string_builder *Builder, const void *Data, u32 DataSize){
    BuilderMaybeResize(Builder, DataSize);
    CopyMemory(&Builder->Buffer[Builder->BufferSize], Data, DataSize);
    Builder->BufferSize += DataSize;
    Builder->Buffer[Builder->BufferSize] = 0;
}

#define BuilderAddVar(Builder, Data) BuilderAddData(Builder, &Data, sizeof(Data))

internal inline void
VBuilderAdd(string_builder *Builder, const char *Format, va_list VarArgs1){
    va_list VarArgs2;
    va_copy(VarArgs2, VarArgs1);
    s32 ExpectedSize = TYLER_VSNPRINTF(0, 0, Format, VarArgs1);
    BuilderMaybeResize(Builder, ExpectedSize);
    
    s32 RemainingSize = Builder->BufferCapacity-Builder->BufferSize;
    s32 Characters = TYLER_VSNPRINTF(&Builder->Buffer[Builder->BufferSize], RemainingSize, Format, VarArgs2);
    Assert(Characters >= 0);
    Builder->BufferSize += Characters;
}

internal inline void
BuilderAdd(string_builder *Builder, const char *Format, ...){
    va_list VarArgs;
    va_start(VarArgs, Format);
    VBuilderAdd(Builder, Format, VarArgs);
    va_end(VarArgs);
}

internal inline void
BuilderAddString(string_builder *Builder, char *S){
    BuilderAddData(Builder, S, CStringLength(S));
}

//~ Helpers
internal inline b8 
StopSeeking(char C){
    b8 Result = (!IsALetter(C) &&
                 !IsANumber(C));
    return Result;
}

internal inline range_s32
SeekForward(const char *Buffer, u32 BufferLength, u32 Start){
    range_s32 Result = MakeRangeS32(Start, BufferLength);
    b8 HitAlphabetic = false;
    for(u32 I=Start; I<=BufferLength; I++){
        char C = Buffer[I];
        Result.End = I;
        if(StopSeeking(C)){
            if(HitAlphabetic) break;
        }else HitAlphabetic = true;
    }
    
    return Result;
}

internal inline range_s32
SeekBackward(const char *Buffer, s32 End){
    range_s32 Result = MakeRangeS32(0, End);
    if(End == 0) return Result;
    b8 HitAlphabetic = false;
    for(s32 I=Result.End-1; I>=0; I--){
        char C = Buffer[I];
        if(StopSeeking(C)){
            if(HitAlphabetic) break;
        }else HitAlphabetic = true;
        Result.Start = I;
    }
    
    return Result;
}

internal inline u32
CountWordMatchCount(const char *A, const char *B){
    u32 Result = 0;
    while(*A && *B){
        if(*A == ' ') { A++; continue; }
        if(*B == ' ') { B++; continue; }
        if(*A != *B){
            if(CharToLower(*A) != CharToLower(*B)) return Result;
        }
        A++;
        B++;
        Result++;
    }
    
    return Result;
}
