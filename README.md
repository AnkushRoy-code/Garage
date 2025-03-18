# Garage
A simple Garage where I (plan to) put up all of my past projects. That is Cross
Platform and Cross GPU_API? It can run in all `Metal`/`Vulkan`/`DX11` or `DX12` IDK
It's all possible just because of SDL_GPU_API

## Yapping
You can compile it easily if you have a newer version of `c++ compiler` I have
tested it to have been compiled by `gcc`, `clang`, `msvc`, `Visual Studio`.

Also you'd need `CMake` I can't think of any other dependency. But if something
doesn't compile please open an issue I'll look into that. All other libraries
are auto fetched using `CPM` that also is fetched using `CMake` so you don't
have to worry about most of the dependencies :)

Yeah you also need `git` to clone the repo or just download the `.zip` file
from Github, I might talk useless stuff but I am not teaching you how to do
that.

Okay, now. The commands.

### Clone
```bash
git clone --depth=1 https://github.com/AnkushRoy-code/Garage.git
```
Remove `--depth=1` if you want to work on my Garage.

### Build

#### Generate Build files
```bash
# Go inside the directory first if you have not
cd Garage
# make the build files
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

You can add `-G` flag to generate your favourite make system. For example
`cmake -G "Unix Makefiles"`.

#### Compile the project
```bash
cmake --build build --config Release -j 8
```
Building will take time because it builds the whole SDL3 repo. A very big
project. Takes about 3 mins in my potato lappy.

If you're using Visual Studio just open the `.sln` file in you VS and compile & run it.

#### Running the project
The compiled executable is present in `build/src/Main/Garage` if you compiled
it using the `cli` and not `Visual Studio`, you can run it by either cd to
`src/Main` and `./Garage` or just directly do `./src/Main/Garage` both of these work.
