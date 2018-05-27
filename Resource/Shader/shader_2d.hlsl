//�O���[�o��

Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

cbuffer global
{
    row_major matrix g_W : packoffset(c0); //���[���h����ˉe�܂ł̕ϊ��s��
    float2 g_ViewPort : packoffset(c4); //�r���[�|�[�g�i�X�N���[���j�T�C�Y
};
//�\����
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

//
//
//�o�[�e�b�N�X�V�F�[�_�[
PS_INPUT VS(float3 Pos : POSITION, float2 UV : TEXCOORD)
{
    PS_INPUT Out;

    Out.Pos = mul(float4(Pos, 1.f), g_W);

    Out.Pos.x = (Out.Pos.x / g_ViewPort.x) * 2;
    Out.Pos.y = (Out.Pos.y / g_ViewPort.y) * 2;

    Out.UV = UV;

    return Out;
}
//
//
//�s�N�Z���V�F�[�_�[
float4 PS(PS_INPUT Input) : SV_Target
{
    float4 color = g_Texture.Sample(g_Sampler, Input.UV);

    return color;
}