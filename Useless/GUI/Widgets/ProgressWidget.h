#ifndef __INCLUDED__USELESS_PROGRESS_WWIDGET_H__
#define __INCLUDED__USELESS_PROGRESS_WWIDGET_H__

#include "Useless/GUI/Widgets/Widget.h"

namespace Useless {

class USELESS_API ProgressWidget : virtual public Widget
{
public:
    ProgressWidget( int range ) : _range(range), _advance(0) {}
    virtual ~ProgressWidget() {}

    void Advance( int delta ) { _advance += delta; if (_advance>_range) { _advance=_range; } SetDirty(); }
    void SetAdvance( int advance ) { assert(advance<=_range); _advance=advance; SetDirty(); }
    void SetRange( int range ) { _range = range; }

    virtual int GetSize() const = 0;

protected:
    int GetPixelAdvance() const
    {
        return (_advance * GetSize() )/_range;
    }

private:
    int _range;
    int _advance;
};

};//namespace Useless
#endif//__INCLUDED__USELESS_PROGRESS_WWIDGET_H__
