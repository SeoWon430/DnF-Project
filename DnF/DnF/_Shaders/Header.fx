//WVP
cbuffer CB_PerFrame
{
    matrix View;
    matrix Projection;
}
matrix World;

//Texture
Texture2D DiffuseMap;

//Color
float4 Color;

//VertexInput
struct VertexInput
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
};

//VertexOutput
struct VertexOutput
{
    float4 Position : SV_POSITION0;
    float2 Uv : UV0;
};

//Function
float4 WorldPosition(float4 position)
{
    return mul(position, World);
}

float4 ViewProjection(float4 position)
{
    position = mul(position, View);
    return  mul(position, Projection);
}

//VertexShader
VertexOutput VS(VertexInput input)
{
    VertexOutput output;

    output.Position = WorldPosition(input.Position);
    output.Position = ViewProjection(output.Position);

    output.Uv = input.Uv;

    return output;
}

//State
SamplerState PointSampler
{
    Filter = MIN_MAG_MIP_POINT;

    AddressU = Mirror;
    AddressV = Mirror;
};

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
};

RasterizerState CullMode_None
{
    CullMode = None;
    DepthClipEnable = false;
};

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

BlendState AlphaBlend_AlphaToCoverage
{
    AlphaToCoverageEnable = true;

    BlendEnable[0] = true;
    DestBlend[0] = INV_SRC_ALPHA;
    SrcBlend[0] = SRC_ALPHA;
    BlendOp[0] = Add;

    SrcBlendAlpha[0] = One;
    DestBlendAlpha[0] = One;
    RenderTargetWriteMask[0] = 0x0F;
};

//Pass
#define P_VP(number, vs, ps) \
pass number \
{ \
    SetRasterizerState(CullMode_None); \
    SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF); \
    \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}