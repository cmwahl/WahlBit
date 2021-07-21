// WahlBit.cpp : Defines the entry point for the application.
//

#include "WahlBit.h"

namespace WahlBit {
	
	// Class
	BitsParser::BitsParser(void* _data, unsigned int _dataLength, unsigned int _bufferLength) {
		data = (unsigned char*)_data;
		dataLength = _dataLength;
		bufferLength = _bufferLength;
		buffer = (unsigned char*)malloc(bufferLength);

	}

	BitsParser::~BitsParser() {
		free(buffer);
	}

	void* BitsParser::getBits(unsigned int numberOfBits) {
		copyToBuffer();

		unsigned char cmon = *buffer;
		//std::cout << "Buffer: " << (unsigned short)cmon << std::endl;

		unsigned int shift = bufferLength * 8 - numberOfBits;
		
		//std::cout << "Start Buffer: " << (unsigned short)cmon << std::endl;
		leftShiftBufferBits((void*)buffer, bufferLength, bitsLoc);
		cmon = *buffer;
		//std::cout << "Left Buffer: " << (unsigned short)cmon << std::endl;
		rightShiftBuffer((void*)buffer, bufferLength, shift);
		cmon = *buffer;
		//std::cout << "Final Buffer: " << (unsigned short)cmon << std::endl << std::endl;

		updateLocs(numberOfBits);

		return (void*)buffer;
	}

	void BitsParser::copyToBuffer() {
		memset(buffer, 0, bufferLength);

		unsigned int bytesLeft = dataLength - bytesLoc;
		unsigned int bytesToWrite = bytesLeft >= bufferLength ? bufferLength : bytesLeft;

		memcpy(buffer, data + bytesLoc, bytesToWrite);
	}

	void BitsParser::updateLocs(unsigned int numberOfBits) {
		bytesLoc += (numberOfBits + bitsLoc) / 8;
		bitsLoc = (numberOfBits + bitsLoc) % 8;
	}

	// UTILITIES
	void rightShiftBuffer(void* buffer, unsigned int const bufferLength, unsigned int const shift) {
		unsigned int byteShift = shift / 8;
		unsigned int bitShift = shift % 8;
		//std::cout << "byteShift: " << byteShift << ", bitShift: " << bitShift << std::endl;
		rightShiftBufferBytes(buffer, bufferLength, byteShift);
		rightShiftBufferBits(buffer, bufferLength, bitShift);
	}

	void rightShiftBufferBytes(void* buffer, unsigned int const bufferLength, unsigned int const shift) {
		unsigned char* ptr = (unsigned char*)buffer;
		unsigned int byteShift = shift > bufferLength - 1 ? bufferLength - 1 : shift;
		
		for (int i = 0; i < bufferLength - byteShift; ++i) {
			ptr[bufferLength - i - 1] = ptr[bufferLength - i - 1 - byteShift];
		}

		memset(ptr, 0, byteShift);
	}

	void rightShiftBufferBits(void* buffer, unsigned int const bufferLength, unsigned int const shift) {
		unsigned char* ptr = (unsigned char*)buffer;
		unsigned int bitShift = shift > 8 ? 8 : shift;

		unsigned int byte = 0;
		unsigned char prevByteBits = 0;
		for (unsigned int i = 1; i < bufferLength; ++i) {
			//std::cout << "rightBitShift loop" << std::endl;
			byte = bufferLength - i;
			prevByteBits = 0;

			ptr[byte] = ptr[byte] >> bitShift;
			
			prevByteBits = ptr[byte - 1] << (8 - bitShift);
			ptr[byte] += prevByteBits;
		}
		//std::cout << "rightShift bitShift: " << bitShift << ", " << (unsigned short)ptr[0] << std::endl;
		ptr[0] = ptr[0] >> bitShift;
		//std::cout << "rightShift bitShift: " << bitShift << ", " << (unsigned short)ptr[0] << std::endl;
	}

	void leftShiftBufferBytes(void* buffer, unsigned int const bufferLength, unsigned int const shift) {
		unsigned char* ptr = (unsigned char*)buffer;
		unsigned int byteShift = shift > bufferLength - 1 ? bufferLength - 1 : shift;

		for (int i = 0; i < bufferLength - byteShift; ++i) {
			ptr[i] = ptr[i + byteShift];
		}

		memset(ptr + bufferLength - byteShift, 0, byteShift);
	}

	void leftShiftBufferBits(void* buffer, unsigned int const bufferLength, unsigned int const shift) {
		unsigned char* ptr = (unsigned char*)buffer;
		unsigned int bitShift = shift > 8 ? 8 : shift;

		unsigned int byte = 0;
		unsigned char nextByteBits = 0;
		for (unsigned int byte = 0; byte < bufferLength - 1; ++byte) {
			//std::cout << "leftBitShift Loop" << std::endl;
			nextByteBits = 0;

			ptr[byte] = ptr[byte] << bitShift;

			nextByteBits = ptr[byte + 1] >> (8 - bitShift);
			ptr[byte] += nextByteBits;
		}
		//std::cout << "leftShift bitShift: " << bitShift << std::endl;
		ptr[bufferLength - 1] = ptr[bufferLength - 1] << bitShift;
	}
}
