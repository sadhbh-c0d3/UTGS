/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE KooLAS C++ Template & Class Library     *
 * CALLED KooLib SOURCE CODE.                                       *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE KooLib SOURCE CODE IS (C) COPYRIGHT 2004                     *
 * by the Sadhbh Code (https://github.com/sadhbh-c0d3)              *
 *                                                                  *
 ********************************************************************

  function:
  last mod: $Id: Fader.h,v 1.1 2004/06/16 16:48:34 koolas Exp $

 ********************************************************************/

#ifndef __INCLUDED_KOOLIB_FADER_H__
#define __INCLUDED_KOOLIB_FADER_H__

#include <deque>

namespace Fx {

    /*! Fader is incremental linear interpolator
     */
    template< class _Phase = int, class _Time = int >
    struct Fader
    {
        _Time _duration;
        _Time _time;
        _Phase _phase;
        _Phase _startPhase;
        _Phase _endPhase;

        Fader( _Time duration, _Phase startPhase, _Phase endPhase )
            : _duration( duration )
            , _startPhase( startPhase )
            , _endPhase( endPhase )
            , _time(0)
            , _phase( startPhase )
            {
            }

        bool operator () ( _Time timeTick )
        {
            if ( _time < _duration )
            {
                _phase = _GetPhaseAtTime( _time );

                if ( _time + timeTick < _duration )
                {
                    _time += timeTick;
                    return false;
                }
            }
            _phase = _endPhase;
            return true;
        }

        _Phase GetPhase() const
        {
            return _phase;
        }

        _Phase _GetPhaseAtTime( _Time time ) const
        {
            _Phase phase = ((time * (_endPhase - _startPhase))/_duration + _startPhase);
            return phase;
        }

        _Phase GetPhaseAtTime( _Time time ) const
        {
            if ( time < _duration )
            {
                return _GetPhaseAtTime( time );
            }
            else
            {
                return _endPhase;
            }
        }
    };

    /*! Envelope is incremental multi-segment linear interpolator
     */
    template< class _Phase = int, class _Time = int >
    struct Envelope : std::deque< Fader< _Phase, _Time > >
    {
        _Phase _phase;

        Envelope( _Phase phase = 0 )
            : _phase( phase )
            {}

        void Add( _Time duration, _Phase endPhase )
        {
            push_back( Fader< _Phase, _Time >( duration, GetEndPhase(), endPhase ));
        }


        bool operator () ( _Time timeTick )
        {
            _Phase phase = _phase;
            
            if ( !empty() )
            {
                _phase = front().GetPhase();
            }

            while ( !empty() && front()( timeTick ))
            {
                _phase = front().GetPhase();
                pop_front();
            }

            return (( phase == _phase ) && empty());
        }

        _Phase GetPhase() const
        {
            return _phase;
        }
        
        //! method is usable only if operator () is never called
        _Phase GetPhaseAtTime( _Time time ) const
        {
            _Time t = 0;
            _Phase phase = _phase;
            for ( const_iterator i = begin(); i != end(); ++i )
            {
                if ( t <= time )
                {
                    if ( time < t + (*i)._duration )
                    {
                        return (*i)._GetPhaseAtTime( time - t );
                    }
                    
                    phase = (*i)._endPhase;
                    t += (*i)._duration;
                }
                else
                {
                    return phase;
                }
            }
            return phase;
        }
        
        _Phase GetEndPhase() const
        {
            if ( !empty() )
            {
                return back()._endPhase;
            }
            else
            {
                return _phase;
            }
        }

        _Phase GetCurrentEndPhase() const
        {
            if ( !empty() )
            {
                return front()._endPhase;
            }
            else
            {
                return _phase;
            }
        }

    };

};//Fx



#endif//__INCLUDED_KOOLIB_FADER_H__
