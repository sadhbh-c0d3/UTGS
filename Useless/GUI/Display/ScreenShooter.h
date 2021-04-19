#ifndef __INCLUDED__USELESS_SCREEN_SHOOTER_H__
#define __INCLUDED__USELESS_SCREEN_SHOOTER_H__

#include "Useless/Graphic/Planes/ImageToFile.h"

namespace Useless {

//REQUIRES
    class Screen;

class USELESS_API ScreenShooter
{
public:    
    ScreenShooter( const Screen      &screen, 
                   const std::string &prefix = "screen", 
                   const std::string &suffix = ".tga", 
                                 int  type_tag = ImageTypes::TGA );

    void Shot();

private:
    const Screen &_screen;
    std::string   _prefix;
    std::string   _suffix;
    int           _count;
    int           _type_tag;
};


};//namespace Useless
#endif//__INCLUDED__USELESS_SCREEN_SHOOTER_H__
