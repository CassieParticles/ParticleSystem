struct VSInput
{
    float3 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_Position;
};

cbuffer offset : register(b0)
{
    float3 translation;
}

VSOutput Main(VSInput input)
{
    VSOutput outData = { float4(input.position + translation, 1.0) };
    return outData;
}