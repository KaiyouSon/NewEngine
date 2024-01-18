// 構造体

// マテリアル
struct Material
{
    float3 ambient;
    float3 diffuse;
    float3 specular;
};

// 平行光源
struct DirectionalLight
{
    float4 color;
    float3 vec;
    uint isActive;
};

// 点光源
struct PointLight
{
    float4 color;
    float3 pos;
    float radius;
    float3 colorRate;
    float decay;
    uint isActive;
};

// スポットライト
struct SpotLight
{
    float4 color;
    float3 vec;
    float radius;
    float3 pos;
    float decay;
    float3 colorRate;
    uint isActive;
    float2 cosAngle;
};

// 光源計算用データ
struct LightCalculateData
{
    float3 vertexPos;
    float3 vertexNormal;
    float3 lightVec;
    float3 lightNormal;
};

// 丸影
struct CircleShadow
{
    float3 vec; // 逆ベクトル
    float3 pos; // 座標
    float disCasterLight; // キャスターとライトの距離
    float3 atten; // 距離減衰係数
    float2 factorAngleCos; // 減衰角度のコサイン
    uint isActive;
};

// フォグ
struct Fog
{
    uint isActive;
    float nearDis;
    float farDis;
    float4 color;
};

// ランバート反射
float3 Lambert(float3 lightV, float3 normal)
{
    float3 lightDir = normalize(float3(lightV));
    float diffuse = saturate(dot(-lightDir, normal));
    float brightness = diffuse + 0.3f;
    float3 resultColor = brightness;
    return resultColor;
}

// フォーン反射
float3 Phone(float3 _lightV, float3 _normal, float3 _eyeDir, float _shininess, float3 _ambient, float3 _diffuse)
{
    // 頂点から視点へのベクトル
    float3 lightDir = normalize(_lightV);
    
    // ライトに向かうベクトルと法線の内積
    float3 dotLightNormal = dot(_lightV, _normal);
    
    // 反射光ベクトル
    float3 reflect = normalize(_lightV + 2 * dotLightNormal * _normal);
    
    // 環境反射光
    float3 ambient = _ambient;
    // 拡散反射光
    float3 diffuse = dotLightNormal * _diffuse;
    
    // 鏡面反射光
    float3 specular = pow(saturate(dot(reflect, _eyeDir)), _shininess);
    
    float3 resultColor = ambient + diffuse + specular;
    
    return resultColor;
}

// 平行光源の計算
// directionalLight - 平行光源の構造体
// material - マテリアルの構造体
// meshNormal - メッシュの法線
// eyeVec - 頂点から視点へのベクトル
// shininess - 光泽度
float3 CalcDirectionalLight(DirectionalLight directionalLight, Material material,
    float3 meshNormal, float3 eyeVec, float shininess)
{
    float3 result = (0, 0, 0);
    
    if (directionalLight.isActive == false)
    {
        return result;
    }
        
    float3 halfVector = normalize(-directionalLight.vec + eyeVec);
    float lightDotNormal = dot(directionalLight.vec, meshNormal);
    float nomralDotHalf = dot(normalize(meshNormal), halfVector);
    
    // アンビエント
    result += material.ambient.rgb + float3(0.2f, 0.2f, 0.2f);
    // ディフューズ
    result += lightDotNormal * material.diffuse.rgb;
    // スペキュラー
    result += pow(saturate(nomralDotHalf), shininess) * material.specular.rgb;
    
    result *= directionalLight.color.rgb;
    
    return result;
}

// 点光源の計算
float3 CalcPointLight(PointLight pointLight, Material material,
    float3 worldPos, float3 meshNormal, float3 eyeVec, float shininess)
{
    float3 result = (0, 0, 0);
    
    if (pointLight.isActive == false)
    {
        return result;
    }
    
    // ライトヘのベクトル
    float3 lightVec = normalize(pointLight.pos - worldPos);
    float d = distance(pointLight.pos, worldPos);
            
    float s = d / pointLight.radius;
    if (s >= 1.0)
    {
        return result;
    }
            
    float s2 = s * s;
            
    float atten = pointLight.decay * ((1 - s2) * (1 - s2));
            
    // ライトに向かうベクトルと法線の内積
    float3 dotLightNormal = dot(lightVec, meshNormal);
            
    float3 diffuse = saturate(dotLightNormal * material.diffuse.rgb);
    
    float3 reflectVec = normalize(-lightVec + 2 * dotLightNormal * meshNormal);
    float3 specular = pow(saturate(dot(reflectVec, eyeVec)), shininess) * material.specular.rgb;
    
    result += atten * (diffuse + specular) * pointLight.color.rgb * pointLight.colorRate.rgb;

    return result;
}

//float3 CalculatePointLight(PointLight _pointLight, Material _material,
//    LightCalculateData _lightCalculateData, float3 _eyeDir, float _shininess)
//{
//    float3 resultColor = 0;
    
//    if (_pointLight.isActive == (uint) true)
//    {
//		// ベクトルの長さ
//        float d = length(_lightCalculateData.lightVec);

//		// 距離減衰係数
//        float atten = 1.0f /
//				(_pointLight.atten.x +
//				_pointLight.atten.y * d +
//				_pointLight.atten.z * d * d);

//		// 反射光ベクトル
//        float3 reflect = normalize(
//        -_lightCalculateData.lightVec + 2 *
//        _lightCalculateData.lightNormal *
//        _lightCalculateData.vertexNormal);
        
//		// 拡散反射光
//        float3 diffuse = _lightCalculateData.lightNormal * _material.diffuse;
//		// 鏡面反射光
//        float3 specular = pow(saturate(dot(reflect, _eyeDir)), _shininess) * _material.specular;

//		// 全て加算する
//        resultColor += atten * (diffuse + specular) * _pointLight.color;
//    }
    
//    return resultColor;
//}

//// スポットライト
//float3 CalculateSpotLight(SpotLight _spotLight, Material _material,
//    LightCalculateData _lightCalculateData, float3 _eyeDir, float _shininess)
//{
//    float3 resultColor = 0;
    
//    if (_spotLight.isActive == (uint) true)
//    {
//		// ベクトルの長さ
//        float d = length(_lightCalculateData.lightVec);

//		// 距離減衰係数
//        float atten = saturate(1.0f /
//				(_spotLight.atten.x +
//					_spotLight.atten.y * d +
//					_spotLight.atten.z * d * d));

//		// 角度減衰
//        float cos = dot(_lightCalculateData.lightVec, _spotLight.vec);
//		// 減衰開始角度から、減衰終了角度にかけて減衰
//		// 減衰開始角度の内側は１倍、減衰終了角度の外側は０倍の輝度
//        float angleAtten = smoothstep(_spotLight.factorAngleCos.y, _spotLight.factorAngleCos.x, cos);
//		// 角度減衰を乗算
//        atten *= angleAtten;

//		// 反射光ベクトル
//        float3 reflect = normalize(
//            -_lightCalculateData.lightVec + 2 *
//            _lightCalculateData.lightNormal *
//            _lightCalculateData.vertexNormal);

//		// 拡散反射光
//        float3 diffuse = _lightCalculateData.lightNormal * _material.diffuse;
//		// 鏡面反射光
//        float3 specular = pow(saturate(dot(reflect, _eyeDir)), _shininess) * _material.specular;

//	    // 全て加算する
//        resultColor += atten * (diffuse + specular) * _spotLight.color;
//    }

//    return resultColor;
//}

// 丸影
float3 CalculateCircleShadow(CircleShadow _circleShadow, float3 _pos)
{
    float3 resultColor = 0;
    
    if (_circleShadow.isActive == (uint) true)
    {
			// オブジェクト表面からキャスターへのベクトル
        float3 casterV = _circleShadow.pos - _pos;

			// 投影方向での距離
        float d = dot(casterV, _circleShadow.vec);

			// 距離減衰係数
        float atten = saturate(1.0f /
				(_circleShadow.atten.x +
					_circleShadow.atten.y * d +
					_circleShadow.atten.z * d * d));
			// 距離がマイナスなら０にする
        atten *= step(0, d);

			// 仮想ライトの座標
        float3 lightPos = _circleShadow.pos + _circleShadow.vec * _circleShadow.disCasterLight;

			// オブジェクト表面からライトへのベクトル
        float3 lightV = normalize(lightPos - _pos);

			// 角度計算
        float cos = dot(lightV, _circleShadow.vec);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は１倍、減衰終了角度の外側は０倍の輝度
        float angleAtten = smoothstep(_circleShadow.factorAngleCos.y, _circleShadow.factorAngleCos.x, cos);
			// 角度減衰を乗算
        atten *= angleAtten;

		// 全て減算する
        resultColor -= atten;
    }
    
    return resultColor;
}

float4 CalculateFog(Fog fog, float dis, float4 currentColor)
{
    if (fog.isActive == (uint) true)
    {
		// カメラの座標と頂点の距離
        float rate = smoothstep(fog.nearDis, fog.farDis, dis);

		// フォグの色
        return fog.color * rate + currentColor * (1 - rate);
    }
    
    return currentColor;
}
