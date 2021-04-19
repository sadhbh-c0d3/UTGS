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
  last mod: $Id: XMLProgram.h,v 1.8 2005/03/08 17:27:27 koolas Exp $

 ********************************************************************/


#ifndef __INCLUDED_XML_PROGRAM_H__
#define __INCLUDED_XML_PROGRAM_H__

#include "koolib/xml/XMLFactory.h"

namespace XMLProgram {

    struct IChunk;
    struct IBlock;
    struct IFiniteStateMachine;
    struct IApplication;
    struct ExecutionState;

    // Opaque chunk of code with internal environment
    struct KOOLIB_API IChunk
    {
        virtual ~IChunk() {}

        // Execute code with XML argument, result is 'true' if success.
        virtual bool Execute( XMLFactory::Node node, ExecutionState &state )
        {
            return false;
        }

        // Query internal environment, result is 'true' if success.
        virtual bool operator >> ( XMLFactory::AttrUniBase &attr )
        {
            return false;
        }
        
        // Allow faster attribute access, for attributes of type int.
        virtual bool operator >> ( XMLFactory::AttrBaseType< int, wchar_t > &attr )
        {
            return operator >> ( static_cast< XMLFactory::AttrUniBase &>( attr ));
        }
        
        // Allow faster attribute access, for attributes of type int.
        virtual bool operator >> ( XMLFactory::AttrBaseType< double, wchar_t > &attr )
        {
            return operator >> ( static_cast< XMLFactory::AttrUniBase &>( attr ));
        }
        
        // Set internal environment, result is 'true' if success.
        virtual bool operator << ( XMLFactory::AttrUniBase &attr )
        {
            return false;
        }
        
        // Allow faster attribute access, for attributes of type int.
        virtual bool operator << ( XMLFactory::AttrBaseType< int, wchar_t > &attr )
        {
            return operator << ( static_cast< XMLFactory::AttrUniBase &>( attr ));
        }
        
        // Allow faster attribute access, for attributes of type int.
        virtual bool operator << ( XMLFactory::AttrBaseType< double, wchar_t > &attr )
        {
            return operator << ( static_cast< XMLFactory::AttrUniBase &>( attr ));
        }

        // Allow fast access to value
        virtual bool operator >> ( TextUtf8 &value )
        {
            return operator >> ( XMLFactory::Attr< TextUtf8, true, wchar_t >(L"value"));
        }

        virtual bool operator >> ( int &value )
        {
            return operator >> ( XMLFactory::Attr< int, true, wchar_t >(L"value"));
        }

        virtual bool operator >> ( double &value )
        {
            return operator >> ( XMLFactory::Attr< double, true, wchar_t >(L"value"));
        }

        // Query internal environment as chunk. Result should be kept in IChunkPtr.
        virtual IChunk* GetChunk( unsigned int name )
        {
            return 0;
        }

        // Query internal environment as chunk. Result should be kept in IChunkPtr.
        virtual IChunk* GetChunk( const TextUtf8 &name )
        {
            return 0;
        }

        // Set internal environment as chunk.
        virtual void SetChunk( unsigned int name, IChunk *pChunk )
        {
        }

        // Set internal environment as chunk.
        virtual void SetChunk( const TextUtf8 &name, IChunk *pChunk )
        {
        }

        // Tell if chunk has lazy value.
        virtual bool IsLazy() { return false; }

        // Query internal environment as FourCC code.
        virtual unsigned int GetFourCC( unsigned int name ) { return 0L; }

        IChunk(): _refCount(0) {}
        IChunk* AddRef()
        {
            ++_refCount;
            return this;
        }
        void RemRef()
        {
            if ( 0 == --_refCount )
            {
                Release();
            }
        }
        virtual void Release()
        {
            delete (this);
        }
        int _refCount;
    };

    // Block of chunks
    struct KOOLIB_API IBlock : virtual IChunk
    {
        // Insert chunk of code into block.
        virtual void AddChunk( unsigned int name, IChunk *chunk ) = 0;
        
        // Insert chunk of code into block.
        virtual void AddChunk( const TextUtf8 &name, IChunk *chunk ) = 0;

        // Get chunk from block. Result should be kept in IChunkPtr.
        virtual IChunk* GetChunk( unsigned int name ) = 0;
        
        // Get chunk from block. Result should be kept in IChunkPtr.
        virtual IChunk* GetChunk( const TextUtf8 &name ) = 0;
    };

    // FSM block containing states and chunks.
    struct KOOLIB_API IFiniteStateMachine : virtual IBlock
    {
        virtual ExecutionState & EnterState ( const TextUtf8 &name ) = 0;
        virtual ExecutionState & GotoState  ( const TextUtf8 &name ) = 0;
        virtual ExecutionState & LeaveState () = 0;
        virtual ExecutionState & GetCurrentExecutionState() = 0;
        virtual TextUtf8 GetCurrentState() const = 0;

        // Declare state.
        virtual void AddState( const TextUtf8 &name, XMLFactory::Node node ) = 0;
        
        virtual ExecutionState &    DuplicateStack() = 0;
        virtual void                ReleaseDuplicate() = 0;
        virtual void ExecuteString( const std::string &s ) = 0;
    };
    
    
    struct KOOLIB_API BasicExecutionState
    {
        typedef void ResultType;
        
        IFiniteStateMachine *_currentFSM;
        IBlock              *_currentBlock;
        ExecutionState      *_prevState;

        BasicExecutionState()
            : _currentFSM(0)
            , _currentBlock(0)
            , _prevState(0)
            {
            }

        virtual ~BasicExecutionState()
        {
        }
    };

    struct KOOLIB_API ExecutionState : BasicExecutionState
    {
        XMLFactory::XMLTagDict< ExecutionState > *_currentDict;

        ExecutionState()
            : _lastResult(0)
            , _currentDict(0)
            {
            }

        ExecutionState( const ExecutionState &other )
            : BasicExecutionState( other )
            , _lastResult(0)
            , _currentDict( other._currentDict )
        {
            SetResult( other.GetResult() );
        }

        ExecutionState & operator = ( const ExecutionState &other )
        {
            BasicExecutionState::operator = ( other );
            SetResult( other.GetResult() );
            return (*this);
        }

        virtual ~ExecutionState()
        {
            SetResult(0);
        }

        void Execute( XMLFactory::Node node )
        {
            _currentDict->Execute( node, *this );
        }

        void ExecuteChildren( XMLFactory::Node node )
        {
            _currentDict->ExecuteChildren( node, *this );
        }

        void SetResult( IChunk *chunk )
        {
            if ( _lastResult )
            {
                _lastResult->RemRef();
            }
            _lastResult = chunk;

            if ( _lastResult )
            {
                _lastResult->AddRef();
            }
        }

        IChunk * GetResult() const
        {
            return _lastResult;
        }

    private:
        IChunk *_lastResult;
    };


};//Program 

#endif//__INCLUDED_XML_PROGRAM_H__
