#include "UselessPch.h"

#include "DisplayState.h"

namespace Useless {

DisplayState DisplayState::BG_substitute () const
{
    /* State substitution table
     *
     * Focussed  | Selected  | any -> Focussed | Selected
     * Focussed  | any             -> Focussed
     * Focussed                    -> Not Focussed
     * Selected  | Highliten | any -> Selected | any
     * Selected  | any             -> Selected
     * Selected                    -> Not Selected
     * Highliten | any             -> Highliten
     * Disabled  | any             -> Disabled
     * Highliten                   -> Normal
     */
    /*if ( Get(FOCUSSED) )
    {
        return CpClr(FOCUSSED);
    }
    else
        return NORMAL;*/
    //-----------------------------------------

    if ( Get(FOCUSSED) )
    {
        if ( Get(SELECTED) )
        {
            if ( CpClr(FOCUSSED|SELECTED) )
            {
                return FOCUSSED|SELECTED;
            }
            return SELECTED;
        }
        else
        {
            if ( CpClr(FOCUSSED) )
            {
                return FOCUSSED;
            }
        }
        return CpClr(FOCUSSED);
    }
    else if ( Get(SELECTED) )
    {
        if ( Get(HIGHLIGHTEN) )
        {
            return CpClr(HIGHLIGHTEN);
        }
        else if ( CpClr(SELECTED) )
        {
            return SELECTED;
        }
        else
        {
            return CpClr(SELECTED);
        }
    }
    else if ( Get(HIGHLIGHTEN) )
    {
        if ( CpClr(HIGHLIGHTEN) )
        {
            return HIGHLIGHTEN;
        }
    }
    else if ( Get(DISABLED) )
    {
        if ( CpClr(DISABLED) )
        {
            return DISABLED;
        }
    }
    return NORMAL;
}


DisplayState DisplayState::FG_substitute () const
{
    if ( Get(FOCUSSED) )
    {
        return CpClr(FOCUSSED);
    }
    return NORMAL;
}

};//namespace Useless

