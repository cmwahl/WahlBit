// WahlBit.cpp : Defines the entry point for the application.
//

#include "WahlBit.h"

namespace WahlBit {
	
	BitsParser::BitsParser(void* _data, unsigned int _dataLength, unsigned int _bufferLength) {
		data = (char*)_data;
		dataLength = _dataLength;
		bufferLength = _bufferLength;
		buffer = (char*)malloc(bufferLength);

	}

	BitsParser::~BitsParser() {
		free(buffer);
	}

	void* BitsParser::getBits(int numberOfBits) {
		copyToBuffer();

		unsigned short totalShift = 
	}

	void BitsParser::copyToBuffer() {
		memcpy(buffer, data + bytesLoc, bufferLength);
	}

}
