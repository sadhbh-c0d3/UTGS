#include <koolib/xml/XMLKompiler.h> //used for faster symbol access
#include <koolib/xml/XMLChunksUseless.h>
#include <gl/GL.h> //@TODO: needed for PainterProxy::SubCanvasPaint
#include "Useless/Graphic/Planes/MemSurface.h" //needed by QueryPixelColor
#include "Useless/Graphic/Planes/ImageToFile.h" //needed by WriteImage

namespace XMLProgram {

    using namespace Useless;
    using namespace XMLFactory;
    using CXML::LazyGetChunkInScope;
    using CXML::LazyGetChunkMember;


    /*******************************

      PointListProxy

    */
    PointListProxy::PointListProxy( const Useless::PointList &points ): _points( points )
    {
        add_methods( this )
            .def("AddPoint", &PointListProxy::AddPoint, "pos")
            .def("SetPoint", &PointListProxy::SetPoint, "index", "pos")
            .def("Size", &PointListProxy::Size)
            .def("Empty", &PointListProxy::Empty)
            .def("At", &PointListProxy::At, "index")
            ;
    }

    PointListProxy::~PointListProxy()
    {
    }
        
    void PointListProxy::AddPoint( const Useless::Pos &pos )
    {
        _points.push_back( pos );
    }
        
    void PointListProxy::SetPoint( int index, const Useless::Pos &pos )
    {
        if ( index < _points.size() )
        {
            _points[ index ] = pos;
        }
        else
        {
            throw Error("At: index exceeds array");
        }
    }
    
    int  PointListProxy::Size()
    {
        return _points.size();
    }

    int PointListProxy::Empty()
    {
        return _points.empty();
    }
    
    Useless::Pos PointListProxy::At( int index )
    {
        if ( index < 0 ) { throw Error("At: invalid index"); }
        if ( index >= _points.size() ) { throw Error("At: index exceeds array"); }
        return _points[ index ];
    }


    /*******************************

      RectListProxy

    */
    RectListProxy::RectListProxy( const Useless::RectList &rects ): _rects( rects )
    {
        add_methods( this )
            .def("AddRect", &RectListProxy::AddRect, "rect")
            .def("SetRect", &RectListProxy::SetRect, "index", "rect")
            .def("Size", &RectListProxy::Size)
            .def("Empty", &RectListProxy::Empty)
            .def("At", &RectListProxy::At, "index")
            .def("Intersect", &RectListProxy::Intersect, "region")
            ;
    }

    RectListProxy::~RectListProxy()
    {
    }
        
    void RectListProxy::AddRect( const Useless::Rect &rect )
    {
        _rects.push_back( rect );
    }
    
    void RectListProxy::SetRect( int index, const Useless::Rect &rect )
    {
        if ( index < _rects.size() )
        {
            _rects[ index ] = rect;
        }
        else
        {
            throw Error("At: index exceeds array");
        }
    }
    
    int  RectListProxy::Size()
    {
        return _rects.size();
    }

    int RectListProxy::Empty()
    {
        return _rects.empty();
    }
    
    Useless::Rect RectListProxy::At( int index )
    {
        if ( index < 0 ) { throw Error("At: invalid index"); }
        if ( index >= _rects.size() ) { throw Error("At: index exceeds array"); }
        return _rects[ index ];
    }
        
    IChunkPtr RectListProxy::Intersect( const Useless::RectList &region )
    {
        RectListProxy *pOut = new RectListProxy( RectList() );
        IChunkPtr pResult = pOut;
        for ( int i=0, I=region.size(); i<I; ++i )
        {
            for ( int j=0, J=_rects.size(); j<J; ++j )
            {
                pOut->_rects.push_back( _rects[j] & region[i] );
            }
        }
        return pOut;
    }


    /*******************************

      ScreenProxy

    */
    ScreenProxy::ScreenProxy( SPointer< Screen > screen ): _spScreen( screen )
    {
        add_methods( _spScreen.get(), this )
            .def("Refresh", &Screen::Refresh )
            .def("Show", &Screen::Show )
            .def("Hide", &Screen::Hide )
            .def("Maximize", &Screen::Maximize )
            .def("Minimize", &Screen::Minimize )
            .def("Restore", &Screen::Restore )
            .def("ShowCursor", &Screen::ShowCursor )
            .def("HideCursor", &Screen::HideCursor )
            .def("GrabFocus", &Screen::GrabFocus )
            .def("InterceptCursor", &Screen::InterceptCursor )
            .def("ReleaseCursor", &Screen::ReleaseCursor )
            .def("SetCursorPos", &Screen::SetCursorPos, "x", "y" )
            .def("GetCursorPos", unconst_method_cast( &Screen::GetCursorPos ))
            .def("Reposition", &Screen::Reposition, "x", "y" )
            .def("Resize", &Screen::Resize, "w", "h" )
            .def("ShowDecorations", &Screen::ShowDecorations )
            .def("HideDecorations", &Screen::HideDecorations )
            .def("GetWindowRect", unconst_method_cast( &Screen::GetWindowRect ))
            .def("GetClientRect", unconst_method_cast( &Screen::GetClientRect ))
            .def("AllowSizing", &Screen::AllowSizing, "allow" )
            .def("NoSizing", unconst_method_cast( &Screen::NoSizing ))
            .def("AllowClear", &Screen::AllowClear, "allow" )
            .def("NoClear", unconst_method_cast( &Screen::NoClear ))
            .def("AllowCursorWrapping", &Screen::AllowCursorWrapping, "allow" )
            .def("NoCursorWrapping", unconst_method_cast( &Screen::NoCursorWrapping ))
            .def("GetTitle", unconst_method_cast( &Screen::GetTitle ))
            .def("GetWidth", unconst_method_cast( &Screen::GetWidth ))
            .def("GetHeight", unconst_method_cast( &Screen::GetHeight ))
            .def("Swap", &Screen::Swap )
            // signals
            .add("OnCreate", make_signal_0_chunk( _spScreen->OnCreate ))
            .add("OnDestroy", make_signal_0_chunk( _spScreen->OnDestroy ))
            .add("OnActivate", make_signal_0_chunk( _spScreen->OnActivate ))
            .add("OnDeactivate", make_signal_0_chunk( _spScreen->OnDeactivate ))
            .add("OnPaint", make_signal_0_chunk( _spScreen->OnPaint ))
            .add("OnMove", make_signal_2_chunk( _spScreen->OnMove ))
            .add("OnResize", make_signal_2_chunk( _spScreen->OnResize ))
            .add("OnIdle", make_signal_2_chunk( _spScreen->OnIdle ))
            .add("OnKeyPress", make_signal_1_chunk( _spScreen->OnKeyPress ))
            .add("OnKeyRelease", make_signal_1_chunk( _spScreen->OnKeyRelease ))
            .add("OnChar", make_signal_1_chunk( _spScreen->OnChar ))
            .add("OnMouseBtn", make_signal_2_chunk( _spScreen->OnMouseBtn ))
            .add("OnDoubleClick", make_signal_1_chunk( _spScreen->OnDoubleClick ))
            .add("OnMouseMove", make_signal_2_chunk( _spScreen->OnMouseMove ))
            .add("OnMouseWheel", make_signal_2_chunk( _spScreen->OnMouseWheel ))
            ;

        add_methods( this )
            .def("CreateImageBuffer", &ScreenProxy::CreateImageBuffer, "width", "height")
            .def("CreateSubImage", &ScreenProxy::CreateSubImage, "area" )
            .def("CreatePainter", &ScreenProxy::CreatePainter )
            .def("GetClipper", &ScreenProxy::GetClipper, "crop" )
            .def("WriteImage", &ScreenProxy::WriteImage, "file" )
            ;
    }

    ScreenProxy::~ScreenProxy()
    {
    }

    IChunkPtr ScreenProxy::CreateImageBuffer( int width, int height )
    {
        Useless::ImageBuffer *pBuf = new Useless::ImageBuffer( width, height );
        IChunkPtr proxy = new ImageProxy( pBuf );
        pBuf->Cooperate( *_spScreen->GetSurface() );
        return proxy;
    }
    
    IChunkPtr ScreenProxy::CreateSubImage( const Rect &area )
    {
        return new ImageProxy( SPointer< ImageBase >( _spScreen->QueryIGraphics( area ), dynamic_cast_tag ));
    }
    
    IChunkPtr ScreenProxy::CreatePainter()
    {
        _spScreen->SetClipper( Rect( _spScreen->GetWidth(), _spScreen->GetHeight() ));
        return new PainterProxy( Painter( *_spScreen ));
    }
        
    IChunkPtr ScreenProxy::GetClipper( const Useless::Rect &crop )
    {
        return new RectListProxy( _spScreen->GetClipper( crop ));
    }

    void ScreenProxy::WriteImage( std::string file )
    {
        ImageToFile::Store( *_spScreen->GetSurface(), file );
    }
    

    /*******************************

      LayoutProxy

    */
    LayoutProxy::LayoutProxy( LayoutBase *layout ): _layout( layout )
    {
        add_methods( this )
            .def("SetWeight", &LayoutProxy::SetWeight, "widget", "weight")
            .def("SetMaxSize", &LayoutProxy::SetMaxSize, "widget", "maxsize")
            .def("SetMinSize", &LayoutProxy::SetMinSize, "widget", "minsize")
            .def("SetOverlap", &LayoutProxy::SetOverlap, "widget", "overlap")
            ;
        add_methods( _layout, this )
            .def("Fill", &LayoutBase::Fill )
            ;
    }

    LayoutProxy::~LayoutProxy()
    {
    }

    void LayoutProxy::SetWeight( Widget *widget, int weight )
    {
        (*_layout) << widget;
        Weight_ attr( weight ); (*_layout) << attr;
    }

    void LayoutProxy::SetMaxSize( Widget *widget, int maxsize )
    {
        (*_layout) << widget;
        MaxSize_ attr( maxsize ); (*_layout) << attr;
    }

    void LayoutProxy::SetMinSize( Widget *widget, int minsize )
    {
        (*_layout) << widget;
        MinSize_ attr( minsize ); (*_layout) << attr;
    }

    void LayoutProxy::SetOverlap( Widget *widget, int overlap )
    {
        (*_layout) << widget;
        Overlap_ attr( overlap ); (*_layout) << attr;
    }

    /*******************************

      WorkspaceProxy

    */
    WorkspaceProxy::WorkspaceProxy( Workspace *workspace ): _workspace( workspace )
    {
        add_methods( _workspace, this )
            .def("GetWorkspaceRect", unconst_method_cast( &Workspace::GetWorkspaceRect ))
            .def("ShowAtPointer", &Workspace::ShowAtPointer, "widget" )
            .def("ShowAtPosition", &Workspace::ShowAtPosition, "pos", "widget" )
            .def("WhereIsPointer", unconst_method_cast( &Workspace::WhereIsPointer ))
            .def("SetPreviousCursor", &Workspace::SetPreviousCursor )
            .def("ShowCursor", &Workspace::ShowCursor )
            .def("HideCursor", &Workspace::HideCursor )
            .def("IsCursorShown", unconst_method_cast( &Workspace::IsCursorShown ))
            .def("IsKeyPressed", unconst_method_cast( &Workspace::IsKeyPressed ), "key_code")
            .def("GetWidth", unconst_method_cast( &Workspace::GetWidth ))
            .def("GetHeight", unconst_method_cast( &Workspace::GetHeight ))
            ;

        add_methods( this )
            .def("SetCursor", &WorkspaceProxy::SetCursor, "name" )
            ;
    }

    WorkspaceProxy::~WorkspaceProxy()
    {
    }

    void WorkspaceProxy::SetCursor( std::string name )
    {
        SPointer< MouseCursor > cursor = MakeMouseCursor( name );
        _workspace->SetCursor( *cursor );
    }

    
};//XMLProgram

