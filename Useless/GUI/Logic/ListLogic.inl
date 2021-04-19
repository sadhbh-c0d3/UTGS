/* In order to use Ctors in this file you must include Util/ParmList.h
 * It's not included in header since we dont want everything to compile
 */

template< class PL> ListLogic( const CPT::Parms&, const PL& parm_list )
{
    Init();

    _hiliten = -1;
    _selector = -1;

    _allways_selected = GetParm( CPT::alw_sel(), parm_list, CPT::alw_sel(0) )();
    _selector_selects = GetParm( CPT::sel_sel(), parm_list, CPT::sel_sel(0) )();
    _follow_hilite    = (GetParm( CPT::full_follow(), parm_list, CPT::full_follow(0) ))? FULL_FOLLOW :
                        (GetParm( CPT::drag_follow(), parm_list, CPT::drag_follow(0) ))? DRAG_FOLLOW : DONT_FOLLOW;

}


