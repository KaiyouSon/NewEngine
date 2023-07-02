#pragma once

struct GaugeParam
{
	float value_;
	float max_;
	float rate_;

	void CalcRate(const float value, const float max);
};