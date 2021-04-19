#ifndef __INCLUDED_USELESS_DT_PAINTER_H__
#define __INCLUDED_USELESS_DT_PAINTER_H__

#include "Useless/GUI/Misc/DataTypes.h"

namespace Useless {

/*! Data Types Painter - paints given DT in given position
 *  Should be instatiated only temporarly (as inline)
 */
class USELESS_API DT_Painter :
    public Visitor< DataObject >,
    public Visitor< DT_Text >,
    public Visitor< DT_Image >,
    public Visitor< DT_PayEntry >,
    public Visitor< DT_PayEntries >,
    public Visitor< DT_SingleRow >
{
public:
    DT_Painter( const Painter &ref_painter,
                      Font    &ref_header_font,
                      Font    &ref_normal_font,
                const Rect    &ref_area ) //!< height is not valid parameter and may differ
        :_painter(ref_painter),
         _header_font(ref_header_font),
         _normal_font(ref_normal_font),
         _area(ref_area)
        {}

    /*! This data object is invisible since its type is unknown
     */
    virtual int Visit( DataObject & )
    {
        _filled_area = Rect();
        return 0;
    }

    /*! Paint text using normal font.
     *  Wrap lines if area width is too narrow.
     *  Return height of painted area.
     */
    virtual int Visit( DT_Text &text )
    {
        int line_space = 2;//W/A
        FontPainter fp( _normal_font, text, _area.GetW() );
        int unwrap_width = fp.GetStatus().GetWidth();

        if ( unwrap_width < fp.GetWidth() )
        {
            int where = Place( unwrap_width, text._placement );
            Pos xy = _area.GetPos() + Pos(where,0);
            fp.Paint( _painter, xy, line_space );
            _filled_area = Rect(0,0, unwrap_width, fp.GetHeight() ) + xy;
        }
        else
        {
            if ( text._placement==50 )
            {
                fp.Paint( _painter, _area.GetPos()+Pos(_area.GetW()/2,0), line_space, FontPainter::CENTER );
            }
            else if( text._placement >50 )
            {
                fp.Paint( _painter, _area.GetPos()+Pos(_area.GetW(),0), line_space, FontPainter::RIGHT );
            }
            else
            {
                fp.Paint( _painter, _area.GetPos(), line_space, FontPainter::LEFT );
            }

            _filled_area = Rect(0,0, _area.GetW(), fp.GetHeight() );
        }
        return fp.GetHeight() + line_space;
    }

    /*! Paint image, Match area left-upper corrner position.
     */
    virtual int Visit( DT_Image &image )
    {
        int where = Place( image.GetWidth(), image._placement );
        _filled_area = Rect( _area.GetX1()+where, _area.GetY1(), image.GetWidth(), image.GetHeight() );
        _painter.ProjectImage( _filled_area.GetX1(), _filled_area.GetY1(), image );
        return _filled_area.GetH();
    }

    /*! Paint pay entry name => value
     */
    virtual int Visit( DT_PayEntry &entry )
    {
        int where_name = Place( _header_font.GetWidth(entry._name), entry._name_placement );
        _header_font.Paint( _painter, entry._name, _area.GetPos() + Pos(where_name,0) );
        int line_space = 2;//W/A

        int where_value = Place( _normal_font.GetWidth(entry._value), entry._value_placement );
        Pos pos( where_value, _header_font.GetHeight() + line_space );
        _normal_font.Paint( _painter, entry._value, _area.GetPos() + pos );

        _filled_area = Rect(0,0, _area.GetW(), pos.y + _normal_font.GetHeight() );
        return _filled_area.GetH() + line_space;
    }

    /*! Paint pay entries name => (value[0],value[1],...)
     */
    virtual int Visit( DT_PayEntries &entries )
    {
        int where_name = Place( _header_font.GetWidth(entries._name), entries._name_placement );
        _header_font.Paint( _painter, entries._name, _area.GetPos() + Pos(where_name,0) );
        int line_space = 2;//W/A
        Pos pos( 0, _header_font.GetHeight() + line_space );
        
        int cell_width = _area.GetW()/entries._columns;
        int ln_height = _normal_font.GetHeight();

        int line, col;
        const Text *p=&*entries._values.begin(), *q=&*entries._values.end();

        // calculate width for each column
        std::vector<int> column_widths(entries._columns);
        memset( &*column_widths.begin(), 0, entries._columns*sizeof(int) );

        for ( col=0; p!=q; (++col)%=entries._columns, ++p )
        {
            int w = _normal_font.GetWidth(*p);
            if ( column_widths[col] < w ) { column_widths[col]=w; }
        }

        // paint every value dispatching into columns
        p = &*entries._values.begin();

        for ( line=0, col=0; p!=q; (++col)%=entries._columns )
        {
            int ox = Place( cell_width, column_widths[col], entries._name_placement );
            //Pos offset( (cell_width - ( column_widths[col] ))/2, 0 );
            int w = _normal_font.GetWidth(*p);
            Pos offset( ox + Place(column_widths[col], w, entries._value_placement), 0 );
            _normal_font.Paint( _painter, *p, _area.GetPos() + pos + offset );
            
            if (col==entries._columns-1)
            {
                if ( ++p!=q )
                {
                    ++line;
                    pos.x = 0;
                    pos.y += ln_height;
                    pos.y += line_space;
                }
            }
            else
            {
                ++p; pos.x += cell_width;
            }
        }
        _filled_area = Rect(0,0, _area.GetW(), pos.y + ln_height );
        return _filled_area.GetH() + line_space;
    }

    virtual int Visit( DT_SingleRow &row );// defined in DT_SingleRow.cpp


    /*! Calculate left position.
     *  width = element width
     *  placement = % of full space that is on the left to an  element
     */
    int Place( int width, int placement )
    {
        assert( placement>=0 && placement<=100 );
        int work_area = _area.GetW() - width;
        return (placement * work_area)/100;
    }

    int Place( int area_width, int width, int placement )
    {
        assert( placement>=0 && placement<=100 );
        int work_area = area_width - width;
        return (placement * work_area)/100;
    }

    Rect _filled_area;

private:
    const Painter &_painter;
          Font    &_header_font;
          Font    &_normal_font;
    const Rect    &_area;
};

};//namespace Useless
#endif//__INCLUDED_USELESS_DT_PAINTER_H__
