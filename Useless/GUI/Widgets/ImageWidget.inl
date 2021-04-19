#if defined(__INCLUDED__CORE_PARM_TYPES_H__) & defined(__INCLUDED__CORE_PARM_LIST_H__)
/* In order to use Ctors in this file you must include Util/ParmList.h
 * It's not included in header since we dont want everything to compile
 */


template< class PL> ImageWidget( const CPT::Parms&, const PL& parm_list )
{
    CTC_PL_InitParm1( _sp_skin, parm_list, CPT::skin, NO_SKIN );
}
#endif
