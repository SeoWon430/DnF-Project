cbuffer CB_PerFrame
{
    matrix View;
    matrix Projection;
};
matrix World;


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

RasterizerState CullMode_None
{
    CullMode = None;            // 컬링 x => 반시계로 그려진 면도 그려줌
    DepthClipEnable = false;    // Otho로 잘리는 면이 없게 설정
};
// Culling : polygon 폐기
// Clip : pixel 폐기

float4 PS(VertexOutput input) : SV_TARGET0
{
    return DiffuseMap.Sample(LinearSampler, input.UV);
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
        SetRasterizerState(CullMode_None);
        SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS())); // Point Sample
    }
}



