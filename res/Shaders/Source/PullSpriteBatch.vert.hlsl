struct SpriteData
{
    float3 Position;
    float Rotation;
    float4 Color;
};

struct Output
{    
    float4 Color : TEXCOORD0;
    float4 Position : SV_Position;
};

StructuredBuffer<SpriteData> DataBuffer : register(t0, space0);

cbuffer UniformBlock : register(b0, space1)
{
    float4x4 ViewProjectionMatrix : packoffset(c0);
};

static const float2 vertexPos[3] = {
    {-0.5f, -1.0f },
    { 0.5f, -1.0f },
    { 0.0f,  1.0f } 
};

Output main(uint id : SV_VertexID)
{
    uint spriteIndex = id / 3;
    uint vert = id % 3;
    SpriteData sprite = DataBuffer[spriteIndex];

    float c = cos(sprite.Rotation);
    float s = sin(sprite.Rotation);

    float2 coord = vertexPos[vert];
    coord *= 16.0f; // size = 16.0f
    float2x2 rotation = {c, s, -s, c};
    coord = mul(coord, rotation);

    float3 coordWithDepth = float3(coord + sprite.Position.xy, sprite.Position.z);

    Output output;

    output.Position = mul(ViewProjectionMatrix, float4(coordWithDepth, 1.0f));
    output.Color = sprite.Color;

    return output;
}
