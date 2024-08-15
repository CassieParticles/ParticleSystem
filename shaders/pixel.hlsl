struct PSInput
{
    float4 position : SV_Position;
};

struct PSOutput
{
    float4 colour : SV_Target0;
};

PSOutput Main(PSInput input)
{
    
    PSOutput output = { float4(float3(1, 0, 0), 1) };
    
    return output;
}