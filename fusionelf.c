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
			printf("\n\n %s \n\n",sect[k].nomSec);  
			k++;
		}
	}
	return sect;
}


ElfFile fusionProgBit(ElfFile elf1, ElfFile elf2)
{
	int i;
	
	int j=0;
	ElfFile elf3;
	
	elf3.tableSections=malloc(sizeof(elf3.tableSections)*10);
	int k=0;
	for(int i=0;i<elf1.headerElf.e_shnum;i++)
	{

		if(elf1.tableSections[i].headerSec.sh_type==SHT_PROGBITS)
		{
			//elf3.tableSections = realloc(elf3.tableSections,taille+elf1.tableSections[i].headerSec.sh_size);
			//taille += elf3.tableSections[k].headerSec.sh_size;
			
			elf3.tableSections[k].nomSec = malloc(sizeof(elf1.tableSections[i].nomSec));
			elf3.tableSections[k].nomSec = elf1.tableSections[i].nomSec;
			
			elf3.tableSections[k].headerSec = elf1.tableSections[k].headerSec;
			
			elf3.tableSections[k].contenuSec = malloc(sizeof(elf1.tableSections[k].contenuSec));
			elf3.tableSections[k].contenuSec = elf1.tableSections[k].contenuSec;
			
			printf("\n %s \n",elf3.tableSections[k].nomSec);
			printf("\n %s \n",elf3.tableSections[k-1].nomSec); 
			k++;
		}
	}
	int nbSection = k+1;
	printf("\n\n %s \n\n",elf3.tableSections[k-2].nomSec);
  


	for(i=0;i<elf2.headerElf.e_shnum;i++)
	{
		printf("\noui1  %d  %s",elf2.tableSections[i].headerSec.sh_type,elf2.tableSections[i].nomSec);
		if(elf2.tableSections[i].headerSec.sh_type==SHT_PROGBITS)
		{
			printf("\nIf passé %s\n",elf2.tableSections[i].nomSec);
			for(j=0;j<nbSection-1;j++)
			{
					
				printf("\nBoucles: i=%d j=%d \t SECTION1= %s SECTION2=%s",i,j,elf3.tableSections[j].nomSec,elf2.tableSections[i].nomSec); 
				if(strcmp(elf3.tableSections[j].nomSec,elf2.tableSections[i].nomSec) == 0)
				{
				
			
					//elf3.tableSections[j].contenuSec = realloc(elf3.tableSections[j].contenuSec,elf2.tableSections[i].headerSec.sh_size+elf3.tableSections[j].headerSec.sh_size);
					elf3.tableSections[j].contenuSec = realloc(elf3.tableSections[j].contenuSec,elf2.tableSections[i].headerSec.sh_size+elf3.tableSections[j].headerSec.sh_size); //realloc


					if (elf3.tableSections[j].contenuSec!=NULL){
						for (int p=0;p<elf2.tableSections[i].headerSec.sh_size;p++){			//fusion des sections
							elf3.tableSections[j].contenuSec[elf3.tableSections[j].headerSec.sh_size+p] = elf2.tableSections[i].contenuSec[p];
						}
						elf3.tableSections[j].headerSec.sh_size = (elf3.tableSections[j].headerSec.sh_size+elf2.tableSections[i].headerSec.sh_size); //modif taille					
					}
				}

			}
		}
	}
	int testpassage;
	i=0;
	for(i=0;i<elf2.headerElf.e_shnum;i++)
	{
		testpassage=0;
		if(elf2.tableSections[i].headerSec.sh_type==SHT_PROGBITS)
		{	printf("\npasse");
			
			j=0;
			for(int j=0;j<nbSection-1;j++)
			//while (j<elf3.headerElf.e_shnum && testpassage==0)
			{
				printf("%s",elf3.tableSections[j].nomSec);
				if(strcmp(elf2.tableSections[i].nomSec,elf3.tableSections[j].nomSec) == 0)
				{
					testpassage=1;
				}
				//j++;
			}
			if(testpassage==0)
			{	
				
				//elf3.tableSections = realloc(elf3.tableSections,(sizeof(elf3.tableSections)+sizeof(elf2.tableSections[i])));
				//elf3.tableSections = realloc(elf3.tableSections,(sizeof(Section)*++nbSection));
				elf3.tableSections[j+1].contenuSec = malloc(elf2.tableSections[i].headerSec.sh_size);
				elf3.tableSections[j+1].nomSec = elf2.tableSections[i].nomSec;
				elf3.tableSections[j+1].headerSec = elf2.tableSections[i].headerSec;
				elf3.tableSections[j+1].contenuSec = elf2.tableSections[i].contenuSec;
				printf("%s",elf3.tableSections[j+1].contenuSec);
			}
		}
	}

		return elf3;
}
