#if defined(__INCLUDED__CORE_PARM_TYPES_H__) & defined(__INCLUDED__CORE_PARM_LIST_H__)

/* In order to use Ctors in this file you must include Util/ParmList.h
 * It's not included in header since we dont want everything to compile
 */

template< class PL> CaptionWidget( const CPT::Parms&, const PL& parm_list )
    :_skins( (const SkinMan&)GetParmStrict( CPT::skins(), parm_list ) ),
     _fonts( (const FontSet&)GetParmStrict( CPT::fonts(), parm_list ) )
{
    CTC_PL_InitParm2( _caption, parm_list, CPT::caption, CPT::text, NO_CAPTION_GIVEN );

    //CTC_PL_InitParm1( _sp_skins, parm_list, CPT::skins, NO_SKIN );
    
    //CTC_PL_InitParm1( _fonts, parm_list, CPT::fonts, NO_FONT );

    _alignment = GetParm<CPT::align>(parm_list, 0);
    _distance = GetParm<CPT::dist>(parm_list, 0);
    _state = 1;
}

#endif
