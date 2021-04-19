#include "UselessPch.h"

#include "Useless/Functional/DelayedExecutor.h"

namespace Useless {

INIT_SINGLETON( OnTimer, USELESS_API );

DelayedExecutor::DelayedExecutor()
{
}


DelayedExecutor::~DelayedExecutor()
{
}

void DelayedExecutor::Execute( __Tasks::iterator task_iter )
{
    __Tasks::iterator next_task_iter = task_iter; ++next_task_iter;

    if ( !(*task_iter)._being_executed )
    {
        (*task_iter)._being_executed = true;
        (*task_iter)._executee();

        // Prevent writing to freed memory
        if ( --next_task_iter == task_iter )
        {   
            (*task_iter)._being_executed = false;
        }
    }
}

void DelayedExecutor::Execute()
{
    __Tasks::iterator i,j = _tasks.begin();

    for( ; j!=_tasks.end(); )
    {
       i=j; ++j;
       __Task &task = *i;

        if ( task._is_waiting )
        {
            if ( Timer()-task._time > task._delay )
            {
                task._is_waiting = false;
                if (task._repeats>0) { --task._repeats; }
                task._time = Timer();
                Execute(i);
            }
        } else
        if ( task._duration>0 )
        {
            if ( task._interval<=0 || task._interval <= Timer()-task._time )
            {
                task._duration -= Timer()-task._time;
                if (task._repeats>0) { --task._repeats; }
                task._time = Timer();
                Execute(i);
            }

        } else
        if ( task._repeats>0 || task._repeats==ALLWAYS )
        {
            if ( task._interval<=0 || task._interval <= Timer()-task._time )
            {
                task._time = Timer();
                if (task._repeats>0) { --task._repeats; }
                Execute(i);
            }
        }
        else
        {
            _tasks.erase(i);
        }
    }
}


DelayedExecutor::TaskID DelayedExecutor::Schedule( const TaskInfo &task_info )
{
    
    __Task task;
    task._is_waiting = true;
    task._being_executed = false;
    task._executee = task_info.p_func;
    task._delay    = task_info.delay;
    task._interval = task_info.interval;
    task._duration = task_info.duration;
    task._repeats  = task_info.repeats;
    _tasks.push_back(task);

    __Tasks::iterator end = _tasks.end(); --end;
    return end;
}

void DelayedExecutor::Free( const DelayedExecutor::TaskID &tid )
{
    __Tasks::iterator i=_tasks.begin(), i2=_tasks.end();

    for ( ; i!=i2; ++i )
    {
        if ( i == tid._id )
        {
            _tasks.erase( tid._id );
            break;
        }
    }
}

};//namespace Useless
