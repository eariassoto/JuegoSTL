/** \class Salida
 * \brief Se encarga de manejar los juegos guardados

 * \version 1.0
 * \author Emmanuel Arias
 *
 */
 #ifndef _SALIDA_H_
#define _SALIDA_H_
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include "Grafo.h"

using namespace std;

class Salida{
public:
    Salida();
    void guardarJuego(int, Grafo::Jugador::Turno, pair<int, int>, pair<int, int>);
    bool hayJuegoGuardado();
    int getCantidadCuadros();
    pair<int, int> getPosJugador();
    pair<int, int> getPosMeta();
    Grafo::Jugador::Turno getTurno();
    void borrarJuego();
private:
    int leerLinea(int);

};

#endif // _SALIDA_H_
