#ifndef __INCLUDED_USELESS_SUB_I_FILE_SYSTEM_H__
#define __INCLUDED_USELESS_SUB_I_FILE_SYSTEM_H__

#include "Useless/File/IFileSystem.h"
#include "Useless/File/FileName.h"

namespace Useless {

class USELESS_API SubIFileSystem : public IFileSystem
{
public:
    SubIFileSystem( const SPointer<IFileSystem> &ifs,
                    const std::string &cut_off, //< real root dir name
                    const std::string &base );  //< mount point

    virtual SPointer<IFile> OpenFile( const std::string &name );
    
    virtual bool Exists( const std::string &name );
    virtual bool IsDir( const std::string &name );

    virtual void ScanDir( ScanCBType cb,
                    const std::string &parent_dir,
                     bool recursive=false );
private:
    SPointer<IFileSystem> _ifs;
    FileName    _cut_off;
    FileName    _base;
    FileName    _real_name;
    ScanCBType  _cb;

    void FoundDir( const std::string&, Types::QUAD, bool );

    bool RealName( const std::string &name);
};

};//namespace Useless
#endif//__INCLUDED_USELESS_SUB_I_FILE_SYSTEM_H__
