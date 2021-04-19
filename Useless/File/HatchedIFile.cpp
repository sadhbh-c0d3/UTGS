#include "UselessPch.h"

#include "Useless/File/Hatchery.h" // <- Look here for declaration
#include "Useless/Error/IOError.h"

namespace Useless {

HatchedIFile::HatchedIFile( Hatchery &hatchery, const std::string &name )
    :_hatchery(hatchery)
{
    _file = hatchery.GainAccess();
    Hatchery::Iterator i = hatchery._nodes.find( _name = name );
    if ( i==hatchery._nodes.end() )
    {
        throw IOError("Failed to hatch file '%s'.",_name.c_str());
    }
    _baseoffset = (*i).second.offset;
    _subsize = (*i).second.size;
    _curoffset = _baseoffset;
    SubIFile::Seek( 0, SEEKSET );
} 

HatchedIFile::~HatchedIFile() throw()
{
    _file = NULL;
    _hatchery.RevokeAccess();
}

}//namespace Useless
