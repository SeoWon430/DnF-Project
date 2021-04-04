cbuffer CB_PerFrame
{
    matrix View;
    matrix Projection;
};
matrix World;

float4 Color = float4(1, 1, 1, 1);



// Types
//-----------------------------------------------------------------------------
struct VertexInput
{
    float4 Position : POSITION0;
};

struct VertexOutput
{
    float4 Position : SV_POSITION0;
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
    
    return output;
}


// PixelShader
//-----------------------------------------------------------------------------

float4 PS(VertexOutput input) : SV_TARGET0
{
    return Color;
}





// Pipline
//-----------------------------------------------------------------------------
technique11 T0
{
    pass P0
    {

        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS())); // Point Sample
    }
}



