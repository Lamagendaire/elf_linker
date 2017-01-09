#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "util.h"
#include <elf.h>


typedef struct 
{//Section complète (nom, header et contenu)
        char * nomSec;
        Elf32_Shdr  headerSec;
        char * contenuSec;
} Section;
typedef struct 
{
        char * nomSymb;
        Elf32_Sym  headerSymb; 
}Symbole;


typedef struct 
{//Fichier ELF (header, sections, symboles, reloactions)
        FILE * fichierElf;
        Elf32_Ehdr headerElf;
        Section * tableSections;
        Symbole*  tableSymb;
        Elf32_Rel* tableRelocation;

}ElfFile;



/*
typedef struct {
        ElfFile * ElfFile1;
        ElfFile * ElfFile2;
} LinkedFiles;
*/


int NameToIndex(char* nom_sect,FILE* ElfFile);
Elf32_Ehdr readHeader(FILE* ElfFile);
Section* readSectionTable(FILE* ElfFile);
Symbole* readSymboleTable(FILE* ElfFile);
Elf32_Rel* readRelocatableTable(FILE* ElfFile);
ElfFile ElfConstructor(FILE* ElfFile);

