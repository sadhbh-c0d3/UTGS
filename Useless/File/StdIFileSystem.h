#ifndef __INCLUDED_USELESS_STDIFILESYSTEM_H__
#define __INCLUDED_USELESS_STDIFILESYSTEM_H__

/*
**  $Id: StdIFileSystem.h,v 1.4 2002/09/12 14:27:36 koolas Exp $
**
**  NAME
**      StdIFileSystem
**
**  PURPOSE
**      Standard file system input access class.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#include "Useless/File/IFileSystem.h"



namespace Useless {


class USELESS_API StdIFileSystem : public IFileSystem
{
public:
    StdIFileSystem() {}
    virtual ~StdIFileSystem() {}

    virtual SPointer<IFile> OpenFile( const std::string &name );
    virtual bool Exists( const std::string &name );
    virtual bool IsDir( const std::string &name );
    virtual void ScanDir( ScanCBType cb, const std::string &name, bool recursive=false );


private:
    StdIFileSystem( const StdIFileSystem& );            // prevent copy
    StdIFileSystem& operator=( const StdIFileSystem& ); // prevent copy
};


} // namespace Useless

#endif // __INCLUDED_USELESS_STDIFILESYSTEM_H__
