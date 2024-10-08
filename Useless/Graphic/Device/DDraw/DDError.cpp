#include "UselessPch.h"

#include "Useless/Graphic/Device/DDraw/DDError.h"

namespace Useless {

DDError::DDError(long dd_error_code) : Error( Deduce(dd_error_code) )
{
    delete[] _temporary;
}

char* DDError::Deduce(long dd_error_code)
{
    _temporary = new char[255];
    switch(dd_error_code)
    {
    case DD_OK :
        sprintf( _temporary, "The request completed successfully. ");
        break;
    case DDERR_ALREADYINITIALIZED :
        sprintf( _temporary, "The object has already been initialized. ");
        break;
    case DDERR_BLTFASTCANTCLIP :
        sprintf( _temporary, "A DirectDrawClipper object is attached to a source surface that has passed into a call to the IDirectDrawSurface7::BltFast method. ");
        break;
    case DDERR_CANNOTATTACHSURFACE :
        sprintf( _temporary, "A surface cannot be attached to another requested surface. ");
        break;
    case DDERR_CANNOTDETACHSURFACE :
        sprintf( _temporary, "A surface cannot be detached from another requested surface. ");
        break;
    case DDERR_CANTCREATEDC :
        sprintf( _temporary, "Windows cannot create any more device contexts (DCs), or a DC has requested a palette-indexed surface when the surface had no palette and the display mode was not palette-indexed (in this case DirectDraw cannot select a proper palette into the DC). ");
        break;
    case DDERR_CANTDUPLICATE :
        sprintf( _temporary, "Primary and 3-D surfaces, or surfaces that are implicitly created, cannot be duplicated. ");
        break;
    case DDERR_CANTLOCKSURFACE :
        sprintf( _temporary, "Access to this surface is refused because an attempt was made to lock the primary surface without DCI support. ");
        break;
    case DDERR_CANTPAGELOCK :
        sprintf( _temporary, "An attempt to page-lock a surface failed. Page lock does not work on a display-memory surface or an emulated primary surface. ");
        break;
    case DDERR_CANTPAGEUNLOCK :
        sprintf( _temporary, "An attempt to page-unlock a surface failed. Page unlock does not work on a display-memory surface or an emulated primary surface. ");
        break;
    case DDERR_CLIPPERISUSINGHWND :
        sprintf( _temporary, "An attempt was made to set a clip list for a DirectDrawClipper object that is already monitoring a window handle. ");
        break;
    case DDERR_COLORKEYNOTSET :
        sprintf( _temporary, "No source color key is specified for this operation. ");
        break;
    case DDERR_CURRENTLYNOTAVAIL :
        sprintf( _temporary, "No support is currently available. ");
        break;
    case DDERR_DDSCAPSCOMPLEXREQUIRED :
        sprintf( _temporary, "New for DirectX 7.0. The surface requires the DDSCAPS_COMPLEX flag. ");
        break;
    case DDERR_DCALREADYCREATED :
        sprintf( _temporary, "A device context (DC) has already been returned for this surface. Only one DC can be retrieved for each surface. ");
        break;
    case DDERR_DEVICEDOESNTOWNSURFACE :
        sprintf( _temporary, "Surfaces created by one DirectDraw device cannot be used directly by another DirectDraw device. ");
        break;
    case DDERR_DIRECTDRAWALREADYCREATED :
        sprintf( _temporary, "A DirectDraw object representing this driver has already been created for this process. ");
        break;
    case DDERR_EXCEPTION :
        sprintf( _temporary, "An exception was encountered while performing the requested operation. ");
        break;
    case DDERR_EXCLUSIVEMODEALREADYSET :
        sprintf( _temporary, "An attempt was made to set the cooperative level when it was already set to exclusive. ");
        break;
    case DDERR_EXPIRED :
        sprintf( _temporary, "The data has expired and is therefore no longer valid. ");
        break;
    case DDERR_GENERIC :
        sprintf( _temporary, "There is an undefined error condition. ");
        break;
    case DDERR_HEIGHTALIGN :
        sprintf( _temporary, "The height of the provided rectangle is not a multiple of the required alignment. ");
        break;
    case DDERR_HWNDALREADYSET :
        sprintf( _temporary, "The DirectDraw cooperative-level window handle has already been set. It cannot be reset while the process has surfaces or palettes created. ");
        break;
    case DDERR_HWNDSUBCLASSED :
        sprintf( _temporary, "DirectDraw is prevented from restoring state because the DirectDraw cooperative-level window handle has been subclassed. ");
        break;
    case DDERR_IMPLICITLYCREATED :
        sprintf( _temporary, "The surface cannot be restored because it is an implicitly created surface. ");
        break;
    case DDERR_INCOMPATIBLEPRIMARY :
        sprintf( _temporary, "The primary surface creation request does not match the existing primary surface. ");
        break;
    case DDERR_INVALIDCAPS :
        sprintf( _temporary, "One or more of the capability bits passed to the callback function are incorrect. ");
        break;
    case DDERR_INVALIDCLIPLIST :
        sprintf( _temporary, "DirectDraw does not support the provided clip list. ");
        break;
    case DDERR_INVALIDDIRECTDRAWGUID :
        sprintf( _temporary, "The globally unique identifier (GUID) passed to the DirectDrawCreate function is not a valid DirectDraw driver identifier. ");
        break;
    case DDERR_INVALIDMODE :
        sprintf( _temporary, "DirectDraw does not support the requested mode. ");
        break;
    case DDERR_INVALIDOBJECT :
        sprintf( _temporary, "DirectDraw received a pointer that was an invalid DirectDraw object. ");
        break;
    case DDERR_INVALIDPARAMS :
        sprintf( _temporary, "One or more of the parameters passed to the method are incorrect. ");
        break;
    case DDERR_INVALIDPIXELFORMAT :
        sprintf( _temporary, "The pixel format was invalid as specified. ");
        break;
    case DDERR_INVALIDPOSITION :
        sprintf( _temporary, "The position of the overlay on the destination is no longer legal. ");
        break;
    case DDERR_INVALIDRECT :
        sprintf( _temporary, "The provided rectangle was invalid. ");
        break;
    case DDERR_INVALIDSTREAM :
        sprintf( _temporary, "The specified stream contains invalid data. ");
        break;
    case DDERR_INVALIDSURFACETYPE :
        sprintf( _temporary, "The surface was of the wrong type. ");
        break;
    case DDERR_LOCKEDSURFACES :
        sprintf( _temporary, "One or more surfaces are locked, causing the failure of the requested operation. ");
        break;
    case DDERR_MOREDATA :
        sprintf( _temporary, "There is more data available than the specified buffer size can hold. ");
        break;
    case DDERR_NEWMODE :
        sprintf( _temporary, "New for DirectX 7.0. When IDirectDraw7::StartModeTest is called with the DDSMT_ISTESTREQUIRED flag, it may return this value to denote that some or all of the resolutions can and should be tested. IDirectDraw7::EvaluateMode returns this value to indicate that the test has switched to a new display mode. ");
        break;
    case DDERR_NO3D :
        sprintf( _temporary, "No 3-D hardware or emulation is present. ");
        break;
    case DDERR_NOALPHAHW :
        sprintf( _temporary, "No alpha-acceleration hardware is present or available, causing the failure of the requested operation. ");
        break;
    case DDERR_NOBLTHW :
        sprintf( _temporary, "No blitter hardware is present. ");
        break;
    case DDERR_NOCLIPLIST :
        sprintf( _temporary, "No clip list is available. ");
        break;
    case DDERR_NOCLIPPERATTACHED :
        sprintf( _temporary, "No DirectDrawClipper object is attached to the surface object. ");
        break;
    case DDERR_NOCOLORCONVHW :
        sprintf( _temporary, "No color-conversion hardware is present or available. ");
        break;
    case DDERR_NOCOLORKEY :
        sprintf( _temporary, "The surface does not currently have a color key. ");
        break;
    case DDERR_NOCOLORKEYHW :
        sprintf( _temporary, "There is no hardware support for the destination color key. ");
        break;
    case DDERR_NOCOOPERATIVELEVELSET :
        sprintf( _temporary, "A create function was called without the IDirectDraw7::SetCooperativeLevel method. ");
        break;
    case DDERR_NODC :
        sprintf( _temporary, "No device context (DC) has ever been created for this surface. ");
        break;
    case DDERR_NODDROPSHW :
        sprintf( _temporary, "No DirectDraw raster-operation (ROP) hardware is available. ");
        break;
    case DDERR_NODIRECTDRAWHW :
        sprintf( _temporary, "Hardware-only DirectDraw object creation is not possible; the driver does not support any hardware. ");
        break;
    case DDERR_NODIRECTDRAWSUPPORT :
        sprintf( _temporary, "DirectDraw support is not possible with the current display driver. ");
        break;
    case DDERR_NODRIVERSUPPORT :
        sprintf( _temporary, "New for DirectX 7.0. Testing cannot proceed because the display adapter driver does not enumerate refresh rates. ");
        break;
    case DDERR_NOEMULATION :
        sprintf( _temporary, "Software emulation is not available. ");
        break;
    case DDERR_NOEXCLUSIVEMODE :
        sprintf( _temporary, "The operation requires the application to have exclusive mode, but the application does not have exclusive mode. ");
        break;
    case DDERR_NOFLIPHW :
        sprintf( _temporary, "Flipping visible surfaces is not supported. ");
        break;
    case DDERR_NOFOCUSWINDOW :
        sprintf( _temporary, "An attempt was made to create or set a device window without first setting the focus window. ");
        break;
    case DDERR_NOGDI :
        sprintf( _temporary, "No GDI is present. ");
        break;
    case DDERR_NOHWND :
        sprintf( _temporary, "Clipper notification requires a window handle, or no window handle has been previously set as the cooperative level window handle. ");
        break;
    case DDERR_NOMIPMAPHW :
        sprintf( _temporary, "No mipmap-capable texture mapping hardware is present or available. ");
        break;
    case DDERR_NOMIRRORHW :
        sprintf( _temporary, "No mirroring hardware is present or available. ");
        break;
    case DDERR_NOMONITORINFORMATION :
        sprintf( _temporary, "New for DirectX 7.0. Testing cannot proceed because the monitor has no associated EDID data. ");
        break;
    case DDERR_NONONLOCALVIDMEM :
        sprintf( _temporary, "An attempt was made to allocate nonlocal video memory from a device that does not support nonlocal video memory. ");
        break;
    case DDERR_NOOPTIMIZEHW :
        sprintf( _temporary, "The device does not support optimized surfaces. ");
        break;
    case DDERR_NOOVERLAYDEST :
        sprintf( _temporary, "The IDirectDrawSurface7::GetOverlayPosition method is called on an overlay that the IDirectDrawSurface7::UpdateOverlay method has not been called on to establish as a destination. ");
        break;
    case DDERR_NOOVERLAYHW :
        sprintf( _temporary, "No overlay hardware is present or available. ");
        break;
    case DDERR_NOPALETTEATTACHED :
        sprintf( _temporary, "No palette object is attached to this surface. ");
        break;
    case DDERR_NOPALETTEHW :
        sprintf( _temporary, "There is no hardware support for 16- or 256-color palettes. ");
        break;
    case DDERR_NORASTEROPHW :
        sprintf( _temporary, "No appropriate raster-operation hardware is present or available. ");
        break;
    case DDERR_NOROTATIONHW :
        sprintf( _temporary, "No rotation hardware is present or available. ");
        break;
    case DDERR_NOSTEREOHARDWARE :
        sprintf( _temporary, "There is no stereo hardware present or available. ");
        break;
    case DDERR_NOSTRETCHHW :
        sprintf( _temporary, "There is no hardware support for stretching. ");
        break;
    case DDERR_NOSURFACELEFT :
        sprintf( _temporary, "There is no hardware present that supports stereo surfaces. ");
        break;
    case DDERR_NOT4BITCOLOR :
        sprintf( _temporary, "The DirectDrawSurface object is not using a 4-bit color palette, and the requested operation requires a 4-bit color palette. ");
        break;
    case DDERR_NOT4BITCOLORINDEX :
        sprintf( _temporary, "The DirectDrawSurface object is not using a 4-bit color index palette, and the requested operation requires a 4-bit color index palette. ");
        break;
    case DDERR_NOT8BITCOLOR :
        sprintf( _temporary, "The DirectDrawSurface object is not using an 8-bit color palette, and the requested operation requires an 8-bit color palette. ");
        break;
    case DDERR_NOTAOVERLAYSURFACE :
        sprintf( _temporary, "An overlay component is called for a nonoverlay surface. ");
        break;
    case DDERR_NOTEXTUREHW :
        sprintf( _temporary, "The operation cannot be carried out because no texture-mapping hardware is present or available. ");
        break;
    case DDERR_NOTFLIPPABLE :
        sprintf( _temporary, "An attempt was made to flip a surface that cannot be flipped. ");
        break;
    case DDERR_NOTFOUND :
        sprintf( _temporary, "The requested item was not found. ");
        break;
    case DDERR_NOTINITIALIZED :
        sprintf( _temporary, "An attempt was made to call an interface method of a DirectDraw object created by CoCreateInstance before the object was initialized. ");
        break;
    case DDERR_NOTLOADED :
        sprintf( _temporary, "The surface is an optimized surface, but it has not yet been allocated any memory. ");
        break;
    case DDERR_NOTLOCKED :
        sprintf( _temporary, "An attempt was made to unlock a surface that was not locked. ");
        break;
    case DDERR_NOTPAGELOCKED :
        sprintf( _temporary, "An attempt was made to page-unlock a surface with no outstanding page locks. ");
        break;
    case DDERR_NOTPALETTIZED :
        sprintf( _temporary, "The surface being used is not a palette-based surface. ");
        break;
    case DDERR_NOVSYNCHW :
        sprintf( _temporary, "There is no hardware support for vertical blank–synchronized operations. ");
        break;
    case DDERR_NOZBUFFERHW :
        sprintf( _temporary, "The operation to create a z-buffer in display memory or to perform a blit, using a z-buffer cannot be carried out because there is no hardware support for z-buffers. ");
        break;
    case DDERR_NOZOVERLAYHW :
        sprintf( _temporary, "The overlay surfaces cannot be z-layered, based on the z-order because the hardware does not support z-ordering of overlays. ");
        break;
    case DDERR_OUTOFCAPS :
        sprintf( _temporary, "The hardware needed for the requested operation has already been allocated. ");
        break;
    case DDERR_OUTOFMEMORY :
        sprintf( _temporary, "DirectDraw does not have enough memory to perform the operation. ");
        break;
    case DDERR_OUTOFVIDEOMEMORY :
        sprintf( _temporary, "DirectDraw does not have enough display memory to perform the operation. ");
        break;
    case DDERR_OVERLAPPINGRECTS :
        sprintf( _temporary, "The source and destination rectangles are on the same surface and overlap each other. ");
        break;
    case DDERR_OVERLAYCANTCLIP :
        sprintf( _temporary, "The hardware does not support clipped overlays. ");
        break;
    case DDERR_OVERLAYCOLORKEYONLYONEACTIVE :
        sprintf( _temporary, "An attempt was made to have more than one color key active on an overlay. ");
        break;
    case DDERR_OVERLAYNOTVISIBLE :
        sprintf( _temporary, "The IDirectDrawSurface7::GetOverlayPosition method was called on a hidden overlay. ");
        break;
    case DDERR_PALETTEBUSY :
        sprintf( _temporary, "Access to this palette is refused because the palette is locked by another thread. ");
        break;
    case DDERR_PRIMARYSURFACEALREADYEXISTS :
        sprintf( _temporary, "This process has already created a primary surface. ");
        break;
    case DDERR_REGIONTOOSMALL :
        sprintf( _temporary, "The region passed to the IDirectDrawClipper::GetClipList method is too small. ");
        break;
    case DDERR_SURFACEALREADYATTACHED :
        sprintf( _temporary, "An attempt was made to attach a surface to another surface to which it is already attached. ");
        break;
    case DDERR_SURFACEALREADYDEPENDENT :
        sprintf( _temporary, "An attempt was made to make a surface a dependency of another surface on which it is already dependent. ");
        break;
    case DDERR_SURFACEBUSY :
        sprintf( _temporary, "Access to the surface is refused because the surface is locked by another thread. ");
        break;
    case DDERR_SURFACEISOBSCURED :
        sprintf( _temporary, "Access to the surface is refused because the surface is obscured. ");
        break;
    case DDERR_SURFACELOST :
        sprintf( _temporary, "Access to the surface is refused because the surface memory is gone. Call the IDirectDrawSurface7::Restore method on this surface to restore the memory associated with it. ");
        break;
    case DDERR_SURFACENOTATTACHED :
        sprintf( _temporary, "The requested surface is not attached. ");
        break;
    case DDERR_TESTFINISHED :
        sprintf( _temporary, "New for DirectX 7.0. When returned by the IDirectDraw7::StartModeTest method, this value means that no test could be initiated because all the resolutions chosen for testing already have refresh rate information in the registry. When returned by IDirectDraw7::EvaluateMode, the value means that DirectDraw has completed a refresh rate test. ");
        break;
    case DDERR_TOOBIGHEIGHT :
        sprintf( _temporary, "The height requested by DirectDraw is too large. ");
        break;
    case DDERR_TOOBIGSIZE :
        sprintf( _temporary, "The size requested by DirectDraw is too large. However, the individual height and width are valid sizes. ");
        break;
    case DDERR_TOOBIGWIDTH :
        sprintf( _temporary, "The width requested by DirectDraw is too large. ");
        break;
    case DDERR_UNSUPPORTED :
        sprintf( _temporary, "The operation is not supported. ");
        break;
    case DDERR_UNSUPPORTEDFORMAT :
        sprintf( _temporary, "The pixel format requested is not supported by DirectDraw. ");
        break;
    case DDERR_UNSUPPORTEDMASK :
        sprintf( _temporary, "The bitmask in the pixel format requested is not supported by DirectDraw. ");
        break;
    case DDERR_UNSUPPORTEDMODE :
        sprintf( _temporary, "The display is currently in an unsupported mode. ");
        break;
    case DDERR_VERTICALBLANKINPROGRESS :
        sprintf( _temporary, "A vertical blank is in progress. ");
        break;
    case DDERR_VIDEONOTACTIVE :
        sprintf( _temporary, "The video port is not active. ");
        break;
    case DDERR_WASSTILLDRAWING :
        sprintf( _temporary, "The previous blit operation that is transferring information to or from this surface is incomplete. ");
        break;
    case DDERR_WRONGMODE :
        sprintf( _temporary, "This surface cannot be restored because it was created in a different mode. ");
        break;
    case DDERR_XALIGN :
        sprintf( _temporary, "The provided rectangle was not horizontally aligned on a required boundary. ");
        break;
    default:
        sprintf( _temporary, "Undocumented error.");
        break;
    }
    return _temporary;
}

};//namespace Useless
