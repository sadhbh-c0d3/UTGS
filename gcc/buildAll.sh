#!/bin/bash
#
#
#
#   Hi, I didn't have time to create makefile files.
#   To compile cross-platform part of this library just run this script.
#
#   NOTE: This library won't compile all files, since it was written for win32.
#   
#   It is possible to make it work under linux (or any other os), since it was
#   designed as x-platorm library, but now it needs some code to be written.
#
#   In such case, one needs to add such components:
#
#   * Useless::Application: for linux i.e. X-Window System)
#   * Useless::GLXScreen: GLX version of Useless::WGLScreen
#   * Useless::ALSASoundCard or Useless::OSSSoundCard: to have sound support
#

echo -e "Building UTGS\n\n"
for f in $(find ../Useless -iname "*.cpp"); do
	echo -e "\n\nCompiling: $f\n\n";
	if [ -e ${f%cpp}o ]; then
		echo "Object allready exist: ${f%cpp}o";
		echo "Object allready exist: ${f%cpp}o" >> build.log;
	else
		g++ -I ../ -I ../Useless -c $f -o ${f%cpp}o;
		if [ -e ${f%cpp}o ]; then
			echo "Compile Success: ${f%cpp}o";
			echo "Compile Success: ${f%cpp}o" >> build.log;
		else
			echo "Compile Failure: ${f%cpp}o";
			echo "Compile Failure: ${f%cpp}o" >> build.log;
			echo "Source: $f" >> failed.log;
		fi;
	fi;
done;
