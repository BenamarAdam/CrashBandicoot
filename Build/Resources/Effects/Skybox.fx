SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

RasterizerState rasterizerState
{
    FillMode = Solid;
    CullMode = None;
};

DepthStencilState depthStencilState
{
    DepthFunc = LESS_EQUAL;
};

TextureCube gCubeMap : CubeMap;

cbuffer cbChangesEveryFrame
{
	matrix matWorldViewProj : WorldViewProjection;
}

struct VS_IN
{
	float3 posL : POSITION;
};

struct VS_OUT
{
	float4 posH : SV_POSITION;
	float3 texC : TEXCOORD;
};

VS_OUT MainVS( VS_IN vIn )
{
	VS_OUT vOut = (VS_OUT)0;
	vOut.posH= mul( float4(vIn.posL,0.0f), matWorldViewProj).xyww; 
	vOut.texC = vIn.posL;

	return vOut;
}

float4 MainPS( VS_OUT pIn): SV_Target
{
	return gCubeMap.Sample(samLinear,pIn.texC);
}

technique10 Render
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, MainVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, MainPS() ) );
		SetRasterizerState(rasterizerState);
        SetDepthStencilState(depthStencilState, 0);
    }
}