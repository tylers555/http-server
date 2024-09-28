
//~ 
void
asset_system::Initialize(memory_arena *Arena, void *Data, u32 DataSize){
    Memory = MakeArena(Arena, Megabytes(128));
    AssetTableInit(Variable,    Arena, MAX_VARIABLES,       Data, DataSize);
    
    //~ Dummy assets
    u8 InvalidColor[] = {0xff, 0x00, 0xff, 0xff};
    stbi_set_flip_vertically_on_load(true);
}
