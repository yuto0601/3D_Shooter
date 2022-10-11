#include	"psvscommon.hlsl"

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT main( float4 Pos : POSITION,
			    float4 Color : COLOR,
				float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	// ���W�ϊ�
	Pos.w = 1.0f;
	Pos = mul(Pos, World);

	// �X�N���[�����W�ɕϊ�
	output.Pos = Pos;
	output.Pos.x = (Pos.x / ViewportWidth.x) * 2 - 1.0f;
	output.Pos.y = 1.0f - (Pos.y / ViewportHeight.x) * 2;
	output.Pos.z = Pos.z;
	output.Pos.w = 1.0f;

	// �J���[�l�Z�b�g
	output.Color = Color;
	// �e�N�X�`�����W�Z�b�g
	output.Tex = Tex;
	return output;
}
