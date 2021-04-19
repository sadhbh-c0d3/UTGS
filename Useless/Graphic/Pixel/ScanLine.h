#ifndef __INCLUDED__USELESS_SCANLINE_H__
#define __INCLUDED__USELESS_SCANLINE_H__

#include "Useless/Graphic/Pixel/PixelPointer.h"
#include "Useless/Graphic/Pixel/Edge.h"

#include <vector>
#include <algorithm>

namespace Useless 
{
    /*! Scanline-like polygon filling algorithm
     *  Uses abstract PixelType and PixelShader
     *
     *  Algorithm draws onto surface delivered via pointer-to-data
     *
     ------------------------------------------------------------------------------------------------*/
    template< class PixelShader, class EdgeCreator >
        void ScanLine( void *pData,
                int  pitch,
                PixelShader  pixel_shader,
                const EdgeCreator &edge_creator,
                const Rect &clip_rect  )
        /*----------------------------------------------------------------------------------------------*/

        {
            typedef typename PixelShader::PixelType PixelType;
            int Ymin;
            int Ymax;
            std::vector< EdgeIterator > _edgeiterators;
            edge_creator.GetEdges( _edgeiterators, Ymin, Ymax );

            PixelPointer<PixelType> pScanLine( (PixelType *)pData, pitch, clip_rect);
            pScanLine.MoveNLines(Ymin);

            size_t line_size = clip_rect.GetW() * sizeof(PixelType);
            //MemBlock line_cache( line_size );
            PixelType *p_line_cache;// = (PixelType*)line_cache.GetPtr();
            PixelType *p_line = (PixelType*)pData;
            ((Types::UBYTE *&)p_line) += pitch * Ymin;


            // for each scanline
            for ( int Y=Ymin; Y<=Ymax; ++Y, pScanLine.NextLine() )
            {
                int i=0, n=_edgeiterators.size();
                std::vector< CrossPoint > crosspnts; crosspnts.reserve(n);

                for ( ; i<n; ++i )
                {
                    EdgeIterator &ei = _edgeiterators.at(i);
                    if ( ei.InRange(Y) || ( Y==Ymax && ei.LastPt(Y) ) )
                    {
                        ei.Next();
                        CrossPoint cp( ei.GetInteger(), ei.LeftBlend(Ymax), ei.RightBlend(Ymax) );
                        crosspnts.push_back(cp);
                    }
                }
                std::sort( crosspnts.begin(), crosspnts.end() );

                if ( Y>=clip_rect.GetY1() && Y<clip_rect.GetY2() )
                {
                    p_line_cache = p_line + clip_rect.GetX1();

                    for ( int i=0, n=crosspnts.size(); i<n-1; i+=2 )
                    {
                        int X0 = crosspnts.at(i).X;
                        int X1 = crosspnts.at(i+1).X;
                        int fillL = 256 - (X0 & 0xFF);
                        int fillR = (X1 & 0xFF);
                        X0 >>=8; X1 >>=8;
                        int blendL = crosspnts.at(i).BlendL;
                        int blendR = crosspnts.at(i+1).BlendR;

                        PixelPointer<PixelType> pPixel = pScanLine;
                        pPixel.MoveNPixels(X0);

                        pixel_shader.SetScanLine( X0, X1, Y);

                        /*for ( int i=0, W=X1-X0; i<W; ++i )
                          {
                         *pPixel = pixel_shader.GetColor( pPixel, i);
                         pPixel.NextPixel();
                         }*/
                        {
                            if ( blendL < 0)
                            {
                                for (int j=0, bl=blendL>-2? fillL:256, dbl=256/blendL; j>blendL; --j, bl+=dbl)
                                {
                                    *pPixel(j,0) = pixel_shader.GetColor( pPixel, j).Get().c.Blend( bl, pPixel(j,0)->Get().c);
                                }
                            }
                            else
                            {
                                *pPixel = pixel_shader.GetColor( pPixel, 0).Get().c.Blend( fillL, pPixel->Get().c);
                            }
                            pPixel.NextPixel();
                            int i,W;
                            for ( i=1, W=X1-X0; i<=W; ++i )
                            {
                                *pPixel = pixel_shader.GetColor( pPixel, i);
                                pPixel.NextPixel();
                            }
                            if ( blendR > 0)
                            {
                                for (int j=0, bl=blendR<2? fillR:256, dbl=256/blendR; j<blendR; ++j, bl-=dbl)
                                {
                                    *pPixel(j,0) = pixel_shader.GetColor( pPixel, W+j).Get().c.Blend( bl, pPixel(j,0)->Get().c);
                                }
                            }
                            else
                            {
                                *pPixel = pixel_shader.GetColor( pPixel, W).Get().c.Blend( fillR, pPixel->Get().c);
                            }
                        }
                    }
                }
                ((Types::UBYTE *&)p_line) += pitch;
            }
        }


};//namespace 

#endif//__INCLUDED__USELESS_SCANLINE_H__
