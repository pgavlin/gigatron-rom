// sysArg0-1 holds the arithmetic function under test
// sysArg2-3 holds the LHS of the operation
// sysArg4-5 holds the RHS of the operation
// sysArg6-7 holds the result

#define ADD 0
#define SUB 1
#define LSH 2
#define RSH 3
#define MUL 4

#ifdef TEST

#define sysFn ((unsigned*)(void*)0x0022)
#define sysArgsw ((unsigned*)(void*)0x0024)

#define OPT(a, b, op) \
	t = a; \
	u = b; \
	((int*)sysArgsw)[1] = a; \
	((int*)sysArgsw)[2] = b; \
	((int*)sysArgsw)[3] = t op u; \
   	__syscall(0xff); \
	((int*)sysArgsw)[3] = t op b; \
	__syscall(0xff); \

#define ADDT(a, b) OPT(a, b, +)
#define SUBT(a, b) OPT(a, b, -)
#define LSHT(a, b) OPT(a, b, <<)
#define RSHT(a, b) OPT(a, b, >>)
#define MULT(a, b) OPT(a, b, *)

void add() {
	int t, u;

	sysArgsw[0] = ADD;
	ADDT(0, 0);
	ADDT(0, 1);
	ADDT(0, -1);
	ADDT(255, 1);
	ADDT(~0, 1);
	ADDT(0x7fff, 1);
}

void sub() {
	int t, u;

	sysArgsw[0] = SUB;
	SUBT(0, 0);
	SUBT(0, 1);
	SUBT(0, ~0);
	SUBT(255, 1);
	SUBT(~0, 1);
	SUBT(0x7fff, 1);
}

void lsh() {
	int t, u;

	sysArgsw[0] = LSH;
	LSHT(1, 0);
	LSHT(1, 1);
	LSHT(1, 8);
	LSHT(~0, 16);
	LSHT((int)0xaaaa, 1);
	LSHT((int)0x5555, 1);
}

void rsh() {
	int t, u;

	sysArgsw[0] = RSH;
	RSHT(1, 0);
	RSHT(1, 1);
	RSHT(0x100, 8);
	RSHT(~0, 16);
	RSHT((int)0xaaaa, 1);
	RSHT((int)0x5555, 1);
}

void mul() {
	int t, u;

	sysArgsw[0] = MUL;
	MULT(1, 0);
	MULT(1, 1);
	MULT(8, 32);
	MULT(0xaa, 0x55);
	MULT(255, 255);
	MULT(256, 256);
	MULT(1, -1);
	MULT(-1, -1);
	MULT(2, -1);
}

void main() {
	*sysFn = 1;

	add();
	sub();
	lsh();
	rsh();
	mul();

	*sysFn = 0;
	__syscall(0xff);
}

#else

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "harness.h"

int fail = 0;

void sys1() {
	uint16_t* sysArgsw = (uint16_t*)&RAM[0x24];

	uint16_t op = sysArgsw[0];
	int16_t a = ((int16_t*)sysArgsw)[1];
	int16_t b = ((int16_t*)sysArgsw)[2];
	int16_t r = ((int16_t*)sysArgsw)[3];

	char* opStr;
	int16_t x;
	switch (op) {
	case ADD: opStr = "+", x = a + b; break;
	case SUB: opStr = "-", x = a - b; break;
	case LSH: opStr = "<<", x = b >= 16 ? a : a << b; break;
	case RSH: opStr = ">>", x = b >= 16 ? a : (int16_t)((uint16_t)a >> b); break;
	case MUL: opStr = "*", x = a * b; break;
	default:
		fprintf(stderr, "error: unknown operation 0x%04x\n", op);
		fail = 1;
		return;
	}

	if (r != x) {
		fprintf(stderr, "error: %hd %s %hd = %hd, expected %hd\n", a, opStr, b, r, x);
		fail = 1;
	}
}

int main(int argc, char* argv[]) {
	char* logFile = NULL;
	if (argc > 1) {
		logFile = argv[1];
	}
	runTest("gigatron.rom", "smath.gt1", logFile, sys1);
	exit(fail ? EXIT_FAILURE : 0);
}

#endif
