#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include "util.h"
#include "elf.h"


int afficherSec(FILE* ElfFile);
FILE* ElfFile = NULL;


int main(int argc, char **argv) 
{ 
  if(argc != 2) {
    printf("usage: %s <ELF_FILE>\n", argv[0]);
    exit(1);
  }

  if((ElfFile = fopen(argv[1], "r")) == NULL) {
    perror("[E] Error opening file:");
    exit(1);
  }
	afficherSec(ElfFile);
} 
	
	
	
	int afficherSec(FILE* ElfFile){
	
	Elf32_Ehdr ELFheader;
	Elf32_Shdr STRheader,ITERheader;
	char *STR_buffer=NULL;
	
	//Lecture du header
	fseek( ElfFile, 0, SEEK_SET );
	fread( &ELFheader , sizeof(Elf32_Ehdr), 1, ElfFile);

	char nom_sect[20];
	int sel = 0;

	printf("nom ou numero de section a afficher:");
	scanf("%s", nom_sect);
	
	if (nom_sect[0] <= '9' && nom_sect[0] >= '0'){
		sel = (int)nom_sect[0]-48;
	}
	
	else {
		int iter_s;
		//find string section
		for ( iter_s=0; iter_s < ELFheader.e_shnum; iter_s++  )
		{
		fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*iter_s), SEEK_SET);
		fread( &STRheader, ELFheader.e_shentsize, 1, ElfFile );
		if ((STRheader.sh_type == SHT_STRTAB) && (STRheader.sh_addr == 0x00000000)){
		  STR_buffer = (char *)malloc( STRheader.sh_size);
		  if (STR_buffer == NULL)
		  {
			printf("Impossible d'allouer la mémoire pour les noms de section\n");
			return -1;
		  }
		  fseek( ElfFile, STRheader.sh_offset, SEEK_SET);
		  fread( STR_buffer, STRheader.sh_size, 1, ElfFile);
		  iter_s=ELFheader.e_shnum+1;
		}
		}
		
		//now iter and print segment names
		fseek(ElfFile, 0, SEEK_SET);
		for (iter_s=0; iter_s < ELFheader.e_shnum; iter_s++)
		{
		fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*iter_s), SEEK_SET);
		fread( &ITERheader, ELFheader.e_shentsize, 1, ElfFile );
		if (nom_sect == STR_buffer+ITERheader.sh_name){
			sel = iter_s;
			break;			
		}
		}
		free( STR_buffer );
		rewind(ElfFile);
		return 0;
	}
	//Lecture du Section Header selectionné
	fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*sel), SEEK_SET);
	fread( &STRheader, ELFheader.e_shentsize, 1, ElfFile );
	
	if(STRheader.sh_size == 0){
		printf("section vide");
	}
	else{
		fseek( ElfFile, STRheader.sh_offset, SEEK_SET);
		fread( &STR_buffer,STRheader.sh_size,1,ElfFile );
		
	}
	return 0;
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
