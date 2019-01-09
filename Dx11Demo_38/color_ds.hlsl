////////////////////////////////////////////////////////////////////////////////
// Filename: color_ds.hlsl
////////////////////////////////////////////////////////////////////////////////

// 도메인 쉐이더는 모자이크 처리된 데이터를 받아서 버텍스 쉐이더 이전에 사용한 것과 같은 최종 꼭지점을 조작하고 
// 변환 하는 데에 사용한다.

/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

struct HullOutputType
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


////////////////////////////////////////////////////////////////////////////////
// Domain Shader
////////////////////////////////////////////////////////////////////////////////
[domain("tri")]

PixelInputType ColorDomainShader(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HullOutputType, 3> patch)
{
	float3 vertexPosition;
	PixelInputType output;
	
	// 새로운 정점의 위치를 ​​결정한다.
	vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;

	// 월드, 뷰 및 투영 행렬에 대해 새 정점의 위치를 ​​계산합니다.
    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// 입력 색상을 픽셀 쉐이더로 보낸다.
	output.color = patch[0].color;

    return output;
}