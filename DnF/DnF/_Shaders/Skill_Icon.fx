cbuffer CB_PerFrame
{
    matrix View;
    matrix Projection;
};
matrix World;

bool isEnable = true;
float coolTime = 0;
Texture2D DiffuseMap;

// Types
//-----------------------------------------------------------------------------
struct VertexInput
{
    float4 Position : POSITION0;
    float2 UV : TEXCOORD0;
};

struct VertexOutput
{
    float4 Position : SV_POSITION0;
    float2 UV : TEXCOORD0;
};


// VertexShader
//-----------------------------------------------------------------------------
VertexOutput VS(VertexInput input)
{
    VertexOutput output;
                        // float4 * matrix
    output.Position = mul(input.Position, World);   // World 변환
    output.Position = mul(output.Position, View);   // View 변환
    output.Position = mul(output.Position, Projection); // Projection 변환
    
    output.UV = input.UV;
    return output;
}


// PixelShader
//-----------------------------------------------------------------------------
SamplerState PointSampler
{
    Filter = MIN_MAG_MIP_POINT;   

    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR; 
};


float4 PS(VertexOutput input) : SV_TARGET0
{
    float4 color = DiffuseMap.Sample(LinearSampler, input.UV);
    
    if (isEnable == false)
    {
        float gray = (color.r + color.g + color.b) / 6;
        color.rgb = gray;
    }
    else if (input.UV.y > 1 - coolTime)
    {
        float gray = (color.r + color.g + color.b) / 6;
        color.rgb = gray;
    }
    return color;
}




// AlphaBlend 텍스쳐 투명 설정
//-----------------------------------------------------------------------------
BlendState AlphaBlend   
{
    BlendEnable[0] = true;          
    DestBlend[0] = INV_SRC_ALPHA;   
    SrcBlend[0] = SRC_ALPHA;        
    BlendOp[0] = Add;               

    SrcBlendAlpha[0] = One;
    DestBlendAlpha[0] = One;
    RenderTargetWriteMask[0] = 0x0F;
};


// Pipline
//-----------------------------------------------------------------------------
technique11 T0
{
    pass P0
    {
        SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS())); // Point Sample
    }
}



