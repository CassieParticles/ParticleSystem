struct PSInput
{
    float4 position : SV_Position;
    float3 colour : COLOR0;
};

struct PSOutput
{
    float4 colour : SV_Target0;
};

PSOutput Main(PSInput input)
{
    
    PSOutput output = { float4(input.colour, 1) };
    
    return output;
}