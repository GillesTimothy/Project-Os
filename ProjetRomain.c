#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/*
typedef struct {
        int numVoiture;         //numero de la voiture
        int tempsSecteur1;      //temps du secteur 1
        int tempsSecteur2;      //temps du secteur 2
        int tempsSecteur3;      //temps du secteur 3
        int tempsTour;         	//temps du tour
        int tempsSecteur1Best;  //meilleur temps au secteur 1
        int tempsSecteur2Best;  //meilleur temps au secteur 2
        int tempsSecteur3Best;  //meilleur temps au secteur 3
        int tempsTourBest;      //meilleur temps au tour
        int numTour;            //numéro du tour actuel
        int Arret;            //nombre de pit stop réalisé par la voiture
        bool end;              	//1 si la voiture a finis la course
        bool out;               //1 si il est elimine
} voiture;
*/

//Variables globales temporaires en attendant d'utiliser la structure ci-dessus
double voiture [20][9]; 						
int tempsSecteur1 = 0;
int tempsSecteur2 = 0;
int tempsSecteur3 = 0;
double tempsTour = 0;
double tempsSecteur1Format = 0;
double tempsSecteur2Format = 0;
double tempsSecteur3Format = 0;
int tempsTourFormat = 0;

int randomizer(int max,int min){
  	int nbrRandom = 0;
  	srand(time(NULL));
  	nbrRandom = (rand() % (max - min + 1)) + min;
  	usleep(1000000); 		//100000 = rapide mais random faussé | 1000000 = lent mais random correct
  	return nbrRandom;
}

int formatage(double tour){
	int reste = (int)tour;
	int process = reste % 60;
	if (process >= 45 && process < 60){
		printf("Temps du tour : 1:%.3lf\n",tour-60);
	}
	else if (process >= 0 && process <= 15){
		if (process < 10){
			printf("Temps du tour : 2:0%.3lf\n",tour-120);
		}
		else{
			printf("Temps du tour : 2:%.3lf\n",tour-120);
		}
	}
}

void tour(){
	tempsSecteur1 = (randomizer(45000,35000));
	tempsSecteur1Format = ((double)tempsSecteur1)/1000;
	//printf("Secteur 1 : %.3lf\n",tempsSecteur1Format);
	tempsSecteur2 = (randomizer(45000,35000));
	tempsSecteur2Format = ((double)tempsSecteur2)/1000;
	//printf("Secteur 2 : %.3lf\n",tempsSecteur2Format);
	tempsSecteur3 = (randomizer(45000,35000));
	tempsSecteur3Format = ((double)tempsSecteur3)/1000;
	//printf("Secteur 3 : %.3lf\n",tempsSecteur3Format);
	tempsTour = ((double)(tempsSecteur1+tempsSecteur2+tempsSecteur3))/1000;
	tempsTourFormat = formatage(tempsTour);
}

double createTable(){
	for(int i = 0; i < 20; i++){
		voiture[i][0] = i+1;
		voiture[i][5] = 0;
		voiture[i][6] = 0;
		voiture[i][7] = 0;
		voiture[i][8] = 0;
	}
	return 0;
}

void run(){
	for(int i = 0; i < 21; i++){
		tour();
		voiture[i][1] = tempsSecteur1Format;
		voiture[i][2] = tempsSecteur2Format;
		voiture[i][3] = tempsSecteur3Format;
		voiture[i][4] = tempsTour;
		if (voiture[i][5] == 0 || voiture[i][5] > voiture[i][1]){
			voiture[i][5] = voiture[i][1];
		}
		if (voiture[i][6] == 0 || voiture[i][6] > voiture[i][2]){
			voiture[i][6] = voiture[i][2];
		}
		if (voiture[i][7] == 0 || voiture[i][7] > voiture[i][3]){
			voiture[i][7] = voiture[i][3];
		}
		if (voiture[i][8] == 0 || voiture[i][8] > voiture[i][4]){
			voiture[i][8] = voiture[i][4];
		}
	}
}

void tableauEssai(){
	printf("╔═════════════╦════════╦════════╦════════╦═════════╦════════╦════════╦════════╦═════════╗\n║   Voiture   ║   S1   ║   S2   ║   S3   ║  Total  ║ BestS1 ║ BestS2 ║ BestS3 ║BestTotal║\n╠═════════════╬════════╬════════╬════════╬═════════╬════════╬════════╬════════╬═════════╣\n");
	for(int i = 0; i<9;i++){													
  		printf("║ \033[01;36mVoiture #0%d\033[0m ║ ", i+1);
 		for(int j =1; j<9;j++){
     		printf("%.3lf ║%c", voiture[i][j], j==8? '\n' : ' ');
  		}
	}
	for(int i = 10; i<21;i++){													
  		printf("║ \033[01;36mVoiture #%d\033[0m ║ ", i);
 		for(int j =1; j<9;j++){
     		printf("%.3lf ║%c", voiture[i][j], j==8? '\n' : ' ');
  		}
	}
	printf("╚═════════════╩════════╩════════╩════════╩═════════╩════════╩════════╩════════╩═════════╝\n");
}

int creationFichier(){
	FILE* fichier = NULL;
	fichier = fopen("Résultat.txt", "w+");
	if (fichier != NULL){
    // On l'écrit dans le fichier
 	fprintf(fichier,"╔═════════════╦════════╦════════╦════════╦═════════╦════════╦════════╦════════╦═════════╗\r\n║   Voiture   ║   S1   ║   S2   ║   S3   ║  Total  ║ BestS1 ║ BestS2 ║ BestS3 ║BestTotal║\r\n╠═════════════╬════════╬════════╬════════╬═════════╬════════╬════════╬════════╬═════════╣\r\n");
	fprintf(fichier,"║ Voiture #0%d ║ ",1);
	for(int j = 1; j<9;j++){
		fprintf(fichier,"%.3lf ║%c", voiture[0][j], j==8? '\r\n' : ' ');
	}
	for(int i = 1; i<9;i++){													
  		fprintf(fichier,"\r\n║ Voiture #0%d ║ ", i+1);
 		for(int j =1; j<9;j++){
     		fprintf(fichier,"%.3lf ║%c", voiture[i][j], j==8? '\r\n' : ' ');
  		}
	}
	for(int i = 10; i<21;i++){													
  		fprintf(fichier,"\r\n║ Voiture #%d ║ ", i);
 		for(int j =1; j<9;j++){
     		fprintf(fichier,"%.3lf ║%c", voiture[i][j], j==8? '\r\n' : ' ');
  		}
	}
	fprintf(fichier,"╚═════════════╩════════╩════════╩════════╩═════════╩════════╩════════╩════════╩═════════╝\r\n");
	fclose(fichier);
 	}
	return 0;
}

int main(){
	createTable();
	run();
	tableauEssai();
	creationFichier();
}