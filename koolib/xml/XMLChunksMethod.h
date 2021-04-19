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
  last mod: $Id: XMLChunksMethod.h,v 1.10 2005/03/18 17:48:36 koolas Exp $



  To convert from C++ type to XML IChunk:
    
    result_traits< _R >::get( const _R & );



  To convert from XML to C++ object:

    argument_traits< _A >::get( const TextAnsi &, Node, ExecutionState & );



  To eliminate result when C++ function returns 'void':
    
    method_call_traits< _R >::call( ExecutionState &, CalleePtr, MethodPtr [,A1 [,A2]] );



  To interpret 'const' methods as 'non-const':
    
    unconst_method_cast( MethodPtr );



  To create bound method IChunk:

    make_method_chunk( CalleePtr, MethodPtr [,const TextAnsi & [,const TextAnsi & ]] );



  To add chunks and bind methods:

    method_adder< Callee > add_methods( Callee *userBlock );
    method_adder< Callee > add_methods( Callee *user, IBlock *block );
    method_adder< Callee > add_methods_block( const TextAnsi &name, Callee *user, IBlock *parentBlock );

    userBlock   = user struct inherting IBlock interface
    user        = any user struct
    block       = block to which methods are added
    name        = name of new block
    parentBlock = parent block of new block

        .def( const TextAnsi &, MethodPtr [,const TextAnsi & [,const TextAnsi & ]] )        
        .add( const TextAnsi &, IChunk * )
        .add_value( const TextAnsi &, const Type & )

 ********************************************************************/


#ifndef __INCLUDED_XML_METHOD_CHUNKS_H__
#define __INCLUDED_XML_METHOD_CHUNKS_H__

#include "XMLProgram.h"
#include <boost/shared_ptr.hpp>

namespace XMLProgram {
    
    using XMLFactory::Node;
    using XMLFactory::Attr;

    template< class _T > struct tag {};

    struct ignore_t
    {
        ignore_t() {}
        ignore_t(...) {}
    };

    template< class _R > struct result_traits
    {
        static IChunk * get( const _R &result )
        {
            return make_value_chunk( result );
        }
    };

    KOOLIB_SPECIALIZATION struct result_traits< IChunk * >
    {
        static IChunk * get( IChunk *ptr )
        {
            return ptr;
        }
    };
    
    KOOLIB_SPECIALIZATION struct result_traits< IChunkPtr >
    {
        static IChunk * get( const IChunkPtr &ptr )
        {
            return ptr.get();
        }
    };

    template< class _R >
        struct method_call_traits
        {
            template< class _Cp, class _Mp >
                static void call( ExecutionState &state, _Cp ptr, _Mp mptr )
                {
                    state.SetResult( result_traits< _R >::get( ((*ptr).*mptr)() ));
                }
            
            template< class _Cp, class _Mp, class _A1 >
                static void call( ExecutionState &state, _Cp ptr, _Mp mptr, const _A1 &a1 )
                {
                    state.SetResult( result_traits< _R >::get( ((*ptr).*mptr)( a1 ) ));
                }
            
            template< class _Cp, class _Mp, class _A1, class _A2 >
                static void call( ExecutionState &state, _Cp ptr, _Mp mptr, const _A1 &a1, const _A2 &a2 )
                {
                    state.SetResult( result_traits< _R >::get( ((*ptr).*mptr)( a1, a2 ) ));
                }
            
            template< class _Cp, class _Mp, class _A1, class _A2, class _A3 >
                static void call( ExecutionState &state, _Cp ptr, _Mp mptr, const _A1 &a1, const _A2 &a2, const _A3 &a3 )
                {
                    state.SetResult( result_traits< _R >::get( ((*ptr).*mptr)( a1, a2, a3 ) ));
                }
            
            template< class _Cp, class _Mp, class _A1, class _A2, class _A3, class _A4 >
                static void call( ExecutionState &state, _Cp ptr, _Mp mptr, const _A1 &a1, const _A2 &a2, const _A3 &a3, const _A4 &a4 )
                {
                    state.SetResult( result_traits< _R >::get( ((*ptr).*mptr)( a1, a2, a3, a4 ) ));
                }
        };

    struct method_call_traits_no_result
    {
        template< class _Cp, class _Mp >
            static void call( ExecutionState &state, _Cp ptr, _Mp mptr )
            {
                ((*ptr).*mptr)();
                state.SetResult( NULL );
            }

        template< class _Cp, class _Mp, class _A1 >
            static void call( ExecutionState &state, _Cp ptr, _Mp mptr, const _A1 &a1 )
            {
                ((*ptr).*mptr)( a1 );
                state.SetResult( NULL );
            }

        template< class _Cp, class _Mp, class _A1, class _A2 >
            static void call( ExecutionState &state, _Cp ptr, _Mp mptr, const _A1 &a1, const _A2 &a2 )
            {
                ((*ptr).*mptr)( a1, a2 );
                state.SetResult( NULL );
            }


        template< class _Cp, class _Mp, class _A1, class _A2, class _A3 >
            static void call( ExecutionState &state, _Cp ptr, _Mp mptr, const _A1 &a1, const _A2 &a2, const _A3 &a3 )
            {
                ((*ptr).*mptr)( a1, a2, a3 );
                state.SetResult( NULL );
            }

        template< class _Cp, class _Mp, class _A1, class _A2, class _A3, class _A4 >
            static void call( ExecutionState &state, _Cp ptr, _Mp mptr, const _A1 &a1, const _A2 &a2, const _A3 &a3, const _A4 &a4 )
            {
                ((*ptr).*mptr)( a1, a2, a3, a4 );
                state.SetResult( NULL );
            }
    };

    KOOLIB_SPECIALIZATION struct method_call_traits< void > : method_call_traits_no_result
    {
    };
    
    KOOLIB_SPECIALIZATION struct method_call_traits< ignore_t > : method_call_traits_no_result
    {
    };

   
    template< class _T > struct attr_traits
    {
        template< const bool _F > static _T & get( Attr< _T, _F > &a ) { return *a; }
    };

    KOOLIB_SPECIALIZATION struct attr_traits< std::string >
    {
        template< const bool _F > static TextUtf8 get( Attr< std::string, _F > &a ) { return a.str(); }
    };
    
    KOOLIB_SPECIALIZATION struct attr_traits< TextUtf8 >
    {
        template< const bool _F > static TextUtf8 get( Attr< TextUtf8, _F > &a ) { return a.str(); }
    };

    template< class _T >
        struct argument_traits
        {
            static _T get( const TextAnsi &name, Node node, ExecutionState &state )
            {
                Attr< _T > attr(name);
                GetAttr( attr, node, state );
                return attr_traits< _T >::get( attr );
            }
        };

    // Use reference wrappers with def_remap_n to avoid refrence-to-refrence problems
    template< class _T > struct refrence_wrapper { refrence_wrapper( _T &x ): m_x(x) {} _T &m_x; operator _T &() { return m_x; } };
    template< class _T > struct const_refrence_wrapper { const_refrence_wrapper( const _T &x ): m_x(x) {} const _T &m_x; operator const _T &() { return m_x; } };
    template< class _T > struct unknown_refrence_wrapper{ unknown_refrence_wrapper( _T &x ): m_x(x) {} _T &m_x; operator _T &() { return m_x; } };
    template< class _T > struct unknown_const_refrence_wrapper { unknown_const_refrence_wrapper( const _T &x ): m_x(x) {} const _T &m_x; operator const _T &() { return m_x; } };

    template< class _T > IChunk *make_value_chunk( const refrence_wrapper< _T > &ref )
    {
        return make_value_chunk( ref.m_x );
    }

    template< class _T > IChunk *make_value_chunk( const const_refrence_wrapper< _T > &ref )
    {
        return make_value_chunk( ref.m_x );
    }

    template< class _T > struct UnknownChunk_byReference : IChunk
    {
        _T &m_ref; UnknownChunk_byReference( _T &ref ): m_ref( ref ) {}
    };

    template< class _T > struct UnknownChunk_byReferenceConst : IChunk
    {
        const _T &m_ref; UnknownChunk_byReferenceConst( const _T &ref ): m_ref( ref ) {}
    };

    template< class _T > struct UnknownChunk_byPointer : IChunk
    {
        _T *m_ptr; UnknownChunk_byPointer( _T *ptr ): m_ptr( ptr ) {}
    };

    template< class _T > struct UnknownChunk_byPointerConst : IChunk
    {
        const _T *m_ptr; UnknownChunk_byPointerConst( const _T *ptr ): m_ptr( ptr ) {}
    };

    template< class _T > IChunk *make_value_chunk( const unknown_refrence_wrapper< _T > &ref )
    {
        return new UnknownChunk_byReference< _T >( ref.m_x );
    }

    template< class _T > IChunk *make_value_chunk( const unknown_const_refrence_wrapper< _T > &ref )
    {
        return new UnknownChunk_byReferenceConst< _T >( ref.m_x );
    }

    // @TODO: Works only if template specialization is supported !!!
    template< class _T > struct argument_traits< unknown_const_refrence_wrapper< _T > >
    {
        static const _T & get( const TextAnsi &name, Node node, ExecutionState &state )
        {
            IChunkPtr pChunk = GetChunk( name, state );
            UnknownChunk_byReferenceConst< _T > *pConst = dynamic_cast< UnknownChunk_byReferenceConst< _T > * >( pChunk.get() );
            if ( 0 == pConst )
            {
                throw Useless::Error("UnknownChunk_byReferenceConst< %s > expected", typeid(_T).name());
            }
            return pConst->m_ref;
        }
    };


    template< class _Ret, class _Cl >
        struct unconst_method_0
        {
            typedef _Ret (_Cl::*Type)();
        };
    
    template< class _Ret, class _Cl >
        KOOLIB_TYPENAME unconst_method_0< _Ret, _Cl >::Type unconst_method_cast( _Ret (_Cl::*mp)() const )
        {
            return reinterpret_cast< _Ret (_Cl::*)() >( mp );
        }

    template< class _Ret, class _Cl, class _A1 >
        struct unconst_method_1
        {
            typedef _Ret (_Cl::*Type)( _A1 );
        };
    
    template< class _Ret, class _Cl, class _A1 >
        KOOLIB_TYPENAME unconst_method_1< _Ret, _Cl, _A1 >::Type unconst_method_cast( _Ret (_Cl::*mp)( _A1 ) const )
        {
            return reinterpret_cast< _Ret (_Cl::*)( _A1 ) >( mp );
        }

    template< class _Ret, class _Cl, class _A1, class _A2 >
        struct unconst_method_2
        {
            typedef _Ret (_Cl::*Type)( _A1, _A2 );
        };
    
    template< class _Ret, class _Cl, class _A1, class _A2 >
        KOOLIB_TYPENAME unconst_method_2< _Ret, _Cl, _A1, _A2 >::Type unconst_method_cast( _Ret (_Cl::*mp)( _A1, _A2 ) const )
        {
            return reinterpret_cast< _Ret (_Cl::*)( _A1, _A2 ) >( mp );
        }

    template< class _Ret, class _Cl, class _A1, class _A2, class _A3 >
        struct unconst_method_3
        {
            typedef _Ret (_Cl::*Type)( _A1, _A2, _A3 );
        };
    
    template< class _Ret, class _Cl, class _A1, class _A2, class _A3 >
        KOOLIB_TYPENAME unconst_method_3< _Ret, _Cl, _A1, _A2, _A3 >::Type unconst_method_cast( _Ret (_Cl::*mp)( _A1, _A2, _A3 ) const )
        {
            return reinterpret_cast< _Ret (_Cl::*)( _A1, _A2, _A3 ) >( mp );
        }

    template< class _Ret, class _Cl, class _A1, class _A2, class _A3, class _A4 >
        struct unconst_method_4
        {
            typedef _Ret (_Cl::*Type)( _A1, _A2, _A3, _A4 );
        };
    
    template< class _Ret, class _Cl, class _A1, class _A2, class _A3, class _A4 >
        KOOLIB_TYPENAME unconst_method_4< _Ret, _Cl, _A1, _A2, _A3, _A4 >::Type unconst_method_cast( _Ret (_Cl::*mp)( _A1, _A2, _A3, _A4 ) const )
        {
            return reinterpret_cast< _Ret (_Cl::*)( _A1, _A2, _A3, _A4 ) >( mp );
        }

    template< class _R, class _Cp, class _Mp >
        struct MethodChunk : virtual IChunk
        {
            _Cp _ptr;
            _Mp _mptr;

            bool Execute( Node node, ExecutionState &state )
            {
                method_call_traits< _R >::call( state, _ptr, _mptr, node );
                return true;
            }
        
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"cpp-type-name" )
                {
                    TextUtf8 result_tn = TypeNameTraits< _R >::name();
                    TextUtf8 callee_tn = TypeNameTraits< _Cp >::name();
                    TextUtf8 method = result_tn+L" "+callee_tn+L" method( node )";
                    attr.str( method );
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    TextUtf8 result_tn = TypeNameTraits< _R >::name();
                    TextUtf8 method = L"function( Node )" + L" -> "+ result_tn;
                    attr.str( method );
                    return true;
                }

                return false;
            }
        
        };
    
    template< class _R, class _Cp, class _Mp >
        struct MethodChunk2 : virtual IChunk
        {
            _Cp _ptr;
            _Mp _mptr;

            bool Execute( Node node, ExecutionState &state )
            {
                ((*_ptr).*_mptr)( node, state );
                return true;
            }
        
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"cpp-type-name" )
                {
                    TextUtf8 result_tn = L"__result__";
                    TextUtf8 callee_tn = TypeNameTraits< _Cp >::name();
                    TextUtf8 method = result_tn+L" "+callee_tn+L" method( node )";
                    attr.str( method );
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    TextUtf8 result_tn = TypeNameTraits< _R >::name();
                    TextUtf8 method = L"function( Node, State ) -> "+ result_tn;
                    attr.str( method );
                    return true;
                }

                return false;
            }
        
        };

    template< class _R, class _Cp, class _Mp >
        struct MethodChunkA0 : virtual IChunk
        {
            _Cp _ptr;
            _Mp _mptr;

            bool Execute( Node node, ExecutionState &state )
            {
                method_call_traits< _R >::call( state, _ptr, _mptr );
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"cpp-type-name" )
                {
                    TextUtf8 result_tn = TypeNameTraits< _R >::name();
                    TextUtf8 callee_tn = TypeNameTraits< _Cp >::name();
                    TextUtf8 method = result_tn+L" "+callee_tn+L" method()";
                    attr.str( method );
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    TextUtf8 result_tn = TypeNameTraits< _R >::name();
                    TextUtf8 method = L"function() -> "+ result_tn;
                    attr.str( method );
                    return true;
                }

                return false;
            }
        };

    template< class _R, class _Cp, class _Mp, class _A1 >
        struct MethodChunkA1 : virtual IChunk
        {
            TextUtf8 _a1;
            _Cp _ptr;
            _Mp _mptr;

            bool Execute( Node node, ExecutionState &state )
            {
                method_call_traits< _R >::call( state, _ptr, _mptr
                        , argument_traits< _A1 >::get( _a1, node ,state )
                        );
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"cpp-type-name" )
                {
                    TextUtf8 result_tn = TypeNameTraits< _R >::name();
                    TextUtf8 callee_tn = TypeNameTraits< _Cp >::name();
                    TextUtf8 arg1_tn = TypeNameTraits< _A1 >::name();
                    TextUtf8 method = result_tn+L" "+callee_tn+L" method( "+arg1_tn+L" )";
                    attr.str( method );
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    TextUtf8 arg1_tn = TypeNameTraits< _A1 >::name();
                    TextUtf8 result_tn = TypeNameTraits< _R >::name();
                    TextUtf8 method = L"function( "+ _a1 +L":"+ arg1_tn +L" ) -> "+ result_tn;
                    attr.str( method );
                    return true;
                }

                return false;
            }
        };

    template< class _R, class _Cp, class _Mp, class _A1, class _A2 >
        struct MethodChunkA2 : virtual IChunk
        {
            TextUtf8 _a1;
            TextUtf8 _a2;
            _Cp _ptr;
            _Mp _mptr;

            bool Execute( Node node, ExecutionState &state )
            {
                method_call_traits< _R >::call( state, _ptr, _mptr
                        , argument_traits< _A1 >::get( _a1, node, state )
                        , argument_traits< _A2 >::get( _a2, node, state )
                        );
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"cpp-type-name" )
                {
                    TextUtf8 result_tn = TypeNameTraits< _R >::name();
                    TextUtf8 callee_tn = TypeNameTraits< _Cp >::name();
                    TextUtf8 arg1_tn = TypeNameTraits< _A1 >::name();
                    TextUtf8 arg2_tn = TypeNameTraits< _A2 >::name();
                    TextUtf8 method = result_tn+L" "+callee_tn+L" method( "+arg1_tn+L", "+arg2_tn+L" )";
                    attr.str( method );
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    TextUtf8 arg1_tn = TypeNameTraits< _A1 >::name();
                    TextUtf8 arg2_tn = TypeNameTraits< _A2 >::name();
                    TextUtf8 result_tn = TypeNameTraits< _R >::name();
                    TextUtf8 method = L"function( "+ _a1 +L":"+ arg1_tn +L", "+ _a2 +L":"+ arg2_tn +L" ) -> "+ result_tn;
                    attr.str( method );
                    return true;
                }

                return false;
            }
        };

    template< class _R, class _Cp, class _Mp, class _A1, class _A2, class _A3 >
        struct MethodChunkA3 : virtual IChunk
        {
            TextUtf8 _a1;
            TextUtf8 _a2;
            TextUtf8 _a3;
            _Cp _ptr;
            _Mp _mptr;

            bool Execute( Node node, ExecutionState &state )
            {
                method_call_traits< _R >::call( state, _ptr, _mptr
                        , argument_traits< _A1 >::get( _a1, node, state )
                        , argument_traits< _A2 >::get( _a2, node, state )
                        , argument_traits< _A3 >::get( _a3, node, state )
                        );
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"cpp-type-name" )
                {
                    TextUtf8 result_tn = TypeNameTraits< _R >::name();
                    TextUtf8 callee_tn = TypeNameTraits< _Cp >::name();
                    TextUtf8 arg1_tn = TypeNameTraits< _A1 >::name();
                    TextUtf8 arg2_tn = TypeNameTraits< _A2 >::name();
                    TextUtf8 arg3_tn = TypeNameTraits< _A3 >::name();
                    TextUtf8 method = result_tn+L" "+callee_tn+L" method( "+arg1_tn+L", "+arg2_tn+L", "+arg3_tn+L" )";
                    attr.str( method );
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    TextUtf8 arg1_tn = TypeNameTraits< _A1 >::name();
                    TextUtf8 arg2_tn = TypeNameTraits< _A2 >::name();
                    TextUtf8 arg3_tn = TypeNameTraits< _A3 >::name();
                    TextUtf8 result_tn = TypeNameTraits< _R >::name();
                    TextUtf8 method = L"function( "+ _a1 +L":"+ arg1_tn +L", "+ _a2 +L":"+ arg2_tn +L", "+ _a3 +L":"+ arg3_tn + L" ) -> "+ result_tn;
                    attr.str( method );
                    return true;
                }

                return false;
            }
        };

    template< class _R, class _Cp, class _Mp, class _A1, class _A2, class _A3, class _A4 >
        struct MethodChunkA4 : virtual IChunk
        {
            TextUtf8 _a1;
            TextUtf8 _a2;
            TextUtf8 _a3;
            TextUtf8 _a4;
            _Cp _ptr;
            _Mp _mptr;

            bool Execute( Node node, ExecutionState &state )
            {
                method_call_traits< _R >::call( state, _ptr, _mptr
                        , argument_traits< _A1 >::get( _a1, node, state )
                        , argument_traits< _A2 >::get( _a2, node, state )
                        , argument_traits< _A3 >::get( _a3, node, state )
                        , argument_traits< _A4 >::get( _a4, node, state )
                        );
                return true;
            }
            
            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"cpp-type-name" )
                {
                    TextUtf8 result_tn = TypeNameTraits< _R >::name();
                    TextUtf8 callee_tn = TypeNameTraits< _Cp >::name();
                    TextUtf8 arg1_tn = TypeNameTraits< _A1 >::name();
                    TextUtf8 arg2_tn = TypeNameTraits< _A2 >::name();
                    TextUtf8 arg3_tn = TypeNameTraits< _A3 >::name();
                    TextUtf8 arg4_tn = TypeNameTraits< _A4 >::name();
                    TextUtf8 method = result_tn+L" "+callee_tn+L" method( "+arg1_tn+L", "+arg2_tn+L", "+arg3_tn+L" "+arg4_tn+L" )";
                    attr.str( method );
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    TextUtf8 arg1_tn = TypeNameTraits< _A1 >::name();
                    TextUtf8 arg2_tn = TypeNameTraits< _A2 >::name();
                    TextUtf8 arg3_tn = TypeNameTraits< _A3 >::name();
                    TextUtf8 arg4_tn = TypeNameTraits< _A4 >::name();
                    TextUtf8 result_tn = TypeNameTraits< _R >::name();
                    TextUtf8 method = L"function( "+ _a1 +L":"+ arg1_tn +L", "+ _a2 +L":"+ arg2_tn +L", "+ _a3 +L":"+ arg3_tn  +L", "+
                        _a4 +L":"+ arg4_tn+ L" ) -> "+ result_tn;
                    attr.str( method );
                    return true;
                }

                return false;
            }
        };

    
    
    template< class _R, class _Cp, class _C > IChunk *
        make_method_chunk( _Cp cp, _R (_C::*mp)( Node ) )
        {
            MethodChunk< _R, _Cp, _R (_C::*)( Node ) > *mc=
                new MethodChunk< _R, _Cp, _R (_C::*)( Node ) >();
            mc->_ptr = cp;
            mc->_mptr = mp;
            return mc;
        }
    
    template< class _R, class _Cp, class _C > IChunk *
        make_method_chunk( _Cp cp, _R (_C::*mp)( Node, ExecutionState & ) )
        {
            MethodChunk2< _R, _Cp, _R (_C::*)( Node, ExecutionState & ) > *mc=
                new MethodChunk2< _R, _Cp, _R (_C::*)( Node, ExecutionState & ) >();
            mc->_ptr = cp;
            mc->_mptr = mp;
            return mc;
        }

    template< class _R, class _Cp, class _C > IChunk *
        make_method_chunk( _Cp cp, _R (_C::*mp)() )
        {
            MethodChunkA0< _R, _Cp, _R (_C::*)() > *mc=
                new MethodChunkA0< _R, _Cp, _R (_C::*)() >();
            mc->_ptr = cp;
            mc->_mptr = mp;
            return mc;
        }

    template< class _R, class _Cp, class _C, class _A1 > IChunk *
        make_method_chunk( _Cp cp, _R (_C::*mp)( _A1 ), TextUtf8 a1 )
        {
            MethodChunkA1< _R, _Cp, _R (_C::*)( _A1 ), _A1 > *mc=
                new MethodChunkA1< _R, _Cp, _R (_C::*)( _A1 ), _A1 >();
            mc->_a1 = a1;
            mc->_ptr = cp;
            mc->_mptr = mp;
            return mc;
        }

    template< class _R, class _Cp, class _C, class _A1, class _A2 > IChunk *
        make_method_chunk( _Cp cp, _R (_C::*mp)( _A1, _A2 ), TextUtf8 a1, TextUtf8 a2 )
        {
            MethodChunkA2< _R, _Cp, _R (_C::*)( _A1, _A2 ), _A1, _A2 > *mc=
                new MethodChunkA2< _R, _Cp, _R (_C::*)( _A1, _A2 ), _A1, _A2 >();
            mc->_a1 = a1;
            mc->_a2 = a2;
            mc->_ptr = cp;
            mc->_mptr = mp;
            return mc;
        }

    template< class _R, class _Cp, class _C, class _A1, class _A2, class _A3 > IChunk *
        make_method_chunk( _Cp cp, _R (_C::*mp)( _A1, _A2, _A3 ), TextUtf8 a1, TextUtf8 a2, TextUtf8 a3 )
        {
            MethodChunkA3< _R, _Cp, _R (_C::*)( _A1, _A2, _A3 ), _A1, _A2, _A3 > *mc=
                new MethodChunkA3< _R, _Cp, _R (_C::*)( _A1, _A2, _A3 ), _A1, _A2, _A3 >();
            mc->_a1 = a1;
            mc->_a2 = a2;
            mc->_a3 = a3;
            mc->_ptr = cp;
            mc->_mptr = mp;
            return mc;
        }
    
    template< class _R, class _Cp, class _C, class _A1, class _A2, class _A3, class _A4 > IChunk *
        make_method_chunk( _Cp cp, _R (_C::*mp)( _A1, _A2, _A3, _A4 ), TextUtf8 a1, TextUtf8 a2, TextUtf8 a3, TextUtf8 a4 )
        {
            MethodChunkA4< _R, _Cp, _R (_C::*)( _A1, _A2, _A3, _A4 ), _A1, _A2, _A3, _A4 > *mc=
                new MethodChunkA4< _R, _Cp, _R (_C::*)( _A1, _A2, _A3, _A4 ), _A1, _A2, _A3, _A4 >();
            mc->_a1 = a1;
            mc->_a2 = a2;
            mc->_a3 = a3;
            mc->_a4 = a4;
            mc->_ptr = cp;
            mc->_mptr = mp;
            return mc;
        }
    


    template< class _R, class _Cp, class _Mp, class _R1 >
    struct method_binder_A0
    {
        _Cp _cp;
        _Mp _mp;

        _R1 apply()
        {
            return ((*_cp).*_mp)();
        }

        method_binder_A0( _Cp cp, _Mp mp ): _cp(cp), _mp(mp) {}
    };

    template< class _R, class _Cp, class _Mp, class _A1, class _R1 >
    struct method_binder_A1
    {
        _Cp _cp;
        _Mp _mp;

        _R1 apply( _A1 a1 )
        {
            return ((*_cp).*_mp)( a1 );
        }

        method_binder_A1( _Cp cp, _Mp mp ): _cp(cp), _mp(mp) {}
    };

    template< class _R, class _Cp, class _Mp, class _A1, class _A2, class _R1 >
    struct method_binder_A2
    {
        _Cp _cp;
        _Mp _mp;

        _R1 apply( _A1 a1, _A2 a2 )
        {
            return ((*_cp).*_mp)( a1, a2 );
        }

        method_binder_A2( _Cp cp, _Mp mp ): _cp(cp), _mp(mp) {}
    };

    template< class _R, class _Cp, class _Mp, class _A1, class _A2, class _A3, class _R1 >
    struct method_binder_A3
    {
        _Cp _cp;
        _Mp _mp;

        _R1 apply( _A1 a1, _A2 a2, _A3 a3 )
        {
            return ((*_cp).*_mp)( a1, a2, a3 );
        }

        method_binder_A3( _Cp cp, _Mp mp ): _cp(cp), _mp(mp) {}
    };

    template< class _R, class _Cp, class _Mp, class _A1, class _A2, class _A3, class _A4, class _R1 >
    struct method_binder_A4
    {
        _Cp _cp;
        _Mp _mp;

        _R1 apply( _A1 a1, _A2 a2, _A3 a3, _A4 a4 )
        {
            return ((*_cp).*_mp)( a1, a2, a3, a4 );
        }

        method_binder_A4( _Cp cp, _Mp mp ): _cp(cp), _mp(mp) {}
    };


    template< class _Type >
        struct method_adder
        {
            _Type *_ptr;
            IBlock *_bPtr;

            method_adder( _Type *ptr ) : _ptr( ptr ), _bPtr( ptr )
			{
			}
            
			method_adder( _Type *ptr, IBlock *bPtr ) : _ptr( ptr ), _bPtr( bPtr )
			{
			}

            method_adder & add( const TextAnsi &name, IChunk *c )
            {
                _bPtr->AddChunk( name, c );
                return (*this);
            }

            template< class _C >
                method_adder & add_value( const TextAnsi &name, const _C &c )
                {
                    _bPtr->AddChunk( name, make_value_chunk( c ));
                    return (*this);
                }

            template< class _R, class _C >
                method_adder & def( const TextAnsi & name, _R (_C::*mp)( Node ) )
                {
                    _bPtr->AddChunk( name, make_method_chunk( _ptr, mp ));
                    return (*this);
                }

            template< class _R, class _C >
                method_adder & def( const TextAnsi & name, _R (_C::*mp)( Node, ExecutionState & ) )
                {
                    _bPtr->AddChunk( name, make_method_chunk( _ptr, mp ));
                    return (*this);
                }

            template< class _R, class _C >
                method_adder & def( const TextAnsi & name, _R (_C::*mp)() )
                {
                    _bPtr->AddChunk( name, make_method_chunk( _ptr, mp ));
                    return (*this);
                }

            template< class _R, class _C, class _A1 >
                method_adder & def( const TextAnsi & name, _R (_C::*mp)( _A1 ), const TextAnsi & a1 )
                {
                    _bPtr->AddChunk( name, make_method_chunk( _ptr, mp, a1 ));
                    return (*this);
                }

            template< class _R, class _C, class _A1, class _A2 >
                method_adder & def( const TextAnsi & name, _R (_C::*mp)( _A1, _A2 ), const TextAnsi & a1, const TextAnsi & a2 )
                {
                    _bPtr->AddChunk( name, make_method_chunk( _ptr, mp, a1, a2 ));
                    return (*this);
                }

            template< class _R, class _C, class _A1, class _A2, class _A3 >
                method_adder & def( const TextAnsi & name, _R (_C::*mp)( _A1, _A2, _A3 )
                        , const TextAnsi & a1, const TextAnsi & a2, const TextAnsi & a3 )
                {
                    _bPtr->AddChunk( name, make_method_chunk( _ptr, mp, a1, a2, a3 ));
                    return (*this);
                }
            
            template< class _R, class _C, class _A1, class _A2, class _A3, class _A4 >
                method_adder & def( const TextAnsi & name, _R (_C::*mp)( _A1, _A2, _A3, _A4 )
                        , const TextAnsi & a1, const TextAnsi & a2, const TextAnsi & a3, const TextAnsi &a4 )
                {
                    _bPtr->AddChunk( name, make_method_chunk( _ptr, mp, a1, a2, a3, a4 ));
                    return (*this);
                }

            // additional helpers allowing result & arguments remaping
            template< class _R1,
                      class _R, class _C >
                method_adder & def_remap_0( const TextAnsi & name, _R (_C::*mp)() )
                {
                    typedef method_binder_A0< _R, _Type *, _R (_C::*)(), _R1 > binder_t;
                    _bPtr->AddChunk( name, make_method_chunk( boost::shared_ptr< binder_t >( new binder_t( _ptr, mp)), &binder_t::apply ));
                    return (*this);
                }

            template< class _R1, class _B1, class _R,
                      class _C, class _A1 >
                method_adder & def_remap_1( const TextAnsi & name, _R (_C::*mp)( _A1 ), const TextAnsi & a1 )
                {
                    typedef method_binder_A1< _R, _Type *, _R (_C::*)( _A1 ), _B1, _R1 > binder_t;
                    _bPtr->AddChunk( name, make_method_chunk( boost::shared_ptr< binder_t >( new binder_t( _ptr, mp)), &binder_t::apply, a1 ));
                    return (*this);
                }
            
            template< class _R1, class _B1, class _B2,
                      class _R, class _C, class _A1, class _A2 >
                method_adder & def_remap_2( const TextAnsi & name, _R (_C::*mp)( _A1, _A2 ), const TextAnsi & a1, const TextAnsi & a2 )
                {
                    typedef method_binder_A2< _R, _Type *, _R (_C::*)( _A1, _A2 ), _B1, _B2, _R1 > binder_t;
                    _bPtr->AddChunk( name, make_method_chunk( boost::shared_ptr< binder_t >( new binder_t( _ptr, mp)), &binder_t::apply, a1, a2 ));
                    return (*this);
                }
            
            template< class _R1, class _B1, class _B2, class _B3,
                      class _R, class _C, class _A1, class _A2, class _A3 >
                method_adder & def_remap_3( const TextAnsi & name, _R (_C::*mp)( _A1, _A2, _A3 )
                        , const TextAnsi & a1, const TextAnsi & a2, const TextAnsi &a3 )
                {
                    typedef method_binder_A3< _R, _Type *, _R (_C::*)( _A1, _A2, _A3 ), _B1, _B2, _B3, _R1 > binder_t;
                    _bPtr->AddChunk( name, make_method_chunk( boost::shared_ptr< binder_t >( new binder_t( _ptr, mp)), &binder_t::apply, a1, a2, a3 ));
                    return (*this);
                }
            
            template< class _R1, class _B1, class _B2, class _B3, class _B4,
                      class _R, class _C, class _A1, class _A2, class _A3, class _A4 >
                method_adder & def_remap_4( const TextAnsi & name, _R (_C::*mp)( _A1, _A2, _A3, _A4 )
                        , const TextAnsi & a1, const TextAnsi & a2, const TextAnsi &a3, const TextAnsi &a4 )
                {
                    typedef method_binder_A4< _R, _Type *, _R (_C::*)( _A1, _A2, _A3, _A4 ), _B1, _B2, _B3, _B4, _R1 > binder_t;
                    _bPtr->AddChunk( name, make_method_chunk( boost::shared_ptr< binder_t >( new binder_t( _ptr, mp)), &binder_t::apply, a1, a2, a3, a4 ));
                    return (*this);
                }
        };

    template< class _Type > method_adder< _Type > add_methods( _Type *object )
    {
        return method_adder< _Type >( object );
    }
    
    template< class _Type > method_adder< _Type > add_methods( _Type *object, IBlock *block )
    {
        return method_adder< _Type >( object, block );
    }
    
    template< class _Type > method_adder< _Type > add_methods_block( const TextAnsi & name, _Type *object, IBlock *parentBlock )
    {
        IBlockPtr block( new XMLCodeBlock() );
        parentBlock->AddChunk( name, block.get() );
        return method_adder< _Type >( object, block.get() );
    }

};//XMLProgram


#endif//__INCLUDED_XML_METHOD_CHUNKS_H__
