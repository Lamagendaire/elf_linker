#include "readtemp.h"

Elf32_Ehdr readHeader(FILE* ElfFile)
{
	rewind(ElfFile);
	Elf32_Ehdr elf1;
	if (1!= fread(&elf1, sizeof(elf1), 1, ElfFile))
	{
		printf("failed to read elf header");
	}
	return elf1;
}

Section* readSectionTable(FILE* ElfFile)
{

	Elf32_Ehdr ELFheader;
	Elf32_Shdr STRheader, ITERheader;
	char *STR_buffer_name=NULL;
	Section* sect = NULL;
	int iter_s; 



	ELFheader=readHeader(ElfFile);
    STR_buffer_name = (char *)malloc( STRheader.sh_size);

    if (STR_buffer_name == NULL) 
    {
	    printf("Impossible d'allouer la mémoire pour les noms de section\n");
    }
    fseek( ElfFile, STRheader.sh_offset, SEEK_SET);
    fread( STR_buffer_name, STRheader.sh_size, 1, ElfFile);

	//iter and print segment names
	fseek(ElfFile, 0, SEEK_SET);
	for ( iter_s=0; iter_s < ELFheader.e_shnum; iter_s++ )
	{
		fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*iter_s), SEEK_SET);
		fread( &ITERheader, ELFheader.e_shentsize, 1, ElfFile );
		sect[iter_s].nomSec = STR_buffer_name+ITERheader.sh_name;
		sect[iter_s].headerSec = ITERheader;
		fseek( ElfFile, sect[iter_s].headerSec.sh_offset, SEEK_SET);
		fread( sect[iter_s].contenuSec,sect[iter_s].headerSec.sh_size,1,ElfFile );
	}
	

	free( STR_buffer_name );
	rewind(ElfFile);
	return sect;
}

int NameToIndex(char* nom_sect,FILE* ElfFile)
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
		
	//iter and print segment names
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

	free(STR_buffer);
	rewind(ElfFile);
	return sel;
}

Symbole* readSymboleTable(FILE* ElfFile)
{
	Elf32_Ehdr ELFheader;
	Elf32_Shdr STRheader;
	Elf32_Sym symb;
	Symbole * sym = NULL;
    char *STR_buffer_name=NULL;
	int i;

    //lecture header du fichier
    ELFheader=readHeader(ElfFile);
    //Lecture table des noms de symboles
    int indexTablestrSymbole = NameToIndex(".strtab", ElfFile);
    fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*indexTablestrSymbole), SEEK_SET);
    fread( &STRheader, ELFheader.e_shentsize, 1, ElfFile );

    STR_buffer_name = (char *)malloc( STRheader.sh_size);

    if (STR_buffer_name == NULL) 
    {
	    printf("Impossible d'allouer la mémoire pour les noms de symboles\n");
    }
    fseek( ElfFile, STRheader.sh_offset, SEEK_SET);
    fread( STR_buffer_name, STRheader.sh_size, 1, ElfFile);
	
    //Lecture de l'en tete de section symbole
	int indice = NameToIndex(".symtab", ElfFile);
	fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*indice), SEEK_SET);
	fread( &STRheader, ELFheader.e_shentsize, 1, ElfFile );

	fseek(ElfFile, STRheader.sh_offset, SEEK_SET);

	//for each entry in the symbol table

	for(i=0; i<(STRheader.sh_size/sizeof(Elf32_Sym)); i++)
	{
	    //read the current symbol
	    fread(&symb,sizeof(Elf32_Sym),1,ElfFile);	
		sym[i].nomSymb = STR_buffer_name+symb.st_name;
		sym[i].headerSymb = symb;

	}
	return sym;
	
}
Elf32_Rel* readRelocatableTable(FILE* ElfFile)
{
	Elf32_Ehdr ELFheader;
	Elf32_Shdr STRheader;
	Elf32_Rel rel;
	Elf32_Rel* rela= NULL;

	int i;
    int iter_s=0;
	
    //récupération header
	fseek( ElfFile, 0, SEEK_SET );	
	fread( &ELFheader , sizeof(Elf32_Ehdr), 1, ElfFile);


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
                rela[i]=rel;
            }
        }
    }
    return rela;	
}

ElfFile ElfConstructor(FILE* elfFile)
{
	ElfFile elf;
	elf.fichierElf = elfFile;
	elf.headerElf = readHeader(elfFile);
	elf.tableSections = readSectionTable(elfFile);
	elf.tableSymb = readSymboleTable(elfFile);
	elf.tableRelocation = readRelocatableTable(elfFile);
	return elf;
}
