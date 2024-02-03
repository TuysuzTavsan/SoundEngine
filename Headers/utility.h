#pragma once
#include <cstdint>

namespace Utility
{
	template<typename T>
	void ConvertBytesLE(char* buffer, T& result, const std::uint64_t& size)
	{
		for (std::uint64_t i = size - 1; i >= 0; i--)
		{
			result = (result << 8) | static_cast<unsigned char>(buffer[i]);
		}
	}

	template<typename T>
	void ConvertBytesBE(char* buffer, T& result, const std::uint64_t& size)
	{
		for (std::uint64_t i = 0; i < size; i++)
		{
			result = (result << 8) | static_cast<unsigned char>(buffer[i]);
		}
	}

}



