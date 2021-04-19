#ifndef __INCLUDED_USELESS_IFILESYSTEM_H__
#define __INCLUDED_USELESS_IFILESYSTEM_H__

/*
**  $Id: IFileSystem.h,v 1.6 2002/09/13 16:22:11 koolas Exp $
**
**  NAME
**      IFileSystem
**
**  PURPOSE
**      File system input access class.
**
**  AUTHOR(S)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#include "Useless/System/Types.h"
#include "Useless/Util/SPointer.h"
#include "Useless/Functional/Functor.h"
#include "Useless/File/IFile.h"
#include <string>



namespace Useless {


class USELESS_API IFileSystem
{
public:
    IFileSystem() {}
    virtual ~IFileSystem() {}

    virtual SPointer<IFile> OpenFile( const std::string &name ) = 0;
    virtual bool Exists( const std::string &name ) = 0;
    virtual bool IsDir( const std::string &name ) = 0;

    typedef Useless::FTernary<
        const std::string&, //!< full path and filename
               Types::QUAD, //!< file size
                      bool  //!< directory indicator
                           >::Ptr ScanCBType;

    virtual void ScanDir( ScanCBType cb, const std::string &parent_dir, bool recursive=false ) = 0;


private:
    IFileSystem( const IFileSystem& );            // prevent copy
    IFileSystem& operator=( const IFileSystem& ); // prevent copy
};


} // namespace Useless

#endif // __INCLUDED_USELESS_IFILESYSTEM_H__
