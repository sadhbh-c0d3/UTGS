#ifndef __INCLUDED__USELESS__LIST_WIDGET_H__
#define __INCLUDED__USELESS__LIST_WIDGET_H__

#include "Useless/GUI/Widgets/ListedTextItems.h"
#include "Useless/GUI/Logic/ListLogic.h"
#include "Useless/GUI/Skins/SkinSet.h"

namespace Useless {

//USES
    class ListStyle;
    class ListStyle2;


/*! \ingroup Widgets
 *  ListWidget is combined from ListedTextItems and ListLogic.
 *  Compatibility with version 1.0 preserved.
 *  v. 2.0
 */
class USELESS_API ListWidget : public ListedTextItems, virtual public ListLogic, public IWList
{
public:
             ListWidget( const FontSet &fonts, const SkinMan &skins, int shift=2 )
                                         :ListedTextItems( fonts, skins, shift) {}
             ListWidget( const ListStyle2 &style );

     virtual~ListWidget() {}

#    include "ListWidget.inl" // ParmList Ctor

     virtual ListWidget&   Items()      { return *this; }
     virtual ActiveWidget& Control()    { return *this; }
     virtual EnumWidget&   Enumerator() { return *this; }

             ListWidget& operator <<(const Text& s) { Insert(s); return *this; }
             ListWidget& operator  ,(const Text& s) { Insert(s); return *this; }


    /* Obsolete ctor
    ---------------------------------------------------------------------------*/
             ListWidget( const Font &normal,
                         const Font &highliten,
                         const Skin &selector_skin,
                                int  shift=4
                        );

             ListWidget( const ListStyle &style );


    /*! Obsolete interface
    ------------------------------------------------------------------*/
             int GetNumEntries() const { return GetNumItems(); }
            Text GetText( int item_no=-1 ) const; // -1 is selected one

    static FontSet CreateFontSet( const Font &n, const Font &hl );
    static SkinSet CreateSkinSet( const Skin &sk);

};




/*! \class ListStyle2
 *  designed for ListWidget v. 2.0
-------------------------------------------------------------------*/
class ListStyle2
{
public:
    ListStyle2( const FontSet &fonts,
                const SkinMan &skins,
                          int  selector_shift=3

              ): fonts(fonts), skins(skins), shift(selector_shift) {}

    const FontSet &fonts;
    const SkinMan &skins;
              int  shift;
};



/*! \class ListStyle
 *  designed for ListWidget v. 1.0
-------------------------------------------------------------------*/
class ListStyle
{
public:
    ListStyle( const Font& normal,
               const Font& highliten,
               const Skin &selector_skin,
                 int selector_shift=8

             ): normal(normal), highliten(highliten), selector_skin(selector_skin), shift(selector_shift) {}

    const  Font  &normal;
    const  Font  &highliten;
    const  Skin  &selector_skin;
            int  shift;
};


};//namespace Useless
#endif//__INCLUDED__USELESS__LIST_WIDGET_H__
