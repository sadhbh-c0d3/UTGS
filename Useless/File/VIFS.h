#ifndef __INCLUDED_USELESS_VIFS_H__
#define __INCLUDED_USELESS_VIFS_H__

#include "Useless/File/IFileSystem.h"
#include "Useless/Util/Storage/AssocVector.h"
#include "Useless/Util/Singleton.h"

namespace Useless {

/*! Virtual Input (Read-Only) File System
 *
 *  It is composite fs containing several file subsystems.
 *  Generally it contains one StdIFileSystem, but user can
 *  mount any other IFileSystem. There is default mount function
 *  for hatcheries.
 */
class USELESS_API VIFS : public IFileSystem
{
public:
    /*! Create default StdIFileSystem */
    VIFS();
    /*! Destroy all file systems */
    virtual ~VIFS();

    enum IFSType { STD=100, HTC=1000, REF=50, USER=500 };

    /*! Mount function for hatcheries */
    void Mount( const std::string &hatchery_file, const std::string &base, int priority=HTC );
    /*! Mount supplied file system*/
    void Mount( const SPointer<IFileSystem> &fs, int priority=USER );
    /*! Mount STD file subsystem, map real_path to base */
    void SubMount( const std::string &real_path, const std::string &base, int priority=REF );

    /*! Retrive minimum priority used to mount given system type */
    int MinPri( IFSType ifs_type ) const;
    /*! Retrive maximum priority used to mount given system type */
    int MaxPri( IFSType ifs_type ) const;

    /*! Just IFileSystem methods overloads
     @{*/
    virtual SPointer<IFile> OpenFile( const std::string &name );
    virtual bool Exists( const std::string &name );
    virtual bool IsDir( const std::string &name );
    virtual void ScanDir( ScanCBType cb, const std::string &name, bool recursive=false );
    /*@}*/

    virtual int ChangePriority( int initialPriority, int desiredPriority );
    virtual int Umount( int atPriority );

private:
    typedef AssocVector< int, SPointer<IFileSystem> > MountedFS;
    MountedFS _mounted;
    
    typedef std::pair< int, int > MinMax;
    MinMax _std_min_max, _htc_min_max, _ref_min_max, _user_min_max;
};

DECLARE_SINGLETON( VIFS, IFS, USELESS_API );

/*! It is user responsibility to create
 */
struct USELESS_API IFSAutoMounter
{
    static void Init();
    static void Destroy() {}
};

DECLARE_PREINITIALIZER( IFSAutoMounter, IFSAutoMounter, USELESS_API );

};//namespace Useless

#endif//__INCLUDED_USELESS_VIFS_H__
