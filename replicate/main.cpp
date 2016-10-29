#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "libct.h"

#pragma comment(lib, "libct.lib")

#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#pragma comment(linker, "/nodefaultlib:MSVCRTD.lib")

int main(int argc, char *argv[])
{
	_asm nop;

	FILE *fin, *fout;
	size_t n, m;
	unsigned char buf[8192];

	if ((fin = fopen("C:\\Users\\giammarco_2\\Documents\\Visual Studio 2013\\Projects\\replicator\\Debug\\replicate.exe", "rb")) == NULL)
		exit(-1);
	if ((fout = fopen("C:\\Users\\giammarco_2\\Documents\\Visual Studio 2013\\Projects\\replicator\\Debug\\replicated.exe", "wb")) == NULL)
		exit(-1);

	do{
		n = fread(buf, 1, sizeof buf, fin);
		if (n)
			m = fwrite(buf, 1, n, fout);
		else
			m = 0;
	} while ((n > 0) && (n = m));

	fclose(fin);
	fclose(fout);

	return 0;
}

extern "C" void __cdecl mainCRTStartup()
{
	int argc = _init_args();
	_init_atexit();
	_initterm(__xc_a, __xc_z);         // Call C++ constructors

	main(argc, _argv);    // Don't handle environment strings

	_doexit();
	ExitProcess(GetTickCount());
}


// I use current (compile time) as a seed

const char time[] = __TIME__; // __TIME__ has the following format: hh:mm:ss in 24-hour time

// Convert time string (hh:mm:ss) into a number
const int DigitToInt(char c) { return c - '0'; }
const int seed = DigitToInt(time[7]) +
DigitToInt(time[6]) * 10 +
DigitToInt(time[4]) * 60 +
DigitToInt(time[3]) * 600 +
DigitToInt(time[1]) * 3600 +
DigitToInt(time[0]) * 36000;


// 1988, Stephen Park and Keith Miller
// "Random Number Generators: Good Ones Are Hard To Find", considered as "minimal standard"
// Park-Miller 31 bit pseudo-random number generator, implemented with G. Carta's optimisation:
// with 32-bit math and without division

template<int N>
struct MetaRandomGenerator
{
private:
	static const unsigned a = 16807;        // 7^5
	static const unsigned m = 2147483647;   // 2^31 - 1

	static const unsigned s = MetaRandomGenerator<N - 1>::value;
	static const unsigned lo = a * (s & 0xFFFF);                // Multiply lower 16 bits by 16807
	static const unsigned hi = a * (s >> 16);                   // Multiply higher 16 bits by 16807
	static const unsigned lo2 = lo + ((hi & 0x7FFF) << 16);     // Combine lower 15 bits of hi with lo's upper bits
	static const unsigned hi2 = hi >> 15;                       // Discard lower 15 bits of hi
	static const unsigned lo3 = lo2 + hi;

public:
	static const unsigned max = m;
	static const unsigned value = lo3 > m ? lo3 - m : lo3;
};

template<>
struct MetaRandomGenerator<0>
{
	static const unsigned value = 1;// seed;
};

// Note: A bias is introduced by the modulo operation.
// However, I do belive it is neglictable in this case (M is far lower than 2^31 - 1)

template<int N, int M>
struct MetaRandom
{
	static const int value = MetaRandomGenerator<N + 1>::value % M;
};