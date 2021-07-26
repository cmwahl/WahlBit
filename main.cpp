

#include "WahlBit.h"

using namespace std;

void testShift();
void testFind();
int main()
{

	testFind();

	return 0;
}

void testShift() {
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

	delete[] myBytes;
	cout << endl;
}

void testFind() {

	unsigned char* myBytes = new unsigned char[5];
	unsigned char* buffer = nullptr;

	myBytes[0] = 1;
	myBytes[1] = 1;
	myBytes[2] = 60;
	myBytes[3] = 128;
	myBytes[4] = 1;



	unsigned char find[2] = { 0 , 242 };

	WahlBit::BitsParser bits((void*)myBytes, 5, 2);
	bits.setBitLoc(1);
	bool found = bits.findBitString((void*)&find, 2, 9);

	cout << "Found: " << found << ", ByteLoc: " << bits.getByteLoc() << ", BitLoc: " << bits.getBitLoc() << endl;
	
}