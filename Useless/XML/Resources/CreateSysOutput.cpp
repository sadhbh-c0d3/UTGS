#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSystem.h"
#include "Useless/Graphic/Planes/TransparentImage.h"

namespace Useless {

    struct SysOut
    {
        SPointer< Screen > _screen;
        std::map< std::string, SPointer< IGraphics > > _sources;

        struct ImageOut
        {
            SPointer< IGraphics > _image;
            Rect  _dstRect;
            ImageBase* getImage() { return dynamic_cast< ImageBase *>( _image.get() ); }
        };

        std::map< std::string, ImageOut > _imageOuts;
        Signal::FuncID _imgFID;

        void UpdateDisplay()
        {
            _screen->Clear( 0xFF000000 );
            for ( std::map< std::string, ImageOut >::iterator
                    itIO = _imageOuts.begin();
                    itIO!= _imageOuts.end();
                    ++itIO )
            {
                (*itIO).second.getImage()->SetDestWidth( (*itIO).second._dstRect.GetW() );
                (*itIO).second.getImage()->SetDestHeight( (*itIO).second._dstRect.GetH() );
                (*itIO).second.getImage()->BlitTo
                    ( (*itIO).second._dstRect.GetX(),
                      (*itIO).second._dstRect.GetY(), *_screen );
            }
            _screen->Swap();
        }
    };

    static SysOut s_SysOut;

    Resource* CreateSysOutputOpen( XMLIterator i, XMLCreatorEnv *env )
    {
        s_SysOut._screen = *ScreenResource( "system", get_attribute( (*i)._attributes, "screen" ));
        s_SysOut._imgFID = s_SysOut._screen->OnPaint.TieVoid( &s_SysOut, &SysOut::UpdateDisplay );
        return 0;
    }

    Resource* CreateSysOutputClose( XMLIterator i, XMLCreatorEnv *env )
    {
        s_SysOut._imageOuts.clear();
        s_SysOut._sources.clear();
        s_SysOut._imgFID.Untie();
        s_SysOut._screen.Reset();        
        return 0;
    }

    Resource* CreateSysOutputImage( XMLIterator i, XMLCreatorEnv *env )
    {
        std::string layer = get_attribute( (*i)._attributes, "layer" );
        std::string source = get_attribute( (*i)._attributes, "src" );
        std::vector< int > sRc = to_integer_vector( get_attribute( (*i)._attributes, "srcRect" ));
        std::vector< int > dRc = to_integer_vector( get_attribute( (*i)._attributes, "dstRect" ));
        std::string target("color");
        try_attribute( (*i)._attributes, "target", &target );

        Rect &srcRect = (Rect&)sRc[0];
        Rect &dstRect = (Rect&)dRc[0];

        if ( s_SysOut._sources.find( source ) == s_SysOut._sources.end() )
        {
            if ( target == "color" )
            {
                s_SysOut._sources[ source ] = new Image( source.c_str() );
            }
            else if ( target == "color+alpha" )
            {
                s_SysOut._sources[ source ] = new TransparentImage( source.c_str(), source.c_str() );
            }
        }
        
        SPointer< IGraphics > img = s_SysOut._sources[ source ];


        s_SysOut._imageOuts[ layer ]._image = img->Copy( srcRect );
        s_SysOut._imageOuts[ layer ]._dstRect = dstRect;

        s_SysOut.UpdateDisplay();
        return 0;
    }

};
