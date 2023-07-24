# CTet

Just a basic stacker game (my specialty LOL) to help me learn C.
Using SDL2 because I'm not in the mood for Win32 + DirectX/Vulkan right now.

# Build
Currently only built for Windows using MSVC or Linux using GCC.
I don't have a Mac to test, I apologize.

If you're running on Linux, don't forget to install the SDL2 shared lib if you haven't already,
as this project does not statically link to allow SDL2 to be updatable by the user.

Everything should compile and link itself. Make sure you get all the git submodules checked out.

Just build with CMake as you normally do (`cmake -Bbuild` in the root dir and then `cmake --build build`).

If you want to use Visual Studio, refer to [this guide](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio)
