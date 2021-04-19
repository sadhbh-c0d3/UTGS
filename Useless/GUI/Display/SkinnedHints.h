#ifndef __INCLUDED__USELESS_SKINNED_HINTS_H__
#define __INCLUDED__USELESS_SKINNED_HINTS_H__

#include "Useless/GUI/Display/HintMan.h"
#include "Useless/GUI/Skins/SkinMixer.h"
#include "Useless/GUI/Text/FontSet.h"
#include "Useless/Util/Storage/AssocVector.h"

namespace Useless {

/*! Manages Hints creating them at show time, and
 *  deleting them at hide. Hints are created from
 *  mixed Skins in SkinMixer and text with assigned Font
 */
class USELESS_API SkinnedHints : public HintMan, public SkinMixer
{
public:
    virtual ~SkinnedHints();
    virtual void ShowHint( Widget *hint_owner );
    virtual void HideHint( Widget *hint_owner );
    virtual bool HasHint ( Widget *hint_owner );


    /* Simple hint information
    ---------------------------*/
    struct Hint
    {
        Hint(): _p_hint(0) {}
        Hint( int s, int f, int a, const Text &t)
            : _skin_no(s), _font_no(f), _alignment(a), _text(t), _p_hint(0) {}

        int     _skin_no;
        int     _font_no;
        int     _alignment;
        Text    _text;

        int     _position_flags;
        Pos     _position;
        Widget *_p_hint;
    };

    void  AddHint( Widget *hint_owner, const Hint &h );

    void  AddHint( Widget *hint_owner, int skin_no, int   font_no,    const Text &t);
    void  AddHint( Widget *hint_owner, int skin_no, const Font &font, const Text &t);

    void  AddHint( Widget *hint_owner, const Skin &skin, int   font_no,    const Text &t);
    void  AddHint( Widget *hint_owner, const Skin &skin, const Font &font, const Text &t);

    bool  RemoveHint( Widget *hint_owner);

    enum  PosFlags { GLOBAL, LOCAL, AT_POINTER };
    void  SetHintPosition( Widget *hint_owner, const Pos &p, int position_flags );

private:
    typedef AssocVector<Widget*, Hint> HintMap;

    HintMap     _hints;
    FontSet     _fonts;

    void PlugHint( Hint &hint, Widget *hint_owner );
    void KillHint( Hint &hint );
    Pos  GetHintPosition( const Hint &hint, Widget *hint_owner) const;
};



};//namespace Useless
#endif//__INCLUDED__USELESS_SKINNED_HINTS_H__
