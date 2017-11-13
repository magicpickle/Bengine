#include "TextureCache.h"
#include "ImageLoader.h"

#include <iostream>

namespace Bengine
{
	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(std::string texturePath)
	{
		//Look up to see is the path is a correct path
		auto mit = _textureMap.find(texturePath);

		//check if its not in the map
		if (mit == _textureMap.end())
		{
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);
			//Insert it into the map
			_textureMap.insert(make_pair(texturePath, newTexture));
			std::cout << "Loaded Texture!\n";
			return newTexture;
		}
		std::cout << "Loaded Cached Texture!\n";
		return mit->second;
	}
}