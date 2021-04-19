#ifndef __INLCUDED__USELESS__THIN_LINE_H__
#define __INLCUDED__USELESS__THIN_LINE_H__

#include "Useless/Graphic/Pixel/Pixel.h"

namespace Useless {

/*
--------------------------------------------------------------*/
template < class PixelPtr >
void DrawLine( PixelPtr p_pix, NormalRGB cRGB, Pos ptA, Pos ptB )
{
    Pos delta = ptB - ptA;
    Pos abs_d = delta.Abs();
    bool flip = false;

    if ( abs_d.y > abs_d.x )
    {
        if ( delta.x < 0 ) { flip = true; }
        if ( delta.y < 0 ) { flip = !flip; std::swap(ptA,ptB); }
    }
    else
    {
        if ( delta.y < 0 ) { flip = true; }
        if ( delta.x < 0 ) { flip = !flip; std::swap(ptA,ptB); }
    }

    p_pix += ptA;

    //-- calculates orthogonal vector
    //Point<float> p = (ptB - ptA).Swap(); p.x=-p.x;
    //float l = p.Sqr();
    //p.x*=abs(p.x); p.y*=abs(p.y);  p*=4; p/=l;
    //Pos ip = p;
    //-- get two background samples
    //delta = ptB - ptA;
    //NormalRGB c1 = (*(p_pix + (delta/2 - ip))).Get().c;
    //NormalRGB c2 = (*(p_pix + (delta/2 + ip))).Get().c;
    //NormalRGB cBG = c1.Blend( 127, c2 );

    delta = (ptB - ptA).Abs();

    //-- If it's horizontal line
    if ( delta.y == 0 )
    {
        for ( int u=0; u<=delta.x; ++u )
        {
            *p_pix = cRGB;
            ++p_pix;
        }
    }
    //-- If it's vertical line
    else if ( delta.x == 0 )
    {
        for ( int v=0; v<=delta.y; ++v )
        {
            *p_pix = cRGB;
            p_pix.Next();
        }
    }
    //-- If it's diagonal line
    else if ( delta.y == delta.x )
    {
        for ( int u=0; u<=delta.x; ++u )
        {
            *p_pix = cRGB;
            if (flip) p_pix.Prev();
            else      p_pix.Next();
            ++p_pix;
        }
    }
    //-- If it's about vertical line
    else if ( delta.y > delta.x + delta.y/2 )
    {
        int inc = (delta.y * 1024) / delta.x;
        int u = 0;
        int v = 0;
        int z = 0;
        int p_v = (z - inc)/1024;
        int n_v = 0;
        int l_v = delta.y;

        while ( u <= delta.x )
        {
            z += inc;
            n_v = z/1024;

            PixelPtr p = p_pix; p += Pos( (flip)? -u:u, p_v);

            int b = 0;
            int h = (n_v - p_v)/2;
			int bi = (2*255*1024)/(n_v - p_v + 1);
			int bl = 0;
            int i = 0;
            int j = p_v;

            for ( ; j<0; ++i, ++j )
            {
                 bl+=bi;
                 p.Next();
            }

            for ( ; i<=h; ++i, ++j )
            {
                 b = Saturate( (bl+=bi)/1024 );
                 p.Write( cRGB.Blend( b, p.Read().Get().c ) );
                 p.Next();
            }

            for ( ; j<=n_v && j<=l_v; ++j )
            {
                 b = Saturate( (bl-=bi)/1024 );
                 p.Write( cRGB.Blend( b, p.Read().Get().c ) );
                 p.Next();
            }
            
            p_v = v;
            v = n_v;
            ++u;
        }
    }
    //-- If it's about horizontal line
    else if ( delta.x > delta.y + delta.x/2 )
    {
        int inc = (delta.x * 1024) / delta.y;
        int u = 0;
        int v = 0;
        int z = 0;
        int p_u = (z - inc)/1024;
        int n_u = 0;
        int l_u = delta.x;

        while ( v <= delta.y )
        {
            z += inc;
            n_u = z/1024;

            PixelPtr p = p_pix; p += Pos(p_u, (flip)? -v:v );

            int b = 0;
            int h = (n_u - p_u)/2;
			int bi = (2*255*1024)/(n_u - p_u + 1);
			int bl = 0;
            int i = 0;
            int j = p_u;

            for ( ; j<0; ++i, ++j )
            {
				 bl+=bi;
                 ++p;
            }

            for ( ; i<=h; ++i, ++j )
            {
				 b = Saturate( (bl+=bi)/1024 );
                 p.Write( cRGB.Blend( b, p.Read().Get().c ) );
                 ++p;
            }

            for ( ; j<=n_u && j<=l_u; ++j )
            {
				b = Saturate( (bl-=bi)/1024 );
                 p.Write( cRGB.Blend( b, p.Read().Get().c ) );
                 ++p;
            }
            
            p_u = u;
            u = n_u;
            ++v;
        }

    }
    //-- If it's diagonal line ( about vertical )
    else if ( delta.y > delta.x )
    {
# ifdef THIN_LINE_DEBUG
        cRGB = MakeRGB(0,0,255); 
# endif
        int dx = (delta.x * 1024) / (delta.y - delta.x);
        int dy = (delta.y * 1024) / (delta.y - delta.x);
        int c = 0;
        int x = 0;
        int y = 0;
        int zx = 0;
        int zy = 0;
        int p_x = (zx - dx)/1024;
        int n_x = 0;
        int l_x = delta.x;
        int p_y = (zy - dy)/1024;
        int n_y = 0;
        int l_y = delta.y;

        while ( y <= delta.y )
        {
            zx += dx;       zy += dy;
            n_x = zx/1024;  n_y = zy/1024;

            PixelPtr p = p_pix; p += Pos( (flip)? -p_x+1:p_x-1, p_y );

            int b = 0;
            int h = (n_y - p_y)/2;
			int bi = (2*255*1024)/(n_y - p_y +1);
			int bl = 0;
            int i = 0;
            int j = p_y;

            for ( ; j<0; ++i, ++j )
            {
                bl+=bi;
                (flip)? --p:++p; p.Next();
            }

            for ( ; i<=h; ++i, ++j )
            {
                 b = Saturate( (bl+=bi)/1024 );
                 p.Write( cRGB.Blend( b, p.Read().Get().c ) );
                (flip)? --p:++p;
                p.Next();
            }

            for ( ; j<=n_y && j<=l_y; ++j )
            {
                 b = Saturate( (bl-=bi)/1024 );
                 p.Write( cRGB.Blend( b, p.Read().Get().c ) );
                (flip)? --p:++p;
                p.Next();
            }
                        
            p_x = x;    p_y = y;
            x = n_x;    y = n_y;
        }
    }
    //-- If it's diagonal line ( about horizontal )
    else if ( delta.x > delta.y )
    {
# ifdef THIN_LINE_DEBUG
        cRGB = MakeRGB(255,0,0);
# endif
        int dx = -(delta.x * 1024) / (delta.y - delta.x);
        int dy = -(delta.y * 1024) / (delta.y - delta.x);
        int c = 0;
        int x = 0;
        int y = 0;
        int zx = 0;
        int zy = 0;
        int p_x = (zx - dx)/1024;
        int n_x = 0;
        int l_x = delta.x;
        int p_y = (zy - dy)/1024;
        int n_y = 0;
        int l_y = delta.y;

        while ( x <= delta.x )
        {
            zx += dx;       zy += dy;
            n_x = zx/1024;  n_y = zy/1024;

            PixelPtr p = p_pix; p += Pos( p_x, (flip)? -p_y+1:p_y-1 );

            int b = 0;
            int h = (n_x - p_x)/2;
			int bi = (2*255*1024)/(n_x - p_x +1);
			int bl = 0;
            int i = 0;
            int j = p_x;

            for ( ; j<0; ++i, ++j )
            {
				bl+=bi;
                (flip)? p.Prev():p.Next(); ++p;
            }

            for ( ; i<=h; ++i, ++j )
            {
                 b = Saturate( (bl+=bi)/1024 );
                 p.Write( cRGB.Blend( b, p.Read().Get().c ) );
                (flip)? p.Prev():p.Next();
                ++p;
            }

            for ( ; j<=n_x && j<=l_x; ++j )
            {
                 b = Saturate( (bl-=bi)/1024 );
                 p.Write( cRGB.Blend( b, p.Read().Get().c ) );
                (flip)? p.Prev():p.Next();
                ++p;
            }
                        
            p_x = x;    p_y = y;
            x = n_x;    y = n_y;
        }
    }
}

};//namespace Useless
#endif//__INLCUDED__USELESS__THIN_LINE_H__
