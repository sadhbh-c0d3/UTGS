#include "Dynamo/classes/CInterfaceProvider.h"

#include "Useless/UselessConfig.h"
#include "Useless/XML/Resources/CreateGUIResources.h"
#include "Useless/Graphic/Planes/TransparentImage.h"
#include "Useless/Graphic/FileIO/ImageFactory.h"
#include "Useless/Graphic/Device/GVM_Context.h"
#include "Useless/File/VIFS.h"


namespace Dynamo {
    using namespace Useless;

    struct CPixelReader : CInterface, virtual IPixelReader
    {
        SPointer< PixelTransfer > m_reader;

        CPixelReader( SPointer< PixelTransfer > reader ): m_reader( reader )
        {
        }
        
        void GetInput ( size_t *width, size_t *height, ColorMode::Constant *mode )
        {
            int w, h, f, c;
            const void *pal;
            m_reader->GetSource( &w, &h, &f, &pal, &c );
            *width = w;
            *height = h;
            switch( f )
            {
                case ImageFormat::SINGLE8:  *mode = ColorMode::GREY; break;
                //case ImageFormat::SINGLE8: *mode = ColorMode::ALPHA; break; //< ambiguos format GREY<->ALPHA?
                case ImageFormat::B8G8R8:   *mode = ColorMode::BGR; break;
                case ImageFormat::B8G8R8A8: *mode = ColorMode::BGRA; break;
                case ImageFormat::R8G8B8:   *mode = ColorMode::RGB; break;
                case ImageFormat::R8G8B8A8: *mode = ColorMode::RGBA; break;
                default:
                    throw Useless::Error("Unknown data format: \"pixel_format = %i\" cannot find appropriate format in Dynamo.CGraphix.", f);
                    break;
            };
        }

        void SetupOutput ( ColorMode::Constant mode, size_t pitch )
        {
            int width; int height; int pf; const void *pal; int nc;
            m_reader->GetSource( &width, &height, &pf, &pal, &nc );
            int newPF = 0;
            switch( mode )
            {
                case ColorMode::GREY:   newPF = ImageFormat::SINGLE8; break;
                case ColorMode::ALPHA:  newPF = ImageFormat::SINGLE8; break;
                case ColorMode::BGR:    newPF = ImageFormat::B8G8R8; break;
                case ColorMode::BGRA:   newPF = ImageFormat::B8G8R8A8; break;
                case ColorMode::RGB:    newPF = ImageFormat::R8G8B8; break;
                case ColorMode::RGBA:   newPF = ImageFormat::R8G8B8A8; break;
                default: break;
            };
            m_reader->SetDestination( width, height, pitch, newPF );
        }

        void ReadPixels  ( void *pointer, Rect4i area )
        {
            m_reader->Store( pointer, reinterpret_cast< Rect& >( area ) );
        }
    };

    struct CPixelWriter : CInterface, virtual IPixelWriter
    {
        SPointer< PixelTransfer > m_writer;

        CPixelWriter( SPointer< PixelTransfer > writer ): m_writer( writer )
        {
        }

        void SetupInput  ( size_t width, size_t height, ColorMode::Constant mode, size_t pitch )
        {
            int newPF = 0;
            switch( mode )
            {
                case ColorMode::GREY:   newPF = ImageFormat::SINGLE8; break;
                case ColorMode::ALPHA:  newPF = ImageFormat::SINGLE8; break;
                case ColorMode::BGR:    newPF = ImageFormat::B8G8R8; break;
                case ColorMode::BGRA:   newPF = ImageFormat::B8G8R8A8; break;
                case ColorMode::RGB:    newPF = ImageFormat::R8G8B8; break;
                case ColorMode::RGBA:   newPF = ImageFormat::R8G8B8A8; break;
                default: break;
            };
            m_writer->SetSource( width, height, pitch, newPF );
        }

        void WritePixels ( const void *pointer, Rect4i area )
        {
            m_writer->Fetch( pointer, reinterpret_cast< Rect& >( area ) );
        }
    };

    struct CPixelWriterWrapper : PixelTransfer
    {
        Hand< IPixelWriter > m_writer;
        ColorMode::Constant  m_mode;
        size_t               m_pitch;
        void                *m_source;
        size_t               m_width;
        size_t               m_height;

        CPixelWriterWrapper( IPixelWriter *writer ): m_writer( writer ), m_source(0)
        {
        }

        //! Obtain transfer type information (combination of flags)
        virtual int  Info()
        {
            return PixelTransfer::TRANSFORM + PixelTransfer::SOURCE;
        }

        //! Fetch data from source
        virtual void Fetch( const void *source, const Rect &area )
        {
            m_writer->WritePixels( source, reinterpret_cast< const Rect4i &>( area ));
        }

        //! Store data into destination
        virtual void Store( void *destination, const Rect &area )
        {
            assert(!"WriterCannotStore");
        }

        //! Transform data from input buffer to destination buffer
        virtual void Transform()
        {
            if ( 0 != m_source )
            {
                Fetch( m_source, Rect(0,0, m_width, m_height));
            }
        }

        //! Set source pixel-stream type
        virtual void SetSource( int  width,
                int  height,
                int  pitch,
                int  pixel_format, //!< use ImageFormat::Format or BPP
                const void *palette = 0,
                int  colors = 0)
        {
            m_width = width;
            m_height = height;
            m_pitch = pitch;

            switch( pixel_format )
            {
                case ImageFormat::SINGLE8:  m_mode = ColorMode::GREY; break;
                //case ImageFormat::SINGLE8: m_mode = ColorMode::ALPHA; break;
                case ImageFormat::B8G8R8:   m_mode = ColorMode::BGR; break;
                case ImageFormat::B8G8R8A8: m_mode = ColorMode::BGRA; break;
                case ImageFormat::R8G8B8:   m_mode = ColorMode::RGB; break;
                case ImageFormat::R8G8B8A8: m_mode = ColorMode::RGBA; break;
                default:
                    throw Useless::Error("Unsupported data format: \"pixel_format = %i\" is not supported by Dynamo.CGraphix.", pixel_format);
                    break;
            };
            if ( width < 0 )
            {
                throw Useless::Error("Unsupported data format: \"width < 0\" is not supported by Dynamo.CGraphix.");
            }
            if ( height < 0 )
            {
                throw Useless::Error("Unsupported data format: \"height < 0\" is not supported by Dynamo.CGraphix.");
            }
            if ( pitch < 0 )
            {
                throw Useless::Error("Unsupported data format: \"pitch < 0\" is not supported by Dynamo.CGraphix.");
            }
            if ( palette != 0 )
            {
                throw Useless::Error("Unsupported data format: \"palette != 0\" is not supported by Dynamo.CGraphix.");
            }
            m_writer->SetupInput( m_width, m_height, m_mode, pitch );
        }

        //! Set destination pixel-stream type
        virtual void SetDestination( int  width,
                int  height,
                int  pitch,
                int  pixel_format, //!< use ImageFormat::Format or BPP
                void *palette = 0,
                int  colors = 0)
        {
            assert(!"WriterCannotSetDestination");
        }

        //! Get source pixel-stream type
        virtual void GetSource( int  *width,
                int  *height,
                int  *pixel_format,
                const void **palette,
                int  *colors )
        {
            assert(!"WriterGetSourceNotImplemented");
        }

        //! Get destination pixel-stream type
        virtual void GetDestination( int  *width,
                int  *height,
                int  *pixel_format,
                void **palette,
                int  *colors )
        {
            assert(!"WriterGetSourceNotImplemented");
        }

        //! Set source buffer pointer. If needed use only before SetSource().
        virtual void SetSourcePointer( void *source)
        {
            m_source = source;
        }

        //! Set destination area pointer. If needed use only before SetDestination().
        virtual void SetDestinaitonPointer( void *destination)
        {
            assert(!"WriterCannotSetDestinationPointer");
        }
    };

    struct CInputGraphicPlane : CInterface, virtual IGraphicPlane
    {
        SPointer< IGraphics > m_plane;

        CInputGraphicPlane( SPointer< IGraphics > plane ): m_plane( plane )
        {
        }

        IGraphicPlane * Copy( Rect4i area )
        {
            return new CInputGraphicPlane( m_plane->Copy( reinterpret_cast< Rect& >( area ) ));
        }

        IGraphicPlane * Copy( Rect4i area, size_t width, size_t height )
        {
            SPointer< IGraphics > plane = m_plane->Copy( reinterpret_cast< Rect& >( area ) );
            if ( ImageBase *bplane = dynamic_cast< ImageBase *>( plane.get() ))
            {
                bplane->SetDestWidth( width );
                bplane->SetDestHeight( height );
            }
            else
            {
                assert( !"WarningCannotSetNewSize" );
            }
            return new CInputGraphicPlane( plane );
        }

        size_t GetWidth()
        {
            return m_plane->GetWidth();
        }

        size_t GetHeight()
        {
            return m_plane->GetHeight();
        }

        size_t GetDestWidth()
        {
            return m_plane->GetDestWidth();
        }

        size_t GetDestHeight()
        {
            return m_plane->GetDestHeight();
        }

        float  GetFrameRate()
        {
            return m_plane->GetFrameRate();
        }

        ColorMode::Constant GetColorMode()
        {
            //@TODO: no easy way to detect pixel format
            return ColorMode::RGBA;
        }

        IPixelWriter * ProvideIPixelWriter()
        {
            Surface *surf = const_cast< Surface *>( m_plane->GetSurface());
            if ( !surf )
            {
                return 0;
            }
            return new CPixelWriter( surf->CreateWriter( Surface::COLOR + Surface::ALPHA ) );
        }

        IPixelReader * ProvideIPixelReader()
        {
            Surface *surf = const_cast< Surface *>( m_plane->GetSurface());
            if ( !surf )
            {
                return 0;
            }
            return new CPixelReader( surf->CreateReader( Surface::COLOR + Surface::ALPHA ) );
        }
    };

    struct CTextureWritter
    {
    };
    
    struct CGraphix : CInterface, virtual IGraphix
    {

        CGraphix()
        {
        }

        ~CGraphix()
        {
        }

        IGraphicPlane * GetImage( std::string imageId )
        {
            return new CInputGraphicPlane( *ImageResource("images", imageId ));
        }

        IGraphicPlane * GetScreen( std::string screenId )
        {
            return 0;
        }

        IGraphicPlane * CreateImage( std::string fileName )
        {
            return new CInputGraphicPlane( new Image( fileName.c_str() ));
        }

        IGraphicPlane * CreateTransparentImage( std::string fileName, std::string maskName )
        {
            return new CInputGraphicPlane( new TransparentImage( fileName.c_str(), maskName.c_str() ));
        }

        void LoadPixels( std::string fileName, IPixelWriter *writer )
        {
            int width, height, pf;
            SPointer< IFile > file = IFS::Instance().OpenFile( fileName );
            SPointer< ImageLoader > loader = ImageFactory::Create( *file, &width, &height, &pf );
            CPixelWriterWrapper wrapper( writer );
            loader->Load( *file, wrapper );
            wrapper.Transform();
        }
        
        GVM_Context * GetGVMContext( std::string screenId )
        {
            SPointer< Screen > screen = *ScreenResource("system", screenId );
            GVM_Context *gvmContext = dynamic_cast< GVM::Context *>( screen->GetSurface() );
            return gvmContext;
        }
    };

    IGraphix * CInterfaceProvider::ProvideIGraphix()
    {
        return new CGraphix();
    }
};
