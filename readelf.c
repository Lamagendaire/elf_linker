#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include "util.h"

#pragma pack(push,1)
#pragma pack(pop)

#define EI_NIDENT       16

/* 32-bit ELF base types. */
typedef unsigned int Elf32_Addr;
typedef unsigned short Elf32_Half;
typedef unsigned int Elf32_Off;
typedef signed int Elf32_Sword;
typedef unsigned int Elf32_Word;

typedef struct elf32_hdr{
  unsigned char e_ident[EI_NIDENT];
  Elf32_Half    e_type;
  Elf32_Half    e_machine;
  Elf32_Word    e_version;
  Elf32_Addr    e_entry;  /* Entry point */
  Elf32_Off e_phoff;
  Elf32_Off e_shoff;
  Elf32_Word    e_flags;
  Elf32_Half    e_ehsize;
  Elf32_Half    e_phentsize;
  Elf32_Half    e_phnum;
  Elf32_Half    e_shentsize;
  Elf32_Half    e_shnum;
  Elf32_Half    e_shstrndx;

} Elf32_Ehdr;

typedef struct elf32_shdr {
  Elf32_Word    sh_name;
  Elf32_Word    sh_type;
  Elf32_Word    sh_flags;
  Elf32_Addr    sh_addr;
  Elf32_Off sh_offset;
  Elf32_Word    sh_size;
  Elf32_Word    sh_link;
  Elf32_Word    sh_info;
  Elf32_Word    sh_addralign;
  Elf32_Word    sh_entsize;
} Elf32_Shdr;

void displaySection(char* identifiant);



int main(int argc, char **argv) 
{
  FILE* ElfFile = NULL;
  uint32_t idx;
  char* Header = NULL;
  char* SectNames = NULL;
  Elf32_Ehdr elf1;
  Elf32_Shdr elfSH;
  if(argc != 2) {
    printf("usage: %s <ELF_FILE>\n", argv[0]);
    exit(1);
  }

  if((ElfFile = fopen(argv[1], "r")) == NULL) {
    perror("[E] Error opening file:");
    exit(1);
  }

 
  Header = malloc(elf1.e_shentsize);
  if (1!= fread(&elf1, sizeof(elf1), 1, ElfFile)){
    printf("failed to read elf header");
    exit(1);
  }
   int i; 

    printf("\nMagic  :");
    for(i=0;i<15;++i) 
        printf("   %x",elf1.e_ident[i]);

    char *class; 

if(is_big_endian()){
    if(elf1.e_ident[4]!=1){
        reverse_4(elf1.e_version);
        reverse_4(elf1.e_flags);
        reverse_2(elf1.e_type);
        reverse_2(elf1.e_machine);
        reverse_2(elf1.e_ehsize);
        reverse_2(elf1.e_phentsize);
        reverse_2(elf1.e_phnum);
        reverse_2(elf1.e_shstrndx);
        reverse_2(elf1.e_entry);
        reverse_2(elf1.e_phoff);
        reverse_2(elf1.e_shoff);
      }
}
else{
  if(elf1.e_ident[4]!=1){
        reverse_4(elf1.e_version);
        reverse_4(elf1.e_flags);
        reverse_2(elf1.e_type);
        reverse_2(elf1.e_machine);
        reverse_2(elf1.e_ehsize);
        reverse_2(elf1.e_phentsize);
        reverse_2(elf1.e_phnum);
        reverse_2(elf1.e_shstrndx);
        reverse_2(elf1.e_entry);
        reverse_2(elf1.e_phoff);
        reverse_2(elf1.e_shoff);
      }
}
    if(elf1.e_ident[4]==2)
        class = "64";
    if(elf1.e_ident[4]==1)
        class = "32";
    if(elf1.e_ident[4]==0)
      class = "Inavalid Class";

    if(elf1.e_machine!=0)
      printf("\nPlatform:%"PRIu32,elf1.e_machine);

    printf("\nTaille mot : %s\n",class);
    printf("\nType :");
    if(elf1.e_type == 1)
        printf("Relocatable\n");
    else if(elf1.e_type == 2)
        printf("Executable\n");
    else if(elf1.e_type == 3)
        printf("Shared Object\n");
    else
        printf("Unknown\n");  

    printf("\nStart of section headers : %"PRIu32,elf1.e_shoff); 
    printf("\nSize of section table: %"PRIu32,elf1.e_shentsize);
    printf("\nNumber of entry of the section table : %"PRIu32,elf1.e_shnum);
    printf("\nEntry index : %"PRIu32,elf1.e_shstrndx);
    printf("\nHeader size : %"PRIu32,elf1.e_ehsize);
    printf("\n");
    
   SectNames = malloc(elfSH.sh_size);
   //fseek(ElfFile, elfSH.sh_offset, SEEK_SET);
   fread(SectNames, 1, elfSH.sh_size, ElfFile);

	// read all section headers
  for (idx = 0; idx < elf1.e_shnum; idx++)
  {
    const char* name = "";
    
	//fseek(ElfFile, elf1.e_shoff + elf1.e_shstrndx * sizeof(elfSH), SEEK_SET);
	fread(&elfSH, 1, sizeof(elfSH), ElfFile);
  
 	printf("%s",elfSH.sh_name);
	//displaySection(name);
	
    // print section name
    /*if (elfSH.sh_name);
      name = SectNames + elfSH.sh_name;
    printf("%2u %s\n", idx, name);*/
  }
  

} 

void displaySection(char* identifiant)
{
	printf("fonction displaySection\n");
}


