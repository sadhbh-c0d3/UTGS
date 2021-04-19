#ifndef __INCLUDED_USELESS_SCROLLBAR_WIDGET_H__
#define __INCLUDED_USELESS_SCROLLBAR_WIDGET_H__

#pragma warning(disable:4250) // inherit via dominance
#include "Useless/GUI/Widgets/ActiveWidget.h"
#include "Useless/GUI/Widgets/ButtonWidget.h"
#include "Useless/GUI/Widgets/DraggerWidget.h"
#include "Useless/GUI/Widgets/BridgeWidget.h"
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Skins/SkinSet.h"
#include "Useless/Util/LiveVariable.h"
#include "Useless/GUI/WidgetInterfaces.h"

namespace Useless {

//USES
    class ScrollbarStyle;

/*! \ingroup Widgets
 *  A Scrollbar Widget. Orientation is set from Skin::GetDirMask().
 */
class USELESS_API ScrollbarWidget : private BridgeWidget, virtual public NoKeyButton, public IWScrollbar
{
public:
    enum Images { PAGE, PAGE_PRESSED, PAGEHL, PAGEDIS, PAGER, PAGER_PRESSED, PAGERHL, PAGERDIS };

    ScrollbarWidget( int size, const SkinSet &skins, int size_limit=16 );
    ScrollbarWidget( const ScrollbarStyle &style);
    virtual ~ScrollbarWidget();

    virtual void Repaint(const WidgetPainter &painter );
    virtual void Resize       (int w, int h);

    virtual bool Accept ( const Request& request);

            void SetAdvance(int advance);
            void SetSize   (int size);
            void SetRange  (int full);
            void UpdateScrollbar();

            void Scroll    (int units) { SetAdvance( _advance+units ); }
             int GetAdvance() const    { return _advance; }
             int GetSize   () const    { return _size; }
             int GetRange  () const    { return _full; }

            void SetLength (int len);
             int GetLength ()        const { return (_horizontal)? GetWidth() : GetHeight(); }
             int GetPagerSizeLimit() const { return _size_limit; }
            bool IsHorizontal()      const { return _horizontal; }
    /////////////////////////////////////////////////////////////////////////
    //               S I G N A L S
    //       TYPE                    | NAME                      | ARGS
    //-----------------------------------------------------------------------
    Signal_1<int>&                   OnAdvance;                 // advance
    Signal_1<int>&                   OnSize;                    // pager size
    Signal_1<int>&                   OnRange;                   // full size
    //
    /////////////////////////////////////////////////////////////////////////

    virtual ScrollbarWidget& Scroller()  { return *this; }

    static SkinSet ExtractPageSkin(const SkinSet &skins);

private:
    LiveVariable<int>  _advance;
    LiveVariable<int>  _size;
    LiveVariable<int>  _full;

    DraggerWidget     *_pager;
    bool               _horizontal;
    int                _size_limit;
    bool               _dragging_pager;

    void OnPager(bool cought);
    void Drag(const Pos& cursor, const Pos& dpos, const Pos& pick);
    void Page(const Pos& p);
    void SetPGSize(int size);
    int  GetPGSize() const;

    void Init( int size, const SkinSet &skins, int size_limit );
};



/*
-------------------------------------------------------------------------------------*/
class ScrollbarStyle
{
public:
    ScrollbarStyle(           int  size, 
                    const SkinSet &skins,
                              int  size_limit=16 

                  ): size(size), skins(skins), size_limit(size_limit) {}

    virtual ~ScrollbarStyle() {}

              int  size;
    const SkinSet  &skins;
              int  size_limit;
};




/*
-------------------------------------------------------------------------------------*/
inline void ScrollbarWidget::SetPGSize(int size)
{
    assert(size>=_size_limit);

    if (_horizontal)
        { _pager->Resize( size, _pager->GetHeight() ); }
    else
        { _pager->Resize( _pager->GetWidth(), size ); }
}

inline int ScrollbarWidget::GetPGSize() const
{
    return (_horizontal)? _pager->GetWidth() : _pager->GetHeight();
}

inline void ScrollbarWidget::SetLength (int len)
{
    Resize( _horizontal ? len:GetWidth(), _horizontal ? GetHeight():len);
}

};//namespace Useless
#endif//__INCLUDED_USELESS_SCROLLBAR_WIDGET_H__
