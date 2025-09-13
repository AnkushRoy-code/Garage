#!/bin/bash

mkdir -p Compiled/MSL Compiled/SPIRV Compiled/DXIL

for file in Source/*.hlsl; do
    filename=$(basename "$file" .hlsl)

    echo "Compiling $filename..."

    # The compiler I use is downloaded from somewhere like this: https://github.com/libsdl-org/SDL_shadercross/actions/runs/14041160733/artifacts/2809508886
    /home/ankush/Downloads/SDL3_shadercross-3.0.0-linux-x64/bin/shadercross "$file" -o "Compiled/MSL/${filename}.msl"
    /home/ankush/Downloads/SDL3_shadercross-3.0.0-linux-x64/bin/shadercross "$file" -o "Compiled/SPIRV/${filename}.spv"
    /home/ankush/Downloads/SDL3_shadercross-3.0.0-linux-x64/bin/shadercross "$file" -o "Compiled/DXIL/${filename}.dxil"
done
