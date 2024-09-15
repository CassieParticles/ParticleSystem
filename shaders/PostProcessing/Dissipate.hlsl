struct PSInput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

struct PSOutput
{
    float4 colour : SV_Target0;
};

cbuffer Timer : register(b0)
{
    float deltaTime;
    float elapsedTime;
};

SamplerState linearsampler : register(s0);
Texture2D Texture : register(t0);

PSOutput Main(PSInput input)
{
    float offset = 1.0 / 800;
    float kernel[9] = { 
            1,1,1,
            1,3,1,
            1,1,1
        };
    
    float4 outDatas[9] = {
        Texture.Sample(linearsampler, input.uv + float2(-offset, offset)),
        Texture.Sample(linearsampler, input.uv + float2(      0, offset)),
        Texture.Sample(linearsampler, input.uv + float2( offset, offset)),
        Texture.Sample(linearsampler, input.uv + float2(-offset,      0)),
        Texture.Sample(linearsampler, input.uv + float2(      0,      0)),
        Texture.Sample(linearsampler, input.uv + float2( offset,      0)),
        Texture.Sample(linearsampler, input.uv + float2(-offset,-offset)),
        Texture.Sample(linearsampler, input.uv + float2(      0,-offset)),
        Texture.Sample(linearsampler, input.uv + float2( offset,-offset)),
        };
    
    float4 sumColour = float4(0,0,0,0);
    float sumValue = 0;
    float4 magnitude = 1.0;
    
    [unroll]
    for (int i = 0; i < 9;++i)
    {
        sumColour += outDatas[i] * kernel[i];
        sumValue += kernel[i];
    }
    sumColour /= sumValue;
    sumColour *= magnitude;
    
    PSOutput output = { sumColour };
       
    return output;

}