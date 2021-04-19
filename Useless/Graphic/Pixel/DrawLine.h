#ifndef __INCLUDED_USELESS_DRAW_LINE_H__
#define __INCLUDED_USELESS_DRAW_LINE_H__

#include "Useless/Graphic/Pixel/Pixel.h"
#include "Useless/Graphic/Pixel/PixelPointer.h"
#include <vector>

namespace Useless {

/*! DrawLine
 *  Very simple line drawing Bresenham algorithm
 *  Uses abstract PixelType
 *  Algorithm draws line in any direction onto surface delivered via pointer-to-data
 -----------------------------------------------------------------------------------------*/
template< class PixelType >
void DrawLine( 
                Useless::Types::UBYTE *pData,
                               int  pitch, 
                         PixelType  rgb_pixel,
                               int  thickness, 
                        const  Pos &q1,
                        const  Pos &q2,
                        const Rect &clip_rect  )
/*----------------------------------------------------------------------------------------*/
{
    PixelType pixel_color = rgb_pixel;
    thickness -= 2;

    PixelPointer<PixelType> pDest( (PixelType *)pData, pitch, clip_rect );
    
    int thd = thickness /2 +1;
    int thr = thickness %2 -1;

    Pos delta = q2-q1;
    Pos abs_delta = delta.Abs();
    Pos p1;
    Pos p2;
    if (!abs_delta) return;

    bool do_clipping = !!clip_rect;
    Useless::Rect line_rect( q1.x<q2.x ? q1.x:q2.x, q1.y<q2.y ? q1.y:q2.y, abs_delta.x+1, abs_delta.y+1 );
    if ( do_clipping && !(clip_rect && line_rect) )  { return; }

    bool is_diagonal   = double(abs_delta.x)/double(abs_delta.y) > 0.5 && double(abs_delta.y)/double(abs_delta.x) > 0.5;
    bool is_horizontal = abs_delta.x > abs_delta.y;
    bool is_vertical   = abs_delta.y > abs_delta.x;

    //-- calculates orthogonal vector
    Point<float> p = (q2 - q1).Swap(); p.x=-p.x;
    float l = p.Sqr();
    p.x*=abs(p.x); p.y*=abs(p.y);  p*=4; p/=l;
    Pos ip = p;

    Pos m = (q2+q1)/2;
    NormalRGB cBG_A = pDest( m.x + ip.x, m.y + ip.y )->Get().c; // Replaced pA->Get().c
    NormalRGB cBG_B = pDest( m.x - ip.x, m.y - ip.y )->Get().c; // Replaced pB->Get().c
    cBG_A.Blend( 127, cBG_B ); cBG_B = cBG_A;

    int similarity = cBG_A.Similarity( rgb_pixel.Get().c );
    bool use_fast_aa = ( delta.Sqr() < 35*35 ) && ( similarity > 32*32 );

    //int color_counter = 0;
    //std::vector< NormalRGB > _bg_pixels;
    //NormalRGB cBG_A, cBG_B;
   
    if ( is_diagonal )   // draw line about 45 deegrees
    {
           p1 = delta.x>0 ?    q1 :  q2;
           p2 = delta.x>0 ?    q2 :  q1;
        delta = delta.x>0 ? delta : -delta;

        pDest.MoveNPixels(p1.x-1);
        pDest.MoveNLines(p1.y);

        bool flip = delta.y<0;
        int Dx = delta.y? ( abs_delta.x<<8 )/abs_delta.y : 0;
        int Dy = delta.x? ( abs_delta.y<<8 )/abs_delta.x : 0;
        int X = 0;//-(1<<8);
        int Y = 0;//-(1<<8);
        int A = 0;
        int B = 0;
        PixelPointer<PixelType> pA;
        PixelPointer<PixelType> pB;

        //-- Fill bg color table
        //{int n = abs_delta.x / 8;
        //for ( int i=0, ix=p1.x; i<=n; ++i, ix+=8 )
        //{
        //    _bg_pixels.push_back( pDest(ix, (Dy*ix)>>8 )->Get().c );
        //    ix += 8;
        //}}

#ifdef DEBUG_LINES
        pixel_color = MakeRGB( 255, 0, (flip)? 255:0 );
#endif
        if (flip) pDest.NextPixel().NextLine().NextLine();

        while ( X <= (abs_delta.x<<8)  )
        {
            //if ( color_counter++ % 8 == 0)
            //{
            //    cBG_A = cBG_B = _bg_pixels[color_counter/8];
            //}

            A = ((Y*Dx)>>8) - X;
            B = ((X*Dy)>>8) - Y;

            int fillA = 255-(A+64);
            int fillB = 255-fillA+128;

            if ((thickness-1)>0) 
            {
                thd = (thickness-1) /2 +1;
                *pDest = pixel_color;
                int i;
                if ((thickness-1)%2)
                {   
                    for ( i=1; i<thd; ++i )
                    {
                        *pDest(-i, flip? -i:i) = pixel_color;
                        *pDest( i, flip? i:-i) = pixel_color;
                    }
                    pA = pDest(-i, flip? -i:i);
                    pB = pDest( i, flip? i:-i);
                }
                else
                {
                    for ( i=1; i<thd; ++i )
                    {
                        *pDest(-i+1, flip? -i:i) = pixel_color;
                        *pDest( i, flip? i-1:-i+1) = pixel_color;
                    }
                    pA = pDest(-i+1, flip? -i:i);
                    pB = pDest( i, flip? i-1:-i+1);
                }
            }

            if ( thickness>1 )
            {
                fillA = Saturate(fillA);
                fillB = Saturate(fillB);
            }
            else if ( thickness==1 )
            {
                pA = pDest(0, flip ? -thd : thd);
                pB = pDest(thd, 0 );
                fillA = Saturate(fillA);
                fillB = Saturate(fillB);
            }
            else if ( thickness==0 )
            {
                pA = pDest(0, flip ? -thd : thd);
                pB = pDest(thd, 0 );
                fillA = Saturate(fillA)/2+Saturate(fillA)/4;
                fillB = Saturate(fillB)/2+Saturate(fillB)/4;
            }
            else if ( thickness==-1 )
            {
                pA = pDest(0, flip ? -thd : thd);
                pB = pDest(thd, 0 );
                fillA = Saturate(fillA)/2+Saturate(fillA)/8;
                fillB = Saturate(fillB)/2+Saturate(fillB)/8;
            }
            else
            {
                fillA = Saturate(fillA)/4+Saturate(fillA)/8;
                fillB = Saturate(fillB)/4+Saturate(fillB)/8;

                pA = pDest( -1, 0 );
                pB = pDest(  0, flip? 1:-1 );
            }

            if ( !use_fast_aa ) { cBG_A = pA->Get().c; cBG_B = pB->Get().c; }
            *pA = pixel_color.Get().c.Blend( fillA, cBG_A);
            *pB = pixel_color.Get().c.Blend( fillB, cBG_B);
            
            if ( A <= B )
            {
                flip?  pDest.PrevLine() : pDest.NextLine();
                Y += 1<<8;
            }
            else
            {
                pDest.NextPixel();
                X += 1<<8;
            }
        }

    }
    else if ( is_horizontal )
    {
           p1 = delta.x>0 ?    q1 :  q2;
           p2 = delta.x>0 ?    q2 :  q1;
        delta = delta.x>0 ? delta : -delta;

        pDest.MoveNPixels(p1.x);
        pDest.MoveNLines(p1.y+1);

        bool flip = delta.y<0;
        if (flip) { delta.y = -delta.y; }

        int d = 0;
        int D = ( delta.y<<8 )/delta.x;
        int x = 0-1;
        int y = 0-D;
        int Y = 0;
        int A = 0;
        int B = 0;
        PixelPointer<PixelType> pA;
        PixelPointer<PixelType> pB;

        //-- Fill bg color table
        //{int n = abs_delta.x / 8;
        //for ( int i=0, ix=p1.x; i<=n; ++i, ix+=8 )
        //{
        //    _bg_pixels.push_back( pDest(x, (D*x)>>8 )->Get().c );
        //    ix += 8;
        //}}

#ifdef DEBUG_LINES
        pixel_color = MakeRGB( (flip)? 255:0 ,255,0);
#endif
        //if (flip) pDest.NextPixel();

        while ( x < delta.x )
        {
            //if ( color_counter++ % 8 == 0)
            //{
            //    cBG_A = cBG_B = _bg_pixels[color_counter/8];
            //}

            x += 1;
            y += D;
            
            A = y - Y;
            B = (1<<8)+Y - y;

            (flip ? pB : pA) = pDest(1,-(thd+thr));
            (flip ? pA : pB) = pDest(1, thd );

            int fillA = 255-(A+64);
            int fillB = 255 - fillA + 64;

            if ( thickness>0 )
            {
                fillA = Saturate(fillA);
                fillB = Saturate(fillB);
            }
            else if ( thickness==0 )
            {
                fillA = Saturate(fillA);
                fillB = Saturate(fillB);
            }
            else if ( thickness==-1 )
            {
                fillA = Saturate(fillA)/2 + Saturate(fillA)/4;
                fillB = Saturate(fillB)/2 + Saturate(fillB)/4;
                (flip ? pB : pA) = pDest( 1, -(thd+thr+1) );
                (flip ? pA : pB) = pDest( 1, thd );
            }
            else
            {
                fillA = Saturate(fillA)/2;
                fillB = Saturate(fillB)/2;

                (flip ? pB : pA) = pDest(  1, 0 );
                (flip ? pA : pB) = pDest(  1, 1 );
            }

            if ( !use_fast_aa ) { cBG_A = pA->Get().c; cBG_B = pB->Get().c; }
            *pA = pixel_color.Get().c.Blend( fillA, cBG_A);
            *pB = pixel_color.Get().c.Blend( fillB, cBG_B);


            int thc = thickness;
            if ( 0<thc-- )  *( pDest(1, 0) ) = pixel_color;

            for( int iy=1; 0<thc--; ++iy)
            {
                                 *( pDest(1, iy) ) = pixel_color;
                if ( 0<thc-- ) { *( pDest(1,-iy) ) = pixel_color; }
            }

            pDest.NextPixel();
            if ( A > B )
            {
                flip ? pDest.PrevLine() : pDest.NextLine();
                Y += 1<<8;
            }
        }
    }
    else  // if is_vertical
    {
           p1 = delta.y>0 ?    q1 :  q2;
           p2 = delta.y>0 ?    q2 :  q1;
        delta = delta.y>0 ? delta : -delta;

        pDest.MoveNPixels(p1.x);
        pDest.MoveNLines(p1.y);

        bool flip = delta.x<0;
        if (flip) { delta.x = -delta.x; }

        int d = 0;
        int D = ( delta.x<<8 )/delta.y;
        int x = 0-D;
        int y = 0-1;
        int X = 0;
        int B = 0;
        int C = 0;
        PixelPointer<PixelType> pA;
        PixelPointer<PixelType> pB;

        //-- Fill bg color table
        //{int n = abs_delta.y / 8;
        //for ( int i=0, iy=p1.y; i<=n; ++i, iy+=8 )
        //{
        //    _bg_pixels.push_back( pDest(y, (D*y)>>8 )->Get().c );
        //    iy += 8;
        //}}

#ifdef DEBUG_LINES
        pixel_color = MakeRGB( 0, (flip)? 255:0, 255);
#endif
        //if (!flip) pDest.NextPixel();

        while ( y < delta.y )
        {
            //if ( color_counter++ % 8 == 0)
            //{
            //    cBG_A = cBG_B = _bg_pixels[color_counter/8];
            //}

            x += D;
            y += 1;

            B = (1<<8)+X - x;
            C = x - X;

            (flip ? pB : pA) = pDest(-(thd+thr), 1 );
            (flip ? pA : pB) = pDest( thd, 1 );

            int fillA = 255-(C+64);
            int fillB = 255 - fillA + 64;

            if ( thickness>0 )
            {
                fillA = Saturate(fillA);
                fillB = Saturate(fillB);
            }
            else if ( thickness==0 )
            {
                fillA = Saturate(fillA);
                fillB = Saturate(fillB);
            }
            else if ( thickness==-1 )
            {
                fillA = Saturate(fillA)/2 + Saturate(fillA)/4;
                fillB = Saturate(fillB)/2 + Saturate(fillB)/4;
                (flip ? pB : pA) = pDest( -(thd+thr+1), 1 );
                (flip ? pA : pB) = pDest( thd, 1 );
            }
            else
            {
                fillA = Saturate(fillA)/2;
                fillB = Saturate(fillB)/2;

                (flip ? pB : pA) = pDest(  0, 1 );
                (flip ? pA : pB) = pDest(  1, 1 );
            }

            if ( !use_fast_aa ) { cBG_A = pA->Get().c; cBG_B = pB->Get().c; }
            *pA = pixel_color.Get().c.Blend( fillA, cBG_A);
            *pB = pixel_color.Get().c.Blend( fillB, cBG_B);

            int thc = thickness;
            if ( 0<thc-- )  *( pDest( 0,1) ) = pixel_color;

            for( int ix=1; 0<thc--; ++ix)
            {
                                 *( pDest( ix,1) ) = pixel_color; 
                if ( 0<thc-- ) { *( pDest(-ix,1) ) = pixel_color; }
            }

            pDest.NextLine();
            if ( C > B )
            {
                flip ? pDest.PrevPixel() : pDest.NextPixel();
                X += 1<<8;
            }                        
        }
       
    }
}


};//namespace Useless

#endif//__INCLUDED_USELESS_DRAW_LINE_H__
