#ifndef __INCLUDED_USELESS_PIXEL_TRANSFER_H__
#define __INCLUDED_USELESS_PIXEL_TRANSFER_H__

namespace Useless {

class Rect;

/*! PixelTransfer - abstract class. It is responsible for transfering pixels
 *  Type may have set flags:
 *
 *  \li SOURCE - then SetDestination() and Store() methods are valid
 *
 *  \li DESTINATION - then SetSource() and Fetch() methods are valid
 *
 *  \li TRANSFORM - then Transform() method is valid
 *
 */
class USELESS_API PixelTransfer
{
public:
    virtual ~PixelTransfer() {}

    enum Type { SOURCE=1, DESTINATION=2, TRANSFORM=4 };

    //! Obtain transfer type information (combination of flags)
    virtual int  Info() = 0;

    //! Fetch data from source
    virtual void Fetch( const void *source, const Rect &area ) = 0;

    //! Store data into destination
    virtual void Store( void *destination, const Rect &area ) = 0;

    //! Transform data from input buffer to destination buffer
    virtual void Transform() = 0;

    //! Set source pixel-stream type
    virtual void SetSource( int  width,
                            int  height,
                            int  pitch,
                            int  pixel_format, //!< use ImageFormat::Format or BPP
                     const void *palette = 0,
                            int  colors = 0) = 0;
    
    //! Set destination pixel-stream type
    virtual void SetDestination( int  width,
                                 int  height,
                                 int  pitch,
                                 int  pixel_format, //!< use ImageFormat::Format or BPP
                                void *palette = 0,
                                 int  colors = 0) = 0;

    //! Get source pixel-stream type
    virtual void GetSource( int  *width,
                            int  *height,
                            int  *pixel_format,
                     const void **palette,
                            int  *colors ) = 0;

    //! Get destination pixel-stream type
    virtual void GetDestination( int  *width,
                                 int  *height,
                                 int  *pixel_format,
                                void **palette,
                                 int  *colors ) = 0;
    
    //! Set source buffer pointer. If needed use only before SetSource().
    virtual void SetSourcePointer( void *source) = 0;

    //! Set destination area pointer. If needed use only before SetDestination().
    virtual void SetDestinaitonPointer( void *destination) = 0;
    
    //! Use this hint, to tell filter, that we don't want any image processing, just copy pixels.
    virtual void SetSpeedHint( bool fastest=false) {}
};

};//namespace Useless
#endif//__INCLUDED_USELESS_PIXEL_TRANSFER_H__
