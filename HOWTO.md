[[Category:BerliosProject:Utgs]]

=This is HOWTO build UTGS library on Windows=

Current version builds with Microsoft Visual C++ 2010.

Project home page: [[BerliosProject:Utgs]]

==Configuration==
    
utgs-config.h contains build configuation.
Alter it to fit your environment.

===Preprocessor definitions===

* USELESS_HAS_DSHOW

Requires Direct X

* USELESS_HAS_VORBIS

Requires Ogg/Vorbis

* USELESS_HAS_CRYPTOPP

Requires Crypto++

* USELESS_HAS_BOOST_SERIALIZATION

Requires Boost library.

'''NOTE''' Boost library is required anyway if you want to build koolib project.

* USELESS_HAS_LIBPNG

Requires libpng and zlib
These libraries are part of GnuWin32

====Default Useless::Screen class implementation====

* USELESS_DEFAULT_SCREEN_GDI

Windows GDI implementation of Useless::Screen class.

* USELESS_DEFAULT_SCREEN_WGL

Windows OpenGL implementation of Useless::Screen class.

'''NOTE''' There is also Useless::Screen implementation for DirectDraw 7. However since Microsoft deprecated DirectDraw 7 classes are not included in the MSVC10 solution.

'''IMPORTANT''' Exactly one option must be selected

====Build type====

NOTE By default static library is created.

=====Static Library=====

* UTGS_STATIC_MSVC10

Should be included in all UTGS projects when building static libs.

'''IMPORTANT''' Project that wants to use UTGS as static lib must define UTGS_STATIC_MSVC10.

=====Dynamic library (DLL)=====

* UTGS_DLL_MSVC10

Project that wants to use UTGS as DLL must define UTGS_DLL_MSVC10.

* USELESS_DLL_MSVC10

Should be used only when building DLL of Useless project.

* USELESS_RESOURCES_DLL_MSVC10

Should be used only when building DLL of Useless.Resources project.

* KOOLIB_DLL_MSVC10

Should be used only when building DLL of koolib project.

==Prerequisites==


===BOOST library [Required]===

* Download BOOST library from http://www.boost.org/
* You don't need to build it
* Set environement variable BOOST_ROOT (e.g BOOST_ROOT = C:\Dev\boost_1_47_0\)


===CRYPTO++ library [Optional - OFF by default]===

* To enable edit utgs-config.h and uncomment: #define USELESS_HAS_CRYPTOPP
* Download CRYPTO++ library from http://www.cryptopp.com/
* Set environment variable CRYPTOPP_ROOT (e.g. CRYPTOPP_ROOT = C:\Dev\CryptoPP\)


===OGG/VORBIS library [Optional - ON by default]===

* To disable edit utgs-config.h and comment: #define USELESS_HAS_VORBIS
* Download vorbis-sdk-1.0rc2.zip from http://developer.berlios.de/project/showfiles.php?group_id=3883
* Set environment variable OGGVORBIS_ROOT (e.g. OGGVORBIS_ROOT = C:\Dev\vorbis-sdk-1.0rc2\sdk)


===LIBPNG/ZLIB library [Optional - ON by default]===

* To disable edit utgs-config.h and comment: #define USELESS_HAS_LIBPNG
* Download GnuWin32.rar from http://developer.berlios.de/project/showfiles.php?group_id=3883
* Set environment variable LIBPNG_ROOT (e.g. C:\Dev\GnuWin32)


==Building==


===Building on Microsoft Visual C++ 2010 Express===

* Open solution file: /vc10/Useless.sln
* There are following targets:
** Debug 
** Release
* Choose one target and build solution

===Running example===

* Go to "KooLiXP Property Pages -> Debugging" and set:
* Command Arguments: "start.xlp"
* Working Directory: "..\..\examples\tests\BigRange"
* Set KooLiXP as Startup Project
* Copy: ogg.dll, vorbis.dll and zlib.dll files to directory where KooLiXP.exe was built.
* Now you should be able to run it ("Debug -> Start Debugging")
* Messages will go to "Output" window


==Running KooLiXP programs==

Simply use KooLiXP.exe with koolixp program.

===Example run===

 C:\Work\utgs> cd examples\tests\BigRange
 C:\Work\utgs\examples\tests\BigRange>..\..\..\vc10\KooLiXP\Release\KooLiXP.exe start.xlp


