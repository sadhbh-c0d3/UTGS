/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE KooLAS C++ Template & Class Library     *
 * CALLED KooLib SOURCE CODE.                                       *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 ********************************************************************

  function:
  last mod: $Id: XMLFactory.cpp,v 1.1.1.1 2004/06/02 19:35:52 koolas Exp $

 ********************************************************************/


#include "XMLFactory.h"
#include "XMLFactoryUtil.h"
//#include "Useless/System/DbgOut.h" - some problems with DLL linkage

namespace XMLFactory {
    
    int OnXMLTagDictInit( const std::string &name )
    {
        //Useless::print_dbg("Initialized XML Tag Dictionary for: %s\n", name.c_str());
        if ( FILE *f = fopen("koolib.log", "r"))
        {
            fclose(f);
            f = fopen("koolib.log", "a+");
            fprintf( f, "Initialized XML Tag Dictionary for: %s\n", name.c_str());
            fclose(f);
        }
        return 0;
    }

};//XMLFactory
