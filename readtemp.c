Elf32_Ehdr readHeader(FILE* ElfFile);
Section* readSectionTable(FILE* ElfFile);
Elf32_Sym* readSymboleTable(FILE* ElfFile);
Elf32_Rel* readRelocatableTable(FILE* ElfFile);


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
	Elf32_Shdr STRheader;
	char *STR_buffer_name=NULL;
	Section* sect;
	int iter_s; 
	
	char* type="";
	char* flags="";


	ELFheader=readHeader(ElfFile).
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
		sect[iter_s]->nomSec = STR_buffer_name+ITERheader.sh_name;
		sect[iter_s]->headerSec = ITERheader;
		fseek( ElfFile, sect[iter_s]->headerSec.sh_offset, SEEK_SET);
		fread( sect[iter_s]->contenuSec,sect[iter_s]->headerSec.sh_size,1,ElfFile );
	}
	

	free( STR_buffer_name );
	rewind(ElfFile);
	return sect;
}

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

Elf32_Sym* readSymboleTable(FILE* ElfFile)
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
    ELFheader=readHeader(ElfFile);
    //Lecture table des noms de symboles
    int indexTablestrSymbole = NameToIndex(".strtab");
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
}