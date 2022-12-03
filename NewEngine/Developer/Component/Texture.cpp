#include "Texture.h"
using namespace std;

void TextureManager::Add(unique_ptr<Texture>&& texture, const string& tag)
{
	list.push_back(move(texture));
	list.back()->SetTag(tag);
}

void TextureManager::Add(std::unique_ptr<Texture>&& texture, const uint32_t& index)
{
	list.push_back(move(texture));
	list.back()->SetIndex(index);
}

Texture* TextureManager::GetBackTexture()
{
	if (list.size() > 0)
	{
		return list.back().get();
	}
}

Texture* TextureManager::GetTexture(const string& tag)
{
	for (const auto& temp : list)
	{
		if (temp->GetTag() == tag)
		{
			return temp.get();
		}
	}
	return nullptr;
}

Texture* TextureManager::GetTexture(const uint32_t& index)
{
	for (const auto& temp : list)
	{
		if (temp->GetIndex() == index)
		{
			return temp.get();
		}
	}
	return nullptr;
}

void TextureManager::DestroyTexture(const string& tag)
{
	for (const auto& temp : list)
	{
		if (temp->GetTag() == tag)
		{
			list.remove(temp);
			break;
		}
	}
}

unique_ptr<TextureManager> gameTextureList(new TextureManager);
unique_ptr<TextureManager> materialTextureList(new TextureManager);