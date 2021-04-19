#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSystem.h"
#include "Useless/XML/Resources/CreateSkins.h"
#include "Useless/XML/Resources/CreateFonts.h"
#include "Useless/XML/Resources/CreateText.h"

namespace Useless {

Resource* CreateHints( XMLIterator i, XMLCreatorEnv *env )
{
    std::string font = get_attribute( (*i)._attributes, "font" );
    std::string skin = get_attribute( (*i)._attributes, "skin" );
    
    std::string text_align;    
    try_attribute( (*i)._attributes, "text_align", &text_align );
    
    int text_alignment = 0;
    if (text_align == "left")
        text_alignment = 0;
    else
    if (text_align == "right")
        text_alignment = 1;
    else
    if (text_align == "center")
        text_alignment = 2;

    FrameSkin *p_skin = dynamic_cast<FrameSkin*>(&QuerySkin(skin));
    if (!p_skin)
        throw Error("Skin %s can\'t be used for Hints. FrameSkin required.", skin.c_str());

    SPointer<Hints> sp_hints = new Hints(QueryFont(font), *p_skin, text_alignment);
    
    double delay;
    if (try_attribute_r( (*i)._attributes, "delay", &delay ))
        sp_hints->SetDelay(delay);

    double fast_show;
    if (try_attribute_r( (*i)._attributes, "fast_show_timeout", &fast_show ))
        sp_hints->SetFastShow(fast_show);

    return new AnyResource< SPointer<Hints> >( sp_hints );
}

void SetHint( Widget *hint_owner, XMLAttributes attributes )
{
    std::string hint_manager;
    if ( !try_attribute( attributes, "hint_manager", &hint_manager ) )
        return;

    SPointer<Hints> *sp_hints;
    Resources::Instance().Query("system", hint_manager, sp_hints);

    std::string hint;
    Text hint_text;
    if (try_attribute( attributes, "hint", &hint ))
    {
        hint_text = MakeText( to_chars( hint ));
    }
    else
    if (try_attribute( attributes, "hint_link", &hint ))
    {
        hint_text = QueryText(hint);
    }
    else
    {
        return;
    }

    std::string hint_flags;
    int flags = -1;
    if (try_attribute( attributes, "hint_flags", &hint_flags ))
    {
        flags = 0;
        std::vector<std::string> flags_vector = to_tokens(hint_flags);
        for (int i = 0; i < flags_vector.size(); i++ )
        { 
            if (flags_vector[i] == "global")
                flags |= Hints::GLOBAL;
            else
            if (flags_vector[i] == "local")
                flags |= Hints::LOCAL;
            else
            if (flags_vector[i] == "at_pointer")
                flags |= Hints::AT_POINTER;
            else
            if (flags_vector[i] == "any_position")
                flags |= Hints::ANY_POSITION;
            else
            if (flags_vector[i] == "above")
                flags |= Hints::ABOVE;
            else
            if (flags_vector[i] == "below")
                flags |= Hints::BELOW;
            else
            if (flags_vector[i] == "left")
                flags |= Hints::LEFT;
            else
            if (flags_vector[i] == "right")
                flags |= Hints::RIGHT;
            else
            if (flags_vector[i] == "any_place")
                flags |= Hints::ANY_PLACE;
            else
            if (flags_vector[i] == "begin")
                flags |= Hints::BEGIN;
            else
            if (flags_vector[i] == "end")
                flags |= Hints::END;
            else
            if (flags_vector[i] == "center")
                flags |= Hints::CENTER;
            else
            if (flags_vector[i] == "any_align")
                flags |= Hints::ANY_ALIGN;
            else
            if (flags_vector[i] == "default_placement")
                flags |= Hints::DEFAULT_PLACEMENT;
            else
            if (flags_vector[i] == "default_alignment")
                flags |= Hints::DEFAULT_ALIGNMENT;
            else
            if (flags_vector[i] == "default")
                flags |= Hints::DEFAULT;
            else
            if (flags_vector[i] == "any_default")
                flags |= Hints::ANY_DEFAULT;
        }
    }

    std::string position;
    if (try_attribute( attributes, "hint_pos", &position ))
    {
        if (flags < 0)
            flags = 0;
        std::vector<int> pos = to_integer_vector( position );
        if ( pos.size() != 2 ) { throw Error("pos attribute must have x and y coordinates"); }

        (*sp_hints)->AddHint(hint_owner, hint_text, Pos(pos[0], pos[1]), flags);
    }
    else
    {
        if (flags < 0)
            flags = Hints::DEFAULT;
        (*sp_hints)->AddHint(hint_owner, hint_text, flags);
    }
}

};//namespace Useless
