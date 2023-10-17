#include "BoundingBox.hlsli"

Texture3D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

// ���C����{�b�N�X�ɓ�����ŋߐړ_�ƍŉ��_
float2 ClucRayToBoxCrossPoint(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir);

// ���C�}�[�`���O
uint RayMarching(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir, Texture3D<float4> tex);

float4 main(V2P i) : SV_TARGET
{
    float3 rayStart = cameraPos;
    float3 rayDir = normalize(i.wpos - cameraPos);
        
    // �{�b�N�X�̍ő�ŏ��l
    float3 boundsMin = float3(-0.5f, -0.5f, -0.5f);
    float3 boundsMax = float3(+0.5f, +0.5f, +0.5f);

    uint hitCount = RayMarching(boundsMin, boundsMax, rayStart, rayDir, tex);
    
    float4 texColor = tex.Sample(smp, i.uvw);
    float4 resultColor = smoothstep(smoothingClamp.x, smoothingClamp.y, texColor * hitCount * fogColorRate);
    
    // ���C�}�[�`���O�ŋ���`�悷�鏈��
    //{
    //    float3 v = rayStart - float3(0, 0, 0);
    //    float dot1 = dot(v, rayDir);
    //    float dot2 = dot(v, v) - (0.5f * 0.5f);
    
    //    float discr = (dot1 * dot1) - dot2;
    //    if (dot1 > 0 && dot2 > 0)
    //    {
    //        resultColor = 0;
    //    }
    //    else if (discr < 0)
    //    {
    //        resultColor = 0;
    //    }
    //    else
    //    {
    //        resultColor = 1;
    //    }
    //}
    
    
    return resultColor;
}

// ���C����{�b�N�X�ɓ�����ŋߐړ_�ƍŉ��_
float2 ClucRayToBoxCrossPoint(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir)
{
    // �e���iX�AY�AZ�j�ɂ��Č�_�͈̔͂��v�Z
    float3 t0 = (boundsMin - rayStart) / rayDir;
    float3 t1 = (boundsMax - rayStart) / rayDir;
    
    // ��_�͈͂�min,max�ɕ�����
    float3 tmin = min(t0, t1);
    float3 tmax = max(t0, t1);
    
    // ���C���{�b�N�X���ɓ���A�ŏ��Ɍ�������p�����[�^T���Z�o�i�����j
    float nearestLength = max(max(tmin.x, tmin.y), tmin.z);
    
    // ���C���{�b�N�X��ʉ߂��A�Ō�Ɍ�������p�����[�^T���Z�o�i�����j
    float farthestLength = min(min(tmax.x, tmax.y), tmax.z);
    
    // �ŋߐړ_�ƍŉ��_�̂��ꂼ��̋�����Ԃ�
    return float2(max(0, nearestLength), max(0, farthestLength));
}

// ���C�}�[�`���O
uint RayMarching(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir, Texture3D<float4> tex)
{
    float2 hitinfo = ClucRayToBoxCrossPoint(boundsMin, boundsMax, rayStart, rayDir);
    
    // ���C�̊J�n�_
    float3 rayPos = rayStart;
    
    // ���������񐔂��i�[����
    uint hitCount = 0;
    
    for (uint step = 0; step < stepCount; step++)
    {
        //float disToSphere = length(rayPos - float3(0, 10, 0));
        //if (disToSphere <= 0.5f)
        //{
        hitCount++;
        //}
        
        // ���̃��C�̍��W���Z�o
        rayPos += rayDir * stepLength;
    }
    
    return hitCount;
}