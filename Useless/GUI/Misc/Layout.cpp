#include "UselessPch.h"

#include "Useless/GUI/Misc/Layout.h"
#include "Useless/GUI/Widgets/HubWidget.h"
#include "Useless/GUI/Widgets/NullWidget.h"
#undef min
#undef max
#include <set>
#include <cmath>
#include <algorithm>

namespace Useless {

Layout::Layout(): _p_root(0), _root_owner(0), _fill_requests(0)
{}

Layout::Layout( HubWidget *p_root ) : _p_root(p_root), _root_owner(0), _fill_requests(0)
{
    _on_resize_root = Tie2Signal( _p_root->OnResize, this, &Layout::Fill );
}

Layout::Layout(const Layout &layout) : _p_root(layout._p_root), _root_owner(0), _fill_requests(0),
    _layout(layout._layout), _children_layouts(layout._children_layouts)
{
    _current = Iterator( _layout.Begin() );
    _on_resize_root = Tie2Signal( _p_root->OnResize, this, &Layout::Fill );
}

Layout& Layout::operator =( const Layout &layout )
{
    if ( _p_root && _root_owner )
    {
        delete _p_root; _root_owner=false;
    }
    if ( layout._root_owner )
    {
        const_cast<Layout&>(layout)._root_owner = false;
        _root_owner = true;
    }
    _p_root = layout._p_root;
    _layout = layout._layout;
    _children_layouts = layout._children_layouts;
    _on_resize_root = Tie2Signal( _p_root->OnResize, this, &Layout::Fill );
    return *this;
}


Layout::~Layout()
{
    if (_p_root && _root_owner ) { delete _p_root; }
    _on_resize_root.Untie();
    
    for ( StupidVector< Signal::FuncID >::Iterator
            cri = _on_child_resize.Begin(); !!cri; ++cri )
        { (*cri).Untie(); }
}

LayoutBase& Layout::operator << ( Widget *p_widget )
{
    if ( 0 == p_widget )
    {
        _current = Iterator( _layout.End() - 1 );
    }
    if ( !!(*p_widget) )
    {
        for ( Iterator it = _layout.Begin(); !!it; ++it )
        {
            if ( it.Key() == p_widget )
            {
                _current = it;
                return (*this);
            }
        }
    }
    if ( !_p_root )
    {
        _p_root = new HubWidget;
        _root_owner = true;
        _on_resize_root = Tie2Signal( _p_root->OnResize, this, &Layout::Fill );
    }
    _on_child_resize.Insert( Tie2Signal( p_widget->OnResize, this, &Layout::Fill ));
    _layout.Insert( WidgetAttrib(p_widget, Attrib_()) );
    _current = Iterator( _layout.End() - 1 );
    _p_root->Insert( p_widget );
    Tie2Signal( p_widget->OnVisible, this, &Layout::Fill );
    return *this;
}

LayoutBase& Layout::operator << ( const Attrib &attrib )
{
    attrib.Accept( _current.Value() );
    return *this;
}

LayoutBase& Layout::operator << ( const LayoutBase &layout )
{
    if ( const Layout *p_l = dynamic_cast<const Layout*>(&layout) )
    {
        HubWidget *p_root = p_l->Get();
        operator << (p_root);
        _children_layouts.Insert( layout.Copy() );
        const_cast<Layout*>(p_l)->_root_owner = false;
    }
    return *this;
}

LayoutBase& Layout::operator << ( const Gap_ &gap )
{
    (*this) << new NullWidget;

    if ( gap._s > 0 )
    {
        (*this) << Weight_(gap._s);
    }
    else if ( gap._s < 0 )
    {
        (*this) << MinSize_(-gap._s);
        (*this) << MaxSize_(-gap._s);
    }
    return *this;
}


void Layout::Fill()
{
    if ( 0 == _fill_requests )
    {
        FillLock guard_fill( this );

        int N = _layout.Size(); // count of cells
        int h = GetSize();      // full area size (direction depends on layout type: H or V )
        StupidVector<int> sz; sz.Resize(N); // cell sizes (algorithm output)

        int  i;   // iterator
        int  ws;  // weight accum.
        int  ss;  // size  accum.
        float ds; // delta accum.
        int  c,C; // non-zero weights counter, all weights counter
        bool too_small = false; // flag telling that we covered too small area
        bool too_large = false; // flag telling that we went out of area bounds

        std::set<int> _small;   // set of iterators - cells which reached minimum size
        std::set<int> _large;   // set of iterators - cells which reached maximum size
        std::set<int> _excluded;// set of iterators - excluded cells
        int fixed;
        bool out;

        int iter_num=0;

        do {

            if ( iter_num++ == N ) { assert("Layout::Fill() forced algorithm break"); break; }

            for ( i=0, ws=0, fixed=0, c=0, C=0; i<N; ++i )
            {
                if ( too_small && _large.find(i)!=_large.end() ) { fixed+=sz[i]; continue; }
                else if ( too_large && _small.find(i)!=_small.end() ) { fixed+=sz[i]; continue; }
                else if ( _excluded.find(i)!=_excluded.end() ) { fixed+=sz[i]; continue; }

                if ( !(*_layout[i].first) || _layout[i].first->IsVisible() )
                {
                    Weight_ wg = _layout[i].second; if (wg.x>0) { ++c; ws += wg.x; } ++C;
                }
            }

            if (!c && C>0) { c=C; ws=C; }
            if (!ws) { break; }

            for ( i=0, ss=0, ds=0, out=false; i<N; ++i )
            {
                if ( !!(*_layout[i].first) && !_layout[i].first->IsVisible() )
                {
                    sz[i] = 0;
                    continue;
                }
                Weight_ wg = _layout[i].second; if ( wg.x<=0 ) { wg.x = ws/c;}
                AlignSize_ alignsize = _layout[i].second;
                AlignSizePad_ alignsizepad = _layout[i].second;

                float fx = float( (h-fixed) * wg.x * c )/float(ws * C);
                int x = fx;
                if ( alignsize.x > 0 ) { x -= alignsizepad.x; x /= alignsize.x; x *= alignsize.x; x += alignsizepad.x; }

                if ( _large.find(i)!=_large.end() ) 
                {
                    if ( !too_small && x<sz[i] ) { _large.erase(_large.find(i)); }
                    else 
                    {
                        if (!too_small) { _excluded.insert(i); }
                        ss+=sz[i]; continue; 
                    }
                }
                if ( _small.find(i)!=_small.end() ) 
                { 
                    if ( !too_large && x>sz[i] ) { _small.erase(_small.find(i)); }
                    else 
                    { 
                        if (!too_large) { _excluded.insert(i); }
                        ss+=sz[i]; continue; 
                    }
                }
                if ( _excluded.find(i)!=_excluded.end() || sz[i]==x ) 
                { 
                    if (!out) { _excluded.insert(i); out=true; }
                    ss+=sz[i]; continue; 
                }

                ds += fx - x;
                if ( ds > 0.5 ) { ++x; ds=0.5-ds; }

                MinSize_ min = _layout[i].second;
                MaxSize_ max = _layout[i].second;

                if ( x < min.x )
                {
                    x = min.x; _small.insert(i);
                }
                else if ( max.x && ( x>max.x ) )
                {
                    x = max.x; _large.insert(i);
                }

                sz[i] = x;
                ss += x;
            }
            too_small = (ss < h);
            too_large = (ss > h);

        } while ( too_small || too_large );

        SetupWidgets(sz);
    }
    else { // postpone Fill() command
        _fill_requests = 2; // limit counter to value of '2'
        return;
    }

    // execute postponed Fill() command
    if ( 1 == _fill_requests )
    {
        --_fill_requests;
        Fill();
    }
}

int  HLayout::GetSize()
{
    HubWidget *p_root = Get();
    Rect area = p_root->GetClientRect( p_root->FirstChild() );
    return area.GetW();
}

int  VLayout::GetSize()
{
    HubWidget *p_root = Get();
    Rect area = p_root->GetClientRect( p_root->FirstChild() );
    return area.GetH();
}

void HLayout::SetupWidgets( const StupidVector<int> &sizes )
{
    HubWidget *p_root = Get();
    Rect area = p_root->GetClientRect( p_root->FirstChild() );
    int w = area.GetW();
    int h = area.GetH();
    Pos p = area.GetPos();

    Iterator  it = _layout.Begin();
    StupidVector<int>::ConstIterator is = sizes.ConstBegin();
    LayoutMap::ConstIterator il = _layout.ConstBegin();
    assert( sizes.Size() == _layout.Size() );

    for ( ; !!it; ++it, ++is, ++il )
    {
        if ( !(*it.Key()) || it.Key()->IsVisible() )
        {
            Overlap_ overlap = it.Value();
            OrthoMaxSize_ omax = il.Value().second;
            OrthoMinSize_ omin = il.Value().second;
            OrthoAlignSize_ oals = il.Value().second;
            OrthoAlignSizePad_ oalsp = il.Value().second;
            OrthoPlacement_ oplace = il.Value().second;
            PadStart_ padstart = il.Value().second;
            PadEnd_   padend = il.Value().second;

            int fullh = h - (padstart.x + padend.x);
            int neww = *is + overlap.x;
            int newh = h;
            if ( omax.x > 0 ) { newh = std::min( newh, omax.x ); }
            if ( omin.x > 0 ) { newh = std::max( newh, omin.x ); }
            if ( oals.x > 0 ) { newh -= oalsp.x; newh /= oals.x; newh *= oals.x; newh += oalsp.x; fullh /= oals.x; fullh *= oals.x; }

            int dx= ( *is+overlap.x - it.Key()->GetWidth() )/2 - overlap.x;
            int dy= padstart.x + (( fullh - newh ) * oplace.x) / 100;

            it.Key()->Resize( neww, newh );
            it.Key()->SetPosition( p+Pos(dx,dy) );
            p += Pos( *is, 0);
        }
    }
}

void VLayout::SetupWidgets( const StupidVector<int> &sizes )
{
    HubWidget *p_root = Get();
    Rect area = p_root->GetClientRect( p_root->FirstChild() );
    int w = area.GetW();
    int h = area.GetH();
    Pos p = area.GetPos();

    Iterator  it = _layout.Begin();
    StupidVector<int>::ConstIterator is = sizes.ConstBegin();
    LayoutMap::ConstIterator il = _layout.ConstBegin();
    assert( sizes.Size() == _layout.Size() );

    for ( ; !!it; ++it, ++is, ++il )
    {
        if ( !(*it.Key()) || it.Key()->IsVisible() )
        {
            Overlap_ overlap = it.Value();
            OrthoMaxSize_ omax = il.Value().second;
            OrthoMinSize_ omin = il.Value().second;
            OrthoAlignSize_ oals = il.Value().second;
            OrthoAlignSizePad_ oalsp = il.Value().second;
            OrthoPlacement_ oplace = il.Value().second;
            PadStart_ padstart = il.Value().second;
            PadEnd_   padend = il.Value().second;

            int fullw = w - (padstart.x + padend.x);
            int neww = fullw;
            int newh = *is + overlap.x;
            if ( omax.x > 0 ) { neww = std::min( neww, omax.x ); }
            if ( omin.x > 0 ) { neww = std::max( neww, omin.x ); }
            if ( oals.x > 0 ) { neww -= oalsp.x; neww /= oals.x; neww *= oals.x; neww += oalsp.x; fullw /= oals.x; fullw *= oals.x; }

            int dx= padstart.x + (( fullw - neww ) * oplace.x ) / 100;
            int dy= ( *is+overlap.x -it.Key()->GetHeight() )/2 - overlap.x;
            
            it.Key()->Resize( neww, newh );
            it.Key()->SetPosition( p+Pos(dx,dy) );
            p += Pos( 0, *is);
        }
    }
}


};//namespace Useless
