struct Input
{
    float4 Color : TEXCOORD0;
    float2 LocalUV : TEXCOORD1;
};

float4 main(Input input) : SV_Target0
{    
	float2 uv = input.LocalUV;
    float dist = length(input.LocalUV);

    if (dist > 1.0) discard;

    // Edge anti-aliasing
    float fade = 0.079; // keep similar to your previous value
    float edge = smoothstep(1.0 - fade, 1.0, dist);
    float alpha = input.Color.a * (1.0 - edge);

    // Reconstruct sphere normal (z >= 0)
    float nz = sqrt(max(0.0, 1.0 - dist * dist));
    float3 normal = normalize(float3(uv.x, uv.y, nz));

    // Simple lighting in view/screen-space (plausible shading):
    // light coming from top-right/front
    float3 lightDir = normalize(float3(0.5, 0.5, 1.0));

    // Ambient + diffuse
    float ambient = 0.12;
    float dif = max(dot(normal, lightDir), 0.0);

    // Specular (Blinn-Phong)
    float3 viewDir = float3(0.0, 0.0, 1.0); // camera points toward +z in this space
    float3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfDir), 0.0), 32.0);

    float3 baseColor = input.Color.rgb;
    float3 lit = baseColor * (ambient + dif * 0.88) + float3(1.0, 1.0, 1.0) * spec * 0.4;

    return float4(lit, alpha);
}
