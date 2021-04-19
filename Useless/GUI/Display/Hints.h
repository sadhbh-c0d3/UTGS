#ifndef __INCLUDED__USELESS_HINTS_H__
#define __INCLUDED__USELESS_HINTS_H__

#include "Useless/GUI/Display/HintMan.h"
#include "Useless/GUI/Text/Font.h"
#include "Useless/GUI/Widgets/SmallHint.h"
#include "Useless/Util/Storage/AssocVector.h"
#include "Useless/Functional/DelayedExecutor.h"

namespace Useless {

/*! Simple easy to use HintMan.
 *  Hints compound of skin and aligned text
 *  Uses SmallHint widgets to show hints.
 */
class USELESS_API Hints : public HintMan
{
public:
    Hints( const Font &font, const FrameSkin &skin, int text_alignment=0 );
    virtual ~Hints();

    virtual void ShowHint( Widget *hint_owner );
    virtual void HideHint( Widget *hint_owner );
    virtual bool HasHint ( Widget *hint_owner );


    enum  PosFlags { GLOBAL=1, LOCAL=2, AT_POINTER=3, ANY_POSITION=0xF };
    void  SetHintPosition( Widget *hint_owner, const Pos &p, int position_flags );
    
    /* 'hint_owner' 'hint_text' are nessesary parameters
     * if nothing else is given, then hint will appear AT_POINTER, else
     * if 'pos' is given then it will appear at this GLOBAL position
     */
    void AddHint( Widget *hint_owner, const Text &hint_text, 
                  const Pos &pos/*=Pos()*/, int position_flags=0 );

	enum PlacementFlags { ABOVE=0<<4, BELOW=1<<4, LEFT=2<<4, RIGHT=3<<4, ANY_PLACE=0x30 };
	enum AlignemntFlags { BEGIN=0<<6, END=1<<6, CENTER=2<<6, ANY_ALIGN=0xC0 };
    enum DefaultFlags   { DEFAULT_PLACEMENT=1<<8, DEFAULT_ALIGNMENT=2<<8, DEFAULT=3<<8, ANY_DEFAULT=0x300 };

	/* Use combination of 'PlacementFlags' and 'AlignemntFlags' to tell whether
	 * you ant hint to be displayed above, below, to the left or to the right of the widget,
	 * and with aligned first corners, last corners or centered. Default hint placement is
	 * above widget and with aligned first corners (here left bounds)
	 */
    void AddHint( Widget *hint_owner, const Text &hint_text, int flags = DEFAULT );

    /* If 'hint_owner' has hint in here, then it removes this hint
     * and returns 'true' else it returns 'false'
     */
    bool RemoveHint( Widget *hint_owner );

    void SetDelay( double delay ) { _delay=delay; }
    void SetFastShow( double timeout ) { _fast_show_timeout=timeout; }


private:
    struct Hint
    {
        Hint(): _p_hint(0) {}

        Text        _text;
        int         _position_flags;
        Pos         _position;
        SmallHint  *_p_hint;
        
        DelayedExecutor::TaskID _show_at_time;
    };
    typedef AssocVector< Widget*, Hint > HintMap;
    HintMap _hints;

    SPointer<Skin>         _sp_skin;
    Font                   _font;
    int                    _text_alignment;

    void PlugHint( Hint &hint, Widget *hint_owner );
    void KillHint( Hint &hint );
    Pos  GetHintPosition( const Hint &hint, Widget *hint_owner) const;

    void ShowHintImpl( Hint &hint, Widget *hint_owner );

    double _delay;
    
    double _fast_show_timeout;
    bool   _fast_show_flag;

    DelayedExecutor::TaskID _fast_show;
    void FastShowTimeout();
};

};//namespace Useless
#endif//__INCLUDED__USELESS_HINTS_H__
