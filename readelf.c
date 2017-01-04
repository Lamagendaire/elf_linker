#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include "util.h"
#include "elf.h"

#pragma pack(push,1)
#pragma pack(pop)



int displayNameSection(FILE* ElfFile);

FILE* ElfFile = NULL;

int displayNameSection(FILE* ElfFile)
{

  Elf32_Ehdr ELFheader;
  Elf32_Shdr STRheader,ITERheader;
  char *STR_buffer=NULL;
  int iter_s; 

  //read header file
  fseek( ElfFile, 0, SEEK_SET );
  fread( &ELFheader , sizeof(Elf32_Ehdr), 1, ElfFile);

  //find string section
  for ( iter_s=0; iter_s < ELFheader.e_shnum; iter_s++  )
  {
    fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*iter_s), SEEK_SET);
    fread( &STRheader, ELFheader.e_shentsize, 1, ElfFile );
    if ((STRheader.sh_type == SHT_STRTAB) && 
      (STRheader.sh_addr == 0x00000000))
    {
      STR_buffer = (char *)malloc( STRheader.sh_size);
      if (STR_buffer == NULL) 
      {
        printf("Cannot allocate memory for section string names\n");
        return 0;
      }
      fseek( ElfFile, STRheader.sh_offset, SEEK_SET);
      fread( STR_buffer, STRheader.sh_size, 1, ElfFile);
      iter_s=ELFheader.e_shnum+1;
    }
  }

  //now iter and print segment names
  fseek(ElfFile, 0, SEEK_SET);
  for ( iter_s=0; iter_s < ELFheader.e_shnum; iter_s++ )
  {
    fseek( ElfFile, ELFheader.e_shoff+(ELFheader.e_shentsize*iter_s), SEEK_SET);
    fread( &ITERheader, ELFheader.e_shentsize, 1, ElfFile );
    if (1==1)
    {
      printf("%s\n", STR_buffer+ITERheader.sh_name);
    }
  }

  free( STR_buffer );
  fclose( ElfFile );

  return 0;
}


int main(int argc, char **argv) 
{
  //uint32_t idx;
  //char* Header = NULL;
  //char* SectNames = NULL;
  Elf32_Ehdr elf1;
  //Elf32_Shdr elfSH;
  if(argc != 2) {
    printf("usage: %s <ELF_FILE>\n", argv[0]);
    exit(1);
  }

  if((ElfFile = fopen(argv[1], "r")) == NULL) {
    perror("[E] Error opening file:");
    exit(1);
  }

 
  //Header = malloc(elf1.e_shentsize);
  if (1!= fread(&elf1, sizeof(elf1), 1, ElfFile)){
    printf("failed to read elf header");
    exit(1);
  }
  int i;
  printf("\n===INFOS HEADER===");
  printf("\nMagic  :");
   for(i=0;i<15;++i) 
	printf("   %x",elf1.e_ident[i]);

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
	printf("\n\nTABLE DE SECTION");
    printf("\n\t Offset : %"PRIu32,elf1.e_shoff); 
    printf("\n\t Taille : %"PRIu32,elf1.e_shentsize);
    printf(" (octets)\n\t Nombre d'entrée(s) : %"PRIu32,elf1.e_shnum);
    printf("\n\nIndex de l'entrée : %i"PRIu32,elf1.e_shstrndx);
    printf("\nTaille totale de l'header : %"PRIu32,elf1.e_ehsize);
    printf(" (octets)\n");
    rewind(ElfFile);
    
    printf("\n===NOM DES SECTIONS===\n");
	displayNameSection(ElfFile);
} 


