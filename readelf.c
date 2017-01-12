#ifndef READELF_C
#define READELF_C

#include "readelf.h"
// ce fichier s'occupe de la mlecture d'un fichhier.o et stocke les informations dans les structures définies


//Lecture du header
Elf32_Ehdr readHeader(FILE* elfFile)
{
	rewind(elfFile);
	Elf32_Ehdr elf1;
	//On lit sur une taille de Elf32_Ehdr à partir du début du fichier pour lire le header
	if (1!= fread(&elf1, sizeof(elf1), 1, elfFile))
	{
		printf("failed to read elf header");
	}
	return elf1;
}

//Lecture de la table des sections
Section* readSectionTable(FILE* elfFile)
{

	Elf32_Ehdr ELFheader;
	Elf32_Shdr STRheader, ITERheader;                                                                                                                                                         
	char *STR_buffer_name=NULL;
	Section* sect = NULL;
	int iter_s; 

	ELFheader=readHeader(elfFile);

	//Déplacement et lecture de shtrtab
	int indexTableSTRSection = NameToIndex(".shstrtab", elfFile);
	fseek( elfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*indexTableSTRSection), SEEK_SET);
	fread( &STRheader, ELFheader.e_shentsize, 1, elfFile );

	// On crée une variable qui contiendra tout les noms de sections
    STR_buffer_name = (char *)malloc( STRheader.sh_size);

    if (STR_buffer_name == NULL) 
    {
	    printf("Impossible d'allouer la mémoire pour les noms de section\n");
    }
    else
    {
	    fseek( elfFile, STRheader.sh_offset, SEEK_SET);
	    fread( STR_buffer_name, STRheader.sh_size, 1, elfFile);

		//Iteration et écriture des noms de section
		fseek(elfFile, 0, SEEK_SET);
		sect = malloc(ELFheader.e_shnum*sizeof(Section));
		
		for ( iter_s=0; iter_s < ELFheader.e_shnum; iter_s++ )
		{
			fseek( elfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*iter_s), SEEK_SET);
			fread( &ITERheader, ELFheader.e_shentsize, 1, elfFile );
			sect[iter_s].nomSec = STR_buffer_name+ITERheader.sh_name; 	//nom
			sect[iter_s].headerSec = ITERheader; //header
			sect[iter_s].contenuSec = malloc(sect[iter_s].headerSec.sh_size);						
			fseek( elfFile, sect[iter_s].headerSec.sh_offset, SEEK_SET);
			fread( sect[iter_s].contenuSec,sect[iter_s].headerSec.sh_size,1,elfFile ); //contenu
		}
		//free(STR_buffer_name);
	}	
		// Replace le curseur du fseek-fread au début du fichier
		rewind(elfFile);
		return sect;
}

//Fonction qui renvoie le numero d'une section entrée en paramètre / Elle rea
int NameToIndex(char* nom_sect,FILE* elfFile)
{
	rewind(elfFile);
	Elf32_Ehdr ELFheader;
	Elf32_Shdr STRheader,ITERheader;
	char *STR_buffer=NULL;
	int sel = 0;
	int iter_s=0;
	//Lecture du Header
	fseek( elfFile, 0, SEEK_SET );
	fread( &ELFheader , sizeof(Elf32_Ehdr), 1, elfFile);

	//Déplacement et lecture de shtrtab
	for ( iter_s=0; iter_s < ELFheader.e_shnum; iter_s++)
	{
	fseek( elfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*iter_s), SEEK_SET);
	fread( &STRheader, ELFheader.e_shentsize, 1, elfFile );
	if ((STRheader.sh_type == SHT_STRTAB) && (STRheader.sh_addr == 0x00000000))
	{
		STR_buffer = malloc( STRheader.sh_size);
		  if (STR_buffer == NULL)
		  {
			printf("Impossible d'allouer la mémoire pour les noms de section\n");
			return -1;
		  }
		  fseek( elfFile, STRheader.sh_offset, SEEK_SET);
		  fread( STR_buffer, STRheader.sh_size, 1, elfFile);
		  iter_s = ELFheader.e_shnum+1;
		}
	}
		
	//Iterer sur les sections et enregistrer les noms de section
	fseek(elfFile, 0, SEEK_SET);
	for (iter_s=0; iter_s < ELFheader.e_shnum; iter_s++)
	{
		fseek( elfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*iter_s), SEEK_SET);
		fread( &ITERheader, ELFheader.e_shentsize, 1, elfFile );
			if (strcmp(nom_sect,STR_buffer+ITERheader.sh_name) == 0 && sel ==0)
			{
				sel = iter_s;
			}
	}

	free(STR_buffer);
	rewind(elfFile);
	return sel;
}

Symbole* readSymboleTable(FILE* elfFile)
{
	Elf32_Ehdr ELFheader;
	Elf32_Shdr STRheader;
	Elf32_Sym symb;
	
    char *STR_buffer_name=NULL;
	int i;

    //lecture header du fichier
    ELFheader=readHeader(elfFile);
    //Lecture table des noms de symboles
    int indexTablestrSymbole = NameToIndex(".strtab", elfFile);
    fseek( elfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*indexTablestrSymbole), SEEK_SET);
    fread( &STRheader, ELFheader.e_shentsize, 1, elfFile );

    STR_buffer_name = (char *)malloc( STRheader.sh_size);

    if (STR_buffer_name == NULL) 
    {
	    printf("Impossible d'allouer la mémoire pour les noms de symboles\n");
    }
    fseek( elfFile, STRheader.sh_offset, SEEK_SET);
    fread( STR_buffer_name, STRheader.sh_size, 1, elfFile);
	
    //Lecture de l'en tete de section symbole
	int indice = NameToIndex(".symtab", elfFile);
	fseek( elfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*indice), SEEK_SET);
	fread( &STRheader, ELFheader.e_shentsize, 1, elfFile );
	
	fseek(elfFile, STRheader.sh_offset, SEEK_SET);

	//Pour chaque entrée dans la table des symboles
	Symbole * sym = malloc((STRheader.sh_size/sizeof(Elf32_Sym))*sizeof(Symbole));
	for(i=0; i<(STRheader.sh_size/sizeof(Elf32_Sym)); i++)
	{
	    //Lecture du symbole courant
	    fread(&symb,sizeof(Elf32_Sym),1,elfFile);	
		sym[i].nomSymb = STR_buffer_name+symb.st_name;
		sym[i].headerSymb = symb;
	}
	return sym;
}

Elf32_Rel* readRelocatableTable(FILE* elfFile)
{
	Elf32_Ehdr ELFheader;
	Elf32_Shdr STRheader;
	Elf32_Rel rel;
	Elf32_Rel* rela= NULL;

	int i;
    int iter_s=0;
	
    //récupération header
	fseek( elfFile, 0, SEEK_SET );	
	fread( &ELFheader , sizeof(Elf32_Ehdr), 1, elfFile);
	rela= malloc(ELFheader.e_shnum* sizeof(Elf32_Rel));

    //les sections de type SHT_REL
   for ( iter_s=0; iter_s < ELFheader.e_shnum; iter_s++)
    {
        fseek( elfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*iter_s), SEEK_SET);
        fread( &STRheader, ELFheader.e_shentsize, 1, elfFile );
        if(STRheader.sh_type==SHT_REL)
        {
            fseek(elfFile, STRheader.sh_offset, SEEK_SET);
            for(i=0; i<(STRheader.sh_size/sizeof(Elf32_Rel)); i++)
            {
                fread(&rel,sizeof(Elf32_Rel),1,elfFile);
                rela[i]=rel;
            }
        }
    }
    return rela;	
}

// Fonction remplissant les structures pour un fichier elf donné en paramètre
ElfFile ElfConstructor(FILE* elfFile)
{
	ElfFile elf;
	//printf("\n==ELFFILE==");
	elf.fichierElf = elfFile;
	//printf("\nfichier chargé");
	elf.headerElf = readHeader(elfFile);
	//printf("\nheader chargé");
	elf.tableSections = readSectionTable(elfFile);
	//printf("\nsections chargé");
	elf.tableSymb = readSymboleTable(elfFile);
	//printf("\nsymb chargé");
	elf.tableRelocation = readRelocatableTable(elfFile);
	return elf;
}

#endif

