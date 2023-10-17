struct FogData
{
    float2 uv;
    float4 color;
};
RWStructuredBuffer<FogData> outputData : register(u0);

[numthreads(540, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID)
{
    // 1920*1080になっているから、[numthreads(1080, 1, 1)]で1CSに1920回for分回す
    
    // スレットごとのfor分回数
    uint countPerThread = 1920;
    
    // グループごとのデータ数(540*1920)
    uint countPerGroup = 540 * countPerThread;
    
    uint startIndex = (DTid.x - 1) * countPerThread + (GTid.x - 1) * countPerGroup;
    uint endIndex = DTid.x * countPerThread + (GTid.x - 1) * countPerGroup;
    
    for (uint i = startIndex; i < endIndex; i++)
    {
        if (i <= 1920)
        {
            outputData[i].color = float4(0, 1, 0, 1);
        }
        else
        {
            outputData[i].color = float4(1, 0, 0, 1);
        }

    }
    
}