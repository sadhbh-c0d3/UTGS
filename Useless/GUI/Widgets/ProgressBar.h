#ifndef __INCLUDED__USELESS_PROGRESS_BAR_H__
#define __INCLUDED__USELESS_PROGRESS_BAR_H__

#include "Useless/GUI/Skins/Skin.h"
#include "Useless/GUI/Widgets/ProgressWidget.h"

namespace Useless {

class USELESS_API ProgressBar : virtual public ProgressWidget
{
public:
    ProgressBar( int range, int size, const Skin &frame, const Skin &interrior, Orientation orient=HORIZONTAL );
    ~ProgressBar();

    virtual void Repaint( const WidgetPainter &painter );
    virtual void Resize ( int w, int h );
    virtual int  GetSize  () const;
  
private:
    SPointer<Skin>  _sp_frame;
    SPointer<Skin>  _sp_interrior;
    bool            _horizontal;
};

};//namespace Useless
#endif//__INCLUDED__USELESS_PROGRESS_BAR_H__
