#include "UselessPch.h"

#include "SoundFactory.h"
#include "Useless/File/IFile.h"
#include "Useless/File/OFile.h"
#include "Useless/File/MemIFile.h"
#include "Useless/Util/Storage/MemBlock.h"
#include "Useless/Error/Error.h"

namespace Useless {

INIT_SINGLETON( SoundFactory, USELESS_API );

const int MaxHeaderBytes = 256;

void __SoundFactory::Register( int st_id, bool (*recognize)(IFile&,Snd::Format*) )
{
    _sound_creators[st_id].recognize = recognize;
}

void __SoundFactory::Register( int st_id, SoundLoader* (*create_loader)() )
{
    _sound_creators[st_id].create_loader = create_loader;
}

void __SoundFactory::Register( int st_id, SoundSaver*  (*create_saver)() )
{
    _sound_creators[st_id].create_saver = create_saver;
}

SPointer< SoundLoader > __SoundFactory::Create( IFile &ifile, Snd::Format *format )
{
    MemBlock mem( MaxHeaderBytes ); MemIFile header(mem);
    ifile.ReadUpto( mem.GetPtr(), MaxHeaderBytes ); ifile.Begin();

    SoundCreatorMap::iterator i = _sound_creators.begin();
    for ( ;i!=_sound_creators.end(); ++i )
    {
        std::pair< int, SoundCreator > p = *i;

        header.Begin();
        if ( (*i).second.recognize( header, format ) )
        {
            return (*i).second.create_loader();
        }
    }
    throw Error("No sound-loader found for file %s", ifile.GetName().c_str() );
}

SPointer< SoundSaver >  __SoundFactory::Create( int st_id )
{
    SoundCreatorMap::iterator i = _sound_creators.find(st_id);
    if ( i==_sound_creators.end() )
    {
        throw Error("No such sound-saver found for st_id=%i", st_id );
    }
    return (*i).second.create_saver();
}


};//namespace Useless
