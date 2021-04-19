#ifndef __INCLUDED_DYNAMO_HOOKS_H__
#define __INCLUDED_DYNAMO_HOOKS_H__

#include "Dynamo/bits.h"

namespace Dynamo {

    // Hooks
    struct IHook_CharValidate : virtual Dynamo::IInterface
    {
        virtual int Apply( int c ) = 0; 
    };

    struct IHook_EndOfAudio : virtual IInterface 
    {
        virtual void Apply( std::string trackName, std::string oggId ) = 0; 
    };

    struct IHook_EndOfBoardObjectAnimation : virtual IInterface 
    {
        virtual void Apply( int boardItemId, Look look ) = 0; 
    };

    struct IHook_EndOfFade : virtual IInterface 
    {
        virtual void Apply() = 0; 
    };

    struct IHook_Number : virtual IInterface 
    {
        virtual void Apply( int no ) = 0; 
    };

    struct IHook_Key : virtual IInterface 
    {
        virtual void Apply( int key ) = 0; 
    };

    struct IHook_NewText : virtual IInterface 
    {
        virtual void Apply( std::basic_string< wchar_t > text ) = 0; 
    };

    struct IHook_Paint : virtual IInterface 
    {
        virtual void Apply( struct IPaint *paint ) = 0; 
    };

    struct IHook_Size : virtual IInterface
    {
        virtual void Apply( int width, int height ) = 0;
    };
    
    struct IHook_PaintBoardObject : virtual IInterface
    {
        virtual void Apply( struct IPaint *paint, Point2i where, int objectId, Look look ) = 0; 
    };

    struct IHook_Pointer : virtual IInterface 
    {
        virtual void Apply( Point2i pt ) = 0;
    };

    struct IHook_TimeTick : virtual IInterface
    {
        virtual void Apply( int ms ) = 0;
    };

};//Dynamo

#endif//__INCLUDED_DYNAMO_HOOKS_H__
