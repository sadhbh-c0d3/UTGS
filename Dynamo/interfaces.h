#ifndef __INCLUDED_DYNAMO_INTERFACES_H__
#define __INCLUDED_DYNAMO_INTERFACES_H__

#include "Dynamo/hooks.h"

namespace Dynamo {
    
    // Constants used to select proper pixel format
    struct ColorMode
    {
        enum Constant { GREY=0, ALPHA=1, RGB=2, RGBA=3, BGR=4, BGRA=5 };
    };

    // Pixel transfer interface - READ
    struct IPixelReader : virtual IInterface
    {
        virtual void GetInput   ( size_t *width, size_t *height, ColorMode::Constant *mode ) = 0;
        virtual void SetupOutput( ColorMode::Constant mode, size_t pitch ) = 0;
        virtual void ReadPixels ( void *pointer, Rect4i area ) = 0;
    };

    // Pixel transfer interface - WRITE
    struct IPixelWriter : virtual IInterface
    {
        virtual void SetupInput ( size_t width, size_t height, ColorMode::Constant mode, size_t pitch ) = 0;
        virtual void WritePixels( const void *pointer, Rect4i area ) = 0;
    };

    // An information attached to graphic data
    struct IGraphicPlane : virtual IInterface
    {
        // Create new IGraphicPlane structure describing some part of existing image
        virtual IGraphicPlane * Copy( Rect4i area ) = 0;
        virtual IGraphicPlane * Copy( Rect4i area, size_t width, size_t height ) = 0;
        // Get dimensions properties
        virtual size_t GetWidth()  = 0;
        virtual size_t GetHeight() = 0;
        virtual size_t GetDestWidth()  = 0;
        virtual size_t GetDestHeight() = 0;
        // Get number of FPS for animated images
        virtual float  GetFrameRate() = 0;
        // Get image channel format
        virtual ColorMode::Constant GetColorMode() = 0;
        // Obtain pixel writter
        virtual IPixelWriter * ProvideIPixelWriter() = 0;
        // Obtain pixel reader
        virtual IPixelReader * ProvideIPixelReader() = 0;
    };

    // Graphic factory
    struct IGraphix : virtual IInterface
    {
        // Find image resource
        virtual IGraphicPlane * GetImage( std::string imageId ) = 0;
        // Find screen resource
        virtual IGraphicPlane * GetScreen( std::string screenId ) = 0;
        // Create new image
        virtual IGraphicPlane * CreateImage( std::string fileName ) = 0;
        // Create new image with alpha channel
        virtual IGraphicPlane * CreateTransparentImage( std::string fileName, std::string maskName ) = 0;
        // Transfer pixels from file to user-supplied writter
        virtual void LoadPixels( std::string fileName, IPixelWriter *writer ) = 0;
        // Get GVM::Context
        virtual GVM_Context * GetGVMContext( std::string screenId ) = 0;
    };

    // Sounds & Music
    struct IAudio : virtual IInterface
    {
        virtual void PlayWavTrack(
                std::string trackName,
                std::string wavId,
                int volume,
                int pan ) = 0;

        virtual void PlayOggTrack(
                std::string trackName,
                std::string oggId,
                int volume,
                int pan ) = 0;

        virtual void SetTrackVolume( int value ) = 0;
        virtual void SetTrackPan( int value ) = 0;

        virtual void SetHook_EndOfAudio( IHook_EndOfAudio * ) = 0;
    };


    // Update InGame Displays
    struct INumberDisplay : virtual IInterface
    {
        virtual void SetNumberDisplay( int number ) = 0;
    };

    struct ITimerDisplay : virtual IInterface
    {
        virtual void SetTimerDisplay( int ms ) = 0;
    };

    struct IItemsOfDisplay : virtual IInterface
    {
        virtual void SetItemsOfDisplay( int items, int total ) = 0;
    };

    struct IOverlayDisplay : virtual IInterface
    {
        virtual void Reset( int opacity ) = 0;

        virtual void ClearAllAfter() = 0;

        virtual void Add( int opacity, int duration ) = 0;

        virtual void SetHook_EndOfFade( IHook_EndOfFade *) = 0;
    };


    // Paint Graphic Elements
    struct IPaint : virtual IInterface
    {
        virtual void BlitImage(
                std::string imageId,
                Rect4i part,
                Rect4i where ) = 0;

        virtual void MultiBlit(
                std::string imageId,
                std::vector< Point2i > positions,
                std::vector< Rect4i > rects ) = 0;

        virtual void FillRect(
                Rect4i where,
                int color ) = 0;

        virtual void DrawRect(
                Rect4i where,
                int color ) = 0;

        virtual void DrawLineSegment(
                Point2i from,
                Point2i to,
                int thick,
                int color ) = 0;

        virtual void DrawLineSegments( std::vector< Point2i > &sectors, int thick, int color ) = 0;

        virtual IPaint * CreateNew( Point2i offset ) = 0;

        virtual void Crop( Rect4i area ) = 0;

        enum { NORMAL, REPLACE, ADD, MULTIPLY, SHADOW };

        virtual void SetBlendMode( int mode ) = 0;        
        virtual void MultiplyAlpha( int value ) = 0;
        virtual void SetColorFilter( unsigned int color ) = 0;
        
        virtual GVM_Context * GetGVMContext() = 0;
        virtual void SetFrustum    ( double left, double right, double bottom, double top, double znear, double zfar ) = 0;
        virtual void SetOrtho      ( double left, double right, double bottom, double top, double znear, double zfar ) = 0;
        virtual void SetOrtho2D    ( double left, double right, double bottom, double top ) = 0; 
        virtual void SetPerspective( double fov, double aspect, double znear, double zfar ) = 0;
        virtual void Invoke        ( const GVM_Message &msg ) = 0;
    };


    // Access Widget Properties
    struct IWidget : virtual IInterface
    {
        virtual IWidget * GetParent() = 0;
        
        virtual Point2i GetPos() = 0;
        virtual void SetPos( Point2i pos ) = 0;
        
        virtual Rect4i GetRect() = 0;
        virtual void   SetRect( Rect4i area ) = 0;

        virtual Rect4i GetAbsoluteRect( Rect4i area ) = 0;
        virtual Rect4i GetRelativeRect( Rect4i area ) = 0;

        virtual void SetDirty( Rect4i area ) = 0;

        virtual void SetOpacity( int opacity ) = 0;
        virtual int  GetOpacity() = 0;

        virtual void SetPhantom( bool phantom ) = 0;
        virtual bool GetPhantom() = 0;

        virtual void SetVisible( bool visible ) = 0;
        virtual bool GetVisible() = 0;
        virtual bool IsReallyVisible() = 0;

        virtual void CaptureKeyboard() = 0;
        virtual void ReleaseKeyboard() = 0;

        virtual void CapturePointer() = 0;
        virtual void ReleasePointer() = 0;

        virtual void IncEagerToRecievePointerEvents() = 0;
        virtual void DecEagerToRecievePointerEvents() = 0;

        virtual void Paint( IPaint *paint ) = 0;

        virtual void SetHook_Paint( IHook_Paint * ) = 0;
        virtual void SetHook_Resize( IHook_Size * ) = 0;
    };

    // Handle {Clicks,Press,Release}{LMB,RMB,MMB}, WhileMoving...
    struct IActiveWidget : virtual IWidget
    {
        virtual void SetEnabled( bool enabled ) = 0;
        virtual bool GetEnabled() = 0;

        virtual void SetKeyEnabled( int key, bool enabled ) = 0;

        virtual bool IsKeyPressed( int key ) = 0;
        virtual Point2i GetTouchPoint( int buttonNo ) = 0;

        virtual void SetHook_Pointer( std::string eventName, IHook_Pointer * ) = 0;
        virtual void SetHook_Key( IHook_Key * ) = 0;
    };

    // Handle OnChange
    struct IEnumWidget : virtual IWidget
    {
        virtual void SetSelected( int item ) = 0;
        virtual int  GetSelected() = 0;

        virtual void SetHook_Select( IHook_Number * ) = 0;
    };

    // Access Widget which paints Text
    struct ITextWidget : virtual IWidget
    {
        // SetText/GetText in ucs-4
        virtual void SetText( std::basic_string< wchar_t > text ) = 0;
        virtual void SetTextF( wchar_t *fmtStr, ... ) = 0;
        virtual std::basic_string< wchar_t > GetText() = 0;

        // SetText/GetText in utf-8, cp1250, iso-8859-2 ...
        virtual void SetText( const char *localeName, std::basic_string< char > text ) = 0;
        virtual void SetTextF( const char *localeName, char *fmtStr, ... ) = 0;
        virtual std::basic_string< char > GetText( const char *localeName ) = 0;
    };

    // Access Widget which gives Text
    struct ICharInputWidget : virtual ITextWidget, virtual IActiveWidget
    { 
        virtual void SetHook_CharValidate( IHook_CharValidate * ) = 0;
        virtual void SetHook_NewText( IHook_NewText * ) = 0;
    };

    // Access Widget which paints Image
    struct IImageWidget : virtual IWidget
    {
        virtual void SetImage( std::string imageId ) = 0;
        virtual void SetSkin( std::string skinId ) = 0;
    };


    // Access Gameboard View
    struct IBoardWidget : virtual IActiveWidget
    {
        virtual void GenObjects( int count, int *names ) = 0;
        virtual void DestroyObjects( int count, int *names ) = 0;

        virtual void GetLook(
                int objectId,
                Look *look ) = 0;

        virtual void GetLoc(
                int objectId,
                Loc3f *loc ) = 0;

        virtual void GetExtents(
                int objectId,
                Loc3f *extents ) = 0;

        virtual void SetLook(
                int objectId,
                Look &look ) = 0;

        virtual void SetLoc(
                int objectId,
                Loc3f &loc ) = 0;

        virtual void SetExtents(
                int objectId,
                Loc3f &extents ) = 0;

        virtual int  GetOrder( int objectId ) = 0;
        virtual void SetOrder( int objectId, int order ) = 0;

        virtual std::vector< int > Pick( Point2i where ) = 0;
        virtual std::vector< int > Pick( Rect4i where ) = 0;
        virtual std::vector< int > Pick( Loc3f &loc ) = 0;

        virtual Point2i ConvertPosition( Loc3f &loc ) = 0;
        virtual Loc3f ConvertPosition( Point2i pt, float w ) = 0;

        virtual Point2i ConvertVector( Loc3f &loc ) = 0;
        virtual Loc3f ConvertVector( Point2i pt, float w ) = 0;


        virtual void SetHook_EndOfBoardObjectAnimation( IHook_EndOfBoardObjectAnimation * ) = 0;
        virtual void SetHook_PaintBoardObject( IHook_PaintBoardObject * ) = 0;
    };


    // Access G.U.I. Properties & Elements
    struct IGui : virtual IInterface
    {
        virtual Point2i GetPointerPosition() = 0;

        virtual IWidget *
            GetWidget( std::string widgetId ) = 0;

        virtual IActiveWidget *
            GetActiveWidget( std::string widgetId ) = 0;

        virtual IEnumWidget *
            GetEnumWidget( std::string widgetId ) = 0;

        virtual ITextWidget *
            GetTextWidget( std::string widgetId ) = 0;

        virtual ICharInputWidget *
            GetCharInputWidget( std::string widgetId ) = 0;

        virtual IImageWidget *
            GetImageWidget( std::string widgetId ) = 0;

        virtual IBoardWidget *
            GetBoardWidget( std::string widgetId ) = 0;

        virtual INumberDisplay *
            MakeNumberDisplay( std::string widgetId ) = 0;

        virtual ITimerDisplay *
            MakeTimerDisplay( std::string widgetId ) = 0;

        virtual IItemsOfDisplay *
            MakeItemsOfDisplay( std::string widgetId ) = 0;
    };


    // Access Player Profile
    struct IPlayerProfile : virtual IInterface
    {
        virtual std::basic_string< wchar_t >
            GetPlayerName() = 0;

        virtual std::string
            GetPlayerIcon() = 0;

        virtual std::string
            GetLastPlayedGame() = 0;
    };


    // Access Current Game Properties
    struct IGameInfo : virtual IInterface
    {
        virtual std::basic_string< wchar_t >
            GetLevelName( int levelNo ) = 0;

        virtual std::basic_string< wchar_t >
            GetStageName( int stageNo ) = 0;

        virtual int GetLevelNo() = 0;
        virtual int GetStageNo() = 0;
    };


    // Access Player Profiles
    struct IProfileManager : virtual IInterface
    {
        virtual std::vector< std::basic_string< wchar_t > >
            GetProfileNames() = 0;

        virtual IPlayerProfile *
            GetPlayerProfile( std::basic_string< wchar_t > playerName ) = 0;

        virtual void RemoveProfile( std::basic_string< wchar_t > playerName ) = 0;
        
        virtual void AddProfile( IPlayerProfile *profile ) = 0;

    };

    // Access Game Server
    struct IGameServer : virtual IInterface
    {
        virtual void SetHook_GameTick( IHook_TimeTick * ) = 0;
        virtual void SetHook_RealTick( IHook_TimeTick * ) = 0;
    };

    // Access Object created by Dynamic Module
    struct IModule : virtual IInterface
    {
        virtual IInterface *GetInstance() = 0;
    };

    // Access XML Hyper Data
    struct IXml : virtual IInterface
    {
        virtual HyperObject & GetData() = 0;

        virtual void WriteData() = 0;
    };

    // KooLiXP Access Interfaces 
    /*@{*/
    struct IXmlScope;
    struct IXmlChunk;
    struct IXmlHook;
    struct IXmlProgram;
    /*@}*/

    // Access KooLiXP Chunk
    struct IXmlChunk : virtual IInterface
    {
        virtual bool    GetAttr( const std::string &name, int *pInt ) = 0;
        virtual bool    GetAttr( const std::string &name, double *pFloat ) = 0;
        virtual bool    GetAttr( const std::string &name, std::basic_string< wchar_t > *pText ) = 0;

        virtual int         AsInt() = 0;
        virtual float       AsFloat() = 0;
        virtual IPaint*     AsIPaint() = 0;
        virtual std::basic_string< wchar_t > AsText() = 0;

        virtual IXmlChunk*  GetChunk( const std::string &name ) = 0;
        virtual IXmlChunk*  GetChunk( unsigned int code ) = 0;
        virtual void        SetChunk( const std::string &name, IXmlChunk *pChunk ) = 0;
        virtual void        Execute( IXmlScope *pScope ) = 0;
        virtual bool        IsEmpty() = 0;
    };

    // Access KooLiXP ExecutionState
    struct IXmlScope : virtual IInterface
    {
        virtual void SetResult( IXmlChunk *pChunk ) = 0;
        
        virtual IXmlChunk*  GetChunk( const std::string &name ) = 0;
        virtual void        SetChunk( const std::string &name, IXmlChunk *pChunk ) = 0;
        virtual void        AddChunk( const std::string &name, IXmlChunk *pChunk ) = 0;
        virtual void        AddMethod( const std::string &name, IXmlHook *pHook ) = 0;
        
        virtual IXmlScope*  NewScope() = 0;
    };

    // Bind Methods in KooLiXP 
    struct IXmlHook : virtual IInterface
    {
        virtual void Apply( IXmlScope *pScope ) = 0;
    };

    // Create Objects in KooLiXP
    struct IXmlProgram : virtual IInterface
    {
        virtual IXmlScope*  NewScope( XMLProgram::ExecutionState &state ) = 0;
        virtual IXmlScope*  GetScope() = 0;
        virtual IXmlChunk*  NewInt( int value ) = 0;
        virtual IXmlChunk*  NewFloat( double value ) = 0;
        virtual IXmlChunk*  NewText( const std::basic_string< wchar_t > &value ) = 0;
        virtual IXmlChunk*  NewList() = 0;
        virtual IXmlChunk*  Append( IXmlChunk *pListChunk, IXmlChunk *pChunk ) = 0;
    };

};//Dynamo

namespace Dynamo {

    // Dynamo Factory Inerface
    struct IInterfaceProvider : virtual IInterface
    {
        virtual IGraphix *
            ProvideIGraphix() = 0;

        virtual IAudio *
            ProvideIAudio() = 0;

        virtual IGui *
            ProvideIGui() = 0;

        virtual IGameServer *
            ProvideIGameServer() = 0;

        virtual IProfileManager *
            ProvideIProfileManager() = 0;

        virtual IPlayerProfile *
            ProvideIPlayerProfile() = 0;

        virtual IGameInfo *
            ProvideIGameInfo() = 0;

        virtual IXml *
            ProvideIXml( std::string xmlId, std::string fileName, bool useEncryption ) = 0;

        virtual IModule *
            ProvideIModule( std::string moduleName, IXmlProgram *pProg, IXmlScope *pScope ) = 0;

        virtual IXmlProgram *
            ProvideIXmlProgram( XMLProgram::IFiniteStateMachine *pFSM ) = 0;
    };

#define Dynamo_DEFINE_CREATE_INSTANCE( _Name ) \
    extern "C" __declspec(dllexport) Dynamo::IInterface *CreateInstance( Dynamo::IInterfaceProvider *provider, Dynamo::IXmlProgram *prog, Dynamo::IXmlScope *scope )\
    { return new _Name( provider, prog, scope ); }

    typedef IInterface *(*CreateInstance_t)( IInterfaceProvider *provider, IXmlProgram *prog, IXmlScope *scope );
};

#endif//__INCLUDED_DYNAMO_INTERFACES_H__
