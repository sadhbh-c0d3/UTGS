#include "UselessPch.h"

#include "FacedFont.h"
#include "Useless/Util/Storage/assoc_vector.h"
#include "Useless/Util/str_conv.h"
#include "Useless/Util/stl_addon.h"
#include "Useless/File/VIFS.h" // for old fmp support
#include "Useless/File/IFileStream.h"
#include "Useless/Util/LangSelector.h" // locale support

namespace Useless {

FacedFont::~FacedFont()
{}

FacedFont::FacedFont( const RefImage &ref_image, const char *desc_file_name )
    :_faces( new ImageDotTracer( ref_image ) ), _styles( new Styles )
{
    std::assoc_vector< std::wstring, int > style_names;
    style_names[L"normal"] = NORMAL;
    style_names[L"bold"]   = BOLD;
    style_names[L"italic"] = ITALIC;
    style_names[L"underlined"] = UNDERLINE;
    style_names[L"inverted"]   = INVERT;


    _kerning = 0;
    _spacing = 0;
    _style = 0;
    _color = 0;
    _prev_color = 0;
    _offset = 0;

    std::string data( desc_file_name );

    int mode;
    int style;
    int style_offset = 0;
    int highest_index = 0;
    std::vector< int > indices;

    enum { INDICES, PROPERTIES, STYLES, STYLE, COLOR };

    if ( data.rfind(".fmp") != -1 ) //old raw fmp compatibility
    {
        SPointer<IFile> file = IFS::Instance().OpenFile(data);
        unsigned char char_code = 0;
        for ( int i=0, n=(*file).GetLen(); i!=n; ++i )
        {
            char_code = (unsigned char)(*file).ReadByte();

            indices.push_back( char_code );
            if ( char_code > highest_index ) { highest_index=char_code; }
        }
    }
    else // normal operation
    {
        IFileStream desc_file1( IFS::Instance().OpenFile(data));
        std::string allFile;
        std::string line;
        std::string encodingName;

        desc_file1 >> data;
        if ( data == "Encoding" )
        {
            desc_file1 >> encodingName;
        }        
        else
        {
            allFile += data + ' ';
        }

        while ( std::getline( desc_file1, line ))
        {
            allFile += line;
            allFile += '\n';
        }
        std::wstring wideFileData;
	std::locale loc( LangSelector::Instance()[ encodingName ] );
        Read( wideFileData, allFile, loc );
        std::basic_stringstream< wchar_t > wideFile( wideFileData );
        std::wstring wideData;

        while ( wideFile >> wideData )
        {
                 if ( wideData == L"Indices" )    { mode = INDICES; wideFile >> wideData; }
            else if ( wideData == L"Style" )      { mode = STYLE;   wideFile >> wideData; }
            else if ( wideData == L"Styles" )     { mode = STYLES;     wideFile >> wideData; }
            else if ( wideData == L"Properties" ) { mode = PROPERTIES; wideFile >> wideData; }
            
            if ( !wideFile ) {
                throw Error("Font Description file SyntaxError");
            }

            if ( mode == INDICES )
            {
                wchar_t wide = to_char( wideData );
                indices.push_back( wide );
                if ( wide > highest_index ) { highest_index = wide; }
            }
            else if ( mode == PROPERTIES )
            {
                if      ( wideData == L"kerning")   { wideFile >> _kerning; }
                else if ( wideData == L"spacing")   { wideFile >> _spacing; }
                else {
                    throw Error("Font Description file SyntaxError (unknown property)");
                }
            }
            else if ( mode == STYLES )
            {
                int flags = to_flags( wideData, style_names );
                (*_styles).Insert( flags, ColorOffset( 0L, style_offset ));
                style_offset += highest_index + 1;
            }
            else if ( mode == STYLE )
            {
                style = to_flags( wideData, style_names );
                mode = COLOR;
            }
            else if ( mode == COLOR )
            {
                ColorOffset colorOffset( to_hex( wideData ), style_offset );
                (*_styles).Insert( style, colorOffset );
                style_offset += highest_index + 1;
            }
        }
    }

    std::vector< int > mapping;
    for ( int i=0, offset=0, n=(*_faces).Size(); i < n; offset += highest_index + 1 )
    {
        std::vector< int >::iterator p = indices.begin(), q = indices.end();
        if ( p >= q )
        {
            throw Error("Font Description file SyntaxError (too few indices)");
        }
        for (; p < q && i < n; ++p, ++i )
        {
            int idx = *p;
            mapping.push_back( idx + offset );
        }
    }
    (*_faces).Remap( mapping );
}

void FacedFont::Paint( const Painter &painter, const Text &text, const Pos &pos )
{
    PointList pnts; RectList rects;
    Think( text, pnts, rects, pos );
    painter.ProjectImages( (*_faces).GetImage( ImageDicer::SOURCE ), pnts, rects );
}

void FacedFont::SetStyle( int style_flags )
{
    _style = style_flags;

    Styles::ConstIterator i;
    if ( (*_styles).Find( style_flags, &i ) )
    {
        int best = 0;
        int best_similarity = 256*256*3;

        for ( ; !!i && i.Key()==style_flags; ++i )
        {
            NormalPixel styleColor = i.Value().first;
            int similarity = styleColor.c.Similarity( _color.c );
            if ( best_similarity > similarity )
            {
                best_similarity = similarity;
                best = i.Value().second;
            }
        }
        _offset = best;
    }
}

void FacedFont::SetColor( NormalPixel color )
{
	_color = color;
    SetStyle( _style );
}

FacedFont::Letters FacedFont::GetStats( const Text &text ) const
{
    FacedFont  my_copy( *this );
    Letters    letters;
    RectList   rects;
    PointList  points;

    my_copy.Think( text, points, rects );

    /*Pos *p = points.begin(), *q = points.end();
    for (; p!=q; ++p )
    {
        letters.Insert( (*p).x );
    }*/

    Rect *p = &*rects.begin(), *q = &*rects.end();
    for (; p!=q; ++p )
    {
        letters.Insert( (*p).GetW() + _kerning );
    }

    return letters;
}

int FacedFont::GetWidth( UniCode c ) const
{
    if ( (*_faces).Exists( c + _offset ) )
    {
        return (*_faces).GetImage( c + _offset ).GetWidth();
    }
    else { return 0; }
}

int FacedFont::GetWidth( const Text &text ) const
{
    FacedFont  my_copy( *this );
    RectList   rects;
    PointList  points;

    my_copy.Think( text, points, rects );

    if ( points.empty() ) { return 0; }
    
    return (*points.rbegin()).x + (*rects.rbegin()).GetW();
}

void FacedFont::ApplyMarkup( const PreformatedText::Markup &mark )
{
    if ( mark.f_style) { SetStyle( mark.style ); }
    if ( mark.f_color == 1) 
		{	
			_prev_color = _color;
			SetColor( mark.fg_color ); 
		} 
	else if( mark.f_color == 2 )
		{ 
			SetColor( _prev_color ); 
		}

    if ( mark.f_backdrop ) { SetBackColor( mark.bg_color ); }
    if ( mark.f_kerning ) { SetKerning( mark.kerning ); }
}

void FacedFont::Preformat( const Text &text )
{
    PreformatedText preformated( text );
    PreformatedText::ConstIterator subtext_i = preformated.ConstBegin();
    for (; !!subtext_i; ++subtext_i )
    {
        const UniCode *p = &*preformated.begin() + (*subtext_i).start;
        const UniCode *q = &*preformated.begin() + (*subtext_i).end;
        ApplyMarkup( *subtext_i );
    }
}

/*! Genberate list of rectangles and positions */
//---------------------------------------------------------------------------
void FacedFont::Think( const PreformatedText &preformated,
                                   PointList &points,
                                    RectList &rects,
                                   const Pos &offset )
{
    PreformatedText::ConstIterator subtext_i = preformated.ConstBegin();
    Pos carret = offset;
    for (; !!subtext_i; ++subtext_i )
    {
        const UniCode *p = &*preformated.begin() + (*subtext_i).start;
        const UniCode *q = &*preformated.begin() + (*subtext_i).end;

        ApplyMarkup( *subtext_i );

        if ( p==q ) { continue; }


        ThinkQuick( p, q, points, rects, carret );
    }
}

void FacedFont::ThinkQuick( const UniCode *p, const UniCode *q,
                            PointList &points,
                             RectList &rects,
                                  Pos &carret )
{
    for (; p!=q; ++p )
    {
        if ( (*_faces).Exists( *p + _offset ) )
        {
            IGraphics &face = (*_faces).GetImage( *p + _offset );
            rects.push_back( face.GetRect() );
            points.push_back( carret );
            carret.x += (*rects.rbegin()).GetW() + _kerning;
        }
    }
}

}; //Useless
