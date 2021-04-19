#ifndef __INCLUDED_USELESS_IMAGE_FACTORY__
#define __INCLUDED_USELESS_IMAGE_FACTORY__

/*
**  $Id: ImageFactory.h,v 1.7 2002/09/13 16:22:16 koolas Exp $
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

#pragma warning(disable:4786)       // disable 'indentifier was truncated...'

#include <map>
#include "Useless/File/IFile.h"
#include "Useless/File/OFile.h"
#include "Useless/Graphic/FileIO/ImageLoader.h"
#include "Useless/Graphic/FileIO/ImageSaver.h"
#include "Useless/Util/SPointer.h"
#include "Useless/Util/PreInitializer.h"

namespace Useless {

/*! \defgroup Graphics Graphics*/

/*! \defgroup ImgIO Image Data Level
 *  \ingroup Graphics
 */

/*! \ingroup ImgIO
 *  Creates image loaders basing on input file passed as argument.
 *  The file is then tried to be recognized by any loader,
 *  If found any compatibile loader, this loader is created and returned.
 */
class USELESS_API ImageFactory
{
public:
    static void Init() { _entries = new Entries; }
    static void Destroy() { delete _entries; }

    static void Register( int id, ImageLoader *(*loader)(), bool (*recognizer)( IFile &, int*,int*,int*) );
    static void RegisterSaver( int id, ImageSaver* (*saver)() );
    static void Unregister( int id );

    static SPointer< ImageLoader > Create( IFile &ifile );
    static SPointer< ImageLoader > Create( IFile &ifile, int *w, int *h, int *bpp );
    static SPointer< ImageSaver >  Create( int id );

private:
    typedef struct __ListEntry
    {
        __ListEntry(): _saver(0), _loader(0), _recognizer(0) {}
        ImageSaver  *(*_saver)();
        ImageLoader *(*_loader)();
        bool (*_recognizer)( IFile &, int *w, int *h, int *bpp);

    } ListEntry;
    
    typedef std::map< int, ListEntry > Entries;
    static Entries *_entries;

    friend class InitImageFactory;
};

DECLARE_PREINITIALIZER( ImageFactory, ImageFactory, USELESS_API );

};
#endif __INCLUDED_USELESS_IMAGE_FACTORY__
