#ifndef __INCLUDED_USELESS_SKIN_MAN_H__
#define __INCLUDED_USELESS_SKIN_MAN_H__

/*
 *    $Id: SkinMan.h,v 1.12 2003/01/20 22:00:31 koolas Exp $
 *
 *    NAME
 *        SkinMan
 *
 *    PURPOSE
 *        Manages skins
 *    
 *    AUTHOR(S)
 *        Sadhbh Code (https://github.com/sadhbh-c0d3)
 * 
*/

#include "Useless/Graphic/Planes/Painter.h"
#include "Useless/Util/CopyingFactory.h"

namespace Useless {

class Widget;

/*! \ingroup Skins
 *  Abstract class for skin manager.
 */
class USELESS_API SkinMan
{
public:
    virtual ~SkinMan() {}

    virtual  int Size     ()           const = 0;
    virtual bool Empty    ()           const = 0;
    virtual bool Exists   ( int id )   const = 0;
    
    virtual  void  Paint    ( int id, const Painter& painter, const Pos& = Pos() ) = 0;

    virtual   int  GetWidth    ( int id )             const = 0;
    virtual   int  GetHeight   ( int id )             const = 0;
    virtual  void  SetSizeAll  ( const Dim2i& d )           = 0;
    virtual Dim2i  GetMaxSize  ()                     const = 0;
    virtual  void  SetSize     ( int id, const Dim2i& d)    = 0;
    virtual   Pos  GetDirection( int id )             const = 0;
    virtual   Pos  GetDirMask  ( int id )             const = 0;
    virtual  void  SetDirMask  ( int id, const Pos & m)     = 0;

    virtual  bool  SafeResize  ( int id, const Dim2i& d);

    //! Some skins may have specific dimentions i.e. cell padding
    virtual  int   GetDimension( int id, int dimID ) const { return 0; }
    
    //! Set cooperation between widget and set of skins
    virtual void Cooperate( int id, Widget *client) = 0;
};

};//namespace Useless
#endif//__INCLUDED_USELESS_SKIN_MAN_H__
