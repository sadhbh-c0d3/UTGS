#include "UselessPch.h"

#pragma warning(disable:4786)
#include "Useless/File/Hatchery.h" // <- look here for declaration
#include "Useless/File/StdIFileSystem.h"
#include "Useless/File/StdIFile.h"
#include "Useless/File/StdOFile.h"
#include "Useless/Util/Storage/MemBlock.h"
#include "Useless/Util/Math.h"
#include "Useless/Error/Error.h"
#include <algorithm>
#include "Useless/File/FileName.h"
#include "Useless/Error/ErrorMessage.h"
#include "Useless/HatcheryConfig.h" // - hatchery configuration

namespace Useless {

    using namespace std;

    void HatcheryBuilder::Create( const std::string &in_dir, const std::string &out_name )
    {
        _base = in_dir;
        _nodes.clear();
        StdIFileSystem fs;
        fs.ScanDir( TernaryCall( this, &HatcheryBuilder::AddNode ), in_dir, true );
        SPointer<OFile> sp_data_file = new StdOFile( out_name, true );
        HatcheryCipher::Out hatch( sp_data_file, g_szKey );

        hatch.Write( __cipherType, 4 );

        int baseoffset = HatcheryCipher::BLOCKSIZE;
        char zeroBuf[ HatcheryCipher::BLOCKSIZE ] = {0,};
        hatch.Write( zeroBuf, HatcheryCipher::BLOCKSIZE - 4 );

        // pass 1 - calc header len
        {
            for(NodeIter i=_nodes.begin(); i!=_nodes.end(); ++i)
            {
                if ( i->second >= 0 )
                {
                    IOHeader head;
                    head[ PATH ].Set( i->first );
                    baseoffset += head.Size();
                }
            }
        }

        int bytesOfAlignment = 0L;

        if ( baseoffset % HatcheryCipher::BLOCKSIZE )
        {
            bytesOfAlignment = HatcheryCipher::BLOCKSIZE - baseoffset % HatcheryCipher::BLOCKSIZE;
        }

        baseoffset += bytesOfAlignment;

        //#   ifndef __ENCRYPT_DATA__ //-- Align baseoffset to BLOCKSIZE
        //#   endif


        // pass 2 - write header
        {
            int offset = baseoffset;
            for(NodeIter i=_nodes.begin(); i!=_nodes.end(); ++i)
            {
                Types::QUAD size = i->second;
                if ( size >= 0 )
                {
                    IOHeader head;
                    head[ PATH ].Set( i->first );
                    head[ OFFSET ].Set( offset );
                    head[ SIZE ].Set( size );
                    head.Save( hatch );

                    offset += size; 
                    if ( offset % HatcheryCipher::BLOCKSIZE )
                    {
                        offset += HatcheryCipher::BLOCKSIZE - offset % HatcheryCipher::BLOCKSIZE;
                    }
                }
            }
        }

        hatch.Write( zeroBuf, bytesOfAlignment );
        C_verify( baseoffset == sp_data_file->GetPos() );

#   ifndef __ENCRYPT_DATA__ //-- Close crypto file, continue with plain data
        hatch.Close();
        C_verify( baseoffset == sp_data_file->GetPos() );
#   endif

        // pass 3 - write contents
        {
            enum { BUFSIZE = 1024*256 };
            MemBlock buf( BUFSIZE );
            int offset = baseoffset;
            for(NodeIter i=_nodes.begin(); i!=_nodes.end(); ++i)
            {
                const std::string &name = i->first;
                Types::QUAD size = i->second;
                offset += size;
                if(size>0)
                {
                    printf("Writting %s: ", (in_dir +"/"+ name).c_str());
                    fflush( stdout );
                    StdIFile ifile( in_dir +"/"+ name );
                    while(size>0)
                    {
                        const int iolen = SimpleMin( size, Types::QUAD(BUFSIZE) );
                        ifile.Read( buf.GetPtr(), iolen );

#                   ifdef __ENCRYPT_DATA__
                        hatch.Write( buf.GetPtr(), iolen );
#                   else
                        sp_data_file->Write( buf.GetPtr(), iolen );
#                   endif

                        size -= iolen;
                        printf(".");
                        fflush( stdout );
                    }
                    printf(" OK\n");
                    fflush( stdout );
                }
                if ( offset % HatcheryCipher::BLOCKSIZE )
                {
                    int align = HatcheryCipher::BLOCKSIZE - offset % HatcheryCipher::BLOCKSIZE;
                    offset += align;                    
                    hatch.Write( zeroBuf, align );
                    C_verify( offset == sp_data_file->GetPos() );
                }

            }
        }

#   ifdef __ENCRYPT_DATA__
        hatch.Close();
#   endif
        sp_data_file->Close();
    }



    void HatcheryBuilder::AddNode( const std::string &orgname, Types::QUAD size, bool isdir )
    {
        if (( orgname.find("CVS") == -1 ) && ( orgname.find(".svn") == -1 ) && !isdir )
        {
            FileName file_name = orgname;
            file_name -= _base;
            std::string name = file_name;

            Types::QUAD storesize = isdir ? -1 : size;
            pair<NodeIter,bool> result = _nodes.insert( make_pair( name, storesize ) );
            bool inserted = result.second;
            if(! inserted) { throw Error("Name conflict."); }
            NodeIter prev = (result.first);
            if(prev != _nodes.begin())
            {
                --prev;
                std::string prevname = prev->first + '/';
                if(name.substr(0,prevname.size())==prevname)
                {
                    C_verify(prev->second==-1);
                    _nodes.erase( prev );
                }
            }

            C_message( HatcheryBuilder, 0, ("Node added:", name.c_str(), (int)size ));
            printf("Found: %s\n", name.c_str());
        } // no CVS files
    }

};//namespace Useless
