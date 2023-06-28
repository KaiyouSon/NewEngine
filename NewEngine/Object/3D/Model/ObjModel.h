#pragma once
#include "Model.h"

struct ObjModel : public Model
{
	ObjModel()
	{
		format = ModelFormat::Obj;
	}
};