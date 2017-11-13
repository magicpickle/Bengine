#include "IOManager.h"

#include <fstream>

namespace Bengine
{
	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
	{
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail())
		{
			perror(filePath.c_str());
			return false;
		}

		//seek to the end
		file.seekg(0, std::ios::end);
		//Get the file size
		int fileSize = file.tellg();
		//Go to the start
		file.seekg(0, std::ios::beg);
		//Reduce the filesize by the header bytes
		fileSize -= file.tellg();
		//the buffer needs to be the size of the file
		buffer.resize(fileSize);
		//A vector of char has a char array in it, so just tell it the memory address
		file.read((char*)&(buffer[0]), fileSize);
		//Close the file
		file.close();
		return true;
	}
}