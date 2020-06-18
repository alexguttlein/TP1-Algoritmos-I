#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MIN_CADENA 4
#define MAX_CADENA 51
#define MAX_CATEGORIAS 4 //cantidad maxima de categorias
#define MAX_JUGADORES 5
#define MAX_PALABRAS 20 //cantidad maxima de palabras que permite tener el juego

typedef char Tstring [MAX_CADENA];
typedef Tstring Tjugadores[MAX_JUGADORES];
typedef Tstring Tcategorias[MAX_CATEGORIAS];
typedef Tstring Tpalabra[MAX_PALABRAS];
typedef int Tvector[MAX_JUGADORES];

//estructura de los datos a cargar
typedef struct{
	Tcategorias Vcategoria;
	Tpalabra Vpalabra;
	Tpalabra Vpista;
	int ML[1];  //ML[0] = max logico Categorias; ML[1] = max logico Palabras
}TdatosJuego;

//estructura de la partida
typedef struct{
	int cant_participantes;
	Tjugadores Vnombre;
	int cant_partidas;
	Tvector puntajes;
}Tpartidas;

//estructura principal
typedef struct{
	TdatosJuego datos_juego;
	Tpartidas partidas;
}Tjuego;

//Instrucciones que aparecen en pantalla al entrar al juego.
void saludo_inicial(){
	printf("\t\t\tBIENVENIDOS AL AHORCADO\n");
	printf("Un juego de vida o muerte para divertirte con tus amigos\n\n");
	printf("Para comenzar deberan ingresar las CATEGORIAS (siempre en MAYUSCULAS), las palabras y alguna pista.\n");
	printf("Tambien deberan indicar el numero y nombre de jugadores y la cantidad de partidas que desean jugar.\n");
	printf("\nSe permite hasta un maximo de %d jugadores\n",MAX_JUGADORES);
	printf("\nAl final de la ultima partida se indicara quien es el GANADOR.\n");
	printf("Se pueden ingresar hasta 6 letras que no estan en la palabra, en caso contrario sentiran el \nsuave roce");
	printf(" de la soga en sus fragiles cuellos hasta perder la vida.\n");
	printf("\nEstan listos para morir?\n\n");
	system("pause");
	printf("\n\n");
}

//Modifica las cadenas quitando el salto de linea del final.
void cambiar_fin_cadena(Tstring cadena){
	int i;
	for(i=0; i<MAX_CADENA;i++){
		if (cadena[i]=='\n')
			cadena[i]='\0';
	}
}

//Inicializa en cero a todos los contadores del juego.
void inicializar_contadores(Tjuego *vector){
	int i;
	for(i=0; i<MAX_JUGADORES; i++){
		vector->datos_juego.ML[i] = 0;
		vector->partidas.puntajes[i] = 0;
		vector->partidas.cant_participantes = 0;
		vector->partidas.cant_partidas = 0;
	}
}

//Se ingresan las categorias de las palabras a adivinar.
void ingresar_categoria(Tjuego *juego){
	int i;
	bool salir = false;
	char condicion_salida[3]="0\n";
	
	printf("Ingrese las categorias (hasta %d), finalice con 0: ",MAX_CATEGORIAS);
	
	for (i=juego->datos_juego.ML[0]; i<MAX_CATEGORIAS && salir==false; i++){
		printf("\nCategoria %d: ",i+1);
		fgets(juego->datos_juego.Vcategoria[i],MAX_CADENA,stdin); 
		fflush(stdin);
		//validar_cadena(); //Falta hacer funcion para validar segun la consigna del tp
	
		if (strcmp(juego->datos_juego.Vcategoria[i],condicion_salida) == 0){
			salir = true;
		}else{ 
			juego->datos_juego.ML[0] +=1;
			cambiar_fin_cadena(juego->datos_juego.Vcategoria[i]);
		}
	}
	if ((MAX_CATEGORIAS - juego->datos_juego.ML[0]) == 0)
		printf("\nSe alcanzo el maximo de categorias. Disculpe las molestias.\n\n");
}

//Se ingresan las palabras a adivinar.
void ingresar_palabra(Tjuego *juego){
	int i, j;
	bool salir = false;
	char condicion_salida[3]="0\n";
	
	if (juego->datos_juego.ML[0]>0){
		printf("\nIngresar palabras (hasta %d por categoria): \n",MAX_PALABRAS/MAX_CATEGORIAS);
		for(i=0; i<juego->datos_juego.ML[0]; i++){
			printf("Categoria %s: ",juego->datos_juego.Vcategoria[i]);
			
			for (j=juego->datos_juego.ML[1]; j<(MAX_PALABRAS / juego->datos_juego.ML[0]) && salir==false; j++){
				printf("Palabra %d: ",j+1);
				fgets(juego->datos_juego.Vpalabra[j],MAX_CADENA,stdin);
				//validar_cadena(); //Falta hacer funcion para validar segun la consigna del tp
				
				if(strcmp(juego->datos_juego.Vpalabra[j],condicion_salida) == 0){
					salir = true;
				}else{
					cambiar_fin_cadena(juego->datos_juego.Vpalabra[j]);
					fflush(stdin);
					printf("Ingrese una breve pista (hasta %d caracteres) para %s: ",MAX_CADENA-1,juego->datos_juego.Vpalabra[j]);
					fgets(juego->datos_juego.Vpista[j],MAX_CADENA,stdin);
					cambiar_fin_cadena(juego->datos_juego.Vpista[j]);
					fflush(stdin);
					juego->datos_juego.ML[1] +=1;
				}
			}
		}
	}else printf("Antes de ingresar una palabra se debe ingresar al menos una categoria.\n");
}

//Se selecciona si se ingresa una categoria o una palabra nueva.
void ingresar_datos(Tjuego *juego){
	int opcion;
	
	do{
		printf("\nSeleccione que desea hacer: \n");
		printf("<1> Ingresar categoria.\n<2> Ingresar palabra.\n<3> Volver al menu anterior.\n");
		scanf("%d",&opcion);
		fflush(stdin);
		switch (opcion){
			case 1:{
				ingresar_categoria(juego);
				break;
			}
			case 2:{
				ingresar_palabra(juego);
				break;
			}
		}
	}while(opcion>=1 && opcion<=2);
}


int main(){
	Tjuego juego;
	int opcion;
	
	saludo_inicial();
	inicializar_contadores(&juego);
	
	do{
		printf("\nSeleccionar opcion deseada:\n");
		printf("<1> Ingresar categorias y palabras.\n<2> Listar los datos ingresados.\n");
		printf("<3> Ingresar datos de los jugadores.\n<4> Comenzar a jugar.\n<5> Salir del juego.\n");
		scanf("%d",&opcion);
		fflush(stdin);
		
		switch(opcion){
			case 1:{
				ingresar_datos(&juego);
				break;
			}
			case 2:{
	//			listar_datos(juego);
				break;
			}
			case 3:{
	//			ingresar_participantes(&juego);
				break;
			}
			case 4:{
	//			realizar_partida(juego);
				break;
			}
		}
	}while(opcion>=1 && opcion<=4);

	printf("\nEsperamos que se hayan divertido.\n");
	return 0;
}
