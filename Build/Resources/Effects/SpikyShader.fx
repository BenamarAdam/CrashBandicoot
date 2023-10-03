//************
// VARIABLES *
//************
float4x4 m_MatrixWorldViewProj : WORLDVIEWPROJECTION;
float4x4 m_MatrixWorld : WORLD;
float3 m_LightDirection : DIRECTION
<
	string Object="TargetLight";
> = float3(0.577f, -0.577f, 0.577f);

float3 gColorDiffuse : COLOR = float3(1,1,1);

float gSpikeLength
<
	string UIWidget="Slider";
	float UIMin=0.0f;
	float UIMax=0.5f;
	float UIStep=0.0001f;
> = 0.2f;

RasterizerState FrontCulling 
{ 
	CullMode = FRONT; 
};

//**********
// STRUCTS *
//**********
struct VS_DATA
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
};

struct GS_DATA
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL;
};

//****************
// VERTEX SHADER *
//****************
VS_DATA MainVS(VS_DATA vsData)
{
	return vsData;
}

//******************
// GEOMETRY SHADER *
//******************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float3 normal)
{
	GS_DATA output = (GS_DATA)0;
	output.Position = mul(float4(pos,1.0f),m_MatrixWorldViewProj);
	output.Normal = mul(normal,m_MatrixWorld);
	triStream.Append(output);
}

[maxvertexcount(9)]
void SpikeGenerator(triangle VS_DATA vertices[3], inout TriangleStream<GS_DATA> triStream)
{
	//Use these variable names
    float3 basePoint, top, left, right, spikeNormal;
	
	basePoint = (vertices[0].Position + vertices[1].Position + vertices[2].Position)/3;
	
	spikeNormal = (vertices[0].Normal + vertices[1].Normal + vertices[2].Normal)/3;
	
	basePoint = basePoint+(gSpikeLength*spikeNormal);
	
	//FACE 1
	left = vertices[0].Position;
	right = basePoint;
	top = vertices[1].Position;
	float3 faceNormal = normalize(cross(top-left,right-left));
	CreateVertex(triStream,left,faceNormal);
	CreateVertex(triStream,top,faceNormal);
	CreateVertex(triStream,right,faceNormal);
	
	triStream.RestartStrip();
	
	//FACE 2
	left = vertices[1].Position;
	right = basePoint;
	top = vertices[2].Position;
	faceNormal = normalize(cross(top-left,right-left));
	CreateVertex(triStream,left,faceNormal);
	CreateVertex(triStream,top,faceNormal);
	CreateVertex(triStream,right,faceNormal);
	
	triStream.RestartStrip();
	
	//FACE 3
	left = vertices[2].Position;
	right = basePoint;
	top = vertices[0].Position;
	faceNormal = normalize(cross(top-left,right-left));
	CreateVertex(triStream,left,faceNormal);
	CreateVertex(triStream,top,faceNormal);
	CreateVertex(triStream,right,faceNormal);
	
}

//***************
// PIXEL SHADER *
//***************
float4 MainPS(GS_DATA input) : SV_TARGET
{
    input.Normal=normalize(input.Normal);
	float diffuseStrength = max(dot(normalize(m_LightDirection),-input.Normal),0.2f); 
	return float4(gColorDiffuse*diffuseStrength,1.0f);
}

//*************
// TECHNIQUES *
//*************
technique10 Default //FXComposer >> Rename to "technique10 Default"
{
    pass p0
    {
        SetRasterizerState(FrontCulling);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(CompileShader(gs_4_0, SpikeGenerator()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}