#if !defined(SNAIL_JUMPY_H)
#define SNAIL_JUMPY_H

// TODO(Tyler): Do this in build.bat
#define SNAIL_JUMPY_DEBUG_BUILD
#define SNAIL_JUMPY_DO_AUTO_SAVE_ON_EXIT true

#define BUCKET_ARRAY_IGNORE_FLAG (1 << 3)

#include "tyler_basics.h"

//~ Constants TODO(Tyler): Several of these should be hotloaded in a variables file
global_constant char *ASSET_FILE_PATH = "assets.sja";

//~ Includes
#include "os.h"
#include "debug.h"
#include "file_processing.h"
#include "random.h"
#include "strings.cpp"
#include "asset.h"


//~ 
struct main_state {
    asset_system Assets;
    os_input Input;
    
    asset_loader AssetLoader;
};

#endif



