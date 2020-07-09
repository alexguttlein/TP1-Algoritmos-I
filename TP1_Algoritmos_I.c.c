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
typedef Tcontador_categorias TVcontador_palabras[1];
typedef char Tstring_short[MAX_ERRORES];
typedef char Tfilas[9];
typedef Tfilas Tmatriz[10];

//estructura de los datos a cargar
typedef struct{
	Tcategorias Vcategoria;
	Tcategorias Vpalabra;
	Tcategorias Vpista;
	int ML_categorias;
	int ML_palabras;
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
	printf("\n\t\t\tBIENVENIDOS AL AHORCADO\n");
	printf(" Un juego de vida o muerte para divertirte con tus amigos.\n\n");
	printf(" Para comenzar deberan ingresar las CATEGORIAS (siempre en MAYUSCULAS), las palabras y alguna pista.\n");
	printf(" Tambien deberan indicar el numero y nombre de jugadores y la cantidad de partidas que desean jugar.\n");
	printf("\n Se permite hasta un maximo de %d jugadores.\n",MAX_JUGADORES);
	printf("\n Al final de la ultima partida se indicara quien es el GANADOR.\n");
	printf(" Se pueden ingresar hasta 6 letras que no estan en la palabra, en caso contrario sentiran el \n suave roce");
	printf(" de la soga en sus fragiles cuellos hasta perder la vida.\n");
	printf("\n Estan listos para morir?\n\n");
	system("pause");
	printf("\n\n");
}


//Reglas del juego
void ver_reglas(){
	printf("\n\tREGLAS:\n\t- Cada jugador ingresara una letra durante su turno. Si es correcta se completara en la palabra;\n");
	printf("\t  si es incorrecta se le agregara puntos y se completara el ahorcado.\n\t- Cada letra mal ingresada es un punto\n");
	printf("\t- Se sumaran 4 puntos a quien ingrese mal la ultima letra (la 7ma) y complete el ahorcado.\n");
	printf("\t- Quien tenga menos cantidad de puntos al finalizar cada partida sera el ganador de la misma.\n");
	printf("\t- El ganador del juego se determinara en base a la suma total de todas las partidas realizadas.\n\n");
}


void about(){
	printf("\n\t\t\t- - - - - - - - - - - - - - - - - - - - -");
	printf("\n\t\t\t|\tALGORITMOS Y PROGRAMACION I     |\n\t\t\t\t\t  TP N%c 1\n",167);
	printf("\t\t\t|\t\t  GRUPO 4\t\t|\n\n\t\t\t");
	printf("|  Alumnos colaboradores:\t\t|\n\t\t\t  - Guttlein Gareis, Alexis Daniel\n\t\t\t| - Shiao, Tomas Jorge\t\t\t|");
	printf("\n\t\t\t- - - - - - - - - - - - - - - - - - - - -\n");
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


//Valida que la cadena ingresada este en mayusculas y el tamaï¿½o predeterminado.
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
	int max_palabras = MAX_PALABRAS/MAX_CATEGORIAS; //mayor cantidad de palabras aceptadas para cada categoria

	for (i=0; i<MAX_CATEGORIAS && datos_juego->ML_categorias>i; i++){
		printf("\nIngresar palabras (hasta %d por categoria):\n",max_palabras);
		for(j=datos_juego->contador_palabras[i][1]; j<MAX_PALABRAS && datos_juego->contador_palabras[i][1] < max_palabras && salir==false; j++){
			printf("Categoria %s. Palabra n%c%d: ",datos_juego->Vcategoria[i],167,j+1);
			ingresar_cadena(datos_juego->Vpalabra[i][j],MAX_CADENA);
			if(strcmp(datos_juego->Vpalabra[i][j],condicion_salida) == 0){
				salir = true;
			}else{
				datos_juego->ML_palabras +=1;
				datos_juego->contador_palabras[i][1] += 1;
				printf("Ingrese una descripcion para %s - %s: ",datos_juego->Vcategoria[i],datos_juego->Vpalabra[i][j]);
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


// Se imprime cada elemento de la lista.
void imprimir_lista(Tcategorias cadena, int ML){
	int i;
	for (i=0; i<ML; i++){
		printf(" - %s\n",cadena[i]);
	}
}


//Se ordenan alfabeticamente los elementos del vector.
void ordenar_categorias(int maximo, Tjuego *juego){
	int i, j;
	Tstring aux;

	for (i=0; i<maximo-1; i++){
		for (j=i+1; j<maximo; j++){
			if (strcmp (*juego->datos_juego.Vcategoria[i], *juego->datos_juego.Vcategoria[j]) > 0){
					strcpy(aux, *juego->datos_juego.Vcategoria[i]);
					strcpy(*juego->datos_juego.Vcategoria[i],*juego->datos_juego.Vcategoria[j]);
					strcpy(*juego->datos_juego.Vcategoria[j],aux);
			}
		}
	}
}

void ordenar_palabras(int max, Tjuego *juego){
	int i, j;
	Tstring temp;
	for(i=0; i< (max-1); i++){
		for(j=1; j<max; j++){
			if(strlen(*juego->datos_juego.Vpalabra[j])>strlen(*juego->datos_juego.Vpalabra[j+1])){
				strcpy(temp, *juego->datos_juego.Vpalabra[j]);
				strcpy(*juego->datos_juego.Vpalabra[j], *juego->datos_juego.Vpalabra[j+1]);
				strcpy(*juego->datos_juego.Vpalabra[j+1], temp);
			}
		}
	}
}

void buscar_categoria(Tjuego juego){
	int i, j, k;
	int vec_posicion[juego.datos_juego.ML_categorias];
	Tstring categoria;

	printf("Ingresar categoria a buscar: ");

	fflush(stdin);
	ingresar_cadena(categoria,MAX_CADENA);

	k=0;
	j=0;
	for (i=0; i<MAX_CATEGORIAS; i++){
		if (strstr(juego.datos_juego.Vcategoria[i][j],categoria) != NULL){
			vec_posicion[k] = i;
			k++;
		}
	}
	int dim_vec = k;

	for (i=0; i<dim_vec; i++){
		printf("\n - %s:\n",juego.datos_juego.Vcategoria[vec_posicion[i]]);
		for (j=0; j<juego.datos_juego.contador_palabras[vec_posicion[i]][1]; j++){
			printf("\t- %s\n",juego.datos_juego.Vpalabra[vec_posicion[i]][j]);
		}
	}
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
			ordenar_categorias(juego.datos_juego.ML_categorias, &juego);
			printf("\nCATEGORIAS POR ORDEN ALFABETICO:\n");
			imprimir_lista(juego.datos_juego.Vcategoria, juego.datos_juego.ML_categorias);
			break;
		}
		case 2:{
			ordenar_palabras(juego.datos_juego.ML_palabras, &juego);
			printf("\nPALABRAS POR CANTIDAD DE CARACTERES:\n");
			imprimir_lista(juego.datos_juego.Vpalabra, juego.datos_juego.ML_palabras);
			break;
		}
		case 3:{
			buscar_categoria(juego);
			break;
		}
	}
}


//Se ingresan datos de la partida (cantidad de jugadores, nombres, cantidad de partidas)
void ingresar_participantes(Tjuego *juego){
	int i;
	juego->partidas.cant_participantes = 0;

	while(juego->partidas.cant_participantes <= 0 || juego->partidas.cant_participantes > MAX_JUGADORES){
		printf("Ingresar cantidad de participantes (maximo %d): ",MAX_JUGADORES);
		scanf("%d",&juego->partidas.cant_participantes);
		fflush(stdin);
	}

	for (i=0; i<juego->partidas.cant_participantes; i++){
		printf("Ingresar nombre del participante n%c %d: ",167,i+1);
		ingresar_cadena(juego->partidas.Vnombre[i],MAX_CADENA);
	}

	while (juego->partidas.cant_partidas <= 0 || juego->partidas.cant_partidas > MAX_PARTIDAS){
		printf("Indicar cantidad de partidas a jugar (max %d): ",MAX_PARTIDAS);
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
	if (juego.datos_juego.ML_palabras == 0){
		printf("\nAntes de comenzar debe ingresar PALABRAS a las categorias.\n");
		verificar = false;
	}
	if (juego.partidas.cant_participantes == 0){
		printf("\nAntes de comenzar debe ingresar la cantidad de PARTICIPANTES\n");
		verificar = false;
	}
	return verificar;
}


//asigna el turno para que cada jugador participe
int asignar_turno(Tjuego juego){
	int i;
	int posicion = juego.tablero.contador_intentos%juego.partidas.cant_participantes;
	printf("\n\t   Turno jugador %d\n",posicion+1);
	printf("\t\t%s\n",juego.partidas.Vnombre[posicion]);

	return posicion;
}

//se ingresa una letra y en base a esta se actualiza el estado del tablero
void actualizar_tablero(Tstring palabra, Tjuego *juego, Tstring palabra_actual, int num_partida){
	int i,j;
	char letra[2];

	int jugador = asignar_turno(*juego);

	ingresar_cadena(letra,2);
	fflush(stdin);

	//si la letra esta en la palabra se guarda en "letras", sino se guarda en "errores" y se suman puntos al usuario
	if (strstr(palabra,letra) != NULL){
		strcat(juego->tablero.letras,letra);
	}else{
		if (juego->tablero.contador_errores == 6){
			strcpy(juego->tablero.errores,letra);
			juego->tablero.contador_errores += 1;
			juego->partidas.puntajes[jugador][num_partida] += 4;
		}else{
			strcat(juego->tablero.errores,letra);
			juego->tablero.contador_errores += 1;
			juego->partidas.puntajes[jugador][num_partida] += 1;
		}
	}

	//se cambian los '_' por las letras ya adivinadas
	for(i=0; i<strlen(palabra);i++){
		if (letra[0] == palabra[i])
			palabra_actual[i]=letra[0];
	}
	juego->tablero.contador_intentos += 1;

	printf("\tRESULTADOS PARTIDA n%c%d:\n",167,num_partida+1);
	for(i=0; i<juego->partidas.cant_participantes; i++){
		printf("\t%s\t\t%d puntos\n",juego->partidas.Vnombre[i],juego->partidas.puntajes[i][num_partida]);
	}
}


//Comprueba si la palabra buscada esta completa
int verificar_fin_partida(Tstring palabra_actual){
	int i;
	bool fin_partida = true;

	for(i=0; i<strlen(palabra_actual);i++){
		if (strstr(palabra_actual,"_") != NULL){
			fin_partida = false;
		}
	}
	return fin_partida;
}


//suma los puntajes de cada partida y determina el ganador del juego
int ganador_final(Tpartidas partida){
	int i, j, posicion;
	Tvector Vec_suma; //se suman los totales acumulados de cada jugador al final del juego
	posicion = 0;

	for(i=0; i<partida.cant_participantes; i++){
		Vec_suma[i] = 0;
	}
	for(i=0; i<partida.cant_participantes; i++){
		for (j=0; j<partida.cant_partidas; j++){
			Vec_suma[i] += partida.puntajes[i][j];
		}
	}
	for(i=1; i<partida.cant_participantes;i++){
		if (Vec_suma[i]<Vec_suma[posicion])
			posicion = i;
	}
	return posicion;
}


//determina el ganador de cada partida
int ganador_partida(Tpartidas partida, int num_partida){
	int i, posicion;
	posicion = 0;

	for(i=1;i<partida.cant_participantes;i++){
		if (partida.puntajes[i][num_partida] < partida.puntajes[posicion][num_partida])
			posicion = i;
	}
	return posicion;
}


//Se comparan los puntos para determinar el ganador
int determinar_ganador(Tpartidas partida,int num_partida){
	int i, j, posicion;

	if (num_partida==partida.cant_partidas){
		posicion = ganador_final(partida);
	} else {
		posicion = ganador_partida(partida, num_partida);
	}
	return posicion;
}


//se dejan en cero los contadores y se vacian las cadenas al iniciar cada partida
void resetear_partida(Ttablero *tablero){
	int i;
	tablero->contador_errores = 0;
	tablero->contador_intentos = 0;
	strcpy(tablero->letras,"");
	strcpy(tablero->errores,"");
	strcpy(tablero->palabra_actual,"");
	strcpy(tablero->palabra_elegida,"");
}


//se muestra el estado del dibujo del ahorcado
void imprimir_tablero(Ttablero tablero, Tstring pista){
	int i;
	system("cls");
	printf("\t\tPISTA: %s\n",pista);
	for(i=0;i<10; i++){
		printf("\n%s",tablero.matriz[i]);
	}
	printf("\t\t%s\n",tablero.palabra_actual);
	printf("%s\n",tablero.errores);
}


//se asignan valores al dibujo del ahorcado segun la cantidad de errores cometidos
void dibujar_tablero(Ttablero *tablero){
	int i;

	strcpy(tablero->matriz[0],"______");
	strcpy(tablero->matriz[1],"|  |");
	strcpy(tablero->matriz[2],"|");
	strcpy(tablero->matriz[3],"|");
	strcpy(tablero->matriz[4],"|");
	strcpy(tablero->matriz[5],"|");
	strcpy(tablero->matriz[6],"|");
	strcpy(tablero->matriz[7],"|______");
	strcpy(tablero->matriz[8],"|______|");

	if(tablero->contador_errores > 0)
		strcpy(tablero->matriz[2],"|  O");
	if(tablero->contador_errores > 1)
		strcpy(tablero->matriz[3],"|  |  ");
	if(tablero->contador_errores > 2)
		strcpy(tablero->matriz[3],"|  |\\");
	if(tablero->contador_errores > 3)
		strcpy(tablero->matriz[3],"| /|\\");
	if(tablero->contador_errores > 4)
		strcpy(tablero->matriz[4],"|  / ");

	if ((tablero->contador_errores > 5)){
		strcpy(tablero->matriz[2],"|  O");
		strcpy(tablero->matriz[3],"| /|\\");
		strcpy(tablero->matriz[4],"|  /\\");
	}
}


//Se realiza cada partida
void realizar_partida(Tjuego *juego, Tstring palabra, Tstring palabra_actual, int num_partida, Tstring pista){
	int i, j;
	int tamano = strlen(palabra);
	bool fin_partida;

	do{
		dibujar_tablero(&juego->tablero);
		imprimir_tablero(juego->tablero, pista);
		actualizar_tablero(palabra, juego, palabra_actual, num_partida);
		fin_partida = verificar_fin_partida(palabra_actual);
	}while(juego->tablero.contador_errores < MAX_ERRORES && fin_partida==false);

	int ganador = determinar_ganador(juego->partidas, num_partida);
	printf("\nGanador de la partida n%c %d: %s\n\n",167,num_partida+1,juego->partidas.Vnombre[ganador]);
	resetear_partida(&juego->tablero);
}


//se asigna '_' a cada caracter distinto de ' ' (espacio)
void reiniciar_palabra_actual(Tstring palabra_actual, Tstring palabra){
	int i;

	strcpy(palabra_actual,palabra);
	for(i=0; i<strlen(palabra);i++)	{
		if (palabra[i]!=32){
			palabra_actual[i] = '_';
		}
	}
}


//Selecciona una palabra al azar y realiza tantas partidas como indique el usuario
void comenzar_juego(Tjuego *juego){
	bool salir = false;
	int i;
	int rand_categ;
	int rand_num;

	if (partida_lista(*juego)){
		printf("\t\t\tCOMIENZA EL JUEGO...\n\t\t\t   MUCHA SUERTE!!\n");

		srand(time(NULL));

		i = 0;
		while (i < juego->partidas.cant_partidas && salir==false){
			rand_categ = rand() % (juego->datos_juego.ML_categorias); //categoria seleccionada al azar
			rand_num = rand() % (juego->datos_juego.contador_palabras[rand_categ][1]); //numero de palabra generado al azar
			strcpy(juego->tablero.palabra_elegida, juego->datos_juego.Vpalabra[rand_categ][rand_num]); //selecciona una palabra al azar

			reiniciar_palabra_actual(juego->tablero.palabra_actual, juego->tablero.palabra_elegida); //cambia los caracteres por '_'

			printf("\t\tComienza la partida n%c %d\n",167,i+1);
			system("pause");
			realizar_partida(juego,juego->tablero.palabra_elegida, juego->tablero.palabra_actual, i, juego->datos_juego.Vpista[rand_categ][rand_num]); //ingresa a la partida
			i += 1;
		}
			int ganador = determinar_ganador(juego->partidas, i);
			printf("\n\t******************************************");
			printf("\n\t GANADOR del juego:  \t\t%s \n", juego->partidas.Vnombre[ganador]);
			printf("\t******************************************\n");
	}
}


int main(){
	Tjuego juego;
	int opcion;

	about();
	saludo_inicial();
	inicializar_contadores(&juego);

	do{
		printf("\n\t\tMENU PRINCIPAL\n");
		printf("\nSeleccionar opcion deseada:\n");
		printf("<1> Ingresar categorias y palabras.\n<2> Listar los datos ingresados.\n");
		printf("<3> Ingresar datos de los jugadores.\n<4> Comenzar a jugar.\n<5> Ver reglas de juego.\n<6> Salir del juego.\n");
		scanf("%i",&opcion);
		fflush(stdin);

		switch(opcion){
			case 1:{
				ingresar_datos(&juego);
				break;
			}
			case 2:{
				listar_datos(juego);
				break;
			}
			case 3:{
				ingresar_participantes(&juego);
				break;
			}
			case 4:{
				comenzar_juego(&juego);
				break;
			}
			case 5:{
				ver_reglas();
				break;
			}
		}
	}while(opcion>=1 && opcion<=5);

	printf("\nEsperamos que se hayan divertido.\n");

	return 0;
}
