#ifndef __INCLUDED_USELESS_HATCHERY_H__
#define __INCLUDED_USELESS_HATCHERY_H__

#include "Useless/HatcheryConfig.h"
#include "Useless/Util/IOStruct.h"
#include "Useless/File/IFileSystem.h"
#include "Useless/File/SubIFile.h"
#include "Useless/File/FileName.h"
#include <map>

namespace Useless {

class USELESS_API HatcheryBase
{
public:
    // Here we define names of structure fields
    // Note: ArgAt type doesn't get any memory space, since it's empty type
    static const ArgAt<0> PATH;
    static const ArgAt<1> OFFSET;
    static const ArgAt<2> SIZE;

    // Here we define types of structure fields
    typedef IOStruct<
        TypeList< IOString,
        TypeList< IOQuadLE,
        TypeList< IOQuadLE, NullType > > > > IOHeader;

    HatcheryBase() {}
};


//! Creates hatchery from directory
class USELESS_API HatcheryBuilder : public HatcheryBase
{
public:
    void Create( const std::string &in_dir, const std::string &out_name );

private:
    typedef std::map< std::string, Types::QUAD > NodeMap;
    typedef NodeMap::iterator NodeIter;
    NodeMap _nodes;
    mutable std::string _base;

    void AddNode( const std::string &orgname, Types::QUAD size, bool isdir );
};


//! Hatchery - packed files
class USELESS_API Hatchery : public HatcheryBase, public IFileSystem
{
public:
    Hatchery();
    Hatchery( const std::string &key );
    void Open( const std::string &in_name, const std::string &base );
    void Close() {}

    virtual SPointer<IFile> OpenFile( const std::string &name );
    virtual bool Exists( const std::string &name );
    virtual bool IsDir( const std::string &name );
    virtual void ScanDir( ScanCBType cb, const std::string &name, bool recursive=false );

    void DumpHeader( std::ostream &out, const std::string &in_name );

    void SetKey( const std::string &key );
    void ResetKey();
    static void SetGlobalKey( const std::string &key );

private:
    struct FileNode
    {
        Types::QUAD offset;
        Types::QUAD size;
        bool IsDir() const { return size==-1; }
    };
    typedef std::map< FileName, FileNode > NodeMap;
    typedef NodeMap::iterator Iterator;
    NodeMap         _nodes;
    std::string     _in_name;
    SPointer<IFile> _file;
    std::string     _key;

    void AddNode( const std::string &name, const FileNode &node );
    
    SPointer<IFile> GainAccess();
    void RevokeAccess();
    friend class HatchedIFile;
};

//! HatchedIFile - IFile created from hatchery
class USELESS_API HatchedIFile : public SubIFile
{
public:
    HatchedIFile( Hatchery &hatchery, const std::string &name );
    virtual ~HatchedIFile() throw();

private:
    Hatchery &_hatchery;
};


};//namespace Useless
#endif//__INCLUDED_USELESS_HATCHERY_H__
