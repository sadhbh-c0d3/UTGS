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
  last mod: $Id: XMLChunksProgram.h,v 1.19 2005/03/18 17:48:36 koolas Exp $

 ********************************************************************/


#ifndef __INCLUDED_XML_PROGRAM_CHUNKS_H__
#define __INCLUDED_XML_PROGRAM_CHUNKS_H__

#include "koolib/xml/XMLProgram.h"
#include "koolib/xml/XMLFactoryUtil.h"
#include "koolib/SymbolDict.h"

#include "Useless/XML/Resources/CreateFromXML.h"
#include "Useless/Functional/Signal.h"

#include <deque>

#include <boost/intrusive_ptr.hpp>


// Register xml.fsm in Useless::Resources
static Useless::RegisterXMLCreator reg_create_programm_fsm(
        "programms::fsm", Useless::CreateXMLDocument );

KOOLIB_API Useless::Resource * CreateRunFSM( Useless::XMLIterator i, Useless::XMLCreatorEnv *env );
static Useless::RegisterXMLCreator reg_create_run_fsm(
        "programms::run", CreateRunFSM );

typedef Useless::ResourceQuery< boost::intrusive_ptr< XMLProgram::IFiniteStateMachine > > FSMResource;


// Publish xml.fsm deserialization
XMLProgram::IFiniteStateMachine &
operator >> ( XMLFactory::Node node, XMLProgram::IFiniteStateMachine &fsm );

namespace boost {
    // Allow internal reference counting via boost::intrusive_ptr
    inline void intrusive_ptr_add_ref( XMLProgram::IChunk *chunk ) { chunk->AddRef(); }
    inline void intrusive_ptr_release( XMLProgram::IChunk *chunk ) { chunk->RemRef(); }
};//boost

namespace XMLProgram {
    
    typedef boost::intrusive_ptr< IChunk > IChunkPtr;
    typedef boost::intrusive_ptr< IBlock > IBlockPtr;

    // Declare tag dictionaries
    USELESS_XML_TAG_DICT_DECLARE( xtdProgram, ExecutionState );
    USELESS_XML_TAG_DICT_DECLARE( xtdFSM_Main, ExecutionState );
    USELESS_XML_TAG_DICT_DECLARE( xtdFSM_State, ExecutionState );
    USELESS_XML_TAG_DICT_DECLARE( xtdImport, ExecutionState );
   


    // We are using XML stuff from Useless and XMLFactory
    using XMLFactory::Node;
    using XMLFactory::Attr;
    using XMLFactory::AttrBase;
    using XMLFactory::AttrBaseType;
    using XMLFactory::AttrProxy;
    using XMLFactory::AttrUniBase;
    using XMLFactory::AttrUniProxy;
    using Useless::XMLDocument;
    using Useless::Signal;

    struct KOOLIB_API FSMStackGuard
    {        
        IFiniteStateMachine *_fsm;
        ExecutionState      *_state;

        FSMStackGuard( IFiniteStateMachine *fsm ): _fsm( fsm ), _state( 0 )
        {
            _state = &_fsm->DuplicateStack();
        }

        ~FSMStackGuard()
        {
            _fsm->ReleaseDuplicate();            
        }

        ExecutionState & GetState()
        {
            return *_state;
        }

        ExecutionState NextState()
        {
            ExecutionState state( *_state );
            state._prevState = _state;
            return state;
        }
        
        ExecutionState NextState( IBlock *pBlock )
        {
            ExecutionState state( *_state );
            state._prevState = _state;
            state._currentBlock = pBlock;
            return state;
        }
    };

    template< class _Type >
        struct TypeNameTraits
        {
            static const char * name() { return typeid( _Type ).name(); }
            static unsigned int fcc() { return 0L; }
        };

    KOOLIB_SPECIALIZATION struct TypeNameTraits< std::string > 
        {
            static const char * name() { return "string"; }
            static unsigned int fcc() { return FOURCC_TYPE_STRING; }
        };
    
    KOOLIB_SPECIALIZATION struct TypeNameTraits< TextUtf8 > 
        {
            static const char * name() { return "unicode-string"; }
            static unsigned int fcc() { return FOURCC_TYPE_TEXT; }
        };
    
    
    KOOLIB_SPECIALIZATION struct TypeNameTraits< int > 
        {
            static const char * name() { return "int"; }
            static unsigned int fcc() { return FOURCC_TYPE_INT; }
        };
    
    KOOLIB_SPECIALIZATION struct TypeNameTraits< double > 
        {
            static const char * name() { return "float"; }
            static unsigned int fcc() { return FOURCC_TYPE_FLOAT; }
        };
    
    KOOLIB_SPECIALIZATION struct TypeNameTraits< IChunkPtr > 
        {
            static const char * name() { return "IChunkPtr"; }
            static unsigned int fcc() { return 0L; }
        };
    
    KOOLIB_SPECIALIZATION struct TypeNameTraits< IBlockPtr > 
        {
            static const char * name() { return "IBlockPtr"; }
            static unsigned int fcc() { return 0L; }
        };

    template< class _Type > struct AttrTraits
    {
        typedef _Type Type;
        typedef Type WideType;

        template< class _CharType >
            static void set_value( AttrBaseType< Type, _CharType > &attr, const Type &value )
            {
                attr = value;
            }
        
        template< class _CharType >
            static void set_value( AttrBaseType< Type, _CharType > &attr, const TextUtf8 &value )
            {
                attr.str( value );
            }
        
        template< class _CharType >
            static void set_value( AttrBaseType< Type, _CharType > &attr, const std::string &value )
            {
                attr.str( TextAnsi( value ));
            }

        template< class _CharType >
            static Type get_value( const AttrBaseType< Type, _CharType > &attr )
            {
                return attr._current;
            }
    
            static void get( TextUtf8::Stream &ss, const Type &value )
            {
                ss << value;
            }

            static void put( TextUtf8::Stream &ss, Type &out )
            {
                ss >> out;
            }
    };

    template<> struct AttrTraits< TextUtf8 >
    {
        typedef TextUtf8 Type;
        typedef Type WideType;

        template< class _CharType >
            static void set_value( AttrBaseType< Type, _CharType > &attr, const Type &value )
            {
                attr.str( value );
            }

        template< class _CharType >
            static Type get_value( const AttrBaseType< Type, _CharType > &attr )
            {
                return attr.str();
            }
    
            static void get( TextUtf8::Stream &ss, const Type &value )
            {
                ss.str( value );
            }

            static void put( TextUtf8::Stream &ss, Type &out )
            {
                out = Type( ss.str() );
            }
    };
    
    template<> struct AttrTraits< TextAnsi >
    {
        typedef TextAnsi Type;
        typedef Type WideType;

        template< class _CharType >
            static void set_value( AttrBaseType< Type, _CharType > &attr, const Type &value )
            {
                attr.str( value );
            }

        template< class _CharType >
            static Type get_value( const AttrBaseType< Type, _CharType > &attr )
            {
                return attr.str();
            }
    
            static void get( TextUtf8::Stream &ss, const Type &value )
            {
                ss.str( value );
            }

            static void put( TextUtf8::Stream &ss, Type &out )
            {
                out = ss.str();
            }
    };
    
    template<> struct AttrTraits< std::string >
    {
        typedef std::string Type;
        typedef TextAnsi WideType;

        template< class _CharType >
            static void set_value( AttrBaseType< Type, _CharType > &attr, const Type &value )
            {
                attr.str( value );
            }

        template< class _CharType >
            static Type get_value( const AttrBaseType< Type, _CharType > &attr )
            {
                return attr.str();
            }
    
            static void get( TextUtf8::Stream &ss, const Type &value )
            {
                ss.str( TextAnsi( value ));
            }

            static void put( TextUtf8::Stream &ss, Type &out )
            {
                out = TextAnsi( ss.str() ).as_string();
            }
    };
    
    template< class _Type, class _CharType >
        _Type get_value( AttrBaseType< _Type, _CharType > &attr )
        {
            return AttrTraits< _Type >::get_value( attr );
        }
    
    template< class _Type, class _CharType, class _Type2 >
        void assign_value( AttrBaseType< _Type, _CharType > &attr, const _Type2 &value )
        {
            AttrTraits< _Type >::set_value( attr, value );
        }

    template< class _Type >
        void put( TextUtf8::Stream &ss, _Type &value )
        {
            AttrTraits< _Type >::put( ss, value );
        }
    
    template< class _Type >
        void get( TextUtf8::Stream &ss, const _Type &value )
        {
            AttrTraits< _Type >::get( ss, value );
        }

    inline void assign( TextUtf8 &a, const TextUtf8 &b ) { a = b; }
    inline void assign( TextUtf8 &a, double b ) { TextUtf8::Stream ss; ss << b; a = TextUtf8( ss.str() ); }
    inline void assign( TextUtf8 &a, int b ) { TextUtf8::Stream ss; ss << b; a = TextUtf8( ss.str() ); }
    inline void assign( double &a, const TextUtf8 &b ) { TextUtf8::Stream ss(b); ss >> a; }
    inline void assign( int &a, const TextUtf8 &b ) { TextUtf8::Stream ss(b); ss >> a; }
    inline void assign( std::string &a, const std::string &b ) { a = b; }
    inline void assign( std::string &a, double b ) { std::stringstream ss; ss << b; a = ss.str(); }
    inline void assign( std::string &a, int b ) { std::stringstream ss; ss << b; a = ss.str(); }
    inline void assign( double &a, const std::string &b ) { std::stringstream ss(b); ss >> a; }
    inline void assign( int &a, const std::string &b ) { std::stringstream ss(b); ss >> a; }
    inline void assign( double &a, double b ) { a = b; }
    inline void assign( double &a, int b ) { a = b; }
    inline void assign( int &a, double b ) { a = (int)b; }
    inline void assign( int &a, int b ) { a = b; }

    template< class _Type > struct ChunkValueTraits
    {
        static _Type value_of( IChunk *pChunk )
        {
            Attr< _Type, true, wchar_t > a(L"value");
            (*pChunk) >> a;
            return get_value( a );
        }
    };

    template<> struct ChunkValueTraits< TextUtf8 >
    {
        static TextUtf8 value_of( IChunk *pChunk )
        {
            TextUtf8 y;
            (*pChunk) >> y;
            return y;
        }
    };

    template<> struct ChunkValueTraits< int >
    {
        static int value_of( IChunk *pChunk )
        {
            int y;
            (*pChunk) >> y;
            return y;
        }
    };

    template<> struct ChunkValueTraits< double >
    {
        static double value_of( IChunk *pChunk )
        {
            double y;
            (*pChunk) >> y;
            return y;
        }
    };

    template<> struct ChunkValueTraits< bool >
    {
        static bool value_of( IChunk *pChunk )
        {
            int y;
            (*pChunk) >> y;
            return y;
        }
    };

    template< class _T > _T value_of( IChunk *pChunk )
    {
        return ChunkValueTraits< _T >::value_of( pChunk );
    }

    template< class _T > _T value_of( IChunkPtr pChunk )
    {
        return ChunkValueTraits< _T >::value_of( pChunk.get() );
    }

    inline TextUtf8 type_of( IChunk *pChunk )
    {
        Attr< TextUtf8, true, wchar_t > a(L"type-name");
        (*pChunk) >> a;
        return TextUtf8( a.str() );
    }
    
    template< class _T > _T attribute_of( IChunk *pChunk, const TextAnsi &name )
    {
        Attr< _T, true, wchar_t > a( name );
        (*pChunk) >> a;
        return get_value( a );
    }
   
    // Get globals
    KOOLIB_API ExecutionState & GetGlobals();
    KOOLIB_API void GlobalsAddRef();
    KOOLIB_API void GlobalsRemRef();

    // Global symbol dictionary
    KOOLIB_API SymbolDict & GetGlobalSymbolDict();
    
    // Convert string to list
    KOOLIB_API IChunk *    CreateSequence( const TextUtf8 &text );

    
    /*! Chunk keeping one value
     */
    template< class _Type >
        struct KOOLIB_API XMLValueChunk : IChunk
        {
            _Type _value;

            XMLValueChunk() {}
            XMLValueChunk( const _Type &value ): _value( value ) {}

            void SetValue( const _Type &value ) { _value = value; }

            unsigned int GetFourCC( unsigned int name )
            {
                if ( FOURCC_NAME_TYPE == name )
                {
                    return TypeNameTraits< _Type >::fcc();
                }
                else
                {
                    return 0L;
                }
            }

            bool operator >> ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"as-string" || attr._name == L"value" )
                {
                    TextUtf8::Stream ss;
                    get( ss, _value );
                    attr.str( ss.str() );
                    return true;
                }
                else if ( attr._name == L"type-name" )
                {
                    attr.str( WidenText( TypeNameTraits< _Type >::name()));
                    return true;
                }
                else if ( attr._name == L"defined-symbols" )
                {
                    attr.str( L"value type-name" );
                    return true;
                }
                return false;
            }

            bool operator << ( XMLFactory::AttrUniBase &attr )
            {
                if ( attr._name == L"as-string" || attr._name == L"value" )
                {
                    TextUtf8::Stream ss( attr.str() );
                    put( ss, _value );
                    return true;
                }
                return false;
            }
        
            bool operator >> ( XMLFactory::AttrBaseType< int, wchar_t > &attr )
            {
                if ( attr._name == L"value" )
                {
                    assign_value( attr, _value );
                    return true;
                }
                return false;
            }

            bool operator >> ( XMLFactory::AttrBaseType< double, wchar_t > &attr )
            {
                if ( attr._name == L"value" )
                {
                    assign_value( attr, _value );
                    return true;
                }
                return false;
            }

            virtual bool operator >> ( TextUtf8 &value )
            {
                assign( value, _value );
                return true;
            }

            virtual bool operator >> ( int &value )
            {
                assign( value, _value );
                return true;
            }

            virtual bool operator >> ( double &value )
            {
                assign( value, _value );
                return true;
            }

            virtual IChunk * GetChunk( const TextUtf8 &name )
            {
                if ( name == L"__sequence__" )
                {
                    TextUtf8::Stream ss;
                    get( ss, _value );
                    return CreateSequence( ss.str() );
                }
                else
                {
                    return 0;
                }
            }
        };

    template< class _Type >
        IChunk * make_value_chunk( const _Type &value )
        {
            return new XMLValueChunk< _Type >( value );
        }
    

    /*! Chunk of XML code
     */
    struct KOOLIB_API XMLCodeChunk : virtual IChunk
    {
        XMLDocument _subDoc;

        XMLCodeChunk()
        {
        }

        XMLCodeChunk( Node node )
            : _subDoc( node.SubTree())
        {
        }

        ~XMLCodeChunk()
        {
        }

        bool Execute( Node node, ExecutionState &state )
        {        
            state.ExecuteChildren( _subDoc.Root() );
            return true;
        }
        
        bool operator >> ( XMLFactory::AttrUniBase &attr )
        {
            if ( attr._name == L"type-name" )
            {
                attr.str(L"xml-code");
                return true;
            }
            else if ( attr._name == L"defined-symbols" )
            {
                attr.str( L"type-name" );
                return true;
            }
            return false;
        }
    };

    /*! Chunk of signal scope
     */
    struct KOOLIB_API SignalFIDChunk : virtual IChunk
    {
        Signal::FuncID _fid;

        SignalFIDChunk()
        {
        }

        SignalFIDChunk( Signal::FuncID fid ): _fid( fid )
        {
        }

        ~SignalFIDChunk()
        {
            _fid.Untie();
        }
        
        bool operator >> ( XMLFactory::AttrUniBase &attr )
        {
            if ( attr._name == L"type-name" )
            {
                attr.str(L"slot-id");
                return true;
            }
            else if ( attr._name == L"defined-symbols" )
            {
                attr.str( L"type-name" );
                return true;
            }
            return false;
        }
    };

    /*! Chunk of XML code connected to signal
     */
    struct KOOLIB_API XMLSignalSlot : SignalFIDChunk, XMLCodeChunk
    {
        IFiniteStateMachine *_fsm;
        IBlockPtr            _block;

        XMLSignalSlot( Signal &signal, Node node, ExecutionState &state, IBlock *pBlock = 0 )
            : XMLCodeChunk( node )
            {
                _fsm = state._currentFSM;
				_fid = signal.TieVoid( this, &XMLProgram::XMLSignalSlot::_Execute );
                _block = pBlock;
            }

        ~XMLSignalSlot()
        {
        }

        XMLSignalSlot * SetBlock( IBlock *pBlock )
        {
            _block = pBlock;
            return this;
        }

        void _Execute()
        {
            FSMStackGuard guard( _fsm );

            if ( _block.get() )
            {
                guard.NextState( _block.get() ).ExecuteChildren( _subDoc.Root() );
            }
            else
            {
                guard.GetState().ExecuteChildren( _subDoc.Root() );
            }
        }
        
        bool Execute( Node node, ExecutionState &state )
        {
            _Execute();
            return true;
        }
        
        bool operator >> ( XMLFactory::AttrUniBase &attr )
        {
            if ( attr._name == L"type-name" )
            {
                attr.str(L"slot");
                return true;
            }
            else if ( attr._name == L"defined-symbols" )
            {
                attr.str( L"type-name" );
                return true;
            }
            return false;
        }
    };


    /*! Block of chunks
     */
    struct KOOLIB_API XMLCodeBlock : virtual IBlock, XMLCodeChunk
    {
        typedef std::multimap< unsigned int, IChunkPtr > ChunkMap;
        ChunkMap _chunks;

        ~XMLCodeBlock();

        void AddChunk( unsigned int name, IChunk *chunk );
        void AddChunk( const TextUtf8 &name, IChunk *chunk );

        IChunk* GetChunk( unsigned int name );
        IChunk* GetChunk( const TextUtf8 &name );
        
        IChunk* _getChunk( unsigned int name );
        IChunk* _getChunk( const TextUtf8 &name );
        
        void SetChunk( unsigned int name, IChunk *pChunk );
        void SetChunk( const TextUtf8 &name, IChunk *pChunk );

        void DropAll();

        bool operator >> ( XMLFactory::AttrUniBase &attr );

        unsigned int GetFourCC( unsigned int name )
        {
            if ( FOURCC_NAME_TYPE == name )
            {
                return FOURCC_TYPE_BLOCK;
            }
            else
            {
                return 0L;
            }
        }
    };

    /*! Finite state machine from XML
     */
    struct KOOLIB_API XMLFiniteStateMachine : virtual IFiniteStateMachine, XMLCodeBlock
    {
        struct State : ExecutionState
        {
            IBlockPtr _block;
            TextUtf8 _name;
            State(): _block( new XMLCodeBlock ) {}
        };

        std::deque< State > _stateStack;
        std::deque< std::deque< State > > _stateStackDuplicate;

        typedef std::map< TextUtf8, XMLDocument > StateDict;
        StateDict _states;
        ExecutionState _baseState;

        XMLFiniteStateMachine();
        ~XMLFiniteStateMachine();
        
        ExecutionState & EnterState ( const TextUtf8 &name );
        ExecutionState & GotoState  ( const TextUtf8 &name );
        ExecutionState & LeaveState ();

        ExecutionState & GetCurrentExecutionState()
        {
            if (! _stateStack.empty() )
            {
                return _stateStack.back();
            }
            else
            {
                return _baseState;
            }
        }

        ExecutionState & DuplicateStack  ();
        void             ReleaseDuplicate();
        void             ExecuteString   ( const std::string &s );

        TextUtf8 GetCurrentState() const
        {
            if (! _stateStack.empty() )
            {
                return _stateStack.back()._name;
            }
            else
            {
                return L"__fsm__";
            }
        }

        void AddState( const TextUtf8 &name, XMLFactory::Node node )
        {
            _states[ name ] = node.SubTree();
        }
        
        bool operator >> ( XMLFactory::AttrUniBase &attr )
        {
            if ( attr._name == L"type-name" )
            {
                attr.str(L"xml-fsm");
                return true;
            }
            else if ( attr._name == L"current-state" )
            {
                attr.str( GetCurrentState() );
                return true;
            }
            else if ( attr._name == L"stack-depth" )
            {
                TextUtf8::Stream ss;
                ss << _stateStack.size();
                attr.str( ss.str() );
                return true;
            }
            else if ( attr._name == L"states-on-stack" )
            {
                TextUtf8 s(L"__fsm__");
                for ( std::deque< State >::iterator it = _stateStack.begin();
                        it != _stateStack.end(); ++it )
                {
                    s += L'<';
                    s += (*it)._name;
                }
                attr.str( s );
                return true;
            }
            else if ( attr._name == L"defined-symbols" )
            {
                XMLCodeBlock::operator >> ( attr );
                attr.str( L"__fsm__" + attr.str() );
                return true;
            }

            return false;
        }
        
        IChunk* GetChunk( const TextUtf8 &name )
        {
            if ( name == L"__fsm__" )
            {
                return this;
            }
            else if ( name == L"__globals__" )
            {
                return GetGlobals()._currentBlock;
            }
            else
            {
                IChunk *pChunk = XMLCodeBlock::GetChunk( name );
                if ( 0 != pChunk )
                {
                    return pChunk;
                }
                for ( std::deque< State >::reverse_iterator it = _stateStack.rbegin();
                        it != _stateStack.rend(); ++it )
                {
                    if ( (*it)._name == name )
                    {
                        return (*it)._currentBlock;
                    }
                }
                return 0;
            }
        }
    };

    // EvaluateObject
    KOOLIB_API IChunkPtr EvaluateObject( Node &node, ExecutionState &state, int flags=0 );

    enum EvobFlags
    {
        EVOB_SEPARATE_BLOCK = 1,
        EVOB_COPY_ATTRIBUTES = 2,
        EVOB_RETURN_BLOCK = 4,     //< By default 
        EVOB_NO_CODE_EXECUTION = 8, //< USE WITH CAUTION (parameter 'state' is OVERWRITTEN)
        EVOB_NUMERIC = 16,
        EVOB_FLOAT = 32,
        EVOB_NO_STRING_EVALUATION = 64
    };

    // Check if 'pChunk' is 'NULL' or points to 'XMLEmpty'
    KOOLIB_API bool      IsEmpty( IChunk *pChunk );

    // Evaluate 'expression' in Reverse Polish Notation
    /*@{*/
    KOOLIB_API bool      EvaluateBoolean   ( TextUtf8 expression );
    KOOLIB_API int       EvaluateInteger   ( TextUtf8 expression );
    KOOLIB_API double    EvaluateFloat     ( TextUtf8 expression );
    /*@}*/

    KOOLIB_API IChunkPtr EvaluateExpression( TextUtf8 expression, ExecutionState &state );

    KOOLIB_API TextUtf8  EvaluateString    ( TextUtf8 value, ExecutionState &state );
    KOOLIB_API TextUtf8  EvaluateString    ( TextUtf8 value, ExecutionState &state, const TextUtf8 &tag );
    KOOLIB_API int       EvaluateString    ( TextUtf8 value, ExecutionState &state, int tag );
    KOOLIB_API double    EvaluateString    ( TextUtf8 value, ExecutionState &state, double tag );

    template< class _Other >
        TextUtf8  EvaluateString    ( TextUtf8 value, ExecutionState &state, const _Other &tag )
        {
            return EvaluateString( value, state );
        }

    KOOLIB_API TextUtf8  EvaluateTextUtf8  ( Node node, ExecutionState &state );
    KOOLIB_API int       EvaluateInteger   ( Node node, ExecutionState &state );
    KOOLIB_API double    EvaluateFloat     ( Node node, ExecutionState &state );

    KOOLIB_API IChunk* GetChunk( TextUtf8 chunkName, IChunk *pChunk, ExecutionState &state, IChunk **pParent = 0 );
    KOOLIB_API IChunk* GetChunk( TextUtf8 chunkName, ExecutionState &state, IChunk **pParent = 0, ExecutionState **pState = 0  );
    
    typedef std::pair< TextUtf8, TextUtf8 > SplitNames;
    /*! Split "name:tail" => ( name, tail )
     */
    KOOLIB_API SplitNames SplitChunkName( TextUtf8 fullName, ExecutionState &state );

    /*! Get value of symbol in given execution scope
     */
    template< class _Type, const int _Req, class _CharType >
        bool GetNodeAttr( Attr< _Type, _Req, _CharType > &attr, const Node &node, ExecutionState &state  )
        {
            if ( !!node )
            {
                Attr< std::string, false > query( TextAnsi( attr._name ) );
                if ( node >> query )
                {
                    assign_value( attr, EvaluateString( query.str(), state, _Type() ));
                    return true;
                }
            }
            return false;
        }

    template< class _Type, const int _Req, class _CharType >
        bool GetAttr( Attr< _Type, _Req, _CharType > &attr, const Node &node, ExecutionState &state )
        {
            if ( GetNodeAttr( attr, node, state ))
            {
                return true;
            }
        
            if ( GetChunkAttr( attr, state ))
            {
                return true;
            }

            if ( _Req )
            {
                Attr< std::string, false > query( TextAnsi( attr._name ) );
                node.RaiseError( query );
            }

            return false;
        }
    
    /*! Get value of "chunkName:value" or "chunkName:attrName"
     */
    template< class _Type,  class _CharType >
        bool GetChunkAttr( AttrBaseType< _Type, _CharType > &attr, ExecutionState &state )
        {
            IChunk *pParent = 0;
            IChunk *pChunk = GetChunk( attr._name, state, &pParent );
            if ( 0 != pChunk )
            {
                assign_value( attr, value_of< AttrTraits< _Type >::WideType >( pChunk ));
                return true;
            }
            else if ( 0 != pParent )
            {
                int where = attr._name.rfind(L':');
                if ( -1 != where )
                {
                    TextAnsi subName = attr._name.substr( where+1, -1 );
                    AttrBaseType< AttrTraits< _Type >::WideType, wchar_t > proxy( subName );
                    if ( (*pParent) >> proxy )
                    {
                        assign_value( attr, get_value( proxy ));
                        return true;
                    }                    
                }
            }
            return false;
        }
    
    template< class _Type, const int _Req, class _CharType >
        bool GetMultiAttr( Attr< _Type, _Req, _CharType > &attr, const Node &node, ExecutionState &state )
        {
            Attr< TextAnsi, _Req, wchar_t > proxy( TextAnsi( attr._name ));
            if ( GetAttr( proxy, node, state ))
            {
                attr.str( proxy.str() );
                return true;
            }
            return false;
        }

    
    
    /*! Empty
     */
    struct KOOLIB_API XMLEmpty : virtual IChunk
    {
        bool operator >> ( XMLFactory::AttrUniBase &attr )
        {
            if ( attr._name == L"type-name" )
            {
                attr.str(L"empty");
                return true;
            }
            return false;
        }

        unsigned int GetFourCC( unsigned int name )
        {
            if ( FOURCC_NAME_TYPE == name )
            {
                return FOURCC_TYPE_EMPTY;
            }
            else
            {
                return 0L;
            }
        }
    };

    /*! ListChunk
     */
    struct KOOLIB_API XMLListChunk : virtual IChunk
    {
        IChunkPtr _head;
        IChunkPtr _tail;

#ifndef NDEBUG
        private:
        XMLListChunk *_next;
        public:
        
        XMLListChunk()
            : _next(0)
            {
            }

        XMLListChunk( IChunk *head, IChunk *tail )
            : _head( head ), _tail( tail )
            {
                _next = dynamic_cast< XMLListChunk *>( tail );
            }

        void SetTail( IChunk *pTail )
        {
            _tail = pTail;
            _next = dynamic_cast< XMLListChunk *>( pTail );
        }
#else        
        XMLListChunk()
            {}

        XMLListChunk( IChunk *head, IChunk *tail )
            : _head( head ), _tail( tail )
            {}
        
        void SetTail( IChunk *pTail )
        {
            _tail = pTail;
        }
#endif

        void SetHead( IChunk *pHead )
        {
            _head = pHead;
        }
        
        IChunk* GetChunk( unsigned int name )
        {
            if ( name == FOURCC_LIST_HEAD )
            {
                return _head.get();
            }
            else if ( name == FOURCC_LIST_TAIL )
            {
                return _tail.get();
            }
            else
            {
                return GetChunkAtPosition4CC( name );
            }
        }

        IChunk* GetChunk( const TextUtf8 &name )
        {
            if ( name == L"head" )
            {
                return _head.get();
            }
            else if ( name == L"tail" )
            {
                return _tail.get();
            }
            else if ( name[0] >= '0' && name[0] <= '9' )
            {
                int position;
                assign( position, name );
                return GetChunkAtPosition( position );
            }
            return 0;
        }

        IChunk *GetChunkAtPosition( int index );
        IChunk *GetChunkAtPosition4CC( unsigned int fcc );
        
        void SetChunk( const  TextUtf8 &name, IChunk *pChunk );
        void SetChunk( unsigned int name, IChunk *pChunk );

        bool Execute( Node node, ExecutionState &state );
        
        unsigned int GetFourCC( unsigned int name );
        
        bool operator >> ( XMLFactory::AttrUniBase &attr )
        {
            if ( attr._name == L"type-name" )
            {
                attr.str(L"list");
                return true;
            }
            else if ( attr._name == L"defined-symbols" )
            {
                attr.str( L"type-name head tail" );
                return true;
            }
            return false;
        }
    };
    
    /*! Curry enabled function chunk
     */
    struct KOOLIB_API XMLFunctionChunk : XMLCodeBlock
    {
        std::vector< TextUtf8 > _parameters;

        XMLFunctionChunk()
        {
        }

        XMLFunctionChunk( Node node )
        {
            Attr< TextUtf8, false > parameters("parameters");

            if ( node >> parameters )
            {
                for ( ; !!parameters; parameters++ )
                {
                    TextUtf8 name = *parameters;
                    _parameters.push_back( name );
                }
            }
            
            _subDoc = node.SubTree();
        }

        ~XMLFunctionChunk()
        {
        }

        bool Execute( Node node, ExecutionState &state );

        bool operator >> ( XMLFactory::AttrUniBase &attr )
        {
            if ( attr._name == L"type-name" )
            {
                attr.str(L"xml-function");
                return true;
            }
            else if ( attr._name == L"parameters" )
            {
                TextAnsi params;
                for ( std::vector< TextUtf8 >::iterator it = _parameters.begin();; )
                {                    
                    params += (*it);
                    ++it;
                    if ( it != _parameters.end() )
                    {
                        params += L' ';
                    }
                    else
                    {
                        break;
                    }
                }
                attr.str( params );
                return true;
            }
            else if ( attr._name == L"defined-symbols" )
            {
                XMLCodeBlock::operator >> ( attr );
                attr.str( L"parameters" + attr.str() );
                return true;
            }
            else
            {
                return XMLCodeBlock::operator >> ( attr );
            }
        }
    };
    
    typedef boost::intrusive_ptr< XMLListChunk > XMLListPtr;


    KOOLIB_API XMLCodeBlock         *CreateBlock();
    KOOLIB_API XMLListChunk         *CreateList ( IChunk *pHead, IChunk *pTail );
    KOOLIB_API XMLEmpty             *CreateEmpty();
    KOOLIB_API XMLValueChunk< int > *CreateValue( int value );
    KOOLIB_API XMLValueChunk< double > *CreateValue( double value );
    KOOLIB_API XMLValueChunk< TextUtf8 > *CreateValue( const TextUtf8 & );
    KOOLIB_API XMLValueChunk< std::string > *CreateValue( const std::string & );

    struct KOOLIB_API SubScope : ExecutionState
    {
        boost::intrusive_ptr< XMLCodeBlock > _block;

        SubScope( ExecutionState &scope ): ExecutionState( scope )
        {
            _block = CreateBlock();
            _currentBlock = _block.get();
            _prevState = &scope;
            SetResult(0);
        }

        ~SubScope()
        {
        }

        void Reuse()
        {
            _block->DropAll();
            SetResult(0);
        }
    };

    // If chunk is lazy evaluate its value, otherwise return chunk.
    inline IChunkPtr Force( IChunkPtr pChunk, ExecutionState &state )
    {
        if ( 0 != pChunk.get() && pChunk->IsLazy() )
        {
            SubScope newState( state );
            do {
                pChunk->Execute( Node(), newState );
                pChunk = newState.GetResult();
                newState.Reuse();
            } while ( 0 != pChunk.get() && pChunk->IsLazy() );
        }
        return pChunk;
    }
    
    // convert xml nodes to LiXP objects:
    //
    //  <block>
    //      <string id="name" value="TagName"/>
    //      <block id="attributes">
    //          <string|integer|float id="AttributeName" value="Attribute Value" />
    //      </block>
    //      <list id="children">
    //          <!-- inner node -->
    //          <!-- inner node -->
    //          <!-- ... -->
    //          <!-- inner node -->
    //      </list>
    //  </block>
    //          
    //
    KOOLIB_API IBlockPtr XMLNodeToBlock( Node node, ExecutionState &state );
        

};//XMLProgram

#endif//__INCLUDED_XML_PROGRAM_CHUNKS_H__
