Compile:

    git submodule init

    git submodule update

    Remember to modify the files "Tarbora/Framework/Maths/glm/CMakeLists.txt"
    and "Tarbora/Framework/PhysicsEngine/bullet3/CMakeLists.txt" to set OFF all
    the tests, examples and demos so you don't have to compile unnecessary code!

    mkdir build

    cd build

    cmake ..

    make

Test:

    ./tarbora
