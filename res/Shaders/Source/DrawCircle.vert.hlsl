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


    // Project center to clip space
    float4 clipCenter = mul(ViewProjectionMatrix, float4(circle.Position, 0.0, 1.0));

    // Compute clip-space offsets for one unit in local X and Y (perspective-correct quad)
    float4 clipRightPoint = mul(ViewProjectionMatrix, float4(circle.Position + float2(circle.Radius, 0.0), 0.0, 1.0));
    float4 clipUpPoint    = mul(ViewProjectionMatrix, float4(circle.Position + float2(0.0, circle.Radius), 0.0, 1.0));

    float4 clipRight = clipRightPoint - clipCenter;
    float4 clipUp    = clipUpPoint - clipCenter;

    // Build final clip position by offsetting the projected center
    float2 lp = localPos[vertexID];
    float4 finalClip = clipCenter + lp.x * clipRight + lp.y * clipUp;

    Output output;
    output.Position = finalClip;
    output.Color    = circle.Color;
    output.LocalUV  = lp; // keep -1..1 space for shading & edge tests
    return output;
}
