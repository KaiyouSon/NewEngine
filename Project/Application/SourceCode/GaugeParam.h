#pragma once

struct GaugeParam
{
	float value;
	float max;
	float rate;

	void CalcRate(const float value_, const float max_);
	void Update();
};
