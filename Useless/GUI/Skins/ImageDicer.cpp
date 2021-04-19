/*
**  $Id: ImageDicer.cpp,v 1.20 2003/01/20 22:00:31 koolas Exp $
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

#include "UselessPch.h"

#include "Useless/GUI/Skins/ImageDicer.h"
#include "Useless/GUI/Widgets/Widget.h"

namespace Useless {


ImageDicer::~ImageDicer()
{}


ImageDicer::ImageDicer( const ImageDicer &dicer )
{
    _images = dicer._images;
}


ImageDicer::ImageDicer( const RefImage &ref_image )
{
    SetSource( ref_image.get().Copy() );
}    


ImageDicer::ImageDicer( const RefImage &ref_image, int columns, int rows )
{
    DiceInit( ref_image.get(), columns, rows );
}

ImageDicer::ImageDicer( const RefImage &ref_image, int columns, int rows, std::vector<int> mapping )
{
    DiceInit( ref_image.get(), columns, rows );
    Remap(mapping);
}

ImageDicer::ImageDicer( const RefImage &ref_image, const RectList &rect_list )
{
    SetSource( ref_image.get().Copy() );
    Dice( rect_list );
}

ImageDicer::ImageDicer( const RefImage &ref_image, const RectList &rect_list, std::vector<int> mapping )
{
    SetSource( ref_image.get().Copy() );
    Dice( rect_list );
    Remap(mapping);
}


int ImageDicer::DiceInit( const RefImage &ref_image, int columns, int rows )
{
    SetSource( ref_image.get().Copy() );
    Rect rect = ref_image.get().GetRect();
    assert( ((rect.w % columns) == 0) && "Image width must be divisible by number of columns." );
    assert( ((rect.h % rows   ) == 0) && "Image height must be divisible by number of rows." );
    rect.x=0;
    rect.y=0;
    rect.w /= columns;
    rect.h /= rows;
    return Dice( rect, columns, rows );
}


int ImageDicer::Dice(const Rect &rect, int columns, int rows)
{
    if (!rect) return -1;

    int i,j,w,h;
    w=rect.GetW();
    h=rect.GetH();
    const IGraphics &ref_img = GetImage(SOURCE);
    Rect R = ref_img.GetRect();
    R.SetXY(0,0);

    for( i=0; i!=rows; ++i)
    {
        Rect r = rect+Pos(0,h*i);
        for( j=0; j!=columns; ++j)
        {
            if ( r && R )
            {
                Insert( ref_img.Copy(r) );
                r += Pos(w,0);
            }
        }
    }
    return 0;
}

int ImageDicer::Dice(const RectList &rect_list)
{
    if ( rect_list.empty() ) return -1;

    int i,n,id;
    id=Size();
    n=rect_list.size();
    const IGraphics &ref_img = GetImage(SOURCE);
    Rect R = ref_img.GetRect();
    R.SetXY(0,0);

    for(i=0; i!=n; ++i)
    {
        if ( rect_list[i] && R )
        {
            Insert( ref_img.Copy(rect_list[i]) );
        }
    }
    return id;
}

void ImageDicer::Dice(const AssocVector<int, Rect> &assoc_rects)
{
    if ( assoc_rects.Empty() ) return;

    AssocVector<int, Rect>::ConstIterator iter;

    const IGraphics &ref_img = GetImage(SOURCE);
    Rect R = ref_img.GetRect();
    R.SetXY(0,0);

    for ( iter = assoc_rects.ConstBegin(); !!iter; ++iter)
    {
        if ( iter.Value() && R )
        {
            assert( iter.Key()!=0 && "ImageDicer::Dice(): ID-Zero is reference image - You cannot overwrite it!" );
            _images.GetMap().Insert( iter.Key(), ref_img.Copy( iter.Value()) );
        }
    }
}

bool ImageDicer::IsColorKeyed() const 
{
    return (_images.Empty())? false : _images.ConstBegin().Value()->HasColorKey(); 
}

NormalPixel ImageDicer::GetColorKey() const 
{
    return (_images.Empty())? 0L : _images.ConstBegin().Value()->GetColorKey().u; 
}

void ImageDicer::SetColorKey(unsigned int c)
{
    if ( dynamic_cast<ImageBase*>( &GetImage(SOURCE) ) == static_cast<ImageBase*>( &GetImage(SOURCE) ) )
    {
        for ( ImageMap::Iterator iter = _images.Begin(); !!iter; ++iter )
        {
            static_cast<ImageBase&>(*iter.Value()).SetColorKey(c);
        }
    }
    else { assert("This ImageDicer cannot set color key"); }
}

void ImageDicer::SetNoColorKey()
{    
    if ( dynamic_cast<ImageBase*>( &GetImage(SOURCE) ) == static_cast<ImageBase*>( &GetImage(SOURCE) ) )
    {
        for ( ImageMap::Iterator iter = _images.Begin(); !!iter; ++iter )
        {
            static_cast<ImageBase&>(*iter.Value()).SetNoColorKey();
        }
    }
    else { assert("This ImageDicer cannot set no color key"); }
}

void  ImageDicer::Paint( int id, const Painter& painter, const Pos& p )
{
    ImageType *pim = _images.Find(id);
    if (pim) painter.ProjectImage( p.x, p.y, **pim);
}

void ImageDicer::Cooperate( int id, Widget *client)
{
    client->BeAlwaysDirty( GetImage(id).GetFrameRate() > 0 );
}

Dim2i ImageDicer::GetMaxSize() const
{
    ImageMap::ConstIterator it = _images.ConstBegin();
    Dim2i d;

    for (; !!it; ++it)
    {
        Dim2i c( it.Value()->GetWidth(), it.Value()->GetHeight() );
        if ( d.x<c.x ) { d.x = c.x; }
        if ( d.y<c.y ) { d.y = c.y; }
    }
    return d;
}

ImageDicer& ImageDicer::Remap( std::vector<int> mapping )
{
    ImageMap map = _images.GetMap();
    _images.Clear();
    _images.GetMap().Insert( SOURCE, map[SOURCE] );

    for ( int i=0, j=0, n=map.Size(); j<n; ++i, ++j )
    {
        if ( map.GetKey(j) == SOURCE ) { ++j; }
        if (j<n)
        _images.GetMap().Insert( mapping[i], map.GetValue(j) );
    }
    return *this;
}


};// namespace Useless
