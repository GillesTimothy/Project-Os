#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <semaphore.h>
#include <sys/sem.h>

double min(double a,double b){
		if(a > b){
			return b;
		}
		if(a < b){
			return a;
		}
		return a;
}

int randomizer(int min,int max){
	int nombre_aleatoire = 0;
	srand(time(NULL) ^ (getpid() << 20));// ^ = XOR, << = decalage bit a bit
	nombre_aleatoire = rand()%(max-min)+min;
	return nombre_aleatoire;
}
double secteur(){
	usleep(12000);
	int min = 40000;
	int max = 50000;
	return ((double)randomizer(min,max)/(double)1000);
	
}
struct voiture{
	int ID;
	char nom;
	double S1;
	double S2;
	double S3;
	double tour;
	double meilleurS1;
	double meilleurS2;
	double meilleurS3;
	double meilleurTour;
	int numeroTour;
	int estCrash;
	int estPit;
	double tempsEcoule;
}voiture[20];
 
int pit(){
	if(randomizer(0,18) == 5){
		return 1;		
	}
	else{
		return 0;
	}
}
	
int crash(){
	if(randomizer(0,600) == 69){
		return 1;		
	}
	else{
		return 0;
	}
}
		

void choixCourse(int nombreVoiture,int tri,double tempsCourse,struct voiture bagnole[20],int typeCourse){
		int i ;
		int shmId;
		int compteur;
		int semId;
		struct sembuf operation;
		int nbLecteur;
		key_t key = ftok("/dev/null", 2502);
		struct voiture *circuit;
		shmId = shmget(key, 20*sizeof(struct voiture), IPC_CREAT|0666);
		if(shmId == -1){
			printf("shmId = -1 ");
			exit(1);
		}
		circuit = shmat(shmId,0,0);
		if(circuit == (struct voiture*)-1){
			printf("shmat = -1");
			exit(1);
		}
		 
		void initSem(){
			key_t semCle;
			semId = semget(semCle, 2, IPC_CREAT | 0666);
			if( semCle < 0 )
			{
				printf("Erreur semid\n");
				exit(0);
			}
			semctl(semId, 0, SETVAL, 1);
			semctl(semId, 1, SETVAL, 1);
		}

		void attendre(int i){
			operation.sem_num = i;
			operation.sem_op = -1;
			operation.sem_flg = SEM_UNDO;
			semop(semId, &operation, 1);
		}

		void signale(int i){
			operation.sem_num = i;
			operation.sem_op = 1;
			operation.sem_flg = SEM_UNDO;
			semop(semId, &operation, 1);
		}

		void commencerLecture(){
			attendre(0);
			nbLecteur++;
			if (nbLecteur == 1) {
				attendre(1);
			}
			signale(0);
		}

		void arreterLecture(){
			attendre(0);
			nbLecteur--;
			if (nbLecteur == 0) {
				signale(1);
			}
			signale(0);
		}
		initSem();
		void trier(int param ){
			struct voiture tempo; //[1]
			int a=0;
			int b=0;
			commencerLecture();
			memcpy(bagnole,circuit,nombreVoiture*sizeof(struct voiture));	
			arreterLecture();	
			if(param <= 0 ){ //trier voiture en fonction de leur meilleur tour
				for( a = 0; a < nombreVoiture ; a++){
					for( b = 0; b < nombreVoiture - 1 ; b++){
						if(bagnole[b].meilleurTour > bagnole[b+1].meilleurTour){
							tempo = bagnole[b+1];
							bagnole[b+1] = bagnole[b];
							bagnole[b] = tempo;
					}		
				}
			}
			}else{//trier en fonction du numéro de tour
				for( a = 0; a < nombreVoiture ; a++){
					for( b = 0; b < nombreVoiture - 1; b++){
						if(bagnole[b].numeroTour < bagnole[b+1].numeroTour){
							tempo = bagnole[b+1];
							bagnole[b+1] = bagnole[b];
							bagnole[b] = tempo;
						}else if(bagnole[b].numeroTour == bagnole[b+1].numeroTour){
							if(bagnole[b].tour > bagnole[b+1].tour){
								tempo = bagnole[b+1];
								bagnole[b+1] = bagnole[b];
								bagnole[b] = tempo;
							}
						}				
					}
				}
			}
		
		}
		void affichage(int trieur){
			trier(trieur);
			double meilleurS1 = 9999999.0;
			double meilleurS2 = 9999999.0;
			double meilleurS3 = 9999999.0;
			double meilleurTour = 99999.0;
			int j;
			int idS1;
			int idS2;
			int idS3;
			int idTour;
			system("clear");
			
			switch(typeCourse)
			{
			case 1: printf("Séance d'essais libres du vendredi matin\t|P1\n\n");
				break;
			case 2 :printf("Séance d'essais libres du vendredi après-midi\t|P2\n\n");
				break;
			case 3 :printf("Séance d'essais libres du samedi matin\t|P3\n\n");
				break;
			case 4 :printf("Séance de qualifications du samedi après-midi\t|Q1\n\n");
				break;	
			case 5 :printf("Séance de qualifications du samedi après-midi\t|Q2\n\n");
				break;	
			case 6 :printf("Séance de qualifications du samedi après-midi\t|Q3\n\n");
				break;			
			case 7 :printf("Course du dimanche après-midi\n\n");
				break;			
			}
			printf("ID \t|");
			printf("S1 \t|");
			printf("S2: \t|");
			printf("S3: \t|");
			printf("tour: \t\t|");
			printf("meilleur S1: \t|");
			printf("meilleur S2: \t|");
			printf("meilleur S3: \t|");
			printf("meilleur tour: ");
			printf("|numéro tour: \t");
			printf("|PIT: ");
			printf("|OUT: \n");
		
			for(j = 0; j < nombreVoiture ; j++){
				printf("%d\t|",bagnole[j].ID);
				printf("%.3f\t|",bagnole[j].S1);
				printf("%.3f\t|",bagnole[j].S2);
				printf("%.3f\t|",bagnole[j].S3);
				printf("%.3f   \t|",bagnole[j].tour);
				printf("%.3f\t\t|",bagnole[j].meilleurS1);
				printf("%.3f\t\t|",bagnole[j].meilleurS2);
				printf("%.3f\t\t|",bagnole[j].meilleurS3);
				printf("%.3f   \t|",bagnole[j].meilleurTour);
				printf("%i\t\t",bagnole[j].numeroTour);
				if(bagnole[j].estPit != 0){
					printf("  %d    ",bagnole[j].estPit);
			
				}else{
					printf("  0    ");
				}
				if(bagnole[j].estCrash == 1){
					printf(" X\n");
			
				}else{
					printf(" 0\n");
				}			
			}
			for(j = 0; j<  nombreVoiture ; j++){
				
				if(meilleurS1 > bagnole[j].meilleurS1){
					idS1 = bagnole[j].ID;
					meilleurS1 = bagnole[j].meilleurS1;
				}
				if(meilleurS2 > bagnole[j].meilleurS2){
					idS2= bagnole[j].ID;
					meilleurS2 = bagnole[j].meilleurS2;
				}
				if(meilleurS3 > bagnole[j].meilleurS3){
					idS3 = bagnole[j].ID;
					meilleurS3 = bagnole[j].meilleurS3;
				}
				if(meilleurTour > bagnole[j].meilleurTour){
					idTour = bagnole[j].ID;
					meilleurTour = bagnole[j].meilleurTour;
				}
			}
			printf("\nmeilleurS1:\t%.3f  | id:%d \n",meilleurS1,idS1);
			printf("meilleurS2:\t%.3f  | id:%d \n",meilleurS2,idS2);
			printf("meilleurS3:\t%.3f  | id:%d \n",meilleurS3,idS3);
			printf("meilleurTour:\t%.3f | id:%d \n",meilleurTour,idTour);
		}
		for(i=0;i < nombreVoiture;i++){
		
			if(fork() == 0){
			
				circuit = shmat(shmId,0,0);
				if(circuit == (struct voiture*)-1){
					printf("shmat = -1");
					exit(1);
				}
				
				circuit[i].tempsEcoule = 0;
				circuit[i].ID =	bagnole[i].ID;
				circuit[i].meilleurS1 = 99;
				circuit[i].meilleurS2 = 99;
				circuit[i].meilleurS3 = 99;
				circuit[i].meilleurTour = 999;
				circuit[i].numeroTour = 0;
				circuit[i].estCrash = 0;
				circuit[i].estPit = 0;
				
				while(circuit[i].estCrash == 0 && circuit[i].tempsEcoule < tempsCourse){
				
					circuit[i].tour = 0;
					
					if(crash() == 1){
						
						circuit[i].estCrash = 1;
						circuit[i].S1 = 0;
						circuit[i].S2 = 0;
						circuit[i].S3 = 0;
						
						exit(0);
					}else{
						attendre(1);
						circuit[i].S1 = secteur();
						circuit[i].tempsEcoule += circuit[i].S1;
						circuit[i].tour += circuit[i].S1; 
						circuit[i].meilleurS1 = min(circuit[i].meilleurS1,circuit[i].S1);
						signale(1);
							if(crash() == 1){
								
								circuit[i].estCrash = 1;
								circuit[i].S2 = 0;
								circuit[i].S3 = 0;
								
								exit(0);
							}else{
								attendre(1);
								circuit[i].S2 = secteur();
								circuit[i].tempsEcoule += circuit[i].S2;
								circuit[i].tour += circuit[i].S2;
								circuit[i].meilleurS2 = min(circuit[i].meilleurS2,circuit[i].S2); 
								signale(1);
							
								if(pit() == 1){
									
									circuit[i].estPit += 1;
									
									usleep(600000);
									attendre(1);
									circuit[i].S3 = ((double)randomizer(20000,25000)/(double)1000) + secteur();
									signale(1);
								}else{
									attendre(1);
									circuit[i].S3 = secteur();
									signale(1);					
								}
								if(crash() == 1){
									
									circuit[i].estCrash = 1;
									circuit[i].S3 = 0;
									
									exit(0);
								}else{
									attendre(1);
									circuit[i].tour += circuit[i].S3; 
									circuit[i].tempsEcoule += circuit[i].S3;
									circuit[i].meilleurS3 = min(circuit[i].meilleurS3,circuit[i].S3);
									circuit[i].meilleurTour = min(circuit[i].meilleurTour,circuit[i].tour);
									circuit[i].numeroTour++;
									signale(1);
									usleep(300000);
								}
							}
						}
				}
				exit(0);
			}
		}
		for(compteur = 0; compteur< ((int)tempsCourse/120*3); compteur++){
			affichage(tri);
			usleep(400000);
		}
		if(shmdt(circuit) == -1) {
           		 printf("shmdt");
          		  exit(1);
     	}	


}
void ligneDepart(struct voiture automobile[20]){
	system("clear");
	printf("Voici la ligne de départ\n");
	for(int i =0; i<20;i++){
		printf("%d: \t %d\n",(i+1),automobile[i].ID);
	}
}

int main (void){
	int ID[20] = {44,77,5,7,3,33,11,31,18,35,27,55,10,28,8,20,2,14,9,16};
	int tempsUnTour = 130;
	int nombreTourCourseFinale = 45;
	struct voiture tri[20];
	for(int i = 0; i < 20 ; i++){
		tri[i].ID = ID[i];
	}		
	choixCourse(20,-1,5400,tri,1); //Essais libres vdd matin P1
	sleep(5);
	choixCourse(20,-1,5400,tri,2); //Essais libres vdd soir P2
	sleep(5);
	choixCourse(20,-1,3600,tri,3); //Essais libres samedi matin
	sleep(5);
	choixCourse(20,-1,1080,tri,4); //Qualif samedi aprem Q1
	sleep(5);
	choixCourse(15,-1,900,tri,5);  //Qualif samedi aprem Q2
	sleep(5);
	choixCourse(10,-1,720,tri,6);  //Qualif samedi aprem Q3
	sleep(5);
	ligneDepart(tri);
	sleep(10);
	choixCourse(20,1,(tempsUnTour*nombreTourCourseFinale),tri,7);  //Course finale du dimanche
	sleep(5);
}
