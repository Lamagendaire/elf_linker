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

Section* copierProgBit(ElfFile elf1)
{
	Section* sect=NULL;
	int k=0;
	for(int i=0;i<elf1.headerElf.e_shnum;i++)
	{
		if(elf1.tableSections[i].headerSec.sh_type==SHT_PROGBITS)
		{
			sect = malloc(sizeof(elf1.tableSections[i].headerSec.sh_size));
			sect[k] = elf1.tableSections[i];
			k++;
		}
	}
	return sect;
}


ElfFile fusionProgBit(ElfFile elf1, ElfFile elf2)
{
	int i;
	
	int j=0;
	int testpassage=0;
	
	ElfFile elf3;
	elf3.tableSections=copierProgBit(elf1);
	elf3.tableSections = malloc(sizeof(Section)*(elf.headerElf.e_shnum+elf2.headerElf.e_shnum));


	for(i=0;i<elf2.headerElf.e_shnum;i++)
	{
		if(elf2.tableSections[i].headerSec.sh_type==SHT_PROGBITS)
		{
			j=0;
			//for(j=0;j<elf1.headerElf.e_shnum;j++)
			while(j<elf3.headerElf.e_shnum && testpassage==0)
			{
				
				if(strcmp(elf3.tableSections[i].nomSec,elf2.tableSections[j].nomSec) == 0)
				{
					
					//elf3.tableSections[j].contenuSec = realloc(elf3.tableSections[j].contenuSec,elf2.tableSections[i].headerSec.sh_size+elf3.tableSections[j].headerSec.sh_size);
					elf3.tableSections[j].headerSec.sh_size = (elf3.tableSections[j].headerSec.sh_size+elf2.tableSections[i].headerSec.sh_size);
					elf3.tableSections[j].contenuSec= realloc(elf3.tableSections[j].contenuSec,elf2.tableSections[i].headerSec.sh_size+elf3.tableSections[j].headerSec.sh_size);
					if (elf3.tableSections[j].contenuSec!=NULL){
						for (int p=0;p<elf2.tableSections[i].headerSec.sh_size;p++)
					elf3.tableSections[j].contenuSec[elf3.tableSections[j].headerSec.sh_size+p] = elf2.tableSections[i].contenuSec[p];
					//(unsigned char *)strcat((char *)elf3.tableSections[j].contenuSec,(char *)elf2.tableSections[i].contenuSec);
					}
				}
				j++;
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
			while (j<elf3.headerElf.e_shnum && testpassage==0)
			{
				if(strcmp(elf2.tableSections[i].nomSec,elf3.tableSections[j].nomSec) == 0)
				{
					testpassage=1;
				}
				j++;
			}
			if(testpassage==0)
			{	
				
				//elf3.tableSections = realloc(elf3.tableSections,(sizeof(elf3.tableSections)+sizeof(elf2.tableSections[i])));
				elf3.tableSections = realloc(elf3.tableSections,(sizeof(elf3.tableSections)+sizeof(elf2.tableSections[i])));
				elf3.tableSections[j+1].contenuSec=malloc(elf2.tableSections[i].headerSec.sh_size);
				elf3.tableSections[j+1].nomSec = elf2.tableSections[i].nomSec;
				elf3.tableSections[j+1].headerSec = elf2.tableSections[i].headerSec;
				elf3.tableSections[j+1].contenuSec = elf2.tableSections[i].contenuSec;
			}
		}
	}

		return elf3;
}
