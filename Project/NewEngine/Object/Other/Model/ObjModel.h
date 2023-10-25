#pragma once
#include "Model.h"

// objモデル
struct ObjModel : public Model
{
	ObjModel()
	{
		format = ModelFormat::Obj;
	}
};
