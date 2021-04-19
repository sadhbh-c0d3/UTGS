#include "Dynamo/classes/CInterfaceProvider.h"

#include "Useless/UselessConfig.h"

#include "Useless/XML/Resources/CreateGUIResources.h"

// used by CActiveWidget hooks
#include "Useless/XML/Resources/SignalMap.h"

// used by Paint
#include "Useless/Graphic/Device/SurfaceExtFun.h"
#include <GL/gl.h>

// used by CTextWidget
#include "Useless/Util/Storage/MemBlock.h"
#ifndef WIN32
#   include <stdio.h>
#   include <stdarg.h>
#   define _vsnprintf vsnprintf
#   define _vsnwprintf vswprintf
#endif

namespace Dynamo {
    
    struct AdvancedPaint : CInterface
    {
        AdvancedPaint( Useless::Widget *widget )
        {
            using namespace Useless;
            _pScreenSurface = widget->GetEnvironment()->GetScreen().GetSurface();
            _BlendFun = (SurfExt::PfBlendFun)(_pScreenSurface->GetExtFun("BlendFun"));
            _ConstColor = (SurfExt::PfConstColor)(_pScreenSurface->GetExtFun("ConstColor"));
            _ConstMatrix = (SurfExt::PfConstMatrix)(_pScreenSurface->GetExtFun("ConstMatrix"));
        }

        void BlendFun( int a, int b, bool e ) { _BlendFun( _pScreenSurface, a, b, e); }
        void ConstColor( int r, int g, int b ) { _ConstColor( _pScreenSurface, r, g, b ); }
        void ConstMatrix( float *m ) { _ConstMatrix( _pScreenSurface, m ); }

        Useless::Surface *               _pScreenSurface;
        Useless::SurfExt::PfBlendFun     _BlendFun;
        Useless::SurfExt::PfConstColor   _ConstColor;
        Useless::SurfExt::PfConstMatrix  _ConstMatrix;
    };

    struct CPaint : CInterface, virtual IPaint
    {
        Useless::WidgetPainter _painter;
        Hand< AdvancedPaint > _advPaint;

        CPaint()
        {
        }

        CPaint( const Useless::WidgetPainter &painter ): _painter( painter )
        {
        }

        ~CPaint()
        {
            if ( _advPaint.get() )
            {
                _advPaint->BlendFun( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, true );
                _advPaint->ConstColor( 255, 255, 255 );
            }
        }

        void BlitImage( std::string imageId,
                Rect4i apart,
                Rect4i awhere )
        {
            Useless::SPointer< Useless::ImageBase > img = Useless::MakeImage( imageId );
            Useless::Rect & where = reinterpret_cast< Useless::Rect &>( awhere );
            Useless::Rect & part = reinterpret_cast< Useless::Rect &>( apart );

            int x=0, y=0, w=0, h=0;

            if ( !!where )
            {
                x = where.x;
                y = where.y;
                if ( 0 != where.w || 0 != where.h )
                {
                    img->SetDestWidth( where.w );
                    img->SetDestHeight( where.h );
                }
            }

            if ( !!part )
            {
                Useless::SPointer< Useless::IGraphics > cp = img->Copy( part );
                img.Reset( dynamic_cast< Useless::ImageBase *>( cp.Release() ));
            }

            _painter.ProjectImage( x, y, *img );
        }

        void MultiBlit( std::string imageId,
                std::vector< Point2i > positions,
                std::vector< Rect4i > rects )
        {
            Useless::SPointer< Useless::ImageBase > image = Useless::MakeImage( imageId );

            _painter.ProjectImages( *image, reinterpret_cast< std::vector< Useless::Pos > &>( positions ),
                    reinterpret_cast< std::vector< Useless::Rect > &>( rects ));
        }

        void FillRect( Rect4i where, int color )
        {
            _painter.PaintRectangle( color, reinterpret_cast< Useless::Rect &>( where ));
        }

        void DrawRect( Rect4i where, int color )
        {
            _painter.PaintBoundary( reinterpret_cast< Useless::Rect &>( where ), color, color, color, color );
        }

        void DrawLineSegment(
                Point2i from,
                Point2i to,
                int thick,
                int color )
        {
            _painter.PaintLine( reinterpret_cast< Useless::Pos &>( from ), reinterpret_cast< Useless::Pos &>( to ),
                    color, thick );
        }

        void DrawLineSegments( std::vector< Point2i > &sectors, int thick, int color )
        {
            _painter.PaintLine( reinterpret_cast< std::vector< Useless::Pos > &>( sectors ), color, thick );
        }

        IPaint * CreateNew( Point2i offset )
        {
            return new CPaint( Useless::WidgetPainter( _painter, reinterpret_cast< Useless::Pos &>( offset )));
        }

        void Crop( Rect4i area )
        {
            _painter.Intersect( reinterpret_cast< Useless::Rect &>( area ));
        }

        void SetBlendMode( int mode )
        {
            InitAdvPaint();
            if ( NORMAL == mode )
            {
                _advPaint->BlendFun( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, true );
            }
            else if ( REPLACE == mode )
            {
                _advPaint->BlendFun( GL_ONE, GL_ZERO, true );
            }
            else if ( ADD == mode ) // add RGBA
            {
                _advPaint->BlendFun( GL_SRC_ALPHA, GL_ONE, true );
            }
            else if ( MULTIPLY == mode ) // multiply RGBA
            {
                _advPaint->BlendFun( GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA, true );
            }
            else if ( SHADOW == mode )
            {
                _advPaint->BlendFun( GL_ZERO, GL_ONE_MINUS_SRC_ALPHA, true );
            }
        }

        void MultiplyAlpha( int value )
        {
            _painter.MultiplyAlpha( value );
        }

        void SetColorFilter( unsigned int c )
        {
            InitAdvPaint();
            Useless::NormalPixel color( c );
            _advPaint->ConstColor( color.c.r, color.c.g, color.c.b );
        }

        void InitAdvPaint()
        {
            if ( 0 == _advPaint.get() )
            {
                _advPaint = new AdvancedPaint( _painter.GetWidget() );
            }
        }
        
        GVM_Context * GetGVMContext()
        {
            return _painter.GetGVMContext();
        }

        void SetFrustum    ( double left, double right, double bottom, double top, double znear, double zfar )
        {
            _painter.SetFrustum( left, right, bottom, top, znear, zfar );
        }

        void SetOrtho      ( double left, double right, double bottom, double top, double znear, double zfar )
        {
            _painter.SetOrtho( left, right, bottom, top, znear, zfar );
        }

        void SetOrtho2D    ( double left, double right, double bottom, double top )
        {
            _painter.SetOrtho2D( left, right, bottom, top );
        }

        void SetPerspective( double fov, double aspect, double znear, double zfar )
        {
            _painter.SetPerspective( fov, aspect, znear, zfar );
        }

        void Invoke        ( const GVM_Message &msg )
        {
            _painter.Invoke( msg );
        }
    };

    struct CWidget : CInterface, virtual IWidget
    {
        std::string             _rName;
        Useless::Widget           *_prWidget;
        Useless::CompoundWidget   *_prCompound;
        Useless::ActiveAreaWidget *_prArea;
        Useless::Signal::FuncID    _sidPaint, _sidResize;
        Hand< IHook_Paint >     _phPaint;
        Hand< IHook_Size  >     _phResize;

        CWidget() : _prWidget(0), _prArea(0), _prCompound(0)
        {}

        CWidget( Useless::Widget *prWidget, const std::string &rName ): _prWidget( prWidget ), _rName( rName )
        {
            _prCompound = dynamic_cast< Useless::CompoundWidget *>( _prWidget );
            _prArea = dynamic_cast< Useless::ActiveAreaWidget *>( _prWidget );
        }

        ~CWidget()
        {
            _sidPaint.Untie();
            _sidResize.Untie();
        }

        Point2i GetPos()
        {
            Useless::Pos pos = _prWidget->GetPosition();
            return reinterpret_cast< Point2i & >( pos );
        }
        
        IWidget * GetParent()
        {
            return new CWidget( _prWidget->GetParent(), std::string("parent::") + _rName );
        }
        
        void SetPos( Point2i pos )
        {
			_prWidget->SetPosition( reinterpret_cast< Useless::Pos & >( pos ));
        }
        
        Rect4i GetRect()
        {
            Useless::Rect rect = _prWidget->GetBoundingRect();
            return reinterpret_cast< Rect4i & >( rect );
        }

        void SetRect( Rect4i area )
        {
            _prWidget->SetBoundingRect( reinterpret_cast< Useless::Rect & >( area ));
        }
        
        Rect4i GetAbsoluteRect( Rect4i area )
        {
            return reinterpret_cast< Rect4i & >( _prWidget->GetAbsoluteRect( reinterpret_cast< Useless::Rect & >( area )));
        }
        
        Rect4i GetRelativeRect( Rect4i area )
        {
            return reinterpret_cast< Rect4i & >( _prWidget->GetRelativeRect( reinterpret_cast< Useless::Rect & >( area )));
        }

        void SetDirty( Rect4i area )
        {
            _prWidget->SetDirty( reinterpret_cast< Useless::Rect & >( area ));
        }

        void SetOpacity( int opacity )
        {
            _prWidget->SetOpacity( opacity );
        }

        int  GetOpacity()
        {
            return _prWidget->GetOpacity();
        }

        void SetPhantom( bool phantom )
        {
            _prWidget->SetPhantom( phantom );
        }

        bool GetPhantom()
        {
            return _prWidget->IsPhantom();
        }

        void SetVisible( bool visible )
        {
            _prWidget->SetVisible( visible );
        }

        bool GetVisible()
        {
            return _prWidget->IsVisible();
        }

        bool IsReallyVisible()
        {
            return _prWidget->IsRealyVisible();
        }

        void CaptureKeyboard()
        {
            _prWidget->FocusIntercept();
        }

        void ReleaseKeyboard()
        {
            _prWidget->FocusRelease();
        }

        void CapturePointer()
        {
            _prWidget->InputIntercept();
        }

        void ReleasePointer()
        {
            _prWidget->InputRelease();
        }

        void IncEagerToRecievePointerEvents()
        {
            _prWidget->InputIncEager();
        }

        void DecEagerToRecievePointerEvents()
        {
            _prWidget->InputDecEager();
        }

        void Paint( IPaint *paint )
        {
            if ( CPaint *cpaint = dynamic_cast< CPaint *>( paint ))
            {
                if ( _prCompound )
                {
                    _prCompound->DoRepaint( cpaint->_painter );
                }
            }
        }

        void SetHook_Paint( IHook_Paint *hook )
        {
            if ( !_phPaint.get() )
            {
                if ( _prArea ) { 
                    _sidPaint = Useless::Tie2Signal( _prArea->OnPaint, this, &CWidget::Repaint );
                }
                if ( _prCompound ) {
                    _sidPaint = Useless::Tie2Signal( _prCompound->OnPaint, this, &CWidget::Repaint );
                }
            }
            _phPaint = hook;
        }
        
        void SetHook_Resize( IHook_Size *hook )
        {
            if ( !_phResize.get() )
            {
                Useless::Tie2Signal(_prWidget->OnResize, hook, &IHook_Size::Apply );
            }
            _phResize = hook;
        }

        void Repaint( const Useless::WidgetPainter &painter )
        {
            Hand< IPaint > paint( new CPaint( painter ));
            _phPaint.get()->Apply( paint.get() );
        }
    };

    struct CActiveWidget : virtual CWidget, virtual IActiveWidget
    {
        Useless::ActiveWidget *_prActive;
        std::map< std::string, Hand< IHook_Pointer > > _phPointer;
        std::map< std::string, Useless::Signal::FuncID > _sidPointer;
        Hand< IHook_Key > _phKey;
        Useless::Signal::FuncID _sidKey;

        CActiveWidget(): _prActive(0)
        {
        }

        CActiveWidget( Useless::ActiveWidget *prActive, const std::string &rName )
            : CWidget( prActive, rName ), _prActive( prActive )
        {
        }

        ~CActiveWidget()
        {
            for( std::map< std::string, Useless::Signal::FuncID >::iterator iter = _sidPointer.begin();
                    iter != _sidPointer.end(); ++iter )
            {
                (*iter).second.Untie();
            }
            _sidKey.Untie();
        }

        void SetEnabled( bool enabled )
        {
            _prActive->SetActive( enabled );
        }
        
        bool GetEnabled()
        {
            return _prActive->IsActive();
        }

        void SetKeyEnabled( int key, bool enabled )
        {
            _prActive->EnableKey( key, enabled );
            _prActive->EnableOnPathKey( key, enabled );
        }

        bool IsKeyPressed( int key )
        {
            return _prActive->IsKeyPressed( key );
        }

        Point2i GetTouchPoint( int buttonNo )
        {
            return reinterpret_cast< Point2i &>( _prActive->GetTouchPos( buttonNo ) );
        }

        void SetHook_Pointer( std::string eventName, IHook_Pointer *hook )
        {
            std::map< std::string, Useless::Signal::FuncID >::iterator found = _sidPointer.find( eventName );
            if ( _sidPointer.end() != found )
            {
                _sidPointer.erase( found );
            }
            Useless::Signal *pSignal = &Useless::SignalMap::Instance().GetSignalByName( eventName, _prActive );
            typedef Useless::Signal_1< Useless::Pos > PointerSignal;
            if ( PointerSignal *pPointer = dynamic_cast< PointerSignal *>( pSignal ))
            {
                _sidPointer[ eventName ] = pPointer->TieUnary( hook
                        , reinterpret_cast< void (IHook_Pointer::*)( Useless::Pos ) >( &IHook_Pointer::Apply )
                        , pPointer->GetVar1() );

                _phPointer[ eventName ] = hook;
            }
            else {
                assert(0);
            }
        }

        void SetHook_Key( IHook_Key *hook )
        {
            if ( _phKey.get() )
            {
                _sidKey.Untie();
            }
            _sidKey = Useless::Tie2Signal( _prActive->OnKeyPress, hook, &IHook_Key::Apply );
            _phKey = hook;
        }
    };
    
    struct CEnumWidget : virtual CWidget, virtual IEnumWidget
    {
        Useless::EnumWidget *_prEnum;
        Useless::Signal::FuncID _sidChange;
        Hand< IHook_Number > _phSelect;
        
        CEnumWidget(): _prEnum(0)
        {
        }

        CEnumWidget( Useless::EnumWidget *prWidget, const std::string &rName ): CWidget( prWidget, rName ), _prEnum( prWidget )
        {
        }
        
        void SetSelected( int item )
        {
            assert( 0 != _prEnum );
            _prEnum->SetSelected( item );
        }

        int  GetSelected()
        {
            assert( 0 != _prEnum );
            return _prEnum->GetSelected();
        }

        void SetHook_Select( IHook_Number *hook )
        {
            assert( 0 != _prEnum );
            if ( _phSelect.get() )
            {
                _sidChange.Untie();
            }
            _sidChange = Useless::Tie2Signal( _prEnum->OnChange, hook, &IHook_Number::Apply );
            _phSelect = hook;
        }
    };
    
    struct CTextWidget : virtual CWidget, virtual ITextWidget
    {
        Useless::BaseTextWidget *_prTextWidget;
        Useless::AdvTextBase    *_prAdvTextWidget;
        Useless::TextField      *_prTextField;

        CTextWidget(): _prTextWidget(0), _prAdvTextWidget(0), _prTextField(0)
        {
        }

        CTextWidget( Useless::Widget *prWidget, const std::string &rName ): CWidget( prWidget, rName )
            , _prTextWidget(0), _prAdvTextWidget(0), _prTextField(0)
        {
            _prTextWidget = dynamic_cast< Useless::BaseTextWidget *>( prWidget );
            _prAdvTextWidget = dynamic_cast< Useless::AdvTextBase *>( prWidget );
            _prTextField = dynamic_cast< Useless::TextField *>( prWidget );
        }

        void SetText( std::basic_string< wchar_t > text )
        {
            _SetText( Useless::MakeText( text ));
        }

        void _SetText( std::basic_string< wchar_t > text )
        {
            if ( _prTextWidget ) {
                _prTextWidget->SetText( text );
            }
            else if ( _prAdvTextWidget ) {
                _prAdvTextWidget->SetText( text );
            }
            else if ( _prTextField ) {
                _prTextField->SetText( text );
            }
        }

        void SetTextF( wchar_t *fmtStr, ... )
        {
            Useless::Text localized = Useless::MakeText( std::basic_string< wchar_t >( fmtStr ));
            const wchar_t *pshort = localized.c_str();
            using namespace Useless;
#       ifdef WIN32
            va_list args;
            va_start( args, fmtStr );
            int size = wcslen( pshort ) * 2;
            int n = -1;
            MemBlock mem;
            wchar_t *pData;

            while ( n == -1 )
            {
                mem.Alloc( (size + 1) * sizeof(wchar_t) );
                mem.Clear();
                pData = (wchar_t*)mem.GetPtr();
                n = _vsnwprintf( pData, size, pshort, args );
                size <<= 1;
            }
            _SetText( pData );
#       else
            assert(0);
#       endif
        }

        std::basic_string< wchar_t > GetText()
        {
            if ( _prTextWidget ) {
                return _prTextWidget->GetText();
            }
            else if ( _prAdvTextWidget ) {
                return _prAdvTextWidget->GetText();
            }
            else if ( _prTextField ) {
                return _prTextField->GetText();
            }
            else {
                return L"";
            }
        }

        void SetText( const char *localeName, std::basic_string< char > text )
        {
            std::basic_string< wchar_t > str;
            Useless::Read( str, text, Useless::LangSelector::Instance()[ localeName ] );
            _SetText( str );
        }

        void SetTextF( const char *localeName, char *pchar, ... )
        {
            using namespace Useless;
            va_list args;
            va_start( args, pchar );
            int size = strlen( pchar ) * 2;
            int n = -1;
            MemBlock mem;

            while ( n == -1 )
            {
                mem.Alloc( (size + 1) * sizeof(char) );
                mem.Clear();
                n = _vsnprintf( (char*)mem.GetPtr(), size, pchar, args );
                size <<= 1;
            } 
            SetText( localeName, (char*)mem.GetPtr());
        }

        std::basic_string< char > GetText( const char *localeName )
        {
            std::basic_string< wchar_t > text = GetText();
            std::basic_string< char > str;
            Useless::Write( str, text, Useless::LangSelector::Instance()[ localeName ] );
            return str;
        }
    };

    struct CCharInputWidget : CTextWidget, CActiveWidget, virtual ICharInputWidget
    { 
        Useless::TextEditLogic *_prTextEdit;
        Hand< IHook_NewText > _phNewText;
        Useless::Signal::FuncID _sidNewText;

        struct CCharValidator : Useless::CharValidator
        {
            Hand< IHook_CharValidate > _phCharValidate;

            CCharValidator( Hand< IHook_CharValidate > hook ): _phCharValidate( hook )
            {
            }

            bool Validate( int &ch ) const
            {
                int result = _phCharValidate.get_const()->Apply( ch );
                if ( result > 0 ) {
                    ch = result;
                }
                return (result > 0);
            }
        };

        CCharInputWidget(): _prTextEdit(0)
        {
        }

        CCharInputWidget( Useless::TextEditLogic *prTextEdit, const std::string &rName ): CWidget( prTextEdit, rName )
            , _prTextEdit( prTextEdit )
        {
        }

        ~CCharInputWidget()
        {
            if ( !!_prTextEdit->GetCharValidator() )
            {
                _prTextEdit->SetCharValidator( 0 );
            }
        }

        void SetHook_CharValidate( IHook_CharValidate *hook )
        {
            _prTextEdit->SetCharValidator( new CCharValidator( hook ));
        }

        void SetHook_NewText( IHook_NewText *hook )
        {
            if ( _phNewText.get() )
            {
                _sidNewText.Untie();
            }
            _sidNewText = Useless::Tie2Signal( _prTextEdit->OnTextChange, hook, &IHook_NewText::Apply );
        }
    };

    struct CImageWidget : CWidget, virtual IImageWidget
    {
        Useless::ImageWidget *_prImage;

        CImageWidget(): _prImage(0)
        {
        }

        CImageWidget( Useless::ImageWidget *prImage, const std::string &rName ): CWidget( prImage, rName )
            , _prImage( prImage )
        {
        }

        ~CImageWidget()
        {
        }

        void SetImage( std::string imageId )
        {
            _prImage->SetSkin( *Useless::MakeImage( imageId ));
        }

        void SetSkin( std::string skinId )
        {
            _prImage->SetSkin( **Useless::SkinResource("skins", skinId ));
        }
    };

    struct CGui : CInterface, virtual IGui
    {
        Useless::GUIMaster *_prGui;

        CGui(): _prGui(0)
        {
        }

        CGui( Useless::GUIMaster *prGui ): _prGui( prGui )
        {
        }

        ~CGui()
        {
        }


        Point2i GetPointerPosition()
        {
            return reinterpret_cast< Point2i &>( _prGui->GetWorkspace().WhereIsPointer() );
        }

        IWidget *GetWidget( std::string widgetId )
        {
            return new CWidget( Useless::WidgetResource< Useless::Widget >::Query( widgetId ), widgetId );
        }

        IActiveWidget * GetActiveWidget( std::string widgetId )
        {
            return new CActiveWidget( Useless::WidgetResource< Useless::ActiveWidget >::Query( widgetId ), widgetId );
        }
        
        IEnumWidget * GetEnumWidget( std::string widgetId )
        {
            return new CEnumWidget( Useless::WidgetResource< Useless::EnumWidget >::Query( widgetId ), widgetId );
        }

        ITextWidget * GetTextWidget( std::string widgetId )
        {
            return new CTextWidget( Useless::WidgetResource< Useless::Widget >::Query( widgetId ), widgetId );
        }

        ICharInputWidget * GetCharInputWidget( std::string widgetId )
        {
            return new CCharInputWidget( Useless::WidgetResource< Useless::TextField >::Query( widgetId ), widgetId );
        }

        IImageWidget * GetImageWidget( std::string widgetId )
        {
            return new CImageWidget( Useless::WidgetResource< Useless::ImageWidget >::Query( widgetId ), widgetId );
        }

        IBoardWidget *GetBoardWidget( std::string widgetId )
        {
            assert(0);
            return 0;
        }

        INumberDisplay *MakeNumberDisplay( std::string widgetId )
        {
            assert(0);
            return 0;
        }

        ITimerDisplay *MakeTimerDisplay( std::string widgetId )
        {
            assert(0);
            return 0;
        }

        IItemsOfDisplay *MakeItemsOfDisplay( std::string widgetId )
        {
            assert(0);
            return 0;
        }
    };

    IGui * CInterfaceProvider::ProvideIGui()
    {
        return new CGui( Useless::GUIMasterResource("system", "gui")->get() );
    }
    
    IPaint *CreateIPaint( const Useless::WidgetPainter &wpaint )
    {
        return new CPaint( wpaint );
    }

};// Dynamo
