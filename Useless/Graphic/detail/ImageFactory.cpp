/*
**  $Id: ImageFactory.cpp,v 1.12 2003/12/12 08:15:23 mroziu Exp $
**
**  NAME
**      ImageFactory
**
**  PURPOSE
**      Load/Save images of registered types
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
*/

#include "UselessPch.h"

#include "Useless/Graphic/FileIO/ImageFactory.h"
#include "Useless/Util/Storage/MemBlock.h"
#include "Useless/File/MemIFile.h"
#include "Useless/Error/Error.h"

namespace Useless {

INIT_PREINITIALIZER(  ImageFactory, ImageFactory, USELESS_API );

ImageFactory::Entries* ImageFactory::_entries = 0;


void ImageFactory::Register( int id, ImageLoader *(*loader)(), bool (*recognizer)( IFile &, int*,int*,int*) )
{
    (*_entries)[id]._loader = loader;
    (*_entries)[id]._recognizer = recognizer;
}

void ImageFactory::RegisterSaver( int id, ImageSaver *(*saver)() )
{
    (*_entries)[id]._saver = saver;
}

void ImageFactory::Unregister( int id )
{
    if ( (*_entries).find(id)==(*_entries).end() ) { throw Error("ImageFactory: Identifier %i does not exist", id); }
    (*_entries).erase(id);
}

SPointer< ImageLoader > ImageFactory::Create( IFile &ifile )
{
    return Create(ifile, NULL, NULL, NULL);
}

SPointer< ImageLoader > ImageFactory::Create( IFile &ifile, int *w, int *h, int *bpp )
{
    MemBlock mem(256); MemIFile header(mem);
    ifile.ReadUpto( mem.GetPtr(), 256); ifile.Seek( 0, IFile::SEEKSET );
    
    bool recognized=false;
    Entries::iterator i;
    for( i=(*_entries).begin(); i!=(*_entries).end() && !recognized; ++i )
    {
        int id = (*i).first;
        ListEntry &ptr = (*i).second;
        if ( !!ptr._recognizer )
        {
            recognized = (*ptr._recognizer)( header, w, h, bpp );
            header.Seek( 0, IFile::SEEKSET );
        }
    }

    if (!recognized) { throw Error("ImageFactory: Image %s could not be loaded", ifile.GetName().c_str() ); }
    return (*(*--i).second._loader)();
}

SPointer< ImageSaver > ImageFactory::Create( int id )
{
    if ( (*_entries).find(id)==(*_entries).end() ) { throw Error("ImageFactory: Identifier %i does not exist", id); }
    return (*(*_entries)[id]._saver)();
}


}; // namespace Useless
