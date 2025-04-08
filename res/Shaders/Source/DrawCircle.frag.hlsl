struct Input
{
    float4 Color : TEXCOORD0;
    float2 LocalUV : TEXCOORD1;
};

float4 main(Input input) : SV_Target0
{
    float dist = length(input.LocalUV);

    float fade = 0.079;
    float color = smoothstep(1.0 - fade, 1.0, dist);

    return float4(input.Color.rgb, input.Color.a * (1.0 - color));
}
