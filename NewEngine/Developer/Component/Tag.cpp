#include "Tag.h"
using namespace std;
unique_ptr<TagList> tagList(new TagList);

Tag::Tag()
{
	tag = "Default";
	//tag = tagList->GetTag(0);
}

void Tag::SetTag(std::string tag)
{
	this->tag = tag;
}

std::string Tag::GetTag()
{
	return tag;
}

TagList::TagList()
{
	list.push_back("Default");
}

string TagList::GetTag(const int& arrayNumber)
{
	return list[arrayNumber];
}

void TagList::AddTag(std::string tag)
{
	list.push_back(tag);
}

void TagList::DestroyTag(std::string tag)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i].c_str() == tag)
		{
			list.erase(list.begin() + i);
		}
	}
}

std::vector<std::string> TagList::GetList()
{
	return list;
}

