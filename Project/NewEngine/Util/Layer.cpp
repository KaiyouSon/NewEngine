#include "Layer.h"

Layer::Layer() :
	tag("Unknown")
{
}

Layer::Layer(const std::string& tag, const int32_t depth, const LayerType type) :
	tag(tag), depth(depth), type(type)
{
}

bool Layer::operator<(const Layer& other) const
{
	return depth < other.depth;
}

bool Layer::operator==(const Layer& other) const
{
	return tag == other.tag && depth == other.depth && type == other.type;
}
