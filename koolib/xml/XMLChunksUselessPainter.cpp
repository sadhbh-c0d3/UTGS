#include "XMLChunksUseless.h"

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

      WidgetPainterProxy

    */
    WidgetPainterProxy::WidgetPainterProxy( const WidgetPainter &painter ): _wpainter( painter ), PainterProxy( painter )
    {
        add_methods( this )
            .def("RepaintWidget", &WidgetPainterProxy::RepaintWidget)
            ;
    }

    WidgetPainterProxy::~WidgetPainterProxy()
    {
    }

    void WidgetPainterProxy::RepaintWidget()
    {
        Widget *widget = _wpainter.GetWidget();
        dynamic_cast_call< CompoundWidget >( widget, &CompoundWidget::DoRepaint, _wpainter );
    }

    /*******************************

      PainterProxy

    */
    PainterProxy::PainterProxy( const Painter &painter ):_painter( painter )
    {
        add_methods( this )
            .def("Blit", &PainterProxy::Blit)
            .def("FastBlit", &PainterProxy::FastBlit)
            .def("FastDrawPolygon", &PainterProxy::FastDrawPolygon )
            .def("FastMultiBlit", &PainterProxy::FastMultiBlit )
            .def("FastFillRect", &PainterProxy::FastFillRect )
            .def("FastDrawRect", &PainterProxy::FastDrawRect )
            .def("FastDrawLine", &PainterProxy::FastDrawLine )
            .def("FillRect", &PainterProxy::FillRect)
            .def("DrawRect", &PainterProxy::DrawRect)
            .def("DrawLine", &PainterProxy::DrawLine)
            .def("DrawPolygon", &PainterProxy::DrawPolygon)
            .def("MultiBlit", &PainterProxy::MultiBlit)
            .def("SubCanvasPaint", &PainterProxy::SubCanvasPaint)
            .def("GetClipper", &PainterProxy::GetClipper, "crop" )
            .def("GetOffset", &PainterProxy::GetOffset )
            .def("Clear", &PainterProxy::Clear, "color" )
            .def("Intersect", &PainterProxy::Intersect, "crop" )
            ;
    }

    PainterProxy::~PainterProxy()
    {
    }

    IChunkPtr PainterProxy::GetClipper( const Useless::Rect &crop )
    {
        Pos offset = _painter.GetOffset();
        RectList absRegion = _painter.GetPlane()->GetClipper( crop + offset );
        RectListProxy *pOut = new RectListProxy( RectList() );
        IChunkPtr pResult = pOut;
        for ( int i=0, I=absRegion.size(); i<I; ++i )
        {
            pOut->_rects.push_back( absRegion[i] - offset );
        }
        return pResult;
    }

    Pos PainterProxy::GetOffset()
    {
        return _painter.GetOffset();
    }

    void PainterProxy::Clear( int color )
    {
        _painter.Clear( color );
    }

    void PainterProxy::Intersect( const Rect &crop )
    {
        _painter.Intersect( crop );
    }

    void PainterProxy::FastBlit( Node __unused__, ExecutionState &_state )
    {
        IChunk *pImage =_state._currentBlock->GetChunk( L"Imag" );
        if ( !pImage ) { throw Error("FastBlit: expects 'Imag'"); }
        ImageProxy *pImageProx = dynamic_cast< ImageProxy *>( pImage );
        if ( !pImageProx ) { throw Error("FastBlit: expects 'Imag' to be type of Image");}            
        IChunk *pCanv = _state._currentBlock->GetChunk( L"Canv" );
        IChunk *pTexc = _state._currentBlock->GetChunk( L"Texc" );
        SPointer< ImageBase > spImg = pImageProx->_spImage;
        if ( 0 != pTexc )
        {
            int x = attribute_of< int >( pTexc, L"x" );
            int y = attribute_of< int >( pTexc, L"y" );
            int w = attribute_of< int >( pTexc, L"w" );
            int h = attribute_of< int >( pTexc, L"h" );
            SPointer< IGraphics > cp = spImg->Copy( Rect(x, y, w, h));
            spImg.Reset( dynamic_cast< ImageBase *>( cp.Release() ));
        }
        if ( !pCanv ) { throw Error("FastBlit: expects 'Canv'");}            
        int x = attribute_of< int >( pCanv, L"x" );
        int y = attribute_of< int >( pCanv, L"y" );
        int w = attribute_of< int >( pCanv, L"w" );
        int h = attribute_of< int >( pCanv, L"h" );
        spImg->SetDestWidth( w );
        spImg->SetDestHeight( h );
        _painter.ProjectImage( x, y, *spImg );
    }
    
    void PainterProxy::FastDrawPolygon( Node __unused__, ExecutionState &state )
    {
        IChunk *pColor = state._currentBlock->GetChunk(L"color");
        if ( !pColor ) { throw Error("FastDrawPolygon: expects 'color'"); }
        IChunk *pVertices = state._currentBlock->GetChunk(L"vertices");
        if ( !pVertices ) { throw Error("FastDrawPolygon: expects 'vertices'"); }
        PointListProxy *vertices = dynamic_cast< PointListProxy *>( pVertices );
        if ( !vertices ) { throw Error("FastDrawPolygon: excpects 'vertices' to be type of PointList"); }
        _painter.PaintPolygon( vertices->_points, value_of< int >( pColor ));
    }

    void PainterProxy::FastMultiBlit( Node __unused__, ExecutionState &state )
    {
        IChunk *pImage = state._currentBlock->GetChunk( L"Imag" );
        if ( !pImage ) { throw Error("FastMultiBlit: expects 'Imag'"); }
        ImageProxy *pImageProx = dynamic_cast< ImageProxy *>( pImage );
        if ( !pImageProx ) { throw Error("FastMultiBlit: expects 'Imag' to be type of Image");}            
        IChunk *pPositions = state._currentBlock->GetChunk(L"positions");
        if ( !pPositions ) { throw Error("FastMultiBlit: expects 'positions'"); }
        PointListProxy *pPointList = dynamic_cast< PointListProxy *>( pPositions );
        if ( !pPointList ) { throw Error("FastMultiBlit: expects 'positions' to be type of PointList"); }        
        IChunk *pRects = state._currentBlock->GetChunk(L"rects");
        if ( !pRects ) { throw Error("FastMultiBlit: expects 'rects'"); }
        RectListProxy *pRectList = dynamic_cast< RectListProxy *>( pRects );
        if ( !pRectList ) { throw Error("FastMultiBlit: expects 'rects' to be type of RectList"); }
        _painter.ProjectImages( *pImageProx->_spImage, pPointList->_points, pRectList->_rects );

    }
    
    void PainterProxy::FastFillRect( Node _node, ExecutionState &_state )
    {
        Attr< Color, true, wchar_t >   _color(L"color");
        GetAttr( _color, _node, _state );

        static LazyGetChunkInScope s_LazyGetX(L"x", true);
        static LazyGetChunkInScope s_LazyGetY(L"y", true);
        static LazyGetChunkInScope s_LazyGetW(L"w", true);
        static LazyGetChunkInScope s_LazyGetH(L"h", true);
        
        IChunk *pChunk = _state._currentBlock;
        int x = value_of< int >( s_LazyGetX( _state ));
        int y = value_of< int >( s_LazyGetY( _state ));
        int w = value_of< int >( s_LazyGetW( _state ));
        int h = value_of< int >( s_LazyGetH( _state ));

        _painter.PaintRectangle( _color->GetInteger(), Rect(x,y,w,h));
    }

    void PainterProxy::FastDrawRect( Node _node, ExecutionState &_state )
    {
        Attr< Color, true, wchar_t >   _color(L"color");
        GetAttr( _color, _node, _state );
        int c = _color->GetInteger();

        static LazyGetChunkInScope s_LazyGetX(L"x", true);
        static LazyGetChunkInScope s_LazyGetY(L"y", true);
        static LazyGetChunkInScope s_LazyGetW(L"w", true);
        static LazyGetChunkInScope s_LazyGetH(L"h", true);
        
        IChunk *pChunk = _state._currentBlock;
        int x = value_of< int >( s_LazyGetX( _state ));
        int y = value_of< int >( s_LazyGetY( _state ));
        int w = value_of< int >( s_LazyGetW( _state ));
        int h = value_of< int >( s_LazyGetH( _state ));

        _painter.PaintBoundary( Rect(x,y,w,h), c, c, c, c);
    }

    void PainterProxy::FastDrawLine( Node _node, ExecutionState &_state )
    {
        static LazyGetChunkInScope s_LazyGetVertices(L"vertices", true);
        static LazyGetChunkInScope s_LazyGetThickness(L"thickness", true);
        static LazyGetChunkInScope s_LazyGetColor(L"color", true);
        
        IChunk *pVertices = s_LazyGetVertices( _state ).get();
        if ( pVertices )
        {
            IChunk *pThickness = s_LazyGetThickness( _state ).get();
            IChunk *pColor = s_LazyGetColor( _state ).get();
            if ( !pThickness ) { throw Error("FastDrawLine: expects 'thickness'"); }
            if ( !pColor ) { throw Error("FastDrawLine: expects 'color'"); }
            PointListProxy *vertices = dynamic_cast< PointListProxy *>( pVertices );
            if ( !vertices ) { throw Error("FastDrawLine: excpects 'vertices' to be type of PointList"); }
            _painter.PaintLine( vertices->_points, value_of< int >( pColor ), value_of< int >( pThickness ));
        }
        else
        {
            Attr< Color, true, wchar_t >   _color(L"color");
            Attr< int, true, wchar_t >     _thickness(L"thickness");

            GetAttr( _color, _node, _state );
            GetAttr( _thickness, _node, _state );
            int c = _color->GetInteger();

            static LazyGetChunkInScope s_LazyGetX0(L"x0", true);
            static LazyGetChunkInScope s_LazyGetX1(L"x1", true);
            static LazyGetChunkInScope s_LazyGetY0(L"y0", true);
            static LazyGetChunkInScope s_LazyGetY1(L"y1", true);

            IChunk *pChunk = _state._currentBlock;
            int x0 = value_of< int >( s_LazyGetX0( _state ));
            int y0 = value_of< int >( s_LazyGetY0( _state ));
            int x1 = value_of< int >( s_LazyGetX1( _state ));
            int y1 = value_of< int >( s_LazyGetY1( _state ));

            _painter.PaintLine( Pos(x0,y0), Pos(x1,y1), c, *_thickness );
        }
    }


    void PainterProxy::Blit( Node _node, ExecutionState &_state )
    {
        Attr< TextAnsi, true, wchar_t > _img(L"img");
        Attr< int, false, wchar_t > _where(L"where");
        Attr< int, false, wchar_t > _from(L"from");

        GetAttr( _img, _node, _state );

        SPointer< ImageBase > img = MakeImage( get_value( _img ) );

        int x=0, y=0, w=0, h=0;

        if ( GetMultiAttr( _from, _node, _state ))
        {
            int cx=0, cy=0, cw=0, ch=0;
            cx = _from++;
            cy = _from++;
            cw = _from++;
            ch = _from++;
            SPointer< IGraphics > cp = img->Copy( Rect(cx, cy, cw, ch));
            img.Reset( dynamic_cast< ImageBase *>( cp.Release() ));
        }

        if ( GetMultiAttr( _where, _node, _state ))
        {
            x = _where++;
            y = _where++;
            if ( !!_where )
            {
                w = _where++;
                h = _where++;
                img->SetDestWidth( w );
                img->SetDestHeight( h );
            }
        }

        _painter.ProjectImage( x, y, *img );
    }

    void PainterProxy::FillRect( Node _node, ExecutionState &_state )
    {
        Attr< int, true, wchar_t > _where(L"where");
        Attr< Color, true, wchar_t >   _color(L"color");

        GetMultiAttr( _where, _node, _state );
        GetAttr( _color, _node, _state );

        int x = _where++;
        int y = _where++;
        int w = _where++;
        int h = _where++;

        _painter.PaintRectangle( _color->GetInteger(), Rect(x, y, w, h));
    }

    void PainterProxy::DrawRect( Node _node, ExecutionState &_state )
    {
        Attr< int, true, wchar_t >     _where(L"where");
        Attr< Color, true, wchar_t >   _color(L"color");

        GetMultiAttr( _where, _node, _state );
        GetAttr( _color, _node, _state );

        int x = _where++;
        int y = _where++;
        int w = _where++;
        int h = _where++;
        int c = _color->GetInteger();

        _painter.PaintBoundary( Rect(x, y, w, h), c, c, c, c);
    }

    void PainterProxy::DrawLine( Node _node, ExecutionState &_state )
    {
        Attr< int, true, wchar_t >     _from(L"from");
        Attr< int, true, wchar_t >     _to(L"to");
        Attr< Color, true, wchar_t >   _color(L"color");
        Attr< int, true, wchar_t >     _thickness(L"thickness");

        GetMultiAttr( _from, _node, _state );
        GetMultiAttr( _to, _node, _state );
        GetAttr( _color, _node, _state );
        GetAttr( _thickness, _node, _state );

        int x1 = _from++;
        int y1 = _from++;
        int x2 = _to++;
        int y2 = _to++;
        int c = _color->GetInteger();

        _painter.PaintLine( Pos(x1,y1), Pos(x2,y2), c, *_thickness );
    }

    void PainterProxy::DrawPolygon( Node _node, ExecutionState &_state )
    {
        Attr< Color, true, wchar_t > _color(L"color");
        GetAttr( _color, _node, _state );
        
        static LazyGetChunkInScope s_LazyGetVertices(L"vertices", true);
        static LazyGetChunkMember s_LazyGetX(L"x");
        static LazyGetChunkMember s_LazyGetY(L"y");
        
        IChunk *pChunk = _state._currentBlock;

        std::vector< Pos > vertices;
        IChunkPtr pObject = s_LazyGetVertices( _state );
        IChunk *pCurrent = pObject.get();
        while ( !IsEmpty( pCurrent ))
        {
            IChunk *pHead = pCurrent->GetChunk( FOURCC_LIST_HEAD );
            int x = value_of< int >( s_LazyGetX( pHead, _state ));
            int y = value_of< int >( s_LazyGetY( pHead, _state ));
            vertices.push_back( Pos( x, y ));
            pCurrent = pCurrent->GetChunk( FOURCC_LIST_TAIL );
        }

        _painter.PaintPolygon( vertices, _color->GetInteger() );
    }

    void PainterProxy::MultiBlit( Node _node, ExecutionState &_state )
    {
        Attr< TextAnsi, true, wchar_t > _img(L"img");

        GetAttr( _img, _node, _state );

        SPointer< ImageBase > img = MakeImage( get_value( _img ) );

        PointList positions;
        RectList  rects;

        IChunkPtr pObject1 = XMLProgram::GetChunk(L"positions", _state);
        IChunkPtr pObject2 = XMLProgram::GetChunk(L"rects", _state);

        IChunk *pCurrent1 = pObject1.get();
        IChunk *pCurrent2 = pObject2.get();

        while ( !IsEmpty( pCurrent1 ) && !IsEmpty( pCurrent2 ))
        {
            IChunk *pHead1 = pCurrent1->GetChunk( FOURCC_LIST_HEAD );
            IChunk *pHead2 = pCurrent2->GetChunk( FOURCC_LIST_HEAD );
            Attr< int, true, wchar_t > _x(L"x");
            Attr< int, true, wchar_t > _y(L"y");
            Attr< int, true, wchar_t > _cx(L"x");
            Attr< int, true, wchar_t > _cy(L"y");
            Attr< int, true, wchar_t > _cw(L"w");
            Attr< int, true, wchar_t > _ch(L"h");
            (*pHead1) >> _x;
            (*pHead1) >> _y;
            (*pHead2) >> _cx;
            (*pHead2) >> _cy;
            (*pHead2) >> _cw;
            (*pHead2) >> _ch;
            positions.push_back( Pos( *_x, *_y ));
            rects.push_back( Rect( *_cx, *_cy, *_cw, *_ch ));
            pCurrent1 = pCurrent1->GetChunk( FOURCC_LIST_TAIL );
            pCurrent2 = pCurrent2->GetChunk( FOURCC_LIST_TAIL );
        }

        _painter.ProjectImages( *img, positions, rects );
    }

    void PainterProxy::SubCanvasPaint( Node __unused__, ExecutionState &state )
    {
        IChunk *pFunc   = state._currentBlock->GetChunk(L"func");
        if ( !pFunc ) { throw Error("SubCanvasPaint: expects 'func'"); }
        IChunk *pOffset = state._currentBlock->GetChunk(L"offset");
        IChunk *pColor  = state._currentBlock->GetChunk(L"color");
        IChunk *pAlpha  = state._currentBlock->GetChunk(L"alpha");
        IChunk *pBlending = state._currentBlock->GetChunk(L"blending");
        IChunk *pCrop   = state._currentBlock->GetChunk(L"crop");

        Pos offset(0,0);
        if ( pOffset ) {
            offset.x = value_of< int >( pOffset->GetChunk(L"x") );
            offset.y = value_of< int >( pOffset->GetChunk(L"y") );
        }

        PainterProxy *pProxy1 = new PainterProxy( Painter( _painter, offset ));

        SubScope newState( state );
        newState._currentBlock->AddChunk(L"__painter__", pProxy1 );

        if ( pCrop ) {
            Rect crop;
            crop.x = value_of< int >( pCrop->GetChunk(L"x") );
            crop.y = value_of< int >( pCrop->GetChunk(L"y") );
            crop.w = value_of< int >( pCrop->GetChunk(L"w") );
            crop.h = value_of< int >( pCrop->GetChunk(L"h") );
            pProxy1->_painter.Intersect( crop );
        }

        if ( pAlpha ) {
            pProxy1->_painter.MultiplyAlpha( value_of< int >( pAlpha ));
        }
        
        AdvancedPaint advPaint( _painter );
        int APtouched = 0;
        
        if ( pBlending )
        {
            TextUtf8 blending = value_of< TextUtf8 >( pBlending );
            APtouched |= 1;
            if ( L"normal" == blending ) {
                advPaint.BlendFun( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, true );
            }
            else if ( L"replace" == blending ) {
                advPaint.BlendFun( GL_ONE, GL_ZERO, true );
            }
            else if ( L"add" == blending ) { // add RGBA
                advPaint.BlendFun( GL_SRC_ALPHA, GL_ONE, true );
            }
            else if ( L"multiply" == blending ) { // multiply RGBA
                advPaint.BlendFun( GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA, true );
            }
            else if ( L"add-color" == blending ) { // add RGB
                advPaint.BlendFun( GL_ONE, GL_ONE_MINUS_SRC_ALPHA, true );
            }
            else if ( L"multiply-color" == blending ) { // multiply RGB
                advPaint.BlendFun( GL_DST_COLOR, GL_ZERO, true );
            }
            else if ( L"shadow" == blending ) {
                advPaint.BlendFun( GL_ZERO, GL_ONE_MINUS_SRC_ALPHA, true );
            }
        }

        if ( pColor )
        {
            APtouched |= 2;
            NormalPixel color( value_of< int >( pColor ));
            advPaint.ConstColor( color.c.r, color.c.g, color.c.b );
        }

        pFunc->Execute( __unused__, newState );

        if ( APtouched & 1 )
        {
            advPaint.BlendFun( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, true );
        }
        if ( APtouched & 2 )
        {
            advPaint.ConstColor( 255, 255, 255 );
        }
    }
   
};//XMLProgram

