#pragma once

#include "stormancer/BuildConfig.h"
#include "stormancer/Streams/bytestreambuf.h"
#include <istream>
#include <vector> 
#include <iostream>

namespace Stormancer
{
	class ibytestream : public std::basic_istream<byte>
	{
	public:

#pragma region public_methods

		ibytestream();

		ibytestream(byte* data, std::streamsize dataSize);

		ibytestream(const ibytestream& other) = delete;

		ibytestream(ibytestream&& other);

		ibytestream& operator>>(char& value);

		ibytestream& operator>>(int8& value);

		ibytestream& operator>>(uint8& value);

		ibytestream& operator>>(int16& value);

		ibytestream& operator>>(uint16& value);

		ibytestream& operator>>(int32& value);

		ibytestream& operator>>(uint32& value);

		ibytestream& operator>>(int64& value);

		ibytestream& operator>>(uint64& value);

		ibytestream& operator>>(float& value);

		ibytestream& operator>>(double& value);

		ibytestream& operator>>(bool& value);

		ibytestream& operator>>(void*& value);

		ibytestream& operator>>(std::vector<byte>& bytes);

		std::vector<byte> bytes();

		byte* startPtr();

		byte* currentPtr();

		byte* endPtr();

		std::streamsize totalSize();

		std::streamsize availableSize();

		std::streamsize currentPosition();

		ibytestream& read(byte* ptr, std::streamsize size);

		ibytestream& read(char* ptr, std::streamsize size);

#pragma endregion

	private:

#pragma region private_methods

		template<typename T>
		inline ibytestream& deserialize(T& value)
		{
			if (good())
			{
				read(reinterpret_cast<byte*>(&value), sizeof(T));
			}
			return (*this);
		}

#pragma endregion

#pragma region private_members

		bytestreambuf _buffer;

#pragma endregion
	};
}
