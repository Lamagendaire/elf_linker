#ifndef SYM_FUS_C
#define SYM_FUS_C

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "util.h"
#include "elf.h"
#include "readelf.h"
#include "display.h"


void Fusion_symboles(ElfFile elf1, ElfFile elf2, ElfFile elf3)
{
	int ind_sym = 0;
	
	for(int i = 0;i<elf1.tableSymb[i].headerSymb.st_size;i++)
	{
		if(ELF32_ST_BIND(elf1.tableSymb[i].headerSymb.st_info) == "LOCAL")
		{
			elf3.tableSymb[ind_sym].nomSymb = elf1.tableSymb[i].nomSymb
			elf3.tableSymb[ind_sym].headerSymb = 
		}
	
	}
}









#endif
