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
typedef int TvectorContador[MAX_CATEGORIAS];

//estructura de los datos a cargar
typedef struct{
	Tcategorias Vcategoria;
	Tpalabra Vpalabra;
	Tpalabra Vpista;
	int ML[2];  //ML[0] = max logico Categorias; ML[1] = max logico Palabras;
	int contador_categorias;
	TvectorContador contador_palabras;
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
		vector->datos_juego.contador_categorias = 0;
		vector->datos_juego.contador_palabras[i] = 0;
	}
}

void ingresar_cadena(Tstring cadena, int tamano){
	bool validar = true;
	
	do{
		fgets(cadena,tamano,stdin); 
		fflush(stdin);
		//validar = validar_cadena(cadena); FALTA FUNCION VALIDAR CADENA
	}while(validar == false);
	
	cambiar_fin_cadena(cadena);
}

//Se ingresan las categorias de las palabras a adivinar.
void ingresar_categoria(Tjuego *juego){
	int i;
	bool salir = false;
	char condicion_salida[3]="0\0";
	printf("Ingrese las categorias (hasta %d), finalice con 0: ",MAX_CATEGORIAS);
	
	for (i=juego->datos_juego.ML[0]; i<MAX_CATEGORIAS && salir==false; i++){
		printf("\nCategoria %d: ",i+1);
		ingresar_cadena(juego->datos_juego.Vcategoria[i],MAX_CADENA);

		if (strcmp(juego->datos_juego.Vcategoria[i],condicion_salida) == 0){
			salir = true;
		}else{ 
			juego->datos_juego.ML[0] +=1;
		}
	}
	if ((MAX_CATEGORIAS - juego->datos_juego.ML[0]) == 0)
		printf("\nSe alcanzo el maximo de categorias. Disculpe las molestias.\n\n");
}

//Se ingresan las palabras a adivinar.
void ingresar_palabra(Tjuego *juego){
	int i, j;
	bool salir = false;
	char condicion_salida[3]="0\0";
	int ML_categoria = juego->datos_juego.ML[0];
	int ML_cadena = juego->datos_juego.ML[1];
	
	for(i=juego->datos_juego.contador_categorias; i<juego->datos_juego.ML[0]; i++){
		printf("\nIngresar palabras (hasta %d por categoria):\n",MAX_PALABRAS/MAX_CATEGORIAS);
		for (j=juego->datos_juego.contador_palabras[i]; j<MAX_PALABRAS/MAX_CATEGORIAS && salir==false; j++){
			printf("Categoria %s. Palabra %d: ",juego->datos_juego.Vcategoria[i],j+1);
			ingresar_cadena(juego->datos_juego.Vpalabra[j],MAX_CADENA);
			if(strcmp(juego->datos_juego.Vpalabra[j],condicion_salida) == 0){
				salir = true;
			}else{
				printf("Ingrese una breve pista (hasta %d caracteres) para %s: ",MAX_CADENA-1,juego->datos_juego.Vpalabra[j]);
				ingresar_cadena(juego->datos_juego.Vpista[j],MAX_CADENA);
				juego->datos_juego.ML[1] +=1;
				juego->datos_juego.contador_palabras[i] += 1;
				if(juego->datos_juego.contador_palabras[i] == MAX_PALABRAS/MAX_CATEGORIAS)
					juego->datos_juego.contador_categorias += 1;
			}
		}
	}
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

void listar_datos(Tjuego juego){
	int i,opcion;
	
	do{
		printf("\n\t\tLISTAS DE DATOS\n");
		printf("\nSeleccione la opcion deseada\n");
		printf("<1> Categorias (por orden alfabetico)\n<2> Palabras segun su largo (de mayor a menor)\n");
		printf("<3> Buscar categoria\n<4> Volver al menu anterior\n");
		scanf("%d",&opcion);
	}while(opcion<1 || opcion>4);
	
	switch (opcion){
		case 1:{
			//ordenar_categorias(&juego);
			break;
		}
		case 2:{
			//ordenar_palabras(juego);
			break;
		}
		case 3:{
			//buscar_categoria(juego);
			break;
		}
	}
}

int main(){
	Tjuego juego;
	int opcion;
	
	saludo_inicial();
	inicializar_contadores(&juego);
	
	do{
		printf("\n\t\tMENU PRINCIPAL\n");
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
	//			comenzar_partida(juego);
				break;
			}
		}
	}while(opcion>=1 && opcion<=4);

	printf("\nEsperamos que se hayan divertido.\n");
	return 0;
}
