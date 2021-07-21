

#include "WahlBit.h"

using namespace std;

int main()
{

	unsigned char* myBytes = new unsigned char[5];
	unsigned char* buffer = nullptr;

	for (int i = 0; i < 5; ++i) {
		myBytes[i] = pow(2, i);
	}
	
	unsigned int bufferLength = 3;
	WahlBit::BitsParser bits((void*)myBytes, 5, bufferLength);
	//WahlBit::leftShiftBufferBytes((void*)myBytes, 5, 2);
	//WahlBit::rightShiftBufferBits((void*)myBytes, 5, 4);

	for (int i = 0; i < 5; ++i) {
		void* ptr = bits.getBits(8);
		unsigned char val = *((unsigned char*)ptr + bufferLength - 1);
		//myBytes[i] = myBytes[i] >> 4;
		//unsigned char val = myBytes[i];
		cout << (unsigned short)val << " ";
	}
	cout << endl;
	//cout << test << endl;

	return 0;
}
