#ifndef __INCLUDED_USELESS_GUI_SKINS_IMAGEDICER_H__
#define __INCLUDED_USELESS_GUI_SKINS_IMAGEDICER_H__

/*
**  $Id: ImageDicer.h,v 1.19 2003/01/20 22:00:31 koolas Exp $
**
**  NAME
**      ImageDicer 
**
**  PURPOSE
**      Dices an Image to rectangular pieces.
**    
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
**      Jacek Trzmiel (jacekt@iavision.com)
*/

#include "Useless/GUI/Skins/SkinMan.h"
#include "Useless/Graphic/Planes/Image.h"
#include "Useless/Graphic/Rect.h"
#include "Useless/Graphic/RectList.h"
#include "Useless/Util/Storage/AutoVector.h"
#include "Useless/GUI/Misc/DisplayState.h"
#include "Useless/Util/CtorWrapper.h"

#include <string>

namespace Useless {

struct ButtonImages;
struct SwitchImages;

/*! \ingroup Skins
 *  Class specialized in dicing image.
 */
class USELESS_API ImageDicer : public SkinMan
{
public:
    enum{ SOURCE = -1 };
    typedef CtorWrapper< IGraphics, Image, const char * > RefImage;

    ImageDicer() {}
    ImageDicer( const ImageDicer &dicer );
    ImageDicer( const RefImage &ref_image );
    ImageDicer( const RefImage &ref_image, int columns, int rows=1 );
    ImageDicer( const RefImage &ref_image, int columns, int rows, std::vector<int> mapping );
    ImageDicer( const RefImage &ref_image, const RectList &rect_list );
    ImageDicer( const RefImage &ref_image, const RectList &rect_list, std::vector<int> mapping );

    virtual ~ImageDicer();

             int     Dice       (const Rect &rect, int columns=1, int rows=1);
             int     Dice       (const RectList &rect_list);
            void     Dice       (const AssocVector<int, Rect> &assoc_rects);

          IGraphics& GetImage   (int id);
    const IGraphics& GetImage   (int id) const;
    bool             IsColorKeyed()      const;
    NormalPixel      GetColorKey()       const;
    void             SetColorKey(unsigned int c);
    void             SetNoColorKey();

    //! Set cooperation between widget and set of skins
    void Cooperate( int id, Widget *client);

    ImageDicer& Remap( std::vector<int> mapping );

    /////////////////////////////////////////////////////////////////////////////////////////
    //  OVERLOADS
    virtual  void  Paint       ( int id, const Painter& painter, const Pos& = Pos() );

    virtual   int  Size        ()           const        { return _images.Size();      }
    virtual  bool  Empty       ()           const        { return _images.Empty();     }
    virtual  bool  Exists      ( int id )   const        { return _images.Find(id); }
    virtual   int  GetWidth    ( int id )   const        { return GetImage(id).GetWidth();  }
    virtual   int  GetHeight   ( int id )   const        { return GetImage(id).GetHeight();  }
    virtual   Pos  GetDirection( int id )   const        { return Pos(0,0); }
    virtual  void  SetSize     ( int id, const Dim2i& d) { assert(!"ImageDicer::SetSize(): Operation not allowed"); }
    virtual   Pos  GetDirMask  ( int id )             const { return Pos(0,0); }
    virtual  void  SetDirMask  ( int id, const Pos & m)     { assert(!"ImageDicer::SetDirMask(): Operation not allowed"); }
    virtual  void  SetSizeAll  ( const Dim2i &d)            { assert(!"ImageDicer::SetSizeAll(): Operation not allowed"); }
    virtual Dim2i  GetMaxSize  ()           const;


private:
    // It is intended, that I define AssocVector, not AutoVector
    typedef SPointer<IGraphics>                 ImageType;
    typedef AssocVector<int, ImageType>         ImageMap;
            AutoVector< SPointer<IGraphics> >  _images;

    void Insert( ImageType image ) { _images.Insert( image ); }
    void SetSource( ImageType image ) { _images.GetMap().Insert( SOURCE, image ); }

    int DiceInit( const RefImage &ref_image, int columns, int rows );
};


//-------------------------------------------------------------------------------------------------------
inline IGraphics& ImageDicer::GetImage(int id)
{
    ImageType *p = _images.Find(id);
    if (!p) { return *_images[SOURCE]; }
    return **p; 
}

inline const IGraphics& ImageDicer::GetImage(int id) const 
{
    ImageType *p = _images.Find(id);
    if (!p) { return *_images[SOURCE]; }
    return **p; 
}


};//namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,SkinMan,ImageDicer);

#endif /__INCLUDED_USELESS_GUI_SKINS_IMAGEDICER_H__

