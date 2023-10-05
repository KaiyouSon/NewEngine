#include "GaugeParam.h"
#include "Util.h"

void GaugeParam::CalcRate(const float value_, const float max_)
{
	this->max = max_;
	this->value = value_;
	this->rate = Max<float>(value_ / max_, 0.f);
}

void GaugeParam::Update()
{
	value = Clamp<float>(value, 0.f, max);
	rate = Clamp<float>(value / max);
}
