/** \class Grafo
 * \brief Representa el tablero de juego mediantes las clases estandan map y vector.
 *
 * \version 1.0
 * \author Emmanuel Arias
 *
 */
#ifndef GRAFO_H_
#define GRAFO_H_
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <vector>
using namespace std;
class Grafo
{
public:

    Grafo(int);
    ~Grafo();

    /** \class Jugador
     * \brief Representa a los jugadores, contiene un par donde esta su ficha
     *
     * \version 1.0
     * \author Emmanuel Arias
     *
     */
    class Jugador
    {
    public:
        Jugador(pair<int, int>);

        bool gano;
        bool empate;
        enum Turno {JUGADOR1, JUGADOR2};
        Turno turno;
        pair<int, int> posicion;

        int getPosicion(char);
        void cambiarTurno();
    };

    Jugador * jugador;
    pair<int, int> meta;

    void moverJugador(int*);
    bool finDelJuego();
    void imprimir();
    map<pair<int, int>, vector<pair<int, int> > >::iterator buscarNodo(int coordX, int coordY);
    bool buscarArista(map<pair<int, int>, vector<pair<int, int> > >::iterator, int, int);

private:
    map<pair<int, int>, vector<pair<int, int> > > r;
    void asociarMatriz(int);
    pair<int, int> asociarFicha(bool, int, int);
};
#endif /* GRAFO_H_ */
