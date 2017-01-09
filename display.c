#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "util.h"
#include "elf.h"




int afficherSection(unsigned char* section)
{
			//affichage du contenu de la section
	for(i=0; i<Elfheader.sh_size; i++)
	{
		if(i % 16 == 0)
		{		//affichage de l'addresse relative des contenus de la section
			printf("\n0x%08x",affichage_addr);
			affichage_addr = affichage_addr + 16;
		}
		if(i%4 == 0)
		{		//espaces apres chaque 8 chiffres affiches
		printf(" ");
		}
		printf("%02x",section[i]);
	}
	printf("\n");
	return 0;
}


int displayHeaderInfos(FILE* ElfFile,Elf32_Ehdr elf1)
{	//affiche les informations du header

	int i;
	printf("\n===INFOS HEADER===");
	printf("\nMagic  :");
	for(i=0;i<15;++i) 
	printf("%x",elf1.e_ident[i]);

	char *class;
	if(elf1.e_ident[4]==2)
		class = "64 bits";
	if(elf1.e_ident[4]==1)
		class = "32 bits";
	if(elf1.e_ident[4]==0)
	  class = "Inavalid Class";

	if(elf1.e_machine!=0)
	  printf("\nPlateforme Cible: %"PRIu32,elf1.e_machine);

	printf("\nTaille des mots : %s",class);
	printf("\nType :");
	if(elf1.e_type == 1)
		printf(" Relocatable");
	else if(elf1.e_type == 2)
		printf(" Executable");
	else if(elf1.e_type == 3)
		printf(" Shared Object");
	else
		printf("Unknown");
		  
	printf("\n\t Offset : %"PRIu32,elf1.e_shoff); 
	printf("\n\t Taille : %"PRIu32,elf1.e_shentsize);
	printf(" (octets)\n\t Nombre d'entrée(s) : %"PRIu32,elf1.e_shnum);
	printf("\n\nIndex de l'entrée : %"PRIu32,elf1.e_shstrndx);
	printf("\nTaille totale de l'header : %"PRIu32,elf1.e_ehsize);
	printf(" (octets)\n");
	return 0;
}


int displaySymbolTable(FILE* ElfFile)
{
	int i;
	int idx;
    char * type=NULL;
    char * link=NULL;
    char buf[12];
    char SHNDX[10];
    char other[10];

    printf("\n===TABLE DES SYMBOLES===\n");
	//for each entry in the symbol table
    printf("NUMERO| NAME     | TYPE     | VALEUR     | TAILLE     | LIEN   | SHNDX | OTHER \n");
	for(i=0; i<(STRheader.sh_size/sizeof(Elf32_Sym)); i++)
	{
	    //read the current symbol
	    fread(&symb,sizeof(Elf32_Sym),1,ElfFile);
			idx=symb.st_name;

	    //numero alias buf
	    sprintf(buf, "[%d]", i);

	     //type
	    switch (ELF32_ST_TYPE(symb.st_info))
	    {
	        case 0:
	            type="NOTYPE";
	            break;
	        case 1:
	            type="OBJECT";
	            break;
	        case 2:
	            type="FUNCTION";
	            break;
	        case 3:
	            type="SECTION";
	            break;
	        case 4:
	            type="FILE";
	            break;
	        default:
	            break;
	    }

	     //bind
	    switch(ELF32_ST_BIND(symb.st_info))
	    {
	        case 0: 
	            link="LOCAL";
	            break;
	        case 1: 
	            link="GLOBAL";
	            break;
	        /*case 2: printf(" WEAK");
	            break;
	        case 13: printf("LOPROC");
	            break;*/
	        default:
	            break;
	    }

	     //shndx
	    if(symb.st_shndx == 0)
	    {
	        sprintf(SHNDX, "%s", "UND");
	    }
	    else if(symb.st_shndx>=10)
	    {
	        sprintf(SHNDX, "%s", "ABS");
	    }
	    else
	    {
	        sprintf(SHNDX, "%d", symb.st_shndx);
	    }

	    //other
	     if(symb.st_other==0)
	    {
	        sprintf(other, "%s","DEFAULT");
	    }
	    else
	    {
	        sprintf(other," %d ",symb.st_other);
	    }


	    printf("%5s | %9s| %8s | 0x%.8x | 0x%.8x | %6s | %5s | %5s", buf, STR_buffer_name+symb.st_name, type, symb.st_value, symb.st_size, link, SHNDX, other);
	    printf("\n");

	}


int displayRelocatableTable(FILE* ElfFile)
{
	Elf32_Rel rel;
	Elf32_Rela rela;
	int i;
    int iter_s=0;

    printf("=========TABLE DE RELOCATION=========\n");
	
    printf("DECALAGE    | TYPE            | INDEX ENTREE\n");

    //les sections de type SHT_REL
   for ( iter_s=0; iter_s < ELFheader.e_shnum; iter_s++  )
    {
            for(i=0; i<(STRheader.sh_size/sizeof(Elf32_Rel)); i++)
            {
                fread(&rel,sizeof(Elf32_Rel),1,ElfFile);
                printf(" 0x%.8x | %15s | %d \n", rel.r_offset, tableRelocationARMCode[ELF32_R_TYPE(rel.r_info)], ELF32_R_SYM(rel.r_info));
            }
        }
    }
void displayMenu(FILE* ElfFile){
   int choix=100;

	 while(choix != 0){
	 	printf("====Menu===\n");
	 	printf("Choisir:\n 0- Sortir\n 1- Afficher les informations du Header\n 2- Afficher la table des sections\n 3- Afficher une section\n 4- Afficher la table des symboles\n 5- Afficher la table de relocation\n");

	 	scanf("%i", &choix);
	 	switch(choix)
	 	{
             case 0:
                 break;
	 		case 1:
	 			displayHeaderInfos(ElfFile);
	 			break;
	 		case 2:
	 			displayNameSection(ElfFile);
	 			break;
	 		case 3:
	 			afficherSec(ElfFile);
	 			break;
	 		case 4:
	 			displaySymbolTable(ElfFile);
	 			break;
	 		case 5:
	 			displayRelocatableTable(ElfFile);
	 			break;
	 		default:
	 			printf("Le choix ne correspond pas\n");
	 			break;
	 		}
	 }
}
