// WahlBit.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

namespace WahlBit {

	class BitsParser {
	public:
		BitsParser(void* _data, unsigned int _dataLength, unsigned int _bufferLength);
		~BitsParser();

		// Get bits methods
		void* getBits(unsigned int numberOfBits);
		void* getBuffer();
		unsigned int getBufferLength();

		// Parser Location Util
		unsigned int getByteLoc();
		unsigned int getBitLoc();
		void setByteLoc(unsigned int _bytesLoc);
		void setBitLoc(unsigned short _bitsLoc);

		// Searches for the bitString from its given position and sets parser position to the bitString
		// Returns true if found, false if reaches endOfBits
		bool findBitString(unsigned long long int bitString, unsigned short numBits);

		

	private:

		unsigned char* data = nullptr;
		unsigned int dataLength = 0;
		unsigned char* buffer = nullptr; // 8 Byte buffer
		unsigned int bufferLength = 0;

		unsigned int bytesLoc = 0;
		unsigned short bitsLoc = 0;

		unsigned long long int getBitsFromByte(void* bytes, unsigned short numOfBits);
		void copyToBuffer();
		void updateLocs(unsigned int numberOfBits);
	};

	void rightShiftBuffer(void* buffer, unsigned int const bufferLength, unsigned int const shift);
	void rightShiftBufferBytes(void* buffer, unsigned int const bufferLength, unsigned int const shift);
	void rightShiftBufferBits(void* buffer, unsigned int const bufferLength, unsigned int const shift);
	void leftShiftBufferBytes(void* buffer, unsigned int const bufferLength, unsigned int const shift);
	void leftShiftBufferBits(void* buffer, unsigned int const bufferLength, unsigned int const shift);

}




