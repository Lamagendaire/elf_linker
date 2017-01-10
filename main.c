#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "util.h"
#include "elf.h"
#include "display.h"
#include "readelf.h"
#include "fusionelf.h"



int main(int argc, char **argv) 
{	
	ElfFile elffile;
	ElfFile elffile2;
	ElfFile elffusion;


  if(argc != 3 && argc != 2  ) {
    printf("usage: %s <ELF_FILE>\n", argv[0]);
    exit(1);
  }

//si argument 1 est pas un fichier-> retourne erreur
  if((elffile.fichierElf = fopen(argv[1], "r")) == NULL) {
    perror("[E] Error opening file ");
    exit(1);
  }

//si argument 2 est pas un fichier -> vérifier si option -display
	if((elffile2.fichierElf = fopen(argv[2], "r")) == NULL) {
  		if(strcmp(argv[2],"-display")==0)
  		{
  				displayMenu(elffile.fichierElf);
  				//printf("afficher");
  		}
	}

//sinon (c'est que c'est un fichier) -> fusion des deux fichiers
  else{
	elffile = ElfConstructor(elffile.fichierElf);
	elffile2.fichierElf = fopen(argv[2], "r");
	elffile2 = ElfConstructor(fopen(argv[2],"r"));

  	printf("Fusion des 2 fichiers");
  	elffusion=fusionProgBit(elffile,elffile2);
  }
}
