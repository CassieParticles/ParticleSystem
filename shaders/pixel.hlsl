struct PSInput
{
    float4 position : SV_Position;
    float brightness : BRIGHTNESS;
};

struct PSOutput
{
    float4 colour : SV_Target0;
};

PSOutput Main(PSInput input)
{
    
    PSOutput output = { float4(mul(float3(1, 0, 0), input.brightness), 1) };
    
    return output;
}