wic readme
===========
Table of Contents
----------------- 

1. Summary
2. Directories and Files
3. Building wic
4. Using wic in your project
5. Conventions
6. Licensing and Distribution
7. Credits

Summary
-------
Wic is a 2D game engine for MacOS written in C++ and distributed as a standalone static library. This is primarily a hobby project, but I hope to make some neat games. 

You can download files, read documentation, and submit bug reports at wic's [github repository](https://github.com/wolearyc/wic). 

Enjoy!

Directories and Files
---------------------

* license.md -- License. 
* readme.md -- Readme. 
* Makefile -- Makefile. 
* deps/ -- Dependency files.
    * lib/ -- Dependency libraries.
	* include/ -- Dependency header files.
* doc/ -- Doxygen documentation.
* include/ -- Header files.
* lib/ -- Dependency libraries.
* src/ -- Source files.
* bin/ -- Libraries [created on build].
    * debug/ -- Debug library (debug/libwic.a) [created on build].
    * release/ -- Release library (release/libwic.a) [created on build].
* obj/ -- Object files [created on build].
    * debug/ -- Debug objects [created on build].
    * release/ -- Release objects [created on build].
	
Building wic
-------------
Wic uses the standard "make" to build. No configure script is included, mostly because the makefile is so small and easy to modify. It should build on any system with gcc, make, and binutils installed. 
From wic's root directory, the following make commands can be executed:

* $ make -- Builds wic as a static library.
* $ make all -- Functions identically to "$ make".
* $ make release -- Functions identically to "$ make".
* $ make debug -- Builds wic as a static library with debug symbols.
* $ make doxygen -- Generates wic's doxygen documentation.
* $ make clean -- Removes all library and object files.

Using wic in your project
--------------------------
First, you'll need to include wic's header files.

1. Copy include/ and deps/ into your project.
2. In your project, delete deps/lib to save space. You won't be needing it.
3. Add include/ and deps/include as include paths. In gcc, this can be done with "-I include/ -I deps/include", assuming include/ and deps/ are in your working directory.

Second, you'll need to link in the appropriate libraries and frameworks.

1. From wic's root directory, run $make.
2. Copy libwic.a from lib/release/ into your project. 
3. Link libwic.a to your project. In gcc, this is done with "-lwic" assuming that libwic.a is in your working directory.
4. You'll also need to link in the Cocoa, Quartz, IOKit, and OpenGL frameworks. In gcc, this can be done with "-framework Cocoa -framework Quartz -framework IOKit -framework OpenGL".

Lastly, you'll need to include "wic_lib.h" in all the code you write. Then you should be good to go!

Conventions
-----------
Wic aims to be extremely consistent, so it has a number of conventions.
* wic uses camelCase
* private members have an underscore suffix
* function parameters that share a name with a public member have an underscore suffix
* public members are only public when they can modified freely and safely. Generally, Wic uses safe, public members (rather than getter/setter methods) in the interest of simplicity and readability.

Licensing and Distribution
--------------------------
Wic is distributed under the GNU Lesser General Public License, Version 3. You must include license.md in all projects which use the entirety or sections of wic.

Credits
-------
Portions of this software are copyright © 2013 The FreeType Project.  All rights reserved. [Website.](http://www.freetype.org/)

Portions of this software use GLFW. Copyright © 2002-2006 Marcus Geelnard and Copyright © 2006-2011 Camilla Berglund. [Website.](http://www.glfw.org/index.html)

Portions of this software use the Simple OpenGL Image Library, which is in the public domain. [Website.](http://www.lonesock.net/soil.html)
