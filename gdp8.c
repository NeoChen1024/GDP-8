/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                      DCC GDP-8                      *
 * digital computer corpration, general data processor *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Neo_Chen: This will be the most useless Virtual Machine ever!
 * ENCODING: UTF-8
 * LANGUAGE: zh_TW, en_US
 */

/* 有沒有從命名看出我在向什麼公司的什麼電腦致敬？ */

/* 大致架構：
 * 多累加器，16 Bit 機器字，16 Bit 定址，加上 8 Bit 分段（共 24 Bit）
 * 中斷，指令權限區分，無虛擬記憶體
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgdp8.h>

/* 16 Bit, AC0 ~ AC3 and MQ */
word_t ac[4]={0,0,0,0}, mq;
uint8_t sc; /* Step Counter */
word_t st=0, field=0; /* Status and Field */
word_t sst=0, sfield=0; /* Saved Status and Field */
word_t *memory;
/* 16 Bit, Memory Addressing (and 8 bit field) */
word_t pc=0;

dev_desc_t dev_table[256]=
{
	{"CPU", cpu_handler, 0x00, 0x00},
	{"CON", console_handler, 0x01, 0x01},
	/*
	{"CLK", clock_handler, 0x02, 0x02},
	{"MUX", terminal_handler, 0x10, 0x1F},
	{"DSK", disk_handler, 0x20, 0x2F},
	{"TAP", tape_handler, 0x30, 0x3F},
	{"NET", network_handler, 0x40, 4F},
	{"MON", monitor_handler, 0x50, 0x5F}
	*/
};

/* Corefile Format:
 * "012345:ABCD"
 * 24 Bit : 16 Bit Hexdecimal
 * Any Invaild Input will be ignored
 */

void load_core(FILE *fp)
{
	extern uint16_t *memory;
	uint16_t word=0;
	uint32_t addr=0;
	uint8_t field=0;
	while(fscanf(fp, "%x:%hx\n", &addr, &word) != EOF)
	{
		MEM(addr) = word;
	}
}

void dump_core(FILE *fp)
{
	extern uint16_t *memory;
	uint32_t addr=0;
	for(addr=0; addr <= 0xFFFFFF; addr++)
	{
		fprintf(fp, "%06x:%04hx\n", addr, memory[addr]);
	}
}

/* Interrupt */
void interrupt(word_t orig_address, unsigned int code, uint8_t device_num)
{
	extern word_t *memory, pc;
	extern word_t field, st, sst, sfield;
	/* Save Original Content */
	sst=st; 
	sfield=field;
	/* Set interrupt reason */
	st=(st & 0xF800) | (code << 8);
	st=(st & 0x00FF) | device_num;
	field|=0xFF00;
	/* Same effect as JMS */
	MEM(0x000000)=orig_address;
	PC=0x1;
	return;
}

void interpret(word_t word)
{
	switch(INST_MASK(word))
	{
		case AND:
			inst_and(word);
			PC++;
			return;
		case ADD:
			inst_add(word);
			PC++;
			return;
		case ISZ:
			inst_isz(word);
			PC++;
			return;
		case DEP:
			inst_dep(word);
			PC++;
			return;
		case JMS:
			inst_jms(word);
			return;
		case JMP:
			inst_jmp(word);
			return;
		case IOT:
			inst_iot(word);
			PC++;
			return;
		case OPR:
			inst_opr(word);
			PC++;
			return;
		case PSH:
			inst_psh(word);
			PC++;
			return;
		case POP:
			inst_pop(word);
			PC++;
			return;
		case CAL:
			inst_cal(word);
			return;
		case RET:
			inst_ret(word);
			return;
		case EUM:
			inst_eum(word);
			return;
		case INT:
			inst_int(word);
			return;
		case SYS:
			inst_sys(word);
			return;
		case STP:
			inst_stp(word);
			return;
		default:
			return;
	}
}

int main (int argc, char **argv)
{
	/* Init */
	memory=calloc(POWTWO(24), 2);
	FILE *corefile;
	int opt;
	
	/* Reset */
	PC = 0x0000;
	FIELD = 0x0000;

	/* Parse Arguments */
	while((opt = getopt(argc, argv, "hf:s:")) != -1)
	{
		switch(opt)
		{
			case 's':
				sscanf(optarg, "%hx", &PC);
				break;
			case 'f':
				if((corefile = fopen(optarg, "r")) == NULL)
				{
					perror(optarg);
					exit(8);
				}
				load_core(corefile);
				break;
			case 'h':
				printf("Usage: %s [-h] [-f file] [-s address]\n", argv[0]);
				break;
			default:
				fprintf(stderr,"%s: %s\n", argv[0], optarg);
				exit(0);
		}
	}

	while(ST_STAT_MASK(word) != 0x7)
	{

	}
	return 0;
}

