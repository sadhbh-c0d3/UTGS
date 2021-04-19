#include "UselessPch.h"

#include "Useless/GUI/Display/ScreenShooter.h"
#include "Useless/Graphic/Device/Screen.h"
#include <sstream>

namespace Useless {

ScreenShooter::ScreenShooter( const Screen &screen, const std::string &prefix, 
                             const std::string &suffix, int type_tag )
        :_screen(screen), _prefix(prefix), _suffix(suffix), _type_tag(type_tag), _count(0)
{}

void ScreenShooter::Shot()
{
    std::string file_name;

	std::stringstream ss;
	ss << _count++;
    file_name = _prefix + ss.str() + _suffix;

    ImageToFile::Store( *const_cast<Screen&>(_screen).GetSurface(), file_name, _type_tag );
}



};//namespace Useless
