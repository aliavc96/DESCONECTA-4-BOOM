#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;


// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar

//valoracion del jugador 1
double Valoracion1(const Environment &estado){

	int jugador = 1;
	int jugador2 = 2;
	int bomba = 4;
	double suma = 0;
	for(int i = 0; i< 7; i++){
		for(int j = 0; j < 7; j++){

			// teniendo en cuenta que la matriz está invertida

			// comprobaciones horizontales
      // obtenemos un beneficio si hay una bomba entre las fichas de mi jugador
			// para ir horizontalmente hacia la izquierda
			// le pongo antes restricciones para que no me de fallo de segmento
				if(i < 6 && j < 1 && estado.See_Casilla(i,j) == jugador && (estado.See_Casilla(i,j+1) == bomba || estado.See_Casilla(i,j) == bomba || estado.See_Casilla(i,j+2) == bomba
					|| estado.See_Casilla(i,j+3) == bomba || estado.See_Casilla(i,j+4) == bomba || estado.See_Casilla(i,j+5) == bomba || estado.See_Casilla(i,j+6) == bomba))
					suma += 10;
				if( i < 6 && j < 1 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i,j+1) == jugador && (estado.See_Casilla(i,j+1) == bomba || estado.See_Casilla(i,j) == bomba || estado.See_Casilla(i,j+2) == bomba
					|| estado.See_Casilla(i,j+3) == bomba || estado.See_Casilla(i,j+4) == bomba || estado.See_Casilla(i,j+5) == bomba || estado.See_Casilla(i,j+6) == bomba))
					suma += 400;
				//if( i < 6 && j < 4 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i,j+1) == jugador && estado.See_Casilla(i,j+2) == jugador && estado.See_Casilla(i,j+3) == bomba)
				//	suma += 400;

				// para ir hacia la derecha
				if( i < 6 && j >= 6 && estado.See_Casilla(i,j) == jugador && (estado.See_Casilla(i,j-1) == bomba || estado.See_Casilla(i,j) == bomba || estado.See_Casilla(i,j-2) == bomba
					|| estado.See_Casilla(i,j-3) == bomba || estado.See_Casilla(i,j-4) == bomba || estado.See_Casilla(i,j-5) == bomba || estado.See_Casilla(i,j-6) == bomba))
					suma += 10;
				if(i < 6 && j >= 6 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i,j-1) == jugador && (estado.See_Casilla(i,j-1) == bomba || estado.See_Casilla(i,j) == bomba || estado.See_Casilla(i,j-2) == bomba
					|| estado.See_Casilla(i,j-3) == bomba || estado.See_Casilla(i,j-4) == bomba || estado.See_Casilla(i,j-5) == bomba || estado.See_Casilla(i,j-6) == bomba))
					suma += 400;
				//if(i < 6 && j >= 3 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i,j-1) == jugador && estado.See_Casilla(i,j-2) == jugador && estado.See_Casilla(i,j-3) == bomba)
				//	suma += 400;

				// comprobaciones verticales

				// solo se puede subir, no bajar y la gravedad en este caso no afecta, porque se supone que ya hemos comprobado que hay fichas debajo
				if(i < 6 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j) == jugador && (estado.See_Casilla(i,j) == bomba || estado.See_Casilla(i+1,j) == bomba ))
					suma += 10;
				if( i < 5 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j) == jugador && estado.See_Casilla(i+2,j) == jugador && (
					estado.See_Casilla(i,j) == bomba || estado.See_Casilla(i+1,j) == bomba || estado.See_Casilla(i+2,j) == bomba))
					suma += 400;
				//if(i < 4 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j) == jugador && estado.See_Casilla(i+2,j) == jugador  && estado.See_Casilla(i+3,j) == bomba)
				//	suma += 400;

				// para que haya una bomba en medio de las fichas del jugador 2
				// para que haya dos casillas del jugador contrario y la bomba se encuentra en medio de las casillas del contrincante 
				if(i < 5 && j < 1 && estado.See_Casilla(i,j) == jugador2 && (estado.See_Casilla(i+1,j) == bomba ||  estado.See_Casilla(i+1,j+1) == bomba
				 || estado.See_Casilla(i+1,j+2) == bomba  || estado.See_Casilla(i+1,j+3) == bomba || estado.See_Casilla(i+1,j+4) == bomba || estado.See_Casilla(i+1,j+5) == bomba
				 	|| estado.See_Casilla(i+1,j+6) == bomba) && estado.See_Casilla(i+2,j) == jugador2 &&  estado.See_Casilla(i+1,j) == jugador)
					suma += 70;

				if(i < 4  && j < 1 && estado.See_Casilla(i,j) == jugador2 && (estado.See_Casilla(i+1,j) == bomba || estado.See_Casilla(i+1,j+1) == bomba ||
				estado.See_Casilla(i+1,j+2) == bomba || estado.See_Casilla(i+1,j+3) == bomba 
				|| estado.See_Casilla(i+1,j+4) == bomba || estado.See_Casilla(i+1,j+5) == bomba || estado.See_Casilla(i+1,j+6) == bomba) && estado.See_Casilla(i+2,j) == jugador2 && estado.See_Casilla(i+3,j) == jugador2
					&& estado.See_Casilla(i+1,j) == jugador)
					suma += 300;
				
				if(i < 4 && j < 1 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j) == jugador2 && (estado.See_Casilla(i+2,j) == bomba || estado.See_Casilla(i+2,j+1) == bomba 
					|| estado.See_Casilla(i+2,j+2) == bomba || estado.See_Casilla(i+2,j+3) == bomba || estado.See_Casilla(i+2,j+4) == bomba || 
					estado.See_Casilla(i+2,j+5) == bomba || estado.See_Casilla(i+2,j+6) == bomba )&& estado.See_Casilla(i+3,j) == jugador2 && estado.See_Casilla(i+2,j) == jugador)
					suma += 300;

				if(i < 3 && j < 1 && estado.See_Casilla(i,j) == jugador2 && (estado.See_Casilla(i+1,j) == bomba || estado.See_Casilla(i+1,j+1) == bomba || estado.See_Casilla(i+1,j+2) == bomba
					|| estado.See_Casilla(i+1,j+3) == bomba || estado.See_Casilla(i+1,j+4) == bomba || estado.See_Casilla(i+1,j+5) == bomba || estado.See_Casilla(i+1,j+6) == bomba ) && estado.See_Casilla(i+2,j) == jugador2
				 && estado.See_Casilla(i+3,j) == jugador2  && estado.See_Casilla(i+4,j) == jugador2 && estado.See_Casilla(i+1,j) == jugador)
					suma += 500;

				if(i < 3 && j < 1 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j) == jugador2 && (estado.See_Casilla(i+2,j) == bomba || estado.See_Casilla(i+2,j+1) == bomba || estado.See_Casilla(i+2,j+2) == bomba ||
					estado.See_Casilla(i+2,j+3) == bomba || estado.See_Casilla(i+2,j+4) == bomba || estado.See_Casilla(i+2,j+5) == bomba 
					|| estado.See_Casilla(i+2,j+6) == bomba ) && estado.See_Casilla(i+3,j) == jugador2 && estado.See_Casilla(i+4,j) == jugador2 && estado.See_Casilla(i+2,j) == jugador)
					suma += 500;

				if(i < 3 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j) == jugador2 && estado.See_Casilla(i+2,j) == jugador2 && (estado.See_Casilla(i+3,j) == bomba ||
					estado.See_Casilla(i+3,j+1) == bomba || estado.See_Casilla(i+3,j+2) == bomba || estado.See_Casilla(i+3,j+3) == bomba || estado.See_Casilla(i+3,j+4) == bomba ||
					estado.See_Casilla(i+3,j+5) == bomba || estado.See_Casilla(i+3,j+6) == bomba) && estado.See_Casilla(i+4,j) == jugador2 && estado.See_Casilla(i+3,j) == jugador)
					suma += 500;


				//comprobaciones en diagonal

				// la diagonal avanza para la derecha
				if(j >= 1 && i < 6 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j-1) == jugador && (estado.See_Casilla(i+1,j-1) == bomba || estado.See_Casilla(i,j) == bomba) )
					suma += 10;
				if(j >= 2 && i < 5 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j-1) == jugador && estado.See_Casilla(i+2,j-2) == jugador && (estado.See_Casilla(i+2,j-2) == bomba
					|| estado.See_Casilla(i+1,j-1) == bomba || estado.See_Casilla(i,j) == bomba)) 
					suma += 400;
				//if(j >= 3 && i < 4 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j-1) == jugador && estado.See_Casilla(i+2,j-2) == jugador && estado.See_Casilla(i+3,j-3) == bomba )
				//	suma += 400;

				// la diagonal avanza para la izquierda
				if(i < 6 && j < 6 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j+1) == jugador &&  (estado.See_Casilla(i+1,j+1) == bomba || estado.See_Casilla(i,j) == bomba) )
					suma += 10;
				if(i < 5 && j < 5 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j+1) == jugador && estado.See_Casilla(i+2,j+2) == jugador &&
					(estado.See_Casilla(i,j) == bomba || estado.See_Casilla(i+1,j+1) == bomba || estado.See_Casilla(i+2,j+2) == bomba)) 
					suma += 400;
				//if(i < 4 && j < 4 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j+1) == jugador && estado.See_Casilla(i+2,j+2) == jugador && estado.See_Casilla(i+3,j+3) == bomba)
				//	suma += 400;


				
				// comprobaciones para ver las posiciones de mi contrincante
				// en vertical
				if(i < 5 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j) == jugador2)
					suma += 10;	

				if(i < 4 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j) == jugador2 && estado.See_Casilla(i+2,j) == jugador2)
					suma += 300;

				// en horizontal
				if(j > 1 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i,j-1) == jugador2)
					suma += 10;

				if(j > 2 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i,j-1) == jugador2 && estado.See_Casilla(i,j) == jugador2)
					suma += 300;
				// en diagonal
				// la diagonal avanza para la derecha
				if(j >= 1 && i < 6 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j-1) == jugador2 ) 
					suma += 10;
				if(j >= 2 && i < 5 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j-1) == jugador2 && estado.See_Casilla(i+2,j-2) == jugador2) 
					suma += 300;
				// la diagonal avanza hacia la izquierda
				if(i < 6 && j < 6 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j+1) == jugador2) 
					suma += 10;
				if(i < 5 && j < 5 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j+1) == jugador2 && estado.See_Casilla(i+2,j+2) == jugador2) 
					suma += 300;


	        
	    }
	}

	return suma;
}
// valoracion del jugador 2
double Valoracion2(const Environment &estado){

	int jugador = 2;
	int jugador2 = 1;
	int bomba = 5;
	double suma = 0;
	for(int i = 0; i< 7; i++){
		for(int j = 0; j < 7; j++){

			// teniendo en cuenta que la matriz está invertida
       // obtenemos un beneficio si hay una bomba entre las fichas de mi jugador
			// para ir horizontalmente hacia la izquierda
			// le pongo antes restricciones para que no me de fallo de segmento

				if(i < 6 && j < 1 && estado.See_Casilla(i,j) == jugador && (estado.See_Casilla(i,j+1) == bomba || estado.See_Casilla(i,j) == bomba || estado.See_Casilla(i,j+2) == bomba
					|| estado.See_Casilla(i,j+3) == bomba || estado.See_Casilla(i,j+4) == bomba || estado.See_Casilla(i,j+5) == bomba || estado.See_Casilla(i,j+6) == bomba))
					suma += 10;
				if( i < 6 && j < 1 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i,j+1) == jugador && (estado.See_Casilla(i,j+1) == bomba || estado.See_Casilla(i,j) == bomba || estado.See_Casilla(i,j+2) == bomba
					|| estado.See_Casilla(i,j+3) == bomba || estado.See_Casilla(i,j+4) == bomba || estado.See_Casilla(i,j+5) == bomba || estado.See_Casilla(i,j+6) == bomba))
					suma += 470;
				//if( i < 6 && j < 4 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i,j+1) == jugador && estado.See_Casilla(i,j+2) == jugador && estado.See_Casilla(i,j+3) == bomba)
				//	suma += 400;

				// para ir hacia la derecha
				if( i < 6 && j >= 6 && estado.See_Casilla(i,j) == jugador && (estado.See_Casilla(i,j-1) == bomba || estado.See_Casilla(i,j) == bomba || estado.See_Casilla(i,j-2) == bomba
					|| estado.See_Casilla(i,j-3) == bomba || estado.See_Casilla(i,j-4) == bomba || estado.See_Casilla(i,j-5) == bomba || estado.See_Casilla(i,j-6) == bomba))
					suma += 10;
				if(i < 6 && j >= 6 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i,j-1) == jugador && (estado.See_Casilla(i,j-1) == bomba || estado.See_Casilla(i,j) == bomba || estado.See_Casilla(i,j-2) == bomba
					|| estado.See_Casilla(i,j-3) == bomba || estado.See_Casilla(i,j-4) == bomba || estado.See_Casilla(i,j-5) == bomba || estado.See_Casilla(i,j-6) == bomba))
					suma += 470;

				// comprobaciones verticales

				// solo se puede subir, no bajar y la gravedad en este caso no afecta, porque se supone que ya hemos comprobado que hay fichas debajo
				if(i < 6 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j) == jugador && (estado.See_Casilla(i,j) == bomba || estado.See_Casilla(i+1,j) == bomba ))
					suma += 10;
				if( i < 5 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j) == jugador && estado.See_Casilla(i+2,j) == jugador && (
					estado.See_Casilla(i,j) == bomba || estado.See_Casilla(i+1,j) == bomba || estado.See_Casilla(i+2,j) == bomba))
					suma += 400;
				//if(i < 4 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j) == jugador && estado.See_Casilla(i+2,j) == jugador  && estado.See_Casilla(i+3,j) == bomba)
				//	suma += 400;

				// para que haya una bomba en medio de las casillas del jugador 2
				if(i < 5 && j < 1 && estado.See_Casilla(i,j) == jugador2 && (estado.See_Casilla(i+1,j) == bomba ||  estado.See_Casilla(i+1,j+1) == bomba
				 || estado.See_Casilla(i+1,j+2) == bomba  || estado.See_Casilla(i+1,j+3) == bomba || estado.See_Casilla(i+1,j+4) == bomba || estado.See_Casilla(i+1,j+5) == bomba
				 	|| estado.See_Casilla(i+1,j+6) == bomba) && estado.See_Casilla(i+2,j) == jugador2 &&  estado.See_Casilla(i+1,j) == jugador)
					suma += 70;

				if(i < 4  && j < 1 && estado.See_Casilla(i,j) == jugador2 && (estado.See_Casilla(i+1,j) == bomba || estado.See_Casilla(i+1,j+1) == bomba ||
				estado.See_Casilla(i+1,j+2) == bomba || estado.See_Casilla(i+1,j+3) == bomba 
				|| estado.See_Casilla(i+1,j+4) == bomba || estado.See_Casilla(i+1,j+5) == bomba || estado.See_Casilla(i+1,j+6) == bomba) && estado.See_Casilla(i+2,j) == jugador2 && estado.See_Casilla(i+3,j) == jugador2
					&& estado.See_Casilla(i+1,j) == jugador)
					suma += 300;
				
				if(i < 4 && j < 1 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j) == jugador2 && (estado.See_Casilla(i+2,j) == bomba || estado.See_Casilla(i+2,j+1) == bomba 
					|| estado.See_Casilla(i+2,j+2) == bomba || estado.See_Casilla(i+2,j+3) == bomba || estado.See_Casilla(i+2,j+4) == bomba || 
					estado.See_Casilla(i+2,j+5) == bomba || estado.See_Casilla(i+2,j+6) == bomba )&& estado.See_Casilla(i+3,j) == jugador2 && estado.See_Casilla(i+2,j) == jugador)
					suma += 300;

				if(i < 3 && j < 1 && estado.See_Casilla(i,j) == jugador2 && (estado.See_Casilla(i+1,j) == bomba || estado.See_Casilla(i+1,j+1) == bomba || estado.See_Casilla(i+1,j+2) == bomba
					|| estado.See_Casilla(i+1,j+3) == bomba || estado.See_Casilla(i+1,j+4) == bomba || estado.See_Casilla(i+1,j+5) == bomba || estado.See_Casilla(i+1,j+6) == bomba ) && estado.See_Casilla(i+2,j) == jugador2
				 && estado.See_Casilla(i+3,j) == jugador2  && estado.See_Casilla(i+4,j) == jugador2 && estado.See_Casilla(i+1,j) == jugador)
					suma += 500;

				if(i < 3 && j < 1 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j) == jugador2 && (estado.See_Casilla(i+2,j) == bomba || estado.See_Casilla(i+2,j+1) == bomba || estado.See_Casilla(i+2,j+2) == bomba ||
					estado.See_Casilla(i+2,j+3) == bomba || estado.See_Casilla(i+2,j+4) == bomba || estado.See_Casilla(i+2,j+5) == bomba 
					|| estado.See_Casilla(i+2,j+6) == bomba ) && estado.See_Casilla(i+3,j) == jugador2 && estado.See_Casilla(i+4,j) == jugador2 && estado.See_Casilla(i+2,j) == jugador)
					suma += 500;

				if(i < 3 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j) == jugador2 && estado.See_Casilla(i+2,j) == jugador2 && (estado.See_Casilla(i+3,j) == bomba ||
					estado.See_Casilla(i+3,j+1) == bomba || estado.See_Casilla(i+3,j+2) == bomba || estado.See_Casilla(i+3,j+3) == bomba || estado.See_Casilla(i+3,j+4) == bomba ||
					estado.See_Casilla(i+3,j+5) == bomba || estado.See_Casilla(i+3,j+6) == bomba) && estado.See_Casilla(i+4,j) == jugador2 && estado.See_Casilla(i+3,j) == jugador)
					suma += 500;



				//comprobaciones en diagonal

				
				// la diagonal avanza para la derecha
				if(j >= 1 && i < 6 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j-1) == jugador && (estado.See_Casilla(i+1,j-1) == bomba || estado.See_Casilla(i,j) == bomba) )
					suma += 10;
				if(j >= 2 && i < 5 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j-1) == jugador && estado.See_Casilla(i+2,j-2) == jugador && (estado.See_Casilla(i+2,j-2) == bomba
					|| estado.See_Casilla(i+1,j-1) == bomba || estado.See_Casilla(i,j) == bomba)) 
					suma += 400;
				//if(j >= 3 && i < 4 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j-1) == jugador && estado.See_Casilla(i+2,j-2) == jugador && estado.See_Casilla(i+3,j-3) == bomba )
				//	suma += 400;

				// la diagonal avanza para la izquierda
				if(i < 6 && j < 6 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j+1) == jugador &&  (estado.See_Casilla(i+1,j+1) == bomba || estado.See_Casilla(i,j) == bomba) )
					suma += 10;
				if(i < 5 && j < 5 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j+1) == jugador && estado.See_Casilla(i+2,j+2) == jugador &&
					(estado.See_Casilla(i,j) == bomba || estado.See_Casilla(i+1,j+1) == bomba || estado.See_Casilla(i+2,j+2) == bomba)) 
					suma += 400;
				//if(i < 4 && j < 4 && estado.See_Casilla(i,j) == jugador && estado.See_Casilla(i+1,j+1) == jugador && estado.See_Casilla(i+2,j+2) == jugador && estado.See_Casilla(i+3,j+3) == bomba)
				//	suma += 4

				// comprobaciones para ver las posiciones de mi contrincante
				// en vertical
				if(i < 5 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j) == jugador2)
					suma += 10;	

				if(i < 4 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j) == jugador2 && estado.See_Casilla(i+2,j) == jugador2)
					suma += 300;

				// en horizontal
				if(j > 1 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i,j-1) == jugador2)
					suma += 10;

				if(j > 2 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i,j-1) == jugador2 && estado.See_Casilla(i,j) == jugador2)
					suma += 300;

				// en diagonal
				// la diagonal avanza para la derecha
				if(j >= 1 && i < 6 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j-1) == jugador2 ) 
					suma += 10;
				if(j >= 2 && i < 5 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j-1) == jugador2 && estado.See_Casilla(i+2,j-2) == jugador2) 
					suma += 300;
				// la diagonal avanza hacia la izquierda
				if(i < 6 && j < 6 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j+1) == jugador2) 
					suma += 10;
				if(i < 5 && j < 5 && estado.See_Casilla(i,j) == jugador2 && estado.See_Casilla(i+1,j+1) == jugador2 && estado.See_Casilla(i+2,j+2) == jugador2) 
					suma += 300;

	    }
	}

	return suma;
}


// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
// En este caso obtenemos la valoración restando según el jugador que nos pertenezca
double Valoracion(const Environment &estado, int jugador){
	if(jugador == 1){
		return Valoracion1(estado) - Valoracion2(estado);
	}

	else
		return Valoracion2(estado) - Valoracion1(estado);
}

// funcion min-max con poda alpha-beta y profundidad limintada
double Player::minimax(const Environment &T,int jugador, int & accion, int prof,double a,double b){

	if(T.JuegoTerminado())
		return ValoracionTest(T, jugador);

	else if(prof == 0) // cuando es un nodo hoja, implementamos mi valoración
		return Valoracion(T, jugador);

	else{

		if(prof % 2 == 0){

			int ultacc = -1;
			int kk;
			double valor;
			//int salir = false;
			//int max = menosinf;
			Environment hijo = T.GenerateNextMove(ultacc);
			while(ultacc < 8){

				valor = minimax(hijo, jugador, kk, prof - 1, a, b);
				if(valor > a){
					a = valor;
					accion = ultacc;
				}
				if(a >= b) { // con esto sale de la funcion y por tanto se produce la poda porque no va a continuar el while
					return b;
				}

				hijo = T.GenerateNextMove(ultacc);
			}

			return a;

		}

		else{


			int ultacc = -1;
			int kk;
			double valor;
			//int salir = false;
			//int max = menosinf;
			Environment hijo = T.GenerateNextMove(ultacc);
			while(ultacc < 8){

				valor = minimax(hijo, jugador, kk, prof - 1, a, b);
				if(valor < b){
					b = valor;
					accion = ultacc;
				}
				if(a >= b) { // con esto sale de la funcion y por tanto se produce la poda porque no va a continuar el while
					return a;
				}

				hijo = T.GenerateNextMove(ultacc);
			}

			return b;
		}

	}



}

// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
/*
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}

*/




// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta
 
    Environment::ActionType accion; // acción que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha, beta; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    //actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;


    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------
    alpha = menosinf;
    beta = masinf;
    int accion1 = -1;

   valor = minimax(actual_,jugador_, accion1, PROFUNDIDAD_ALFABETA,alpha,beta);

    accion = static_cast< Environment::ActionType > (accion1);

     cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;
    return accion;
}
