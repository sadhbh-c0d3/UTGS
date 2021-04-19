#ifndef __INCLUDED__USELESS__LISTED_TEXT_ITEMS_H__
#define __INCLUDED__USELESS__LISTED_TEXT_ITEMS_H__

#include "Useless/GUI/Logic/MultiItemLogic.h"
#include "Useless/GUI/Misc/SkinSelector.h"
#include "Useless/GUI/Misc/FontSelector.h"
#include "Useless/GUI/Text/Text.h"
#include "Useless/Util/Storage/StupidVector.h"
#include "Useless/Util/CPT.h"

namespace Useless
{

class USELESS_API ListedTextItems : virtual public MultiItemLogic
{
public:

#include "ListedTextItems.inl" // ParmList Ctor

             ListedTextItems( const FontSet &fonts, const SkinMan &skins, int shift=2 );
    virtual ~ListedTextItems();

    virtual  int  GetState( int item_no )         const;
    virtual bool  SetState( int item_no, int state_no );
    virtual bool  HasState( int item_no, int state_no );

    virtual  int  ItemAtPosition( const Pos &pos) const;
    virtual  int  GetNumItems()                   const { return _items.Size(); }
    virtual Rect  GetItemBound( int item_no  )    const;

    virtual void  Repaint( const WidgetPainter &painter );

            void  Insert(const Text& text); //!< It's stupid insert at end
            void  Remove( int item_no );
            void  Remove(const Text& text); //!< Find and remove item with this text
            void  ClearEntries();

            Text  GetText( int item_no ) const;
            void  SetText( int item_no, const Text &text );
            void  SetText( const Text &prev_text, const Text &new_text );

              int GetItemNo( const Text &text) const;

             int  GetFontHeight () const;
             int  GetEntryHeight() const;
            void  SetEntryHeight( int h);
             int  GetDefaultEntryHeight() const;
           Dim2i  GetSelectorSize() const;
            void  SetSelectorSize( int w, int h=-1);

    enum { GEN_FOCUSSED=1, GEN_HIGHLIGHT=2 };
    void SetupSkin( int flags );
    
    void PaintArea( const Painter &painter, const Rect &area, int stateID);

    DEFINE_CYCLIC_VISITABLE(Widget);

private:
    SkinSelector _skins;
    FontSelector _fonts;

    struct __Item
    { 
         Text _caption;
         int  _state;
    };

    StupidVector<__Item>  _items;

        int  _entry_height;
      Dim2i  _selector_size;
        int  _shift;

             void CheckSize(const Text &text);

          __Item& GetItem   ( int item_no );
    const __Item& GetItem   ( int item_no )  const;

    int GetH() const { return GetEntryHeight()*GetNumItems(); }
};

/*
------------------------------------------------------------------*/
inline ListedTextItems::__Item& ListedTextItems::GetItem( int item_no )
{
    return _items[item_no];
}

inline const ListedTextItems::__Item& ListedTextItems::GetItem( int item_no ) const
{
    return _items[item_no];
}

inline Text ListedTextItems::GetText( int item_no ) const
{
    if ( item_no >=0 && item_no<GetNumItems() )  { return GetItem(item_no)._caption;  }
    else                                         { return Text(); }
}



};//namespace Useless
#endif//__INCLUDED__USELESS__LISTED_TEXT_ITEMS_H__
