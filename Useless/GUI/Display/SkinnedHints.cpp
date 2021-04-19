#include "UselessPch.h"

#include "Useless/GUI/Display/SkinnedHints.h"
#include "Useless/GUI/Workspace.h"
#include "Useless/GUI/Widgets/SkinManagedWidget.h"
#include "Useless/GUI/Skins/CaptionSkin.h"

//warning C4311: 'type cast' : pointer truncation from 'Useless::Widget *' to 'int'
#pragma warning(disable:4311)

namespace Useless {
//@todo: SkinnedHints have very similar code - unification of code

SkinnedHints::~SkinnedHints()
{
    HintMap::Iterator it_hint = _hints.Begin();
    for (; !!it_hint; ++it_hint )  { KillHint( *it_hint ); }
}

void SkinnedHints::ShowHint( Widget *hint_owner )
{
    assert(!!hint_owner);

    Hint *p; if ( p=_hints.Find(hint_owner) )
    {
        if ( ! (*p)._p_hint )   // Create hint
        {
            p->_p_hint = new SkinManagedWidget( p->_skin_no, *this );
            PlugHint( *p, hint_owner );
        }
        else { p->_p_hint->Show(); }
    }
}

void SkinnedHints::HideHint( Widget *hint_owner )
{
    assert(!!hint_owner);

    Hint *p = _hints.Find( hint_owner );
    if (p) { KillHint(*p); }
}

bool SkinnedHints::HasHint( Widget *hint_owner )
{
    return !!_hints.Find(hint_owner);
}

Pos  SkinnedHints::GetHintPosition( const Hint &hint, Widget *hint_owner) const
{
    switch( hint._position_flags )
    {
        case GLOBAL:
            return hint._position;

        case LOCAL:
            return hint_owner->GetAbsolutePos( hint._position );

        case AT_POINTER:
        {
            const WidgetEnvironment *penv = hint._p_hint->GetEnvironment();
            if ( penv ) { return penv->GetWorkspace().WhereIsPointer(); }
        }
    }
    return Pos();
}

void SkinnedHints::SetHintPosition( Widget *hint_owner, const Pos &pos, int position_flags )
{
    Hint *p = _hints.Find( hint_owner );
    if (p)
    {
        p->_position_flags = position_flags;
        p->_position = pos;
        if (p->_p_hint)
        {
            const Pos &hint_p = GetHintPosition(*p,hint_owner);
            p->_p_hint->SetPosition( hint_p);
        }
    }
}


void SkinnedHints::AddHint( Widget *hint_owner, const Hint &h )
{
    SkinMixer::Mix( (int)hint_owner,
        CaptionSkin( h._text, _fonts.GetFont(h._font_no) ),
        h._alignment );
        
    RemoveHint(hint_owner);
    _hints.Insert( hint_owner, h);
}

void SkinnedHints::AddHint( Widget *hint_owner, int skin_no, int   font_no,    const Text &t)
{
    Hint hint( skin_no,
        font_no, Rect::HALIGNCENTER|Rect::VALIGNCENTER,
        t);
        
    AddHint( hint_owner, hint);
}

void SkinnedHints::AddHint( Widget *hint_owner, int skin_no, const Font &font, const Text &t)
{
    AddHint( hint_owner, skin_no, _fonts.Insert(font), t );
}

void SkinnedHints::AddHint( Widget *hint_owner, const Skin &skin, int   font_no,    const Text &t)
{
    SkinMixer::Mix( (int)hint_owner, skin );
    AddHint( hint_owner, (int)hint_owner, font_no, t);
}

void SkinnedHints::AddHint( Widget *hint_owner, const Skin &skin, const Font &font, const Text &t)
{
    SkinMixer::Mix( (int)hint_owner, skin );
    AddHint( hint_owner, (int)hint_owner, _fonts.Insert(font), t );
}


bool SkinnedHints::RemoveHint( Widget *hint_owner)
{
    HintMap::ConstIterator it_hint; if ( _hints.Find(hint_owner, &it_hint) )
    {
        KillHint( (Hint&)(*it_hint) );
        _hints.RemoveAt(it_hint);
        return true;
    }
    else
        return false;
}


/*! Check if hint were created, then
 *  check if Workspace owns this hint 
 *  eventually delete or remove this hint 
 */
void SkinnedHints::KillHint( Hint &hint )
{
    if ( hint._p_hint )
    {
        const WidgetEnvironment *penv;
        
        if ( (penv=hint._p_hint->GetEnvironment()) 
        &&   (penv->GetWorkspace().InWhichLayer( hint._p_hint )!=Workspace::NO_LAYER) )
        {
            penv->GetWorkspace().Remove( hint._p_hint );
        }
        else { delete hint._p_hint; }
        hint._p_hint = NULL;
    }
}

void SkinnedHints::PlugHint( Hint &hint, Widget *hint_owner )
{
    if ( const WidgetEnvironment *penv= hint_owner->GetEnvironment() )
    {
        if ( penv->GetWorkspace().InWhichLayer( hint._p_hint )==Workspace::NO_LAYER )
        {
            penv->GetWorkspace().AddHint( hint._p_hint, hint._position );
            penv->GetWorkspace().ShowAtPosition( GetHintPosition(hint,hint_owner), hint._p_hint );
        }
    }
}


};//namespace Useless
