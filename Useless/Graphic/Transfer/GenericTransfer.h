#ifndef __INCLUDED_USELESS_GENERIC_TRANSFER_H__
#define __INCLUDED_USELESS_GENERIC_TRANSFER_H__

#include "Useless/Graphic/Transfer/PixelTransfer.h"
#include "Useless/Graphic/Rect.h"
#include "Useless/Util/Storage/MemBlock.h"

namespace Useless {

/*! GenericTransfer - transfer pixels from source to destination
 *  using transformation if needed.
 *
 *  Block diagram:
 *
 *  [source] -(Fetch)-> [input_buffer] -(Transform)-> [output_buffer] -(Store)-> [destination]
 *
 *  If source and destination have the same image format then:
 *
 *  [source] -(Fetch)-> [destination]
 *
 *  [source] -(Store)-> [destination]
 *
 *\verbatim
 *  Useage:
 *
 *  1) Load graphics onto surface:
 *
 *  GenericTransfer transfer( 0, surface_data_ptr);
 *  transfer.SetDestination( surface_width, surface_height, surface_pitch, surface_format, 0, 0 );
 *  transfer.SetSource( source_width, source_height, source_pitch, source_format, 0, 0);
 *  transfer.Fetch( source_data_ptr, Rect( 0, 0, surface_width, surface_height));
 *  if ( transfer.NeedTransform())
 *  {
 *      transfer.Transform();
 *      transfer.StoreWholeImage();
 *  }
 *
 *
 *  2) Read graphics from surface:
 *
 *  GenericTransfer transfer( surface_data_ptr, 0 );
 *  transfer.SetSource( surface_width, surface_height, surface_pitch, surface_format, 0, 0 );
 *  transfer.SetDestination( dest_width, dest_height, dest_pitch, dest_format, 0, 0);
 *  if ( transfer.NeedTransform())
 *  {
 *      transfer.FetchWholeImage();
 *      transfer.Transform();
 *  }
 *  transfer.Store( dest_ptr, Rect( 0, 0, surface_width, surface_height));
 *
 *\endverbatim      
 *
 *
 *
 */
class USELESS_API GenericTransfer : public PixelTransfer
{
public:
    /*! Source and destination data pointers are optional, and
     *  they might be used to allow GenericTransfer to access
     *  directly those pixel planes.
     */
    GenericTransfer( void *source=0, void *destination=0 );

    //! Obtain transfer type information (combination of flags)
    virtual int  Info() { return SOURCE+DESTINATION+TRANSFORM; }

    //! Fetch data from source
    virtual void Fetch( const void *source, const Rect &area );

    //! Store data into destination
    virtual void Store( void *destination, const Rect &area );

    //! Transform data from input buffer to destination buffer
    virtual void Transform();

    //! Set source pixel-stream type
    virtual void SetSource( int  width,
                            int  height,
                            int  pitch,
                            int  pixel_format, //!< use ImageFormat::Format or BPP
                     const void *palette = 0,
                            int  colors = 0);
    
    //! Set destination pixel-stream type
    virtual void SetDestination( int  width,
                                 int  height,
                                 int  pitch,
                                 int  pixel_format, //!< use ImageFormat::Format or BPP
                                void *palette = 0,
                                 int  colors = 0);

    //! Get source pixel-stream type
    virtual void GetSource( int  *width,
                            int  *height,
                            int  *pixel_format,
                     const void **palette,
                            int  *colors );

    //! Get destination pixel-stream type
    virtual void GetDestination( int  *width,
                                 int  *height,
                                 int  *pixel_format,
                                void **palette,
                                 int  *colors );

    //! Set source buffer pointer
    virtual void SetSourcePointer( void *source);

    //! Set destination area pointer
    virtual void SetDestinaitonPointer( void *destination);
    
    virtual void SetSpeedHint( bool fastest=false) { _fastest=fastest; }

    //! Tell if we need to call Transform() and then Store()
    bool NeedTransform() const;
    
    void FetchWholeImage();
    void StoreWholeImage();

protected:
    void Initialize( void *source, void *destination );

private:
    int _s_width;
    int _s_height;
    int _s_pitch;
    int _s_pixel_format;
    int _s_palette_colors;
    const void *_s_palette;

    int _d_width;
    int _d_height;
    int _d_pitch;
    int _d_pixel_format;
    int _d_palette_colors;
    void *_d_palette;

    MemBlock _input_buffer;
    MemBlock _output_buffer;

    void *_input_dest;
    void *_out_source;

    void *_source;
    void *_destination;

    bool _need_transform;
    bool _need_fetch;
    bool _need_store;
    
    bool _fastest;

    // Inner helper method
    template< class SrcPixelType, class DstPixelType >
    void TransformImp( const SrcPixelType&, const DstPixelType& );

    void PrepareConverter( int );
};

};//namespace Useless
#endif//__INCLUDED_USELESS_GENERIC_TRANSFER_H__
