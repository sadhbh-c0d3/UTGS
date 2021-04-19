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

#include <koolib/xml/XMLKompiler.h>

namespace XMLFactory {
    
    using namespace XMLProgram;

    static USELESS_XML_TAG_DICT( xtdProgramCompile, ExecutionState,
            // symbols
            .AddReg< CXML::LET >("let")
            .AddReg< CXML::SET >("set")
            .AddReg< CXML::GET >("get")
            .AddReg< CXML::LOOKUP >("lookup")
            .AddReg< CXML::REGISTER >("register")
            .AddReg< CXML::IS_NOT_EMPTY >("is-not-empty")
            .AddReg< CXML::IS_DEFINED   >("is-defined")
            .AddReg< CXML::IS_EMPTY     >("is-empty")
            .AddReg< CXML::IS_NOT_DEFINED   >("is-not-defined")
            .AddReg< CXML::IS_DEFINED_EMPTY >("is-defined-empty")
            .AddReg< CXML::IS_DEFINED_NOT_EMPTY >("is-defined-not-empty")
            .AddReg< CXML::IS_NOT_DEFINED_OR_EMPTY >("is-not-defined-or-empty")
            .AddReg< CXML::TYPE_OF >("type-of")
            .AddReg< CXML::ERROR_CHUNK >("error")
            .AddReg< CXML::NOCOMPILE >("nocompile")

            // functions
            .AddReg< CXML::FUNCTION  >("function")
            .AddReg< CXML::CALL      >("call")
            .AddReg< CXML::LAZY_CALL >("lazy-call")
            
            // newer functions
            .AddReg< CXML::APPLY     >("apply")
            .AddReg< CXML::CURRY     >("curry")
            .AddReg< CXML::LISTNODE  >("listnode")

            // block
            .AddReg< CXML::BLOCK >("block")
            .AddReg< CXML::EXTRACT >("extract")
            .AddReg< CXML::DOIN >("doin")
            .AddReg< CXML::DO >("do")
            .AddReg< CXML::OBJECT >("object")

            // list operations
            .AddReg< CXML::EMPTY    >("empty")
            .AddReg< CXML::LIST_NODE>("list-node")
            .AddReg< CXML::LIST     >("list")
            .AddReg< CXML::MAP  >("map")
            .AddReg< CXML::FOLD >("fold")
            .AddReg< CXML::SEEK >("seek")
            .AddReg< CXML::HEAD >("head")
            .AddReg< CXML::RANGE    >("range")
            .AddReg< CXML::ZIP  >("zip")

            // accumulative operators
            .AddReg< CXML::ADD  >("add")
            .AddReg< CXML::MUL  >("mul")
            .AddReg< CXML::FADD >("fadd")
            .AddReg< CXML::FMUL >("fmul")
            .AddReg< CXML::AND  >("and")
            .AddReg< CXML::OR   >("or")
            .AddReg< CXML::CAT  >("cat")
            .AddReg< CXML::INC  >("inc")
            .AddReg< CXML::FINC >("finc")
           
            // comparison 
            .AddReg< CXML::COMPARE  >("compare")
            .AddReg< CXML::FCOMPARE >("fcompare")
            .AddReg< CXML::STRCMP   >("strcmp")
            .AddReg< CXML::PTRCMP   >("ptrcmp")
            .AddReg< CXML::TYPECMP  >("typecmp")
            .AddReg< CXML::LESS     >("less")
            .AddReg< CXML::LESS_STR >("less-str")
            .AddReg< CXML::LESS_PTR >("less-ptr")
            .AddReg< CXML::LESS_INT >("less-int")
            .AddReg< CXML::LESS_FLOAT >("less-float")
            .AddReg< CXML::LESS_TYPE >("less-type")
            .AddReg< CXML::GREATER  >("greater")
            .AddReg< CXML::GREATER_STR >("greater-str")
            .AddReg< CXML::GREATER_PTR >("greater-ptr")
            .AddReg< CXML::GREATER_INT >("greater-int")
            .AddReg< CXML::GREATER_FLOAT >("greater-float")
            .AddReg< CXML::GREATER_TYPE >("greater-type")
            .AddReg< CXML::EQUAL    >("equal")
            .AddReg< CXML::EQUAL_STR >("equal-str")
            .AddReg< CXML::EQUAL_PTR >("equal-ptr")
            .AddReg< CXML::EQUAL_INT >("equal-int")
            .AddReg< CXML::EQUAL_FLOAT >("equal-float")
            .AddReg< CXML::EQUAL_TYPE  >("equal-type")
            
            // unary operators
            .AddReg< CXML::NOT              >("not")
            .AddReg< CXML::MINUS< int >     >("minus")
            .AddReg< CXML::MINUS< double >  >("fminus")
            .AddReg< CXML::RECIPROCAL       >("reciprocal")

            // primitives
            .AddReg< CXML::BOOLEAN  >("boolean")
            .AddReg< CXML::INTEGER  >("integer")
            .AddReg< CXML::FLOAT    >("float")
            .AddReg< CXML::STRING   >("string")
            
            .AddReg< CXML::TYPE_OF_INTEGER >("type-of-integer")
            .AddReg< CXML::TYPE_OF_FLOAT  >("type-of-float")
            .AddReg< CXML::TYPE_OF_STRING >("type-of-string")
            .AddReg< CXML::TYPE_OF_BINARY >("type-of-binary")
            .AddReg< CXML::TYPE_OF_BLOCK >("type-of-block")
            .AddReg< CXML::TYPE_OF_LIST  >("type-of-list")
            .AddReg< CXML::TYPE_OF_EMPTY >("type-of-empty")
            .AddReg< CXML::TYPE_OF_LAZY  >("type-of-lazy")

            .AddReg< CXML::TRUE_VALUE >("true")
            .AddReg< CXML::FALSE_VALUE >("false")

            // just copy xml nodes
            .AddReg< CXML::COPY     >("copy")

            // conditional execution
            .AddReg< CXML::IF       >("if")
            .AddReg< CXML::CHOOSE   >("choose")
            );
};

namespace XMLProgram {

    using namespace XMLFactory;
    
    namespace XML
    {
        struct COMPILE {};
    };

    template< class _Type >
    _Type * HookErrors( Node node, _Type *pChunk )
    {
        if ( CXML::CXmlErrors *err = dynamic_cast< CXML::CXmlErrors *>( pChunk ))
        {
            err->_line = node->_line;
            err->_source = node->_source;
        }
        return pChunk;
    }
    
    IChunkPtr SelectExecutionModel( IChunk *pChunk )
    {
        if ( 1 == pChunk->GetFourCC( FOURCC_NAME_CONST ))
        {
            return pChunk ;
        }
        else if ( 1 == pChunk->GetFourCC( FOURCC_NAME_GET ))
        {
            return pChunk ;
        }
        else
        {
            return new CXML::EvalInNewScope( pChunk );
        }
    }

    void HookDefinition( Node node, ExecutionState &state )
    {
        Attr< TextUtf8, false, wchar_t > _id(L"id");
        if ( GetAttr( _id, node, state ))
        {
            IChunk *pChunk = state.GetResult();
            state.SetResult( HookErrors( node, new CXML::LET( _id.str(), SelectExecutionModel( pChunk ).get() )));
        }
    }

    IChunkPtr Compile( Node node, ExecutionState &state, XMLFactory::XMLTagDict< ExecutionState > *currentDict )
    {
        SubScope newState( state );
        newState._currentDict = currentDict;

        IChunkPtr pEnd = new XMLEmpty();
        IChunkPtr pHead = pEnd;
        XMLListChunk *pLast = 0;

        for ( Node child( node.GetFirstChild() ); !!child; ++child )
        {
            newState.Execute( child );
            if ( !IsEmpty( newState.GetResult() ))
            {
                XMLListChunk *p = new XMLListChunk( newState.GetResult(), pEnd.get() );
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
            newState.Reuse();
        }
        if ( IsEmpty( pHead.get() ))
        {
            return pEnd;
        }
        else if ( IsEmpty( pHead->GetChunk( FOURCC_LIST_TAIL )))
        {
            return pHead->GetChunk( FOURCC_LIST_HEAD );
        }
        else
        {
            return pHead;
        }
    }
    
    IChunkPtr Compile( Node node, ExecutionState &state )
    {
        return Compile( node, state, &xtdProgramCompile );
    }
    

    IChunkPtr CompileOneNode( Node node, ExecutionState &state )
    {
        SubScope newState( state );
        newState._currentDict = &xtdProgramCompile;
        newState.Execute( node );
        return newState.GetResult();
    }

    IChunkPtr CompileRightValue( Node node, ExecutionState &state )
    {
        Attr< TextUtf8, false, wchar_t > _select(L"select");
        Attr< TextUtf8, false, wchar_t > _force(L"force");
        if ( GetAttr( _select, node, state ))
        {
            return HookErrors( node, new CXML::GET( _select.str() ));
        }
        else if ( GetAttr( _force, node, state ))
        {
            return HookErrors( node, new CXML::CALL( _force.str(), new XMLEmpty() ));
        }
        else
        {
            return SelectExecutionModel( Compile( node, state ).get() ).get();
        }
    }
    
    IChunkPtr NoCompile( Node node, ExecutionState &state )
    {
        return new XMLCodeChunk( node );
    }

};//XMLProgram

namespace XMLFactory {

    using namespace XMLProgram;
    //---------------------------------------------------------

#define LOCAL_TAG_HANDLER( _name )  USELESS_XML_TAG_HANDLER( _name, ExecutionState )

    LOCAL_TAG_HANDLER( XML::COMPILE )
    {
        Attr< int, false, wchar_t > _run(L"run");
        if ( GetAttr( _run, _node, _state ) && 1 == *_run )
        {
            IChunkPtr compiled = Compile( _node, _state );
            compiled->Execute( Node(), _state );
        }
        else
        {
            _state.SetResult( Compile( _node, _state ).get() );
        }
    }

    LOCAL_TAG_HANDLER( CXML::NOCOMPILE )
    {
        Attr< int, false, wchar_t > _run(L"run");
        if ( GetAttr( _run, _node, _state ) && 1 == *_run )
        {
            _state.SetResult( NoCompile( _node, _state ).get() );
        }
        else
        {
            throw Useless::Error("<nocompile run=\"0\"> is not supported!");
        }
    }


    //---------------------------------------------------------
    //
    // Basic nodes.
    //

    /*
     * Define NEW symbol.
     *
     *  <let id="Name" [select="ChunkId"|force="ChunkId"]>[<!-- code -->]</let>
     */
    LOCAL_TAG_HANDLER( CXML::LET )
    {
        Attr< TextUtf8, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        _state.SetResult( HookErrors( _node, new CXML::LET( _id.str(), CompileRightValue( _node, _state ).get() )));
    }

    /*
     * Reset EXISTING symbol.
     *
     *  <set id="Name" [select="ChunkId"|force="ChunkId"]>[<!-- code -->]</let>
     */
    LOCAL_TAG_HANDLER( CXML::SET )
    {
        Attr< TextUtf8, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        _state.SetResult( HookErrors( _node, new CXML::SET( _id.str(), CompileRightValue( _node, _state ).get() )));
    }
    
    LOCAL_TAG_HANDLER( CXML::INC )
    {
        Attr< TextUtf8, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        _state.SetResult( HookErrors( _node, new CXML::INC( _id.str(), CompileRightValue( _node, _state ).get() )));
    }
    
    LOCAL_TAG_HANDLER( CXML::FINC )
    {
        Attr< TextUtf8, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        _state.SetResult( HookErrors( _node, new CXML::FINC( _id.str(), CompileRightValue( _node, _state ).get() )));
    }

    /*
     * Get EXISTING symbol.
     *
     *  <get id="Name"/>
     *
     */
    LOCAL_TAG_HANDLER( CXML::GET )
    {
        Attr< TextUtf8, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        _state.SetResult( HookErrors( _node, new CXML::GET( _id.str() )));
    }

    /*
     * Find symbol in EXISTING dict.
     *
     *  <lookup>
     *      <!-- let id="dict" -->
     *      <!-- let id="name" -->
     *  </lookup>
     */
    LOCAL_TAG_HANDLER( CXML::LOOKUP )
    {
        _state.SetResult( HookErrors( _node, new CXML::LOOKUP( Compile( _node, _state ).get() )));
        HookDefinition( _node, _state );
    }

    /*
     * Add symbol to EXISTING dict.
     *
     *  <register>
     *      <!-- let id="dict" -->
     *      <!-- let id="name" -->
     *      <!-- let id="value" -->
     *  </register>
     */
    LOCAL_TAG_HANDLER( CXML::REGISTER )
    {
        _state.SetResult( HookErrors( _node, new CXML::REGISTER( Compile( _node, _state ).get() )));
    }


    /*
     * Check if EXISTING symbol refers to <empty>.
     */
    LOCAL_TAG_HANDLER( CXML::IS_NOT_EMPTY )
    {
        Attr< TextUtf8, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        _state.SetResult( HookErrors( _node, new CXML::IS_NOT_EMPTY( _id.str() )));
    }

    /*
     * Check if symbol exists.
     */
    LOCAL_TAG_HANDLER( CXML::IS_DEFINED )
    {
        Attr< TextUtf8, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        _state.SetResult( HookErrors( _node, new CXML::IS_DEFINED( _id.str() )));
    }

    /*
     * Check if symbol is empty.
     */
    LOCAL_TAG_HANDLER( CXML::IS_EMPTY )
    {
        Attr< TextUtf8, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        _state.SetResult( HookErrors( _node, new CXML::NOT( new CXML::IS_NOT_EMPTY( _id.str() )) ));
    }
    
    /*
     * Check if symbol is not defied.
     */
    LOCAL_TAG_HANDLER( CXML::IS_NOT_DEFINED )
    {
        Attr< TextUtf8, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        _state.SetResult( HookErrors( _node, new CXML::NOT( new CXML::IS_DEFINED( _id.str() )) ));
    }
    
    /*
     * Check if symbol is defied and is it empty.
     */
    LOCAL_TAG_HANDLER( CXML::IS_DEFINED_EMPTY )
    {
        Attr< TextUtf8, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        boost::intrusive_ptr< CXML::ValueAccumulator< CXML::OpAND > > adder = new CXML::ValueAccumulator< CXML::OpAND >();
        adder->Add( new CXML::IS_DEFINED( _id.str() ));
        adder->Add( new CXML::NOT( new CXML::IS_NOT_EMPTY( _id.str() )));
        _state.SetResult( HookErrors( _node, adder.get() ));
    }
    
    /*
     * Check if symbol is defined and not empty.
     */
    LOCAL_TAG_HANDLER( CXML::IS_DEFINED_NOT_EMPTY )
    {
        Attr< TextUtf8, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        boost::intrusive_ptr< CXML::ValueAccumulator< CXML::OpAND > > adder = new CXML::ValueAccumulator< CXML::OpAND >();
        adder->Add( new CXML::IS_DEFINED( _id.str() ));
        adder->Add( new CXML::IS_NOT_EMPTY( _id.str() ));
        _state.SetResult( HookErrors( _node, adder.get() ));
    }

    /*
     * Check if symbol id not defined or is empty.
     */
    LOCAL_TAG_HANDLER( CXML::IS_NOT_DEFINED_OR_EMPTY )
    {
        Attr< TextUtf8, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        boost::intrusive_ptr< CXML::ValueAccumulator< CXML::OpOR > > adder = new CXML::ValueAccumulator< CXML::OpOR >();
        adder->Add( new CXML::NOT( new CXML::IS_DEFINED( _id.str() )));
        adder->Add( new CXML::NOT( new CXML::IS_NOT_EMPTY( _id.str() )));
        _state.SetResult( HookErrors( _node, adder.get() ));
    }

    /*
     * Get FCC type of object
     */
    LOCAL_TAG_HANDLER( CXML::TYPE_OF )
    {
        _state.SetResult( HookErrors( _node, new CXML::TYPE_OF( CompileRightValue( _node, _state ).get() )));
        HookDefinition( _node, _state );
    }

    /*
     * Throw error message.
     */
    LOCAL_TAG_HANDLER( CXML::ERROR_CHUNK )
    {
        if ( _node.HasChildren() )
        {
            _state.SetResult( HookErrors( _node, new CXML::ERROR_CHUNK( Compile( _node, _state ).get() )));
        }
        else
        {
            _state.SetResult( HookErrors( _node, new CXML::ERROR_CHUNK(
                            new CXML::EvaluableValue< TextUtf8 >( _node->_value )
                            )));
        }
    }

    /*
     * Define NEW symbol refering to executable chunk.
     *
     *  <function id="Name" parameters="Name Name .. Name">
     *      <!-- code -->
     *  </function>
     */
    LOCAL_TAG_HANDLER( CXML::FUNCTION )
    {
        Attr< TextUtf8, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        _state.SetResult( HookErrors( _node, new CXML::LET( _id.str(), new CXML::ConstantValue( Compile( _node, _state ).get() ))));
    }

    /*
     * Invoke chunk associated with EXISTING symbol in place where <call> occures.
     *
     *  <call id="Name">
     *      <!-- params -->
     *  </call>
     *
     *  (*)Note: <!-- params --> are defined in invocation scope.
     */
    LOCAL_TAG_HANDLER( CXML::CALL )
    {
        Attr< TextUtf8, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        CXML::CALL *pCall = new CXML::CALL( _id.str(), Compile( _node, _state ).get() );
        _state.SetResult( HookErrors( _node, pCall ));
    }
    
    /*
     * Invoke chunk associated with EXISTING symbol in place where <call> occures.
     *
     *  <apply id="Name" select="Name">
     *      <!-- params -->
     *  </apply>
     *
     *  (*)Note: <!-- params --> are defined in invocation scope.
     */
    LOCAL_TAG_HANDLER( CXML::APPLY )
    {
        Attr< TextUtf8, true, wchar_t > _select(L"select");
        GetAttr( _select, _node, _state );
        CXML::CALL *pCall = new CXML::CALL( _select.str(), Compile( _node, _state ).get() );
        _state.SetResult( HookErrors( _node, pCall ));
        HookDefinition( _node, _state );
    }

    /*
     * Postpone invocation of chunk associated with EXISTING symbol.
     *
     *  <lazy-call id="Name">
     *      <!-- params -->
     *  </lazy-call>
     *
     * (*)Note: <!-- params --> are evaluated in place where <lazy-call> occures.
     */
    LOCAL_TAG_HANDLER( CXML::LAZY_CALL )
    {
        Attr< TextUtf8, true, wchar_t > _id(L"id");
        GetAttr( _id, _node, _state );
        _state.SetResult( HookErrors( _node, new CXML::LAZY_CALL( _id.str(), Compile( _node, _state ).get() )));
    }

    /*
     * Postpone invocation of chunk associated with EXISTING symbol.
     *
     *  <curry id="Name" select="Name">
     *      <!-- params -->
     *  </curry>
     *
     * (*)Note: <!-- params --> are evaluated in place where <curry> occures.
     */
    LOCAL_TAG_HANDLER( CXML::CURRY )
    {
        Attr< TextUtf8, true, wchar_t > _select(L"select");
        GetAttr( _select, _node, _state );
        _state.SetResult( HookErrors( _node, new CXML::LAZY_CALL( _select.str(), Compile( _node, _state ).get() )));
        HookDefinition( _node, _state );
    }


    /*
     * Create block.
     *
     *  <block [id="Name"]>
     *      <!-- let -->
     *      <!-- let -->
     *      ..
     *      <!-- let -->
     *  </block>
     */
    LOCAL_TAG_HANDLER( CXML::BLOCK )
    {
        _state.SetResult( new CXML::BLOCK( Compile( _node, _state ).get() ));
        HookDefinition( _node, _state );
    }

    /*
     * Extract all symbols into curernt block.
     *
     *  <extract select="Name"/>
     */
    LOCAL_TAG_HANDLER( CXML::EXTRACT )
    {
        Attr< TextUtf8, true, wchar_t > _select(L"select");
        GetAttr( _select, _node, _state );
        _state.SetResult( HookErrors( _node, new CXML::EXTRACT( _select.str() )));
    }

    /* 
     * Execute <!-- code --> in block ChunkId.
     *
     *  <doin select="ChunkId" [blockResult="Boolean"]>
     *      <!-- code -->
     *  </doin>
     *
     * Execute <!-- code --> in block Force( ChunkId ).
     *
     *  <doin force="ChunkId" [blockResult="Boolean"]>
     *      <!-- code -->
     *  </doin>
     *
     * Execute <!-- code --> in Force( <!-- getter --> ).
     *
     *  <doin [blockResult="Boolean"]>
     *      <!-- getter -->
     *      <!-- code -->
     *  </doin>
     *
     * (*)Note: result is block if 'blockResult' = 1 or code result 
     *
     */
    LOCAL_TAG_HANDLER( CXML::DOIN )
    {
        Attr< TextUtf8, false, char > _select("select");
        Attr< TextUtf8, false, char > _force("force");
        Attr< int, false, char >      _blockResult("blockResult");

        bool blockResult = (_node >> _blockResult) && *_blockResult;
        
        if ( _node >> _select )
        {
            _state.SetResult( HookErrors( _node,
                        new CXML::DOIN(
                            new CXML::GET( _select.str() ),
                            Compile( _node, _state ).get(),
                            blockResult )));
        }
        else if ( _node >> _force )
        {
            _state.SetResult( HookErrors( _node,
                        new CXML::DOIN(
                            new CXML::LazyCall( new CXML::GET( _select.str() )),
                            Compile( _node, _state ).get(),
                            blockResult )));
        }
        else
        {
            IChunkPtr pInner = Compile( _node, _state );
            IChunkPtr pCond = pInner->GetChunk( FOURCC_LIST_HEAD );
            IChunkPtr pFunc = pInner->GetChunk( FOURCC_LIST_TAIL );
            _state.SetResult( HookErrors( _node,
                        new CXML::DOIN( pCond.get(), pFunc.get(), blockResult )));
        }
        HookDefinition( _node, _state );
    }

    /*
     * Execute <!-- code --> in new block
     *
     *  <do>
     *      <!-- code -->
     *  </do>
     *
     */
    LOCAL_TAG_HANDLER( CXML::DO )
    {
        _state.SetResult( HookErrors( _node, CompileRightValue( _node, _state ).get() ));
        HookDefinition( _node, _state );
    }

    
    /*
     * Create object.
     *
     *  <object [id="Name"]>
     *      <!-- let -->
     *      <!-- let -->
     *      ..
     *      <!-- let -->
     *      <!-- function -->
     *      <!-- method -->
     *  </object>
     */
    LOCAL_TAG_HANDLER( CXML::OBJECT )
    {
        IChunkPtr pPrivate = Compile( _node, _state );
        IChunkPtr pMethods = CreateEmpty();
        {
            IChunkPtr pEnd = CreateEmpty();
            IChunkPtr pHead = pEnd;
            XMLListChunk *pLast = 0;

            for ( Node child( _node.GetFirstChild() ); !!child; ++child )
            {
                if ( child->_name == "function" )
                {
                    Attr< TextUtf8, true, wchar_t > _id(L"id");
                    GetAttr( _id, child, _state );
                    IChunkPtr pResult = HookErrors( child,
                            new CXML::LET( _id.str(), new CXML::LAZY_CALL( L"__private__:" + _id.str(),
                                    new CXML::LET( L"__self__", new CXML::GET( L"__private__" ))
                                    ))
                            );
                    XMLListChunk *p = new XMLListChunk( pResult.get(), pEnd.get() );
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
            }
            if ( IsEmpty( pHead.get() ))
            {
                pMethods = pEnd;
            }
            else if ( IsEmpty( pHead->GetChunk( FOURCC_LIST_TAIL )))
            {
                pMethods = pHead->GetChunk( FOURCC_LIST_HEAD );
            }
            else
            {
                pMethods = pHead;
            }
        }
        Attr< TextUtf8, false, wchar_t > _extend(L"extend");
        if ( !GetAttr( _extend, _node, _state ))
        {
            _state.SetResult( new CXML::OBJECT(  pPrivate.get(), pMethods.get() ));
        }
        else
        {
            _state.SetResult( new CXML::OBJECT(  pPrivate.get(), pMethods.get(), _extend.str() ));
        }
        HookDefinition( _node, _state );
    }
    

    //---------------------------------------------------------
    //
    // Accumulatives operations
    //

    template< class _OpType >
        struct AccumHandler
        {
            static IChunkPtr Create( Node _node, ExecutionState &_state )
            {
                IChunkPtr pChunk = Compile( _node, _state );
                boost::intrusive_ptr< CXML::ValueAccumulator< _OpType > > adder = new CXML::ValueAccumulator< _OpType >();
                while ( !IsEmpty( pChunk.get()))
                {
                    adder->Add( pChunk->GetChunk( FOURCC_LIST_HEAD ) );
                    pChunk = pChunk->GetChunk( FOURCC_LIST_TAIL );
                }
                return adder.get();
            }
        };

#define CXML_ACCUM_HANDLER( _Name, _OpType ) \
    LOCAL_TAG_HANDLER( _Name ) \
    { \
        IChunkPtr adder = AccumHandler< _OpType >::Create( _node, _state ); \
        _state.SetResult( HookErrors( _node, adder.get() )); \
        HookDefinition( _node, _state ); \
    }
    
    // <add><!-- integer --></add>
    CXML_ACCUM_HANDLER( CXML::ADD, CXML::OpADD< int > );

    // <mul><!-- integer --></mul>
    CXML_ACCUM_HANDLER( CXML::MUL, CXML::OpMUL< int > );

    // <fadd><!-- float --></fadd>
    CXML_ACCUM_HANDLER( CXML::FADD, CXML::OpADD< double > );

    // <fmul><!-- float --></fmul>
    CXML_ACCUM_HANDLER( CXML::FMUL, CXML::OpMUL< double > );

    // <and><!-- boolean --></and>
    CXML_ACCUM_HANDLER( CXML::AND, CXML::OpAND );

    // <or><!-- boolean --></or>
    CXML_ACCUM_HANDLER( CXML::OR, CXML::OpOR );

    // <compare><!-- integer --></compare>
    CXML_ACCUM_HANDLER( CXML::COMPARE, CXML::OpCOMPARE< int > );

    // <fcompare><!-- float --></fcompare>
    CXML_ACCUM_HANDLER( CXML::FCOMPARE, CXML::OpCOMPARE< double > );

    // <strcmp><!-- string --></strcmp>
    CXML_ACCUM_HANDLER( CXML::STRCMP, CXML::OpCOMPARE< TextUtf8 > );
    
    // <typecmp><!-- type --></typecmp>
    CXML_ACCUM_HANDLER( CXML::TYPECMP, CXML::OpCOMPARE< int > );

    // <cat><!-- string --></cat>
    LOCAL_TAG_HANDLER( CXML::CAT ) 
    { 
        Attr< TextUtf8, false, wchar_t > _separator(L"separator");
        GetAttr( _separator, _node, _state );
        IChunkPtr pChunk = Compile( _node, _state ); 
        boost::intrusive_ptr< CXML::ValueAccumulator< CXML::OpCAT > > 
            adder = new CXML::ValueAccumulator< CXML::OpCAT >( _separator.str() ); 
        while ( !IsEmpty( pChunk.get())) 
        { 
            adder->Add( pChunk->GetChunk( FOURCC_LIST_HEAD ) ); 
            pChunk = pChunk->GetChunk( FOURCC_LIST_TAIL ); 
        } 
        _state.SetResult( HookErrors( _node, adder.get() )); 
        HookDefinition( _node, _state ); 
    }

    struct PtrComparingHandler
    {
        static IChunkPtr Create( Node _node, ExecutionState &_state )
        {
            IChunkPtr ptr[2];
            int i=0;
            for ( Node child(_node.GetFirstChild()); !!child && i < 2; ++child, ++i )
            {
                ptr[i] = CompileOneNode( child, _state );
            }
            return new CXML::PTRCMP( ptr[0].get(), ptr[1].get() );
        }
    };

    // <ptrcmp><!-- pointer --></ptrcmp>
    LOCAL_TAG_HANDLER( CXML::PTRCMP )
    {
		_state.SetResult( PtrComparingHandler::Create( _node, _state ).get() );
        HookDefinition( _node, _state );
    }

    // <less><!-- compare --></less>
    LOCAL_TAG_HANDLER( CXML::LESS )
    {
        IChunkPtr compiled = CompileRightValue( _node, _state );
        if ( 0 != compiled->GetChunk( FOURCC_LIST_TAIL ) || IsEmpty( compiled.get() ))
        {
            throw Useless::Error("<less> expects exactly one child.\n"
                    "The child(or select/force) must return the value in set of { -1, 0, 1}.\n"
                    "-1 is 'less', 0 is 'equal' and 1 is 'greater'."
                    "You've probably forgotten <compare> node.\n");
        }
        _state.SetResult( new CXML::LESS( compiled.get() ));
        HookDefinition( _node, _state );
    }

    // <greater><!-- compare --></greater>
    LOCAL_TAG_HANDLER( CXML::GREATER )
    {
        IChunkPtr compiled = CompileRightValue( _node, _state );
        if ( 0 != compiled->GetChunk( FOURCC_LIST_TAIL ) || IsEmpty( compiled.get() ))
        {
            throw Useless::Error("<greater> expects exactly one child.\n"
                    "The child(or select/force) must return the value in set of { -1, 0, 1}.\n"
                    "-1 is 'less', 0 is 'equal' and 1 is 'greater'."
                    "You've probably forgotten <compare> node.\n");
        }
        _state.SetResult( new CXML::GREATER( compiled.get() ));
        HookDefinition( _node, _state );
    }

    // <equal><!-- compare --></equal>
    LOCAL_TAG_HANDLER( CXML::EQUAL )
    {
        IChunkPtr compiled = CompileRightValue( _node, _state );
        if ( 0 != compiled->GetChunk( FOURCC_LIST_TAIL ) || IsEmpty( compiled.get() ))
        {
            throw Useless::Error("<equal> expects exactly one child.\n"
                    "The child(or select/force) must return the value in set of { -1, 0, 1}.\n"
                    "-1 is 'less', 0 is 'equal' and 1 is 'greater'."
                    "You've probably forgotten <compare> node.\n");
        }
        _state.SetResult( new CXML::EQUAL( compiled.get() ));
        HookDefinition( _node, _state );
    }

#define CXML_COMPARING_HANDLER( _Name, _Comp, _OpType ) \
    LOCAL_TAG_HANDLER( _Name ) \
    {\
        IChunkPtr compiled = AccumHandler< _OpType >::Create( _node, _state );\
        _state.SetResult( new _Comp( compiled.get() )); \
        HookDefinition( _node, _state ); \
    }

    CXML_COMPARING_HANDLER( CXML::EQUAL_STR, CXML::EQUAL, CXML::OpCOMPARE< TextUtf8 > );
    CXML_COMPARING_HANDLER( CXML::EQUAL_INT, CXML::EQUAL, CXML::OpCOMPARE< int > );
    CXML_COMPARING_HANDLER( CXML::EQUAL_FLOAT, CXML::EQUAL, CXML::OpCOMPARE< double > );
    CXML_COMPARING_HANDLER( CXML::EQUAL_TYPE, CXML::EQUAL, CXML::OpCOMPARE< int > );
    
    CXML_COMPARING_HANDLER( CXML::LESS_STR, CXML::LESS, CXML::OpCOMPARE< TextUtf8 > );
    CXML_COMPARING_HANDLER( CXML::LESS_INT, CXML::LESS, CXML::OpCOMPARE< int > );
    CXML_COMPARING_HANDLER( CXML::LESS_FLOAT, CXML::LESS, CXML::OpCOMPARE< double > );
    CXML_COMPARING_HANDLER( CXML::LESS_TYPE, CXML::LESS, CXML::OpCOMPARE< int > );
    
    CXML_COMPARING_HANDLER( CXML::GREATER_STR, CXML::GREATER, CXML::OpCOMPARE< TextUtf8 > );
    CXML_COMPARING_HANDLER( CXML::GREATER_INT, CXML::GREATER, CXML::OpCOMPARE< int > );
    CXML_COMPARING_HANDLER( CXML::GREATER_FLOAT, CXML::GREATER, CXML::OpCOMPARE< double > );
    CXML_COMPARING_HANDLER( CXML::GREATER_TYPE, CXML::GREATER, CXML::OpCOMPARE< int > );
    
    LOCAL_TAG_HANDLER( CXML::EQUAL_PTR )
    {
		_state.SetResult( new CXML::EQUAL( PtrComparingHandler::Create( _node, _state ).get() ));
        HookDefinition( _node, _state );
    }
    
    LOCAL_TAG_HANDLER( CXML::LESS_PTR )
    {
		_state.SetResult( new CXML::LESS( PtrComparingHandler::Create( _node, _state ).get() ));
        HookDefinition( _node, _state );
    }
    
    LOCAL_TAG_HANDLER( CXML::GREATER_PTR )
    {
		_state.SetResult( new CXML::GREATER( PtrComparingHandler::Create( _node, _state ).get() ));
        HookDefinition( _node, _state );
    }

    // <not><!-- boolean --></not>
    LOCAL_TAG_HANDLER( CXML::NOT )
    {
        _state.SetResult( new CXML::NOT( CompileRightValue( _node, _state ).get() ));
        HookDefinition( _node, _state );
    }

    // <minus><!-- integer --></minus>
    LOCAL_TAG_HANDLER( CXML::MINUS< int > )
    {
        _state.SetResult( new CXML::MINUS< int >( CompileRightValue( _node, _state ).get() ));
        HookDefinition( _node, _state );
    }

    // <fminus><!-- float --></fminus>
    LOCAL_TAG_HANDLER( CXML::MINUS< double > )
    {
        _state.SetResult( new CXML::MINUS< double >( CompileRightValue( _node, _state ).get() ));
        HookDefinition( _node, _state );
    }

    // <reciprocal><!-- float --></reciprocal>
    LOCAL_TAG_HANDLER( CXML::RECIPROCAL )
    {
        _state.SetResult( new CXML::RECIPROCAL( CompileRightValue( _node, _state ).get() ));
        HookDefinition( _node, _state );
    }


    //---------------------------------------------------------
    //
    // Primitive types
    //

    template< class _Type >
        struct ValueHandler
        {
            static IChunkPtr Create( Node _node, ExecutionState &_state )
            {
                Attr< TextUtf8, false, wchar_t > _select(L"select");
                Attr< TextUtf8, false, wchar_t > _force (L"force");
                Attr< _Type, false, wchar_t >    _value (L"value");
                
                if ( GetAttr( _value, _node, _state )) 
                { 
                    return new CXML::ConstantValue( CreateValue( *_value ) );
                } 
                else if ( GetAttr( _select, _node, _state ))
                {
                    return new CXML::ValueConvertion< _Type >( new CXML::GET( _select.str() ));
                }
                else if ( GetAttr( _force, _node, _state ))
                {
                    return new CXML::ValueConvertion< _Type >( new CXML::LazyCall( new CXML::GET( _force.str() )));
                }
                else if ( _node.HasChildren() )
                {
                    return new CXML::ValueConvertion< _Type >( Compile( _node, _state ).get() );
                }
                else
                {
                    return new CXML::EvaluableValue< _Type >( _node->_value );
                }
            }
        };

#define CXML_VALUE_HANDLER( _Name, _Type ) \
    LOCAL_TAG_HANDLER( _Name ) \
    { \
        _state.SetResult( ValueHandler< _Type >::Create( _node, _state ).get() ); \
        HookDefinition( _node, _state );\
    }

    /*
     *  <boolean|integer|float|string [id="Name"] [value="ConstValue"|select="ChunkId"|force="ChunkId"]>[Expression]</->
     */
    CXML_VALUE_HANDLER( CXML::BOOLEAN, int );
    CXML_VALUE_HANDLER( CXML::INTEGER, int );
    CXML_VALUE_HANDLER( CXML::FLOAT, double );
    CXML_VALUE_HANDLER( CXML::STRING, TextUtf8 );


    LOCAL_TAG_HANDLER( CXML::TYPE_OF_INTEGER )
    {
        _state.SetResult( new CXML::ConstantValue( CreateValue( (int)FOURCC_TYPE_INT ) ));
        HookDefinition( _node, _state );
    }
    
    LOCAL_TAG_HANDLER( CXML::TYPE_OF_FLOAT )
    {
        _state.SetResult( new CXML::ConstantValue( CreateValue( (int)FOURCC_TYPE_FLOAT ) ));
        HookDefinition( _node, _state );
    }
    
    LOCAL_TAG_HANDLER( CXML::TYPE_OF_STRING )
    {
        _state.SetResult( new CXML::ConstantValue( CreateValue( (int)FOURCC_TYPE_TEXT ) ));
        HookDefinition( _node, _state );
    }
    
    LOCAL_TAG_HANDLER( CXML::TYPE_OF_BINARY )
    {
        _state.SetResult( new CXML::ConstantValue( CreateValue( (int)FOURCC_TYPE_STRING ) ));
        HookDefinition( _node, _state );
    }
    
    LOCAL_TAG_HANDLER( CXML::TYPE_OF_BLOCK )
    {
        _state.SetResult( new CXML::ConstantValue( CreateValue( (int)FOURCC_TYPE_BLOCK ) ));
        HookDefinition( _node, _state );
    }
    
    LOCAL_TAG_HANDLER( CXML::TYPE_OF_LIST )
    {
        _state.SetResult( new CXML::ConstantValue( CreateValue( (int)FOURCC_TYPE_LIST ) ));
        HookDefinition( _node, _state );
    }
    
    LOCAL_TAG_HANDLER( CXML::TYPE_OF_EMPTY )
    {
        _state.SetResult( new CXML::ConstantValue( CreateValue( (int)FOURCC_TYPE_EMPTY ) ));
        HookDefinition( _node, _state );
    }
    
    LOCAL_TAG_HANDLER( CXML::TYPE_OF_LAZY )
    {
        _state.SetResult( new CXML::ConstantValue( CreateValue( (int)FOURCC_TYPE_LAZY ) ));
        HookDefinition( _node, _state );
    }

    LOCAL_TAG_HANDLER( CXML::TRUE_VALUE )
    {
        _state.SetResult( new CXML::ConstantValue( CreateValue( (int)true ) ));
        HookDefinition( _node, _state );
    }

    LOCAL_TAG_HANDLER( CXML::FALSE_VALUE )
    {
        _state.SetResult( new CXML::ConstantValue( CreateValue( (int)false ) ));
        HookDefinition( _node, _state );
    }


    /*
     *  <copy>
     *      <!-- any xml nodes -->
     *  </copy>
     */
    LOCAL_TAG_HANDLER( CXML::COPY )
    {
        _state.SetResult( HookErrors( _node, new CXML::COPY( _node )));
        HookDefinition( _node, _state );
    }


    //---------------------------------------------------------
    //
    // List operations
    //

    /*
     * Create <empty>
     */
    LOCAL_TAG_HANDLER( CXML::EMPTY )
    {
        _state.SetResult( new CXML::ConstantValue( new XMLEmpty() ));
        HookDefinition( _node, _state );
    }

    /*
     * Create one list-node.
     *
     *  <list-node>
     *      <head>
     *          <!-- code -->
     *      </head>
     *      <tail>
     *          <!-- tail-code -->
     *      </tail>
     *  </list-node>
     *
     *  (*)Note: The result of <!-- tail-code --> is list node or function returning list node.
     */
    LOCAL_TAG_HANDLER( CXML::LIST_NODE )
    {
        IChunkPtr pHead, pTail;
        for ( Node child( _node.GetFirstChild() ); !!child; ++child )
        {
            if ( child->_name == "head" )
            {
                if ( pHead.get() ) { throw Useless::Error("<list-node> Expects exactly one 'head'"); }
                pHead = CompileRightValue( child, _state );
            }
            else if ( child->_name == "tail" )
            {
                if ( pTail.get() ) { throw Useless::Error("<list-node> Expects exactly one 'tail'"); }
                pTail = CompileRightValue( child, _state );
            }
        }
        _state.SetResult( HookErrors( _node, new CXML::LIST_NODE( pHead.get(), pTail.get() )));
    }

    /*
     * Create one list-node.
     *
     *  <listnode>
     *      <!-- let id="head" -->
     *      <!-- let id="tail" -->
     *  </listnode>
     *
     *  (*)Note: The result of <!-- tail --> is list node or function returning list node.
     */
    LOCAL_TAG_HANDLER( CXML::LISTNODE )
    {
        _state.SetResult( HookErrors( _node, new CXML::LISTNODE( Compile( _node, _state ).get() )));
        HookDefinition( _node, _state );
    }

    /*
     * Create unnamed list:
     *
     *  <list>
     *      <!-- code -->
     *      <!-- code -->
     *      ..
     *      <!-- code -->
     *  </list>
     *
     * Create list with name "$(id)".
     *
     *  <list id="Name">
     *      <!-- code -->
     *      <!-- code -->
     *      ..
     *      <!-- code -->
     *  </list>
     */
    LOCAL_TAG_HANDLER( CXML::LIST )
    {
        CXML::LIST_NODE *last = 0;
        IChunkPtr guard = new CXML::ConstantValue( new XMLEmpty() );
        IChunkPtr root = guard;
        for ( Node child( _node.GetFirstChild() ); !!child; ++child )
        {
            CXML::LIST_NODE *p = HookErrors( child, new CXML::LIST_NODE( CompileOneNode( child, _state ).get(), guard.get() ));
            if ( 0 == last )
            {
                root = p;
            }
            else
            {
                last->_pTail = p;
            }
            last = p;
        }
        _state.SetResult( HookErrors( _node, root.get() ));
        HookDefinition( _node, _state );
    }
    /*
     * Create a list from values returned by applying "func" for each element of "list".
     *
     *  <map id="Name" iterator="Name">
     *      <!-- let id="list" -->
     *      <!-- let id="func" -->
     *  </map>
     *
     */
    LOCAL_TAG_HANDLER( CXML::MAP )
    {
        Node child( _node.GetFirstChild() );
        if ( !!child )
        {
            IChunkPtr pFunctor;
            Attr< TextUtf8, false, wchar_t > _select(L"select");
            
            if ( "iterator" == child->_name )
            {
                Attr< TextUtf8, true > _iteratorId("id");
                if ( !(child >> _iteratorId))
                {
                    throw Useless::Error("<map><iterator> required 'id' attribute.");
                }
                ++child;
                IChunkPtr listDef;
                if ( "list" == child->_name )
                {
                    listDef = new CXML::LET( L"list", CompileRightValue( child, _state ).get() );
                }
                else if ( "range" == child->_name )
                {
                    listDef = new CXML::LET( L"list", CompileOneNode( child, _state ).get() );
                }
                else
                {
                    throw Useless::Error("<map> expected <list> or <range>.");
                }
                if ( !child )
                {
                    throw Useless::Error("<map> requires function body.");
                }
                IChunkPtr pFunc;
                if ( "function" == child->_name )
                {
                    pFunc = CompileRightValue( child, _state );
                }
                else
                {
                    std::list< IChunkPtr > defs;
                    for ( ++child; !!child; ++child )
                    {
                        defs.push_back( CompileOneNode( child, _state ));
                    }
                    pFunc = CreateEmpty();
                    for ( std::list< IChunkPtr >::reverse_iterator i = defs.rbegin(); i != defs.rend(); ++i )
                    {
                        pFunc = CreateList( (*i).get(), pFunc.get() );
                    }
                }
                IChunkPtr funcDef = new CXML::LET( L"func", new CXML::ConstantValue( pFunc.get() ));
                pFunctor = new CXML::MAP( _iteratorId.str(), CreateList( listDef.get(), CreateList( funcDef.get(), CreateEmpty() )) );
            }
            else
            {
                Attr< TextUtf8, true, wchar_t > _iterator(L"iterator");
                GetAttr( _iterator, _node, _state );
                pFunctor = new CXML::MAP( _iterator.str(), Compile( _node, _state ).get() );
            }
            _state.SetResult( HookErrors( _node, pFunctor.get() ));
            HookDefinition( _node, _state );
        }
        else
        {
            throw Useless::Error("<map> expects children nodes {list, function}.");
        }
    }

    /*
     * For each element of "list" apply function "func", with accumulator equal to "accum".
     * Chunk "func" must accept parameters "$(iterator)" and "$(accumulator)".
     *
     *  <fold iterator="Name" accumulator="Name">
     *      <!-- let id="list" -->
     *      <!-- let id="func" -->
     *      <!-- let id="accum" -->
     *  </fold>
     *
     */
    LOCAL_TAG_HANDLER( CXML::FOLD )
    {
        Node child( _node.GetFirstChild() );
        if ( !!child )
        {
            IChunkPtr pFunctor;
            Attr< TextUtf8, false, wchar_t > _select(L"select");
            
            if ( "iterator" == child->_name )
            {
                Attr< TextUtf8, true > _iteratorId("id");
                if ( !(child >> _iteratorId))
                {
                    throw Useless::Error("<fold><iterator> required 'id' attribute.");
                }
                ++child;
                if ( "accumulator" != child->_name )
                {
                    throw Useless::Error("<fold> expected <accumulator>.");
                }
                Attr< TextUtf8, true > _accumulatorId("id");
                if ( !(child >> _accumulatorId))
                {
                    throw Useless::Error("<fold><accumulator> required 'id' attribute.");
                }
                IChunkPtr accumDef = new CXML::LET( _accumulatorId.str(), CompileRightValue( child, _state ).get());
                ++child;
                IChunkPtr listDef;
                if ( "list" == child->_name )
                {
                    listDef = new CXML::LET( L"list", CompileRightValue( child, _state ).get() );
                }
                else if ( "range" == child->_name )
                {
                    listDef = new CXML::LET( L"list", CompileOneNode( child, _state ).get() );
                }
                else
                {
                    throw Useless::Error("<fold> expected <list> or <range>.");
                }
                if ( !child )
                {
                    throw Useless::Error("<fold> requires function body.");
                }
                IChunkPtr pFunc;
                if ( "function" == child->_name )
                {
                    pFunc = CompileRightValue( child, _state );
                }
                else
                {
                    std::list< IChunkPtr > defs;
                    for ( ++child; !!child; ++child )
                    {
                        defs.push_back( CompileOneNode( child, _state ));
                    }
                    pFunc = CreateEmpty();
                    for ( std::list< IChunkPtr >::reverse_iterator i = defs.rbegin(); i != defs.rend(); ++i )
                    {
                        pFunc = CreateList( (*i).get(), pFunc.get() );
                    }
                }
                IChunkPtr funcDef = new CXML::LET( L"func", new CXML::ConstantValue( pFunc.get() ));
                pFunctor = new CXML::FOLD( _iteratorId.str(), _accumulatorId.str(),
                                CreateList( listDef.get(), CreateList( accumDef.get(), CreateList( funcDef.get(), CreateEmpty() )))
                                );
            }
            else
            {
                Attr< TextUtf8, true, wchar_t > _iterator(L"iterator");
                Attr< TextUtf8, true, wchar_t > _accumulator(L"accumulator");
                GetAttr( _iterator, _node, _state );
                GetAttr( _accumulator, _node, _state );
                pFunctor = new CXML::FOLD( _iterator.str(), _accumulator.str(), Compile( _node, _state ).get() );
            }
            _state.SetResult( HookErrors( _node, pFunctor.get() ));
            HookDefinition( _node, _state );
        }
        else
        {
            throw Useless::Error("<map> expects children nodes {list, function}.");
        }
        
    }

    /*
     * Obtain list after "count" of elements.
     *
     *  <seek>
     *      <!-- let id="count" -->
     *      <!-- let id="list" -->
     *  </seek>
     */
    LOCAL_TAG_HANDLER( CXML::SEEK )
    {
        _state.SetResult( HookErrors( _node, new CXML::SEEK( Compile( _node, _state ).get() )));
        HookDefinition( _node, _state );
    }
    
    /*
     * Create list of first "count" of elements.
     *
     *  <head>
     *      <!-- let id="count" -->
     *      <!-- let id="list" -->
     *  </head>
     */
    LOCAL_TAG_HANDLER( CXML::HEAD )
    {
        _state.SetResult( HookErrors( _node, new CXML::HEAD( Compile( _node, _state ).get() )));
        HookDefinition( _node, _state );
    }
    
    /*
     * Create list of integers in range ["first", "first + "count").
     *
     *  <head>
     *      <!-- let id="first" -->
     *      <!-- let id="count" -->
     *  </head>
     */
    LOCAL_TAG_HANDLER( CXML::RANGE )
    {
        _state.SetResult( HookErrors( _node, new CXML::RANGE( Compile( _node, _state ).get() )));
        HookDefinition( _node, _state );
    }

    /*
     * Create zip to iterate over parallel lists.
     *
     *  <zip>
     *      <!-- list-code -->
     *      <!-- list-code -->
     *      ..
     *      <!-- list-code -->
     *  </zip>
     */
    LOCAL_TAG_HANDLER( CXML::ZIP )
    {
        _state.SetResult( HookErrors( _node, new CXML::ZIP( Compile( _node, _state ).get() )));
        HookDefinition( _node, _state );
    }



    //---------------------------------------------------------
    //
    // Conditional code execution
    //

    /* 
     * Execute <!-- code --> if EvaluateBoolean( EvaluateString( Expression )) is 1.
     *
     *  <if test="Expression">
     *      <!-- code -->
     *  </if>
     *
     * Execute <!-- code --> if value_of< int >( ChunkId ) is 1.
     *
     *  <if select="ChunkId">
     *      <!-- code -->
     *  </if>
     *
     * Execute <!-- code --> if value_of< int >( Force( ChunkId )) is 1.
     *
     *  <if force="ChunkId">
     *      <!-- code -->
     *  </if>
     *
     * Execute <!-- code --> if value_of< int >( Force( <!-- boolean --> )) is 1.
     *
     *  <if>
     *      <!-- boolean -->
     *      <!-- code -->
     *  </if>
     *
     * (*)Note: When <!-- code --> is not executed, resultant value is <empty>.
     *
     */
    LOCAL_TAG_HANDLER( CXML::IF )
    {
        Attr< TextUtf8, false, char > _cond("test");
        Attr< TextUtf8, false, char > _select("select");
        Attr< TextUtf8, false, char > _force("force");
        if ( _node >> _cond )
        {
            _state.SetResult( HookErrors( _node, new CXML::IF( new CXML::EvaluableValue< int >( _cond.str() ), Compile( _node, _state ).get() )));
        }
        else if ( _node >> _select )
        {
            _state.SetResult( HookErrors( _node, new CXML::IF( new CXML::GET( _select.str() ), Compile( _node, _state ).get() )));
        }
        else if ( _node >> _force )
        {
            _state.SetResult( HookErrors( _node, new CXML::IF( new CXML::LazyCall( new CXML::GET( _select.str() )), Compile( _node, _state ).get() )));
        }
        else
        {
            IChunkPtr pInner = Compile( _node, _state );
            IChunkPtr pCond = pInner->GetChunk( FOURCC_LIST_HEAD );
            IChunkPtr pFunc = pInner->GetChunk( FOURCC_LIST_TAIL );
            _state.SetResult( HookErrors( _node, new CXML::IF( pCond.get(), pFunc.get() )));
        }
        HookDefinition( _node, _state );
    }

    /*
     * Similar to <if> with difference <choose> allows multiple conditions.
     * Each case is enclosed in <!-- when --> node.
     * Default case is enclosed in <otherwise> node.
     *
     *  <choose>
     *      <!-- when -->
     *      <otherwise>
     *          <!-- code -->
     *      </otherwise>
     *  </choose>
     *
     *  where <!-- when --> is one of:
     *
     *  <when test="Expression">
     *      <!-- code -->
     *  </when>
     *
     *  <when select="ChunkId">
     *      <!-- code -->
     *  </when>
     *
     *  <when force="ChunkId">
     *      <!-- code -->
     *  </when>
     *
     *  <when>
     *      <!-- boolean -->
     *      <!-- code -->
     *  </when>
     *
     *
     */
    LOCAL_TAG_HANDLER( CXML::CHOOSE )
    {
        boost::intrusive_ptr< CXML::CHOOSE > pChoose = new CXML::CHOOSE;
        for ( Node child( _node.GetFirstChild()); !!child; ++child )
        {
            if ( child->_name == "when" )
            {
                Attr< TextUtf8, false, char > _test("test");
                Attr< TextUtf8, false, char > _select("select");
                Attr< TextUtf8, false, char > _force("force");
                if ( child >> _test )
                {
                    pChoose->AddOption( new CXML::EvaluableValue< int >( _test.str() ), Compile( child, _state ));
                }
                else if ( child >> _select )
                {
                    pChoose->AddOption( HookErrors( child, new CXML::GET( _select.str() )), Compile( child, _state ));
                }
                else if ( child >> _force )
                {
                    pChoose->AddOption( new CXML::LazyCall( HookErrors( child, new CXML::GET( _force.str() ))), Compile( child, _state ));
                }
                else
                {
                    IChunkPtr pInner = Compile( child, _state );
                    IChunkPtr pCond = pInner->GetChunk( FOURCC_LIST_HEAD );
                    IChunkPtr pFunc = pInner->GetChunk( FOURCC_LIST_TAIL );
                    pChoose->AddOption( pCond.get(), pFunc.get() );
                }
            }
            else
            {
                if ( child->_name != "otherwise" ) { throw Useless::Error("<choose> May contain <when> and <otherwise>"); }
                pChoose->SetOtherwise( Compile( child, _state ));
            }
        }
        _state.SetResult( HookErrors( _node, pChoose.get() ));
        HookDefinition( _node, _state );
    }

    //---------------------------------------------------------

};//XMLFactory
