/*!
    \mainpage
    \section intro_sec Introduction
    Tarbora is a Game Engine meant to be very easily expandable and modifiable. 
    \section install_sec Installation
    \subsection windows On Windows
    Windows isn't supported yet, but it will be very soon.
    \subsection linux On Linux
    Get the source code from GitHub:
    \code{.sh}
        git clone https://github.com/SolansPuig/Tarbora.git YourProject
        cd YourProject
    \endcode

    Initialize the submodules:
    \code{.sh}
        git submodule init
        git submodule update
    \endcode
    And modify the files "Tarbora/Framework/Maths/glm/CMakeLists.txt"
    and "Tarbora/Framework/PhysicsEngine/bullet3/CMakeLists.txt" to set OFF all
    the tests, examples and demos so you don't have to compile unnecessary code!

    Create the build directory:
    \code{.sh}
        mkdir build
        cd build
    \endcode

    Compile the source code (it will probabliy take several minutes):
    \code{.sh}
        cmake ..
        make
    \endcode

    And test it!
    \code{.sh}
        ./tarbora
    \endcode

    \section getting_started How to start
    I will set up some tutorials in the future, but for now, you'll have to figure
    everything by looking at this documentation.

    To start easy, try to modify only the main.cpp file and the Resources folder,
    and then also the files under the Demo folder.

    Do not touch anything inside the folder Tarbora until you really
    know what you are doing, but if you mess something up, just delete everything
    and repeat the installation process.
*/
