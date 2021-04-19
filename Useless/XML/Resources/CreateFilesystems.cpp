#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateFilesystems.h"
#include "Useless/File/StdIFileSystem.h"

namespace Useless {

    namespace {
        struct mount_cb : Functor_3< const std::string &, Types::QUAD, bool >
        {
            void operator()( const std::string &fileName, Types::QUAD fileSize, bool isDirectory )
            {
                // scan for all data{n}.htc

                if ( !isDirectory )
                {
                    std::string base("data");
                    std::string ext(".htc");
                    int p = fileName.find( base );

                    if ( ext == fileName.substr( fileName.size()-ext.size(), ext.size() ) && p != -1 )
                    {
                        std::string pri = fileName.substr( p + base.size(), fileName.size()-ext.size()-base.size()-p );
                        int priInt = (10 * VIFS::HTC) - (pri.empty() ? 0 : to_integer( pri ));
                        IFS::Instance().Mount( fileName, "data", priInt );
                    }
                }
            }
        };

        std::string EvalDirExpr( std::string dirExpr )
        {
            int brOpen = dirExpr.find("$(", 0 );
            if ( -1 != brOpen )
            {
                int brClose = dirExpr.find(")", brOpen+2 );
                if ( -1 != brClose )
                {
                    std::string envName = dirExpr.substr( brOpen+2, brClose-( brOpen+2 ));
                    const char *cEnv = getenv( envName.c_str() );
                    if (! cEnv )
                    {
                        throw Error("$(%s) is not set.", envName.c_str() );
                    }
                    dirExpr = dirExpr.substr( 0, brOpen ) + cEnv + dirExpr.substr( brClose+1, -1 );
                }
            }
            if ( !IFS::Instance().Exists( dirExpr ))
            {
                throw Error("\"%s\" is not file or directory.", dirExpr.c_str());
            }
            return dirExpr;
        }
    };

    Resource* CreateMountDir( XMLIterator i, XMLCreatorEnv *env )
    {
        const XMLAttributes &a = (*i)._attributes;
        std::string path = EvalDirExpr( get_attribute(a, "path") );
        IFS::Instance().SubMount( path, get_attribute(a, "point"), to_integer( get_attribute(a, "priority")) );
        return 0;
    }

    Resource* CreateMountHtc( XMLIterator i, XMLCreatorEnv *env )
    {
        const XMLAttributes &a = (*i)._attributes;
        std::string path = EvalDirExpr( get_attribute(a, "path") );
        IFS::Instance().Mount( path, get_attribute(a, "point"), to_integer( get_attribute(a, "priority")) );
        return 0;
    }

    Resource* CreateMountScan( XMLIterator i, XMLCreatorEnv *env )
    {
        const XMLAttributes &a = (*i)._attributes;
        StdIFileSystem().ScanDir( IFileSystem::ScanCBType( new mount_cb ), get_attribute(a, "path"), false );
        return 0;
    }

};
