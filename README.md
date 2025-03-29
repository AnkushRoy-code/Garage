# Garage
A simple Garage where I put up some of my projects. That is Cross Platform. It
can run in all `Metal`/`Vulkan`/`DX11` or `DX12` idk. It's all possible just
because of SDL_GPU_API

## Compilation
Compiling is straightforward, you just need a modern C++ compiler. It has been tested with:
- `gcc`
- `clang`
- `msvc`
- `Visual Studio`

## Dependencies
- CMake (If you have a CMake version less than 3.30 then change the root `CMakeLists.txt:01` to something less)
- Git
- A C/C++ compiler that supports C++23 or more.
- Additional Dependencies (That you do not have to worry about):
  - SDL3
  - Dear ImGui
  - ImPlot
  - Tracy Profiler (Off by default)
  - miniaudio

### Clone
```bash
git clone --depth=1 https://github.com/AnkushRoy-code/Garage.git
```

> [!NOTE]
> Remove `--depth=1` if you want to clone the old stuff.

### Build

#### Generate Build files
```bash
# Go inside the directory first if you have not
cd Garage
# make the build files
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```
> [!NOTE]
> You can add `-G` flag to generate your favourite make system. For example `cmake -G "Unix Makefiles"`.

#### Compile the project
For Visual Studio users, open the `.sln` file, make sure to select `Garage.exe` as the target (as it may default to something else), and build the project from there.

```bash
cmake --build build --config Release -j 8
```
> [!NOTE]
> Building this for the first time will take a while.

### Running the Project
If compiled via CLI (not Visual Studio), the executable is located at `build/src/Main/Garage`. Run it using:
```bash
# Navigate to the executable's directory
cd src/Main && ./Garage
```
Or simply:
```bash
./src/Main/Garage
```

