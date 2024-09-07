struct VSInput
{
    float2 vertexPosition:VertexPosition;
    float2 instancePosition : POSITION;
    float3 instanceColour : COLOR0;
};

struct VSOutput
{
    float4 position : SV_Position;
    float3 colour : COLOR0;
};

cbuffer WindowSize : register(b0)
{
    int width;
    int height;
};

cbuffer ParticleData : register(b1)
{
    int particleSize;
    int pixelsPerUnit;
};

cbuffer Timer : register(b2)
{
    
}

VSOutput Main(VSInput input)
{
    VSOutput data;
    
    float2 vertexPosition = input.vertexPosition * particleSize + input.instancePosition * pixelsPerUnit;   //Gets size between 0->WindowSize
    float2 invSize = float2(1.0 / width, 1.0 / height);
    
    vertexPosition *= invSize;  //Scales position to be between 0->1
    
    vertexPosition *= 2;    //Scales and translates position to be between -1->1
    vertexPosition -= 1;
    
    data.position = float4(vertexPosition, 0, 1);
    data.colour = input.instanceColour;
    
    return data;
}