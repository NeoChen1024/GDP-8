/* =================================================== *\
||                 D C C   G D P - 8                   ||
|| digital computer corpration, general data processor ||
\* =================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>

#pragma once /* Only include once */

typedef uint8_t byte_t;
typedef uint8_t bit_t;
typedef uint64_t word_t;
typedef uint32_t addr_t;
typedef uint8_t reg_t;
typedef int32_t offset_t;
typedef uint8_t memory_t;

/* Definition */

enum InstructionList
{
	/* Special */
	NOP=	0x00,
	HLT=	0x01,
	BRK=	0x02,
	SLP=	0x03,
	WAIT=	0x04,
	IDLE=	0x05,
	INT=	0x06,
	EUM=	0x07,
	SYS=	0x08,
	IO=	0x09,
	IRET=	0x0A,
	/* Load / Store */
	LD=	0x10,
	ST=	0x11,
	LI=	0x12,
	LIW=	0x13,
	/* Arithmetic */
	ADD=	0x20,
	SUB=	0x21,
	INC=	0x22,
	DEC=	0x23,
	MUL=	0x24,
	DIV=	0x25,
	MOD=	0x26,
	ADDI=	0x27,
	SUBI=	0x28,
	INCI=	0x29,
	DECI=	0x2A,
	/* Bitwise operation */
	AND=	0x30,
	OR=	0x31,
	NOT=	0x32,
	XOR=	0x33,
	RTR=	0x34,
	RTL=	0x35,
	SHR=	0x36,
	SHL=	0x37,
	RTBR=	0x38,
	RTBL=	0x39,
	SHBR=	0x3A,
	SHBL=	0x3B,
	/* Data Moving */
	SWP=	0x40,
	MOV=	0x41,
	MOVC=	0x42,
	PUSH=	0x50,
	POP=	0x51,
	/* Flag operation */
	CMP=	0x60,
	TCH=	0x61,
	STB=	0x62,
	CLB=	0x63,
	CLR=	0x64,
	/* Jump */
	J=	0x70,
	JI=	0x71,
	JS=	0x72,
	JSI=	0x73,
	JR=	0x74,
	JSR=	0x75,
	RS=	0x76,
	/* Call */
	C=	0x80,
	CI=	0x81,
	CR=	0x82,
	R=	0x83,
};

#define OP_INSTS 55

struct arg_u
{
	reg_t reg;
};

struct arg_d
{
	reg_t dst;
	reg_t src;
};

struct arg_iq
{
	reg_t reg;
	uint16_t value;
};

struct arg_iw
{
	reg_t reg;
	word_t value;
};

struct arg_ls
{
	bit_t ind;
	reg_t index;
	bit_t indirect;
	bit_t size;
};

struct arg_jc
{
	bit_t indirect;
	addr_t address;
};

struct arg_rjc
{
	bit_t indirect;
	bit_t condition;
	offset_t offset;
};

struct arg_io
{
	byte_t dev;
	byte_t op;
	reg_t reg;
};

struct arg_sys
{
	byte_t num;
	reg_t reg1;
	reg_t reg2;
};

union arg_union
{
	struct arg_u	_u;
	struct arg_d	_d;
	struct arg_iq	_iq;
	struct arg_iw	_iw;
	struct arg_ls	_ls;
	struct arg_jc	_jc;
	struct arg_rjc	_rjc;
	struct arg_io	_io;
};

struct Instruction
{
	byte_t op;
	union arg_union arg;
};

struct Register
{
	word_t r[256];
	addr_t pc;
	addr_t sp;
	addr_t sra;
	addr_t ipc;
	byte_t iv;
	byte_t status;
};

struct InternalRegister
{
	struct Instruction inst;
	bit_t usermode;
	bit_t interrupt;
	word_t display;
};

struct CPU
{
	struct Register reg;
	struct InternalRegister ireg;
	memory_t *mem;
};

struct Handler
{
	bit_t defined;
	bit_t size;
	void(*exec)(struct CPU *cpu, struct Instruction *inst);
	void(*decode)(memory_t *memory, struct Instruction *inst);
};

/* Global Functions */
void panic(const char *msg);
int fetch(memory_t *memory, struct Instruction *inst);
void cpu_init(struct CPU *cpu);
void cpu_mainloop(struct CPU *cpu, addr_t address);
