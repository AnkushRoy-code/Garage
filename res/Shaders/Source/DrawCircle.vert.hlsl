struct CircleData
{
    float2 Position;
    float Radius;
    float _padding;
    float4 Color;
};

StructuredBuffer<CircleData> DataBuffer : register(t0, space0);

cbuffer UniformBlock : register(b0, space1)
{
    float4x4 ViewProjectionMatrix : packoffset(c0);
};

struct Output
{    
    float4 Color : TEXCOORD0;
    float2 LocalUV  : TEXCOORD1;
    float4 Position : SV_Position;
};


Output main(uint vertexID : SV_VertexID, uint instanceID : SV_InstanceID)
{
    float2 localPos[4] = {
        float2(-1.0, -1.0),  // bottom-left
        float2( 1.0, -1.0),  // bottom-right
        float2(-1.0,  1.0),  // top-left
        float2( 1.0,  1.0)   // top-right
    };

    CircleData circle = DataBuffer[instanceID];

    float2 offset = localPos[vertexID] * circle.Radius;
    float2 worldPos = circle.Position + offset;

    Output output;
    output.Position = mul(ViewProjectionMatrix, float4(worldPos, 0.0, 1.0));
    output.Color    = circle.Color;
    output.LocalUV  = localPos[vertexID];
    return output;
}
