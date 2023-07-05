#include "GaugeParam.h"
#include "Util.h"

void GaugeParam::CalcRate(const float value, const float max)
{
	this->max = max;
	this->value = value;
	this->rate = Max<float>(value / max, 0.f);
}

void GaugeParam::Update()
{
	value = Clamp<float>(value, 0.f, max);
	rate = Clamp<float>(value / max);
}
