cbuffer CB_PerFrame
{
    matrix View;
    matrix Projection;
};
/* Constant Buffer : c++에서 shader로 넘어 올 때
        View, Projection 두개를 묶어서 한번에 받음, 2개 받을 것을 1번에 받음
        View, Projection 둘 다 바뀌지 않으면 받는 것을 생략
                        하나라도 바뀌면 둘다 다시 받음
    => 자주 안 바뀌는 것들을 모아서 받아서 최적화
*/
matrix World;
// matrix == float4x4


Texture2D DiffuseMap;
// C++의 SRV를 받아주는 텍스쳐 타입, UV에 따른 픽셀의 정보를 가짐



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
// 역할 : 공간 변환  mul(World), mul(View), mul(Projection)
//  => 정점을 3차원 공간 안에서 카메라를 기준으로 보여 줌
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


/* VS->RS->PS
    Resterizer : VS까지는 정점만 있었지만
        이 단계에서 정점 사이를 선형 보간하여 픽셀을 구성 (Sampling)
        여기서는 받아온 Position과 UV가 보간 됨
*/


// PixelShader
//-----------------------------------------------------------------------------
//SamplerState samp;    // 기본값
SamplerState PointSampler
{
    Filter = MIN_MAG_MIP_POINT;   // 옆 픽셀로 채움 => 도트가 튐
    //Filter = MIN_MAG_MIP_LINEAR;    // 양옆 픽셀의 중간 값으로 채움 => 도트가 뿌염

    AddressU = Wrap;
    AddressV = Wrap;
    //AddressU = Clamp;
    //AddressV = Clamp;
    //AddressU = Mirror;
    //AddressV = Mirror;
};
// 텍스쳐에 대한 확대 축소시 픽셀을 계산하여 채워넣는 방식, 옵션
//      Shader에서 State는 옵션의 의미

float4 PS_POINT(VertexOutput input) : SV_TARGET0
{
    return DiffuseMap.Sample(PointSampler, input.UV);
    // 받아온 텍스쳐에 대해 샘플링하여 픽셀의 색상을 리턴
    //  input.UV : 특정 픽셀의 위치(좌표)
}


SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;    // 양옆 픽셀의 중간 값으로 채움 => 도트가 뿌염
};
float4 PS_LINEAR(VertexOutput input) : SV_TARGET0
{
    return DiffuseMap.Sample(LinearSampler, input.UV);
}



float Time = 0;
float4 PS_Lerp(VertexOutput input) : SV_TARGET0
{
    float4 diffuse = DiffuseMap.Sample(PointSampler, input.UV);
    //if(diffuse.a < 1.0f)
    //    diffuse = float4(1, 0, 0, 0);

    float4 red = float4(0, 1, 1, diffuse.a);

    if(Time>1)
        Time = 0;
    float4 c = lerp(diffuse, red, Time);    // 선형 보간
                    // 파라미터3 : 0이면 파라미터1 색상 그대로
                    //              1이면  파라미터2 색상 그대로
                    //              0~1사이면 선형 보간


    return c;
}





// AlphaBlend 텍스쳐 투명 설정
//-----------------------------------------------------------------------------
BlendState AlphaBlend   // 픽셀의 색상 섞을때의 계산 방식
{
    // 인덱스 : RenderTarget번호 (여기서는 1개만 사용)
    BlendEnable[0] = true;          
    DestBlend[0] = INV_SRC_ALPHA;   // 1-SRC_ALPHA
    SrcBlend[0] = SRC_ALPHA;        // SRC_ALPHA : Src의 a값
    BlendOp[0] = Add;               // 연산은 +로

    SrcBlendAlpha[0] = One;
    DestBlendAlpha[0] = One;
    RenderTargetWriteMask[0] = 0x0F;
};
/* Blend수식
    ResultColor = DestBlend * Color1 (Op) SrcBlend * Color2
        Dest : destination 먼저 그려진 이미지
        Src : source 그 위에 그려질 이미지
*/

BlendState AlphaBlend_AlphaToCoverage
{
    AlphaToCoverageEnable = true;   // Alpha Sorting을 해주는 옵션, 성능이 않좋음
    // => 이미지의 테두리 부분(불투명<->투명)이 손실 됨

    // Alpha Sorting : Aplha, Depth, Position에 따른 그리는 순서가 중요
    //              Aplha와 관련하여 위 그리는 순서를 정렬하는 것을 의미
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
        // 이 코드가 있어야 이미지의 투명한 부분이 투명으로 처리됨
        SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);    // 투명처리

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_POINT()));      // Point Sample
    }
    pass P1
    {
        SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);    // 투명처리

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_LINEAR()));     // Linear Sample
    }
    pass P2
    {
        SetBlendState(AlphaBlend_AlphaToCoverage, float4(0, 0, 0, 0), 0xFF);    // Alpha Sorting

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_LINEAR())); // Linear Sample
    }

    pass P3
    {
        SetBlendState(AlphaBlend_AlphaToCoverage, float4(0, 0, 0, 0), 0xFF); // Alpha Sorting

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_Lerp())); // Linear Sample
    }
}



