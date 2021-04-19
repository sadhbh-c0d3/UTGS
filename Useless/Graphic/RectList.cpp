#include "UselessPch.h"

#include <set>
#include "Useless/Graphic/RectList.h"

namespace Useless {


/*! This function tesselates list of rectangles into belts of unified y's
 *
 *  RangesList = Array
 *
 *  (
 *
 *      Range(y1,y2) => Array( Range(x11,x12), Range(x12,x13), ... )
 *
 *      Range(y2,y3) => Array( Range(x21,x22), Range(x22,x23), ... )
 *
 *      ..........................................................
 *  )
 */
RangesList RectList::GetRanges( Range y_range ) const
{
    if ( this->empty() ) { return RangesList(); }

    bool clipped = y_range!=Range(0,0);

    typedef std::set<int> Ints;
    Ints    sorted_ys;
    int k = this->size();

    if (!clipped)
    {
        while (k--)
        {
            sorted_ys.insert( this->at(k).GetY1() );
            sorted_ys.insert( this->at(k).GetY2() );
        }
    }
    else
    {
        while (k--)
        {
            int y1 = this->at(k).GetY1();
            int y2 = this->at(k).GetY2();

            if ( y1 > y_range.first && y1 < y_range.second ) { sorted_ys.insert( y1 ); }
            if ( y2 > y_range.first && y2 < y_range.second ) { sorted_ys.insert( y2 ); }
        }
        sorted_ys.insert( y_range.first );
        sorted_ys.insert( y_range.second );
    }

    RangesList rangeslist;
    if ( sorted_ys.empty() ) { return rangeslist; }

    Ints::iterator    yi=sorted_ys.begin();
    int y1,y2 = *yi++;

    for ( ; yi!=sorted_ys.end(); ++yi)
    {
        y1 = y2; y2 = *yi;
    
        RangeList belt_ranges;

        int k = this->size(); while(k--)
        {
            Rect kr = this->at(k);
            if ( kr.GetY1()<y2 && kr.GetY2()>y1 )
            {
                Range rg = Range(kr.GetX1(), kr.GetX2() );
                belt_ranges.Insert(rg);
            }
        }

        k = belt_ranges.Size();
        int x1 = 0;
        int x2 = 0;
        int opened = 0;
        RangeList opt_ranges;

        for ( int i=0; i!=k; ++i )
        {
            Range rg = belt_ranges[i];
            if (!opened++)
            {
                x1 = rg.first;
                x2 = rg.second;
            }
            else
            {
                if ( rg.first<=x2 && rg.second>=x1 )
                { 
                    if ( x1 > rg.first)  x1 = rg.first;
                    if ( x2 < rg.second) x2 = rg.second;
                }
                else 
                {
                    opt_ranges.Insert( Range(x1,x2) );
                    x1 = rg.first;
                    x2 = rg.second;
                }
            }
        }
        if ( opened) { opt_ranges.Insert( Range(x1,x2) );  }

        if (!opt_ranges.Empty()) rangeslist.Insert( Range(y1, y2), opt_ranges );

    }

    return rangeslist;
}

RectList RectList::GetOptimized() const
{
    RectList optimized_rects;

    RangesList rgl = GetRanges();
    
    for ( RangesList::Iterator iter = rgl.Begin(); !!iter; ++iter )
    {
        Range yr = iter.Key();
        int jj = iter.Value().Size();
        while ( jj-- )
        {
            Range xr = iter.Value()[jj];
            Rect rc( xr.first, yr.first, xr.second-xr.first, yr.second-yr.first );
            optimized_rects.push_back(rc);
        }
    }

    return optimized_rects;
}

};//namespace Useless
