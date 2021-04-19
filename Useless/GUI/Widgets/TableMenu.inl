#if defined(__INCLUDED__CORE_PARM_TYPES_H__) & defined(__INCLUDED__CORE_PARM_LIST_H__)
/* In order to use Ctors in this file you must include Util/ParmList.h
 * It's not included in header since we dont want everything to compile
 */

template< class PL> TableMenu( const CPT::Parms&, const PL& parm_list )
    :DropDown( CTC::use_parms, parm_list ), _p_list(0)
{}
#endif
