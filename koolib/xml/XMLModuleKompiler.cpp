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
 ********************************************************************/

#define MEAN_AND_LEAN
#include <koolib/xml/XMLModuleBuiltIn.h>
#include <koolib/xml/XMLChunksMethod.h>
#include <koolib/xml/XMLKompiler.h>
#include "Useless/File/VIFS.h"
#include "Useless/File/IFileStream.h"
#undef XMLDocument

namespace XMLProgram {
    
    namespace XML_IMPORT
    {

        struct KOMPILER : XMLCodeBlock
        {
            KOMPILER()
            {
                add_methods( this )
                    .def("CompileString", &KOMPILER::CompileString)
                    .def("CompileFile", &KOMPILER::CompileFile)
                    .def("RunString", &KOMPILER::RunString)
                    .def("RunFile", &KOMPILER::RunFile)
                    ;
            }

            void CompileString( Node __unused__, ExecutionState &state )
            {
                IChunk *pValue = XMLProgram::GetChunk(L"code", state);
                if ( !pValue ) { throw Useless::Error("CompileString: 'code' expected"); }
                XMLValueChunk< std::string > *pData = dynamic_cast< XMLValueChunk< std::string > *>( pValue );
                if ( pData ) {
                    Useless::XMLDocument doc( pData->_value, "" );
                    state.SetResult( XMLProgram::Compile( doc.Root(), state ).get() );
                }
                else {
                    Useless::XMLDocument doc( value_of< TextUtf8 >( pValue ).as_string(), "" );
                    state.SetResult( XMLProgram::Compile( doc.Root(), state ).get() );
                }
            }

            void RunString( Node __unused__, ExecutionState &state )
            {
                CompileString( __unused__, state );
                IChunkPtr pResult = state.GetResult();
                pResult->Execute( __unused__, state );
            }

            void CompileFile( Node __unused__, ExecutionState &state )
            {
                IChunk *pFile = XMLProgram::GetChunk(L"file", state);
                if ( !pFile ) { throw Useless::Error("RunFile: 'file' expected"); }
                std::string file = value_of< TextUtf8 >( pFile ).as_string();                
                Useless::SPointer< Useless::IFile > fileInput = Useless::IFS::Instance().OpenFile( file );
                Useless::IFileStream stream( fileInput );
                std::string ln;
                std::string data;
                while ( std::getline( stream, ln ))
                {
                    data += ln;
                    data += "\n";
                }
                Useless::XMLDocument doc( data, file );
                state.SetResult( XMLProgram::Compile( doc.Root(), state ).get() );
            }
            
            void RunFile( Node __unused__, ExecutionState &state )
            {
                CompileFile( __unused__, state );
                IChunkPtr pResult = state.GetResult();
                pResult->Execute( __unused__, state );
            }
        };
    };// namespace XML_IMPORT
};//namespace XMLProgram

namespace XMLFactory {
    using namespace XMLProgram;

#define DEFINE_MODULE_IMPORT( _modName ) USELESS_XML_TAG_HANDLER( _modName, ExecutionState ) \
{\
    _state.SetResult( new _modName() ); \
}

DEFINE_MODULE_IMPORT( XML_IMPORT::KOMPILER );

};//XMLFactory
