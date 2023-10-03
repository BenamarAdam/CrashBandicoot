float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float4x4 gWorldViewProj_Light;
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float gShadowMapBias = 0.01f;
float4x4 gBones[70];

Texture2D gDiffuseMap;
Texture2D gShadowMap;

SamplerComparisonState cmpSampler
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// or Mirror or Clamp or Border
    AddressV = Wrap;// or Mirror or Clamp or Border
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 BoneIndices : BLENDINDICES;
	float4 BoneWeights : BLENDWEIGHTS;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 lPos : TEXCOORD1;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//TODO: complete Vertex Shader 
	//Hint: use the previously made shaders PosNormTex3D_Shadow and PosNormTex3D_Skinned as a guide
	float4 pos = float4(0, 0, 0, 0);
	float3 norm = float3(0, 0, 0);

	for (int i{}; i < 4; ++i)
	{
		float idx = input.BoneIndices[i];
		if (idx < 0)
		{
			continue;
		}

		pos += float4(mul(float4(input.pos, 1.f), gBones[idx]).xyz, 1.f) * input.BoneWeights[i];
		norm += mul(input.normal, (float3x3) gBones[idx]) * input.BoneWeights[i];
	}
	norm = normalize(norm);

	output.pos = mul(pos, gWorldViewProj);
	output.normal = normalize(mul(norm, (float3x3) gWorld));
	output.texCoord = input.texCoord;
	output.lPos = mul(pos, gWorldViewProj_Light);

	return output;
}

float2 texOffset(int u, int v)
{
	//TODO: return offseted value (our shadow map has the following dimensions: 1280 * 720)
	return float2(float(u) / 1280.f, float(v) / 720.f);
}

float EvaluateShadowMap(float4 lpos)
{
	//TODO: complete
	lpos.xyz /= lpos.w;

	if (lpos.x < -1.0f || lpos.x>1.0f || lpos.y < -1.0f || lpos.y>1.0f || lpos.z < 0.0f || lpos.z>1.0f)
	{
		return 0.f;
	}
	lpos.x = lpos.x / 2 + 0.5;
	lpos.y = lpos.y / -2 + 0.5;
	lpos.z -= gShadowMapBias;

	float total = 0;

	for (float y = -1.5f; y <= 1.5f; y += 1.f)
	{
		for (float x = -1.5f; x <= 1.5f; x += 1.f)
		{
			total += gShadowMap.SampleCmpLevelZero(cmpSampler, lpos.xy + texOffset(x, y), lpos.z);
		}
	}

	return (total / 16.0f) * 0.5 + 0.5;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float shadowValue = EvaluateShadowMap(input.lPos);

	float4 diffuseColor = gDiffuseMap.Sample( samLinear,input.texCoord );
	float3 color_rgb= diffuseColor.rgb;
	float color_a = diffuseColor.a;
	
	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;

	return float4( color_rgb * shadowValue, color_a );
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

