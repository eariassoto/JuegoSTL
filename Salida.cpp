#include "Salida.h"
Salida::Salida()
{
}
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

bool Salida::hayJuegoGuardado()
{
    ifstream input;
    input.open("juego.wtf");
    bool b = input.is_open();
    input.close();
    return b;
}

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

void Salida::borrarJuego(){
    remove( "juego.wtf" );
}
