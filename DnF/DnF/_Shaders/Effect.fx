#include "Header.fx"

float4 PS_Diffuse(VertexOutput input) : SV_TARGET0
{
    float4 color = DiffuseMap.Sample(LinearSampler, input.Uv);
    return color;
}
    
technique11 T0
{
    P_VP(P0, VS, PS_Diffuse)

}