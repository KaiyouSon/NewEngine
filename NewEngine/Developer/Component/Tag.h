#pragma once
#include "IComponent.h"
#include <memory>
#include <string>
#include <vector>

class Tag : public IComponent
{
private:
	std::string tag;

public:
	Tag();
	void SetTag(std::string tag);
	std::string GetTag();
};

class TagList
{
private:
	std::vector<std::string> list;
public:
	TagList();
	std::string GetTag(const int& arrayNumber);

	void AddTag(std::string tag);
	void DestroyTag(std::string tag);

	std::vector<std::string> GetList();
};

extern std::unique_ptr<TagList> tagList;