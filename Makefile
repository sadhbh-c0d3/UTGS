SDKOUTDIR=SDKout

all: sdkinc sdklib sdkbin sdkkoolixp sdkdoc

sdkinc: sdkinckoolib sdkincuseless sdkincdynamo sdkincstrmbase 
	mkdir -p $(SDKOUTDIR)/include

sdkinckoolib:
	find koolib -type d -not -ipath "*.svn*" -exec mkdir -p $(SDKOUTDIR)/include/{} \;;
	find koolib -type f -iname "*.h" -exec cp {} $(SDKOUTDIR)/include/{} \;;

sdkincuseless:
	find Useless -type d -not -ipath "*.svn*" -exec mkdir -p $(SDKOUTDIR)/include/{} \;;
	find Useless -type f -iname "*.h" -exec cp {} $(SDKOUTDIR)/include/{} \;;

sdkincdynamo:
	find Dynamo -type d -not -ipath "*.svn*" -exec mkdir -p $(SDKOUTDIR)/include/{} \;;
	find Dynamo -type f -iname "*.h" -exec cp {} $(SDKOUTDIR)/include/{} \;;

sdkincstrmbase:
	find strmbase -type d -not -ipath "*.svn*" -exec mkdir -p $(SDKOUTDIR)/include/{} \;;
	find strmbase -type f -iname "*.h" -exec cp {} $(SDKOUTDIR)/include/{} \;;

sdklib:
	mkdir -p $(SDKOUTDIR)/lib
	cp vc7.1/UselessDll/Release\ DLL/UselessDll.lib $(SDKOUTDIR)/lib/utgsbase.lib
	cp vc7.1/UselessDll/Release\ DLL/UselessResourcesDll.lib $(SDKOUTDIR)/lib/utgsres.lib
	cp vc7.1/koolib/Release\ DLL/koolib.lib $(SDKOUTDIR)/lib/utgskl.lib

sdkbin:
	mkdir -p $(SDKOUTDIR)/bin
	cp vc7.1/bin/Release/utgsbase.dll $(SDKOUTDIR)/bin/utgsbase.dll
	cp vc7.1/bin/Release/utgsres.dll $(SDKOUTDIR)/bin/utgsres.dll
	cp vc7.1/bin/Release/utgskl.dll $(SDKOUTDIR)/bin/utgskl.dll
	cp vc7.1/bin/Release/KooLiXP.exe $(SDKOUTDIR)/bin/KooLiXP.exe

sdklibd:
	mkdir -p $(SDKOUTDIR)/lib
	cp vc7.1/UselessDll/Debug\ DLL/UselessDll.lib $(SDKOUTDIR)/lib/utgsbased.lib
	cp vc7.1/UselessDll/Debug\ DLL/UselessResourcesDll.lib $(SDKOUTDIR)/lib/utgsresd.lib
	cp vc7.1/koolib/Debug\ DLL/koolib.lib $(SDKOUTDIR)/lib/utgskld.lib

sdkbind:
	mkdir -p $(SDKOUTDIR)/bin
	cp vc7.1/bin/Debug/utgsbased.dll $(SDKOUTDIR)/bin/utgsbased.dll
	cp vc7.1/bin/Debug/utgsresd.dll $(SDKOUTDIR)/bin/utgsresd.dll
	cp vc7.1/bin/Debug/utgskld.dll $(SDKOUTDIR)/bin/utgskld.dll
	cp vc7.1/bin/Debug/KooLiXPd.exe $(SDKOUTDIR)/bin/KooLiXPd.exe

sdkkoolixp:
	mkdir -p $(SDKOUTDIR)/samples/Useless/KooLiXP
	find KooLiXP -type d -not -ipath "*.svn*" -exec mkdir -p $(SDKOUTDIR)/samples/Useless/{} \;;
	find KooLiXP -type f -iname "*.h"   -exec cp {} $(SDKOUTDIR)/samples/Useless/{} \;;
	find KooLiXP -type f -iname "*.cpp" -exec cp {} $(SDKOUTDIR)/samples/Useless/{} \;;
	cp vc7.1/KooLiXP/KooLiXP.rc $(SDKOUTDIR)/samples/Useless/KooLiXP/KooLiXP.rc
	cp vc7.1/KooLiXP/resource.h $(SDKOUTDIR)/samples/Useless/KooLiXP/resource.h
	cp vc7.1/KooLiXP/bitmap1.bmp $(SDKOUTDIR)/samples/Useless/KooLiXP/bitmap1.bmp
	cp vc7.1/KooLiXP/icon1.ico $(SDKOUTDIR)/samples/Useless/KooLiXP/icon1.ico

sdkdoc:
	mkdir -p $(SDKOUTDIR)/www
	find www -type d -not -ipath "*.svn*" -exec mkdir -p $(SDKOUTDIR)/{} \;;
	find www -type f -iname "*.xml" -exec cp {} $(SDKOUTDIR)/{} \;;
	find www -type f -iname "*.xsl" -exec cp {} $(SDKOUTDIR)/{} \;;
	find www -type f -iname "*.html" -exec cp {} $(SDKOUTDIR)/{} \;;
	find www -type f -iname "*.jpg" -exec cp {} $(SDKOUTDIR)/{} \;;
	find www -type f -iname "*.png" -exec cp {} $(SDKOUTDIR)/{} \;;
	mv $(SDKOUTDIR)/www $(SDKOUTDIR)/doc

