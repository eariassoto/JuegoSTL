#include "Salida.h"
Salida::Salida()
{
}


/** \brief Guarda la informacion necesaria del juego
 *
 * \param tam int tamano del tablero
 * \param turno Grafo::Jugador::Turno el ultimo turno del jugador
 * \param posJ pair<int, int> par con la posicion del jugador
 * \param posM pair<int, int> par con la posicion de meta
 * \return void
 *
 */
void Salida::guardarJuego(int tam, Grafo::Jugador::Turno turno, pair<int, int> posJ, pair<int, int> posM)
{
    ofstream output;
    output.open ("juego.wtf");
    output << tam << "\n";
    output << turno << "\n";
    output << posJ.first << "\n";
    output << posJ.second << "\n";
    output << posM.first << "\n";
    output << posM.second << "\n";
    output.close();
}

/** \brief Consulta si si hay un juego guardado previamente
 *
 * \return bool True si lo encuentra
 *
 */
bool Salida::hayJuegoGuardado()
{
    ifstream input;
    input.open("juego.wtf");
    bool b = input.is_open();
    input.close();
    return b;
}

/** \brief Lee una linea de un archivo
 *
 * \param n int linea l que se quiere leer
 * \return int contenido de la linea
 *
 */
int Salida::leerLinea(int n)
{
    string hilera;
    ifstream input ("juego.wtf");
    if (input.is_open())
    {
        for(int i = 0; i < n; i++)
        {
            getline(input, hilera);
        }
        input.close();
    }
    return atoi(hilera.c_str());
}

int Salida::getCantidadCuadros()
{
    return leerLinea(1);
}

Grafo::Jugador::Turno Salida::getTurno()
{
    return (Grafo::Jugador::Turno)leerLinea(2);
}

pair<int, int> Salida::getPosJugador()
{
    pair<int, int> pos;
    pos.first = leerLinea(3);
    pos.second = leerLinea(4);
    return pos;
}

pair<int, int> Salida::getPosMeta()
{
    pair<int, int> pos;
    pos.first = leerLinea(5);
    pos.second = leerLinea(6);
    return pos;
}

/** \brief Borra el juego anterior, si lo hay.
 *
 * \return void
 *
 */
void Salida::borrarJuego()
{
    remove( "juego.wtf" );
}
