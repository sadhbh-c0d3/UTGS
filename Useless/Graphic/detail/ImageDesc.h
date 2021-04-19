#ifndef __INCLUDED_IMAGE_DESC_H__
#define __INCLUDED_IMAGE_DESC_H__

namespace Useless {

/*! \ingroup GfxPlanes
 *  \verbatim
 *  Class defines constatns used to determine pixelformat of an image.
 *
 *  NOTE:
 *      ImageFormat::Format casts directly to INTEGER so 
 *      if you have 32bpp then ImageFormat::Format(32) = B8G8R8A8
 *      or if you have 16bpp then ImageFormat::Format(16) = B5G6R5 and so on;
 *  \endverbatim
 */
class USELESS_API ImageFormat       
{
public:   
    /*! \enum Format
     *
     *  Rule: bytes_per_pixel = (Format+7)/8
     */
    enum Format 
    {   //32 bpp
        B8G8R8A8 = 32,
        R8G8B8A8 = 31,
        B8G8R8A0 = 30,
        R8G8B8A0 = 29,
        ALPHA8   = 28,
        ALPHA8PM = 27,
        //24 bpp
        B8G8R8   = 24,
        R8G8B8   = 23,
        //16 bpp
        R5G6B5   = 16,
        A1R5G5B5 = 15,
        R5G5B5   = 15,
        A4R4G4B4 = 14,
        A4B4G4R4 = 13,
        ALPHA4   = 12,
        ALPHA1   = 11,
        //8 bpp
        SINGLE8  = 8,
        INDEX8   = 8,
        //---
        INVALID  = 0
    };

//---- IMAGE FORMAT CLASS
public:    
    ImageFormat( int formatID): _format( Format(formatID))
    {}
    
    ImageFormat( Format format): _format(format)
    {}
    
    ImageFormat( unsigned long maskR,
                 unsigned long maskG,
                 unsigned long maskB,
                 unsigned long bitCount)
    {
        _format = Recognize( maskR, maskG, maskB, bitCount);
    }
    
    int Bytes()    const { return GetBpp(_format); }
    int Bits()     const { return Bytes() * 8; }
    int FormatID() const { return _format; }
    
    Format GetFormat() const { return _format;}
    operator Format() const { return _format;}

private:
    Format _format;


//---- IMAGE FORMAT STATIC INTERFACE
public:    
    // Select appropriate type based on color masks for each  color
    static Format Recognize( unsigned long maskR, unsigned long MaskG,
                             unsigned long MaskB, unsigned long bitcount);

    // Get color masks for each color
    static void GetColorMask( ImageFormat::Format fmt, unsigned long *maskR, 
                             unsigned long *MaskG, unsigned long *MaskB );

    // Get number of bits per each channel
    static void GetBitCount( ImageFormat::Format fmt, int *bitsR, int *bitsG, int *bitsB );

    // Get number of Bytes per pixel
    static int GetBpp( ImageFormat::Format fmt );

    // Encode RGB pixel in given format
    static unsigned long Encode( ImageFormat::Format fmt,
                                 unsigned char r,
                                 unsigned char g,
                                 unsigned char b );

    // Encode RGB pixel given as 32 bit long number into given format
    static unsigned long Encode( ImageFormat::Format fmt, unsigned int color_in );

    // Decode component colors from specified format
    static void Decode( ImageFormat::Format fmt, unsigned long color_in,
                        unsigned char *r,
                        unsigned char *g,
                        unsigned char *b );

    // Decode component colors into 32-bit long number from specified format
    static unsigned long Decode( ImageFormat::Format fmt, unsigned long color_in );
};


/*! \class ImageDesc
 *  Dummy class, small size. Used to inform about basic image parameters, such as 
 *  dimmentions, pixelformat, color-key
 */
class ImageDesc
{
public:
    ImageDesc(): width(0), height(0), is_color_keyed(0), color_key(0), format(ImageFormat::INVALID) {}
    ImageDesc(int w, int h, int f, bool ck=false, unsigned int c=0L): 
        width(w), height(h), format(ImageFormat::Format(f)), is_color_keyed(ck), color_key(c) {}

    bool operator==(const ImageDesc& desc) const
    {
        return desc.color_key==color_key && desc.format==format && desc.is_color_keyed==is_color_keyed &&
               desc.width==width && desc.height==height;
    }
    bool operator!=(const ImageDesc& desc) const { return !( desc==(*this) ); }

    int                     width;
    int                     height;
    ImageFormat::Format     format;
    bool                    is_color_keyed;
    unsigned long           color_key;      
};

};// namespace Useless
#endif  //__INCLUDED_IMAGE_DESC_H__

