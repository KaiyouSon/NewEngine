struct ParticleData
{
    float3 pos;
    float3 moveVec;
    float3 moveAccel;
    float2 scale;
    float scaleAccel;
    float shininess;
    float4 color;
};
RWStructuredBuffer<ParticleData> outputData : register(u0);

struct LeadEffect
{
    uint timer;
    uint maxTimer;
    float timerRate;
    uint startIndex;
    uint endIndex;
};
RWStructuredBuffer<LeadEffect> leadEffect : register(u1);

// �p�[�e�B�N���̍ő吔
cbuffer ConstantBufferMaxParticleData : register(b0)
{
    uint max;
}

// �p�[�e�B�N���̃x�[�X�x�N�g��
cbuffer ConstantBufferVec : register(b1)
{
    float3 vec;
}

// �^������
float Random01(float2 seed);

// �^�������V�[�h
float2 RandomSeed(float2 seed, uint2 index);

// �C�ӎ���]
float3 AnyAxisRotation(float3 vec, float3 axis, float angle);

// ������
ParticleData InitParticleData(uint index);

[numthreads(1000, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x == 0)
    {
        leadEffect[0].timer++;
        if (leadEffect[0].timer >= leadEffect[0].maxTimer)
        {
            // ������
            for (uint i = 0; i < 3; i++)
            {
                outputData[leadEffect[0].endIndex] = InitParticleData(leadEffect[0].endIndex);
                leadEffect[0].endIndex++;
            }
            
            leadEffect[0].timer = 0;
            if (leadEffect[0].endIndex >= max)
            {
                leadEffect[0].endIndex = 0;
            }
        }
    }
    
    
    // area == 10w�ɂȂ��Ă��邩��A[numthreads(1000, 1, 1)]����1CS��10��for����
    
    // 100�̃f�[�^���������邽�߂̃C���f�b�N�X���v�Z
    uint dataPerThread = max / 1000;
    uint startIndex = (DTid.x - 1) * dataPerThread;
    uint endIndex = DTid.x * dataPerThread;

    for (uint i = startIndex; i < endIndex; i++)
    {
        if (i >= leadEffect[0].endIndex)
        {
            return;
        }
        
        ParticleData result = outputData[i];
        
        // �X�P�[����0�̏ꍇ
        if (result.scale.x <= 0)
        {
            result.scale = 0;
            continue;
        }
        
        // ���W
        result.pos += normalize(result.moveVec) * result.moveAccel;
        
        result.scale -= 0.0001f;
        
        // �o�̓f�[�^����������
        outputData[i] = result;
    }
}

// �^������
float Random01(float2 seed)
{
    float result = frac(sin(dot(seed, float2(12.9898f, 78.223f))) * 43758.5453f);
    return result;
}

// �^�������V�[�h
float2 RandomSeed(float2 seed, uint2 index)
{
    float random = Random01(seed);
    float2 result = float2(random / index.x, random * index.y);
    return result;
}

// �C�ӎ���]
float3 AnyAxisRotation(float3 vec, float3 axis, float angle)
{
    float radian = angle * (3.1415926f / 180.f);
    float hlafRadian = radian / 2;
    float4 q = float4(normalize(axis).xyz * sin(hlafRadian), cos(hlafRadian));
    
    float3 qVec = q.xyz;
    float3 t = 2.0f * cross(qVec, vec);
    
    float3 result = vec + q.w * t + cross(qVec, t);
    return result;
}

// ������
ParticleData InitParticleData(uint index)
{
    ParticleData result;
    
    // �C���f�b�N�X(x,y)
    float2 seed = uint2(index % 256, index / 256) + uint2(-1, 1);

    // ���W
    result.pos = 0;

    // �E�x�N�g�������߂�
    float3 rightVec = cross(vec, float3(0, 1, 0));
        
    // �x�[�X�x�N�g���̉E�x�N�g����ɏ㉺�ɉ�]
    seed = RandomSeed(seed, index);
    float angle1 = Random01(seed) * 30 - 15;
    float3 roted1 = AnyAxisRotation(vec, rightVec, angle1);

    // �x�[�X�x�N�g������ɂ�������]
    seed = RandomSeed(seed, index);
    float angle2 = Random01(seed) * 360;
    result.moveVec = AnyAxisRotation(roted1.xyz, vec, angle2).xyz;
        
    // �ړ����x
    const float rate = 0.005f;
    const float baseSpeed = 0.005f;
    seed = RandomSeed(seed, index);
    result.moveAccel.x = baseSpeed + Random01(seed) * rate;
    seed = RandomSeed(seed, index);
    result.moveAccel.y = baseSpeed + Random01(seed) * rate;
    seed = RandomSeed(seed, index);
    result.moveAccel.z = baseSpeed + Random01(seed) * rate;
        
    // �X�P�[��
    seed = RandomSeed(seed, index);
    result.scale = 0.15f + Random01(seed) * 0.25f;

    // �X�P�[���̕ω���
    const float baseScaleAccel = result.scale > 0.25f ? 0.0005f : 0.0001f;
    seed = RandomSeed(seed, index);
    result.scaleAccel = baseScaleAccel + Random01(seed) * 0.0005f;
    
    // �P�x
    seed = RandomSeed(seed, index);
    result.shininess = 0.75f + Random01(seed) * 0.75f;
        
    // �F
    result.color = float4(0.76f, 0.75f, 0.47f, 1.f);
    
    return result;
}