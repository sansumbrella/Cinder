LINUX SPECIFIC
--------------

This version is being tested against Xubuntu 14.04 and uses glfw3 for window/context creation/destruction and mouse/keyboard interaction etc.

Currently mouse and keyboard input is relatively complete but might need to rethink in some parts.

You should be able to build and run the following examples after compiling the library:

*BasicApp, BasicAppMultiWindow, BasicParticles, ListenerBasic, EaseGallery, CustomLerp, bezierPath, SurfaceBasic, shaderTest, FBOBasic*

In theory you should be able to run all examples that do not require resource loading.. 

UPDATE: The resource / asset loading handlers now are implemented and should work in most cases but it definitely needs more testing. 
This is demonstrated on the BasicApp example but also on SurfaceBasic.

Big thanks to safetydank for his work on the [android port](https://github.com/safetydank/Cinder/). The ImageSourceFileFreeImage implementation for loading images through FreeImage comes directly from there. 

Since GLFW3 is included as a submodule you will need to run `git submodule update --init` from `$CINDER_ROOT` in order to actually fetch the source code.

You should not need to worry about building GLFW3 since this will be handled by CMake.

The most important thing to take care of when building the cinder lib or/and the examples is to have the env variable `CINDER_ROOT` pointing to the root dir of your Cinder installation.

For example:
`export CINDER_ROOT=~/Some/dir/Cinder`

NOTE: Use absolute path to point to the directory.

An error message will appear if you forget to do this reminding you to export the specific env variable before invoking any make commands.

You will need the following libraries `GLEW`, `GL`, `X11`, `Xxf86vm`, `pthread`, `Xrandr`, `Xi`, `Xcursor`, `boost_system`, `boost_filesystem`, `z`, `freeimage` .


You can run the following to install required dependencies using apt:

`sudo apt-get update && sudo apt-get install build-essential cmake libgl1-mesa-dev libxrandr-dev libxi-dev libxcursor-dev libfreeimage-dev libpng12-dev libglew-dev libboost-system1.55.0 libboost-system1.55-dev libboost-filesystem1.55.0 libboost-filesystem1.55-dev libpthread-stubs0-dev zlib1g-dev` 

You can optionally install codeblocks also if you wish to use it as an IDE since the cmake configuration generates C::B project files along with the standard Makefiles. 

`sudo apt-get install codeblocks`

In order to build the library :

`cd $CINDER_ROOT && make` or `cd $CINDER_ROOT && make debug`.

The make command should trigger the build of GLFW3 followed by Cinder.

In order to build one of the examples:

`cd example/dir && make` or `cd example/dir && make debug` 

You can check the relevant GNUmakefiles for more options.
