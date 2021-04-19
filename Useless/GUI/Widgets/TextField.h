#ifndef __INCLUDED__USELESS__TEXT_FIELD_H__
#define __INCLUDED__USELESS__TEXT_FIELD_H__

#include "Useless/GUI/Logic/TextEditLogic.h"
#include "Useless/GUI/Text/FontSet.h"
#include "Useless/GUI/Skins/SkinMan.h"

namespace Useless {

class USELESS_API TextField : virtual public TextEditLogic
{
public:
    TextField( int w, int h, const FontSet &fonts, const SkinMan &skins, const Text &text );
    virtual ~TextField();

    enum FontFaces { NORMAL=1, FOCUSSED=2, DISABLED=3, SELECTED=4 };
    enum CarCodes  { BEGIN=1, END=2 };
    enum Alignment { LEFT, RIGHT };

    //-- Widget overloads
    virtual void Repaint( const WidgetPainter& );
    virtual void Resize ( int w, int h );

    //-- TextEditLogic overloads
    virtual bool SetState( int );
    virtual bool HasState( int ) const;
    virtual int  GetState() const;
    virtual void ApplyMotion( int );
    virtual void ApplyChange();
    virtual int  GetLetterAt( const Pos& ) const;
    virtual bool Validate( int &ch ) const;

    void SetAlignment( Alignment a ) { _alignment=a; SetDirty(); }
    void SetLeftMargin ( int d)  { _frame_left = d; SetDirty(); }
    void SetRightMargin( int d)  { _frame_right = d; SetDirty(); }

    int GetOrigin() const;

private:
    FontSet          _fonts;
    SPointer< SkinMan > _sp_skins;
    int              _status;

    //-- View information
    int _text_shift;
    int _frame_left;
    int _frame_right;

    //-- Dimensions information cache
    Font::Letters _letters;
    int           _begin_marker;
    int           _end_marker;
    int           _marker_width;
    int           _alignment;

    //-- Internal helper inlines
          Font& GetFont( int );
    const Font& GetFont( int ) const;
       SkinMan& GetSkinMan();
            int GetSkinID( int ) const;

    //-- Blink
    void Blink();
    bool _blink;
};



/* Helpers
-------------------------------------------------------*/
inline Font& TextField::GetFont( int id )
{
    if ( _fonts.Exists(id) ) return _fonts.GetFont(id); else
    if ( _fonts.Exists(NORMAL) ) return _fonts.GetFont(NORMAL); else
    return _fonts.GetFont();
}

inline const Font& TextField::GetFont( int id ) const
{
    if ( _fonts.Exists(id) ) return _fonts.GetFont(id); else
    if ( _fonts.Exists(NORMAL) ) return _fonts.GetFont(NORMAL); else
    return _fonts.GetFont();
}

inline int TextField::GetSkinID( int id ) const
{
    if ( _sp_skins->Exists(id) ) { return id; }
    else if ( _sp_skins->Exists(NORMAL) ) { return NORMAL; }
    else return 0;
}

inline SkinMan& TextField::GetSkinMan()
{
    return *_sp_skins;
}

};//namespace Useless
#endif//__INCLUDED__USELESS__TEXT_FIELD_H__
