#include "IFieldObject.h"

FieldObjectType IFieldObject::GetType()
{
	return mType;
}

void IFieldObject::SetType(const FieldObjectType type)
{
	mType = type;
}
