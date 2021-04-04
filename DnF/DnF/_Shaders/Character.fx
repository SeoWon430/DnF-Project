#include "Header.fx"

float3 blendColor = float3(0, 0, 0);
float alpha = 1;
float2 outLineSize = float2(0.0009, 0.0006);
float4 outLineColor = float4(0, 0, 0, 0);

float4 PS_Diffuse(VertexOutput input) : SV_TARGET0
{
    float4 color = DiffuseMap.Sample(LinearSampler, input.Uv);
    color.rgb += blendColor;
    color.a *= alpha;
    
    float outLine = 8.0 * color.a;
    outLine -= DiffuseMap.Sample(LinearSampler, input.Uv + float2(outLineSize.x, 0)).a;
    outLine -= DiffuseMap.Sample(LinearSampler, input.Uv + float2(-outLineSize.x, 0)).a;
    outLine -= DiffuseMap.Sample(LinearSampler, input.Uv + float2(0, outLineSize.y)).a;
    outLine -= DiffuseMap.Sample(LinearSampler, input.Uv + float2(0, -outLineSize.y)).a;
   
    outLine -= DiffuseMap.Sample(LinearSampler, input.Uv + float2(outLineSize.x, outLineSize.y)).a;
    outLine -= DiffuseMap.Sample(LinearSampler, input.Uv + float2(-outLineSize.x, outLineSize.y)).a;
    outLine -= DiffuseMap.Sample(LinearSampler, input.Uv + float2(-outLineSize.x, outLineSize.y)).a;
    outLine -= DiffuseMap.Sample(LinearSampler, input.Uv + float2(outLineSize.x, -outLineSize.y)).a;
    
    if (outLine != 0.0 && color.a == 0.0)
        color = outLineColor;
   
    
    return color;
}
    


technique11 T0
{
    P_VP(P0, VS, PS_Diffuse)

}