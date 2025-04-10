cbuffer ScreenParams : register(b0, space1)
{
    float2 screenResolution;
};

struct FragmentInput
{
    float time : TEXCOORD0;
    float4 Position : SV_Position;
};

float4 main(FragmentInput input) : SV_Target0
{
    float2 uv = input.Position.xy / screenResolution;
    
    float frequency = 8.0;
    float phaseOffset = 2.09439;
    
    float r = 0.5 + 0.5 * sin(uv.x * frequency + input.time);
    float g = 0.5 + 0.5 * sin(uv.y * frequency + input.time + phaseOffset);
    float b = 0.5 + 0.5 * sin((uv.x + uv.y) * frequency + input.time + 2.0 * phaseOffset);
    
    return float4(r, g, b, 1.0);
}
