#include "VolumetricFog.hlsli"

Texture3D<float4> tex : register(t0);
SamplerState smp : register(s0);

// ���C����{�b�N�X�ɓ�����ŋߐړ_�ƍŉ��_
float2 ClucRayToBoxCrossPoint(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir);

// ���C�}�[�`���O
float4 RayMarching(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir);

// �ŏ��l��0�ɍő�l��1�ɂ��l��lerp����
float3 MapValueTo01(float3 boundsMin, float3 boundsMax, float3 value);

float4 main(V2P i) : SV_TARGET
{
    float3 rayStart = cameraPos;
    float3 rayDir = normalize(i.wpos - cameraPos);
        
    // �{�b�N�X�̍ő�ŏ��l
    float3 boundsMin = objectPos + objectScale * float3(-0.5f, -0.5f, -0.5f);
    float3 boundsMax = objectPos + objectScale * float3(+0.5f, +0.5f, +0.5f);

    // �J��������̋������v�Z
    float dis = distance(cameraPos, i.wpos);
    float disRate = smoothstep(fogClamp.x, fogClamp.y, dis);
    
    float4 resultColor = RayMarching(boundsMin, boundsMax, rayStart, rayDir) * disRate;
    
    resultColor = resultColor * fogColor * fogColorRate;
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
float4 RayMarching(float3 boundsMin, float3 boundsMax, float3 rayStart, float3 rayDir)
{
    float2 hitinfo = ClucRayToBoxCrossPoint(boundsMin, boundsMax, rayStart, rayDir);
    
    // ���������񐔂��i�[����
    uint hitCount = 0;
    
    float colorDensity = 0;
    
    // ���C���΂��ē������Ă�����
    if (hitinfo.x <= hitinfo.y)
    {
        // ���C�̊J�n�_(���������_����)
        float3 rayPos = rayStart + rayDir * hitinfo.x;
        
        // �X�e�b�v����i��
        //[unroll(1000)] 
        [loop]
        for (uint i = 0; i < stepCount; i++)
        {
            float3 uvw = MapValueTo01(boundsMin, boundsMax, rayPos);
            colorDensity += tex.Sample(smp, (uvw + offset)).r * stepLength * density;
        
            // ���̃��C�̍��W���Z�o
            rayPos += rayDir * stepLength;
        }
    }
    
    return colorDensity;
}

// �ŏ��l��0�ɍő�l��1�ɂ��l��lerp����
float3 MapValueTo01(float3 boundsMin, float3 boundsMax, float3 value)
{
    float3 result = (value - boundsMin) / (boundsMax - boundsMin);
    return result;
}