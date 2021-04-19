#include "Useless/UselessConfig.h"

#undef GetFirstChild

/* Include media I/Os

*/

// BMP is included in Uselsss.Graphic.FileIO
#include "Useless/Graphic/FileIO/BMPLoader.h"

// TGA is included in Useless.Graphic.FileIO
#include "Useless/Graphic/FileIO/TGALoader.h"

// PNG requires extrernal libPNG
#ifdef USELESS_HAS_LIBPNG
#   include "Useless/Graphic/FileIO/PNGLoader.h"
#endif

// WAV is included in Useless.Sound
#include "Useless/Sound/FileIO/WaveLoader.h"


#include "Useless/Graphic/Planes/TransparentImage.h"
#include "Useless/Sound/SampleFile.h"

#include "Useless/File/VIFS.h"
#include "Useless/File/StdOFile.h"
#include "Useless/Util/Crypto.h"

#include "Useless/Util/XMLTag.h"

#include "Useless/System/factory/CreateScreen.h"

#include "Useless/Graphic/device/winGDI/GDIPrinter.h" // < non x-platform solution - requires testing

#include <koolib/xml/XMLModuleUseless.h>
#include <cmath>
#define M_PI 3.14159265358979323846



namespace XMLProgram {

    struct GDIPageProxy : XMLCodeBlock
    {
        SPointer< GDIPage > m_page;

        GDIPageProxy( SPointer< GDIPage > p ): m_page( p )
        {
            add_methods( m_page.get(), this )
                .def("Finish", &GDIPage::Finish )
                ;

            add_methods( this )
                .def("BlitImage", &GDIPageProxy::BlitImage )
                ;
        }
        
        void BlitImage( Node __unused__, ExecutionState &_state )
        {
            IChunk *pImage =_state._currentBlock->GetChunk( L"Imag" );
            if ( !pImage ) { throw Error("BlitImage: expects 'Imag'"); }
            ImageProxy *pImageProx = dynamic_cast< ImageProxy *>( pImage );
            if ( !pImageProx ) { throw Error("BlitImage: expects 'Imag' to be type of Image");} 
            IChunk *pCanv = _state._currentBlock->GetChunk( L"Canv" );
            if ( !pCanv ) { throw Error("BlitImage: expects 'Canv'");}            
            int x = attribute_of< int >( pCanv, L"x" );
            int y = attribute_of< int >( pCanv, L"y" );
            int w = attribute_of< int >( pCanv, L"w" );
            int h = attribute_of< int >( pCanv, L"h" );
            SPointer< ImageBase > spImg = pImageProx->_spImage;
            m_page->BlitImage( Rect( x, y, w, h ), *spImg );
        }

    };

    struct GDIDocumentProxy : XMLCodeBlock
    {
        SPointer< GDIDocument > m_document;

        GDIDocumentProxy( SPointer< GDIDocument > d ): m_document( d )
        {
            add_methods( m_document.get(), this )
                .def("Print", &GDIDocument::Print )
                ;

            add_methods( this )
                .def("CreatePage", &GDIDocumentProxy::CreatePage )
                ;
        }

        IChunkPtr CreatePage()
        {
            return new GDIPageProxy( m_document->CreateGDIPage() );
        }
    };
    
    struct GDIPrinterProxy : XMLCodeBlock
    {
        SPointer< GDIPrinter > m_printer;

        GDIPrinterProxy( SPointer< GDIPrinter > p ): m_printer( p )
        {
            add_methods( this )
                .def("CreateDocument", &GDIPrinterProxy::CreateDocument, "title" )
                .add("sizeOnPaperX", make_value_chunk( m_printer->m_sizeOnPaperX ))
                .add("sizeOnPaperY", make_value_chunk( m_printer->m_sizeOnPaperY ))
                .add("xLeft", make_value_chunk( m_printer->m_xLeft ))
                .add("yTop", make_value_chunk( m_printer->m_yTop ))
                .add("inchOnPaperX", make_value_chunk( m_printer->m_inchOnPaperX ))
                .add("inchOnPaperY", make_value_chunk( m_printer->m_inchOnPaperY ))
                .add("sizeInchesX", make_value_chunk( m_printer->m_sizeInchesX ))
                .add("sizeInchesY", make_value_chunk( m_printer->m_sizeInchesY ))
                ;
        }

        IChunkPtr CreateDocument( TextUtf8 title )
        {
            return new GDIDocumentProxy( m_printer->CreateGDIDocument( title ));
        }
    };

    /*****************************************************************

      Modules to be imported

     *****************************************************************/

    namespace XML_IMPORT {



        /*****************************************************************

          Module Useless_Tk

*/

        USELESS_TK::USELESS_TK( Node node, ExecutionState &state )
        {
            Attr< std::string, false > soundcard("soundcard");
            Attr< std::string, false > screen("screen");
            if ( GetAttr( screen, node, state ))
            {
                _screen = *ScreenResource("system", screen.str());
            }
            if ( GetAttr( soundcard, node, state ))
            {
                _soundcard = *SoundCardResource("system", soundcard.str());
            }

            add_methods( this )
                .def("CirclePoint", &USELESS_TK::CirclePoint, "i", "n", "start", "quaterArea")
                .def("GetColorByName", &USELESS_TK::GetColorByName, "name")
                .def("MakeRGB", &USELESS_TK::MakeRGB, "red", "green", "blue")
                .def("MakeRGBA", &USELESS_TK::MakeRGBA, "red", "green", "blue", "alpha")
                .def("GetKeyCode", &USELESS_TK::GetKeyCode, "name")
                .def("GetKeyName", &USELESS_TK::GetKeyName, "code")
                .def("SetInputLocale", &USELESS_TK::SetInputLocale, "name")
                .def("CreatePointList", &USELESS_TK::CreatePointList )
                .def("CreateRectList", &USELESS_TK::CreateRectList )
                .def("CreateScreen", &USELESS_TK::CreateScreen )
                .def("CreatePrinter", &USELESS_TK::CreatePrinter )
                .def("CreateImageBuffer", &USELESS_TK::CreateImageBuffer, "width", "height")
                .def("CreateImage", &USELESS_TK::CreateImage )
                .def("CreateSample", &USELESS_TK::CreateSample, "file" )
                .def("CreateChannel", &USELESS_TK::CreateChannel )
                .def("CreateOggVorbis", &USELESS_TK::CreateOggVorbis, "file")
                .def("CreateVideo", &USELESS_TK::CreateVideo, "file")
                .def("CreateMovie", &USELESS_TK::CreateMovie, "file")
                .def("CreateMovie2", &USELESS_TK::CreateMovie2, "file", "audio")
                .def("CreateFontPainter", &USELESS_TK::CreateFontPainter )
                .def("IsDir", &USELESS_TK::IsDir, "path")
                .def("FileExists", &USELESS_TK::FileExists, "path")
                .def("ListFiles", &USELESS_TK::ListFiles, "path")
                .def("ReadFile", &USELESS_TK::ReadFile )
                .def("WriteFile", &USELESS_TK::WriteFile )
                .def("ToXMLString", &USELESS_TK::ToXMLString )
                .def("FromXMLString", &USELESS_TK::FromXMLString )
                ;
        }


        Useless::Pos USELESS_TK::CirclePoint( double i, double n, double start, const Useless::Rect &quaterArea )
        {
            double angle = i * 2 * M_PI / n + start * M_PI / 180.0;
            double x = std::cos( angle ) * quaterArea.GetW() + quaterArea.GetX();
            double y = std::sin( angle ) * quaterArea.GetH() + quaterArea.GetY();
            return Useless::Pos( x, y );
        }
        
        int USELESS_TK::GetColorByName( std::string name )
        {
            Attr< Color > color("color");
            color.str( name );
            return *color;
        }

        int USELESS_TK::MakeRGB( int red, int green, int blue )
        {
            return Useless::NormalPixel( Useless::MakeRGB( red, green, blue )).u;
        }

        int USELESS_TK::MakeRGBA( int red, int green, int blue, int alpha )
        {
            return Useless::NormalPixel( Useless::MakeRGBA( red, green, blue, alpha )).u;
        }
        
        int USELESS_TK::GetKeyCode( std::string name )
        {
            return Keys::GetCode( name );
        }
        
        std::string USELESS_TK::GetKeyName( int keyCode )
        {
            const Keys::CodeMap &codes = Keys::GetCodeMap();
            for( Keys::CodeMap::const_iterator it = codes.begin(); it != codes.end(); ++it )
            {
                if ( (*it).second == keyCode )
                {
                    return (*it).first;
                }
            }
            return "NONE";
        }
            
        void USELESS_TK::SetInputLocale( std::string localeName )
        {
            Application::SetEncoding( localeName.c_str() );
        }

        void USELESS_TK::CreatePointList( Node node, ExecutionState &state )
        {
            PointListProxy *pProxy;
            state.SetResult( pProxy = new PointListProxy( PointList() ));
            IChunkPtr pList = XMLProgram::GetChunk(L"list", state);
            while ( !IsEmpty( pList.get()) )
            {
                pList = Force( pList, state );
                if ( IsEmpty( pList.get() )) { break; }
                IChunk *pElement = pList->GetChunk( FOURCC_LIST_HEAD );
                pProxy->AddPoint( Pos(
                    value_of< int >( pElement->GetChunk(L"x")),
                    value_of< int >( pElement->GetChunk(L"y"))
                            ));
                pList = pList->GetChunk( FOURCC_LIST_TAIL );
            }
        }

        void USELESS_TK::CreateRectList( Node node, ExecutionState &state )
        {
            RectListProxy *pProxy;
            state.SetResult( pProxy = new RectListProxy( RectList() ));
            IChunkPtr pList = XMLProgram::GetChunk(L"list", state);
            while ( !IsEmpty( pList.get()) )
            {
                pList = Force( pList, state );
                if ( IsEmpty( pList.get() )) { break; }
                IChunk *pElement = pList->GetChunk( FOURCC_LIST_HEAD );
                pProxy->AddRect( Rect (
                    value_of< int >( pElement->GetChunk(L"x")),
                    value_of< int >( pElement->GetChunk(L"y")),
                    value_of< int >( pElement->GetChunk(L"w")),
                    value_of< int >( pElement->GetChunk(L"h"))
                            ));
                pList = pList->GetChunk( FOURCC_LIST_TAIL );
            }
        }
        
        void USELESS_TK::CreateScreen( Node node, ExecutionState &state )
        {
            Attr< std::string, true, char > _title("title");
            Attr< int, true, char > _w("w");
            Attr< int, true, char > _h("h");
            Attr< int, false,char > _windowed("windowed");
            
            GetAttr( _title, node, state );
            GetAttr( _w, node, state );
            GetAttr( _h, node, state );

	    SPointer< Screen > spScreen = Creator< Screen >::Create( _title.str().c_str() );
            
            if ( GetAttr( _windowed, node, state ) && *_windowed )
            {
                spScreen->OpenWindowed( *_w, *_h );
            }
            else
            {
                Attr< int, true,  char > _bpp("bpp");
                Attr< int, false, char > _refresh("refresh");
                GetAttr( _bpp, node, state );
                if ( GetAttr( _refresh, node, state ))
                {
                    spScreen->Open( *_w, *_h, *_bpp, *_refresh );
                }
                else
                {
                    spScreen->Open( *_w, *_h, *_bpp );
                }
            }
            state.SetResult( new ScreenProxy( spScreen ));
        }

        void USELESS_TK::CreatePrinter( Node node, ExecutionState &state )
        {
            Attr< std::string, false, char > _paperSize("paperSize");
            Attr< std::string, false, char > _paperOrient("paperOrient");
            GetAttr( _paperSize, node, state );
            GetAttr( _paperOrient, node, state );
            int paperSize = DMPAPER_A4;
            int paperOrient = DMORIENT_PORTRAIT;
            if ( "Landscape" == _paperOrient.str() )
            {
                paperOrient = DMORIENT_LANDSCAPE;
            }
            typedef std::map< std::string, int > Name2Int;
            static Name2Int s_PaperSizes;
            if ( s_PaperSizes.empty() )
            {
                s_PaperSizes["Letter"] = DMPAPER_LETTER;
                s_PaperSizes["Legal"] = DMPAPER_LEGAL;
                s_PaperSizes["A3"] = DMPAPER_A3;
                s_PaperSizes["A3 Rotated"] = DMPAPER_A3_ROTATED;
                s_PaperSizes["A4"] = DMPAPER_A4;
                s_PaperSizes["A4 Rotated"] = DMPAPER_A4_ROTATED;
                s_PaperSizes["B5"] = DMPAPER_B5;
                s_PaperSizes["Letter Rotated"] = DMPAPER_LETTER_ROTATED;
                s_PaperSizes["Letter Small"] = DMPAPER_LETTERSMALL;
            }
            Name2Int::iterator foundSize = s_PaperSizes.find( _paperSize.str() );
			if ( s_PaperSizes.end() != foundSize ) { paperSize = (*foundSize).second; }
            state.SetResult( new GDIPrinterProxy( CreateGDIPrinter( paperSize, paperOrient ) ));
        }

        void USELESS_TK::CreateImage( Node node, ExecutionState &state )
        {
            Attr< std::string, false, char > _color("color");
            Attr< std::string, false, char > _alpha("alpha");
            Attr< std::string, false, char > _color_alpha("color+alpha");

            if ( GetAttr( _color, node, state ))
            {
                if ( GetAttr( _alpha, node, state ))
                {                        
                    state.SetResult( new ImageProxy( new TransparentImage( _color.str().c_str(), _alpha.str().c_str() )));
                }
                else
                {
                    state.SetResult( new ImageProxy( new Image( _color.str().c_str() )));
                }
            }
            else if ( GetAttr( _color_alpha, node, state ))
            {
                state.SetResult( new ImageProxy( new TransparentImage( _color_alpha.str().c_str(), _color_alpha.str().c_str())));
            }
            else
            {
                throw Error("GetImage requires 'color' or 'color+alpha' defined.");
            }
        }
        
        IChunkPtr USELESS_TK::CreateImageBuffer( int width, int height )
        {
            Useless::ImageBuffer *pBuf = new Useless::ImageBuffer( width, height );
            IChunkPtr proxy = new ImageProxy( pBuf );
            pBuf->Cooperate( *_screen->GetSurface() );
            return proxy;
        }
        
        IChunkPtr USELESS_TK::CreateSample( std::string fileName )
        {
            return new SampleProxy( new SampleFile( fileName ));
        }

        void USELESS_TK::CreateChannel( Node __unused__, ExecutionState &state )
        {
            IChunkPtr p = XMLProgram::GetChunk(L"sample", state);
            IChunk *pLoop = XMLProgram::GetChunk(L"loop", state);
            if ( SampleProxy *pSample = dynamic_cast< SampleProxy *>( p.get() ))
            {
                bool loop = false;
                if ( pLoop ) { loop = value_of< int >( pLoop ); }                    
                Channel ch = _soundcard->Play( *pSample->_spSample, loop, true );
                state.SetResult( new ChannelProxy( ch ));
            }
            else
            {
                throw Error("Sample expected");
            }
        }

#ifdef USELESS_HAS_VORBIS
        IChunkPtr USELESS_TK::CreateOggVorbis( std::string fileName )
        {
            return new OggVorbisProxy(
                    IFS::Instance().OpenFile( fileName ),
                    _soundcard );
        }
#else
        IChunkPtr USELESS_TK::CreateOggVorbis( std::string fileName )
		{
			return CreateBlock();
		}
#endif

#ifdef USELESS_HAS_DSHOW
        IChunkPtr USELESS_TK::CreateVideo( std::string fileName )
        {
            SPointer< DSMedia > spMedia = new DSMedia( fileName.c_str(), DSMedia::VIDEO );
            spMedia->Cooperate( *_screen->GetSurface() );
            return new DSMediaProxy( spMedia );
        }

        IChunkPtr USELESS_TK::CreateMovie( std::string fileName )
        {
            SPointer< DSMedia > spMedia = new DSMedia( fileName.c_str(), DSMedia::VIDEO + DSMedia::AUDIO );
            spMedia->Cooperate( *_screen->GetSurface() );
            spMedia->Cooperate( *_soundcard );
            return new DSMediaProxy( spMedia );
        }
        
        IChunkPtr USELESS_TK::CreateMovie2( std::string fileName, TextUtf8 audioName )
        {
            SPointer< DSMedia > spMedia = new DSMedia( fileName.c_str(), audioName );
            spMedia->Cooperate( *_screen->GetSurface() );
            spMedia->Cooperate( *_soundcard );
            return new DSMediaProxy( spMedia );
        }
#else
        IChunkPtr USELESS_TK::CreateVideo( std::string fileName )
        {
            return CreateBlock();
        }
        IChunkPtr USELESS_TK::CreateMovie( std::string fileName )
        {
            return CreateBlock();
        }

        IChunkPtr USELESS_TK::CreateMovie2( std::string fileName, TextUtf8 audioName )
        {
            return CreateBlock();
        }
#endif
            
        void USELESS_TK::CreateFontPainter( Node node, ExecutionState &state )
        {
            FontPainter fp;
            IChunkPtr pFont = XMLProgram::GetChunk( L"font", state );
            IChunkPtr pText = XMLProgram::GetChunk( L"text", state );
            IChunkPtr pWidth = XMLProgram::GetChunk( L"width", state );
            if ( !!pFont )
            {
                fp.SetFont( argument_traits< const Useless::Font & >::get("font", node, state ));
            }
            if ( !!pText )
            {
                fp.SetText( value_of< TextUtf8 >( pText ));
            }
            if ( !!pWidth )
            {
                fp.SetWidth( value_of< int >( pWidth ));
            }
            state.SetResult( new FontPainterProxy( fp ));
        }



        struct scan_dir_function : Functor_3< const std::string &, Types::QUAD, bool >
        {
            typedef std::list< std::pair< std::string, bool > > FileList; FileList &m_files;
            scan_dir_function( FileList &files ):m_files( files ) {}
            void operator()( const std::string &fileName, Types::QUAD fileSize, bool isDirectory )
            {
                m_files.push_back( std::make_pair( fileName, isDirectory ));
            }
        };

        int USELESS_TK::IsDir( std::string path )
        {
            return IFS::Instance().IsDir( path );
        }

        int USELESS_TK::FileExists( std::string path )
        {
            return IFS::Instance().Exists( path );
        }

        IChunkPtr USELESS_TK::ListFiles( std::string path )
        {
            scan_dir_function::FileList files;
            IFS::Instance().ScanDir( IFileSystem::ScanCBType( new scan_dir_function( files )), path, false );
            IChunkPtr pPrev = CreateEmpty();
            for ( scan_dir_function::FileList::reverse_iterator it=files.rbegin(); it!=files.rend(); ++it )
            {
                IBlockPtr pBlock = CreateBlock();
                pBlock->AddChunk(L"file", CreateValue( (*it).first ));
                pBlock->AddChunk(L"dir", CreateValue( (*it).second ));
                pPrev = CreateList( pBlock.get(), pPrev.get() );
            }
            return pPrev;
        }

        void write_string_to_file( const std::string &fileName, std::string &stringData, bool useEncryption )
        {
            SPointer< OFile > outFile = new StdOFile( fileName );
            if ( !useEncryption )
            {
                outFile->WriteUpto( stringData.c_str(), stringData.length() );
            }
            else
            {
                std::string &s = stringData;
                AlignForEncryption( s );
                CreateEncryptedOFile( outFile )->Write( s.c_str(), s.length() );
            }
        }

        void USELESS_TK::WriteFile( Node node, ExecutionState &state )
        {
            Attr< std::string > atFile("file");
            Attr< int, false  > atEncr("encrypt");
            GetAttr( atFile, node, state );
            bool useEncryption = ( GetAttr( atEncr, node, state ) && 1 == *atEncr );
            IChunk *pValue = XMLProgram::GetChunk(L"data", state);
            if ( !pValue ) { throw Error("WriteFile: 'data' expected"); }
            XMLValueChunk< std::string > *pData = dynamic_cast< XMLValueChunk< std::string > *>( pValue );
            if ( pData ) {
                write_string_to_file( atFile.str(), pData->_value, useEncryption );
            }
            else {
                write_string_to_file( atFile.str(), value_of< TextUtf8 >( pValue ).as_string(), useEncryption );
            }
            state.SetResult( CreateEmpty() );
        }

        void USELESS_TK::ReadFile( Node node, ExecutionState &state )
        {
            Attr< std::string > atFile("file");
            Attr< int, false  > atEncr("encrypt");
            GetAttr( atFile, node, state );
            SPointer< IFile > inFile = IFS::Instance().OpenFile( atFile.str() );
            if ( GetAttr( atEncr, node, state ) && 1 == *atEncr )
            {
				inFile = CreateEncryptedIFile( inFile );
            }
            size_t size = inFile->GetLen();
            XMLValueChunk< std::string > *pData = new XMLValueChunk< std::string >();
            pData->_value.resize( size );
            inFile->ReadUpto( &pData->_value[0], size );
            state.SetResult( pData );
        }

        std::string to_xml_string( IChunk *pRoot, ExecutionState &state )
        {
            IChunk   *pName = pRoot->GetChunk(L"name");
            if ( !pName ) { throw Error("ToXMLString: 'name' expected"); }
            IChunkPtr pAttr = pRoot->GetChunk(L"attributes");
            if ( !!pAttr ) {
                pAttr = pAttr->GetChunk(L"__symbols__");
            }
            XMLTag tag( value_of< TextUtf8 >( pName ));
            while ( !IsEmpty( pAttr.get() ))
            {
                pAttr = Force( pAttr, state );
                if ( IsEmpty( pAttr.get() )) { break; }
                IChunk *pAttrX = pAttr->GetChunk( FOURCC_LIST_HEAD );
                IChunk *pAttrName = pAttrX->GetChunk( FOURCC_LIST_HEAD );
                IChunk *pAttrValu = pAttrX->GetChunk( FOURCC_LIST_TAIL )->GetChunk( FOURCC_LIST_HEAD );
                tag.setattr( value_of< TextUtf8 >( pAttrName ), value_of< TextUtf8 >( pAttrValu ));
                pAttr = pAttr->GetChunk( FOURCC_LIST_TAIL );
            }
            IChunk   *pValu = pRoot->GetChunk(L"value");
            if ( !IsEmpty( pValu ))
            {
                tag.setvalue( value_of< TextUtf8 >( pValu ));
            }
            else {
                IChunkPtr pChld = pRoot->GetChunk(L"children");
                while ( !IsEmpty( pChld.get() ))
                {
                    pChld = Force( pChld, state );
                    if ( IsEmpty( pChld.get() )) { break; }
                    tag.appendvalue( to_xml_string( pChld->GetChunk( FOURCC_LIST_HEAD ), state ));
                    pChld = pChld->GetChunk( FOURCC_LIST_TAIL );
                }
            }
            return tag.to_string();
        }

        void USELESS_TK::ToXMLString( Node __unused__, ExecutionState &state )
        {
            IChunk *pRoot = XMLProgram::GetChunk(L"root", state);
            if ( !pRoot ) { throw Error("ToXMLString: 'root' expected"); }
            state.SetResult( CreateValue( to_xml_string( pRoot, state ) ));
        }

        void USELESS_TK::FromXMLString( Node __unused__, ExecutionState &state )
        {
            IChunk *pXml = XMLProgram::GetChunk(L"xml", state);
            XMLValueChunk< std::string > *pStr = dynamic_cast< XMLValueChunk< std::string > *>( pXml );
            if ( !pXml ) { throw Error("FromXMLString: string 'xml' expected"); }
            Useless::XMLDocument doc( pStr->_value );
            state.SetResult( XMLNodeToBlock( doc.Root(), state ).get() );
        }




        /*****************************************************************

          Module Useless_Resources

*/

        USELESS_RESOURCES::USELESS_RESOURCES( Node node, ExecutionState &state )
        {
            add_methods( this )
                .def("GetScreen", &USELESS_RESOURCES::GetScreen, "name" )
                .def("GetImage", &USELESS_RESOURCES::GetImage, "name" )
                .def("GetSample", &USELESS_RESOURCES::GetSample, "name" )
                .def("GetFont", &USELESS_RESOURCES::GetFont, "name" )
                .def("GetSkin", &USELESS_RESOURCES::GetSkin, "name" )
                .def("GetWidget", &USELESS_RESOURCES::GetWidget, "name" )
                .def("GetLayout", &USELESS_RESOURCES::GetLayout, "name" )
                .def("GetWorkspace", &USELESS_RESOURCES::GetWorkspace, "name" )
                .def("RaiseEvent", &USELESS_RESOURCES::RaiseEvent, "name", "arg" )
                .def("CreateFromXMLString", &USELESS_RESOURCES::CreateFromXMLString )
                .def("KillResources", &USELESS_RESOURCES::KillResources, "category", "name", "recursive" )
                ;
        }

        IChunkPtr USELESS_RESOURCES::GetScreen( std::string name )
        {
            return new ScreenProxy( *ScreenResource("system", name ));
        }
        
        IChunkPtr USELESS_RESOURCES::GetImage( std::string name )
        {
            return new ImageProxy( *ImageResource("images", name ));
        }

        IChunkPtr USELESS_RESOURCES::GetSample( std::string name )
        {
            return new SampleProxy( *SampleResource("sounds", name ));
        }
        
        IChunkPtr USELESS_RESOURCES::GetFont   ( std::string name )
        {
            return new FontProxy( *FontResource("fonts", name ));
        }
        
        IChunkPtr USELESS_RESOURCES::GetSkin   ( std::string name )
        {
            return new SkinProxy( **SkinResource("skins", name ));
        }

        IChunkPtr USELESS_RESOURCES::GetWidget( std::string name )
        {
            return new WidgetProxy( WidgetResource< Widget >::Query( name ));
        }

        IChunkPtr USELESS_RESOURCES::GetLayout( std::string name )
        {
            return new LayoutProxy( *ResourceQuery< LayoutBase * >("layouts", name ));
        }
            
        IChunkPtr USELESS_RESOURCES::GetWorkspace( std::string name )
        {
            GUIMasterResource resGUI( "system", name );
            return new WorkspaceProxy( &(*resGUI)->GetWorkspace() );
        }
            
        void USELESS_RESOURCES::RaiseEvent( std::string name, std::string arg )
        {
            SignalEventResource resEvt( "events", name );
            Signal sig;
            (*resEvt)->Raise( &sig, arg );
        }

        void USELESS_RESOURCES::CreateFromXMLString( Node __unused__, ExecutionState &state )
        {
            IChunk *pXML = XMLProgram::GetChunk(L"xml", state);
            if ( !pXML ) { throw Error("CreateFromXMLString: 'xml' expected"); }
            XMLValueChunk< std::string > *pString = dynamic_cast< XMLValueChunk< std::string > *>( pXML );
            if ( pString ) {
                CreateFromXML( pString->_value );
            }
            else {
                CreateFromXML( value_of< TextUtf8 >( pXML ).as_string() );
            }
            state.SetResult( CreateEmpty() );
        }
            
        void USELESS_RESOURCES::KillResources( std::string category, std::string name, int recursive )
        {
			Useless::Resources::Instance().Remove( category, name, recursive );
        }
        
        // defined elsewhere
        struct SIMPLE_IPC;


    };//XML_IMPORT

    
    
    
    /*****************************************************************

      Import all Useless stuff

     */

    USELESS_XML_TAG_DICT_DEFINE( xtdImportUseless, ExecutionState );

// not in cryptopp DLL
#ifdef KOOLIB_WITH_SIMPLE_IPC //USELESS_HAS_CRYPTOPP
#   define ADDREG_SIMPLE_IPC() .AddReg< XML_IMPORT::SIMPLE_IPC >("SimpleIPC")
#else
#   define ADDREG_SIMPLE_IPC()
#endif

    
    static USELESS_XML_TAG_DICT_Extend( xtdImportUseless, ExecutionState, BasicProgram,
            .AddReg< XML_IMPORT::USELESS_TK >("Toolkit")
            .AddReg< XML_IMPORT::USELESS_RESOURCES >("Resources")
            ADDREG_SIMPLE_IPC()
            );
    
    static USELESS_XML_TAG_DICT_AddBase( xtdImport, ExecutionState, xtdImportUseless );


};//XMLProgram



namespace XMLFactory {
    using namespace XMLProgram;

#define DEFINE_MODULE_IMPORT( _modName ) USELESS_XML_TAG_HANDLER( _modName, ExecutionState ) \
{\
    _state.SetResult( new _modName( _node, _state ) ); \
}

DEFINE_MODULE_IMPORT( XML_IMPORT::USELESS_TK );
DEFINE_MODULE_IMPORT( XML_IMPORT::USELESS_RESOURCES );

};//XMLFactory
