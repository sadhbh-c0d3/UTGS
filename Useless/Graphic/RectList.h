#ifndef __INCLUDED_USELESS_RECT_LIST_H__
#define __INCLUDED_USELESS_RECT_LIST_H__

#pragma warning(disable:4786)       // disable 'indentifier was truncated...'
#include <vector>
#include "Useless/Graphic/Rect.h"
#include "Useless/Util/Storage/AssocVector.h"
#include "Useless/Util/Storage/SortedVector.h"

namespace Useless {

/*! Creates a typename for point list*/
typedef std::vector< Pos >  PointList;

/*! Dummy Range class*/
typedef struct std::pair< long, long> Range;

/*! Creates a typename for range list*/
typedef SortedVector< Range > RangeList;

/*! Creates a typename for list of range-lists*/
typedef AssocVector< Range, RangeList > RangesList;

/*! Simple rectangle list*/
class USELESS_API RectList : public std::vector< Rect >
{
public:
    RectList() {}
    RectList( int n ): std::vector< Rect >(n) {}
    RectList( const std::vector< Rect > &rectlist ): std::vector< Rect >(rectlist) {}
    RectList( const Rect &r ) { push_back(r); }

    RangesList GetRanges( Range y_range = Range(0,0) ) const;

    RectList GetOptimized() const;

#ifndef WIN32
    const Rect& at(int i) const { return std::vector<Rect>::operator[](i);}
    Rect& at(int i) { return std::vector<Rect>::operator[](i);}
#endif

};

};// namespace Useless

#endif //__INCLUDED_USELESS_RECT_LIST_H__
