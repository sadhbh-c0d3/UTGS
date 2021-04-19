#include "UselessPch.h"

#include "Useless/GUI/Display/Hints.h"
#include "Useless/GUI/Workspace.h"
#include "Useless/GUI/Display/MouseCursor.h"
#include "Useless/ErrorConfig.h"

namespace Useless {
//@todo: SkinnedHints have very similar code - unification of code

Hints::Hints( const Font &font, const FrameSkin &skin, int text_alignment )
    : _font(font), _sp_skin(&CopyOf<Skin>(skin)), _text_alignment(text_alignment),
      _delay(0.75), _fast_show_timeout(0.15), _fast_show_flag(0)
{}

Hints::~Hints()
{
    HintMap::Iterator it_hint = _hints.Begin();
    for (; !!it_hint; ++it_hint )  { KillHint( *it_hint ); }
}

void Hints::ShowHint( Widget *hint_owner )
{
    Hint *p; if ( p=_hints.Find(hint_owner) )
    {
        if ( _delay == 0.0 || _fast_show_flag )
        {
            ShowHintImpl( *p, hint_owner );
        }
        else
        {
            p->_show_at_time.Free();
            p->_show_at_time = OnTimer::Instance().Schedule(
                    PackFunctor( BinaryCall( this, &Hints::ShowHintImpl), PackDeference2Holder(p), hint_owner),
                    _delay);
        }
    }
}

void Hints::FastShowTimeout()
{
    _fast_show_flag = 0;
    _fast_show.Free();
}

void Hints::ShowHintImpl( Hint &hint, Widget *hint_owner )
{
    assert(!!hint_owner);

    if ( ! hint._p_hint )   // Create hint
    {
        hint._p_hint = new SmallHint( _font, *_sp_skin, hint._text, _text_alignment );
        hint._p_hint->FitText();
        PlugHint( hint, hint_owner );
    }
    else
    {
        hint._p_hint->Show();
    }

    if ( _fast_show_timeout > 0 )
    {
        _fast_show_flag = true;
        _fast_show.Free();
    }
}

void Hints::HideHint( Widget *hint_owner )
{
    assert(!!hint_owner);

    Hint *p = _hints.Find( hint_owner );
    if (p) { KillHint(*p); }
}

bool Hints::HasHint( Widget *hint_owner )
{
    return !!_hints.Find(hint_owner);
}

void Hints::AddHint( Widget *hint_owner, const Text &hint_text, 
                     const Pos &pos, int position_flags )
{
    RemoveHint(hint_owner);
    Hint hint;
    hint._text = hint_text;
    hint._position = pos;
    hint._position_flags = (!position_flags)? ( (!pos)? AT_POINTER : GLOBAL ) : position_flags;
    _hints.Insert(hint_owner, hint);
    
}

void Hints::AddHint( Widget *hint_owner, const Text &hint_text, int flags )
{
    RemoveHint(hint_owner);
    Hint hint;
    hint._text = hint_text;
    hint._position_flags = LOCAL | flags;
    _hints.Insert(hint_owner, hint);
    
}


bool Hints::RemoveHint( Widget *hint_owner )
{
    if ( Hint *p = _hints.Find(hint_owner) )
    {
        KillHint(*p);
        _hints.Remove(hint_owner);
        return true;
    }
    else
        return false;
}

Pos  Hints::GetHintPosition( const Hint &hint, Widget *hint_owner) const
{
    const WidgetEnvironment *penv = hint._p_hint->GetEnvironment();
    const Workspace& workspace = penv->GetWorkspace();
    Rect c_abs  = workspace.GetCursor().GetRect() + penv->GetWorkspace().WhereIsPointer();
    Rect cursor = hint_owner->GetRelativeRect( c_abs );
    int space_w = workspace.GetWidth();
    int space_h = workspace.GetHeight();
    int hint_w  = hint._p_hint->GetWidth();
    int hint_h  = hint._p_hint->GetHeight();
    int owner_w = hint_owner->GetWidth();
    int owner_h = hint_owner->GetHeight();

    Pos l( space_w-hint_w, space_h-hint_h );
	Pos position;

	int position_flags = hint._position_flags & ANY_POSITION;
	int placement_flags = hint._position_flags & ANY_PLACE;
	int alignment_flags = hint._position_flags & ANY_ALIGN;
    int defaults = hint._position_flags & ANY_DEFAULT;
    if ( defaults & DEFAULT_ALIGNMENT ) { alignment_flags = END; }
    if ( defaults & DEFAULT_PLACEMENT ) 
     { placement_flags = ( hint_owner->GetAbsolutePos(Pos()).y < space_h*0.8 )? BELOW : ABOVE; }


    for ( int i=2; i>0; --i )
    {
        switch( position_flags )
        {
            case GLOBAL:
                return hint._position;

            case LOCAL:
		    {
			    if ( !!hint._position ) return hint_owner->GetAbsolutePos( hint._position );

			    switch ( placement_flags )
			    {
				    case ABOVE: position.y = -hint_h; break;
				    case BELOW: position.y =  hint_h; break;
				    case LEFT:  position.x = -hint_w; break;
				    case RIGHT: position.x =  hint_w; break;
			    }
		
                switch ( placement_flags )
			    {
				    case ABOVE:
				    switch ( alignment_flags )
				    {
					    case BEGIN:	 position.x = 0; break;
					    case END:    position.x = owner_w-hint_w; break;
					    case CENTER: position.x =(owner_h-hint_w)/2; break;
				    }
                    break;

				    case BELOW:
                    {
				        switch ( alignment_flags )
				        {
                            case BEGIN:
                            {
                                position.x = 0;
                                if ( cursor.GetY2()-owner_h/4>position.y && cursor.GetX1()<position.x )
                                {
                                    if ( hint_w > 2*owner_w || cursor.GetX2()>(owner_w*3)/4 )
                                        position.y = cursor.GetY2() - owner_h/4;
                                    else
                                        if ( cursor.GetX2()>owner_w/2 )
                                             position.x = owner_w/2;
                                        else position.x = (owner_w*3)/4;
                                }
                            }
                            break;

                            case END:
                            {
                                position.x = owner_w-hint_w;
                                if ( cursor.GetY2()-owner_h/4>position.y && cursor.GetX2()>position.x )
                                {
                                    if ( hint_w > 2*owner_w || cursor.GetX1()<owner_w/4 )
                                        position.y = cursor.GetY2() - owner_h/4;
                                    else
                                        if ( cursor.GetX1()>owner_w/2 )
                                             position.x = owner_w/2-hint_w;
                                        else position.x = owner_w/4-hint_w;
                                }
                            }
                            break;
                        }
                    }
				    break;

				    case LEFT: 
				    case RIGHT:
				    switch ( alignment_flags )
				    {
					    case BEGIN:	 position.y = 0; break;
					    case END:    position.y = owner_h-hint_h; break;
					    case CENTER: position.y =(owner_h-hint_h)/2; break;
				    }
                    break;
			    }
		    }
            break;

            case AT_POINTER:
            {   
                switch( placement_flags )
                {
                    case ABOVE:  position = Pos( cursor.GetX1(), cursor.GetY1()-hint_h ); break;
                    case BELOW:  position = Pos( cursor.GetX1(), cursor.GetY2() ); break;
                }
            }
            break;
        }
        position = hint_owner->GetAbsolutePos( position );
        if ( position.y < 0 && placement_flags==ABOVE ) { placement_flags = BELOW; }
        else if ( position.y > l.y && placement_flags==BELOW ) { placement_flags = ABOVE; }
        else if ( position.x < 0 && placement_flags==LEFT  ) { placement_flags = RIGHT; }
        else if ( position.x > l.x && placement_flags==RIGHT  ) { placement_flags = LEFT; }

        else if ( (position.x < 0 && hint_w>c_abs.GetX1() || 
                   position.x > l.x && l.x-hint_w<c_abs.GetX2()) && placement_flags==BELOW  ) 
        {
            position.y = c_abs.GetY2() - owner_h/4; 
        }
        else break;
    }

	return position;
}

void Hints::SetHintPosition( Widget *hint_owner, const Pos &pos, int position_flags )
{
    if ( Hint *p = _hints.Find( hint_owner ) )
    {
        p->_position_flags = position_flags;
        p->_position = pos;
        if (p->_p_hint) { p->_p_hint->SetPosition( GetHintPosition(*p,hint_owner) ); }
    }
}

void Hints::KillHint( Hint &hint )
{
    hint._show_at_time.Free();

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

    if ( _fast_show_timeout > 0 )
    {
        _fast_show.Free();
        _fast_show = OnTimer::Instance().Schedule(
            this, &Hints::FastShowTimeout, _fast_show_timeout);
    }
}


void Hints::PlugHint( Hint &hint, Widget *hint_owner )
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
