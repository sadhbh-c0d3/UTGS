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
  last mod: $Id: XMLChunksProgram.cpp,v 1.23 2005/03/18 17:48:36 koolas Exp $

 ********************************************************************/

#include "koolib/xml/XMLChunksProgram.h"
#include "koolib/xml/XMLChunksMethod.h"
#include "koolib/xml/XMLKompiler.h"
#include "koolib/FastAllocator.h"
#include "koolib/dynamic_cast_call.h"
#include "koolib/Random.h"
#include <cmath>

using namespace Useless;
using namespace XMLFactory;
using namespace XMLProgram;

namespace XMLProgram {
    // Define tag dictionaries
    namespace XML
    {
        struct FSM
        {
            struct STATE {};
            struct GOTO {};
            struct ENTER{};
            struct LEAVE {};
        };

        struct XML {};
        struct CALL {};
        struct RESULT {};
        struct EXPORT {};
        struct SELECT {};
        struct TEST {};
        struct DEFINE {};
        struct DEFINED {};
        struct RUN {};
        struct KILL {};
        struct SEND {};
        struct IMPORT {};

        struct CHUNK {};
        struct BLOCK {};
        struct EMPTY {};

        // list operations
        struct LIST {};
        struct MERGE {};
        struct MAP {};
        struct ZIP {};
        struct REVERSE {};
        struct SORT {};
        struct SHUFFLE {};
        struct SAMPLE {};      

        struct LET {};
        struct SET {};
        struct FUNCTION {};
        struct GET {};
        struct CHOOSE {};
        struct IF {};
        struct DO {};
        struct COMPILE {};

        // evaluate 
        struct EVAL {};
        struct VALUE {};
        struct FLOOR {};
        struct CEIL {};
        struct ROUND {};
        struct STRING {};
        struct BOOLEAN {};
        struct INTEGER {};
        struct FLOAT {};
        // arithmetic operations
        struct ADD {};
        struct MULTIPLY {};
        struct RECIPROCAL {};
        struct MINUS {};
        // comparison
        struct COMPARE {};
        struct LESS {};
        struct GREATER {};
        struct LESS_EQUAL {};
        struct GREATER_EQUAL {};
        struct EQUAL {};
        // boolean operations
        struct OR {};
        struct AND {};
        struct NOT {};
        // regex operations
        struct REPLACE {};
        struct CAT {};
    };
    
    USELESS_XML_TAG_DICT_DEFINE( xtdProgram, ExecutionState );
    USELESS_XML_TAG_DICT_DEFINE( xtdFSM_Main, ExecutionState );
    USELESS_XML_TAG_DICT_DEFINE( xtdFSM_State, ExecutionState );
    USELESS_XML_TAG_DICT_DEFINE( xtdImport, ExecutionState );
 
    // Tags allowed everywhere, where code chunk is expected
    static USELESS_XML_TAG_DICT_Extend( xtdProgram, ExecutionState, BasicProgram,
            .AddReg< XML::XML            >("xml") 
            .AddReg< XML::CALL           >("call")
            .AddReg< XML::RESULT         >("result")
            .AddReg< XML::EXPORT         >("export")
            .AddReg< XML::SELECT         >("select")
            .AddReg< XML::TEST           >("test")
            .AddReg< XML::CHUNK          >("chunk")
            .AddReg< XML::BLOCK          >("block")
            .AddReg< XML::EMPTY          >("empty")
            .AddReg< XML::DEFINE         >("define")
            .AddReg< XML::DEFINED        >("defined")
            .AddReg< XML::RUN            >("run")
            .AddReg< XML::KILL           >("kill")
            .AddReg< XML::SEND           >("send")
            .AddReg< XML::IMPORT         >("import")
            .AddReg< XML::LIST           >("list")
            .AddReg< XML::MERGE          >("merge")
            .AddReg< XML::MAP            >("map")
            .AddReg< XML::ZIP            >("zip")
            .AddReg< XML::REVERSE        >("reverse")
            .AddReg< XML::SORT           >("sort")
            .AddReg< XML::SHUFFLE        >("shuffle")
            .AddReg< XML::SAMPLE         >("sample")
            .AddReg< XML::LET            >("let")
            .AddReg< XML::SET            >("set")
            .AddReg< XML::FUNCTION       >("function")
            .AddReg< XML::GET            >("get")
            .AddReg< XML::CHOOSE         >("choose")
            .AddReg< XML::IF             >("if")
            .AddReg< XML::DO             >("do")
            .AddReg< XML::COMPILE        >("compile")
            .AddReg< XML::FSM            >("fsm")
            .AddReg< XML::EVAL           >("eval")
            .AddReg< XML::VALUE          >("value")
            .AddReg< XML::FLOOR          >("floor")
            .AddReg< XML::CEIL           >("ceil")
            .AddReg< XML::ROUND          >("round")
            .AddReg< XML::STRING         >("string")
            .AddReg< XML::BOOLEAN        >("boolean")
            .AddReg< XML::INTEGER        >("integer")
            .AddReg< XML::FLOAT          >("float")
            .AddReg< XML::ADD            >("add")
            .AddReg< XML::MULTIPLY       >("multiply")
            .AddReg< XML::RECIPROCAL     >("reciprocal")
            .AddReg< XML::MINUS          >("minus")
            .AddReg< XML::COMPARE        >("compare")
            .AddReg< XML::LESS           >("less")
            .AddReg< XML::GREATER        >("greater")
            .AddReg< XML::LESS_EQUAL     >("less-equal")
            .AddReg< XML::GREATER_EQUAL  >("greater-equal")
            .AddReg< XML::EQUAL          >("equal")
            .AddReg< XML::OR             >("or")
            .AddReg< XML::AND            >("and")
            .AddReg< XML::NOT            >("not")
            .AddReg< XML::REPLACE        >("replace")
            .AddReg< XML::CAT            >("cat")
            );

    // Tags allowed, inside fsm definition
    static USELESS_XML_TAG_DICT_Extend( xtdFSM_Main, ExecutionState, BasicProgram,
            .AddBase( &xtdProgram )
            .AddReg< XML::FSM::STATE >("state")
            .AddReg< XML::FSM::GOTO >("goto")
            .AddReg< XML::FSM::ENTER >("enter")
            .AddReg< XML::FSM::LEAVE >("leave")
            .HideReg("fsm")
            );

    // Tags allowed inside fsm::state definition
    static USELESS_XML_TAG_DICT_Extend( xtdFSM_State, ExecutionState, BasicProgram,
            .AddBase( &xtdFSM_Main )
            .HideReg("state")
            );
    
    XMLCodeBlock *CreateBlock()
    {
        return Memory::FastAllocate< XMLCodeBlock >();
    }

    XMLEmpty *CreateEmpty()
    {
        // use only one instance of XMLEmpty in whole world
        static boost::intrusive_ptr< XMLEmpty > s_Empty;
        if ( !s_Empty )
        {
            s_Empty = new XMLEmpty();
        }
        return s_Empty.get();
        //return Memory::FastAllocate< XMLEmpty >();
    }

    XMLValueChunk< int > *CreateValue( int value )
    {
        // use only one instance of 0 and 1
        static boost::intrusive_ptr< XMLValueChunk< int > > s_Zero, s_One;
        switch( value )
        {
            case 0:
                if ( !s_Zero )
                {
                    s_Zero = new XMLValueChunk< int >( 0 );
                }
                return s_Zero.get();
                
            case 1:
                if ( !s_One )
                {
                    s_One = new XMLValueChunk< int >( 1 );
                }
                return s_One.get();
                
            default:
                XMLValueChunk< int > *pValue = Memory::FastAllocate< XMLValueChunk< int > >();
                pValue->SetValue( value );
                return pValue;
        }
    }
    
    XMLValueChunk< double > *CreateValue( double value )
    {
        XMLValueChunk< double > *pValue = Memory::FastAllocate< XMLValueChunk< double > >();
        pValue->SetValue( value );
        return pValue;
    }
    
    XMLValueChunk< TextUtf8 > *CreateValue( const TextUtf8 &value )
    {
        XMLValueChunk< TextUtf8 > *pValue = Memory::FastAllocate< XMLValueChunk< TextUtf8 > >();
        pValue->SetValue( value );
        return pValue;
    }
    
    XMLValueChunk< std::string > *CreateValue( const std::string &value )
    {
        XMLValueChunk< std::string > *pValue = Memory::FastAllocate< XMLValueChunk< std::string > >();
        pValue->SetValue( value );
        return pValue;
    }

    XMLListChunk *CreateList( IChunk *pHead, IChunk *pTail )
    {
        XMLListChunk *pList = Memory::FastAllocate< XMLListChunk >();
        pList->SetHead( pHead );
        pList->SetTail( pTail );
        return pList;
    }

  

    TextUtf8 g_lastExpression, g_lastExpressionValue;

    IChunkPtr EvaluateObject( Node &_node, ExecutionState &_state, int flags )
    {
        if ( _node.HasChildren() || (( flags & EVOB_COPY_ATTRIBUTES ) && !(_node->_attributes.empty()) ))
        {
            IBlockPtr newBlock;

            ExecutionState newState( _state );            
            newState._prevState = &_state;
            
            if ( flags & EVOB_SEPARATE_BLOCK )
            {
                newBlock = new XMLCodeBlock;
                newState._currentBlock = newBlock.get();

                if ( flags & EVOB_COPY_ATTRIBUTES )
                {
                    for ( std::map< std::string, std::string >::const_iterator
                            itAttr = _node->_attributes.begin();
                            itAttr != _node->_attributes.end();
                            ++itAttr )
                    {
                        // "id" should not be copied
                        if ( (*itAttr).first != "id" )
                        {
                            IChunkPtr result = EvaluateExpression( (*itAttr).second, _state );
                            newBlock->AddChunk( (*itAttr).first, result.get() );
                        }
                    }
                }
                
                if ( 0 == ( flags & EVOB_NO_CODE_EXECUTION ))
                {
                    newState.ExecuteChildren( _node );
                }
                else
                {
                    // Since code is not executed, caller should have possibility
                    // to use 'newState' for customized code execution.
                    // Caution: _state variable becomes out type variable.
                    _state = newState;
                    if ( flags & EVOB_RETURN_BLOCK )
                    {
                        return newBlock;
                    }
                    else
                    {
                        return 0;
                    }
                }

                if ( flags & EVOB_RETURN_BLOCK )
                {
                    return newBlock;
                }
                else
                {
                    return newState.GetResult();
                }
            }
            else
            {
                newState.ExecuteChildren( _node );
                return newState.GetResult();
            }
        }
        else
        {
            if ( flags & EVOB_NO_STRING_EVALUATION )
            {
                return 0;
            }

            if ( flags & EVOB_NUMERIC )
            {
                if ( flags & EVOB_FLOAT )
                {
                    return CreateValue( EvaluateString( _node->_value, _state, double() ));
                }
                else
                {
                    return CreateValue( EvaluateString( _node->_value, _state, int() ));
                }
            }
            else
            {
                return CreateValue( EvaluateString( _node->_value, _state, TextUtf8() ));
            }
        }
    }
    
        
    IChunkPtr EvaluateExpression( TextUtf8 expression, ExecutionState &state )
    {
        return CreateValue( EvaluateString( expression, state ));
    }

    TextUtf8 EvaluateString( TextUtf8 value, ExecutionState &state, const TextUtf8 &tag )
    {
        return EvaluateString( value, state );
    }

    int EvaluateString( TextUtf8 value, ExecutionState &state, int tag )
    {
        return EvaluateInteger( EvaluateString( value, state ));
    }

    double EvaluateString( TextUtf8 value, ExecutionState &state, double tag )
    {
        return EvaluateFloat( EvaluateString( value, state ));
    }
    
    TextUtf8 EvaluateTextUtf8( Node node, ExecutionState &state )
    {
        IChunkPtr result = EvaluateObject( node, state, EVOB_SEPARATE_BLOCK | EVOB_NO_STRING_EVALUATION );
        if ( 0 == result.get() )
        {
            return EvaluateString( node->_value, state );
        }
        else
        {
            return value_of< TextUtf8 >( result.get() );
        }
    }

    int EvaluateInteger( Node node, ExecutionState &state )
    {
        IChunkPtr result = EvaluateObject( node, state, EVOB_SEPARATE_BLOCK | EVOB_NO_STRING_EVALUATION );
        if ( 0 == result.get() )
        {
            return EvaluateInteger( EvaluateString( node->_value, state ));
        }
        else
        {
            return value_of< int >( result.get() );
        }
    }
    
    double EvaluateFloat( Node node, ExecutionState &state )
    {
        IChunkPtr result = EvaluateObject( node, state, EVOB_SEPARATE_BLOCK | EVOB_NO_STRING_EVALUATION );
        if ( 0 == result.get() )
        {
            return EvaluateFloat( EvaluateString( node->_value, state ));
        }
        else
        {
            return value_of< double >( result.get() );
        }
    }


    /*! Evaluate string expression, replacing all occurences of $(look-up-name) with value.
     */
    TextUtf8 EvaluateString( TextUtf8 value, ExecutionState &state )
    {
        g_lastExpression = value;
        TextUtf8 result;

        int p=0,q=0;
        
        enum CvtMode { CVT_STRING, CVT_INTEGER, CVT_FLOAT, CVT_NOT, CVT_BOOL };
        CvtMode cvtMode;
        int dp = 0;
        
        while ( true )
        {
            cvtMode = CVT_STRING;
            p = value.find(L"$(",q);
            dp = 2;
            
            if ( -1 == p )
            {
                cvtMode = CVT_INTEGER;
                p = value.find(L"#(",q);
                dp = 2;
            }
            
            if ( -1 == p )
            {
                cvtMode = CVT_FLOAT;
                p = value.find(L"#.(",q);
                dp = 3;
            }
            
            if ( -1 == p )
            {
                cvtMode = CVT_NOT;
                p = value.find(L"!(",q);
                dp = 2;
            }
            
            if ( -1 == p )
            {
                cvtMode = CVT_BOOL;
                p = value.find(L"!!(",q);
                dp = 3;
            }

            if ( -1 == p )
            {
                result += value.substr(q, value.size()-q);
                break;
            }
            result += value.substr(q, p-q);
            p += dp;
            q = value.find(L")", p);
            if ( -1 == q )
            {
                q = value.size() - p;
            }
            TextUtf8 name = value.substr(p, q-p);
            if ( CVT_STRING == cvtMode )
            {
                Attr< TextUtf8 , true, wchar_t > a( name );
                GetChunkAttr( a, state );
                result += TextUtf8( a.str());
            }
            else if ( CVT_INTEGER == cvtMode )
            {
                Attr< int, true, wchar_t > a( name );
                GetChunkAttr( a, state );
                TextUtf8 value;
                assign( value, *a );
                result += value;
            }
            else if ( CVT_FLOAT == cvtMode )
            {
                Attr< double, true, wchar_t > a( name );
                GetChunkAttr( a, state );
                TextUtf8 value;
                assign( value, *a );
                if ( -1 == value.find(L".") )
                {
                    value += L".0";
                }
                result += value;
            }
            else if ( CVT_NOT == cvtMode )
            {
                Attr< int, true, wchar_t > a( name );
                GetChunkAttr( a, state );
                TextUtf8 value;
                assign( value, !(*a) );
                result += value;
            }
            else if ( CVT_BOOL == cvtMode )
            {
                Attr< int, true, wchar_t > a( name );
                GetChunkAttr( a, state );
                TextUtf8 value;
                assign( value, !!(*a) );
                result += value;
            }
            q += 1;
        }
        
        g_lastExpressionValue = result;
        return result;
    }

    typedef std::pair< TextUtf8, TextUtf8 > SplitNames;

    /*! SplitChunkName - used to split name like: Object:Attribute to pair {Object, Attribute}
     */
    SplitNames SplitChunkName( TextUtf8 fullName, ExecutionState &state )
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
        names.first = EvaluateString( names.first, state );
        names.second = EvaluateString( names.second, state );
        return names;
    }

    /*! GetChunk - used to recursively find chunk
     */
    IChunk* GetChunk( TextUtf8 chunkName, IChunk *pChunk, ExecutionState &state, IChunk **pParent )
    {
        if ( chunkName.empty() )
        {
            return pChunk;
        }
        else if ( 0 == pChunk )
        {
            return 0;
        }
        else
        {
            SplitNames names = SplitChunkName( chunkName, state );
            if ( !names.first.empty() && pParent ) { *pParent = pChunk; }
            return GetChunk( names.second, pChunk->GetChunk( names.first ), state, pParent );
        }
    }

    /*! GetChunk - used to find in current visible scopes
     */
    IChunk* GetChunk( TextUtf8 chunkName, ExecutionState &state, IChunk **pParent, ExecutionState **pState )
    {
        IChunk *pChunk;
        ExecutionState *state1 = &state;
        if ( pParent ) { *pParent = 0; }
        if ( pState )  { *pState = &state; }

        if ( chunkName.empty() )
        {
            return 0;
        }
        else
        {
            SplitNames names = SplitChunkName( chunkName, state );

            if ( names.first == L"__result__" )
            {
                if ( pChunk = state1->GetResult() )
                {
                    return GetChunk( names.second, pChunk, state, pParent );
                }
            }
            else if ( names.first == L"__globals__" )
            {
                if ( pChunk = GetGlobals()._currentBlock )
                {
                    return GetChunk( names.second, pChunk, state, pParent );
                }
            }
            else
            {
                while ( state1 )
                {
                    if ( pChunk = state1->_currentBlock->GetChunk( names.first ) )
                    {
                        if ( pParent ) { *pParent = state1->_currentBlock; }
                        return GetChunk( names.second, pChunk, state, pParent );
                    }
                    state1 = state1->_prevState;
                    if ( pState ) { *pState = state1; }
                }
            }
        }
        return 0;
    }

    XMLFiniteStateMachine::XMLFiniteStateMachine()
    {
        GlobalsAddRef();
        _baseState._currentFSM = this;
        _baseState._currentBlock = this;
        _baseState._currentDict = &xtdFSM_Main;
        _baseState._prevState = &GetGlobals();
        add_methods( this )
            .def_remap_1< ignore_t, TextUtf8 >(L"EnterState", &XMLFiniteStateMachine::EnterState, "name")
            .def_remap_1< ignore_t, TextUtf8 >(L"GotoState", &XMLFiniteStateMachine::GotoState, "name")
            .def_remap_0< ignore_t >(L"LeaveState", &XMLFiniteStateMachine::LeaveState )
            .def(L"GetCurrentState", unconst_method_cast( &XMLFiniteStateMachine::GetCurrentState ))
            .def_remap_1< void, std::string >(L"ExecuteString", &XMLFiniteStateMachine::ExecuteString, "xml")
            ;
    }
        
    XMLFiniteStateMachine::~XMLFiniteStateMachine()
    {
        GlobalsRemRef();
    }

    ExecutionState & XMLFiniteStateMachine::DuplicateStack()
    {
        _stateStackDuplicate.resize( _stateStackDuplicate.size() + 1 );
        std::deque< State > &stateStackDuplicate = _stateStackDuplicate.back();
        std::deque< State >::iterator i0, i1 = _stateStack.begin(), i2 = _stateStack.end();
        for ( i0 = i2; i1 != i2; ++i1 )
        {
            State s = *i1;
            if ( i0 == i2 ) {
                s._prevState = &_baseState;
            }
            else {
                s._prevState = &(*i0);
            }
            stateStackDuplicate.push_back( s );
            i0 = stateStackDuplicate.end();
            --i0;
        }
        if (! stateStackDuplicate.empty() )
        {
            State &s = stateStackDuplicate.back();
            return s;
        }
        else
        {
            return _baseState;
        }
    }

    void XMLFiniteStateMachine::ReleaseDuplicate()
    {
        assert( !_stateStackDuplicate.empty() );
        _stateStackDuplicate.pop_back();
    }
        
    void XMLFiniteStateMachine::ExecuteString( const std::string &s )
    {
        XMLDocument doc("<xml>"+ s +"</xml>");
        DuplicateStack().ExecuteChildren( doc.Root() );
        ReleaseDuplicate();
    }
    
    ExecutionState & XMLFiniteStateMachine::EnterState ( const TextUtf8 &name )
    {
        State newState;
        newState._prevState = &GetCurrentExecutionState();
        newState._name = name;
        newState._currentFSM = this;
        newState._currentBlock = newState._block.get();
        newState._currentDict = &xtdFSM_State;
        _stateStack.push_back( newState );

        StateDict::iterator itState = _states.find( name );

        if ( itState != _states.end() )
        {
            GetCurrentExecutionState().ExecuteChildren( (*itState).second.Root() );
        }

        return GetCurrentExecutionState();
    }

    ExecutionState & XMLFiniteStateMachine::GotoState ( const TextUtf8 &name )
    {
        if (! _stateStack.empty() )
        {
            LeaveState();
        }
        return EnterState( name );
    }

    ExecutionState & XMLFiniteStateMachine::LeaveState ()
    {
        assert( !_stateStack.empty() );
        TextUtf8 name = GetCurrentState();
        
        if ( 0 != XMLProgram::GetChunk(L"OnLeave", GetCurrentExecutionState() ))
        {
            ExecuteString( XMLTag("call")
                    .setattr("id","OnLeave")
                    .to_string() );
        }

        IChunk *result = GetCurrentExecutionState().GetResult();
        _stateStack.pop_back();
        GetCurrentExecutionState().SetResult( result );

        if ( 0 != XMLProgram::GetChunk(L"OnReturn", GetCurrentExecutionState() ))
        {
            ExecuteString( XMLTag("call")
                    .setattr("id", "OnReturn")
                    .setattr("state", name )
                    .to_string() );
        }
        return GetCurrentExecutionState();
    }

    XMLCodeBlock::~XMLCodeBlock()
    {
    }

    void XMLCodeBlock::DropAll()
    {
        _chunks.clear();
    }

    void XMLCodeBlock::AddChunk( unsigned int name, IChunk *chunk )
    {
        _chunks.insert( std::make_pair( name, chunk ));
    }

    void XMLCodeBlock::AddChunk( const TextUtf8 &name, IChunk *chunk )
    {
        unsigned int id = GetGlobalSymbolDict().AddSymbol( name );
        _chunks.insert( std::make_pair( id, chunk ));
    }

    IChunk* XMLCodeBlock::_getChunk( unsigned int id )
    {
        ChunkMap::iterator itChunk = _chunks.find( id );
        if ( itChunk != _chunks.end() )
        {
            return (*itChunk).second.get();
        }
        else
        {
            static unsigned int s_BlkId = GetGlobalSymbolDict().AddSymbol( L"__block__" );
            static unsigned int s_SymId = GetGlobalSymbolDict().AddSymbol( L"__symbols__" );
            
            if ( id == s_BlkId )
            {
                return (this);
            }

            if ( id == s_SymId )
            {
                typedef std::multimap< TextUtf8, IChunkPtr > _SortBin; _SortBin sortBin;
                for ( ChunkMap::iterator it = _chunks.begin(); it != _chunks.end(); ++it )
                {
                    sortBin.insert( std::make_pair( GetGlobalSymbolDict().GetSymbolName( (*it).first ), (*it).second ));
                }
                IChunkPtr result = CreateEmpty();
                for ( _SortBin::reverse_iterator it = sortBin.rbegin(); it != sortBin.rend(); ++it )
                {
                    result = CreateList( // create pair [NAME, VALUE]
                            CreateList( CreateValue( (*it).first ), //< 1-st: NAME
                                CreateList( (*it).second.get(), CreateEmpty() )) //<< 2-nd: VALUE
                            , result.get() );
                }
                GetGlobals().SetResult( result.get() );
                return result.get();
            }
            return 0;
        }
    }
    
    IChunk* XMLCodeBlock::_getChunk( const TextUtf8 &name )
    {
        return 0;
    }

    IChunk* XMLCodeBlock::GetChunk( unsigned int id )
    {
        assert ( id != SymbolDict::NO_SYMBOL );
        
        if ( IChunk *ptr = _getChunk( id ))
        {
            return ptr;
        }
        else
        {
            return 0;//_getChunk( GetGlobalSymbolDict().GetSymbolName( id ));
        }
    }

    IChunk* XMLCodeBlock::GetChunk( const TextUtf8 &name )
    {
        if ( IChunk *ptr = _getChunk( GetGlobalSymbolDict().GetSymbolId( name ) ))
        {
            return ptr;
        }
        else
        {
            return _getChunk( name );
        }
    }
    
    bool XMLCodeBlock::operator >> ( XMLFactory::AttrUniBase &attr )
    {
        IChunkPtr pChunk = GetChunk( attr._name );

        if ( !!pChunk )
        {
            Attr< TextUtf8, true, wchar_t > attr1(L"value");
            (*pChunk) >> attr1;
            attr.str( attr1.str() );
            return true;
        }        
        else if ( attr._name == L"type-name" )
        {
            attr.str(L"xml-block");
            return true;
        }
        else if ( attr._name == L"defined-symbols" )
        {
            TextUtf8 s(L"type-name");
            for ( ChunkMap::iterator it = _chunks.begin(); it != _chunks.end(); ++it )
            {
                s += L' ';
                s += GetGlobalSymbolDict().GetSymbolName( (*it).first );
            }
            attr.str( s );
            return true;
        }
        return false;
    }

        
    void XMLCodeBlock::SetChunk( unsigned int id, IChunk *pChunk )
    {
        ChunkMap::iterator it = _chunks.find( id );
        if ( it != _chunks.end() )
        {
            (*it).second = pChunk;
        }
        else
        {
            // error vs. ignore ?
        }
    }

    void XMLCodeBlock::SetChunk( const TextUtf8 &name, IChunk *pChunk )
    {
        SetChunk( GetGlobalSymbolDict().GetSymbolId( name ), pChunk );
    }
    
    IChunk* CreateSequence( const TextUtf8 &text )
    {
        IChunkPtr result;
        result = new XMLEmpty();
        for ( int i=text.size(); i>0; --i )
        {
            TextUtf8 c;
            c += text[i-1];
            result = CreateList( CreateValue( c ), result.get() );
        }
        GetGlobals().SetResult( result.get() );
        return result.get();
    }

    IChunk * XMLListChunk::GetChunkAtPosition( int index )
    {
        if ( index <= 0 ) { return 0; }
        IChunkPtr pList = this;
        while ( !IsEmpty( pList.get() ))
        {
            //pList = Force( pList.get(), state );
            //if ( IsEmpty( pList.get() )) { return 0; }
            if ( 0 == --index ) {
                return pList->GetChunk( FOURCC_LIST_HEAD );
            }
            pList = pList->GetChunk( FOURCC_LIST_TAIL );
        }
        return 0;
    }
    
    IChunk * XMLListChunk::GetChunkAtPosition4CC( unsigned int fcc )
    {
             if ( fcc ==  FOURCC_LIST_1ST ) return GetChunkAtPosition(1);
        else if ( fcc ==  FOURCC_LIST_2ND ) return GetChunkAtPosition(2);
        else if ( fcc ==  FOURCC_LIST_3RD ) return GetChunkAtPosition(3);
        else if ( fcc ==  FOURCC_LIST_4TH ) return GetChunkAtPosition(4);
        else if ( fcc ==  FOURCC_LIST_5TH ) return GetChunkAtPosition(5);
        else if ( fcc ==  FOURCC_LIST_6TH ) return GetChunkAtPosition(6);
        else if ( fcc ==  FOURCC_LIST_7TH ) return GetChunkAtPosition(7);
        else if ( fcc ==  FOURCC_LIST_8TH ) return GetChunkAtPosition(8);
        else if ( fcc ==  FOURCC_LIST_9TH ) return GetChunkAtPosition(9);
        return 0;
    }
    
    void XMLListChunk::SetChunk( const TextUtf8 &name, IChunk *pChunk )
    {
        if ( name == L"head" )
        {
            _head = pChunk;
        }
        else if ( name == L"tail" )
        {
            if ( IsEmpty( pChunk ) || dynamic_cast< XMLListChunk *>( pChunk ))
            {
                _tail = pChunk;
            }
            else
            {
                throw Error("Only list may be assigned to list:tail");
            }
        }
    }
    
    void XMLListChunk::SetChunk( unsigned int name, IChunk *pChunk )
    {
        SetChunk( TextUtf8( StringFrom4CC( name )), pChunk );
    }

    bool XMLListChunk::Execute( Node node, ExecutionState &state )
    {
        IChunkPtr guard( this );
        if ( _head->Execute( node, state ) && ( IsEmpty( _tail.get() ) ||_tail->Execute( node, state ) ))
        {
            return true;
        }
        else
        {
            throw Error("Not all chunks of list are executable");
            return false;
        }
    }

    unsigned int XMLListChunk::GetFourCC( unsigned int name )
    {
        if ( FOURCC_NAME_TYPE == name )
        {
            return FOURCC_TYPE_LIST;
        }
        /* Probably it is NP problem, and I can't see any purpose of having
         * the list of instructions that all are constant (may be invoked w/o separate scope-block)
        else if ( name == FOURCC_NAME_CONST )
        {
            // fold over all items, check if they are constant
            IChunkPtr guard( this );
            if ( _head->GetFourCC( FOURCC_NAME_CONST ) && ( IsEmpty( _tail.get() ) || _tail->GetFourCC( FOURCC_NAME_CONST ) ))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        */
        else
        {
            return 0L;
        }
    }

    bool XMLFunctionChunk::Execute( Node node, ExecutionState &state )
    {
        if ( _parameters.empty() )
        {
            // In current implementation <call> creates separate block for code execution.
            // So we don't need to repeat this activity.
            // We need to attach chunks defined in our function to execution environment.
            // To avoid changes in function's block, we put in-between current state and previous one.
            ExecutionState midState( state );
            midState._currentBlock = this;
            state._prevState = &midState;
            state.ExecuteChildren( _subDoc.Root() );
            state._prevState = midState._prevState;
        }
        else
        {
            XMLFunctionChunk *pFunc;
            IChunkPtr guardFunc( pFunc = new XMLFunctionChunk() );
            pFunc->_chunks = _chunks;
            pFunc->_subDoc = _subDoc;

            for ( std::vector< TextUtf8 >::iterator itParam = _parameters.begin();
                    itParam != _parameters.end(); ++itParam )
            {
                IChunk *pChunk = XMLProgram::GetChunk( *itParam, state );
                if ( pChunk )
                {
                    pFunc->AddChunk( *itParam, pChunk );
                }
                else
                {
                    pFunc->_parameters.push_back( *itParam );
                }
            }

            if ( pFunc->_parameters.empty() )
            {
                pFunc->Execute( node, state );
            }
            else
            {
                state.SetResult( pFunc );
            }
        }
        return true;
    } 

    bool IsEmpty( IChunk *pChunk )
    {
        if ( 0 == pChunk || dynamic_cast< XMLEmpty *>( pChunk )) {
            return true;
        }
        else {
            return false;
        }
    }

    static IBlockPtr      g_GlobalsBlock;
    static ExecutionState g_GlobalExecState;
    static int            g_GlobalsRefCount = 0;

    void GlobalsAddRef()
    {
        if ( 0 == g_GlobalsRefCount )
        {
            g_GlobalsBlock = CreateBlock();
            g_GlobalExecState._currentFSM = 0;
            g_GlobalExecState._currentBlock = g_GlobalsBlock.get();
            g_GlobalExecState._currentDict = &xtdProgram;
            g_GlobalExecState._prevState = 0;
            g_GlobalsBlock->AddChunk(L"__Empty__", CreateEmpty());
            g_GlobalsBlock->AddChunk(L"__Zero__", CreateValue( 0 ));
            g_GlobalsBlock->AddChunk(L"__One__", CreateValue( 1 ));
            g_GlobalsBlock->AddChunk(L"__False__", CreateValue( 0 ));
            g_GlobalsBlock->AddChunk(L"__True__", CreateValue( 1 ));
        }
        ++g_GlobalsRefCount;
    }

    void GlobalsRemRef()
    {
        if ( g_GlobalsRefCount > 0 )
        {
            --g_GlobalsRefCount;
        }
        if ( 0 == g_GlobalsRefCount )
        {         
            g_GlobalsBlock = 0;
            g_GlobalExecState._currentBlock = 0;
            g_GlobalExecState.SetResult(0);
        }
    }
    
    ExecutionState & GetGlobals()
    {
        if ( !g_GlobalsBlock )
        {
            throw Error("Cannot access __globals__");
        }
        return g_GlobalExecState;
    }

    SymbolDict & GetGlobalSymbolDict()
    {
        static SymbolDict s_Dict;
        return s_Dict;
    }
    
    IBlockPtr XMLNodeToBlock( Node node, ExecutionState &state )
    {
        IBlockPtr pBlock = CreateBlock();
        pBlock->AddChunk(L"name", CreateValue( TextUtf8( node->_name )) );
        IBlockPtr pAttr = CreateBlock();
        pBlock->AddChunk(L"attributes", pAttr.get() );
        XMLAttributes::const_iterator iter = node->_attributes.begin();
        for (; iter != node->_attributes.end(); ++iter )
        {
            pAttr->AddChunk( TextUtf8( (*iter).first ), CreateValue( TextUtf8( (*iter).second )) );
        }
        if ( !node.HasChildren())
        {
            pBlock->AddChunk(L"value", CreateValue( TextUtf8( node->_value )) );
        }
        else
        {
            std::list< IChunkPtr > children;                    
            for ( Node child = node.GetFirstChild(); !!child; ++child )
            {
                children.push_back( XMLNodeToBlock( child, state ).get() );
            }
            std::list< IChunkPtr >::reverse_iterator it = children.rbegin();                    
            IChunkPtr pPrev = CreateEmpty();
            for (; it != children.rend(); ++it )
            {
                pPrev = CreateList( (*it).get(), pPrev.get() );
            }
            pBlock->AddChunk(L"children", pPrev.get() );
        }
        return pBlock;
    }


};//XMLProgram







namespace XMLFactory {

    using namespace XMLProgram;
    using namespace Useless;

//#define LOCAL_TAG_HANDLER( _name )  Useless_XML_TAG_HANDLER( _name, ExecutionState )

const int G_DebugNodeStackSize = 128;
Node *g_Debug_NodeStack[ G_DebugNodeStackSize ];
int   g_Debug_StackTop = G_DebugNodeStackSize;

#define LOCAL_TAG_HANDLER( _name ) \
    void _Execute_Node( _name &, Node _node, ExecutionState &_state );\
    USELESS_XML_TAG_HANDLER( _name, ExecutionState ) \
    {\
        if ( 0 == g_Debug_StackTop ) { _Execute_Node( _name(), _node, _state ); }\
        else { \
            g_Debug_NodeStack[ --g_Debug_StackTop ] = &_node; \
            _Execute_Node( _name(), _node, _state );\
            g_Debug_NodeStack[ g_Debug_StackTop++ ] = 0;\
        }\
    }\
    ExecutionState::ResultType _Execute_Node( _name &, Node _node, ExecutionState &_state )

    /*! <fsm>
     */
    LOCAL_TAG_HANDLER( XML::FSM )
    {
        _state._currentDict->ExecuteChildren( _node, _state );
    }

    /*! <state>
     */
    LOCAL_TAG_HANDLER( XML::FSM::STATE )
    {
        Attr< TextUtf8, true, wchar_t > id(L"id");

        GetAttr( id, _node, _state );

        _state._currentFSM->AddState( id.str(), _node );
    }
    
    /*! <goto>
     */
    LOCAL_TAG_HANDLER( XML::FSM::GOTO )
    {
        Attr< TextUtf8, true, wchar_t > id(L"id");

        GetAttr( id, _node, _state );

        ExecutionState s( _state );
        s._prevState = &_state;
        s._currentBlock = _state._currentFSM->GotoState( id.str() )._currentBlock;
        s.ExecuteChildren( _node );
    }
    
    /*! <enter>
     */
    LOCAL_TAG_HANDLER( XML::FSM::ENTER )
    {
        Attr< TextUtf8, true, wchar_t > id(L"id");
        
        GetAttr( id, _node, _state );

        ExecutionState s( _state );
        s._prevState = &_state;
        s._currentBlock = _state._currentFSM->EnterState( id.str() )._currentBlock;
        s.ExecuteChildren( _node );
    }

    /* <leave>
     */
    LOCAL_TAG_HANDLER( XML::FSM::LEAVE )
    {
        Attr< TextUtf8, false, wchar_t >   id(L"id");
        Attr< bool, false, wchar_t >        test(L"test");

        if ( GetAttr( id, _node, _state ))
        {
            TextUtf8 name = _state._currentFSM->GetCurrentState();
            if ( name != id.str() )
            {
                if ( !GetAttr( test, _node, _state ) || ( 0 == *test ))
                {
                    throw Error("LeaveState: Cannot leave '%S', since __fsm__:current-state = '%S'"
                            , id.str().c_str(), name.c_str() );
                }
            }
            else
            {
                _state._currentFSM->LeaveState();
            }
        }
        else
        {
            _state._currentFSM->LeaveState();
        }
    }
    
    /*! <chunk>
     */
    LOCAL_TAG_HANDLER( XML::CHUNK )
    {
        Attr< TextUtf8, false, wchar_t > id(L"id");

        if ( GetAttr( id, _node, _state ))
        {
            _state._currentBlock->AddChunk( id.str(), new XMLCodeChunk( _node ));
        }
        else
        {
            _state.SetResult( new XMLCodeChunk( _node ));
        }
    }

    /*! block
     */
    LOCAL_TAG_HANDLER( XML::BLOCK )
    {
        _state.SetResult( EvaluateObject( _node, _state, EVOB_SEPARATE_BLOCK | EVOB_COPY_ATTRIBUTES | EVOB_RETURN_BLOCK ).get() );
    }

    /*! empty
     */
    LOCAL_TAG_HANDLER( XML::EMPTY )
    {
        _state.SetResult( new XMLEmpty() );
    }
    

    LOCAL_TAG_HANDLER( XML::XML )
    {
        //@TODO: string
        Attr< std::string, false, char > category("category");

        if ( GetAttr( category, _node, _state ))
        {
            Useless::CategoryFromXML( category.str(), _node._iter );
        }
        else
        {
            Attr< std::string, false, char > inc("include");
            if ( GetAttr( inc, _node, _state ))
            {
                Useless::CreateFromXML("<xml><include src=\""+inc.str()+"\"/></xml>");
            }
            else
            {
                assert(0);
            }
        }

    }


    /*! <call> result := f( <arg-list> )
     */
    LOCAL_TAG_HANDLER( XML::CALL )
    {
        Attr< TextUtf8, false, wchar_t > _id(L"id");
        Attr< int, false, wchar_t >      _tryExec(L"try");
        Attr< int, false, wchar_t >      _copyAttr(L"copyAttr");
        Attr< int, false, wchar_t >      _noGlobals(L"noGlobals");
        Attr< int, false, wchar_t >      _separateBlock(L"separateBlock");
        Attr< int, false, wchar_t >      _clearBlock(L"clearBlock");

        // create new environment for that call        
        ExecutionState newState( _state );

        int flags = EVOB_NO_CODE_EXECUTION;

        if ( !GetAttr( _separateBlock, _node, _state ) || 1 == *_separateBlock )
        {
            flags |= EVOB_SEPARATE_BLOCK | EVOB_RETURN_BLOCK;
        }
        
        if ( !GetAttr( _copyAttr, _node, _state ) || 1 == *_copyAttr )
        {
            flags |= EVOB_COPY_ATTRIBUTES;
        }

        if ( GetAttr( _clearBlock, _node, _state ) && 1 == *_clearBlock )
        {
            dynamic_cast< XMLCodeBlock *>( _state._currentBlock )->DropAll();
        }

        IChunkPtr guard = EvaluateObject( _node, newState, flags );
        
        // previous is _state (needed because we use EVOB_NO_CODE_EXECUTION flag)
        newState._prevState = &_state;

        // first, execute all interrior tags
        newState.ExecuteChildren( _node );

        if ( GetAttr( _noGlobals, _node, _state ) && 1 == *_noGlobals )
        {
            newState._prevState = 0;
        }
        
        // execute named chunk
        if ( GetAttr( _id, _node, _state ))
        {
            IChunkPtr pChunk = GetChunk( _id.str(), _state );
            if ( !pChunk || !pChunk->Execute( _node, newState ))
            {
                if ( !GetAttr( _tryExec, _node, _state ) || 0 == *_tryExec )
                {
                    throw Error("<call>: Cannot execute method %S.", _id->c_str() );
                }
                else
                {
                    _state.SetResult( CreateValue( TextUtf8(L"call-error" )));
                }
            }
            else
            {
                _state.SetResult( newState.GetResult() );
            }
        }
        // execute result
        else
        {
			if  ( !newState.GetResult() )
			{
				throw Error("<call>: Cannot execute NULL object, the result from inner nodes is NULL.", _id->c_str() );
			}
			else
			{
                newState.GetResult()->Execute( _node, newState );
                _state.SetResult( newState.GetResult() );
			}
        }
    }
   


    /*! <result> assigns last call result value to variable named 'as'.
     */
    LOCAL_TAG_HANDLER( XML::RESULT )
    {
        Attr< TextUtf8, true, wchar_t > as(L"as");
        GetAttr( as, _node, _state );        
        _state._currentBlock->AddChunk( as.str(), _state.GetResult() );
    }


    /*! <export> export local chunk to outer block
     */
    LOCAL_TAG_HANDLER( XML::EXPORT )
    {
        Attr< TextUtf8, true, wchar_t > ref(L"from");
        Attr< TextUtf8, true, wchar_t > as(L"as");
        GetAttr( as, _node, _state );
        GetAttr( ref, _node, _state );

        if ( as.str() == L"__result__" )
        {
            _state.SetResult( GetChunk( ref.str(), _state ));
        }
        else
        {
            _state._prevState->_currentBlock->AddChunk( as.str(), GetChunk( ref.str(), _state ));
        }
    }
    
   

    /*! <select>
     *
     *  switch( for )
     *  {
     *      case a1: ... break;
     *      case a2: ... break;
     *          :
     *      case aN: ... break;
     *      default: ...
     *  };
     */
    LOCAL_TAG_HANDLER( XML::SELECT )
    {
        Attr< TextUtf8, false, wchar_t > ref(L"from");
        
        if ( !GetAttr( ref, _node, _state ))
        {
            ref.str( L"__result__" );
        }
        
        Attr< TextUtf8, true, wchar_t > value( ref.str() );
        GetChunkAttr( value, _state );

        
        int caseCount = 0;

        for ( Node node(_node.GetFirstChild()); !!node; ++node )
        {
            if( node->_name == "case" )
            {
                Attr< TextUtf8, true, wchar_t > case_id(L"id");
                GetAttr( case_id, node, _state );

                if ( case_id.str() == value.str() ||
                        (case_id.str() == L"true" && value.str() == L"1") ||
                        (case_id.str() == L"false" && value.str() == L"0")
                   )
                {
                    _state.ExecuteChildren( node );
                    ++caseCount;
                }
            }
            else if ( node->_name == "default" )
            {
                if ( 0 == caseCount )
                {
                    _state.ExecuteChildren( node );
                    ++caseCount;
                }
            }
        }

        assert( 0 != caseCount && "<select>");
        assert( 1 == caseCount && "<select>");
    }



    /*! <test>
     */
    LOCAL_TAG_HANDLER( XML::TEST )
    {
        Attr< TextUtf8, false, wchar_t > ref(L"from");
        Attr< TextUtf8, false, wchar_t > inv(L"invert");

        if ( !GetAttr( inv, _node, _state ))
        {
            inv = L"0";
        }
        
        if ( !GetAttr( ref, _node, _state ))
        {
            ref.str( L"__result__" );
        }
        
        Attr< TextUtf8, true, wchar_t > value( ref.str() );
        GetChunkAttr( value, _state );

        if ( ( value.str() != L"false" && (*value) != L"0" && !(*value).empty() ) ||
             ( inv.str() == L"true" || inv.str() == L"1" ) )
        {
            _state.ExecuteChildren( _node );
        }
    }



    /*! <define> is used to create new variable inside current scope
     */
    LOCAL_TAG_HANDLER( XML::DEFINE )
    {
        Attr< TextUtf8, true, wchar_t > as(L"as");
        Attr< TextUtf8, false, wchar_t > ref(L"from");

        GetAttr( as, _node, _state );
        
        if (! GetAttr( ref, _node, _state ))
        {
            IChunkPtr result = EvaluateObject( _node, _state );
            _state._currentBlock->AddChunk( as.str(), result.get() );
        }
        else
        {
            _state._currentBlock->AddChunk( as.str(), GetChunk( ref.str(), _state ));
        }
    }
    
    /*! <defined> is used to test if name is defined in currently visible scopes.
     */
    LOCAL_TAG_HANDLER( XML::DEFINED )
    {        
        Attr< TextUtf8, true, wchar_t > id(L"id");
        Attr< int, false, wchar_t > test(L"test");

        GetAttr( id, _node, _state );
        
        if (! GetAttr( test, _node, _state ))
        {
            test = false;
        }

        IChunk *pChunk = GetChunk( id.str(), _state );

        if ( *test )
        {
            _state.SetResult( CreateValue( (int)(!!pChunk) ));
        }
        else if ( !pChunk )
        {
            throw Error("Chunk %S not found in current scope", id.str().c_str() );
        }
    }

    LOCAL_TAG_HANDLER( XML::SET )
    {
        Attr< TextUtf8, true, wchar_t >      id(L"id");        
        Attr< TextUtf8, false, wchar_t > select(L"select");
        
        GetAttr( id, _node, _state );
        
        IChunk *pParent;
        IChunk *pChunk = GetChunk( id.str(), _state, &pParent );
        IChunkPtr pResult;
        
        if ( !pChunk || !pParent )
        {
            throw Error("Variable or Function '%S' is not defined", id.str().c_str() );
        }

        SplitNames names = SplitChunkName( *id, _state );
        while ( !names.second.empty() )
        {
            names = SplitChunkName( names.second, _state );
        }

        if ( GetAttr( select, _node, _state ) && !!(pResult = GetChunk( *select, _state )))
        {
            pParent->SetChunk( names.first, pResult.get() );
        }
        else
        {
            pResult = EvaluateObject( _node, _state, EVOB_SEPARATE_BLOCK );
            pParent->SetChunk( names.first, pResult.get() );
        }        
    }
    
    /*! <let> is used to create new variable inside current scope
     */
    LOCAL_TAG_HANDLER( XML::LET )
    {
        Attr< TextUtf8, true, wchar_t >      id(L"id");
        Attr< TextUtf8, false, wchar_t > select(L"select");
        
        GetAttr( id, _node, _state );

        if ( _state._currentBlock->GetChunk( id.str() ))
        {
            throw Error("Variable or Function '%S' is already defined", id.str().c_str() );
        }
        else
        {
            IChunk *pChunk = 0;
            if ( GetAttr( select, _node, _state ) && (pChunk = GetChunk( select.str(), _state )))
            {
                _state._currentBlock->AddChunk( id.str(), pChunk );
            }
            else
            {
                IChunkPtr result = EvaluateObject( _node, _state, EVOB_SEPARATE_BLOCK );
                _state._currentBlock->AddChunk( id.str(), result.get() );
            }
        }
    }
    
    LOCAL_TAG_HANDLER( XML::FUNCTION )        
    {
        Attr< TextUtf8, false, wchar_t > id(L"id");
        
        if ( GetAttr( id, _node, _state ))
        {
            if ( _state._currentBlock->GetChunk( id.str() ))
            {
                throw Error("Variable or Function '%S' is already defined", id.str().c_str() );
            }
            else
            {
                _state._currentBlock->AddChunk( id.str(), new XMLFunctionChunk( _node ));
            }
        }
        else
        {
            _state.SetResult( new XMLFunctionChunk( _node ));
        }
    }

    LOCAL_TAG_HANDLER( XML::GET )
    {
        Attr< TextUtf8, true, wchar_t > id(L"id");
        
        GetAttr( id, _node, _state );

        IChunk *pChunk = GetChunk( id.str(), _state );

        if ( pChunk )
        {
            _state.SetResult( pChunk );
        }
        else
        {
            _state.SetResult( EvaluateObject( _node, _state ).get() );
        }
    }

    LOCAL_TAG_HANDLER( XML::CHOOSE )
    {
        for ( Node child = _node.GetFirstChild(); !!child; ++child )
        {
            if ( child->_name == "when" )
            {
                Attr< TextUtf8 > test("test");
                child >> test;

                if ( EvaluateBoolean( EvaluateString( test.str(), _state )) )
                {
                    _state.SetResult( EvaluateObject( child, _state, EVOB_SEPARATE_BLOCK ).get());
                    break;
                }
            }
            else if ( child->_name == "otherwise" )
            {
                _state.SetResult( EvaluateObject( child, _state, EVOB_SEPARATE_BLOCK ).get());
                break;
            }
        }
    }

    LOCAL_TAG_HANDLER( XML::IF )
    {
        Attr< TextUtf8 > test("test");

        _node >> test;

        if ( EvaluateBoolean( EvaluateString( test.str(), _state )) )
        {
            _state.SetResult( EvaluateObject( _node, _state, EVOB_SEPARATE_BLOCK ).get() );
        }
    }

    LOCAL_TAG_HANDLER( XML::DO )
    {
        Attr< TextUtf8, false, wchar_t > _in(L"in");

        if ( GetAttr( _in, _node, _state ))
        {            
            IChunk *pChunk = GetChunk( _in.str(), _state );            
            ExecutionState state( _state );
            state._prevState = &_state;
            state._currentBlock = dynamic_cast< IBlock *>( pChunk );
            _state.SetResult( EvaluateObject( _node, state, 0).get() );
        }
        else
        {
            _state.SetResult( EvaluateObject( _node, _state, EVOB_SEPARATE_BLOCK ).get() );
        }
    }
    
    LOCAL_TAG_HANDLER( XML::EVAL )
    {
        Attr< TextUtf8, false, wchar_t > ref(L"from");
        
        if ( !GetAttr( ref, _node, _state ))
        {
            ref.str(L"__result__");
        }
        
        Attr< TextUtf8, true, wchar_t > value( ref.str() );
        GetChunkAttr( value, _state );

        TextUtf8 out = EvaluateString( value.str(), _state );
        _state.SetResult( CreateValue( out ));
    }

    LOCAL_TAG_HANDLER( XML::VALUE )
    {
        _state.SetResult( EvaluateObject( _node, _state ).get() );
    }

    LOCAL_TAG_HANDLER( XML::FLOOR )
    {
        int y = std::floor( EvaluateFloat( _node, _state ));
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::CEIL )
    {
        int y = std::ceil( EvaluateFloat( _node, _state ));
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::ROUND )
    {
        int y = std::floor( EvaluateFloat( _node, _state ) + 0.5 );
        _state.SetResult( CreateValue( y ));
    }
    
    LOCAL_TAG_HANDLER( XML::STRING )
    {        
        _state.SetResult( CreateValue( EvaluateTextUtf8( _node, _state ) ));
    }

    LOCAL_TAG_HANDLER( XML::BOOLEAN )
    {
        int y = ( EvaluateInteger( _node, _state ) != 0 );
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::INTEGER )
    {
        Attr< int, false, char > _value("value");

        int y = 0;
        
        if ( _node >> _value )
        {
            y = *_value;
        }
        else
        {
            y = EvaluateInteger( _node, _state );
        }
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::FLOAT )
    {
        Attr< int, false, char > _value("value");

        double y = 0;
        
        if ( _node >> _value )
        {
            y = *_value;
        }
        else
        {
            y = EvaluateFloat( _node, _state );
        }
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::ADD )
    {
        double y = 0;

        for ( Node child( _node.GetFirstChild() ); !!child; ++child )
        {
            _state.SetResult(0);            
            _state.Execute( child );

            if ( !IsEmpty( _state.GetResult() ))
            {
                Attr< double, true, wchar_t > x(L"value");            
                (*_state.GetResult()) >> x;
                y += (*x);
            }
        }
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::MULTIPLY )
    {
        double y = 1;

        for ( Node child( _node.GetFirstChild() ); !!child; ++child )
        {
            _state.SetResult(0);
            _state.Execute( child );

            if ( !IsEmpty( _state.GetResult() ))
            {
                Attr< double, true, wchar_t > x(L"value");            
                (*_state.GetResult()) >> x;
                y *= (*x);
            }
        }        
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::RECIPROCAL )
    {
        double y = 1.0 / EvaluateFloat( _node, _state );
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::MINUS )
    {
        double y = -EvaluateFloat( _node, _state );
        _state.SetResult( CreateValue( y ));
    }
    
    
    
    
    

    LOCAL_TAG_HANDLER( XML::COMPARE )
    {
        Node child( _node.GetFirstChild() );

        int x1 = 0;        
        int x2 = 0;
        double f1 = 0;
        double f2 = 0;
        TextUtf8 t1, t2;
        int T1=0, T2=0;

        IChunkPtr first, second;
        TextUtf8 firstType, secondType;

        if ( !!child )
        {
            first = EvaluateObject( child, _state );
            firstType = attribute_of< TextUtf8 >( first.get(), "type-name" );
            if ( L"string" == firstType )
            {
                t1 = value_of< TextUtf8 >( first.get() );
                T1 = 1;
            }
            else if ( L"int" == firstType )
            {
                x1 = value_of< int >( first.get() );
                T1 = 2;
            }
            else if ( L"float" == firstType )
            {
                f1 = value_of< double >( first.get() );
                T1 = 3;
            }
            ++child;
            if ( !!child )
            {
                second = EvaluateObject( child, _state );
                secondType = attribute_of< TextUtf8 >( second.get(), "type-name" );
                if ( L"string" == secondType )
                {
                    t2 = value_of< TextUtf8 >( second.get() );
                    T2 = 1;
                }
                else if ( L"int" == secondType )
                {
                    x2 = value_of< int >( second.get() );
                    T2 = 2;
                }
                else if ( L"float" == secondType )
                {
                    f2 = value_of< double >( second.get() );
                    T2 = 3;
                }
            }
        }

        int result;

        if (( 0 == T1 && 1 == T2 ) && 0 == T2 )
        {
            result = 0;
        }
        if ( 1 == T1 && ( 1 == T2 || 0 == T2 ))
        {            
            int result = (t1 > t2 ? 1 : (t1 < t2 ? -1 : 0));
        }
        else if ( 2 == T1 && ( 2 == T2 || 0 == T2 ))
        {
            int result = (x1 > x2 ? 1 : (x1 < x2 ? -1 : 0));
        }
        else if ( 3 == T1 && ( 3 == T2 || 0 == T2 ))
        {
            int result = (f1 > f2 ? 1 : (f1 < f2 ? -1 : 0));
        }
        else
        {
            throw Error("<compare> Both args must be of the same type: string, int or float.");
        }
        _state.SetResult( CreateValue( result ));
    }

    LOCAL_TAG_HANDLER( XML::LESS )
    {
        int y = ( EvaluateInteger( _node, _state ) < 0);
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::GREATER )
    {
        int y = ( EvaluateInteger( _node, _state ) > 0);
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::EQUAL )
    {
        int y = ( EvaluateInteger( _node, _state ) == 0);
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::LESS_EQUAL )
    {
        int y = ( EvaluateInteger( _node, _state ) <= 0);
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::GREATER_EQUAL )
    {
        int y = ( EvaluateInteger( _node, _state ) >= 0);
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::OR )
    {
        bool y = 0;

        for ( Node child( _node.GetFirstChild() ); !!child; ++child )
        {
            _state.SetResult(0);
            _state.Execute( child );

            if ( !IsEmpty( _state.GetResult() ))
            {
                Attr< int, true, wchar_t > x(L"value");            
                (*_state.GetResult()) >> x;
                y |= !!(*x);

                if ( true == y ) 
                { break; }
            }
        }
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::AND )
    {
        bool y = 1;

        for ( Node child( _node.GetFirstChild() ); !!child; ++child )
        {
            _state.SetResult(0);
            _state.Execute( child );

            if ( !IsEmpty( _state.GetResult() ))
            {
                Attr< int, true, wchar_t > x(L"value");            
                (*_state.GetResult()) >> x;
                y &= !!(*x);

                if ( false == y ) 
                { break; }
            }
        }
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::NOT )
    {
        bool y = !EvaluateInteger( _node, _state );
        _state.SetResult( CreateValue( y ));
    }

    LOCAL_TAG_HANDLER( XML::REPLACE )
    {
        throw Error("<replace> is not implemented yet");
        //Attr< TextUtf8 > in("in");
        //Attr< TextUtf8 > pattern("pattern");

        //GetAttr( in, _node, _state );
        //GetAttr( pattern, _node, _state );

        //ereg_replace( pattern, _node->_value, *in );
    }
    
    LOCAL_TAG_HANDLER( XML::CAT )
    {
        Attr< TextUtf8, false, wchar_t > _separator(L"separator");
        
        bool hasSeparator = GetAttr( _separator, _node, _state );

        TextUtf8 result;

        for ( Node child( _node.GetFirstChild() ); !!child; ++child )
        {
            _state.SetResult( 0 );
            _state.Execute( child );
            IChunk *pResult = _state.GetResult();
            
            if ( !IsEmpty( pResult ))
            {
                if ( type_of( pResult ) == L"list" )
                {
                    do {
                        if ( hasSeparator && !result.empty() )
                        {
                            result += _separator.str();
                        }
                        result += value_of< TextUtf8 >( pResult->GetChunk( FOURCC_LIST_HEAD ) );
                        pResult = pResult->GetChunk( FOURCC_LIST_TAIL );
                    } while ( !IsEmpty( pResult ));
                }
                else
                {
                    if ( hasSeparator && !result.empty() )
                    {
                        result += _separator.str();
                    }
                    result += value_of< TextUtf8 >( pResult );
                }
            }
        }
        _state.SetResult( CreateValue( result ));
    }


    LOCAL_TAG_HANDLER( XML::LIST )
    {
        Attr< TextUtf8, false, wchar_t > _extend(L"extend");
        
        IChunkPtr root = new XMLEmpty();
        IChunkPtr guard( root );
        XMLListPtr last = 0;

        if ( GetAttr( _extend, _node, _state ))
        {
            root = GetChunk( _extend.str(), _state );
        }

        for ( Node child( _node.GetFirstChild()); !!child; ++child )
        {
            _state.SetResult(0);
            _state.Execute( child );
            IChunk *pResult = _state.GetResult();            

            if ( !IsEmpty( pResult ))
            {
                XMLListPtr p( new XMLListChunk( pResult, guard.get() ));
                if ( IsEmpty( root.get() )) {
                    root = p.get();
                }
                else {
                    last->SetTail( p.get());
                }
                last = p;
            }
        }

        _state.SetResult( root.get() );
    }

    LOCAL_TAG_HANDLER( XML::MERGE )
    {
        IChunkPtr root = new XMLEmpty();
        IChunkPtr guard( root );
        XMLListPtr last = 0;

        for ( Node child( _node.GetFirstChild()); !!child; ++child )
        {
            _state.SetResult(0);
            _state.Execute( child );
            IChunk *pResult = _state.GetResult();            

            if ( !IsEmpty( pResult ))
            {
                XMLListPtr p;
                if ( type_of( pResult ) == L"list" )
                {
                    do {
                        p = new XMLListChunk( pResult->GetChunk( FOURCC_LIST_HEAD ), guard.get() );
                        if ( IsEmpty( root.get() )) {
                            root = p.get();
                        }
                        else {
                            last->SetTail( p.get());
                        }
                        last = p;
                        pResult = pResult->GetChunk( FOURCC_LIST_TAIL );
                    }
                    while ( !IsEmpty( pResult ));
                }
                else
                {
                    p = new XMLListChunk( pResult, guard.get() );
                    if ( IsEmpty( root.get() )) {
                        root = p.get();
                    }
                    else {
                        last->SetTail( p.get());
                    }
                    last = p;
                }
            }
        }
        _state.SetResult( root.get() );
    }

    LOCAL_TAG_HANDLER( XML::MAP )
    {
        Attr< TextUtf8, true, wchar_t > _iterator(L"iterator");
        Attr< TextUtf8, false, wchar_t >    _list(L"list");
        Attr< int, true, wchar_t >         _begin(L"begin");
        Attr< int, true, wchar_t >           _end(L"end");

        GetAttr( _iterator, _node, _state );
        
        IChunkPtr root = new XMLEmpty();
        IChunkPtr guard( root );
        XMLListPtr last = 0;
        
        boost::intrusive_ptr< XMLCodeBlock > newBlock = new XMLCodeBlock();
        ExecutionState state( _state );
        state._prevState = &_state;
        state._currentBlock = newBlock.get();

        if ( GetAttr( _list, _node, _state ))
        {
            IChunkPtr pChunk = GetChunk( _list.str(), _state );
                    
            while ( !IsEmpty( pChunk.get() ))
            {
                if ( type_of( pChunk.get() ) == L"list" )
                { 
                    state.SetResult(0);
                    newBlock->AddChunk( _iterator.str(), pChunk->GetChunk( FOURCC_LIST_HEAD ) );
                    state.ExecuteChildren( _node );
                    IChunk *pResult = state.GetResult();
                    newBlock->DropAll();

                    if ( !IsEmpty( pResult ))
                    {
                        XMLListPtr p = new XMLListChunk( pResult, guard.get() );
                        if ( IsEmpty( root.get() )) {
                            root = p.get();
                        }
                        else {
                            last->SetTail( p.get());
                        }
                        last = p;
                    }
                    pChunk = pChunk->GetChunk( FOURCC_LIST_TAIL );
                }
                else
                {
                    throw Error("map: Unexpected chunk in list:tail");
                }
            }
        }
        else
        {
            GetAttr( _begin, _node, _state );
            GetAttr( _end, _node, _state );

            for ( int i=(*_begin), I=(*_end); i<I; ++i )
            {                
                state.SetResult(0);
                newBlock->AddChunk( _iterator.str(), CreateValue( i ));
                state.ExecuteChildren( _node );
                IChunk *pResult = state.GetResult();
                newBlock->DropAll();

                if ( !IsEmpty( pResult ))
                {
                    XMLListPtr p = new XMLListChunk( pResult, guard.get() );
                    if ( IsEmpty( root.get() )) {
                        root = p.get();
                    }
                    else {
                        last->SetTail( p.get());
                    }
                    last = p;
                }
            }
        }
        
        _state.SetResult( root.get() );
    }

    LOCAL_TAG_HANDLER( XML::ZIP )
    {
        std::vector< XMLListPtr > lists;
        std::vector< XMLListChunk *> nodePtrs;

        for ( Node child( _node.GetFirstChild()); !!child; ++child )
        {
            _state.SetResult(0);
            _state.Execute( child );
            IChunk *pObject = _state.GetResult();
            if ( !IsEmpty( pObject ))
            {
                if ( XMLListChunk *pList = dynamic_cast< XMLListChunk *>( pObject ))
                {
                    lists.push_back( pList );
                    nodePtrs.push_back( pList );
                }
                else
                {
                    throw Error("zip: each component must be a list or empty");
                }
            }
        }

        if ( nodePtrs.empty() )
        {
            _state.SetResult( new XMLEmpty() );
            return;
        }
        
        IChunkPtr root = new XMLEmpty();
        IChunkPtr guard( root );
        XMLListPtr last = 0;

        while ( !IsEmpty( nodePtrs[0] ))
        {
            IChunkPtr ilast = new XMLEmpty();
            XMLListPtr ihand;

            for ( std::vector< XMLListChunk * >::reverse_iterator itLPtr = nodePtrs.rbegin();
                    itLPtr != nodePtrs.rend(); ++itLPtr )
            {
                ihand = new XMLListChunk( (*itLPtr)->_head.get(), ilast.get() );
                ilast = ihand;
                (*itLPtr) = dynamic_cast< XMLListChunk *>( (*itLPtr)->_tail.get() );
            }

            XMLListPtr p = new XMLListChunk( ilast.get(), guard.get() );            
            if ( IsEmpty( root.get() )) {
                root = p.get();
            }
            else {
                last->SetTail( p.get());
            }
            last = p;
        }

        _state.SetResult( root.get() );

    }

    LOCAL_TAG_HANDLER( XML::REVERSE )
    {
        IChunkPtr pChunk = EvaluateObject( _node, _state, EVOB_SEPARATE_BLOCK );
        IChunkPtr last = new XMLEmpty();
        
        if ( IsEmpty( pChunk.get() ))
        {
            _state.SetResult( new XMLEmpty() );
            return;
        }
        
        while ( !IsEmpty( pChunk.get() ))
        {
            if ( XMLListChunk *pList = dynamic_cast< XMLListChunk *>( pChunk.get() ))
            {
                last = new XMLListChunk( pList->_head.get(), last.get() );
                pChunk = pList->_tail;
            }
        }

        _state.SetResult( last.get() );
    }

    struct CompareIChunks
    {
        ExecutionState &_state;
        TextUtf8     _expression;

        CompareIChunks( ExecutionState &state, TextUtf8 test )
            : _state( state ), _expression( test ) {}

        bool operator () ( IChunk *first, IChunk *second )
        {
            IBlockPtr newBlock = new XMLCodeBlock();
            newBlock->AddChunk(L"first", first);
            newBlock->AddChunk(L"second", second);
            ExecutionState state( _state );
            state._prevState = &_state;
            state._currentBlock = newBlock.get();
            return EvaluateBoolean( EvaluateString( _expression, state ));
        }
    };


    template< class _SortBin > IChunkPtr _sort_list( IChunkPtr pObject, CompareIChunks &compare, bool ascending )
    {
        typedef _SortBin SortBin;
        IChunkPtr pChunk = pObject;

        SortBin sortedChunks( compare );
        
        if ( IsEmpty( pChunk.get() ))
        {
            return new XMLEmpty();
        }

        while ( !IsEmpty( pChunk.get() ))
        {
            if ( XMLListChunk *pList = dynamic_cast< XMLListChunk *>( pChunk.get() ))
            {
                sortedChunks.insert( pList->_head.get() );
                pChunk = pList->_tail;
            }
        }
        
        IChunkPtr last = new XMLEmpty();
        XMLListPtr hand;

        if ( 0 == ascending )
        {
            for ( SortBin::reverse_iterator it = sortedChunks.rbegin();
                    it != sortedChunks.rend(); ++it )
            {
                hand = new XMLListChunk( (*it), last.get() );
                last = hand;
            }
        }
        else
        {
            for ( SortBin::iterator it = sortedChunks.begin();
                    it != sortedChunks.end(); ++it )
            {
                hand = new XMLListChunk( (*it), last.get() );
                last = hand;
            }
        }
        
        return last;
    };


    LOCAL_TAG_HANDLER( XML::SORT )
    {
        //@TODO: char
        Attr< TextUtf8, false, char > _test("test");
        Attr< int, false, wchar_t > _ascending(L"ascending");
        Attr< int, false, wchar_t >  _distinct(L"distinct");

        if (!(_node >> _test))
        {
            _test.str("$(first) $(second) <");
        }

        IChunkPtr pObject = EvaluateObject( _node, _state, EVOB_SEPARATE_BLOCK );
        CompareIChunks compare( _state, _test.str() );
        
        typedef std::set< IChunk *, CompareIChunks > DistinctSortBin;
        typedef std::multiset< IChunk *, CompareIChunks > StableSortBin;
        
        if (!GetAttr( _ascending, _node, _state ))
        {
            _ascending = 1;
        }

        if ( !GetAttr( _distinct, _node, _state ) || 0 == (*_distinct))
        {
            _state.SetResult( _sort_list< StableSortBin >( pObject, compare, *_ascending ).get() );
        }
        else
        {
            _state.SetResult( _sort_list< DistinctSortBin >( pObject, compare, *_ascending ).get() );
        }
    }

    LOCAL_TAG_HANDLER( XML::SHUFFLE )
    {
        IChunkPtr pObject = EvaluateObject( _node, _state, EVOB_SEPARATE_BLOCK );
        IChunkPtr pChunk = pObject;
        
        if ( IsEmpty( pChunk.get() ))
        {
            _state.SetResult( new XMLEmpty() );
            return;
        }

        std::vector< IChunk *> chunks;
        
        while ( !IsEmpty( pChunk.get() ))
        {
            if ( XMLListChunk *pList = dynamic_cast< XMLListChunk *>( pChunk.get() ))
            {
                chunks.push_back( pList->_head.get() );
                pChunk = pList->_tail;
            }
        }

        Random::Shuffle( chunks.begin(), chunks.end() );

        IChunkPtr last;
        XMLListPtr hand;

        for ( std::vector< IChunk *>::iterator it = chunks.begin();
                it != chunks.end(); ++it )
        {
            hand = new XMLListChunk( (*it), last.get() );
            last = hand;
        }

        _state.SetResult( last.get() );
    }

    LOCAL_TAG_HANDLER( XML::SAMPLE )
    {
        IChunkPtr pObject = EvaluateObject( _node, _state, EVOB_SEPARATE_BLOCK );
        IChunkPtr pChunk = pObject;
        
        if ( IsEmpty( pChunk.get() ))
        {
            _state.SetResult( new XMLEmpty() );
            return;
        }
        
        std::vector< IChunk *> chunks;
        
        while ( !IsEmpty( pChunk.get() ))
        {
            if ( XMLListChunk *pList = dynamic_cast< XMLListChunk *>( pChunk.get() ))
            {
                chunks.push_back( pList->_head.get() );
                pChunk = pList->_tail;
            }
        }

        if ( chunks.size() > 1 )
        {
            int index = Random::SampleRange( 0, chunks.size()-1 );
            _state.SetResult( chunks[ index ] );
        }
        else
        {
            _state.SetResult( chunks[0] );
        }
    }

};//XMLFactory


IFiniteStateMachine & operator >> ( Node node, IFiniteStateMachine &fsm )
{
    fsm.GetCurrentExecutionState().ExecuteChildren( node );
    return fsm;
}






namespace XMLProgram {

    struct Operand
    {
        TextUtf8 _as_string;
        int      _as_int;
        double   _as_double;
        int      _type;

        Operand(): _type(0) {}

        Operand & set( const TextUtf8 & x )
        {
            _type = _type_of( x );
            if ( 1 == _type )
            {
                _as_string = x;
            }
            else if ( 2 == _type )
            {
                std::basic_stringstream< wchar_t > ss( x );
                ss >> _as_int;
            }
            else if ( 3 == _type )
            {
                std::basic_stringstream< wchar_t > ss( x );
                ss >> _as_double;
            }
            return *this;
        }

        Operand & set( int x )
        {
            _as_int = x;
            _type = 2;
            return *this;
        }

        Operand & set( double x )
        {
            _as_double = x;
            _type = 3;
            return *this;
        }
        
        void get( TextUtf8 &x )
        {
            if ( 0 == _type )
            {
                x = TextUtf8();
            }
            else if ( 1 == _type )
            {
                x = _as_string;
            }
            else if ( 2 == _type )
            {
                std::basic_stringstream< wchar_t > ss;
                ss << _as_int;
                ss >> x;
            }
            else if ( 3 == _type )
            {
                std::basic_stringstream< wchar_t > ss;
                ss << _as_double;
                ss >> x;
            }
        }

        void get( int &x )
        {
            if ( 0 == _type )
            {
                x = 0;
            }
            else if ( 1 == _type )
            {
                TextUtf8 le = g_lastExpression;
                TextUtf8 lv = g_lastExpressionValue;
                throw Error("Operand value cannot be converted to integer.\nLast Expression: '%s'\nExpression Values: '%s'", le.c_str(), lv.c_str());
            }
            else if ( 2 == _type )
            {
                x = _as_int;
            }
            else if ( 3 == _type )
            {
                x = _as_double;
            }
        }
        
        void get( double &x )
        {
            if ( 0 == _type )
            {
                x = 0;
            }
            else if ( 1 == _type )
            {
                TextUtf8 le = g_lastExpression;
                TextUtf8 lv = g_lastExpressionValue;
                throw Error("Operand value cannot be converted to double.\nLast Expression: '%s'\nExpression Values: '%s'", le.c_str(), lv.c_str());
            }
            else if ( 2 == _type )
            {
                x = _as_int;
            }
            else if ( 3 == _type )
            {
                x = _as_double;
            }
        }

        bool isNumber() const
        {
            return (2 & _type);
        }

        bool isFloat() const
        {
            return (3 == _type);
        }

        int _type_of( const TextUtf8 &x )
        {
            int type = 0;

            TextUtf8::const_iterator iter = x.begin();

            if ( x.end() == iter )
            {
                return 0;
            }

            int u = *iter;

            // number begins from  [0123456789-.]
            if ( '-' == u || ( '0' <= u && u <= '9' ))
            {
                type = 2;
                if ( -1 != x.find('.') ) { return 3; } else { return 2; }//(*)
            }

            // double may begin from '.'
            else if ( '.' == u )
            {
                return 3;//(*)
                type = 3;
            }

            // it is a string
            else
            {
                return 1;
            }

            // scan: number may contain only [0123456789] and double may contain only one '.'
            // (*) BUG: Expressions like 1.0e+10 are not supported.
            for( ++iter; x.end() != iter; ++iter )
            {
                u = (*iter);
                
                if ( '.' == u )
                {
                    if ( 3 == type )
                    {
                        return 1;
                    }
                    else
                    {
                        type = 3;
                    }
                }
                else if ( '0' <= u && u <= '9' )
                {
                }
                else
                {
                    return 1;
                }
            }
            return type;
        }

    };

    struct Operandor
    {
        std::vector< Operand > _operands;

        template< class _X > bool pop( _X &x )
        {
            if ( _operands.empty() )
            {
                return false;
            }
            else
            {
                _operands.back().get( x );
                _operands.pop_back();
                return true;
            }
        }

        void push( int x )
        {
            _operands.push_back( Operand().set( x ));
        }
        
        void push( double x )
        {
            _operands.push_back( Operand().set( x ));
        }

        Operand & get_operand( int index )
        {
            if ( index <= 0 )
            {
                index = _operands.size() + index;
            }
            
            if ( index < _operands.size() )
            {
                return _operands[ index ];
            }
            else
            {
                TextUtf8 le = g_lastExpression;
                TextUtf8 lv = g_lastExpressionValue;
                throw Error("In expression, too few operands.\nLast Expression: '%s'\nExpression Values: '%s'", le.c_str(), lv.c_str());
            }
        }

        template< class _X > struct Comparator
        {
            static void apply( Operandor &operandor, TextUtf8 x, _X a, _X b )
            {
                if ( x == L"<" || x == L"less" )
                {
                    operandor.push( a < b );
                }
                else if ( x == L"<=" || x == L"leq" )
                {
                    operandor.push( a <= b );
                }
                else if ( x == L">" || x == L"gt" )
                {
                    operandor.push( a > b );
                }
                else if ( x == L">=" || x == L"gteq" )
                {
                    operandor.push( a >= b );
                }
                else if ( x == L"=" || x == L"eqal" )
                {
                    operandor.push( a == b );
                }
                else if ( x == L"max" )
                {
                    operandor.push( std::max( a, b ));
                }
                else if ( x == L"min" )
                {
                    operandor.push( std::min( a, b ));
                }
            }            
        };

        template< class _X > struct NumericEvaluator
        {
            static void apply_unary( Operandor &operandor, TextUtf8 x )
            {
                _X z;
                operandor.pop( z );
                if ( x == L"minus" )
                {
                    operandor.push( -z );
                }
                else if ( x == L"not" )
                {
                    operandor.push( !z );
                }
                else if ( x == L"sin" )
                {
                    operandor.push( std::sin( (double)z ));
                }
                else if ( x == L"cos" )
                {
                    operandor.push( std::cos( (double)z ));
                }
                else if ( x == L"acos" )
                {
                    operandor.push( std::acos( (double)z ));
                }
                else if ( x == L"asin" )
                {
                    operandor.push( std::asin( (double)z ));
                }
                else if ( x == L"random_sample" )
                {
                    int k = 0;
                    if ( z > 1 ) { k = Random::SampleRange( 0, z-1 ); }
                    operandor.push( k );
                }
                else if ( x == L"floor" )
                {
                    operandor.push( std::floor( (double)z ));
                }
                else if ( x== L"ceil" )
                {
                    operandor.push( std::ceil( (double)z ));
                }
            }

            static void apply_binary( Operandor &operandor, TextUtf8 x )
            {
                _X a,b;
                operandor.pop(b);
                operandor.pop(a);

                if ( x == L"+" || x == L"add" )
                {
                    operandor.push(a+b);
                }
                else if ( x == L"-" || x == L"sub" )
                {
                    operandor.push(a-b);
                }
                else if ( x == L"*" || x == L"mul" )
                {
                    operandor.push(a*b);
                }
                else if ( x == L"/" || x == L"div" )
                {
                    operandor.push(a/b);
                }
                else if ( x == L"and" )
                {
                    operandor.push(a&&b);
                }
                else if ( x == L"or" )
                {
                    operandor.push(a||b);
                }
                else
                {
                    Comparator< _X >::apply( operandor, x, a, b );
                }
            }
        };

        void push( TextUtf8 x )
        {
            if ( x == L"minus" || x == L"not" || x == L"sin" || x == L"cos"
                    || x == L"asin" || x == L"acos" || x == L"random_sample" 
                    || x == L"floor" || x == L"ceil" )
            {
                if ( get_operand(-1).isNumber())
                {
                    if ( get_operand(-1).isFloat())
                    {
                        NumericEvaluator< double >::apply_unary( *this, x );
                    }
                    else
                    {
                        NumericEvaluator< int >::apply_unary( *this, x );
                    }
                }
            }
            else if ( x == L"<" || x == L">" || x == L"<=" || x == L">=" || x == L"=" ||
                      x == L"+" || x == L"-" || x == L"*" || x == L"/" || x == L"and" || x == L"or" ||
                      x == L"add" || x == L"sub" || x == L"mul" || x == L"div" ||
                      x == L"equal" || x == L"less" || x == L"gt" || x == L"leq" || x == L"gteq" ||
                      x == L"max" || x == L"min" )
            {
                if ( get_operand(-2).isNumber() && get_operand(-1).isNumber() )
                {
                    if ( get_operand(-2).isFloat() || get_operand(-1).isFloat() )
                    {
                        NumericEvaluator< double >::apply_binary( *this, x );
                    }
                    else
                    {
                        NumericEvaluator< int >::apply_binary( *this, x );
                    }
                }
                else
                {
                    TextUtf8 a,b;
                    pop(a);
                    pop(b);
                    if ( x == L"+" )
                    {
                        push( a + b );
                    }
                    else                    
                    {
                        Comparator< TextUtf8 >::apply( *this, x, a, b );
                    }
                }
            }
            else
            {
                _operands.push_back( Operand().set( x ));
            }
        }
    };
    

    void Operande( Operandor &sop, const TextUtf8 &expression )
    {
        std::basic_stringstream< wchar_t > tokenizer( expression );
        TextUtf8 token;

        while ( tokenizer >> token )
        {
            if ( '(' == token[0] )
            {
                TextUtf8 all = token;
                while ( ')' != token[ token.size() - 1] && tokenizer >> token )
                {
                    all += ' ';
                    all += token;
                }
                if ( ')' != token[ token.size() - 1] )
                {
                    throw Error("Bad expression: %s", TextUtf8( expression ).c_str() );
                }
                sop.push( all.substr(1, all.size() - 2));
            }
            else
            {
                sop.push( token );
            }
        }
    }

    bool EvaluateBoolean( TextUtf8 expression )
    {            
        Operandor sop;
        Operande( sop, expression );
        int result;
        sop.pop( result );
        return result;
    }
    
    int EvaluateInteger( TextUtf8 expression )
    {
        Operandor sop;
        Operande( sop, expression );
        int result;
        sop.pop( result );
        return result;
    }
    
    double EvaluateFloat( TextUtf8 expression )
    {
        Operandor sop;
        Operande( sop, expression );
        double result;
        sop.pop( result );
        return result;
    }

    static CXML::EvaluableValue< int > reg_evaluable_value_int(L"0");
    static CXML::EvaluableValue< double > reg_evaluable_value_float(L"0");

    namespace CXML {
        template< class _Type >
            bool EvaluableValue< _Type >::Execute( Node __unused__, ExecutionState &state )
            {
                TextUtf8 expression = EvaluateString( _expression, state );
                Operandor sop;
                Operande( sop, expression );
                _Type result;
                sop.pop( result );
                state.SetResult( CreateValue( result ));
                return true;
            }
        
        template<>
            bool EvaluableValue< TextUtf8 >::Execute( Node __unused__, ExecutionState &state )
            {
                TextUtf8 expression = EvaluateString( _expression, state );
                state.SetResult( CreateValue( expression ));
                return true;
            }
    };
};
