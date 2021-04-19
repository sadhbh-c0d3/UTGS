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

 ********************************************************************/
#ifndef __INCLUDED_KOOLIB_XML_MODULE_DYNAMO_H__
#define __INCLUDED_KOOLIB_XML_MODULE_DYNAMO_H__

#include "koolib/xml/XMLChunksProgram.h"
#include "Dynamo/interfaces.h"

namespace XMLProgram {
    
    using namespace Useless;
    using namespace XMLFactory;
    
    USELESS_XML_TAG_DICT_DECLARE( xtdImportDynamo, ExecutionState );
    
    /*****************************************************************

      Modules to be imported

     *****************************************************************/

    namespace XML_IMPORT {

        struct KOOLIB_BIND_API DYNAMO : XMLCodeBlock
        {
            Dynamo::Hand< Dynamo::IInterfaceProvider > _provider;

            DYNAMO( Node node, ExecutionState &state );
            ~DYNAMO();

            void LoadModule( Node _node, ExecutionState &_state );
            
            IChunkPtr LoadHyperData( std::string name, std::string file, int encrypted );
        };
        
    };//XML_IMPORT
};//XMLProgram

#endif//__INCLUDED_KOOLIB_XML_MODULE_DYNAMO_H__
