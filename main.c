#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "util.h"
#include "elf.h"
// #include "display.h"

int main(int argc, char **argv) 
{
	FILE* ElfFile = NULL;
	FILE* ElfFile2 = NULL;	

  if(argc != 3 && argc != 2  ) {
    printf("usage: %s <ELF_FILE>\n", argv[0]);
    exit(1);
  }

  if((ElfFile = fopen(argv[1], "r")) == NULL) {
    perror("[E] Error opening file:");
    exit(1);
  }

  if((ElfFile2 = fopen(argv[2], "r")) == NULL) {
  		if(strcmp(argv[2],"-display")==0)
  		{  			
  				displayMenu(ElfFile);
  				//printf("Afficher");
  		}
  	}

  else{
  	printf("Fusion des 2 fichiers");
  }

// TODO: A Mettre dans display.c

 //  int choix=100;

	// while(choix != 0){
	// 	printf("====Menu===\n");
	// 	printf("Choisir:\n 0- Sortir\n 1- Afficher les informations du Header\n 2- Afficher la table des sections\n 3- Afficher une section\n 4- Afficher la table des symboles\n 5- Afficher la table de relocation\n");

	// 	scanf("%i", &choix);
	// 	switch(choix)
	// 	{
 //            case 0:
 //                break;
	// 		case 1:
	// 			displayHeaderInfos(ElfFile);
	// 			break;
	// 		case 2:
	// 			displayNameSection(ElfFile);
	// 			break;
	// 		case 3:
	// 			afficherSec(ElfFile);
	// 			break;
	// 		case 4:
	// 			displaySymbolTable(ElfFile);
	// 			break;
	// 		case 5:
	// 			displayRelocatableTable(ElfFile);
	// 			break;
	// 		default:
	// 			printf("Le choix ne correspond pas\n");
	// 			break;
	// 		}
	// }
	
} 