#ifndef __INCLUDED_USELESS_ROLLER_WIDGET_H__
#define __INCLUDED_USELESS_ROLLER_WIDGET_H__

#include "Useless/GUI/Widgets/Widget.h"
#include "Useless/GUI/Widgets/HubWidget.h"
#include "Useless/Graphic/Planes/Image.h"
#include "Useless/Util/CtorWrapper.h"

namespace Useless {

/*! Vertical ruller
 */
class USELESS_API RollerWidget : public Widget
{
public:
    typedef CtorWrapper< ImageBase, Image, const char * > RefImage;
    
    RollerWidget( const RefImage &image, int range );
    ~RollerWidget();

    virtual void Repaint( const WidgetPainter& painter );
    virtual void Resize ( int w, int h );

    void SetValue( int value, int rollDir );
    void SetRollSpeed( float speed );
    int  GetValue() const { return _value; }
    float GetRollSpeed() const { return _rollSpeed; }
    
    Signal OnOverRoll;
    Signal OnUnderRoll;

private:
    int _range;
    int _value;
    int _offset;
    float _currentValue;
    float _rollSpeed;
    float _rollDir;
    SPointer< ImageBase > _sp_image;

    void Tick( int T, int DT );

};

/*! ScoreRoller combines few rollers, with range 0..9
 */
class USELESS_API ScoreRoller : public HubWidget
{
public:
    ScoreRoller( const RollerWidget::RefImage &image, int numRollers );
    ~ScoreRoller();

    void SetScore( int score );
    int GetScore() { return _score; }
    RollerWidget *GetRoller( int rollerNo );
    
    virtual void Resize ( int w, int h );
    
    void SetRollSpeed( float speed );
    float GetRollSpeed() const { return _rollSpeed; }

private:
    int _score;
    float _rollSpeed;
    std::vector< RollerWidget *> _rollers;

    void OverRoll( int rollerNo );
    void UnderRoll( int rollerNo );
};

}; //Useless

#endif//__INCLUDED_USELESS_ROLLER_WIDGET_H__
