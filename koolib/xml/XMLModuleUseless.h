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
#ifndef __INCLUDED_KOOLIB_XML_MODULE_USELESS_H__
#define __INCLUDED_KOOLIB_XML_MODULE_USELESS_H__

#include <koolib/xml/XMLChunksUseless.h>

namespace XMLProgram {

    using namespace Useless;
    using namespace XMLFactory;
    
    USELESS_XML_TAG_DICT_DECLARE( xtdImportUseless, ExecutionState );


    /*****************************************************************

      Modules to be imported

     *****************************************************************/

    namespace XML_IMPORT {

        /*****************************************************************

          Module Useless_Tk

        */

        struct KOOLIB_BIND_API USELESS_TK : XMLCodeBlock
        {
            SPointer< SoundCard > _soundcard;
            SPointer< Screen >    _screen;

            USELESS_TK( Node node, ExecutionState &state );

            Useless::Pos    CirclePoint( double i, double n, double start, const Useless::Rect &quaterArea );
            int         GetColorByName( std::string name );
            int         MakeRGB( int red, int green, int blue );
            int         MakeRGBA( int red, int green, int blue, int alpha );
            int         GetKeyCode( std::string name );
            std::string GetKeyName( int keyCode );
            void        SetInputLocale( std::string localeName );
            void        CreatePointList ( Node node, ExecutionState &state );
            void        CreateRectList  ( Node node, ExecutionState &state );
            void        CreateScreen    ( Node node, ExecutionState &state );
            void        CreatePrinter   ( Node node, ExecutionState &state );
            void        CreateImage     ( Node node, ExecutionState &state );
            IChunkPtr   CreateImageBuffer( int width, int height );
            IChunkPtr   CreateSample    ( std::string fileName );
            void        CreateChannel   ( Node __unused__, ExecutionState &state );
            IChunkPtr   CreateOggVorbis ( std::string fileName );
            IChunkPtr   CreateVideo     ( std::string fileName );
            IChunkPtr   CreateMovie     ( std::string fileName );
            IChunkPtr   CreateMovie2    ( std::string fileName, TextUtf8 audioName );
            void        CreateFontPainter( Node node, ExecutionState &state );

            int       IsDir( std::string path );
            int       FileExists( std::string path );
            IChunkPtr ListFiles( std::string path );

            void WriteFile  ( Node node, ExecutionState &state );
            void ReadFile   ( Node node, ExecutionState &state );

            void ToXMLString    ( Node __unused__, ExecutionState &state );
            void FromXMLString  ( Node __unused__, ExecutionState &state );
        };



        /*****************************************************************

          Module Useless_Resources

        */

        struct KOOLIB_BIND_API USELESS_RESOURCES : XMLCodeBlock
        {
            USELESS_RESOURCES( Node node, ExecutionState &state );

            IChunkPtr GetScreen ( std::string name );
            IChunkPtr GetImage  ( std::string name );
            IChunkPtr GetSample ( std::string name );
            IChunkPtr GetFont   ( std::string name );
            IChunkPtr GetSkin   ( std::string name );
            IChunkPtr GetWidget ( std::string name );
            IChunkPtr GetLayout ( std::string name );
            IChunkPtr GetWorkspace( std::string name );
            
            void RaiseEvent( std::string name, std::string arg );

            void KillResources  ( std::string category, std::string name, int recursive );

            void CreateFromXMLString( Node __unused__, ExecutionState &state );
        };



    };//XML_IMPORT

};//XMLProgram

#endif//__INCLUDED_KOOLIB_XML_MODULE_USELESS_H__
