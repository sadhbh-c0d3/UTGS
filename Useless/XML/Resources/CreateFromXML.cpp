#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateFromXML.h"
#include "Useless/File/VIFS.h"
#include "Useless/File/IFileStream.h"
#include "Useless/Util/str_conv.h"

#include <direct.h>

#include "Useless/System/Application.h"

namespace Useless {

INIT_SINGLETON( XMLCreators, USELESS_RESOURCES_API );

std::set< std::string > s_includedFiles;


void _DispatchSystemMessages()
{
/*  
    static Timer t;
    Timer t1;
    if ( t1 - t > 0.05 )
    {
        Application::Step( Application::DEQUEUE | Application::TIMER );
        t = t1;
    }
*/
}

/*! Create resources in in given category
 */
void CategoryFromXML( const std::string &category, XMLIterator i )
{
    try{
        for ( i.StepInto(); !!i; ++i )
        {
            XMLCreatorEnv environment;
            environment.Set( "category", category.c_str() );

            std::string id = (*i)._name; //< Default id is name[n]
            if ( try_attribute( (*i)._attributes, "id", &id ) )
            {
                environment.Set( "name", id.c_str() );
            }

            if ( Resource *ptr = XMLCreators::Instance().Create( category, i, &environment ))
            {
                Resources::Instance().Insert( category, id, ptr );
            }
        }
    }
    catch( Error &e )
    {
        throw XMLParser::XMLError( (*i)._source, (*i)._line, e.GetError() );
    }
}

/*! Create child resources
 */
void ChildsFromXML( XMLIterator i, XMLCreatorEnv *env )
{
    SiblingsFromXML( i.StepInto(), env );
}

void SiblingsFromXML( XMLIterator i, XMLCreatorEnv *env )
{
    try{
        std::string branch;
        const char *p_branch;
        if ( (*env).Query( "branch", p_branch ) )
        {
            branch = p_branch;
        }

        for ( /*i.StepInto()*/; !!i; ++i )
        {
            const char *p_cat;
            (*env).Query("category", p_cat );
            std::string category( p_cat );
            std::string name( (*i)._name );

            int posCat = name.find("::");
            if ( -1 != posCat )
            {
                category = name.substr( 0, posCat );
                name = name.substr( posCat+2, name.size()-(posCat+2));
#pragma message("use of category::name changes const xml-node")
                const_cast< std::string & >( (*i)._name ) = name;
            }

            std::string id = (branch.empty() ? name : branch + "." + name);
            if ( try_attribute( (*i)._attributes, "id", &id ) )
            {
                id = (branch.empty() ? id : branch + "." + id );
                (*env).Set( "name", id.c_str() );
            }
            else
            {
                (*env).Set( "name", branch.c_str() );
            }

            if ( Resource *ptr = XMLCreators::Instance().Create( category, i, env ) )
            {
                Resources::Instance().Insert( category, id, ptr );
            }

            _DispatchSystemMessages();
        }
    }
    catch( XMLParser::XMLError & )
    {
        throw;
    }
    catch( Error &e )
    {
        throw XMLParser::XMLError( (*i)._source, (*i)._line, e.GetError() );
    }
}

/*! Create all resources from XML string
 */
void CreateFromXML( const std::string &xml_sring, const std::string &source )
{
    XMLDocument _xml( xml_sring, source);
    for ( XMLIterator i=_xml.Root().StepInto(); !!i; ++i )
    {
        const std::string name = (*i)._name;

        if ( name == "include")
        {
            std::string oldPath;
            std::string newPath;
            if ( try_attribute( (*i)._attributes, "chroot", &newPath ))
            {   
                int brOpen = newPath.find("$(", 0 );
                if ( -1 != brOpen )
                {
                    int brClose = newPath.find(")", brOpen+2 );
                    if ( -1 != brClose )
                    {
                        std::string envName = newPath.substr( brOpen+2, brClose-( brOpen+2 ));
                        const char *cEnv = getenv( envName.c_str() );
                        if (! cEnv )
                        {
                            throw Error("$(%s) is not set.", envName.c_str() );
                        }
                        newPath = newPath.substr( 0, brOpen ) + cEnv + newPath.substr( brClose+1, -1 );
                    }
                }
                if ( !IFS::Instance().IsDir( newPath ))
                {
                    throw Error("\"%s\" is not directory.", newPath.c_str());
                }

                char cwd[_MAX_PATH];
                _getcwd( cwd, _MAX_PATH );
                oldPath = cwd;
                _chdir( newPath.c_str() );
            }
            std::string src = get_attribute( (*i)._attributes, "src");
            if ( s_includedFiles.insert( src ).second )
            {
                CreateFromXMLFile( src );
            }
            if ( !oldPath.empty() )
            {
                _chdir( oldPath.c_str() );
            }
        }
        else
        {
            CategoryFromXML( name, i );
        }
    }
}

/*! Create all resources from XML file
 */
void CreateFromXMLFile( const std::string &xml_file )
{
    CreateFromXMLFile( IFS::Instance().OpenFile( xml_file ));
}

void CreateFromXMLFile( SPointer< IFile > fileInput )
{
    std::string ln,data;
    IFileStream desc_file( fileInput );

    while ( std::getline( desc_file, ln ) )
    {
        data += ln;
        data += "\n";
    }

    std::string name = fileInput->GetName();
    if ( ':' != name[1] )
    {
        char buffer[255] = {0,};
        memset( buffer, 0, 255);
        getcwd( buffer, 255);
        std::string cdir( buffer );
        name = cdir + "/" + name;
    }
    CreateFromXML( data, name );
}

} // namespace Useless
