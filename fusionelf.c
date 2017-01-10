#include "fusionelf.h"

// copie fichier 1
// si section = PROGBITS (dans fichier 1)
// 	look fichier 2
// 	si section != PROGBITS	-> avancer un peu
// 	sinon
// 		if(cmp(section(fichier1, section(fichier2))==1)
// 			concat(both);
// 		else
// 			printf(section(fichier1));
// 			printf(section(fichier2));

ElfFile fusionProgBit (ElfFile elf1, ElfFile elf2)
{
	ElfFile elf3;
	
	elf3.tableSections = malloc(elf1.tableSections[0].headerSec.sh_size*(elf1.headerElf.e_shnum+elf2.headerElf.e_shnum));
	elf3 = Comparaison(elf1, elf2);

	return elf3;
}

ElfFile Comparaison(ElfFile elf1, ElfFile elf2)
{
	int i;
	int k=0;
	int j=0;
	int testpassage=0;
	ElfFile elf3;
	for(i=0;i<elf2.headerElf.e_shnum;i++)
	{
		testpassage=0;
		if(elf1.tableSections[i].headerSec.sh_type==SHT_PROGBITS)
		{
			j=0;
			//for(j=0;j<elf1.headerElf.e_shnum;j++)
			while(j<elf1.headerElf.e_shnum && testpassage==0)
			{
				
				if(strcmp(elf1.tableSections[i].nomSec,elf2.tableSections[j].nomSec) == 0)
				{
					elf3.tableSections[k].nomSec = elf1.tableSections[i].nomSec;
					elf3.tableSections[k].headerSec = elf1.tableSections[i].headerSec;
					elf3.tableSections[k].contenuSec = malloc(elf3.tableSections[k].headerSec.sh_size);

					//elf3.tableSections[k].headerSec.sh_size = (elf1.tableSections[i].headerSec.sh_size+elf2.tableSections[j].headerSec.sh_size);
					elf3.tableSections[k].contenuSec = (unsigned char *)strcat((char *)elf1.tableSections[i].contenuSec,(char *)elf2.tableSections[j].contenuSec);
					k++;
					testpassage=1;
				}
				j++;
			}
			if(testpassage==0)
			{
					elf3.tableSections[k].nomSec = elf1.tableSections[i].nomSec;
					elf3.tableSections[k].headerSec = elf1.tableSections[i].headerSec;
					elf3.tableSections[k].contenuSec=elf1.tableSections[i].contenuSec;
					k++;
			}
		}
	}
	for(i=0;i<elf2.headerElf.e_shnum;i++)
	{
		testpassage=0;
		if(elf2.tableSections[i].headerSec.sh_type==SHT_PROGBITS)
		{
			j=0;
			//for(int j=0;j<elf2.headerElf.e_shnum;j++)
			while (j<elf1.headerElf.e_shnum && testpassage==0)
			{
				if(strcmp(elf2.tableSections[i].nomSec,elf1.tableSections[j].nomSec) == 0)
				{
					testpassage=1;
				}
				j++;
			}
			if(testpassage==0)
			{
				elf3.tableSections[k].nomSec = elf2.tableSections[i].nomSec;
				elf3.tableSections[k].headerSec = elf2.tableSections[i].headerSec;
				elf3.tableSections[k].contenuSec = elf2.tableSections[i].contenuSec;
				k++;
			}
		}
	}
	return elf3;
}
