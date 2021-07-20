

#include "WahlBit.h"

using namespace std;

int main()
{
	
	unsigned short test = 1;
	void* testptr = (void*)&test;

	char* mychar = (char*)testptr;
	*mychar = 0;
	mychar++;
	*mychar = 1;
	mychar--;

	unsigned long long int bigNum = 5;
	unsigned char littleNum = 1;

	cout << test << endl;

	return 0;
}
