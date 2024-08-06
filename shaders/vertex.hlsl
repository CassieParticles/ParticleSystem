struct VSInput
{
    float3 position : POSITION;
    uint vertexID : SV_VertexID;
};

struct VSOutput
{
    float4 position : SV_Position;
    float brightness : BRIGHTNESS;
};

cbuffer offset : register(b0)
{
    float3 translation;
}

StructuredBuffer<float> brightnessBuffer : register(t0);

VSOutput Main(VSInput input)
{
    VSOutput outData = { float4(input.position + translation, 1.0), brightnessBuffer.Load(input.vertexID) };
    return outData;
}