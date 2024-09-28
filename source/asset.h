#ifndef SNAIL_JUMPY_ASSET_H
#define SNAIL_JUMPY_ASSET_H

//~ Assets
enum asset_loading_status {
    AssetLoadingStatus_Okay,
    AssetLoadingStatus_Warnings,
    AssetLoadingStatus_Errors,
};

global_constant u32 SJA_MAX_ARRAY_ITEM_COUNT = 128;
global_constant u32 MAX_ASSETS_PER_TYPE      = 128;
global_constant u32 MAX_VARIABLES            = 256;

global_constant u32 MAX_SPRITE_SHEET_ANIMATION_FRAMES = 32;
global_constant u32 MAX_SPRITE_SHEET_ANIMATIONS  = 32;
global_constant u32 MAX_SPRITE_SHEET_PIECES = 5;

global_constant u32 MAX_ENTITY_ASSET_BOUNDARIES = 8;

global_constant u32 MAX_TILEMAP_BOUNDARIES = 8;

#define ASSET_TAGS \
ASSET_TAG("background",       Background)  \
ASSET_TAG("snail",            Snail)       \
ASSET_TAG("dragonfly",        Dragonfly)   \
ASSET_TAG("boxing",           Boxing)      \
ASSET_TAG("trail_bouncy",     TrailBouncy) \
ASSET_TAG("trail_speedy",     TrailSpeedy) \
ASSET_TAG("trail_sticky",     TrailSticky) \
ASSET_TAG("animated",         Animated)    \
ASSET_TAG("art",              Art)         \
ASSET_TAG("arrow",            Arrow)       \
ASSET_TAG("no_editor",        NoEditor)    \
ASSET_TAG("transparent",      Transparent) \
ASSET_TAG("platform",         Platform)    \

;

#define ASSET_TAG(S, N) AssetTag_##N,
enum asset_tag_id {
    AssetTag_None = 0,
    
    ASSET_TAGS
        
        AssetTag_TOTAL
};
#undef ASSET_TAG

union asset_tag {
    struct {
        u8 A;
        u8 B;
        u8 C;
        u8 D;
    };
    u8 E[4];
    u32 All;
};

//~ Asset ID

//~
#if defined(SNAIL_JUMPY_USE_PROCESSED_ASSETS) 
struct asset_id {
    u64 ID;
};

struct asset_loading_data {
};

#define asset_table(Name, ValueType) ValueType Name##Table[Name##ID_TOTAL]
#define AssetTableInit(Name, Arena, MaxCount, Data, DataSize) ProcessedAssetsInitialize##Name(Arena, Name##Table, Data, DataSize)
#define AssetTableGet_(Prefix, Name, Key) (&(Prefix Name##Table[Key.ID]))
#define AssetTableFind_(Prefix, Name, Key) (&(Prefix Name##Table[Key.ID]))
#define AssetTableCount(Assets, Name) (Assets)->Name##ID_TOTAL

#define AssetID(Name, ID) MakeAssetID(Name##ID_##ID)
#define AssetIDName(Name, ID_) Name##NameTable[ID_.ID]

#define AssetsGet_(System, Name, Key) AssetTableGet_((System)->, Name, Key)
#define AssetsFind_(System, Name, Key) AssetTableFind_((System)->, Name, Key)
#define AssetsFind(System, Name, Key) AssetsFind_(System, Name, AssetID(Name, Key))

#define GetVar(Assets, ID_)      AssetsFind(Assets, Variable, ID_)->S
#define GetVarS32(Assets, ID_)   AssetsFind(Assets, Variable, ID_)->S32
#define GetVarAsset(Assets, ID_) AssetsFind(Assets, Variable, ID_)->Asset
#define GetVarName(Assets, ID_)  AssetsFind(Assets, Variable, ID_)->NameData

//~ 
#else 
struct asset_id {
    const char *TableName;
    u64 ID;
};

internal inline asset_id
MakeAssetID(const char *TableName, string_id ID){
    asset_id Result;
    Result.TableName = Strings.GetPermanentString(TableName);
    Result.ID = ID.ID;
    return Result;
}

internal inline asset_id
MakeAssetID(const char *TableName, asset_id ID){
    asset_id Result = ID;
    return Result;
}

internal inline asset_id
MakeAssetID(const char *TableName, const char *S){
    return MakeAssetID(TableName, Strings.GetString(S));
}

template<typename ValueType>
internal ValueType *
AssetTableFindByKey_(hash_table<asset_id, ValueType> *Table, asset_id Name){
    if(Name.ID){
        ValueType *Result = HashTableFindPtr(Table, Name);
        if(Result && !IsLoadedAssetValid(&Result->LoadingData)) return 0;
        return Result;
    }
    return 0;
}

internal constexpr u64
HashKey(asset_id Value) {
    u64 Result = Value.ID;
    return(Result);
}

internal constexpr b32
CompareKeys(asset_id A, asset_id B){
    b32 Result = (A.ID == B.ID);
    return(Result);
}

#define asset_table(Name, ValueType) hash_table<asset_id, ValueType> Name##Table
#define AssetTableInit(Name, Arena, MaxCount, Data, DataSize) HashTableInit(&(Name##Table), Arena, MaxCount)
#define AssetTableGet_(Prefix, Name, Key) HashTableGetPtr(&(Prefix Name##Table), MakeAssetID(#Name, Key))
#define AssetTableFind_(Prefix, Name, Key) AssetTableFindByKey_(&(Prefix Name##Table), MakeAssetID(#Name, Key))
#define AssetTableCount(Prefix, Name) (Prefix Name##Table).Count
#define ASSET_TABLE_FOR_EACH_(It, Prefix, Name) HASH_TABLE_FOR_EACH_BUCKET(It, &(Prefix Name##Table))
#define ASSET_TABLE_FOR_EACH(It, System, Name) ASSET_TABLE_FOR_EACH_(It, (System)->, Name)

#define AssetsGet_(System, Name, Key) AssetTableGet_((System)->, Name, Key)
#define AssetsFind_(System, Name, Key) AssetTableFind_((System)->, Name, Key)
#define AssetsFind(System, Name, Key) AssetsFind_(System, Name, #Key)
#define AssetsCount(System, Name) AssetTableCount((System)->, Name)

#define AssetID(Name, ID_) MakeAssetID(#Name, #ID_)
#define AssetIDName(Name, ID_) Strings.GetString(MakeString((ID_).ID))

#define GetVar(Assets, ID_)      AssetsFind(Assets, Variable, ID_)->S
#define GetVarS32(Assets, ID_)   AssetsFind(Assets, Variable, ID_)->S32
#define GetVarAsset(Assets, ID_) AssetsFind(Assets, Variable, ID_)->Asset
#define GetVarName(Assets, ID_)  AssetsFind(Assets, Variable, ID_)->NameData

struct asset_loading_data {
    asset_loading_status Status;
};

internal inline b8 
IsLoadedAssetValid(asset_loading_data *Data){
    return (Data->Status != AssetLoadingStatus_Errors);
}

#endif

internal inline asset_id
MakeAssetID(u32 ID){
    asset_id Result;
    Result.ID = ID;
    return Result;
}

internal inline b8
operator==(asset_id A, asset_id B){
    return (A.ID == B.ID);
}

internal inline b8
operator!=(asset_id A, asset_id B){
    return (A.ID != B.ID);
}

//~ Variables
struct asset_variable {
    asset_loading_data LoadingData;
    
    const char *S;
    asset_id Asset;
    s32 S32;
};

//~ Special commands
typedef u32 special_commands;
enum special_commands_ {
    SpecialCommand_None               = (0 << 0),
};

//~ Asset system
global_constant color             ERROR_COLOR = MakeColor(1.0f, 0.0f, 1.0f);

global_constant u32 MAX_TILEMAP_SLOTS = 32;

struct asset_system {
    //~ Asset stuff
    memory_arena Memory;
    asset_table(Variable,    asset_variable);
    
    void Initialize(memory_arena *Arena, void *Data=0, u32 DataSize=0);
};


//~ Asset loading
struct audio_mixer;
struct world_manager;
struct player_data;
struct enemy_data;
struct asset_loader {
    asset_system *MainAssets;
    asset_system InProgress;
    
    audio_mixer *Mixer;
    world_manager *Worlds;
    
    player_data *PlayerData;
    enemy_data *EnemyData;
    
    //~ Logging 
    const char *CurrentCommand;
    const char *CurrentAsset;
    const char *CurrentAttribute;
    asset_loading_status LoadingStatus;
    u32 LoadCounter;
    
    asset_loading_status ChooseStatus(asset_loading_status Status);
    void BeginCommand(const char *Name);
    void LogWarning(const char *Format, ...);
    void VLogWarning(const char *Format, va_list VarArgs);
    b8 SeekNextAttribute();
    b8 SeekEndOfFunction();
    b8 SeekNextCommand();
    void FailCommand(asset_loading_data *Data, const char *Format, ...);
    
    //~ SJA reading and parsing
    u64 LastFileWriteTime;
    hash_table<const char *, char>         ASCIITable;
    hash_table<const char *, asset_tag_id> TagTable;
    
    file_reader Reader;
    
    u32 ExpectPositiveInteger_();
    
    v2                  ExpectTypeV2();
    array<s32>          ExpectTypeArrayS32();
    array<const char *> ExpectTypeArrayCString();
    color               ExpectTypeColor();
    asset_tag           MaybeExpectTag();
    rect                ExpectTypeRect();
    
    void Initialize(memory_arena *Arena, asset_system *Assets, 
                    audio_mixer *Mixer_, world_manager *Worlds_, 
                    player_data *PlayerData_, enemy_data *EnemyData_);
    
    b8 DoAttribute(const char *String, const char *Attribute);
    
    asset_loading_status ProcessCommand();
    asset_loading_status ProcessIgnore();
    
    special_commands SpecialCommands;
    array<const char *> WorldsToLoad;
    const char *StartupWorld;
    
    asset_loading_status ProcessSpecialCommands();
    asset_loading_status ProcessVariables();
    
    
    asset_loading_status ExpectDescriptionStrings(string_builder *Builder);
    
    asset_loading_status LoadAssetFile(const char *Path);
};


//~ Asset processing
struct asset_processor_texture {
    u8 *Pixels;
    u32 Width;
    u32 Height;
    u32 Channels;
};

struct asset_processor {
    dynamic_array<asset_processor_texture> Textures;
    string_builder SJAPBuilder;
    string_builder IDBuilder;
    string_builder NameBuilder;
    string_builder AssetBuilder;
    const char *CurrentAttribute;
    
    b8 DoEmitCheck;
};

#pragma pack(push, 1)
struct sjap_header {
    char SJAP[4];
};
#pragma pack(pop)

#endif //SNAIL_JUMPY_ASSET_H
