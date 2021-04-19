#if defined(__INCLUDED__CORE_PARM_TYPES_H__) & defined(__INCLUDED__CORE_PARM_LIST_H__)
/* In order to use Ctors in this file you must include Util/ParmList.h
 * It's not included in header since we dont want everything to compile
 */

template< class PL> ListedTextItems( const CPT::Parms&, const PL& parm_list )
    :_skins( (const SkinMan&)GetParmStrict( CPT::skins(), parm_list ), true ),
     _fonts( (const FontSet&)GetParmStrict( CPT::fonts(), parm_list ) )
{
    _shift = GetParm( CPT::shift(), parm_list, CPT::shift( GetParm( CPT::dist(), parm_list, CPT::dist(2) )() ) )();

    _entry_height = _fonts.Get().GetFont(0).GetHeight() + 2;
    _selector_size = _skins.Get().GetMaxSize();
    _entry_height = (_entry_height>_selector_size.y)? _entry_height:_selector_size.y;

}
#endif
