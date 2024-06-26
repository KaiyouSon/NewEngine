struct ParticleData
{
    uint isActive;
    float3 pos;
    float3 moveVec;
    float3 moveAccel;
    float2 scale;
    float shininess;
    float4 color;
};

struct TimerData
{
    uint timer;
    uint maxTimer;
    float timerRate;
};

static const uint MaxNum = 64;
RWStructuredBuffer<ParticleData> inputData : register(u0);
RWStructuredBuffer<TimerData> timerData : register(u1);

ParticleData InitParticleData(const uint index)
{
    ParticleData result;
    
    result.isActive = true;
    result.pos = 0;
    result.moveVec = float3(1, 1, 1);
    result.moveAccel = float3(0.25f, 0.25f, 0.25f);
    result.scale = float2(1, 1);
    result.shininess = 2.f;
    result.color = float4(1, 1, 1, 1);
    
    return result;
}

static uint init = false;

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    //if (init == false)
    //{
    //    for (uint i = 0; i < MaxNum; i++)
    //    {
    //        inputData[i].isActive = false;
    //        //inputData[i].pos = 0;
    //        inputData[i].moveVec = float3(1, 1, 1);
    //        inputData[i].moveAccel = float3(0.25f, 0.25f, 0.25f);
    //        inputData[i].scale = float2(1, 1);
    //        inputData[i].shininess = 2.f;
    //        inputData[i].color = float4(1, 1, 1, 1);
    //    }
    //    init = true;
    //}


    
    //if (timer >= maxTimer)
    //{
    //    for (uint i = 0; i < MaxNum; i++)
    //    {
    //        inputData[i] = InitParticleData(index);
    //        index++; // 次のパーティクルの初期化
    //        if (index >= MaxNum) // 最大数に達したらリセット
    //        {
    //            index = 0;
    //        }
    //    }
    //    timer = 0; // timer をリセット
    //}
    
    // 生成処理
    timerData[0].timer++;
    timerData[0].maxTimer = 30;
    if (timerData[0].timer >= timerData[0].maxTimer)
    {
        for (uint i = 0; i < MaxNum; i++)
        {
            if (inputData[i].isActive == true)
            {
                continue;
            }
            inputData[i] = InitParticleData(0);
        }
        timerData[0].timer = 0; // timer をリセット
    }
    
    // 出力データを設定
    for (uint i = 0; i < MaxNum; i++)
    {
        ParticleData result = inputData[i];
        
        if (result.isActive == false)
        {
            continue;
        }
        
        // 更新処理
        result.pos += normalize(result.moveVec) * result.moveAccel;
        result.scale -= 0.1f;
            
        if (result.scale.x <= 0)
        {
            result.scale = 0;
            result.isActive = false;
        }
        
        // 出力データを書き込む
        inputData[i] = result;
    }
}
