This is a first attempt for linux port of cinder.

This version is so far tested under Xubuntu 13.10 running on virtualbox.

A lot of stuff missing still and possible a lot of bugs that I havent realised yet but nevertheless this version runs BasicApp BasicAppMultiWindow BasicParticles and ListenerBasic.

Probably you can run already more of the examples but havent found the time to test yet.

The most important thing to take care of when building the lib or/and the examples is to have the env variable CINDER_ROOT pointing to the root dir of your Cinder installation.

An error message will appear if you forget to do this reminding you to export the specific env variable before invoking any make commands.

You will need the following libraries GLEW, GL, X11, boost_system, boost_filesystem, z .

Tested with libglew-1.8, libboost-system1.54, libboost-filesystem1.54 .

In order to build the library :

cd $CINDER_ROOT && make or cd $CINDER_ROOT && make debug.

In order to build one of the examples:

cd example/dir && make or cd example/dir && make debug 

You can check the relevant GNUmakefiles for more options.
