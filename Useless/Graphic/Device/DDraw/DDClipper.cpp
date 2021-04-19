#include "DDClipper.h"
#include "Useless/Graphic/Device/DDraw/DDScreen.h"
#include "Useless/Graphic/Device/DDraw/DDSurface.h"
#include "Useless/Util/Storage/MemBlock.h"
#include "Useless/ErrorConfig.h"
#define   DDClipper_ERROR(e) APP_CRITICAL(e, "DDClipper")

namespace Useless {

DDClipper::DDClipper(): _dd_clipper(NULL) {}

DDClipper::DDClipper(LPDIRECTDRAWCLIPPER pDDClipper)  // AddRef not used here ...?
{
    _dd_clipper = pDDClipper;
}

DDClipper::DDClipper(const DDClipper& DDClipper): _dd_clipper(DDClipper._dd_clipper) 
{
    if (_dd_clipper) _dd_clipper->AddRef();
}

DDClipper& DDClipper::operator=(const DDClipper &DDClipper) 
{ 
    if (_dd_clipper) _dd_clipper->Release();
    _dd_clipper=DDClipper._dd_clipper;
    if (_dd_clipper) _dd_clipper->AddRef();
    return *this;
}

DDClipper::~DDClipper()
{
    if (_dd_clipper )
    int refcnt = _dd_clipper->Release();
    _dd_clipper=NULL;
}


DDClipper::DDClipper(const Rect &rect): _dd_clipper(NULL)
{
    if(!! rect)
    {
        DDraw7Ptr dd = DDCreator::GetDirectDraw();
        dd->CreateClipper(0, &_dd_clipper, NULL);

        char rgbf[sizeof(RGNDATAHEADER) + sizeof(RECT)];
        LPRGNDATA regn = (LPRGNDATA)(&rgbf);
        regn->rdh.dwSize = sizeof(RGNDATAHEADER);
        regn->rdh.iType = RDH_RECTANGLES;
        regn->rdh.nCount = 1;
        regn->rdh.nRgnSize = sizeof(RECT);
        regn->rdh.rcBound.left  = rect.GetX1();
        regn->rdh.rcBound.top   = rect.GetY1();
        regn->rdh.rcBound.right = rect.GetX2();
        regn->rdh.rcBound.bottom= rect.GetY2();
        LPRECT rc = (LPRECT)(regn->Buffer);
        rc[0].left  = rect.GetX1();
        rc[0].top   = rect.GetY1();
        rc[0].right = rect.GetX2();
        rc[0].bottom= rect.GetY2();

        if ( _dd_clipper->SetClipList(regn,0) != DD_OK ) 
        { 
            DDClipper_ERROR( Useless::Error("DDClipper::DDClipper() :SetClipList() failed") ); 
        }
    }
}

RECT g_LastRegion[1024];
int g_SizeLastRegion = 0;

DDClipper::DDClipper(const RectList &rect_list): _dd_clipper(NULL)
{
    int i,j,num,size =num= rect_list.size();
    for ( j=0; j!=size; ++j ) { if (!rect_list[j]) --num; }
    if (num > 0)
    {        
        DDraw7Ptr dd = DDCreator::GetDirectDraw();
        dd->CreateClipper(0, &_dd_clipper, NULL);

        MemBlock region_buffer( sizeof(RGNDATAHEADER) + sizeof(RECT)*num );
        LPRGNDATA regn = (LPRGNDATA)( region_buffer.GetPtr() );
        regn->rdh.dwSize = sizeof(RGNDATAHEADER);
        regn->rdh.iType = RDH_RECTANGLES;
        regn->rdh.nCount = num;
        regn->rdh.nRgnSize = sizeof(RECT);
        LPRECT rc = (LPRECT)(regn->Buffer);
    
        Rect bound( rect_list[0] );

        g_SizeLastRegion = 0;
        
        for(j=0, i=0; j!=size; ++j)
        {
            if (!rect_list[j]) { continue; }
            
            rc[i].left  = rect_list[j].GetX1();
            rc[i].top   = rect_list[j].GetY1();
            rc[i].right = rect_list[j].GetX2();
            rc[i].bottom= rect_list[j].GetY2();

#ifdef DEBUG_REGION
            g_LastRegion[ g_SizeLastRegion++ ] = rc[i];
#endif

            bound |= rect_list[j];
            ++i;
        }

        assert( i == num );
        
        regn->rdh.rcBound.left  = bound.GetX1();
        regn->rdh.rcBound.top   = bound.GetY1();
        regn->rdh.rcBound.right = bound.GetX2();
        regn->rdh.rcBound.bottom= bound.GetY2();
        
#ifdef DEBUG_REGION
        g_LastRegion[ g_SizeLastRegion++ ] = regn->rdh.rcBound;
#endif

        if ( _dd_clipper->SetClipList(regn,0) != DD_OK ) 
        { 
            DDClipper_ERROR( Useless::Error("DDClipper::DDClipper() :SetClipList() failed") );
        }
    }
}


RectList DDClipper::GetRectangles() const
{
    int r;
    if(! _dd_clipper) { return RectList(); }
    Types::ULONG csize;
    r=_dd_clipper->GetClipList(NULL,NULL,&csize);
    C_verify( r == DD_OK );

    MemBlock region_buffer( csize );
    LPRGNDATA regn = (LPRGNDATA)( region_buffer.GetPtr() );
    r=_dd_clipper->GetClipList(NULL,regn,&csize);
    LPRECT rc = (LPRECT)(regn->Buffer);
    C_verify( r == DD_OK );

    RectList rect_list( regn->rdh.nCount );
    for( int i=0; i!= regn->rdh.nCount; ++i )
    {
        rect_list[i] = Rect( rc[i].left, rc[i].top, 0, 0 );
        rect_list[i].SetX2( rc[i].right  );
        rect_list[i].SetY2( rc[i].bottom );
    }

    return rect_list;
}


RectList DDClipper::GetRectangles(const Rect &rect) const
{
    int r;
    if(! _dd_clipper) { return RectList(); }
    RECT crect;
    LPRECT prc=NULL;
    crect.left = rect.GetX1(); crect.right  = rect.GetX2();
    crect.top  = rect.GetY1(); crect.bottom = rect.GetY2();
    prc = &crect;
    Types::ULONG csize;
    r=_dd_clipper->GetClipList(prc,NULL,&csize);
    C_verify( r == DD_OK );

    MemBlock region_buffer( csize );
    LPRGNDATA regn = (LPRGNDATA)( region_buffer.GetPtr() );
    r=_dd_clipper->GetClipList(prc,regn,&csize);
    LPRECT rc = (LPRECT)(regn->Buffer);
    C_verify( r == DD_OK );

    RectList rect_list( regn->rdh.nCount );
    for( int i=0; i!= regn->rdh.nCount; ++i )
    {
        rect_list[i] = Rect( rc[i].left, rc[i].top, 0, 0 );
        rect_list[i].SetX2( rc[i].right  );
        rect_list[i].SetY2( rc[i].bottom );
    }

    return rect_list;
}

RectList DDClipper::GetRectangles(const RectList &rect_list) const
{
    RectList rects;
    for( int i=0; i!=rect_list.size(); ++i)
    {
        RectList rcs = GetRectangles( rect_list[i] );
        for( int k=0; k!=rcs.size(); ++k ) rects.push_back( rcs[k] );
    }

    return rects;
}


};// namespace Useless
