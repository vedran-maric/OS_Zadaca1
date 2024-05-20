#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

/*Zadaca1 - MALO TI FALI DA RIJESIS!! SAMO TREBAS NEKAKO VARIJABLU ucitano OMOGUCITI DA BUDE PROCITANA UNUTAR signal_handler BEZ DA BUDE GLOBALNA VARIJABLA 
- bolje nemoj, ne radi se klasicno prosljedjivanje vec posebne kentre*/
int ucitano;

/*Handler za SIGNALE*/
static void signal_handler(int signum){
FILE *status;

switch(signum){

	case SIGUSR1:
    		/* Ispiši trenutni broj */
    		printf("(preko handlera)Trenutni broj u obradi: %d\n", ucitano);
    		break;

	case SIGTERM:
    		/* Ispiši, spremi trenutni broj i ugasi program*/
	   	printf("Zapisujem broj u status.txt: %d\n", ucitano);
    		status = fopen("status.txt","w");
		fprintf(status, "%d\n", ucitano);
		fclose(status);
		exit(0);
		break;

	case SIGINT:
		/*Ugasi program*/
		printf("Gasim program!\n");
		exit(0);
		break;

	default:
		printf("Primljen nepoznat signal!");
		break;
	}


}


int main(){
/*regularne varijable*/
FILE *obrada, *status, *obrada_pretraga;


/* <SIGNALI> */
/*Varijable za SIGNALE*/
struct sigaction sa;

sa.sa_handler = signal_handler;
sigemptyset(&sa.sa_mask);
sa.sa_flags = SA_RESTART; /*Govori da nastavi s radom kada zavrsi dio s prekidom*/

if (sigaction(SIGUSR1, &sa, NULL)==-1)
{
	printf("Neka neplanirana greska je iskocila\n");
	exit(1);
}
if (sigaction(SIGTERM, &sa, NULL)==-1)
{
	printf("Neka neplanirana greska je iskocila\n");
	exit(1);
}
if (sigaction(SIGINT, &sa, NULL)==-1)
{
	printf("Neka neplanirana greska je iskocila\n");
	exit(1);
}
/* </SIGNALI> */


/*otvaranje datoteke*/
status = fopen("status.txt","r");
/*Ucitavanje broja iz status.txt*/
fscanf(status, "%d", &ucitano);
fclose(status);

/*Ako je status.txt nula, onda citaj iz obrade.txt*/
if (ucitano == 0)
{
	obrada_pretraga = fopen("obrada.txt","r");
	int pretraga;
	fscanf(obrada_pretraga, "%d", &pretraga);
	printf("Ucitao sam %d\n",pretraga);
	/*Pretrazuj zadnji broj u obrada.txt*/
	while(!feof(obrada_pretraga)){
		fscanf(obrada_pretraga, "%d", &pretraga);
	}
	ucitano = round(sqrt(pretraga));
	fclose(obrada_pretraga);
}

/*Upisi broj 0 u status.txt(obrada u tijeku)*/
status = fopen("status.txt","w");
fprintf(status, "%d\n", 0);
fclose(status);

/*upisivanje kvadrata broja u obrada.txt*/
obrada = fopen("obrada.txt", "a");
int i = 0; /*Privremeno*/
while (ucitano > 0 && i < 30)
{	
	/*Upisi u obrada.txt kvadrat*/
	fprintf(obrada, "%d\n", ucitano*ucitano);
	usleep(500000);

	i++; /*privremeno*/
	ucitano++; /*obradjuje sljedeci broj*/
}
fclose(obrada);

/*Upisi broj do kojeg si stao u status.txt*/
status = fopen("status.txt","w");
fprintf(status, "%d\n", (ucitano));
fclose(status);

return 0;
}
