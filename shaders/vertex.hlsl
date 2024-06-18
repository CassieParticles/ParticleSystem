struct VSInput
{
    float3 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_Position;
};

VSOutput Main(VSInput input)
{
    VSOutput outData = { float4(input.position, 1.0) };
    return outData;
}