#if defined(__INCLUDED__CORE_PARM_TYPES_H__) & defined(__INCLUDED__CORE_PARM_LIST_H__)
/* In order to use Ctors in this file you must include Util/ParmList.h
 * It's not included in header since we dont want everything to compile
 */

template< class PL> ListWidget( const CPT::Parms&, const PL& parm_list )
    :ListedTextItems( CPT::use_parms, 
     PrepareParmList(parm_list, ParmList_3<CPT::skins, CPT::fonts, CPT::shift>() ) ),
     ListLogic( CPT::use_parms, parm_list)
{}

struct Helper
{
    static SkinSet CreateSkins( const Skin &sk  ) 
        { return ListWidget::CreateSkinSet( sk ); }

    static FontSet CreateFonts( const Font &fn, const Font &fh ) 
        { return ListWidget::CreateFontSet( fn, fh ); }

    static SkinSet CreateSkins( const CTC::NullType & ) { return SkinSet(); }
    static FontSet CreateFonts( const CTC::NullType &, const CTC::NullType & ) { return FontSet(); }
    static FontSet CreateFonts( const Font &, const CTC::NullType & ) { return FontSet(); }
    static FontSet CreateFonts( const CTC::NullType &, const Font & ) { return FontSet(); }
};


template< class OUT_PL, class PL > static OUT_PL PrepareParmList( const PL& parm_list, const OUT_PL & )
{
    CTC_PL_CheckParm2( parm_list, CPT::skins, CPT::selector, NO_SELECTOR_SKIN );
    CTC_PL_CheckParm2( parm_list, CPT::fonts, CPT::font, NO_DEFAULT_FONT );
    CTC_PL_CheckParm2( parm_list, CPT::fonts, CPT::hilite, NO_HIGHLITEN_FONT );

    static SkinSet skin_set;
    static FontSet font_set;

    skin_set = Helper::CreateSkins( parm_list[CTC::type<CPT::selector>()] );
    font_set = Helper::CreateFonts( parm_list[CTC::type<CPT::font>()], parm_list[CTC::type<CPT::hilite>()] );


    CPT::skins ss = GetParm( CPT::skins(), parm_list, (const SkinMan&)skin_set );
    CPT::fonts fs = GetParm( CPT::fonts(), parm_list, font_set );
    CPT::shift sh = GetParm( CPT::shift(), parm_list, CPT::shift( GetParm( CPT::dist(), parm_list, CPT::dist(2) )() ) );

    return ParmList_3<CPT::skins, CPT::fonts, CPT::shift>(ss,fs,sh);
}


#endif
