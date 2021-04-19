#include "Useless/UselessPch.h"

#include "Useless/Graphic/Device/DDraw/DDSurface.h"
#include "Useless/Graphic/Device/DDraw/DDClipper.h"
#include "Useless/System/TypeInfo.h"

#include "Useless/Graphic/Pixel/Pixel.h"
#include "Useless/Util/LockObj.h"

#include "Useless/ErrorConfig.h"
#include "Useless/Graphic/Device/DDraw/DDError.h"
#define   DDSURFACE_ERROR(e) APP_CRITICAL(e, "DDSurface")
namespace Useless {

DDSurface::DDSurface(): _dd_surface(0), _software_surface(0)
{}

DDSurface::DDSurface( const Surf::Properties &prop )
{
    Create(prop);
}

/*! Create DDSurface compiling to properties
 */
void DDSurface::Create(const Surf::Properties &prop)
{
    _software_surface = NULL;
    _dd_surface.Release();

    DDraw7Ptr pdd = DDCreator::GetDirectDraw();
    _dds_desc = prop;

    if ( prop.alpha ) 
    {
        _properties = prop;
        _properties.pixelformat = ImageFormat::B8G8R8;
        _software_surface = new MemorySurface( _properties );
    }
    else
    {
        assert( prop.color );

        // Create the DDraw surface
        int r = pdd->CreateSurface( &_dds_desc, &_dd_surface, NULL );
        if( r !=DD_OK ) { DDSURFACE_ERROR( DDError(r) ); }

            //DDSURFACE_ERROR(Error("DDSurface::DDSurface(): CreateSurface"));

        // Prepare the DDSURFACEDESC structure
        _dds_desc.dwSize = sizeof(_dds_desc);

        // Get the DDSURFACEDESC structure for this surface
        _dd_surface->GetSurfaceDesc( &_dds_desc );
        _properties = prop;
        Surf::Properties p(_dds_desc);
        _properties.pixelformat = p.pixelformat;
        if (_properties.has_color_key) { SetColorKey(_properties.color_key); }

        // Create the Z surface
        if ( prop.zbuffer )
        {
            DDSURFACEDESC2 desc;
            desc = prop;

            ZeroMemory( &desc.ddpfPixelFormat, sizeof(DDPIXELFORMAT) );
            desc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
            desc.ddsCaps.dwCaps |= DDSCAPS_ZBUFFER;
            desc.ddpfPixelFormat.dwZBufferBitDepth = 16;
            desc.ddpfPixelFormat.dwZBitMask = 0xFFFF;
            desc.ddpfPixelFormat.dwFlags = DDPF_ZBUFFER;
            desc.dwFlags |= DDSD_PIXELFORMAT;
            LPDIRECTDRAWSURFACE7 zsurface;
            if( pdd->CreateSurface( &desc, &zsurface, NULL ) !=DD_OK )
                DDSURFACE_ERROR(Error("DDSurface::DDSurface(): CreateSurface ZBuffer"));
            if (_dd_surface->AddAttachedSurface(zsurface) != DD_OK)
                DDSURFACE_ERROR(Error("DDSurface::DDSurface(): CreateSurface AddAttachedSurface"));
        }

        /*********************************************
        // DDRAWERROR: ALPHA NOT SUPPORTED THIS RELEASE
        // Create the Alpha surface
        if ( prop.alpha )
        {
            DDSURFACEDESC2 desc;
            desc = prop;

            ZeroMemory( &desc.ddpfPixelFormat, sizeof(DDPIXELFORMAT) );
            desc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
            desc.ddsCaps.dwCaps |= DDSCAPS_ALPHA ;
            desc.ddpfPixelFormat.dwAlphaBitDepth = 8;
            desc.ddpfPixelFormat.dwLuminanceAlphaBitMask = 0xFF;
            desc.ddpfPixelFormat.dwFlags = DDPF_ALPHA;
            desc.dwFlags |= DDSD_PIXELFORMAT;
            LPDIRECTDRAWSURFACE7 asurface;
            if( pdd->CreateSurface( &desc, &asurface, NULL ) !=DD_OK )
                DDSURFACE_ERROR(Error("DDSurface::DDSurface(): CreateSurface Alpha"));
            if (_dd_surface->AddAttachedSurface(asurface) != DD_OK)
                DDSURFACE_ERROR(Error("DDSurface::DDSurface(): CreateSurface AddAttachedSurface"));
        }
        *********************************************/
    }
    
}

/*! Create DDSurface from API.DirectDraw surface
 */
DDSurface::DDSurface( const DDrawSurface7 &dd_surface ): _software_surface(0)
{
    Leave_SW_Mode();

    if ( !!dd_surface )
    {
        // Get the DDSURFACEDESC structure for this surface
        _dds_desc.dwSize = sizeof(_dds_desc);
        dd_surface->GetSurfaceDesc( &_dds_desc );
    }
    _dd_surface = dd_surface;
    _properties = Surf::Properties(_dds_desc);
}

/*! Create reference to existing DDSurface ( not tested may CRASH!!! )
 */
DDSurface::DDSurface( const DDSurface &dds )
{
    Leave_SW_Mode();

    if ( dds._software_surface )
    {
        DDSURFACE_ERROR(Error("DDSurface::DDSurface(): Cannot add new reference to memory surface"));
    }
    else
    {
        _dd_surface = dds._dd_surface;
        if( !!_dd_surface )
        {
            // Get the DDSURFACEDESC structure for this surface
            _dds_desc.dwSize = sizeof(_dds_desc);
            _dd_surface->GetSurfaceDesc( &_dds_desc );
        }
    }
}

/*! Say good bye
 */
DDSurface::~DDSurface()
{
    Release();
}

/*! Create another DDSurface which compiles to properties
 */
Surface* DDSurface::CreateSurface( Surf::Properties &properties ) const
{
    if ( properties.pixelformat == ImageFormat::INVALID ||
         properties.pixelformat == _properties.pixelformat ||
         properties.alpha && !properties.color )
    {
        DDSurface *psurface = new DDSurface( properties );
        psurface->GetProperties( &properties );
        return psurface;
    }
    else
    {
        return NULL;
    }
}

/*! Release API resources
 */
void  DDSurface::Release()
{
    Leave_SW_Mode();

    if ( _software_surface )
    {
        delete _software_surface;
    }
    else if ( !!_dd_surface ) 
    { 
        if ( _properties.zbuffer )
        {
            DDSCAPS2 ddsCaps;
            ZeroMemory( &ddsCaps, sizeof(DDSCAPS2) );
            ddsCaps.dwCaps |= DDSCAPS_ZBUFFER;

            LPDIRECTDRAWSURFACE7 zsurface;
            _dd_surface->GetAttachedSurface( &ddsCaps, &zsurface);
            if (zsurface)
                _dd_surface->DeleteAttachedSurface( 0, zsurface);
        }
        _dd_surface.Release();
    }
}

/*! Overloaded virtual Surface::Blit() redirects execution to DDSurface::Blt()
 */
void DDSurface::Blit( int x, int y, const Surface &src, const Useless::Rect &src_rect, const Surf::BlitFX &fx )
{
    if ( typeid(src)==typeid(const DDSurface) )
    {
        if ( fx.stretch && ( !fx.width || !fx.height ) ) return;

        const DDSurface &dd_surf = (const DDSurface&)src;
        
        int w = (fx.stretch)? fx.width : src_rect.GetW();
        int h = (fx.stretch)? fx.height : src_rect.GetH();

        Rect dest_rec( x, y, w, h );

        if ( dd_surf._properties.alpha )
        {
            SW_Blit( dest_rec, dd_surf, src_rect, fx );
        }
        else
        {
            HW_Blit( dest_rec, dd_surf, src_rect, fx );
        }
    }
}

/*! DDSurface specialized Blt
 */
void DDSurface::HW_Blit( const Rect &d,
                         const DDSurface &source,
                         const Rect &s,
                         const Surf::BlitFX &fx )
{
    Leave_SW_Mode();
    const_cast<DDSurface&>( source ).Leave_SW_Mode();

    LPRECT psr,pdr;
    RECT src;
    RECT drc;
    if((!! s) || (!! d))
    {
        src.left   = s.GetX1();     drc.left   = d.GetX1();
        src.right  = s.GetX2();     drc.right  = d.GetX2();
        src.top    = s.GetY1();     drc.top    = d.GetY1();
        src.bottom = s.GetY2();     drc.bottom = d.GetY2();
    }
    if(! s) { psr=0; } else { psr=&src; }
    if(! d) { pdr=0; } else { pdr=&drc; }
    
    int ckflg = (source._properties.has_color_key)? DDBLT_KEYSRC : 0;
    
   // do
    //{
    DDCreator::_blitter_status =
    _dd_surface->Blt( pdr, source.GetDDrawSurface().get(),
        psr, fx.GetDDFlags() | ckflg | DDCreator::_blitter_flags, &((DDBLTFX)fx) );

   // } while ( DDCreator::_blitter_status != DD_OK );
}

void DDSurface::SW_Blit( const Rect &d,
                         const DDSurface &source,
                         const Rect &s,
                         const Surf::BlitFX &fx )
{
    if ( Enter_SW_Mode() )
    {
        //RectList clip_list = _clipper.GetRectangles(d).GetOptimized();
        //ASSERT: rectangles are optimized
        Rect dest_clp( 0,0, _properties.width, _properties.height );

        source._software_surface->InvertAlpha( fx.invert_src_alpha );
        source._software_surface->SetFactors( fx.alpha_factors[0], fx.alpha_factors[1] );

        if ( source._properties.color )
        {
            if ( fx.rotation )
            {
                int r = ((d.GetW()<d.GetH())? d.GetH():d.GetH()) * 1.42;
                Pos pivot = (d.GetP1()+d.GetP2())/2;
                Rect max_area ( pivot.x-r/2, pivot.y-r/2, r, r );
                RectList clip_list = _clipper.GetRectangles(max_area);
                int k=clip_list.size(); while(k--)
                {
                    Rect c = dest_clp & clip_list[k];
                    source._software_surface->RotoBlit( c, _data_ptr, s,
                        (3.1415926*fx.angle)/180.0, pivot );
                }
            }
            else
            {
                RectList clip_list = _clipper.GetRectangles(d);
                int k=clip_list.size(); while(k--)
                {
                    Rect c = dest_clp & clip_list[k];
                    Pos  p = s.GetPos() - d.GetPos();
                    source._software_surface->Blit( c.GetX(), c.GetY(), _data_ptr, c+p );
                }
            }
        }
        else if ( fx.flood_fill )
        {
            if ( fx.rotation )
            {
                int r = ((d.GetW()<d.GetH())? d.GetH():d.GetH()) * 1.42;
                Pos pivot = (d.GetP1()+d.GetP2())/2;
                Rect max_area ( pivot.x-r/2, pivot.y-r/2, r, r );
                RectList clip_list = _clipper.GetRectangles(max_area);
                int k=clip_list.size(); while(k--)
                {
                    Rect c = dest_clp & clip_list[k];
                    source._software_surface->RotoColorFill( c, _data_ptr, s,
                        fx.flood_color, (3.1415926*fx.angle)/180.0, pivot );
                }
            }
            else
            {
                RectList clip_list = _clipper.GetRectangles(d);
                int k=clip_list.size(); while(k--)
                {
                    Rect c = dest_clp & clip_list[k];
                    Pos  p = s.GetPos() - d.GetPos();
                    source._software_surface->ColorFill(
                        c.GetX(), c.GetY(),_data_ptr, c+p, fx.flood_color );
                }
            }
        }
    }
}

bool DDSurface::Enter_SW_Mode()
{
    if ( !IsIn_SW_Mode() )
    {
        Lock( COLOR, &_data_ptr );
    }
    return _data_ptr.ptr;
}

void DDSurface::Leave_SW_Mode()
{
    if ( IsIn_SW_Mode() )
    {
        Unlock( COLOR); _data_ptr = Surf::Ptr();
    }
}

bool  DDSurface::IsValidBlitSource( const Surface &surface ) const
{
    if ( const DDSurface *p = dynamic_cast< const DDSurface * >( &surface ))
    {
        return p->IsValid();
    }    
    else
    {
        return false;
    }
}

/*! Check if surface exists & is not lost
 */
bool DDSurface::IsValid() const
{
    if ( !_software_surface && (!_dd_surface || _dd_surface->IsLost()))
    { return false; }
    else
    { return true; }
}


/*! Restore DirectDraw surface ( releases previous surface and creates new one )
 */
/*void DDSurface::Restore()
{
    assert( !IsIn_SW_Mode() );

    _dd_surface.Release();

    DDraw7Ptr pdd = DDCreator::GetDirectDraw();

    int result = pdd->CreateSurface( &_dds_desc, &_dd_surface, NULL );
    if( result == DD_OK )
    {
        _dds_desc.dwSize = sizeof(_dds_desc);
        _dd_surface->GetSurfaceDesc( &_dds_desc );
    }
}*/


/*! Lock surface, and give surface information.
 */
bool DDSurface::Lock( int channel_format, Surf::Ptr *surf_ptr )
{
    assert( !IsIn_SW_Mode() );

    assert(!!surf_ptr);
    switch( channel_format )
    {
        case COLOR:
        {
            if ( _software_surface )
            {
                _software_surface->Lock( &surf_ptr->ptr,
                                         &surf_ptr->pitch,
                                         &surf_ptr->format );
            }
            else
            {
                DDSURFACEDESC2 desc;
                ZeroMemory( &desc, sizeof(desc) );
                desc.dwSize = sizeof(desc);

                if ( _dd_surface->Lock(0, &desc, 0, 0 ) == DD_OK )
                {
                    surf_ptr->ptr = desc.lpSurface;
                    surf_ptr->pitch = desc.lPitch;
                    surf_ptr->format = ImageFormat::Recognize
                                (
                                    desc.ddpfPixelFormat.dwRBitMask, 
                                    desc.ddpfPixelFormat.dwGBitMask,
                                    desc.ddpfPixelFormat.dwBBitMask,
                                    desc.ddpfPixelFormat.dwRGBBitCount
                                );
                }
                else { return false; }
            }
            break;
        }
        case ALPHA:
        {
            if ( _software_surface )
            {
                _software_surface->LockAlpha( &surf_ptr->ptr,
                                              &surf_ptr->pitch );
                surf_ptr->format = ImageFormat::SINGLE8;
            }
            break;
        }

        case DEPTH:
        {
            assert(0);
            return false;
        };
    }//end switch

    return true;
}

/*! Unlock surface
*/
void DDSurface::Unlock( int channel_format )
{
    switch( channel_format )
    {
        case COLOR:
        {
            if ( _software_surface )
            {
                _software_surface->Unlock();
            }
            else
            {
                _dd_surface->Unlock(NULL);
            }
            break;
        }
        case ALPHA:
            break;

        case DEPTH:
            break;

    };//end switch
}



/*! Clear area
 */
void DDSurface::Clear(unsigned long color, const Useless::Rect &drc )
{
    Leave_SW_Mode();

    if ( _software_surface )
    {
        assert(0);
    }
    else
    {
        DDBLTFX ddbltfx;
        ZeroMemory( &ddbltfx, sizeof(ddbltfx) );
        ddbltfx.dwSize      = sizeof(ddbltfx);
        ddbltfx.dwFillColor = ImageFormat::Encode( _properties.pixelformat, color );

        if(!! drc)
        {
            RECT r;
            r.left=drc.GetX1(); r.right =drc.GetX2();
            r.top =drc.GetY1(); r.bottom=drc.GetY2();
            _dd_surface->Blt( &r, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
        }
        else
        {
            _dd_surface->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
        }
    }
}

/*! Get surface properties
 */
void DDSurface::GetProperties ( Surf::Properties *prop ) const
{
    *prop = _properties;
}

/*! Create new surface which compiles to properties
 */
bool DDSurface::SetProperties( const Surf::Properties &prop )
{
    Release();
    Create(prop);
    return true;
}

/*! Compare with other surface equality
 */
bool DDSurface::operator==( const Surface& s )
{
    if ( typeid(s)==typeid(const DDSurface) )
    {
        const DDSurface &dds = (const DDSurface&)s;
        return dds._dd_surface==_dd_surface && dds._software_surface==_software_surface;
    }
    return false;
}

/*! Compare with other surface for inequality
 */
bool DDSurface::operator!=(const Surface& s)
{
    return !(*this == s);
}

/*! Set clip region
 */
bool DDSurface::SetClipper(const Useless::RectList &rl)
{
    if (_software_surface) { return false; }

    Leave_SW_Mode(); //really it shouldn be here ... it would slow down

    return SetClipper( DDClipper( rl ));
}

RectList DDSurface::GetClipper( const Rect &crop ) const
{
    if (_software_surface) { return 0; }

    const_cast<DDSurface*>(this)->Leave_SW_Mode(); //really it shouldn be here ... it would slow down

    if ( !crop )
    {
        return _clipper.GetRectangles();
    }
    else
    {
        return _clipper.GetRectangles( crop );
    }
}


/*! Set color key
 */
bool DDSurface::SetColorKey(unsigned long color)
{
    if (_software_surface || !_dd_surface ) { return false; }

    Leave_SW_Mode(); //really it shouldn be here ... it would slow down

    _properties.has_color_key = true;
    _properties.color_key = color;

    DDCOLORKEY ddck;
    ddck.dwColorSpaceLowValue  = ImageFormat::Encode( _properties.pixelformat, color );
    ddck.dwColorSpaceHighValue = ImageFormat::Encode( _properties.pixelformat, color );
    
    return !!_dd_surface->SetColorKey( DDCKEY_SRCBLT, &ddck );
}

/*! Set clipper
 */
bool DDSurface::SetClipper(const DDClipper& clipper)
{
    if (_software_surface) { return false; }

    Leave_SW_Mode(); //really it shouldn be here ... it would slow down

    RectList rl;
    clipper.GetRectangles(rl);
    _dd_surface->SetClipper( clipper._dd_clipper );
    _clipper = clipper;
    _clipper.GetRectangles(rl);
    return 0;
}


/* API specific functions
-------------------------------------------------------------------------------------------*/
LPDIRECTDRAWCLIPPER DDSurface::GetDDrawClipper() const
{
    LPDIRECTDRAWCLIPPER dd_clipper=0;
    _dd_surface->GetClipper( &dd_clipper );
    return dd_clipper;
}

int DDSurface::DrawText( HFONT hFont,
                         TCHAR* strText,
                         DWORD dwOriginX,
                         DWORD dwOriginY,
                         COLORREF crBackground,
                         COLORREF crForeground,
                         DWORD dwBkMode )
{
    assert(!_software_surface);

    Leave_SW_Mode(); //really it shouldn be here ... it would slow down

    HDC     hDC = NULL;
    HRESULT hr;

    if( !_dd_surface || strText == NULL )
        return E_INVALIDARG;

    // Make sure this surface is restored.
    if( FAILED( hr = _dd_surface->Restore() ) )
        return hr;

    if( FAILED( hr = _dd_surface->GetDC( &hDC ) ) )
        return hr;

    // Set the background and foreground color
    SetBkMode( hDC, dwBkMode );
    SetBkColor( hDC, crBackground );
    SetTextColor( hDC, crForeground );

    if( hFont )
        SelectObject( hDC, hFont );

    // Use GDI to draw the text on the surface
    TextOut( hDC, dwOriginX, dwOriginY, strText, _tcslen(strText) );

    if( FAILED( hr = _dd_surface->ReleaseDC( hDC ) ) )
        return hr;

    return S_OK;
}


};//namespace Useless
