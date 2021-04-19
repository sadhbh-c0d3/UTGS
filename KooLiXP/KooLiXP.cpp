#include "Useless/UselessLib.h"
#include "Useless/System/Application.h"
#include "Useless/GUI/Display/ScreenShooter.h"
#include "Useless/Graphic/Device/winGDI/GDIScreen.h" //!< used for KooLiXP splash
#include "Useless/Graphic/Device/winGDI/GDISurface.h" //!< used for KooLiXP splash

#include "koolib/xml/XMLChunksGUI.h"
#include "koolib/xml/XMLModuleBuiltIn.h"
#include "koolib/xml/XMLModuleUseless.h"
#include "koolib/xml/XMLModuleDynamo.h"

#include "resource.h"

#include <direct.h>
#include <iostream>

namespace Useless {
    USELESS_API extern bool G_EnableDDrawScr;
    USELESS_API extern bool G_DisableAltTab;
    USELESS_API extern bool G_EnableDebugF11;
    USELESS_API extern bool G_EnablePrintScr;
    USELESS_API extern bool G_nVidiaViewport;
    USELESS_API extern double G_GLTesselatorStartingTexelOffset;
    USELESS_API extern double G_GLTesselatorEndingTexelOffset;
};

void report(const char *text)
{
    OutputDebugStringA(text);
}

int get_argc()
{
    return Useless::Application::GetArgC();
}

char op_convert(wchar_t w)
{
    return (char)w;
}

std::string get_argv( int i )
{
    std::wstring wname( Useless::Application::GetArgV()[i] );
    std::string name( wname.size(), 0 );
    std::transform( wname.begin(), wname.end(), name.begin(), &op_convert );
    return name;
}

std::string g_ExcptText;

INT_PTR CALLBACK KDialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    Useless::Text converted;
    switch( uMsg )
    {
    case WM_INITDIALOG:
        for ( int i=0; i<g_ExcptText.size(); ++i )
        {
            char c = g_ExcptText[i];
            if ( c == '\n')
            {
                converted += '\r';
                converted += '\n';
            }
            else if ( c == '\t' )
            {
                converted += ' ';
                converted += ' ';
                converted += ' ';
                converted += ' ';
            }
            else {
                converted += c;
            }
        }

        // FIXME
        //SetDlgItemText( hwndDlg, /*IDC_OUTPUT*/ 0, converted.c_str() );
        break;

    case WM_COMMAND:
        switch( LOWORD( wParam ))
        {
        case IDOK:
            EndDialog( hwndDlg, IDOK );
            break;
        case IDCANCEL:
            EndDialog( hwndDlg, IDCANCEL );
            break;
        };
        break;

    default:
	break;
    };
    return false;
}



struct KooLiXP
{ 
    Useless::Error _error;
    Useless::ScreenResource::Type _screen;
    bool        _snapshotOnly;
    bool        _helpOnly;
    bool        _noSplash;
    std::string _file;
    std::string _guiScreen;

    KooLiXP()
        : _snapshotOnly( false )
        , _helpOnly( false )
        , _noSplash( false )
        {
            XMLProgram::GlobalsAddRef();
            Useless::Application::SetIdleDT(5);

            for ( int i=1, I=get_argc(); i < I; ++i )
            {
                if ( 0 == get_argv(i).find("--"))
                {
                    if ( get_argv(i) == "--chroot" )
                    {
                        _chdir( get_argv(++i).c_str() );
                    }
                    else if( get_argv(i) == "--snapshot" )
                    {
                        _snapshotOnly = true;
                    }
                    else if( get_argv(i) == "--no-splash" )
                    {
                        _noSplash = true;
                    }
                    else if ( get_argv(i) == "--gui-screen" )
                    {
                        _guiScreen = get_argv(++i);
                    }
                    else if ( std::string::npos != get_argv(i).rfind( "-direct-draw-screen" ))
                    {
                        Useless::G_EnableDDrawScr = (std::string::npos == get_argv(i).find("--no"));
                    }
                    else if ( std::string::npos != get_argv(i).rfind( "-alt-tab" ))
                    {
                        Useless::G_DisableAltTab =  ( 0 == get_argv(i).find("--no"));
                    }
                    else if ( std::string::npos != get_argv(i).rfind( "-print-screen" ))
                    {
                        Useless::G_EnablePrintScr = (std::string::npos == get_argv(i).find("--no"));
                    }
                    else if ( std::string::npos != get_argv(i).rfind( "-debug-keys" ))
                    {
                        Useless::G_EnableDebugF11 = (std::string::npos == get_argv(i).find("--no"));
                    }
                    else if ( std::string::npos != get_argv(i).rfind( "-viewport-fix" ))
                    {
                        Useless::G_nVidiaViewport = ( 0 == get_argv(i).find("--no"));
                    }
                    else if ( get_argv(i) == "--idle-time-delta" )
                    {
                        Useless::Application::SetIdleDT( Useless::to_integer( get_argv(++i)));
                    }
                    else if ( get_argv(i) == "--texel-offsets" )
                    {
                        Useless::G_GLTesselatorStartingTexelOffset = Useless::to_real( get_argv(++i));
                        Useless::G_GLTesselatorEndingTexelOffset = Useless::to_real( get_argv(++i));
                    }
                    else if ( get_argv(i) == "--help" )
                    {
                        std::stringstream ss;
                        PutInfo( ss );
                        PutUsage( ss );
                        _helpOnly = true;
                        _error = Useless::Error("%s", ss.str().c_str());
                    }
                    else if ( get_argv(i) == "--" )
                    {
                        XMLProgram::ExecutionState &gS = XMLProgram::GetGlobals();
                        XMLProgram::IBlock *pBlock = 0;
                        gS._currentBlock->AddChunk( L"argv", pBlock = XMLProgram::CreateBlock() );
                        for ( ++i; i < I; ++i )
                        {
                            if ( get_argv(i) == "--" )
                            {
                                break;
                            }
                            int p = get_argv(i).find("=");
                            if ( std::string::npos != p )
                            {
                                std::string name  = get_argv(i).substr(0,p);
                                std::string value = get_argv(i).substr(p+1,-1);
                                pBlock->AddChunk( TextUtf8( name ), XMLProgram::CreateValue( TextUtf8( value )));
                            }
                            else
                            {
                                pBlock->AddChunk( get_argv(i), XMLProgram::CreateValue( 1 ));
                            }
                        }
                    }
                    else
                    {
                        std::stringstream ss;
                        PutInfo( ss );
                        PutError( ss, "Bad option '"+ get_argv(i) +"' given.\n\nFor usage type KooLiXP --help.");
                        _helpOnly = true;
                        _error = Useless::Error("%s", ss.str().c_str());
                    }
                }
                else
                {
                    _file = get_argv(i);
                }
            }
            if ( !_helpOnly )
            {
                if ( _file.empty() )
                {
                    std::stringstream ss;
                    PutInfo( ss );
                    PutError( ss, "No filename given.\n\nFor usage type KooLiXP --help.");
                    _helpOnly = true;
                    _error = Useless::Error("%s", ss.str().c_str());
                }
                else if ( !Useless::IFS::Instance().Exists( _file ))
                {
                    std::stringstream ss;
                    PutInfo( ss );
                    PutError( ss, "File '"+ _file +"' was not found.\n\nFor usage type KooLiXP --help.");
                    _helpOnly = true;
                    _error = Useless::Error("%s", ss.str().c_str());
                }
            }
        }

    int MakeScreenshot()
    {
        _screen->HideDecorations();
        _screen->Reposition(0,0);
        Useless::Application::Step();
        Useless::ScreenShooter(*_screen).Shot();
        return 0;
    }

    int Run()
    {
        try {
            if ( _helpOnly )
            {
                throw _error;
                return 0;
            }
            else
            {
                try {
                    // FIXME (Visual Studio Express does not support Resource Editing
                    //if ( !_noSplash )
                    //{
                    //    Useless::Surf::Properties splashProps;
                    //    splashProps.width = 320;
                    //    splashProps.height = 240;
                    //    splashProps.pixelformat = Useless::ImageFormat::B8G8R8A8;
                    //    _screen = new Useless::GDIScreen( Useless::Error("KooLiXP: Starting %s, Please wait...", _file.c_str()).GetError() );
                    //    int whereX = (::GetSystemMetrics( SM_CXFULLSCREEN ) - splashProps.width)/2;
                    //    int whereY = (::GetSystemMetrics( SM_CYFULLSCREEN ) - splashProps.height)/2;
                    //    _screen->Reposition( whereX, whereY );
                    //    _screen->HideDecorations();
                    //    _screen->OpenWindowed( splashProps.width, splashProps.height);
                    //    _screen->SetClipper( Useless::Rect(splashProps.width, splashProps.height) );
                    //    Useless::GDISurface splashSurf( splashProps, _screen->GetHandle(), 0/*IDB_BITMAP1*/ );
                    //    _screen->GetSurface()->Blit( 0, 0, splashSurf, Useless::Rect(splashProps.width, splashProps.height) );
                    //    _screen->Swap();
                    //}

                    Useless::CreateFromXMLFile( _file );

                    if ( !!_screen )
                    {
                        _screen->Close();
                    }
                    _screen = *Useless::ScreenResource("system", "screen");
                    if ( !_guiScreen.empty() )
                    {
                        Useless::SetCurrentScreen( Useless::WidgetResource< Useless::Widget >::Query( _guiScreen ));
                    }
                    if ( _snapshotOnly )
                    {
                        return MakeScreenshot();
                    }
                    else
                    {
                        return Useless::Application::Run();
                    }
                }
                catch ( const std::exception &e )
                {
                    std::stringstream ss;
                    PutInfo( ss );
                    PutError( ss, e.what() );
					throw Useless::Error( ss.str().c_str() );
                }
            }
        }
        catch ( const std::exception &e )
        {
            if ( !!_screen )
            {
                _screen->Close();
            }
            g_ExcptText = e.what();
            report(g_ExcptText.c_str());

            MessageBoxA(NULL, g_ExcptText.c_str(), "Problem occured", MB_OK);

            // FIXME Visual Studio Express does not allow resource editing
            // int hr = (int)DialogBox(Useless::Application::GetInstance(), MAKEINTRESOURCE( IDD_EXCEPTION ), NULL, KDialogProc );
    //        if ( IDOK == hr )
    //        {
				//throw e;
    //        }
            return -1;
        }
    }

    ~KooLiXP()
    {
        XMLProgram::GlobalsRemRef();
        Useless::Resources::Kill();
    }

    void PutInfo( std::ostream &out )
    {
        out << "KooLiXP Runner" << std::endl;
        out << std::endl;
    }

    void PutError( std::ostream &out, const std::string &message )
    {
        out << "Error : " << message << std::endl;
        out << std::endl;
    }

    void PutUsage( std::ostream &out )
    {
        out << "Usage : " << "KooLiXP.exe [switches] filename [-- global=value ... --]" << std::endl;
        out << std::endl;
        out << "\t" << "--chroot" << "\n\t\t\t" << "change root directory" << std::endl;
        out << std::endl;
        out << "\t" << "--snapshot" << "\n\t\t\t" << "write screen snapshot to screen0.tga" << std::endl;
        out << std::endl;
        out << "\t" << "--gui-screen" << "\n\t\t\t" << "set current gui container widget" << std::endl;
        out << std::endl;
        out << "\t" << "--direct-draw-screen" << "\n\t\t\t" << "open screen via ddraw" << std::endl;
        out << std::endl;
        out << "\t" << "--no-alt-tab" << "\n\t\t\t" << "disable alt tab" << std::endl;
        out << std::endl;
        out << "\t" << "--no-print-screen" << "\n\t\t\t" << "disable print screen" << std::endl;
        out << std::endl;
        out << "\t" << "--no-debug-keys" << "\n\t\t\t" << "disable debug F11 key" << std::endl;
        out << std::endl;
        out << "\t" << "--viewport-fix" << "\n\t\t\t" << "fixes problem with Matrox" << std::endl;
        out << std::endl;
        out << "\t" << "--idle-time-delta n" << "\n\t\t\t" << "minimum miliseconds between subsequent updates" << std::endl;
        out << std::endl;
        out << "\t" << "--texel-offsets n m" << "\n\t\t\t" << "n,m are real numbers. " << "\n\t\t\t"
            << "try 'n m' = '0.5 0.5' to fix image on Radeon cards." << std::endl;
        out << std::endl;
    }
};

int Main()
{
    KooLiXP object;
    return object.Run();
}
