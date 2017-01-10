#include "display.h"



//les 30 premiers codes de la table des codes de relocation ARM
char * tableRelocationARMCode[]={"R_ARM-NONE","R_ARM_PC24","R_ARM_ABS32","R_ARM_REL32","R_ARM_LDR_PC_G0","R_ARM_ABS16","R_ARM_ABS12","R_ARM_THM_ABS5","R_ARM_ABS8","R_ARM_SBREL32",
"R_ARM_THM_CALL","R_ARM_THM_PC8","R_ARM_BREL_ADJ","R_ARM_TLS_DESC","R_ARM_THM_SWI8","R_ARM_XPC25","R_ARM_THM_XPC22","R_ARM_TLS_DTPMOD32","R_ARM_TLS_DTPOFF32","R_ARM_TLS_TPOFF32",
"R_ARM_COPY","R_ARM_GLOB_DAT","R_ARM_JUMP_SLOT","R_ARM_RELATIVE","R_ARM_GOTOFF32","R_ARM_BASE_PREL","R_ARM_GOT_BREL","R_ARM_PLT32","R_ARM_CALL","R_ARM_JUMP24","R_ARM_THM_JUMP24"};


void displaySection(ElfFile elf1)
{
	int i=0;
	int affichage_addr=0;
	int choix =  0;
	char nom_sect[20];

	
	printf("nom ou numero de section a afficher:");
	int numcar = scanf("%s", nom_sect);

	if (nom_sect[0] <= '9' && nom_sect[0] >= '0')
	{
		while(i < numcar)
		{
			choix = (int)nom_sect[i]-48;
			i++;
		}
	}
	else
	{
		choix=NameToIndex(nom_sect,elf1.fichierElf);
	}	
	
			//affichage du contenu de la section
	for(i=0; i<elf1.tableSections[choix].headerSec.sh_size; i++)
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
		printf("%02x",elf1.tableSections[choix].contenuSec[i]);
	}
	printf("\n");
}

void displaySectionTable(ElfFile elf1)
{	//affiche les noms de sections
	//iter and print segment names
	char* type;
	int iter_s=0;

  	printf("NUMERO | NAME           | TYPE         | OFFSET | ADDR | SIZE | ENTSIZE| LINK | INFO | ADDRALIGN| FLAGS\n");
	for ( iter_s=0; iter_s < elf1.headerElf.e_shnum; iter_s++ )
	{
		switch(elf1.tableSections[iter_s].headerSec.sh_type)
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
		  printf("%6i |%15s | %12s | %6i | %4i | %4i | %6i | %4i | %4i | %8i | %3i \n",iter_s, elf1.tableSections[iter_s].nomSec,type,elf1.tableSections[iter_s].headerSec.sh_offset ,
			elf1.tableSections[iter_s].headerSec.sh_addr,elf1.tableSections[iter_s].headerSec.sh_size,elf1.tableSections[iter_s].headerSec.sh_entsize, elf1.tableSections[iter_s].headerSec.sh_link,
			elf1.tableSections[iter_s].headerSec.sh_info, elf1.tableSections[iter_s].headerSec.sh_addralign, elf1.tableSections[iter_s].headerSec.sh_flags);
		}
	}
}

void displayHeaderInfos(ElfFile elf1)
{	
	//affiche les informations du header
	
	int i;
	printf("\n===INFOS HEADER===");
	printf("\nMagic  :");
	for(i=0;i<15;++i) 
	printf("%x",elf1.headerElf.e_ident[i]);

	char *class;
	if(elf1.headerElf.e_ident[4]==2)
		class = "64 bits";
	if(elf1.headerElf.e_ident[4]==1)
		class = "32 bits";
	if(elf1.headerElf.e_ident[4]==0)
	  class = "Inavalid Class";

	if(elf1.headerElf.e_machine!=0)
	  printf("\nPlateforme Cible: %"PRIu32,elf1.headerElf.e_machine);

	printf("\nTaille des mots : %s",class);
	printf("\nType :");
	if(elf1.headerElf.e_type == 1)
		printf(" Relocatable");
	else if(elf1.headerElf.e_type == 2)
		printf(" Executable");
	else if(elf1.headerElf.e_type == 3)
		printf(" Shared Object");
	else
		printf("Unknown");
		  
	printf("\n\t Offset : %"PRIu32,elf1.headerElf.e_shoff); 
	printf("\n\t Taille : %"PRIu32,elf1.headerElf.e_shentsize);
	printf(" (octets)\n\t Nombre d'entrée(s) : %"PRIu32,elf1.headerElf.e_shnum);
	printf("\n\nIndex de l'entrée : %"PRIu32,elf1.headerElf.e_shstrndx);
	printf("\nTaille totale de l'header : %"PRIu32,elf1.headerElf.e_ehsize);
	printf(" (octets)\n");

}


void displaySymbolTable(ElfFile elf1)
{
	int i;
    char * type=NULL;
    char * link=NULL;
    char buf[12];
    char SHNDX[10];
    char other[10];
    int indiceTabSym = NameToIndex(".symtab",elf1.fichierElf);

	printf("\n===TABLE DES SYMBOLES===\n");
	//for each entry in the symbol table
    printf("NUMERO| NAME     | TYPE     | VALEUR     | TAILLE     | LIEN   | SHNDX | OTHER \n");
	for(i=0; i<(elf1.tableSections[indiceTabSym].headerSec.sh_size/sizeof(Elf32_Sym)); i++)
	{
	    //numero alias buf
	    sprintf(buf, "[%d]", i);

	     //type
	    switch (ELF32_ST_TYPE(elf1.tableSymb[i].headerSymb.st_info))
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
	    switch(ELF32_ST_BIND(elf1.tableSymb[i].headerSymb.st_info))
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
	    if(elf1.tableSymb[i].headerSymb.st_shndx == 0)
	    {
	        sprintf(SHNDX, "%s", "UND");
	    }
	    else if(elf1.tableSymb[i].headerSymb.st_shndx >= 10)
	    {
	        sprintf(SHNDX, "%s", "ABS");
	    }
	    else
	    {
	        sprintf(SHNDX, "%d", elf1.tableSymb[i].headerSymb.st_shndx);
	    }

	    //other
	     if(elf1.tableSymb[i].headerSymb.st_other==0)
	    {
	        sprintf(other, "%s","DEFAULT");
	    }
	    else
	    {
	        sprintf(other," %d ",elf1.tableSymb[i].headerSymb.st_other);
	    }


	    printf("%5s | %9s| %8s | 0x%.8x | 0x%.8x | %6s | %5s | %5s", buf, elf1.tableSymb[i].nomSymb, type, elf1.tableSymb[i].headerSymb.st_value, elf1.tableSymb[i].headerSymb.st_size, link, SHNDX, other);
	    printf("\n");

	}

}

void displayRelocatableTable(ElfFile elf1)
{
		
	//Elf32_Rel rel;
	//Elf32_Rela rela;
	int i = NameToIndex(".rel.text",elf1.fichierElf);
    int iter_s=0;
	
    printf("=========TABLE DE RELOCATION=========\n");
	
    printf("DECALAGE    | TYPE            | INDEX ENTREE\n");

    //les sections de type SHT_REL
	for ( iter_s=0; iter_s < elf1.tableSections[i].headerSec.sh_size/sizeof(Elf32_Rel); iter_s++  )
	{
		printf(" 0x%.8x | %15s | %d \n", elf1.tableRelocation[iter_s].r_offset, tableRelocationARMCode[ELF32_R_TYPE(elf1.tableRelocation[iter_s].r_info)], ELF32_R_SYM(elf1.tableRelocation[iter_s].r_info));
	}
}

void displayMenu(FILE* elffile_o)
{
   int choix=100;
	ElfFile elffile = ElfConstructor(elffile_o);
	elffile.headerElf = readHeader(elffile_o);
	elffile.tableSections = readSectionTable(elffile_o);
	elffile.tableSymb = readSymboleTable(elffile_o);
	elffile.tableRelocation = readRelocatableTable(elffile_o);
	
	
	 while(choix != 0){
	 	printf("====Menu===\n");
	 	printf("Choisir:\n 0- Sortir\n 1- Afficher les informations du Header\n 2- Afficher la table des sections\n 3- Afficher une section\n 4- Afficher la table des symboles\n 5- Afficher la table de relocation\n");

	 	scanf("%i", &choix);
	 	switch(choix)
	 	{
             case 0:
                 break;
	 		case 1:
	 			displayHeaderInfos(elffile);
	 			break;
	 		case 2:
	 			displaySectionTable(elffile);
	 			break;
	 		case 3:
	 			displaySection(elffile);
	 			break;
	 		case 4:
	 			displaySymbolTable(elffile);
	 			break;
	 		case 5:
	 			displayRelocatableTable(elffile);
	 			break;
	 		default:
	 			printf("Le choix ne correspond pas\n");
	 			break;
	 		}
	 }
}
