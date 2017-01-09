#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "util.h"
#include "elf.h"


int displayHeaderInfos(FILE* ElfFile);
int displayNameSection(FILE* ElfFile);
int afficherSec(FILE* ElfFile);
int displaySymbolTable(FILE* ElfFile);
int NameToIndex(char* nom_sect);


FILE* ElfFile = NULL;

//les 30 premiers codes de la table des codes de relocation ARM
char * tableRelocationARMCode[]={"R_ARM-NONE","R_ARM_PC24","R_ARM_ABS32","R_ARM_REL32","R_ARM_LDR_PC_G0","R_ARM_ABS16","R_ARM_ABS12","R_ARM_THM_ABS5","R_ARM_ABS8","R_ARM_SBREL32",
"R_ARM_THM_CALL","R_ARM_THM_PC8","R_ARM_BREL_ADJ","R_ARM_TLS_DESC","R_ARM_THM_SWI8","R_ARM_XPC25","R_ARM_THM_XPC22","R_ARM_TLS_DTPMOD32","R_ARM_TLS_DTPOFF32","R_ARM_TLS_TPOFF32",
"R_ARM_COPY","R_ARM_GLOB_DAT","R_ARM_JUMP_SLOT","R_ARM_RELATIVE","R_ARM_GOTOFF32","R_ARM_BASE_PREL","R_ARM_GOT_BREL","R_ARM_PLT32","R_ARM_CALL","R_ARM_JUMP24","R_ARM_THM_JUMP24"};

int NameToIndex(char* nom_sect)
{
	rewind(ElfFile);
	Elf32_Ehdr ELFheader;
	Elf32_Shdr STRheader,ITERheader;
	char *STR_buffer=NULL;
	int sel = 0;
	int iter_s=0;
	//read header
	fseek( ElfFile, 0, SEEK_SET );
	fread( &ELFheader , sizeof(Elf32_Ehdr), 1, ElfFile);


	//find string section
	for ( iter_s=0; iter_s < ELFheader.e_shnum; iter_s++)
	{
	fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*iter_s), SEEK_SET);
	fread( &STRheader, ELFheader.e_shentsize, 1, ElfFile );
	if ((STRheader.sh_type == SHT_STRTAB) && (STRheader.sh_addr == 0x00000000))
	{
		STR_buffer = malloc( STRheader.sh_size);
		  if (STR_buffer == NULL)
		  {
			printf("Impossible d'allouer la mémoire pour les noms de section\n");
			return -1;
		  }
		  fseek( ElfFile, STRheader.sh_offset, SEEK_SET);
		  fread( STR_buffer, STRheader.sh_size, 1, ElfFile);
		  iter_s = ELFheader.e_shnum+1;
		}
	}
		
	//now iter and print segment names
	fseek(ElfFile, 0, SEEK_SET);
	for (iter_s=0; iter_s < ELFheader.e_shnum; iter_s++)
	{
		fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*iter_s), SEEK_SET);
		fread( &ITERheader, ELFheader.e_shentsize, 1, ElfFile );
			if (strcmp(nom_sect,STR_buffer+ITERheader.sh_name) == 0 && sel ==0)
			{
				sel = iter_s;
			}
	}
	// printf("%d\n",sel);
	free(STR_buffer);
	rewind(ElfFile);
	return sel;
}


int displayHeaderInfos(FILE* ElfFile)
{//affiche les informations du header
	Elf32_Ehdr elf1;
	
	if (1!= fread(&elf1, sizeof(elf1), 1, ElfFile))
	{
		printf("failed to read elf header");
		return -1;
	}
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
	rewind(ElfFile);
	return 0;
}

int displayNameSection(FILE* ElfFile)
{	//affiche les noms de sections
	
	Elf32_Ehdr ELFheader;
	Elf32_Shdr STRheader,ITERheader;
	char *STR_buffer_name=NULL;
	int iter_s; 
	
	char* type="";
	char* flags="";

	//Lecture du header
	fseek( ElfFile, 0, SEEK_SET );
	fread( &ELFheader , sizeof(Elf32_Ehdr), 1, ElfFile);
	printf("\n===TABLE DES SECTIONS===\n");
	
    //find string section
    int indexTablestrSection = NameToIndex(".shstrtab");
	fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*indexTablestrSection), SEEK_SET);
	fread( &STRheader, ELFheader.e_shentsize, 1, ElfFile );

    //printf("coucou");
    STR_buffer_name = (char *)malloc( STRheader.sh_size);

    if (STR_buffer_name == NULL) 
    {
	    printf("Impossible d'allouer la mémoire pour les noms de section\n");
	    return -1;
    }
    fseek( ElfFile, STRheader.sh_offset, SEEK_SET);
    fread( STR_buffer_name, STRheader.sh_size, 1, ElfFile);

	//now iter and print segment names
	fseek(ElfFile, 0, SEEK_SET);
  	printf("NUMERO | NAME           | TYPE         | OFFSET | ADDR | SIZE | ENTSIZE| LINK | INFO | ADDRALIGN| FLAGS\n");
	for ( iter_s=0; iter_s < ELFheader.e_shnum; iter_s++ )
	{
		fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*iter_s), SEEK_SET);
		fread( &ITERheader, ELFheader.e_shentsize, 1, ElfFile );
		switch(ITERheader.sh_type)
		{
			case 0: type = "NULL";
		            break;
		    case 1: type = "PROGBITS";
		            break;
		    case 2: type = "SYMTAB";
		            break;
		    case 3: type = "STRTAB";
		            break;
		    case 4: type = "RELA";
		            break;
		    case 5: type = "HASH";
		            break;
		    case 6: type = "DYNAMIC";
		            break;
		    case 7: type = "NOTE";
		            break;
		    case 8: type = "NOBITS";
		            break;
		    case 9: type = "REL";
		            break;
		    case 10: type = "SHLIB";
		            break;
		    case 11: type = "DYNSYM";
		            break;
		    default:
		            break;
		}
		if (1==1)
		{
		  printf("%6i |%15s | %12s | %6i | %4i | %4i | %6i | %4i | %4i | %8i | %3i \n",iter_s, 
		  	STR_buffer_name+ITERheader.sh_name , type ,ITERheader.sh_offset , ITERheader.sh_addr, 
		  	ITERheader.sh_size, ITERheader.sh_entsize, ITERheader.sh_link, 
		  	ITERheader.sh_info, ITERheader.sh_addralign, ITERheader.sh_flags);
		}
	}	
	free( STR_buffer_name );
	rewind(ElfFile);
	return 0;
}

int afficherSec(FILE* ElfFile)
{
	
	Elf32_Ehdr ELFheader;
	Elf32_Shdr STRheader,ITERheader;
	char *STR_buffer=NULL;
	
	int numcar = 0;
	int i = 0;
	
	printf("\n===AFFICHAGE CONTENU SECTION===\n");
	
	//Lecture du header
	fseek( ElfFile, 0, SEEK_SET );
	fread( &ELFheader , sizeof(Elf32_Ehdr), 1, ElfFile);

	char nom_sect[20];
	int sel = 0;
	
	printf("nom ou numero de section a afficher:");
	numcar = scanf("%s", nom_sect);
	
	if (nom_sect[0] <= '9' && nom_sect[0] >= '0')
	{
		while(i < numcar)
		{
			sel = (int)nom_sect[i]-48;
			i++;
		}
	}
	else
	{
		sel=NameToIndex(nom_sect);
	}
	//Lecture du Section Header selectionné
	unsigned char *STR_buffer2=NULL;
	int affichage_addr = 0;
	
	fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*sel), SEEK_SET);
	fread( &STRheader, ELFheader.e_shentsize, 1, ElfFile );
	STR_buffer2 = malloc(sizeof(char)*STRheader.sh_size);			//allouer une taille suffisante pour les grandes tailles de sections
	
	//cas où la section est vide
	if(STRheader.sh_size == 0)
	{
		printf("section vide");
	}
	else
	{
		fseek( ElfFile, STRheader.sh_offset, SEEK_SET);
		fread( STR_buffer2,STRheader.sh_size,1,ElfFile );
	}
	i=0;

	//affichage du contenu de la section
	for(i=0; i<STRheader.sh_size; i++)
	{	if(i % 16 == 0)
		{					//affichage de l'addresse relative des contenus de la section
			printf("\n0x%08x",affichage_addr);
			affichage_addr = affichage_addr + 16;
		}
		if(i%4 == 0)
		{			//espaces apres chaque 8 chiffres affiches
			printf(" ");
		}
		printf("%02x",STR_buffer2[i]);
	}
	printf("\n");

	free(STR_buffer2);
	rewind(ElfFile);
	return 0;
}

int displaySymbolTable(FILE* ElfFile)
{
	Elf32_Ehdr ELFheader;
	Elf32_Shdr STRheader,ITERheader;
	Elf32_Sym symb;
    char *STR_buffer_name=NULL;
	int indice;
	int i;
	int idx;
	int num_sym;
    char * type=NULL;
    char * link=NULL;
    char buf[12];
    char SHNDX[10];
    char other[10];

    printf("\n===TABLE DES SYMBOLES===\n");

    //lecture header du fichier
    fseek(ElfFile, 0, SEEK_SET);
    fread(&ELFheader, sizeof(Elf32_Ehdr),1,ElfFile);

    //Lecture table des noms de symboles
    int indexTablestrSymbole = NameToIndex(".strtab");
    fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*indexTablestrSymbole), SEEK_SET);
    fread( &STRheader, ELFheader.e_shentsize, 1, ElfFile );

    STR_buffer_name = (char *)malloc( STRheader.sh_size);

    if (STR_buffer_name == NULL) 
    {
	    printf("Impossible d'allouer la mémoire pour les noms de symboles\n");
	    return -1;
    }
    fseek( ElfFile, STRheader.sh_offset, SEEK_SET);
    fread( STR_buffer_name, STRheader.sh_size, 1, ElfFile);
	
    //Lecture de l'en tete de section symbole
	indice = NameToIndex(".symtab");
	fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*indice), SEEK_SET);
	fread( &STRheader, ELFheader.e_shentsize, 1, ElfFile );

	fseek(ElfFile, STRheader.sh_offset, SEEK_SET);

	//for each entry in the symbol table
    printf("NUMERO| NAME     | TYPE     | VALEUR     | TAILLE     | LIEN   | SHNDX | OTHER \n");
	for(i=0; i<(STRheader.sh_size/sizeof(Elf32_Sym)); i++)
	{
	    //read the current symbol
	    fread(&symb,sizeof(Elf32_Sym),1,ElfFile);
			idx=symb.st_name;

	    //multiple lines to get formatting correct
	    //prints index in brackets right aligned
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
	
}

int displayRelocatableTable(FILE* ElfFile)
{

	Elf32_Ehdr ELFheader;
	Elf32_Shdr STRheader,ITERheader;
	Elf32_Rel rel;
	Elf32_Rela rela;
	char *STR_buffer=NULL;
	int i;
    int iter_s=0;

    printf("=========TABLE DE RELOCATION=========\n");
	
    //récupération header
	fseek( ElfFile, 0, SEEK_SET );	
	fread( &ELFheader , sizeof(Elf32_Ehdr), 1, ElfFile);

    printf("DECALAGE    | TYPE            | INDEX ENTREE\n");

    //les sections de type SHT_REL
   for ( iter_s=0; iter_s < ELFheader.e_shnum; iter_s++  )
    {
        fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*iter_s), SEEK_SET);
        fread( &STRheader, ELFheader.e_shentsize, 1, ElfFile );
        if(STRheader.sh_type==SHT_REL)
        {
            fseek(ElfFile, STRheader.sh_offset, SEEK_SET);
            for(i=0; i<(STRheader.sh_size/sizeof(Elf32_Rel)); i++)
            {
                fread(&rel,sizeof(Elf32_Rel),1,ElfFile);
                printf(" 0x%.8x | %15s | %d \n", rel.r_offset, tableRelocationARMCode[ELF32_R_TYPE(rel.r_info)], ELF32_R_SYM(rel.r_info));
            }
        }
    }	
}


int main(int argc, char **argv) 
{
  //uint32_t idx;
  //char* Header = NULL;
	//char* SectNames = NULL;
  //Elf32_Shdr elfSH;
  if(argc != 2) {
    printf("usage: %s <ELF_FILE>\n", argv[0]);
    exit(1);
  }

  if((ElfFile = fopen(argv[1], "r")) == NULL) {
    perror("[E] Error opening file:");
    exit(1);
  }
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

