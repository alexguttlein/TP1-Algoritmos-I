/*ENUNCIADO:
1) Ingresar la lista de categorías del juego (ej. Películas, Grupos musicales, Marcas de autos, etc.)						OK
Ingresar la lista general de palabras, una pista por cada una, el puntaje por palabra y a que								OK
categoría pertenece.
Para este punto se debe presentar un menú que permita seleccionar la opción deseada. 										OK
Al momento de realizar el ingreso de datos se debe:
a) Validar que las categorías de las películas existan al cargar la categoría de cada palabra.								OK
b) Validar que las palabras tengan más de 4 letras y menos de 50 y todo en mayúscula. 										OK

2) Listar los datos ingresados. Para este punto se debe presentar un menú que permita seleccionar							OK
la opción deseada. Los listados deben poder ordenarse según:
a) Listado de categorías ordenado alfabéticamente en forma ascendente														FALTA
b) Listado de palabras: alfabéticamente en forma ascendente o por cantidad de caracteres en forma descendente.				FALTA
c) Se ingresa una categoría e informar todas las palabras ordenadas alfabéticamente.										FALTA

3)Realizar una partida de la siguiente forma:
a) armar la base del ahorcado y los espacios seleccionando una palabra random. Se rehace el dibujo en cada letra			FALTA
b) Solicitar ingreso de carácter validar si existe en la palabra agregarlo si no armar el ahorcado							FALTA
c) Se pueden ingresar hasta 6 letras que no están en la palabra en caso contrario se pierde la partida.						FALTA

4) Ingresar cantidad de participantes (y sus nombres), ingresar cantidad de partidas. 										OK
Una vez realizadas todas las partidas indicar:
a) Por cada partida que participante/participantes son los ganadores (los que tienen menos puntos en contra)				FALTA
b) Que participante/participantes son los ganadores de todo el juego 														FALTA

NOTA: los puntos en contra son las letras ingresadas por cada partida (en caso de formar el ahorcado 
suma 10 puntos en vez de 6)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MIN_CADENA 5
#define MAX_CADENA 51
#define MAX_CATEGORIAS 4 //cantidad maxima de categorias
#define MAX_JUGADORES 5
#define MAX_PALABRAS 20 //cantidad maxima de palabras que permite tener el juego
#define MAX_ERRORES 7 // cantidad max de equivocaciones permitidas
#define MAX_PARTIDAS 5 //cantidad max de partidas

typedef char Tstring[MAX_CADENA];
typedef Tstring Tjugadores[MAX_JUGADORES];
typedef Tstring Tpalabra[MAX_PALABRAS];
typedef Tpalabra Tcategorias[MAX_CATEGORIAS];
typedef int Tvector[MAX_JUGADORES];
typedef Tvector Tvec_puntajes[MAX_PARTIDAS];
typedef int Tcontador_categorias[MAX_CATEGORIAS];
typedef Tcontador_categorias TVcontador_palabras[0];
typedef int TvectorContador[MAX_CATEGORIAS];
typedef char Tstring_short[MAX_ERRORES];
typedef char Tfilas[9];
typedef Tfilas Tmatriz[10];

//estructura de los datos a cargar
typedef struct{
	Tcategorias Vcategoria;
	Tcategorias Vpalabra;
	Tcategorias Vpista;
	int ML_categorias;
	int ML_palabras;  	//ML[0] = max logico Categorias; ML[1] = max logico Palabras;
	TVcontador_palabras contador_palabras;
}TdatosJuego;

//estructura de la partida
typedef struct{
	int cant_participantes;
	Tjugadores Vnombre;
	int cant_partidas;
	Tvec_puntajes puntajes;
}Tpartidas;

//estructura tablero de juego
typedef struct{
	Tstring_short errores; 		//letras incorrectas ingresadas
	Tstring palabra_elegida; 	//palabra generada aleatoriamente
	Tstring palabra_actual; 	//estado actual de la palabra a adivinar
	Tstring letras;				//letras correctas que se han ingresado
	int contador_errores;
	int contador_intentos;
	Tmatriz matriz;
}Ttablero;

//estructura principal
typedef struct{
	TdatosJuego datos_juego;
	Tpartidas partidas;
	Ttablero tablero;
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
	for(i = 0; i < MAX_CADENA; i++){
		if (cadena[i]=='\n')
			cadena[i]='\0';
	}
}


//Inicializa todos los contadores del juego.
void inicializar_contadores(Tjuego *vector){
	int i,j;
	
	for(i=0; i<MAX_JUGADORES; i++){
		vector->datos_juego.ML_categorias = 0;
		vector->datos_juego.ML_palabras = 0;
		vector->partidas.cant_participantes = 0;
		vector->partidas.cant_partidas = 0;
		vector->tablero.contador_errores = 0;
		vector->tablero.contador_intentos = 0;
	}
	for (i=0; i<MAX_JUGADORES; i++){
		for(j=0; j<MAX_PARTIDAS; j++){
			vector->partidas.puntajes[i][j]= 0;
		}
	}
	for (i=0; i<MAX_CATEGORIAS; i++){
		for (j=0; j<MAX_PALABRAS; j++){
			vector->datos_juego.contador_palabras[i][j] = 0;
		}
	}
}


//valida que la cadena ingresada este toda en MAYUSCULAS
int validar_mayusculas(Tstring cadena){
	int i;
	bool validar = true;
	
	for (i=0; i<strlen(cadena);i++){
		if (cadena[i]>=97 && cadena[i]<=122){
			validar = false;
		}
	}
	if (validar == false)
		printf("\nIngreso incorrecto. Ingrese toda la palabra en MAYUSCULAS\n");
	return validar;
}


//valida si una cadena tiene el largo dentro de los rangos prestablecidos
int validar_largo_cadena(Tstring cadena){
	int i;
	bool valido = true;
	
	if (strcmp(cadena,"0\n")==0)
		valido = true;
	else{
		if (strlen(cadena)<MIN_CADENA || strlen(cadena)>MAX_CADENA)
			valido = false;
	}
	
	if (valido == false)
		printf("Error de ingreso. Ingrese una palabra de entre %d y %d caracteres\n",MIN_CADENA-1,MAX_CADENA-1);
	
	return valido;
}


//Valida que la cadena ingresada este en mayusculas y el tamaño predeterminado.
int validar_cadena(Tstring cadena,int tamano){
	int i;
	bool validar = true;
	bool validar_mayuscula = true;
	bool validar_largo = true;
	
	validar_mayuscula = validar_mayusculas(cadena);
	if (tamano>2)
		validar_largo = validar_largo_cadena(cadena);
	
	if (!validar_mayuscula || !validar_largo)
		validar = false;
	return validar;
}


//se ingresa una cadena y se realizan las validaciones necesarias
void ingresar_cadena(Tstring cadena, int tamano){
	bool validar = true;
	
	do{
		fgets(cadena,tamano,stdin); 
		fflush(stdin);
		validar = validar_cadena(cadena,tamano);
	}while(validar == false);
	
	cambiar_fin_cadena(cadena);
}


//Se ingresan las categorias de las palabras a adivinar.
void ingresar_categoria(TdatosJuego *datos_juego){
	int i;
	bool salir = false;
	char condicion_salida[3]="0\0";
	int ML_categoria = datos_juego->ML_categorias;
	
	printf("Ingrese las categorias (hasta %d), finalice con 0: ",MAX_CATEGORIAS);
	
	for (i=ML_categoria; i<MAX_CATEGORIAS && salir==false; i++){
		printf("\nCategoria n%c%d: ",167,i+1);
		ingresar_cadena(*datos_juego->Vcategoria[i],MAX_CADENA);

		if (strcmp(*datos_juego->Vcategoria[i],condicion_salida) == 0){
			salir = true;
		}else{ 
			datos_juego->ML_categorias +=1;
		}
	}
	if ((MAX_CATEGORIAS - datos_juego->ML_categorias) == 0)
		printf("\nSe alcanzo el maximo de categorias. Disculpe las molestias.\n\n");
}


//Se ingresan las palabras a adivinar.
void ingresar_palabra(TdatosJuego *datos_juego){
	int i, j;
	bool salir = false;
	char condicion_salida[3]="0\0";
	int pos_inicial = datos_juego->contador_palabras[0][0];
	int max_palabras = MAX_PALABRAS/MAX_CATEGORIAS; //mayor cantidad de palabras aceptadas para cada categoria
	
	for (i=0; i<MAX_CATEGORIAS && datos_juego->ML_categorias>i; i++){
		printf("\nIngresar palabras (hasta %d por categoria):\n",max_palabras);
		for(j=pos_inicial; j<MAX_PALABRAS && datos_juego->contador_palabras[i][0] < max_palabras && salir==false; j++){
			printf("Categoria %s. Palabra n%c%d: ",datos_juego->Vcategoria[i],167,j+1);
			ingresar_cadena(datos_juego->Vpalabra[i][j],MAX_CADENA);
			if(strcmp(datos_juego->Vpalabra[i][j],condicion_salida) == 0){
				salir = true;
			}else{
				datos_juego->ML_palabras +=1;
				datos_juego->contador_palabras[i][0] += 1;
				printf("Ingrese una descripcion para %s - %s: ",datos_juego->Vcategoria[i],datos_juego->Vpalabra[i][0]);
				ingresar_cadena(datos_juego->Vpista[i][j],MAX_CADENA);
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
				ingresar_categoria(&juego->datos_juego);
				break;
			}
			case 2:{
				ingresar_palabra(&juego->datos_juego);
				break;
			}
		}
	}while(opcion>=1 && opcion<=2);
}


//se selecciona la manera de listar los datos ingresados
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
			//ordenar_categorias();
			break;
		}
		case 2:{
			//ordenar_palabras();
			break;
		}
		case 3:{
			//buscar_categoria();
			break;
		}
	}
}


//Se ingresan datos de la partida (cantidad de jugadores, nombres, cantidad de partidas)
void ingresar_participantes(Tjuego *juego){
	int i;
	
	while(juego->partidas.cant_participantes <= 0){
		printf("Ingresar cantidad de participantes: ");
		scanf("%d",&juego->partidas.cant_participantes);
		fflush(stdin);	
	}
	
	for (i=0; i<juego->partidas.cant_participantes; i++){
		printf("Ingresar nombre del participante n%c %d: ",167,i+1);
		ingresar_cadena(juego->partidas.Vnombre[i],MAX_CADENA);
	}
	
	while (juego->partidas.cant_partidas <= 0){
		printf("Indicar cantidad de partidas a jugar: ");
		scanf("%d",&juego->partidas.cant_partidas);
		fflush(stdin);
	}
}


//Se verifica que se hayan ingresado todos los datos necesarios para comenzar una partida
int partida_lista(Tjuego juego){
	int i;
	bool verificar = true;
	if (juego.datos_juego.ML_categorias == 0){
		printf("\nAntes de comenzar debe ingresar CATEGORIAS.\n");
		verificar = false;
	}
	if (juego.datos_juego.contador_palabras[0] == 0){
		printf("\nAntes de comenzar debe ingresar PALABRAS a las categorias.\n");
		verificar = false;
	}
	if (juego.partidas.cant_participantes == 0){
		printf("\nAntes de comenzar debe ingresar la cantidad de PARTICIPANTES\n");
		verificar = false;
	}
	return verificar;
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
		scanf("%i",&opcion);		
		fflush(stdin);
		
		switch(opcion){
			case 1:{
				ingresar_datos(&juego);
				break;
			}
			case 2:{
//				listar_datos(juego);
				break;
			}
			case 3:{
				ingresar_participantes(&juego);
				break;
			}
			case 4:{
//				comenzar_partida(&juego);
				break;
			}
		}
	}while(opcion>=1 && opcion<=4);

	printf("\nEsperamos que se hayan divertido.\n");
	return 0;
}
