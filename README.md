# CTet

Just a basic stacker game (my specialty LOL) to help me learn C.
Using SDL2 because I'm not in the mood for Win32 + DirectX/Vulkan right now.

# Build
Currently only built for Windows using MSVC. Ensure the SDL2 devel kit is installed into your env variables properly.
You can find a copy of the SDL2 devel kit [here](https://github.com/libsdl-org/SDL/releases/latest);
select the SDL2-devel-version-VC zip,
and then point the `SDL2_DIR` environment variable to the folder inside (the one that contains the include and lib folders).
Then, just build with CMake as you normally do (`cmake -Bbuild` in the root dir and then `cmake --build build`).
As a warning, the build process is designed for ninja,
so it might not place the SDL dll file correctly in the out folder if you compile with just the Visual Studio tools.
Just move it from the bin folder into the Debug/Release subfolder.