#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSkins.h"
#include "Useless/Graphic/Planes/Image.h"
#include "Useless/Graphic/Planes/TransparentImage.h"

namespace Useless {

    Resource* CreateImage( XMLIterator i, XMLCreatorEnv *env )
    {
        const XMLParser::Node &node = *i;

        std::string src, alpha;
        src = get_attribute( node._attributes, "src" );
        int color;
        SPointer< ImageBase > p_image;

        int count=1;
        std::string::iterator p,q;
        if ( try_attribute_i( node._attributes, "count", &count ))
        {
            int comma = src.rfind(".", src.size());
            for ( int i=comma-1; i>0; --i )
            {
                if ( src[i] < '0' || src[i] > '9' )
                {
                    p = src.begin()+(i+1);
                    q = src.begin()+comma;
                    break;
                }
            }

            const char *c_id;
            env->Query("name", c_id );
                
            if ( try_attribute( node._attributes, "alpha", &alpha ) )
            {
                std::string::iterator p1,q1;
                int comma1 = alpha.rfind(".", alpha.size());
                for ( int i=comma1-1; i>0; --i )
                {
                    if ( alpha[i] < '0' || alpha[i] > '9' )
                    {
                        p1 = alpha.begin()+(i+1);
                        q1 = alpha.begin()+comma1;
                        break;
                    }
                }
                
                for ( int i=0; i<count; ++i )
                {
                    std::string img_num( from_integer< char >( i ));
                    std::copy( img_num.begin(), img_num.end(), q-img_num.size() );
                    std::copy( img_num.begin(), img_num.end(), q1-img_num.size() );

                    p_image = new TransparentImage( src.c_str(), alpha.c_str() );

                    Resources::Instance().Insert("images", std::string( c_id ),
                            new AnyResource< SPointer< ImageBase > >( p_image ));
                }
            }
            else
            {
                for ( int i=0; i<count; ++i )
                {
                    std::string img_num( from_integer< char >( i ));
                    std::copy( img_num.begin(), img_num.end(), q-img_num.size() );

                    if ( try_attribute_h( node._attributes, "color_key", &color ) )
                    {
                        p_image = new Image( src.c_str(), color);
                    }
                    else
                    {
                        p_image = new Image( src.c_str() );
                    }

                    Resources::Instance().Insert("images", std::string( c_id ),
                            new AnyResource< SPointer< ImageBase > >( p_image ));
                }
            }
            return 0;
        }
        else
        {
            if ( try_attribute_h( node._attributes, "color_key", &color ) )
            {
                p_image = new Image( src.c_str(), color);
            }
            else if ( try_attribute( node._attributes, "alpha", &alpha ) )
            {
                p_image = new TransparentImage( src.c_str(), alpha.c_str() );
            }
            else
            {
                p_image = new Image( src.c_str() );
            }
        
            return new AnyResource< SPointer<ImageBase> >( p_image );
        }
    }

    SPointer<ImageBase> MakeImage( const std::string &id )
    {
        int lb, rb;
        std::string name;
        bool use_crop_rect;
        Rect crop_rect;

        if ( (lb=id.find("(",0))!=-1 && (rb=id.find(")",0))!=-1 )
        {
            if ( lb+1 < rb )
            {
                name = id.substr(0, lb);
                std::vector<int> r = to_integer_vector(id.substr(lb+1, lb-rb-2));
                memcpy( &crop_rect.x, &*r.begin(), 4*sizeof(int) );
                use_crop_rect = true;
            }
            else
            {
                throw Error("QueryImage: Bad syntax");
            }
        }
        else
        {
            name = id; use_crop_rect = false;
        }

        SPointer<ImageBase> sp_image;

        if ( Resources::Instance().Exists("images", name) )
        {
            SPointer<ImageBase> *p_image;
            Resources::Instance().Query("images", name, p_image);
            sp_image = *p_image;
        }
        else
        {
            sp_image = new Image( name.c_str() );
            Resources::Instance().Insert( "images", name, new AnyResource< SPointer<ImageBase> >(sp_image) );
        }

        if ( use_crop_rect )
        {
            sp_image = static_cast<ImageBase*>(sp_image->Copy( crop_rect ).Release());
        }

        return sp_image;
    }

    Resource* CreateImageCopy( XMLIterator i, XMLCreatorEnv *env )
    {
        std::string img = get_attribute( (*i)._attributes, "img" );
        std::string rectStr = get_attribute( (*i)._attributes, "rect" );
        std::vector< int > r = to_integer_vector( rectStr );
        Rect rect( r[0], r[1], r[2], r[3] );
        int count = 0;
        if ( try_attribute_i( (*i)._attributes, "count", &count ))
        {
            std::string id = get_attribute( (*i)._attributes, "id" );
            for ( int i=0; i<count; ++i )
            {
                SPointer< IGraphics > spImage = QueryImage( img + '[' + from_integer< char >(i) + ']' ).Copy( rect );
                Resources::Instance().Insert( "images", id, new AnyResource< SPointer< ImageBase > >( dynamic_cast< ImageBase *>( spImage.Release() )));
            }

            return NULL;
        }
        else
        {
            SPointer< IGraphics > spImage = QueryImage( img ).Copy( rect );
            return new AnyResource< SPointer< ImageBase > >( dynamic_cast< ImageBase *>( spImage.Release() ));
        }
    }

};//namespace Useless
