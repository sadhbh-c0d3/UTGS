/*
**  $Id: Signal.cpp,v 1.27 2004/07/02 10:07:29 koolas Exp $
**
**  NAME
**      Signal 
**
**  PURPOSE
**      Executes Functors on the list
**
**  AUTHOR(S)
**      Sadhbh Code (https://github.com/sadhbh-c0d3)
**
*/

#include "UselessPch.h"

#include "Useless/Functional/Signal.h"
#include "Useless/ErrorConfig.h"

#include <set>

std::vector< Useless::CommonReference<Useless::Signal> > Useless::Signal::_sig_schedule;

namespace Useless {

Signal::Signal()
{ 
    _is_disabled = false;
    _self_pointer.SetTarget(this);
}

Signal::~Signal()
{
    _self_pointer.SetTarget(NULL);
}

Signal::Signal( const Signal &other )
{
    _is_disabled = other._is_disabled;
    _packed_functors = other._packed_functors;
    
    _self_pointer.SetTarget(NULL);
    _self_pointer = other._self_pointer;
    _self_pointer.SetTarget(this);
    
    CommonReference< Signal > it_pointer; 
    ((Signal&)other)._self_pointer = it_pointer; 
    it_pointer.SetTarget( (Signal*)&other );
}

Signal& Signal::operator = ( const Signal &other )
{
    _is_disabled = other._is_disabled;
    _packed_functors = other._packed_functors;
    
    _self_pointer.SetTarget(NULL);
    _self_pointer = other._self_pointer;
    _self_pointer.SetTarget(this);
    
    CommonReference< Signal > it_pointer; 
    ((Signal&)other)._self_pointer = it_pointer; 
    it_pointer.SetTarget( (Signal*)&other );
    
    return (*this);
}


Signal::FuncID Signal::Tie( PkFunc pfunp)
{ 
    //static Mutex _mutex; //At one moment only one thread may access this method
    //Lock guard(_mutex);  //this way it as safe as possibile
    
    size_t i=0, n=_packed_functors.size();
    while ( i<n && _packed_functors[i].first==i ) { ++i; }
    _packed_functors.insert( _packed_functors.begin()+i, std::make_pair( i, pfunp) );
    return FuncID(_self_pointer, i);
}

bool Signal::Send()
{
    if ( Empty() || IsDisabled() ) return false;

    static std::set< Signal* > _signal_guardian;
    {
        //Mutex _mutex;       // Signal cannot be sent more than one time at once
        //Lock guard(_mutex); // 
        if ( _signal_guardian.find(this)!=_signal_guardian.end() )
            return Schedule();
        else
            _signal_guardian.insert(this);
    }

    for ( size_t i=0, n=_packed_functors.size(); i<n; ++i )
    {
        _packed_functors[i].second();
    }
    {
        //Mutex _mutex;       // Signal cannot be sent more than one time at once
        //Lock guard(_mutex); // 
        _signal_guardian.erase( _signal_guardian.find(this) );
    }
    return true;
}

bool Signal::WasScheduled() const
{
    ScheduleMap::iterator i = _sig_schedule.begin();
    ScheduleMap::iterator end = _sig_schedule.end();
    for ( ;i!=end; ++i )
    {
        if ( (*i).GetTarget()==this ) { return true; }
    }
    return false;
}

bool Signal::Schedule()
{
    //static Mutex _mutex; //At one moment only one thread may access this method
    //Lock guard(_mutex);  //this way it as safe as possibile
    if ( !Empty() )
    {
        HUGE_LOG( Error("Schedule: %0x", this ), "Signal" );
        _sig_schedule.push_back(_self_pointer);
        return true;
    }
    return false;
}

void Signal::Dispatch()
{   
    //static Mutex _mutex; //At one moment only one thread may access this method
    //Lock guard(_mutex);

    size_t i, n = _sig_schedule.size();
    ScheduleMap temp(_sig_schedule);
    _sig_schedule.clear();

    for (i=0; i!=n; ++i)
    {
        if ( temp[i] ) {
            try {
                temp[i]->Send();
            }
            catch( Error &e ) {
                SendErrorText( 0, "Signal:Dispatch", Error( "Signal at %0x failed: %s", temp[i].GetTarget(), e.GetError() ).GetError() );
            }
            catch( std::exception &x ) {
                SendErrorText( 0, "Signal:Dispatch", Error( "Signal at %0x failed: %s", temp[i].GetTarget(), x.what() ).GetError() );
            }
            catch( ... ) {
                SendErrorText( 0, "Signal:Dispatch", Error( "Signal at %0x failed. Unknown reason.", temp[i].GetTarget() ).GetError() );
            }
        }        
    }
}

}; // namespace Useless
