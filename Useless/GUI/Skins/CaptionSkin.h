#ifndef __INCLUDED__USELESS_CAPTION_SKIN_H__
#define __INCLUDED__USELESS_CAPTION_SKIN_H__

#include "Useless/GUI/Skins/Skin.h"
#include "Useless/GUI/Text/Font.h"
#include "Useless/GUI/Text/Text.h"

namespace Useless {

class USELESS_API CaptionSkin : public Skin
{
public:
    CaptionSkin() {}
    CaptionSkin( const Text &caption, const Font &font );
    virtual ~CaptionSkin();

    DEFINE_CYCLIC_VISITABLE(Skin);

    virtual  void  Paint    ( const Painter& painter, const Pos& =Pos() );

    virtual   int  GetWidth ()             const { return _font.GetWidth(_caption); }
    virtual   int  GetHeight()             const { return _font.GetHeight(); }
    virtual  void  SetSize  ( const Dim2i &d)  { assert("CaptionSkin is not resizeable. Only text size matters"); }
    virtual   Pos  GetDirection()        const { return Pos(); }
    virtual  bool        IsColorKeyed() const { return _font.GetSkin().IsColorKeyed(); }
    virtual  NormalPixel GetColorKey()  const { return _font.GetSkin().GetColorKey(); }
 
             void  SetCaption(const Text &caption) { _caption=caption; }
             Text  GetCaption()              const { return _caption; }
             
private:
    Font _font;
    Text _caption;
};

    
};// namespace Useless


#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,Skin,CaptionSkin);

#endif//__INCLUDED__USELESS_CAPTION_SKIN_H__
