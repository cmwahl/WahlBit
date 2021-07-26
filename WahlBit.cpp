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

		//unsigned char cmon = *buffer;
		//std::cout << "Buffer: " << (unsigned short)cmon << std::endl;

		unsigned int shift = bufferLength * 8 - numberOfBits;

		if (bytesLoc + (bitsLoc + numberOfBits) / 8 >= dataLength) {
			endOfBits = true;
			return nullptr;
		}
		
		//std::cout << "Start Buffer: " << (unsigned short)cmon << std::endl;
		leftShiftBufferBits((void*)buffer, bufferLength, bitsLoc);
		//cmon = *buffer;
		//std::cout << "Left Buffer: " << (unsigned short)cmon << std::endl;
		rightShiftBuffer((void*)buffer, bufferLength, shift);
		//cmon = *buffer;
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

		if (bytesLoc + (bitsLoc + numberOfBits) / 8 >= dataLength) {
			endOfBits = true;
			return;
		}

		bytesLoc += (numberOfBits + bitsLoc) / 8;
		bitsLoc = (numberOfBits + bitsLoc) % 8;
	}

	void* BitsParser::getBuffer() {
		return (void*)buffer;
	}

	unsigned int BitsParser::getBufferLength() {
		return bufferLength;
	}

	unsigned int BitsParser::getByteLoc() {
		return bytesLoc;
	}

	unsigned int BitsParser::getBitLoc() {
		return bitsLoc;
	}

	bool BitsParser::setByteLoc(unsigned int _bytesLoc) {
		if (_bytesLoc >= dataLength) {
			return false;
		}
		bytesLoc = _bytesLoc;
		return true;
	}

	bool BitsParser::setBitLoc(unsigned short _bitsLoc) {
		if (_bitsLoc > 8) {
			return false;
		}
		bitsLoc = _bitsLoc;
		return true;
	}

	bool BitsParser::findBitString(void* _bitString, unsigned int bitStringLength, unsigned short numBits) {
		char* bitString = (char*)_bitString;
		unsigned int startByte = getByteLoc();
		unsigned int startBit = getBitLoc();

		if (numBits / 8 > bitStringLength || numBits / 8 > bufferLength) {
			return false;
		}
		// Get initial bits
		getBits(numBits);

		// Loop
		bool match = false;
		while (!match) {
			// Compare
			match = bitsCompare((void*)bitString, bitStringLength, (void*)buffer, bufferLength, numBits);

			if (match) {
				break;
			}
			// Get next bit
			startBit += 1;
			startByte += startBit / 8;
			startBit = startBit % 8;

			if (startByte == dataLength) {
				endOfBits = true;
				return false;
			}

			leftShiftBufferBits((void*)buffer, bufferLength, 1);

			// Clear unwanted bit
			buffer[bufferLength - numBits / 8 - 1] = buffer[bufferLength - (numBits / 8) - 1] << (8 - numBits);
			buffer[bufferLength - numBits / 8 - 1] = buffer[bufferLength - (numBits / 8) - 1] >> (8 - numBits);

			unsigned char addTo = (data[startByte + (startBit + numBits - 1) / 8] << (startBit + numBits - 1)%8); //This is wrong, don't need startBit, need the numBits away from startBit. Start a leastSig bit/byte
			addTo = addTo >> 7;
			buffer[bufferLength - 1] += addTo;
		}

		setBitLoc(startBit);
		setByteLoc(startByte);
		return match;
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
		
		for(int i = 0; i < bufferLength - byteShift; ++i) {
			ptr[bufferLength - i - 1] = ptr[bufferLength - i - 1 - byteShift];
		}

		memset(ptr, 0, byteShift);
	}

	void rightShiftBufferBits(void* buffer, unsigned int const bufferLength, unsigned int const shift) {
		unsigned char* ptr = (unsigned char*)buffer;
		unsigned int bitShift = shift > 8 ? 8 : shift;

		unsigned int byte = 0;
		unsigned char prevByteBits = 0;
		for(unsigned int i = 1; i < bufferLength; ++i) {
			byte = bufferLength - i;
			prevByteBits = 0;

			ptr[byte] = ptr[byte] >> bitShift;
			
			prevByteBits = ptr[byte - 1] << (8 - bitShift);
			ptr[byte] += prevByteBits;
		}
		ptr[0] = ptr[0] >> bitShift;
	}

	void leftShiftBuffer(void* buffer, unsigned int const bufferLength, unsigned int const shift) {
		unsigned int byteShift = shift / 8;
		unsigned int bitShift = shift % 8;
		leftShiftBufferBytes(buffer, bufferLength, byteShift);
		leftShiftBufferBits(buffer, bufferLength, bitShift);
	}

	void leftShiftBufferBytes(void* buffer, unsigned int const bufferLength, unsigned int const shift) {
		unsigned char* ptr = (unsigned char*)buffer;
		unsigned int byteShift = shift > bufferLength - 1 ? bufferLength - 1 : shift;

		for(int i = 0; i < bufferLength - byteShift; ++i) {
			ptr[i] = ptr[i + byteShift];
		}

		memset(ptr + bufferLength - byteShift, 0, byteShift);
	}

	void leftShiftBufferBits(void* buffer, unsigned int const bufferLength, unsigned int const shift) {
		unsigned char* ptr = (unsigned char*)buffer;
		unsigned int bitShift = shift > 8 ? 8 : shift;

		unsigned int byte = 0;
		unsigned char nextByteBits = 0;
		for(unsigned int byte = 0; byte < bufferLength - 1; ++byte) {
			nextByteBits = 0;

			ptr[byte] = ptr[byte] << bitShift;

			nextByteBits = ptr[byte + 1] >> (8 - bitShift);
			ptr[byte] += nextByteBits;
		}
		ptr[bufferLength - 1] = ptr[bufferLength - 1] << bitShift;
	}

	bool bitsCompare(void* _buffer, unsigned int bufferLength, void* _data, unsigned int dataLength, unsigned int bits) {
		char* buffer = (char*)_buffer;
		char* data = (char*)_data;
		bool match = true;

		unsigned char read = data[dataLength - 1];

		if (bits / 8 > bufferLength || bits / 8 > dataLength) {
			return false;
		}
		for (int i = 0; i < bits / 8 + 1; ++i) {
			unsigned char testBuffer = buffer[bufferLength - i - 1];
			unsigned char testData = data[dataLength - i - 1];
			if (i == bits / 8) {
				testBuffer = testBuffer << 8 - bits;
				testBuffer = testBuffer >> 8 - bits;

				testData = testData << 8 - bits;
				testData = testData >> 8 - bits;
			}
			if (testBuffer != testData) {
				match = false;
				break;
			}
		}
		return match;
	}
}
