#include "koolib/xml/XMLModuleDynamo.h"
#include "koolib/xml/XMLChunksMethod.h"
#include "koolib/HyperObject.h"
#include "Dynamo/classes/CInterfaceProvider.h"

namespace XMLProgram {
    
    template< class _Type > struct DynamoChunk : XMLCodeBlock
    {
        Dynamo::Hand< _Type > _object;
        DynamoChunk()
        {
        }

        DynamoChunk( Dynamo::Hand< _Type > object ): _object( object )
        {
        }

        ~DynamoChunk()
        {
            static_cast< XMLCodeBlock &>(*this) = XMLCodeBlock();
            _object = 0;
        }
    };// struct DynamoChunk< _Type >

    
    struct DynamoXmlChunk : IChunk
    {
        Dynamo::Hand< Dynamo::IXml > _xml;
        IChunkPtr _writeData;

        DynamoXmlChunk( Dynamo::Hand< Dynamo::IXml > xml ): _xml( xml )        
        {
            _writeData = make_method_chunk( _xml.get(), &Dynamo::IXml::WriteData );
        }

        ~DynamoXmlChunk()
        {
        }

        IChunk* GetChunk( const TextUtf8 &name )
        {
            if ( name == L"WriteData" )
            {
                return _writeData.get();
            }
            else
            {
                std::string sname = TextUtf8( name );
                TextUtf8 value( _xml->GetData().get( sname ));
                GetGlobals().SetResult( CreateValue( value ));
                return GetGlobals().GetResult();
            }
        }

        void SetChunk( const TextUtf8 &name, IChunk *pChunk )
        {
            if ( name == L"WriteData" )
            {
                _writeData = pChunk;
            }
            else
            {
                std::string sname = TextUtf8( name );
                std::string value = value_of< TextUtf8 >( pChunk );
                _xml->GetData()[ sname ] = value;
            }
        }

        bool operator >> ( XMLFactory::AttrUniBase &attr )
        {
            if ( attr._name == L"type-name" )
            {
                attr.str(L"xml-hyper-data");
            }
            else
            {
                std::string name = TextUtf8( attr._name );
                TextUtf8 value( _xml->GetData()[ name ] );
                attr.str( value );
            }
            return true;
        }

        bool operator << ( XMLFactory::AttrUniBase &attr )
        {
            if ( attr._name == L"WriteData" )
            {
                throw Useless::Error("Cannot set hyper data. Field name 'WriteData' is reserved.");
            }
            std::string name = TextUtf8( attr._name );
            std::string value = TextUtf8( attr.str() );
            _xml->GetData()[ name ] = value;
            return true;
        }
    };//struct DynamoXmlChunk



    namespace XML_IMPORT {

        DYNAMO::DYNAMO( Node node, ExecutionState &state )
        {
			_provider = new Dynamo::CInterfaceProvider();
            add_methods( this )
                .def("LoadModule", &DYNAMO::LoadModule )
                .def("LoadHyperData", &DYNAMO::LoadHyperData, "name", "file", "encrypted")
                ;
        }

        DYNAMO::~DYNAMO()
        {
        }
            
        void DYNAMO::LoadModule( Node _node, ExecutionState &_state )
        {
            Attr< std::string > _name("name");
            GetAttr( _name, _node, _state );
            DynamoChunk< Dynamo::IModule > *pGCh = new DynamoChunk< Dynamo::IModule >();
            IBlockPtr guard( pGCh );
            ExecutionState state( _state );
            state._prevState = &_state;
            state._currentBlock = pGCh;
            Dynamo::Hand< Dynamo::IXmlProgram > prog = _provider->ProvideIXmlProgram( _state._currentFSM );
            Dynamo::Hand< Dynamo::IXmlScope > scope = prog->NewScope( state );
            pGCh->_object = _provider->ProvideIModule( _name.str(), prog.get(), scope.get() );
            _state.SetResult( pGCh );
        }
        
        IChunkPtr DYNAMO::LoadHyperData  ( std::string name, std::string file, int encrypted )
        {
            return new DynamoXmlChunk( _provider->ProvideIXml( name, file, encrypted ));
        }
        
    };//XML_IMPORT
    
    /*****************************************************************

      Import all Dynamo stuff

     */

    USELESS_XML_TAG_DICT_DEFINE( xtdImportDynamo, ExecutionState );

    static USELESS_XML_TAG_DICT_Extend( xtdImportDynamo, ExecutionState, BasicProgram,
            .AddReg< XML_IMPORT::DYNAMO >("Dynamo")
            );
    
    static USELESS_XML_TAG_DICT_AddBase( xtdImport, ExecutionState, xtdImportDynamo );
    
};//XMLProgram


namespace XMLFactory {
    using namespace XMLProgram;

#define DEFINE_MODULE_IMPORT( _modName ) USELESS_XML_TAG_HANDLER( _modName, ExecutionState ) \
{\
    _state.SetResult( new _modName( _node, _state ) ); \
}

DEFINE_MODULE_IMPORT( XML_IMPORT::DYNAMO );

};//XMLFactory

