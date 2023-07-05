#pragma once

struct GaugeParam
{
	float value;
	float max;
	float rate;

	void CalcRate(const float value, const float max);
	void Update();
};