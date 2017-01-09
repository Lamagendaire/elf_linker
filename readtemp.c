Elf32_Ehdr readHeader(FILE* ElfFile);
Elf32_Shdr* readSectionTable(FILE* ElfFile);
unsigned char* readSection(FILE* ElfFile);
Elf32_Sym* readSymboleTable(FILE* ElfFile);
Elf32_Rel* readRelocatableTable(FILE* ElfFile);


Elf32_Ehdr readHeader(FILE* ElfFile)
{

	Elf32_Ehdr elf1;
	if (1!= fread(&elf1, sizeof(elf1), 1, ElfFile))
	{
		printf("failed to read elf header");
		return -1;
	}
	return elf1;
}

Elf32_Shdr* readSectionTable(FILE* ElfFile)
{

	Elf32_Ehdr ELFheader;
	Elf32_Shdr STRheader;
	Elf32_Shdr* SectionTable==NULL;
	char *STR_buffer_name=NULL;
	int iter_s; 
	
	char* type="";
	char* flags="";

	int indexTablestrSection = NameToIndex(".shstrtab");
	fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*indexTablestrSection), SEEK_SET);
	fread( &STRheader, ELFheader.e_shentsize, 1, ElfFile );

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
		fread(SectionTable[iter_s], ELFheader.e_shentsize, 1, ElfFile );
	}	
	free( STR_buffer_name );
	return SectionTable;
}

unsigned char* readSection(FILE* ElfFile){

	Elf32_Ehdr ELFheader;
	Elf32_Shdr* STRheader,ITERheader;
	char *STR_buffer=NULL;
	
	int numcar = 0;
	int i = 0;

	STRheader=readSectionTable(FILE* ElfFile);


	if(STRheader.sh_size == 0)
	{
		printf("section vide"); //todo - cas ou num/nom de section est invalide
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
		{	//affichage de l'addresse relative des contenus de la section
			printf("\n0x%08x",affichage_addr);
			affichage_addr = affichage_addr + 16;
		}
		if(i%4 == 0)
		{	//espaces apres chaque 8 chiffres affiches
			printf(" ");
		}
		printf("%02x",STR_buffer2[i]);
	}
	printf("\n");

	free(STR_buffer2);
	rewind(ElfFile);
	return 0;
}
