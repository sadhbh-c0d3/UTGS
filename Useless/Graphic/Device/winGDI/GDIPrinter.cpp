#include "UselessPch.h"

#include "GDIPrinter.h"
#include "Useless/Graphic/Planes/IGraphics.h"
#include "Useless/Graphic/Planes/MemSurface.h"
#include "Useless/Graphic/Device/Surface.h"
#include <stdio.h>

namespace Useless {

    namespace {
        void LogError( FILE *fDump )
        {
            LPVOID lpMsgBuf;
            FormatMessage( 
                    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                    FORMAT_MESSAGE_FROM_SYSTEM | 
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    GetLastError(),
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                    (LPTSTR) &lpMsgBuf,
                    0,
                    NULL 
                    );
            _ftprintf( fDump, _T("Error: %s\n"), lpMsgBuf );
            LocalFree( lpMsgBuf );
        }
    };//unnamed

    /* (1) */
    std::vector< char > g_vGDIPrinterData;
    std::vector< char > g_vGDIPrinterData2;
    bool g_bRotate90 = false;
    bool g_bFlipX = true;
    /* (1) */

    GDIPage::GDIPage( ::HDC hdc ): m_hdc( hdc )
    {
        ::StartPage( m_hdc );
    }

    GDIPage::~GDIPage()
    {
    }

    void GDIPage::BlitImage( const Rect &onPaper, IGraphics &image )
    {
        long lRasterCaps = GetDeviceCaps( m_hdc, RASTERCAPS );
        bool bScaling = lRasterCaps & RC_SCALING;
        bool bStretchBlt = lRasterCaps & RC_STRETCHBLT;
        bool bStretchDIB = lRasterCaps & RC_STRETCHDIB;

        FILE *fDump = fopen("Printers.log", "a+");
        _ftprintf( fDump, _T("[Printer.Document.Page.BlitImage]\n") );
        _ftprintf( fDump, _T("Raster Caps: bScaling=%i, bStertchBlt=%i, bStertchDIB=%i\n"),
                bScaling, bStretchBlt, bStretchDIB );
        fclose( fDump );

        if ( !bStretchDIB )
            return;

        const Surface *pS = image.GetSurface();
        if ( !pS )
        {
            image.Cooperate( MemSurface() );
            pS = image.GetSurface();
        }
        SPointer< PixelTransfer > pT = pS->CreateReader( Surface::COLOR );
        if ( !pT )
        {
            image.Cooperate( MemSurface() );
            pT = pS->CreateReader( Surface::COLOR );
        }
        Rect sR = image.GetRect();
        int width, height, pixFmt, numColors;
        const void *palette;
        int bytesPerPel = 3;
        int outPixFmt = ImageFormat::B8G8R8;
        pT->GetSource( &width, &height, &pixFmt, &palette, &numColors );
        pT->SetDestination( width, height, bytesPerPel * sR.GetW(), outPixFmt );
        pT->Transform();

        /* (1) */
        g_vGDIPrinterData.resize( sR.GetW() * sR.GetH() * bytesPerPel + sizeof( ::BITMAPINFOHEADER ) );
        ::BITMAPINFO *bitmap = (::BITMAPINFO*)(&g_vGDIPrinterData[0]);
        bitmap->bmiHeader.biSize = sizeof( ::BITMAPINFOHEADER );
        bitmap->bmiHeader.biWidth = sR.GetW();
        bitmap->bmiHeader.biHeight = sR.GetH();
        bitmap->bmiHeader.biPlanes = 1;
        bitmap->bmiHeader.biBitCount = 24;
        bitmap->bmiHeader.biCompression = BI_RGB;
        bitmap->bmiHeader.biSizeImage = sR.GetW() * sR.GetH() * bytesPerPel;
        bitmap->bmiHeader.biXPelsPerMeter = 0;
        bitmap->bmiHeader.biYPelsPerMeter = 0;
        bitmap->bmiHeader.biClrUsed = 0;
        bitmap->bmiHeader.biClrImportant = 0;
        pT->Store( bitmap->bmiColors, sR );

        if ( g_bRotate90 )
        {
            g_vGDIPrinterData2 = g_vGDIPrinterData;
            ::BITMAPINFO *bitmap2 = (::BITMAPINFO*)(&g_vGDIPrinterData2[0]);
            std::swap( bitmap2->bmiHeader.biWidth, bitmap2->bmiHeader.biHeight );
        
            char *linePtr = (char*)(bitmap->bmiColors);
            char *linePtr2 = (char*)(bitmap2->bmiColors);
            int pitch = bitmap->bmiHeader.biWidth * bytesPerPel;
            int pitch2 = bitmap2->bmiHeader.biWidth * bytesPerPel;
            for ( int y = 0; y < sR.GetH(); ++y )
            {
                char *pixPtr = linePtr;
                char *pixPtr2 = linePtr2;
                for ( int x = 0; x < sR.GetW(); ++x )
                {
                    pixPtr2[0] = pixPtr[0];
                    pixPtr2[1] = pixPtr[1];
                    pixPtr2[2] = pixPtr[2];
                    
                    pixPtr += bytesPerPel;
                    pixPtr2 += pitch2;
                }
                linePtr += pitch;
                linePtr2 += bytesPerPel;
            }

            bitmap = bitmap2;
        }

        if ( g_bFlipX )
        {
            char *linePtr = (char*)bitmap->bmiColors;
            int pitch = bitmap->bmiHeader.biWidth * bytesPerPel;
            for ( int y = 0; y < sR.GetH(); ++y )
            {
                char *pixPtr1 = linePtr;
                linePtr += pitch;
                char *pixPtr2 = linePtr;
                for ( int x = 0; x < sR.GetW()/2; ++x )
                {
                    pixPtr2 -= bytesPerPel;
                    std::swap( *(pixPtr1 + 0), *(pixPtr2 + 0));
                    std::swap( *(pixPtr1 + 1), *(pixPtr2 + 1));
                    std::swap( *(pixPtr1 + 2), *(pixPtr2 + 2));
                    pixPtr1 += bytesPerPel;
                }
            }
        }

        ::StretchDIBits( m_hdc, onPaper.GetX(), onPaper.GetY(), onPaper.GetW(), onPaper.GetH(),
                0, 0, bitmap->bmiHeader.biWidth, bitmap->bmiHeader.biHeight, bitmap->bmiColors, bitmap, DIB_RGB_COLORS, SRCCOPY );
        /* (1) */

        /* (2) std::string data;
           data.resize( sR.GetW() * sR.GetH() * 3 );
           pT->Store( &data[0], sR );
           char *linePtr = &data[0];
           for ( int y = 0; y < sR.GetH(); ++y )
           {
           char *pixPtr1 = linePtr;
           linePtr += bytesPerPel * sR.GetW();
           char *pixPtr2 = linePtr;
           for ( int x = 0; x < sR.GetW()/2; ++x )
           {
           pixPtr2 -= bytesPerPel;
           std::swap( *(pixPtr1 + 0), *(pixPtr2 + 2));
           std::swap( *(pixPtr1 + 1), *(pixPtr2 + 1));
           std::swap( *(pixPtr1 + 2), *(pixPtr2 + 0));
           pixPtr1 += bytesPerPel;
           }
           }
           HDC hRoot = GetDC( NULL );
           HDC hMem = CreateCompatibleDC( hRoot );
           ReleaseDC( NULL, hRoot );
           HBITMAP hbm = CreateBitmap( sR.GetW(), sR.GetH(), 1, 24, &data[0] );
           SelectObject( hMem, hbm );
           StretchBlt( m_hdc, onPaper.GetX(), onPaper.GetY(), onPaper.GetW(), onPaper.GetH(),
           hMem, 0, 0, sR.GetW(), sR.GetH(), SRCCOPY );
           DeleteObject( hbm );
           DeleteDC( hMem );
           (2) */
    }

    void GDIPage::Finish()
    {
        ::EndPage( m_hdc );
    }

    TCHAR op_convert(wchar_t w)
    {
        return (TCHAR)w;
    }

    GDIDocument::GDIDocument( ::HDC hdc, const std::basic_string< wchar_t > &title ): m_hdc( hdc )
    {
        std::basic_string<TCHAR> title_str;
        std::transform(title.begin(), title.end(), std::back_inserter(title_str), &op_convert);
        std::memset( &m_doc, 0, sizeof( ::DOCINFO ));
        m_doc.cbSize = sizeof( ::DOCINFO );
        m_doc.lpszDocName = title_str.c_str();
        m_doc.lpszOutput = NULL;
        m_doc.lpszDatatype = NULL;
        m_doc.fwType = 0;
        ::StartDoc( m_hdc, &m_doc );
    }

    GDIDocument::~GDIDocument()
    {
    }

    SPointer< GDIPage > GDIDocument::CreateGDIPage()
    {
        return new GDIPage( m_hdc );
    }

    void GDIDocument::Print()
    {
        ::EndDoc( m_hdc );
        Sleep( 0 );
    }

    SPointer< GDIDocument > GDIPrinter::CreateGDIDocument( const std::basic_string< wchar_t > &title )
    {
        GDIDocument *pDoc = new GDIDocument( m_hdc, title );
        return pDoc;
    }

    GDIPrinter::GDIPrinter( const GDIPrinterInfo &info, int paperSize, int paperOrient ): m_info( info ), m_hdc(0)
    {
        FILE *fDump = fopen("Printers.log", "a+");
        _ftprintf( fDump, _T("[Printer]\n") );

        ::DEVMODE devmode;
        std::memset( &devmode, 0, sizeof( ::DEVMODE ));
        devmode.dmSize = sizeof( ::DEVMODE );
        devmode.dmFields = DM_PAPERSIZE | DM_ORIENTATION;
        devmode.dmPaperSize = paperSize;
        //devmode.dmOrientation = paperOrient;
        devmode.dmOrientation = DMORIENT_PORTRAIT;
        g_bRotate90 = ( paperOrient == DMORIENT_LANDSCAPE );
        g_bFlipX = ( paperOrient != DMORIENT_LANDSCAPE );

        do {
#define USELESS_GDI_PRINTER_CREATE_DC( _Driver, _Name, _Devmode ) \
            _ftprintf( fDump, _T("CreateDC( %s, %s, NULL, %0x )\n"), _Driver, _Name, _Devmode ); \
                m_hdc = ::CreateDC( _Driver, _Name, NULL, _Devmode ); \
                if ( !!m_hdc ) { break; } \
                    LogError( fDump );

            USELESS_GDI_PRINTER_CREATE_DC( _T("WINSPOOL"), m_info.m_name.c_str(), &devmode );

            USELESS_GDI_PRINTER_CREATE_DC( m_info.m_driver.c_str(), m_info.m_name.c_str(), &devmode );
            USELESS_GDI_PRINTER_CREATE_DC( _T("WINSPOOL"), m_info.m_name.c_str(), &devmode );
            USELESS_GDI_PRINTER_CREATE_DC( NULL, m_info.m_name.c_str(), &devmode );

            USELESS_GDI_PRINTER_CREATE_DC( m_info.m_driver.c_str(), m_info.m_name.c_str(), NULL );
            USELESS_GDI_PRINTER_CREATE_DC( _T("WINSPOOL"), m_info.m_name.c_str(), NULL );
            USELESS_GDI_PRINTER_CREATE_DC( NULL, m_info.m_name.c_str(), NULL );

        } while( false );

        if ( m_hdc )
        {
            GDIPrintManager::Instance().RegisterGDIPrinter( this );
        }

        m_sizeOnPaperX = ::GetDeviceCaps( m_hdc, HORZRES );
        m_sizeOnPaperY = ::GetDeviceCaps( m_hdc, VERTRES );
        m_xLeft = ::GetDeviceCaps( m_hdc, PHYSICALOFFSETX );
        m_yTop = ::GetDeviceCaps( m_hdc, PHYSICALOFFSETY );
        m_inchOnPaperX = ::GetDeviceCaps( m_hdc, LOGPIXELSX );
        m_inchOnPaperY = ::GetDeviceCaps( m_hdc, LOGPIXELSY );
        m_sizeInchesX = m_sizeOnPaperX / (float)m_inchOnPaperX;
        m_sizeInchesY = m_sizeOnPaperY / (float)m_inchOnPaperY;

        _ftprintf( fDump, _T("m_info = {Printer: %s; Port: %s; Driver: %s; Local: %s}\n"),
                m_info.m_name.c_str(), m_info.m_port.c_str(), m_info.m_driver.c_str(),
                (m_info.m_local ? _T("TRUE") : _T("FALSE") ));
        _ftprintf( fDump, _T("m_hdc = %0x\n"), m_hdc );
        _ftprintf( fDump, _T("m_sizeOnPaperX = %d\n"), m_sizeOnPaperX );
        _ftprintf( fDump, _T("m_sizeOnPaperY = %d\n"), m_sizeOnPaperY );
        _ftprintf( fDump, _T("m_xLeft = %d\n"), m_xLeft );
        _ftprintf( fDump, _T("m_yTop = %d\n"), m_yTop );
        _ftprintf( fDump, _T("m_inchOnPaperX = %d\n"), m_inchOnPaperX );
        _ftprintf( fDump, _T("m_inchOnPaperY = %d\n"), m_inchOnPaperY );
        _ftprintf( fDump, _T("m_sizeInchesX = %f\n"), m_sizeInchesX );
        _ftprintf( fDump, _T("m_sizeInchesY = %f\n"), m_sizeInchesY );
        _ftprintf( fDump, _T("paperSize = %i\n"), paperSize );
        _ftprintf( fDump, _T("paperOrient = %i\n"), paperOrient );


        fclose( fDump );
    }

    GDIPrinter::~GDIPrinter()
    {
        if ( m_hdc )
        {
            GDIPrintManager::Instance().UnregisterGDIPrinter( this );
            ::DeleteDC( m_hdc );
        }
    }


    ::BOOL GDIPrinter::OnAbort( int iError )
    {
        return TRUE;
    }


    GDIPrintManager::GDIPrintManager()
    {
        std::vector< ::BYTE > printerEnum;
        ::DWORD cbNeeded1, cbNeeded2;
        ::DWORD cReturned1, cReturned2;

        ::EnumPrinters( PRINTER_ENUM_LOCAL, NULL, 2, NULL, 0, &cbNeeded1, &cReturned1 );
        ::EnumPrinters( PRINTER_ENUM_CONNECTIONS, NULL, 2, NULL, 0, &cbNeeded2, &cReturned2 );

        printerEnum.resize( cbNeeded1 + cbNeeded2 );
        ::PRINTER_INFO_2 *pInfo1 = (::PRINTER_INFO_2 *)&printerEnum[0];
        ::PRINTER_INFO_2 *pInfo2 = (::PRINTER_INFO_2 *)&printerEnum[cbNeeded1];

        ::EnumPrinters( PRINTER_ENUM_LOCAL, NULL, 2, &printerEnum[0], cbNeeded1, &cbNeeded1, &cReturned1 );
        ::EnumPrinters( PRINTER_ENUM_CONNECTIONS, NULL, 2, &printerEnum[cbNeeded1], cbNeeded2, &cbNeeded2, &cReturned2 );

        m_printers.resize( cReturned1 + cReturned2 );

        FILE *fDump = fopen("Printers.log", "a+");
        _ftprintf( fDump, _T("[Printers]\n") );

        for ( int i=0; i < cReturned1; ++i )
        {
            GDIPrinterInfo &info = m_printers[ i ];
            info.m_name = pInfo1[i].pPrinterName;
            info.m_port = pInfo1[i].pPortName;
            info.m_driver = pInfo1[i].pDriverName;
            info.m_local = true;
            _ftprintf( fDump, _T("Printer: %s; Port: %s; Driver: %s; Local: TRUE\n"),
                    info.m_name.c_str(), info.m_port.c_str(), info.m_driver.c_str() );
        }

        for ( int i=0; i < cReturned2; ++i )
        {
            GDIPrinterInfo &info = m_printers[ cReturned1 + i ];
            info.m_name = pInfo2[i].pPrinterName;
            info.m_port = pInfo2[i].pPortName;
            info.m_driver = pInfo2[i].pDriverName;
            info.m_local = false;
            _ftprintf( fDump, _T("Printer: %s Port: %s Driver: %s Local: FALSE\n"),
                    info.m_name.c_str(), info.m_port.c_str(), info.m_driver.c_str() );
        }

        fclose( fDump );

    }

    GDIPrintManager::~GDIPrintManager()
    {
    }

    BOOL CALLBACK GAbortProc( HDC hdc, int iError )
    {
        if ( 0 == iError )
            return TRUE;
        else
            return FALSE;
    }


    ::BOOL GDIPrintManager::AbortProc( ::HDC hdc, int iError )
    {
        /*for ( std::set< GDIPrinter * >::iterator it = GDIPrintManager::Instance().m_printerInstances.begin();
          it != GDIPrintManager::Instance().m_printerInstances.end(); ++it )
          {
          if ( (*it)->m_hdc == hdc )
          {
          (*it)->OnAbort( iError );
          }
          }*/
        if ( 0 == iError )
            return TRUE;
        else
            return FALSE;
    }

    void GDIPrintManager::RegisterGDIPrinter( GDIPrinter *printer )
    {
        m_printerInstances.insert( printer );
        //SetAbortProc( printer->m_hdc, (ABORTPROC)&GDIPrintManager::AbortProc );
        SetAbortProc( printer->m_hdc, GAbortProc );
    }

    void GDIPrintManager::UnregisterGDIPrinter( GDIPrinter *printer )
    {
        std::set< GDIPrinter * >::iterator it = m_printerInstances.find( printer );
        if ( it != m_printerInstances.end() )
        {
            m_printerInstances.erase( it );
        }
    }

    GDIPrintManager & GDIPrintManager::Instance()
    {
        static GDIPrintManager s_manager;
        return s_manager;
    }

    SPointer< GDIPrinter > CreateGDIPrinter( int paperSize, int paperOrient )
    {
        using namespace Useless;
        GDIPrinter *pPrinter = 0;

        FILE *fDump = fopen("Printers.log", "a+");
        TCHAR lpReturnedString[256];
        DWORD dwRes = GetProfileString( _T("windows"), _T("device"), _T("none"), lpReturnedString, 256 );
        _ftprintf( fDump, _T("windows.device = %s\n"), lpReturnedString );
        fclose( fDump );
        std::basic_string< TCHAR > strDefaultPrn( lpReturnedString );
        int comaName = strDefaultPrn.find(_T(","));
        int comaDrv = strDefaultPrn.find(_T(","), comaName+1 );
        GDIPrinterInfo prnInfo;
        prnInfo.m_name = strDefaultPrn.substr(0, comaName);
        prnInfo.m_driver = strDefaultPrn.substr( comaName+1, comaDrv-comaName-1 );
        prnInfo.m_port = strDefaultPrn.substr( comaDrv+1, -1 );
        pPrinter = new GDIPrinter( prnInfo, paperSize, paperOrient );
        if ( 0 != pPrinter->m_hdc )
        {
            return pPrinter;
        }
        delete pPrinter;

        for ( GDIPrintManager::const_iterator it = GDIPrintManager::Instance().begin();
                it != GDIPrintManager::Instance().end(); ++it )
        {
            if ( (*it).m_port != _T("MSFAX:") )
            {
                pPrinter = new GDIPrinter( *it, paperSize, paperOrient );
                if ( 0 != pPrinter->m_hdc )
                {
                    break;
                }
                delete pPrinter;
            }
        }
        return pPrinter;
    }

};//Useless

