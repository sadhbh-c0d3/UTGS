#ifndef __INCLUDED_USELESS_DELAYED_EXECUTOR_H__
#define __INCLUDED_USELESS_DELAYED_EXECUTOR_H__

#pragma warning(disable:4786)       // disable 'indentifier was truncated...'
#include "Useless/Util/Singleton.h"
#include "Useless/Util/SingletonKiller.h"
#include "Signal.h"
#include "PackedFunctor.h"
#include "Useless/System/Timer.h"
#include "Useless/Util/TemplateDefines.h"
#include <list>

namespace Useless {

class USELESS_API DelayedExecutor
{
    DelayedExecutor();
    ~DelayedExecutor();

    struct __Task
    {
        Timer        _time;
        unsigned     _is_waiting : 1,
                     _being_executed : 1;
        double       _delay;
        double       _interval;
        double       _duration;
        long         _repeats;
        PkFunc       _executee;
    };

    typedef std::list< __Task> __Tasks; 

    __Tasks _tasks;
    Timer   _zero;

    void Execute( __Tasks::iterator task_iter );

    friend class CreateUsingNew<DelayedExecutor>;

public:
    struct TaskInfo
    {
        TaskInfo(): interval(0), duration(0), repeats(0) {}
        TaskInfo( PkFunc  p_func, 
                  double  delay,
                  double  interval = 0,
                  double  duration = 0,
                    long  repeats = 0
                  ): p_func(p_func), delay(delay), interval(interval), 
                     duration(duration), repeats(repeats)  {}

            PkFunc  p_func;
            double  delay;
            double  interval;
            double  duration;
              long  repeats;   
    };

    class TaskID
    {
        TaskID( const __Tasks::iterator &id ): _id(id) {}
        __Tasks::iterator _id;
        friend class DelayedExecutor;
    public:
        TaskID() {}
        void Free();
    };

    enum { ALLWAYS=-1 }; 
    //Note:
    // Task will excecute forever
    // if interval==ALLWAYS or repeats==ALLWAYS

    TaskID Schedule( const TaskInfo &task_info );
    TaskID Schedule( PkFunc  p_func, double delay=0, long repeats=0 )
    {
        return Schedule( TaskInfo( p_func, delay, 0,0, repeats) );
    }

    void Free( const TaskID &tid );
    void Execute();
    friend class TaskID;


    /* Here are some helper methods */
    template< class CP, class C, class R >
    TaskID Schedule( const CP &cp, R (C::*mp)(), double delay=0, long repeats=0 )
    {
        return Schedule( PackFunctor( VoidCall(cp,mp)), delay, repeats );
    }

    template< class CP, class C, class R, class T1, class A1 >
    TaskID Schedule( const CP &cp, R (C::*mp)(T1), const A1 &a1, double delay=0, long repeats=0 )
    {
        return Schedule( PackFunctor( UnaryCall(cp,mp),a1), delay, repeats );
    }

    template< class CP, class C, class R, class T1, class A1, class T2, class A2 >
    TaskID Schedule( const CP &cp, R (C::*mp)(T1,T2), const A1 &a1, const A2 &a2,
                     double delay=0, long repeats=0 )
    {
        return Schedule( PackFunctor( BinaryCall(cp,mp),a1,a2), delay, repeats );
    }

    template< class CP, class C, class R, class T1, class A1, class T2, class A2, class T3, class A3 >
    TaskID Schedule( const CP &cp, R (C::*mp)(T1,T2,T3), const A1 &a1, const A2 &a2, const A3 &a3,
                     double delay=0, long repeats=0 )
    {
        return Schedule( PackFunctor( TernaryCall(cp,mp),a1,a2,a3), delay, repeats );
    }

    template< class CP, class C, class R, class T1, class A1, class T2, class A2, class T3, class A3, class T4, class A4 >
    TaskID Schedule( const CP &cp, R (C::*mp)(T1,T2,T3,T4), const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4,
                     double delay=0, long repeats=0 )
    {
        return Schedule( PackFunctor( TetraryCall(cp,mp),a1,a2,a3,a4), delay, repeats );
    }

};

DECLARE_SINGLETON( DelayedExecutor, OnTimer, USELESS_API )

inline void DelayedExecutor::TaskID::Free()
{
    OnTimer::Instance().Free( *this );
    *this = TaskID();
}

};//namespace Useless

#endif//__INCLUDED_USELESS_DELAYED_EXECUTOR_H__
