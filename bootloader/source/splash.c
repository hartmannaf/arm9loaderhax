#include "splash.h"
#include "draw.h"
#include "log.h"
#include "fatfs/sdmmc/sdmmc.h"
#include "fatfs/ff.h"

#define TMPADDRESS 0x24100000

bool drawBootSplash(loaderConfiguration* loaderConfig)
{
    if(loaderConfig->bootsplash>0)
    {
        switch(loaderConfig->bootsplash)
        {/* 0 - disabled, 1 - splash screen, 2 - entry info, 3 - both */ 
            case 1:
            case 3:
                if(splash_image(loaderConfig->bootsplash_image)!=0)
                    splash_ascii();
                break;
            case 2:
                splash_ascii();
                break;
        }
        return true;
    }
    else
        return false;
}

bool drawSplash(configuration* app)
{
    if(app->splash>0)
    {
        switch(app->splash)
        {/* 0 - disabled, 1 - splash screen, 2 - entry info, 3 - both */ 
            case 1:
                if(splash_image(app->splash_image)!=0)
                    splash_ascii();
                break;
            case 2:
                splash_ascii_extendet(app->path);
                break;
            case 3:
                splash_image_extendet(app->splash_image,app->path);
                break;
        }
        debug("Showing Splash for %llu ms",app->splashDelay);
        for(volatile u64 i=0;i<0xEFF*app->splashDelay;i++);
        return true;
    }
    else
        return false;
}

int splash_ascii()
{
    // print BootCtr logo
    // http://patorjk.com/software/taag/#p=display&f=Bigfig&t=BootCtr
    ClearScreen(TOP_SCREEN0,0);
    DrawStringF(5,5,ASCII_ART_TEMPLATE, VERSION_STRING);
    return 0;
}

int splash_image(char *splash_path)
{
    // load image in memory, doing proper error checking
    FIL splash_file;
    unsigned int br;
    if(strlen(splash_path)==0)
    {
        debug("Splash image not set, use default screen");
        return -1;
    }
    if(f_open(&splash_file, splash_path, FA_READ | FA_OPEN_EXISTING) != FR_OK)
    {
        debug("Couldn't open splash image %s.", splash_path);
        return -1;
    }

    //load splash to templocation in memory to prevent visible drawing
    f_read(&splash_file, (void*)(TMPADDRESS), 0x00600000, &br);
    // copy splash image to framebuffer
    memcpy((void*)(*(u32*)0x23FFFE00),(void*)TMPADDRESS,br);

    return 0;    
}

int splash_ascii_extendet(const char* payloadName)
{
    ClearScreen(TOP_SCREEN0,0);
    DrawStringF(5,5,ASCII_ART_TEMPLATE_EXTENDET, VERSION_STRING, payloadName);
    return 0;
} 

int splash_image_extendet(char *splash_path,const char* payloadName)
{
    if(splash_image(splash_path)!=0)
        return splash_ascii_extendet(payloadName);
    return 0;
}
