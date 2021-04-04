cbuffer CB_PerFrame
{
    matrix View;
    matrix Projection;
};
/* Constant Buffer : c++���� shader�� �Ѿ� �� ��
        View, Projection �ΰ��� ��� �ѹ��� ����, 2�� ���� ���� 1���� ����
        View, Projection �� �� �ٲ��� ������ �޴� ���� ����
                        �ϳ��� �ٲ�� �Ѵ� �ٽ� ����
    => ���� �� �ٲ�� �͵��� ��Ƽ� �޾Ƽ� ����ȭ
*/
matrix World;
// matrix == float4x4


Texture2D DiffuseMap;
// C++�� SRV�� �޾��ִ� �ؽ��� Ÿ��, UV�� ���� �ȼ��� ������ ����



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
// ���� : ���� ��ȯ  mul(World), mul(View), mul(Projection)
//  => ������ 3���� ���� �ȿ��� ī�޶� �������� ���� ��
VertexOutput VS(VertexInput input)
{
    VertexOutput output;
                        // float4 * matrix
    output.Position = mul(input.Position, World);   // World ��ȯ
    output.Position = mul(output.Position, View);   // View ��ȯ
    output.Position = mul(output.Position, Projection); // Projection ��ȯ
    
    output.UV = input.UV;
    return output;
}


/* VS->RS->PS
    Resterizer : VS������ ������ �־�����
        �� �ܰ迡�� ���� ���̸� ���� �����Ͽ� �ȼ��� ���� (Sampling)
        ���⼭�� �޾ƿ� Position�� UV�� ���� ��
*/


// PixelShader
//-----------------------------------------------------------------------------
//SamplerState samp;    // �⺻��
SamplerState PointSampler
{
    Filter = MIN_MAG_MIP_POINT;   // �� �ȼ��� ä�� => ��Ʈ�� Ʀ
    //Filter = MIN_MAG_MIP_LINEAR;    // �翷 �ȼ��� �߰� ������ ä�� => ��Ʈ�� �ѿ�

    AddressU = Wrap;
    AddressV = Wrap;
    //AddressU = Clamp;
    //AddressV = Clamp;
    //AddressU = Mirror;
    //AddressV = Mirror;
};
// �ؽ��Ŀ� ���� Ȯ�� ��ҽ� �ȼ��� ����Ͽ� ä���ִ� ���, �ɼ�
//      Shader���� State�� �ɼ��� �ǹ�

float4 PS_POINT(VertexOutput input) : SV_TARGET0
{
    return DiffuseMap.Sample(PointSampler, input.UV);
    // �޾ƿ� �ؽ��Ŀ� ���� ���ø��Ͽ� �ȼ��� ������ ����
    //  input.UV : Ư�� �ȼ��� ��ġ(��ǥ)
}


SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;    // �翷 �ȼ��� �߰� ������ ä�� => ��Ʈ�� �ѿ�
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
    float4 c = lerp(diffuse, red, Time);    // ���� ����
                    // �Ķ����3 : 0�̸� �Ķ����1 ���� �״��
                    //              1�̸�  �Ķ����2 ���� �״��
                    //              0~1���̸� ���� ����


    return c;
}





// AlphaBlend �ؽ��� ���� ����
//-----------------------------------------------------------------------------
BlendState AlphaBlend   // �ȼ��� ���� �������� ��� ���
{
    // �ε��� : RenderTarget��ȣ (���⼭�� 1���� ���)
    BlendEnable[0] = true;          
    DestBlend[0] = INV_SRC_ALPHA;   // 1-SRC_ALPHA
    SrcBlend[0] = SRC_ALPHA;        // SRC_ALPHA : Src�� a��
    BlendOp[0] = Add;               // ������ +��

    SrcBlendAlpha[0] = One;
    DestBlendAlpha[0] = One;
    RenderTargetWriteMask[0] = 0x0F;
};
/* Blend����
    ResultColor = DestBlend * Color1 (Op) SrcBlend * Color2
        Dest : destination ���� �׷��� �̹���
        Src : source �� ���� �׷��� �̹���
*/

BlendState AlphaBlend_AlphaToCoverage
{
    AlphaToCoverageEnable = true;   // Alpha Sorting�� ���ִ� �ɼ�, ������ ������
    // => �̹����� �׵θ� �κ�(������<->����)�� �ս� ��

    // Alpha Sorting : Aplha, Depth, Position�� ���� �׸��� ������ �߿�
    //              Aplha�� �����Ͽ� �� �׸��� ������ �����ϴ� ���� �ǹ�
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
        // �� �ڵ尡 �־�� �̹����� ������ �κ��� �������� ó����
        SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);    // ����ó��

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS_POINT()));      // Point Sample
    }
    pass P1
    {
        SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);    // ����ó��

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



