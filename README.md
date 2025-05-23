<br/>
<div align="center">
  <a href="https://github.com/AnkushRoy-code/Garage">
    <img src="docs/Logo/AnkushGarage.png" alt="Logo" width="80" height="80">
  </a>
  <h3 align="center">Ankush's Garage</h3>
  <p align="center">
    A collection of my Creations 
    <br/>
    <br/>
    <a href="https://ankushroy-code.github.io/Garage"><strong>Explore the docs »</strong></a>
    <br/>
    <br/>
    <a href="https://github.com/AnkushRoy-code/Garage/releases/latest">Install Latest .</a>  
    <a href="https://github.com/AnkushRoy-code/Garage/issues/new?labels=bug&amp;template=bug_report.md">Report Bug .</a>
    <a href="https://github.com/AnkushRoy-code/Garage/issues/new?labels=enhancement&amp;&template=feature_request.md">Request Feature</a>
  </p>
</div>

# Garage
A simple Garage where I put up some of my projects. That is Cross Platform. It
can run in all `Metal`/`Vulkan`/`DX11` or `DX12` idk. It's all possible just
because of SDL_GPU API

![A pic of circles](https://github.com/user-attachments/assets/44cd2197-438a-4de6-8e18-1ab0b6faaec4)

## Website
There is a website for the documentation
[here](https://ankushroy-code.github.io/Garage/), of this site that is
generated by `doxygen`, you may want to take a look at it if you... want to
know how I did things here or how a particular project works. I have them all
documented. As of now I haven't done anything, all of these shit I just wrote
are "future plans".

![A pic of circles](https://github.com/user-attachments/assets/33f8d553-f75a-42e0-85b4-32cc39350b63)

## Compilation
Compiling is straightforward, you just need a modern C++ compiler. It has been
tested with:
- `gcc`
- `clang`
- `msvc`
- `Visual Studio`

## Dependencies
- CMake (If you have a CMake version less than 3.30 then change the root
`CMakeLists.txt:01` to something less)
- Git
- A C/C++ compiler that supports C++23 or more.
- Additional Dependencies (That you do not have to worry about):
  - SDL3
  - Dear ImGui
  - ImPlot
  - Tracy Profiler (Off by default)

### Clone
```bash
git clone --depth=1 https://github.com/AnkushRoy-code/Garage.git
```

> [!TIP]
> Remove `--depth=1` if you want to clone the old stuff.

### Build

#### Generate Build files
```bash
# Go inside the directory first if you have not
cd Garage
# make the build files
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```
> [!TIP]
> You can add `-G` flag to generate your favourite make system. For example
> `cmake -G "Unix Makefiles"`.

#### Compile the project
For Visual Studio users, open the `.sln` file.

> [!IMPORTANT]
> If using Visual Studio Make sure to select `Garage.exe` as the target (as it
> may default to something else), otherwise it won't work.

Others can just do this:
```bash
cmake --build build --config Release -j 8
```
> [!NOTE]
> Building this for the first time will take a while.

### Running the Project
If compiled via CLI (not Visual Studio), the executable is located at
`build/src/Main/Garage`. Run it using:
```bash
# Navigate to the executable's directory
cd src/Main && ./Garage
```
Or simply:
```bash
./src/Main/Garage
```

## Screenshots

![A image of boid simulation](https://github.com/user-attachments/assets/541cf3f0-071e-432f-a261-5bfca31a9b3b)
![A image of triangle](https://github.com/user-attachments/assets/0c71dfad-f8e9-4658-bbaa-367db6cc8c2b)
![A image of boid simulation second](https://github.com/user-attachments/assets/60b3852a-b001-4109-9e9f-3e54b456c48e)

## Demo Video


It has some artifacts because of compression. And I am not the best at using ffmpeg or it is ffmpeg's fault for being so hard. 


https://github.com/user-attachments/assets/35f9639f-b4a0-46cf-a745-c89b1688f3c9



## Developers
[Ankush Roy](https://github.com/AnkushRoy-code)
