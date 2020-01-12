/**
 * Simple program for loading patient data and saving it to a .txt file
 * Ondrej Sedlacek, 8.10.19 
 */
#include <stdio.h>

void ctiSouslovi(int limit, char *souslovi, char *b);

/**
 * Reads patient data from user, saves in file PACRRRRRRRRRR.txt
 * RR stands for rodne cislo
 */
int main(){
	const int limit = 100;
	char jmeno[limit], bydliste[limit], rodne[limit], b[limit];

	printf("zadejte jmeno pacienta (ukoncene teckou): ");
	ctiSouslovi(limit, jmeno, b);

	printf("zadejte rodne cislo: ");
	for(int i = 0; i < limit; rodne[i++] = 0){} // clear array 
	
	scanf("%10s", rodne);
	
	int c;
	while ((c = getchar())!= '\n' && c != EOF){ }// clear stdin buffer

	printf("zadejte bydliste (ukoncene teckou): ");
	ctiSouslovi(limit, bydliste, b);
	
	char filename[18];
	sprintf(filename, "PAC%10s.txt", rodne);
	
	FILE* soubor = fopen(filename, "w");
	if(soubor != NULL){
		fprintf(soubor, "Jmeno: %s\n", jmeno);
		fprintf(soubor, "Rodne cislo: %s\n", rodne);
		fprintf(soubor, "Bydliste: %s\n", bydliste);
	}
	fclose(soubor);
	return 0;
}

/**
 * Reads strigs separated by whitespace from console
 * stops when '.' is present or a limit of characters is reached
 @param limit Max number of characters +1
 @param souslovi Function result, entered strings separated by ' '
 @param b Used as buffer
*/
void ctiSouslovi(int limit, char *souslovi, char *b){
	for(int i = 0; i < limit; souslovi[i] = b[i] = 0, i++){} // clear arrays

	int i_b, i_souslovi;
	i_b = i_souslovi = 0;

	while(i_souslovi < limit){
	scanf("%s", b);
	i_b = 0;
	while(b[i_b] != '\0' && b[i_b] != '.' && i_souslovi < limit){
		souslovi[i_souslovi] = b[i_b];
		i_souslovi++;
		i_b++;
	}

	if(b[i_b] == '.'){
		souslovi[i_souslovi] = '\0';
		break;
	}

	souslovi[(i_souslovi < limit)? i_souslovi : limit - 1] = ' ';
	i_souslovi++;
	}
}
