texture tex0;

sampler2D texSampler = sampler_state
{
    Texture = <tex0>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

float4 test(float2 uv : TEXCOORD0) : COLOR
{
    float4 col = tex2D(texSampler, uv);
    
    float gray = dot(col.rgb, float3(0.299, 0.587, 0.114)); // ‹P“xŒvŽZ
    float4 outCol;
    outCol.rbg = gray;
    outCol.a = col.a;
    
    return outCol;
}

technique tech
{
    pass P0
    {
        PixelShader = compile ps_3_0 test();
    }
}