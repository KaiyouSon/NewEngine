#include "ColliderObject.hlsli"

V2P main(Appdata i)
{
    float4 vpos = mul(wvpMat, i.pos);

    // �s�N�Z���V�F�[�_�[�ɓn���l
    V2P output = (V2P) 0;
    output.svpos = vpos;
    return output;
}