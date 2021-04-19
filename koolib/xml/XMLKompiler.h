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
  last mod: $Id: $

 ********************************************************************/
#ifndef __INCLUDED_KOOLIXP_CXML_H__
#define __INCLUDED_KOOLIXP_CXML_H__

#include "koolib/xml/XMLChunksProgram.h"
#include <cmath>

namespace XMLProgram {
    
    // Compile xml nodes to list of executable functors.
    // The resultant list is an executable functor.
    KOOLIB_API IChunkPtr Compile( Node node, ExecutionState &state );

    // Compile xml nodes to list of executable functors.
    // The resultant list is expected to be executed to provide a right-value.
    KOOLIB_API IChunkPtr CompileRightValue( Node node, ExecutionState &state );
    
    // Create Uncompiled xml function.
    KOOLIB_API IChunkPtr NoCompile( Node node, ExecutionState &state );

    //---------------------------------------------------------
    //
    //  T h e
    //
    //  K oo L i X P
    //
    //  C O M P I L E R
    //
    //---------------------------------------------------------

    namespace CXML {

        struct KOOLIB_API CXmlErrors
        {
            struct ErrorInSourceAtLine : Useless::Error
            {
                ErrorInSourceAtLine( const std::string &source, int line, const std::string &text )
                    :   Useless::Error("%s\n{ %i, %s }\n", text.c_str(), line, source.c_str() )
                    {}
            };
            
            struct ErrorInSourceAtLineWithHelp : ErrorInSourceAtLine
            {
                ErrorInSourceAtLineWithHelp( const std::string &source, int line, const std::string &text )
                    :   ErrorInSourceAtLine( source, line, text )
                    {}
            };
            
            int         _line;
            std::string _source;
            
            CXmlErrors(): _line(-1) {}
            virtual ~CXmlErrors() {}
            
            void RaiseError( Useless::Error &e )
            {
                if ( 0 == dynamic_cast< ErrorInSourceAtLineWithHelp *>( &e ))
                {
                    if ( IChunk *p = dynamic_cast< IChunk *>( this ))
                    {
                        Attr< TextAnsi, false, wchar_t > help(L"help");
                        if ( (*p) >> help )
                        {
                            throw ErrorInSourceAtLineWithHelp( _source, _line, std::string( e.GetError() ) + TextAnsi( L"\n" + help.str() ).as_string());
                        }
                    }
                }
                throw ErrorInSourceAtLine( _source, _line, e.GetError() );
            }
        };

        struct KOOLIB_API EvalInNewScope : IChunk
        {
            IChunkPtr _lazyValue;

            EvalInNewScope( IChunk *pLazyValue ): _lazyValue( pLazyValue )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                SubScope newState( state );
                _lazyValue->Execute( __unused__, newState );
                state.SetResult( newState.GetResult() );
                return true;
            }
        };

        template< class _Type >
            struct KOOLIB_API ValueConvertion : IChunk
            {
                IChunkPtr _lazyValue;
                unsigned int _typeName;

                ValueConvertion( IChunk *pLazyValue ): _lazyValue( pLazyValue )
                {
                    _typeName = TypeNameTraits< _Type >::fcc();
                }

                bool Execute( Node __unused__, ExecutionState &state )
                {
                    SubScope newState( state );
                    _lazyValue->Execute( __unused__, newState );
                    if ( 0 != _typeName && newState.GetResult()->GetFourCC( FOURCC_NAME_TYPE ) == _typeName )
                    {
                        state.SetResult( newState.GetResult() );
                    }
                    else
                    {
                      _Type value = value_of< _Type >( newState.GetResult() );
                      state.SetResult( CreateValue( value ));
                    }
                    return true;
                }

                unsigned int GetFourCC( unsigned int name )
                {
                    if ( name == FOURCC_NAME_CONST || name == FOURCC_NAME_GET )
                    {
                        return _lazyValue->GetFourCC( name );
                    }
                    else
                    {
                        return 0;
                    }
                }
            };

        struct KOOLIB_API ConstantValue : IChunk
        {
            IChunkPtr _constValue;

            ConstantValue( IChunk *pChunk ): _constValue( pChunk )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                state.SetResult( _constValue.get() );
                return true;
            }

            unsigned int GetFourCC( unsigned int name )
            {
                if ( name == FOURCC_NAME_CONST )
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        };

        template< class _Type >
            struct KOOLIB_API EvaluableValue : IChunk
            {
                TextUtf8 _expression;

                EvaluableValue( const TextUtf8 &expression ): _expression( expression )
                {
                }

                bool Execute( Node __unused__, ExecutionState &state );
            };

        struct KOOLIB_API LazyCall : IChunk
        {
            IChunkPtr _pFunc;
            IBlockPtr _pEnv;
            LazyCall( IChunk *pFunc ): _pFunc( pFunc )
            {
                _pEnv = CreateBlock();
            }
            LazyCall( IChunk *pFunc, IBlock *pBlock ): _pEnv( pBlock ), _pFunc( pFunc )
            {
            }
            
            bool Execute( Node __unused__, ExecutionState &state )
            {
                ExecutionState newState( state );
                newState._prevState = &state;
                newState._currentBlock = _pEnv.get();
                SubScope newState1( newState );
                _pFunc->Execute( __unused__, newState1 );
                state.SetResult( newState1.GetResult() );
                return true;
            }

            bool IsLazy()
            {
                return true;
            }

            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"type-name" )
                {
                    attr.str(L"lazy-call");
                    return true;
                }
                else if ( attr._name == L"is-lazy" )
                {
                    attr.str(L"1");
                    return true;
                }
                else
                {
                    return (*_pEnv).operator >> ( attr );
                }
            }

            unsigned int GetFourCC( unsigned int name )
            {
                if ( FOURCC_NAME_TYPE == name )
                {
                    return FOURCC_TYPE_LAZY;
                }
                else if ( FOURCC_NAME_LAZY )
                {
                    return 1L;
                }
                else
                {
                    return 0L;
                }
            }
        };

        struct KOOLIB_API LazyZip : IChunk
        {
            IChunkPtr _pHead; //< list of nodes, is NOT list of node:head-s
            IChunkPtr _pTail; //< list of ^(list of nodes)^
            
            LazyZip(){}
            LazyZip( IChunk *pListOfLists ): _pHead( pListOfLists )
            {
            }
        
            IChunk* GetChunk( const TextUtf8 &name )
            {
                return GetChunk( Make4CC( name.c_str() ));
            }

            IChunk *GetChunk( unsigned int name )
            {
                /*
                 * zip ( l1=[a,b,c]; l2=[1,2,3] ) => [ [ l1, l2 ], [ l1:tail, l2:tail ], ... ]
                 *
                 * zip:head => [ l1, l2 ] should be [ l1:head, l2:head ]
                 *
                 * zip:head = [ [a,b,c], [1,2,3] ]
                 * zip:tail = [ [[b,c], [2,3]], [[c], [3]] ]
                 *
                 * maybe it should be:
                 *
                 * zip:head = [ a, 1 ]
                 * zip:tail = [ [b, 2], [c, 3] ]
                 *
                 */
                if ( name == FOURCC_LIST_HEAD )
                {
                    return _pHead.get();
                }
                else if ( name == FOURCC_LIST_TAIL )
                {
                    if ( 0 == _pTail.get() )
                    {
                        IChunkPtr pEnd = new XMLEmpty();
                        IChunkPtr pHead = pEnd;
                        XMLListChunk *pLast = 0;

                        IChunk *pList = _pHead.get();
                        while ( !IsEmpty( pList ))
                        {
                            IChunk *pNext = pList->GetChunk( FOURCC_LIST_HEAD )->GetChunk( FOURCC_LIST_TAIL );

                            if ( !IsEmpty( pNext ))
                            {
                                XMLListChunk *p = CreateList( pNext, pEnd.get() );
                                if ( 0 == pLast )
                                {
                                    pHead = p;
                                }
                                else
                                {                
                                    pLast->SetTail( p );
                                }
                                pLast = p;
                            }
                            else
                            {
                                // next row is not complete
                                _pTail = pEnd;
                                break;
                            }
                            pList = pList->GetChunk( FOURCC_LIST_TAIL );
                        }
                        if ( !IsEmpty( pHead.get() ))
                        {
                            _pTail = new LazyZip( pHead.get() );
                        }
                        else
                        {
                            _pTail = pEnd;
                        }
                    }
                    return _pTail.get();
                }
                else
                {
                    return 0;
                }
            }
        };

        struct KOOLIB_API LazyHead : IChunk
        {
            IChunkPtr _pChunk;
            int       _count;

            LazyHead(){}
            LazyHead( int nCount, IChunk *pChunk ): _count(nCount), _pChunk(pChunk)
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )                
            {
                if ( 0 == _count )
                {
                    state.SetResult( CreateEmpty() );
                }
                else
                {
                    _pChunk = Force( _pChunk, state );
                    if ( IsEmpty( _pChunk.get() ))
                    {
                        state.SetResult( CreateEmpty() );
                    }
                    else
                    {
                        IChunkPtr pHead = _pChunk->GetChunk( FOURCC_LIST_HEAD );                
                        if ( _count - 1 > 0 )
                        {
                            IChunkPtr pTail = _pChunk->GetChunk( FOURCC_LIST_TAIL );                
                            state.SetResult( CreateList( pHead.get(), new LazyHead( _count - 1, pTail.get() ) ));
                        }
                        else
                        {
                            state.SetResult( CreateList( pHead.get(), CreateEmpty() ));
                        }
                    }
                }
                return true;
            }

            bool IsLazy()
            {
                return true;
            }
        };
        
        struct KOOLIB_API LazyRange : IChunk
        {
            int _current;
            int _count;

            LazyRange(){}
            LazyRange( int nCurrent, int nCount ): _current(nCurrent), _count(nCount)
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )                
            {
                if ( 0 == _count )
                {
                    state.SetResult( CreateEmpty() );
                }
                else
                {
                    if ( -1 == _count || _count - 1 > 0 )
                    {
                        state.SetResult( CreateList( CreateValue( _current ), new LazyRange( _current + 1, _count - 1 ) ));
                    }
                    else
                    {
                        state.SetResult( CreateList( CreateValue( _current ), CreateEmpty() ));
                    }
                }
                return true;
            }

            bool IsLazy()
            {
                return true;
            }
        };
        
        struct KOOLIB_API LazyGetChunk
        {
            LazyGetChunk *_next;
            LazyGetChunk(): _next(0) {}
            void SetNext( LazyGetChunk *next ) { _next = next; }
            virtual ~LazyGetChunk() { if (_next) { delete _next; } }
            virtual IChunkPtr Get( IChunkPtr parent, ExecutionState &state ) = 0;
        };

        struct KOOLIB_API LazyGetChunkMember : LazyGetChunk 
        {
            TextUtf8        _name;
            unsigned int    _id;

            LazyGetChunkMember( const TextUtf8 &name ): _name( name )
            {
                _id = GetGlobalSymbolDict().AddSymbol( name );
            }

            IChunkPtr Get( IChunkPtr parent, ExecutionState &state )
            {
                IChunkPtr pChunk;
                if ( !!_id )
                {
                    pChunk = parent->GetChunk( _id );
                }
                if ( !pChunk )
                {
                    pChunk = parent->GetChunk( _name );
                }
                if ( 0 != pChunk.get() )
                {
                    if ( 0 != _next )
                    {
                        return _next->Get( pChunk, state );
                    }
                    else
                    {
                        return pChunk;
                    }
                }
                else
                {
                    return 0;
                }
            }

            IChunkPtr operator()( IChunkPtr parent, ExecutionState &state )
            {
                return Get( parent, state );
            }
        };

        struct KOOLIB_API LazyGetChunkInScope : LazyGetChunk
        {
            bool     _noThrow;
            TextUtf8 _name;
            unsigned int _id;
            TextUtf8 _fullName;

            SplitNames _SplitChunkName( TextUtf8 fullName )
            {
                SplitNames names;

                int p = fullName.find(L':');
                if ( -1 != p )
                {
                    names.first  = fullName.substr( 0, p );
                    names.second = fullName.substr( p+1, fullName.size() - (p+1) );
                }
                else
                {
                    names.first = fullName;
                }
                names.first = names.first;
                names.second = names.second;
                return names;
            }

            LazyGetChunkInScope(): _noThrow(0)
            {
            }

            LazyGetChunkInScope( const TextUtf8 &name, bool noThrow = false ): _fullName( name ), _noThrow( noThrow )
            {
                SplitNames p = _SplitChunkName( name );
                _name = p.first;
                _id = GetGlobalSymbolDict().AddSymbol( _name );
                LazyGetChunk *g = this;
                p = _SplitChunkName( p.second );
                while ( !p.first.empty() )
                {
                    LazyGetChunk *n = new LazyGetChunkMember( p.first );
                    g->SetNext( n );
                    g = n;
                    p = _SplitChunkName( p.second );
                }
            }
            
            IChunkPtr Get( IChunkPtr parent, ExecutionState &state )
            {
                IChunkPtr pChunk;
                if ( !!_id )
                {
                    ExecutionState *s = &state;
                    while ( !pChunk && 0 != s )
                    {
                        pChunk = s->_currentBlock->GetChunk( _id );
                        s = s->_prevState;
                    }
                }
                if ( !pChunk )
                {
                    ExecutionState *s = &state;
                    while ( !pChunk && 0 != s )
                    {
                        pChunk = s->_currentBlock->GetChunk( _name );
                        s = s->_prevState;
                    }
                }
                if ( 0 != pChunk.get() )
                {
                    if ( 0 != _next )
                    {
                        pChunk = _next->Get( pChunk, state );
                        if ( 0 != pChunk.get() )
                        {
                            return pChunk;
                        }
                        else if ( _noThrow )
                        {
                            return 0;
                        }
                        else
                        {
                            SplitNames p = _SplitChunkName( _fullName );
                            throw Useless::Error("In '%S' undefined symbol part '%S'", _fullName.c_str(), p.second.c_str());
                        }
                    }
                    else
                    {
                        return pChunk;
                    }
                }
                else if ( _noThrow )
                {
                    return 0;
                }
                else
                {
                    throw Useless::Error("Undefined symbol '%S'", _fullName.c_str());
                }
            }
            
            IChunkPtr operator() ( ExecutionState &state )
            {
                return Get( 0, state );
            }
        };


        struct KOOLIB_API LET : IChunk, CXmlErrors
        {
            TextUtf8     _name;
            unsigned int _id;
            IChunkPtr    _value;

            LET(){}
            LET( const TextUtf8 &name, IChunk *value ): _name( name ), _value( value )
            {
                _id = GetGlobalSymbolDict().AddSymbol( _name );
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    IChunk *pChunk = state._currentBlock->GetChunk( _name );
                    if ( 0 != pChunk ) { throw Useless::Error("<let> Symbol already defined %S", _name.c_str() ); }
                    _value->Execute( __unused__, state );
                    if ( 0 != state.GetResult() )
                    {
                        if ( !!_id )
                        {
                            state._currentBlock->AddChunk( _id, state.GetResult() );
                        }
                        else
                        {
                            state._currentBlock->AddChunk( _name, state.GetResult() );
                        }
                    }
                    state.SetResult(0);
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }

            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<let id [select|call]>[<!-- right-value -->]</let>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-let");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API SET : IChunk, CXmlErrors
        {
            TextUtf8     _name;
            IChunkPtr    _value;

            SET(){}
            SET( const TextUtf8 &name, IChunk *value ): _name( name ), _value( value )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    IChunk *pParent = 0;
                    IChunk *pChunk = XMLProgram::GetChunk( _name, state, &pParent );
                    if ( !pChunk ) { throw Useless::Error("<set> Undefined symbol %S", _name.c_str() ); }
                    if ( !pParent) { throw Useless::Error("<set> Undefined symbol parent %S", _name.c_str() ); }
                    SplitNames names = SplitChunkName( _name, state );
                    while ( !names.second.empty() )
                    {
                        names = SplitChunkName( names.second, state );
                    }
                    _value->Execute( __unused__, state );
                    if ( 0 != state.GetResult() )
                    {
                        pParent->SetChunk( names.first, state.GetResult() );
                    }
                    state.SetResult(0);
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }

            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<set id [select|call]>[<!-- right-value -->]</set>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-set");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };


        struct KOOLIB_API GET : IChunk, CXmlErrors
        {
            LazyGetChunkInScope _getChunk;

            GET() {}
            GET( const TextUtf8 &name ): _getChunk( name )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    state.SetResult( _getChunk.Get( 0, state ).get() );
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }

            unsigned int GetFourCC( unsigned int name )
            {
                if ( name == FOURCC_NAME_GET )
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<get id>[<!-- right-value -->]</get>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-get");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API LOOKUP : IChunk, CXmlErrors
        {
            IChunkPtr _pChunk;
            LOOKUP() {}
            LOOKUP( IChunk *pChunk ): _pChunk(pChunk)
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    static LazyGetChunkInScope s_LazyGetDict(L"dict", true);
                    static LazyGetChunkInScope s_LazyGetName(L"name", true);
                    static LazyGetChunkInScope s_LazyGetDefault(L"default", true);
                    SubScope newState( state );
                    _pChunk->Execute( __unused__, newState );
                    IChunkPtr pDict = s_LazyGetDict.Get( 0, newState );
                    IChunkPtr pName = s_LazyGetName.Get( 0, newState );
                    IChunkPtr pDefault = s_LazyGetDefault.Get( 0, newState );
                    if ( !pDict ) { throw Useless::Error("<lookup> expects 'dict'"); }
                    if ( !pName ) { throw Useless::Error("<lookup> expects 'name'"); }                    
                    TextUtf8 name = value_of< TextUtf8 >( pName.get() );
                    IChunk *value = pDict->GetChunk( name );
                    if ( 0 != value )
                    {   // return found value
                        state.SetResult( value );
                    }
                    else
                    {   // return default value or 0 (is-defined will return FALSE)
                        state.SetResult( pDefault.get() );
                    }
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<lookup><!-- dict, name --></lookup>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-lookup");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API REGISTER : IChunk, CXmlErrors
        {
            IChunkPtr _pChunk;
            REGISTER() {}
            REGISTER( IChunk *pChunk ): _pChunk( pChunk )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    static LazyGetChunkInScope s_LazyGetDict(L"dict", true);
                    static LazyGetChunkInScope s_LazyGetName(L"name", true);
                    static LazyGetChunkInScope s_LazyGetValue(L"value", true);
                    SubScope newState( state );
                    _pChunk->Execute( __unused__, newState );
                    IChunkPtr pDict = s_LazyGetDict.Get( 0, newState );
                    IChunkPtr pName = s_LazyGetName.Get( 0, newState );
                    IChunkPtr pValue = s_LazyGetValue.Get( 0, newState );
                    if ( !pName ) { throw Useless::Error("<register> expects 'name'"); }
                    if ( !pValue ) { throw Useless::Error("<register> expects 'value'"); }
                    IBlock *pBlock;
                    if ( !pDict )
                    {
                        // insert symbol into current (OUTER) scope
                        pBlock = state._currentBlock;
                    }
                    else
                    {
                        // use supplied scope
                        pBlock = dynamic_cast< IBlock * >( pDict.get() );
		        if ( !pBlock ) { throw Useless::Error("<register> expects 'dict' to be type of Block"); }
                    }
                    TextUtf8 name = value_of< TextUtf8 >( pName.get() );
                    if ( !pBlock->GetChunk( name ))
                    {   // define new entry 
                        pBlock->AddChunk( name, pValue.get() );
                    }
                    else
                    {   // reset existing entry
                        pBlock->SetChunk( name, pValue.get() );
                    }
                }
                catch( Useless::Error &e )
                {
                    RaiseError( e );
                }
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<register>[<!-- [dict], name, value -->]</register>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-get");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct IS_EMPTY {};
        struct IS_NOT_DEFINED {};
        struct IS_DEFINED_EMPTY {};
        struct IS_DEFINED_NOT_EMPTY {};
        struct IS_NOT_DEFINED_OR_EMPTY {};
        
        struct KOOLIB_API IS_NOT_EMPTY : IChunk, CXmlErrors
        {            
            LazyGetChunkInScope _getChunk;

            IS_NOT_EMPTY() {}
            IS_NOT_EMPTY( const TextUtf8 &name ): _getChunk( name )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    IChunkPtr pChunk = _getChunk.Get( 0, state );
                    state.SetResult( CreateValue( (int)!IsEmpty( pChunk.get() )) );
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<is-not-empty id/>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-is-not-empty");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API IS_DEFINED : IChunk, CXmlErrors
        {
            LazyGetChunkInScope _getChunk;

            IS_DEFINED() {}
            IS_DEFINED( const TextUtf8 &name ): _getChunk( name, true )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                IChunkPtr pChunk = _getChunk.Get( 0, state );
                state.SetResult( CreateValue( (int)( 0 != pChunk.get() )) );
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<is-defined id/>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-is-defined");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API TYPE_OF : IChunk, CXmlErrors
        {
            IChunkPtr _pChunk;

            TYPE_OF(){}
            TYPE_OF( IChunk *pChunk ): _pChunk( pChunk )
            {
            }            
            
            bool Execute( Node __unused__, ExecutionState &state )
            {
                state.SetResult( CreateValue( (int)(0)));
                _pChunk->Execute( __unused__, state );
                IChunk *result = state.GetResult();
                if ( !!result )
                {
                    state.SetResult( CreateValue( ( int )( result->GetFourCC( FOURCC_NAME_TYPE ) )));
                }
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<type-of [select|force]>[<!-- right-value -->]</type-of>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-type-of");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct TYPE_OF_INTEGER {};
        struct TYPE_OF_FLOAT {};
        struct TYPE_OF_STRING {};
        struct TYPE_OF_BINARY {};
        struct TYPE_OF_BLOCK {};
        struct TYPE_OF_LIST {};
        struct TYPE_OF_EMPTY {};
        struct TYPE_OF_LAZY {};

        struct KOOLIB_API ERROR_CHUNK : IChunk, CXmlErrors
        {
            IChunkPtr _pChunk;
            
            ERROR_CHUNK() {}
            ERROR_CHUNK( IChunk *pChunk ): _pChunk( pChunk )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                std::string errString;
                try {
                    _pChunk->Execute( __unused__, state );
                    errString = value_of< TextUtf8 >( state.GetResult() );
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                throw Useless::Error("%s", errString.c_str());
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<error>[<!-- right-value -->]</error>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-error");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API BLOCK : IChunk, CXmlErrors
        {
            IChunkPtr _pChunk;
            BLOCK(){}
            BLOCK( IChunk *pChunk ): _pChunk( pChunk )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    IBlockPtr pBlock = CreateBlock();
                    ExecutionState newState( state );
                    newState._prevState = &state;
                    newState._currentBlock = pBlock.get();
                    _pChunk->Execute( __unused__, newState );
                    state.SetResult( pBlock.get() );
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<block>[<!-- right-value -->]</block>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-block");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };
        
        struct KOOLIB_API OBJECT : IChunk, CXmlErrors
        {
            IChunkPtr _pPrivate;
            IChunkPtr _pMethods;
            LazyGetChunkInScope _getObjectToExtend;
            OBJECT(){}
            OBJECT( IChunk *pPrivate, IChunk *pMethods ): _pPrivate( pPrivate ), _pMethods( pMethods )
            {
            }
            OBJECT( IChunk *pPrivate, IChunk *pMethods, TextUtf8 objectToExtend ): _pPrivate( pPrivate ), _pMethods( pMethods ), _getObjectToExtend( objectToExtend )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    IBlockPtr pBlock;
                    IBlock *pPrivate;
                    if ( _getObjectToExtend._name.empty() )
                    {
                        pBlock = CreateBlock();
                        pBlock->AddChunk(L"__private__", pPrivate = CreateBlock());
                    }
                    else
                    {
                        IChunkPtr object = _getObjectToExtend( state );
                        IChunk *priv = object->GetChunk(L"__private__");
                        pBlock = dynamic_cast< IBlock *>( object.get() );
                        pPrivate = dynamic_cast< IBlock *>( priv );
                        if ( !pBlock || !pPrivate ) {
                            throw Useless::Error("<object extend=\"name\">Chunk refered by 'name' is not an <object> type.");
                        }
                    }

                    ExecutionState newState1( state );
                    newState1._prevState = &state;
                    newState1._currentBlock = pPrivate;                    
                    _pPrivate->Execute( __unused__, newState1 );
                    
                    ExecutionState newState2( state );
                    newState2._prevState = &state;
                    newState2._currentBlock = pBlock.get();
                    _pMethods->Execute( __unused__, newState2 );
                    
                    state.SetResult( pBlock.get() );
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<object [extend]>[<!-- right-value -->]</object>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-object");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };


        struct KOOLIB_API EXTRACT : IChunk, CXmlErrors
        {
            LazyGetChunkInScope _getChunk;
            EXTRACT() {}
            EXTRACT( const TextUtf8 &name ): _getChunk( name )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    IChunkPtr pChunk = _getChunk.Get( 0, state );
                    IChunkPtr pList = pChunk->GetChunk(L"__symbols__");
                    while ( !IsEmpty( pList.get() ))
                    {
                        pList = Force( pList, state );
                        if ( IsEmpty( pList.get() )) { break; }
                        IChunk *pElement = pList->GetChunk( FOURCC_LIST_HEAD );
                        IChunk *pSymbolName = pElement->GetChunk( FOURCC_LIST_HEAD );
                        IChunk *pSymbolValue = pElement->GetChunk( FOURCC_LIST_TAIL )->GetChunk( FOURCC_LIST_HEAD );
                        state._currentBlock->AddChunk( value_of< TextUtf8 >( pSymbolName ), pSymbolValue );
                        pList = pList->GetChunk( FOURCC_LIST_TAIL );
                    }
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }

            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<extract select/>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-extract");
                    return true;
                }
                else
                {
                    return false;
                }
            }
            
        };

        struct FUNCTION {};

        struct KOOLIB_API MAP : IChunk, CXmlErrors
        {
            TextUtf8  _iterator;
            IChunkPtr _pChunk;
            unsigned int _iterId;
            
            MAP(){}
            MAP( const TextUtf8 &iterator, IChunk *pChunk ): _iterator( iterator ), _pChunk( pChunk )
            {
                _iterId = GetGlobalSymbolDict().AddSymbol( _iterator );
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    static LazyGetChunkInScope s_LazyGetList(L"list", true);
                    static LazyGetChunkInScope s_LazyGetFunc(L"func", true);
                    SubScope newState( state );
                    _pChunk->Execute( __unused__, newState );
                    IChunkPtr pEnd = CreateEmpty();
                    IChunkPtr pHead = pEnd;
                    XMLListChunk *pLast = 0;
                    IChunkPtr pList = s_LazyGetList.Get( 0, newState );
                    IChunkPtr pFunc = s_LazyGetFunc.Get( 0, newState );
                    if ( !pList.get() ) { throw Useless::Error("<map> Undefined symbol 'list'"); }
                    if ( !pFunc.get() ) { throw Useless::Error("<map> Undefined symbol 'func'"); }
                    while ( !IsEmpty( pList.get() ))
                    {
                        pList = Force( pList, state );
                        if ( IsEmpty( pList.get() )) { break; }
                        IChunk *pElement = pList->GetChunk( FOURCC_LIST_HEAD );
                        newState.Reuse();
                        newState._currentBlock->AddChunk( _iterId, pElement );
                        pFunc->Execute( __unused__, newState );
                        if ( !IsEmpty( newState.GetResult() ))
                        {
                            XMLListChunk *p = CreateList( newState.GetResult(), pEnd.get() );
                            if ( 0 == pLast )
                            {
                                pHead = p;
                            }
                            else
                            {                
                                pLast->SetTail( p );
                            }
                            pLast = p;
                        }
                        pList = pList->GetChunk( FOURCC_LIST_TAIL );
                    }
                    state.SetResult( pHead.get() );
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }

            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<map>\n"
                            L"\t<iterator id/>\n"
                            L"\t{<list [select]/>/<range><!-- [first], count --></range>}\n"
                            L"\t<!-- instructions -->\n"
                            L"</map>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-map");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API FOLD : IChunk, CXmlErrors
        {
            TextUtf8  _iterator;
            TextUtf8  _accum;
            IChunkPtr _pChunk;
            unsigned int _iterId;
            unsigned int _accumId;
            LazyGetChunkInScope _getAccum;
            FOLD(){}
            FOLD( const TextUtf8 &iter, const TextUtf8 &accum, IChunk *pChunk )
                :_iterator(iter),_accum(accum),_pChunk(pChunk)
            {
                _iterId = GetGlobalSymbolDict().AddSymbol( _iterator );
                _accumId = GetGlobalSymbolDict().AddSymbol( _accum );
                _getAccum = LazyGetChunkInScope( _accum, true);
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    static LazyGetChunkInScope s_LazyGetFunc(L"func", true);
                    static LazyGetChunkInScope s_LazyGetList(L"list", true);
                    static LazyGetChunkInScope s_LazyGetAccum(L"accum", true);
                    SubScope newState( state );
                    _pChunk->Execute( __unused__, newState );
                    IChunkPtr pFunc  = s_LazyGetFunc.Get( 0,  newState);
                    IChunkPtr pList  = s_LazyGetList.Get( 0,  newState);
                    IChunkPtr pAccum = s_LazyGetAccum.Get( 0, newState);
                    if ( !pAccum.get() )
                    {   // try new accumulator name (semantic sugar)
                        pAccum = _getAccum.Get( 0, newState );
                    }
                    if ( !pFunc.get() ) { throw Useless::Error("<fold> Undefined symbol 'func'"); }
                    if ( !pList.get() ) { throw Useless::Error("<fold> Undefined symbol 'list'"); }
                    if ( !pAccum.get()) { throw Useless::Error("<fold> Undefined symbol 'accum'"); }
                    while ( !IsEmpty( pList.get() ))
                    {
                        pList = Force( pList, state );
                        if ( IsEmpty( pList.get() )) { break; }
                        IChunk *pElement = pList->GetChunk( FOURCC_LIST_HEAD );
                        newState.Reuse();
                        newState._currentBlock->AddChunk( _iterId, pElement );
                        newState._currentBlock->AddChunk( _accumId, pAccum.get() );
                        pFunc->Execute( __unused__, newState );
                        pList = pList->GetChunk( FOURCC_LIST_TAIL );
                        pAccum = newState.GetResult();
                    }
                    state.SetResult( pAccum.get() );
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }

            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<fold>\n"
                            L"\t<iterator id/>\n"
                            L"\t<accumulator id [select]><!-- right-value --></accumulator>\n"
                            L"\t{<list [select]/>/<range><!-- [first], count --></range>}\n"
                            L"\t<!-- instructions -->\n"
                            L"</fold>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-fold");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API SEEK : IChunk, CXmlErrors
        {
            IChunkPtr _pChunk;

            SEEK(){}
            SEEK( IChunk *pChunk ): _pChunk(pChunk)
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    static LazyGetChunkInScope s_LazyGetCount(L"count", true);
                    static LazyGetChunkInScope s_LazyGetList(L"list", true);
                    SubScope newState( state );
                    _pChunk->Execute( __unused__, newState );
                    IChunkPtr pCount = s_LazyGetCount.Get( 0, newState );
                    IChunkPtr pList = s_LazyGetList.Get( 0, newState );
                    if ( !pList.get() ) { throw Useless::Error("<seek> Undefined symbol 'list'"); }
                    if ( !pCount.get() ) { throw Useless::Error("<seek> Undefined symbol 'count'"); }
                    int Count = value_of< int >( pCount.get() );
                    for ( int i = 0; i < Count && !IsEmpty( pList.get() ); ++i)
                    {
                        pList = Force( pList, state );
                        if ( IsEmpty( pList.get() )) { break; }
                        pList = pList->GetChunk( FOURCC_LIST_TAIL );
                    }
                    state.SetResult( pList.get() );
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }

            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<seek>\n<!-- 'list' 'count' -->\n</seek>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-seek");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API HEAD : IChunk, CXmlErrors
        {
            IChunkPtr _pChunk;

            HEAD(){}
            HEAD( IChunk *pChunk ): _pChunk(pChunk)
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    static LazyGetChunkInScope s_LazyGetCount(L"count", true);
                    static LazyGetChunkInScope s_LazyGetList(L"list", true);
                    SubScope newState( state );
                    _pChunk->Execute( __unused__, newState );
                    IChunkPtr pCount = s_LazyGetCount.Get( 0, newState );
                    IChunkPtr pList = s_LazyGetList.Get( 0, newState );
                    if ( !pList.get() ) { throw Useless::Error("<head> Undefined symbol 'list'"); }
                    if ( !pCount.get() ) { throw Useless::Error("<head> Undefined symbol 'count'"); }
                    int nCount = value_of< int >( pCount.get() );
                    state.SetResult( new LazyHead( nCount, pList.get() ));
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }

            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<head>\n<!-- 'list' 'count' -->\n</head>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-head");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API RANGE : IChunk, CXmlErrors
        {
            IChunkPtr _pChunk;

            RANGE() {}
            RANGE( IChunk *pChunk ): _pChunk( pChunk )
            {
            }
            
            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    static LazyGetChunkInScope s_LazyGetCount(L"count", true);
                    static LazyGetChunkInScope s_LazyGetFirst(L"first", true);
                    SubScope newState( state );
                    _pChunk->Execute( __unused__, newState );
                    IChunkPtr pCount = s_LazyGetCount.Get( 0, newState );
                    IChunkPtr pFirst  = s_LazyGetFirst.Get( 0, newState );
                    int nFirst = 0;
                    if ( 0 != pFirst.get() ) 
                    {
                        nFirst = value_of< int >( pFirst.get() );
                    }
                    if ( !pCount.get() ) { throw Useless::Error("<range> Undefined symbol 'count'"); }
                    int nCount = value_of< int >( pCount.get() );
                    state.SetResult( new LazyRange( nFirst, nCount ));
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }

            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<range>\n<!-- 'first' 'count' -->\n</range>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-range");
                    return true;
                }
                else
                {
                    return false;
                }
            }

        };
        

        struct KOOLIB_API ZIP : IChunk, CXmlErrors
        {
            IChunkPtr _pChunk;

            ZIP(){}
            ZIP( IChunk *pChunk ): _pChunk( pChunk )
            {}

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    SubScope newState( state );
                    IChunkPtr pEnd = CreateEmpty();
                    IChunkPtr pHead = pEnd;
                    XMLListChunk *pLast = 0;

                    IChunkPtr pList = _pChunk.get();
                    while ( !IsEmpty( pList.get() ))
                    {
                        pList = Force( pList, state );
                        if ( IsEmpty( pList.get() )) { break; }
                        IChunk *pElement = pList->GetChunk( FOURCC_LIST_HEAD );
                        newState.Reuse();
                        pElement->Execute( __unused__, newState );
                        IChunk *pNext = newState.GetResult();
                        if ( !IsEmpty( pNext ))
                        {
                            XMLListChunk *p = CreateList( pNext, pEnd.get() );
                            if ( 0 == pLast )
                            {
                                pHead = p;
                            }
                            else
                            {                
                                pLast->SetTail( p );
                            }
                            pLast = p;
                        }
                        pList = pList->GetChunk( FOURCC_LIST_TAIL );
                    }
                    state.SetResult( new LazyZip( pHead.get() ));
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }

            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<zip>\n<!-- several lists -->\n</zip>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-zip");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API CALL : IChunk, CXmlErrors
        {
            LazyGetChunkInScope _getChunk;
            IChunkPtr _paramDecl;

            CALL() {}
            CALL( const TextUtf8 &name, IChunk *paramDecl ): _getChunk( name ), _paramDecl( paramDecl )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    IChunkPtr pChunk = _getChunk.Get( 0, state );
                    SubScope newState( state );
                    _paramDecl->Execute( __unused__, newState );
                    pChunk->Execute( __unused__, newState );
                    state.SetResult( newState.GetResult() );
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }

            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    //attr.str(L"<call id>\n<!-- parameters -->\n</call>");
                    attr.str(L"<apply select>\n<!-- parameters -->\n</apply>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    //attr.str(L"instruction-call");
                    attr.str(L"instruction-apply");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API LAZY_CALL : IChunk, CXmlErrors
        {
            LazyGetChunkInScope _getChunk;
            IChunkPtr _paramDecl;

            LAZY_CALL() {}
            LAZY_CALL( const TextUtf8 &name, IChunk *paramDecl ): _getChunk( name ), _paramDecl( paramDecl )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    IChunkPtr pChunk = _getChunk.Get( 0, state );
                    SubScope newState( state );
                    _paramDecl->Execute( __unused__, newState );
                    state.SetResult( new LazyCall( pChunk.get(), newState._currentBlock ));
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }

            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    //attr.str(L"<lazy-call id>\n<!-- parameters -->\n</lazy-call>");
                    attr.str(L"<curry select>\n<!-- parameters -->\n</curry>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-curry");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };
        
        struct KOOLIB_API LIST_NODE : IChunk, CXmlErrors
        {
            IChunkPtr _pHead;
            IChunkPtr _pTail;
            
            LIST_NODE(){}
            LIST_NODE( IChunk *pHead, IChunk *pTail ): _pHead( pHead ), _pTail( pTail )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    SubScope newState( state );
                    _pHead->Execute( __unused__, newState );
                    IChunkPtr pHead = newState.GetResult();
                    newState.Reuse();
                    _pTail->Execute( __unused__, newState );
                    IChunkPtr pTail = newState.GetResult();
                    state.SetResult( CreateList( pHead.get(), pTail.get() ));
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<listnode>\n<!-- head -->\n<!-- tail -->\n</listnode>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-list-node");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };
        
        struct KOOLIB_API LISTNODE : IChunk, CXmlErrors
        {
            IChunkPtr _pChunk;
            LISTNODE() {}
            LISTNODE( IChunk *pChunk ): _pChunk( pChunk )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    SubScope newState( state );
                    _pChunk->Execute( __unused__, newState );
                    IChunkPtr pHead = newState._currentBlock->GetChunk( FOURCC_LIST_HEAD );
                    IChunkPtr pTail = newState._currentBlock->GetChunk( FOURCC_LIST_TAIL );
                    state.SetResult( CreateList( pHead.get(), pTail.get() ));
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }

            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<listnode>[<!-- head, tail -->]</listnode>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-listnode");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API DOIN : IChunk, CXmlErrors
        {
            IChunkPtr _pGetter;
            IChunkPtr _pChunk;
            bool _blockResult;
            DOIN() {}
            DOIN( IChunk *pGetter, IChunk *pChunk, bool blockResult )
                : _pGetter( pGetter ), _pChunk( pChunk ), _blockResult( blockResult )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    state.SetResult(0);
                    _pGetter->Execute( __unused__, state );
                    ExecutionState newState( state );
                    newState._prevState = &state;
                    newState._currentBlock = dynamic_cast< IBlock *>( state.GetResult() );
                    if ( !newState._currentBlock ) { throw Useless::Error("<doin> block expected"); }
                    _pChunk->Execute( __unused__, newState );
                    if ( !_blockResult )
                    {
                        state.SetResult( newState.GetResult() );
                    }
                    // else: state.SetResult is not required here
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<doin [select]>[<!-- block -->]<!-- right-value --></get>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-doin");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct LIST {};        
        struct APPLY {};
        struct CURRY {};
        struct DO {};        

        struct EMPTY {};
        struct BOOLEAN {};
        struct INTEGER {};
        struct FLOAT {};
        struct STRING {};

        struct KOOLIB_API COPY : IChunk, CXmlErrors
        {
            Useless::XMLDocument _nodes;
            COPY(){}
            COPY( Node node ): _nodes( node.SubTree() )
            {}

            bool Execute( Node __unused__, ExecutionState &state )
            {
                IBlockPtr pBlock = XMLNodeToBlock( _nodes.Root(), state );
                state.SetResult( pBlock.get() );
                return true;
            }
        };

        struct NOCOMPILE {};

        struct ADD {};
        struct MUL {};
        struct FADD {};
        struct FMUL {};
        struct OR {};
        struct AND {};
        struct COMPARE {};
        struct FCOMPARE {};
        struct STRCMP {};
        struct TYPECMP {};
        struct CAT {};

        template< class _T >
        struct KOOLIB_API Increase : IChunk, CXmlErrors
        {
            TextUtf8     _name;
            IChunkPtr    _value;

            Increase() {}
            Increase( const TextUtf8 &name, IChunk *pChunk ): _name( name ), _value( pChunk )
            {
            }
            
            bool Execute( Node __unused__, ExecutionState &state )
			{
                try {
                    IChunk *pParent = 0;
                    IChunk *pChunk = XMLProgram::GetChunk( _name, state, &pParent );
                    if ( !pChunk ) { throw Useless::Error("<inc> Undefined symbol %S", _name.c_str() ); }
                    if ( !pParent) { throw Useless::Error("<inc> Undefined symbol parent %S", _name.c_str() ); }
                    SplitNames names = SplitChunkName( _name, state );
                    while ( !names.second.empty() )
                    {
                        names = SplitChunkName( names.second, state );
                    }
                    _value->Execute( __unused__, state );
                    if ( 0 != state.GetResult() )
                    {
                        _T in1, in2;
                        (*pChunk) >> in1;
                        (*state.GetResult()) >> in2;
                        pParent->SetChunk( names.first, CreateValue( in1 + in2 ));
                    }
                    state.SetResult(0);
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }

            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<inc id [select|call]>[<!-- right-value -->]</inc>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-inc");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };
        
        typedef Increase< int > INC;
        typedef Increase< double > FINC;
        
        struct KOOLIB_API LESS : IChunk
        {
            IChunkPtr _value;
            LESS(){}
            LESS( IChunk *value ): _value( value )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                _value->Execute( __unused__, state );
                state.SetResult( CreateValue( 0 > value_of< int >( state.GetResult() ) ));
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<less [select]>[<!-- right-value {-1,0,1} -->]</less>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-less");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API GREATER : IChunk
        {
            IChunkPtr _value;
            GREATER(){}
            GREATER( IChunk *value ): _value( value )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                _value->Execute( __unused__, state );
                state.SetResult( CreateValue( 0 <  value_of< int >( state.GetResult() ) ));
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<greater [select]>[<!-- right-value {-1,0,1} -->]</greater>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-greater");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API EQUAL : IChunk
        {
            IChunkPtr _value;
            EQUAL(){}
            EQUAL( IChunk *value ): _value( value )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                _value->Execute( __unused__, state );
                state.SetResult( CreateValue( 0 == value_of< int >( state.GetResult() ) ));
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<equal [select]>[<!-- right-value {-1,0,1} -->]</equal>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-equal");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API NOT : IChunk
        {
            IChunkPtr _value;
            NOT(){}
            NOT( IChunk *value ): _value( value )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                _value->Execute( __unused__, state );
                state.SetResult( CreateValue( !value_of< int >( state.GetResult() ) ));
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<not [select]>[<!-- right-value -->]</not>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-not");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        template< class _Type >
        struct KOOLIB_API MINUS : IChunk
        {
            IChunkPtr _value;
            MINUS(){}
            MINUS( IChunk *value ): _value( value )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                _value->Execute( __unused__, state );
                state.SetResult( CreateValue( -value_of< _Type >( state.GetResult() ) ));
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<minus [select]>[<!-- right-value -->]</minus>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-minus");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API RECIPROCAL : IChunk
        {
            IChunkPtr _value;
            RECIPROCAL(){}
            RECIPROCAL( IChunk *value ): _value( value )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                _value->Execute( __unused__, state );
                state.SetResult( CreateValue( 1.0/value_of< double >( state.GetResult() ) ));
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<reciprocal [select]>[<!-- right-value -->]</reciprocal>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-reciprocal");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API IF : IChunk, CXmlErrors
        {
            IChunkPtr _pCond;
            IChunkPtr _pChunk;

            IF(){}
            IF( IChunk *pCond, IChunk *pChunk ): _pCond( pCond ), _pChunk( pChunk )
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    SubScope newState( state );
                    _pCond->Execute( __unused__, newState );
                    if ( 0 == !value_of< int >( newState.GetResult() ))
                    {
                        newState.Reuse();
                        _pChunk->Execute( __unused__, newState );
                        state.SetResult( newState.GetResult() );
                    }
                    else
                    {
                        state.SetResult( CreateEmpty() );
                    }
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<if [select|force]>[<!-- cond -->][<!-- right-value -->]</if>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-if");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        struct KOOLIB_API CHOOSE : IChunk, CXmlErrors
        {
            std::vector< std::pair< IChunkPtr, IChunkPtr > > _options;
            IChunkPtr _otherwise;

            CHOOSE()
            {
            }

            void AddOption( IChunkPtr pTest, IChunkPtr pOption )
            {
                _options.push_back( std::make_pair( pTest, pOption ));
            }

            void SetOtherwise( IChunkPtr pOtherwise )
            {
                if ( 0 != _otherwise.get() ) { throw Useless::Error("<choose> May contain only one <othrwise>"); }
                _otherwise = pOtherwise;
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                try {
                    SubScope newState( state );
                    for ( int i=0, I=_options.size(); i<I; ++i )
                    {
                        _options[i].first->Execute( __unused__, newState );
                        if ( 0 == !value_of< int >( newState.GetResult() ))
                        {                    
                            newState.Reuse();
                            _options[i].second->Execute( __unused__, newState );
                            state.SetResult( newState.GetResult() );
                            return true;
                        }
                        newState.Reuse();
                    }
                    _otherwise->Execute( __unused__, newState );
                    state.SetResult( newState.GetResult() );
                }
                catch( Useless::Error &e ) { RaiseError( e ); }
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"help" )
                {
                    attr.str(L"<chose>\n"
                            L"\t<when><!-- instructions --></when>\n"
                            L"\t<otherwise><!-- instructions --></othrwise>\n"
                            L"</choose>");
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str(L"instruction-choose");
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        template < class _Op > struct OpParamSetupNone
        {
            static void Setup( _Op &op ) {}
        };

        template< class _Op >
            struct KOOLIB_API ValueAccumulator : IChunk, CXmlErrors
            {
                typedef typename _Op::Type Type;
                typedef typename _Op::CType CType;
                std::vector< IChunkPtr > _input;
                CType _params;

                ValueAccumulator()
                {}

                ValueAccumulator( const CType &params ): _params( params )
                {}

                void Add( IChunk *pChunk )
                {
                    _input.push_back( pChunk );
                }

                bool Execute( Node __unused__, ExecutionState &state )
                {
                    try {
                        SubScope newState( state );
                        _Op op( _params );
                        for ( std::vector< IChunkPtr >::iterator it = _input.begin();
                                it != _input.end(); ++it )
                        {
                            (*it)->Execute( __unused__, newState );
                            if ( !IsEmpty( newState.GetResult() ))
                            {
                                op.apply( value_of< Type >( newState.GetResult() ));
                            }
                            if ( op.is_result() )
                            {
                                break;
                            }
                            newState.Reuse();
                        }
                        state.SetResult( CreateValue( op.get() ));
                    }
                    catch( Useless::Error &e ) { RaiseError( e ); }
                    return true;
                }
            };

        template< class _Type > struct OpADD
        {
            typedef _Type Type;
            typedef int CType;
            Type _acc; OpADD( int  =  0): _acc(0) {}
            void apply( const Type &in ) { _acc += in; }
            const Type & get() { return _acc; }
            bool is_result() { return false; }
        };

        template< class _Type > struct OpMUL
        {
            typedef _Type Type;
            typedef int CType;
            Type _acc; OpMUL( int = 0 ): _acc(1) {}
            void apply( const Type &in ) { _acc *= in; }
            const Type & get() { return _acc; }
            bool is_result() { return false; }
        };

        struct OpOR
        {
            typedef bool Type;
            typedef int CType;
            Type _acc; OpOR( int = 0 ): _acc(0) {}
            void apply( Type in ) { _acc |= in; }
            const Type & get() { return _acc; }
            bool is_result() { return (1==_acc); }
        };

        struct OpAND
        {
            typedef bool Type;
            typedef int CType;
            Type _acc; OpAND( int = 0 ): _acc(1) {}
            void apply( Type in ) { _acc &= in; }
            const Type & get() { return _acc; }
            bool is_result() { return (0==_acc); }
        };

        template< class _Type >
            struct OpCOMPARE
            {
                typedef _Type Type;
                typedef int CType;
                Type _x[2]; int _n;
                OpCOMPARE( int = 0 ): _n(0) {}
                void apply( const Type &in ) { if ( 2 > _n )
                    { _x[_n++] = in; } }
                int get()
                {
                    if ( 2 <= _n )
                    {
                        return (_x[0] < _x[1] ? -1 : (_x[1] < _x[0] ? 1 : 0));
                    }
                    else
                    {
                        throw Useless::Error("<compare> at least two arguments expected (1-st and 2-nd are compared).");
                    }
                }
                bool is_result() { return (2==_n); }
            };

        struct OpCAT
        {
            typedef TextUtf8 Type;
            typedef TextUtf8 CType;
            Type _acc;
            Type _separator;
            OpCAT( const TextUtf8 &separator = L"" ): _separator( separator )
            {}
            void apply( const Type &in )
            {
                if ( !_acc.empty() )
                {
                    _acc += _separator;
                }
                _acc += in;
            }
            const Type & get() { return _acc; }
            bool is_result() { return false; }
        };
        
        
        // Used to compare objects via pointers.
        struct PTRCMP : IChunk
        {
            IChunkPtr _first;
            IChunkPtr _second;
            PTRCMP( IChunk *pFirst, IChunk *pSecond ): _first(pFirst), _second(pSecond)
            {
            }

            bool Execute( Node __unused__, ExecutionState &state )
            {
                OpCOMPARE< IChunk * > op;
                SubScope newState( state );
                if ( _first.get() )
                {
                    _first->Execute( __unused__, newState );
                    op.apply( newState.GetResult() );
                    newState.Reuse();
                }
                if ( _second.get() )
                {
                    _second->Execute( __unused__, newState );
                    op.apply( newState.GetResult() );
                }
                state.SetResult( CreateValue( op.get() ));
                return true;
            }
        };

        struct EQUAL_STR {};
        struct EQUAL_PTR {};
        struct EQUAL_INT {};
        struct EQUAL_FLOAT {};
        struct EQUAL_TYPE {};
        
        struct LESS_STR {};
        struct LESS_PTR {};
        struct LESS_INT {};
        struct LESS_FLOAT {};
        struct LESS_TYPE {};
        
        struct GREATER_STR {};
        struct GREATER_PTR {};
        struct GREATER_INT {};
        struct GREATER_FLOAT {};
        struct GREATER_TYPE {};

        struct TRUE_VALUE {};
        struct FALSE_VALUE {};

    };

};//XMLProgram

#endif//__INCLUDED_KOOLIXP_CXML_H__
