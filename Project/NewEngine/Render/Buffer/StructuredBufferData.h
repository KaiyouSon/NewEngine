#pragma once
#include "Util.h"

namespace StructuredBufferData
{
	struct STimer
	{
		uint32_t timer;
		uint32_t maxTimer;
		float timeRate;
	};

	struct SLeadEffect
	{
		uint32_t timer;
		uint32_t maxTimer;
		float timeRate;
		uint32_t startIndex;
		uint32_t endIndex;
	};

	struct SAirEffect
	{
		uint32_t timer;
		uint32_t maxTimer;
		float timeRate;
		uint32_t index;
	};

	struct SBossAttackTrajectoryEffect
	{
		uint32_t timer;
		uint32_t maxTimer;
		float timeRate;
		uint32_t index;
	};

	struct SFog
	{
		Color color;
	};
}