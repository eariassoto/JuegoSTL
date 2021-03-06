#include "Grafo.h"

Grafo::Grafo(int n)
{
    this->asociarMatriz(n);
    int mitad = floor((n-1)/2);
    jugador = new Jugador(asociarFicha(true, 0, mitad));
    meta = asociarFicha(false, mitad, n);
}

Grafo::Grafo(int n, Grafo::Jugador::Turno t, pair<int, int> j, pair<int, int> m){
    this->asociarMatriz(n);
    int mitad = floor((n-1)/2);
    jugador = new Jugador(t, j);
    meta = m;
}

Grafo::Jugador::Jugador(Grafo::Jugador::Turno t, pair<int, int> n)
{
    this->posicion = n;
    this->turno = t;
    this->gano = false;
    this->empate = false;
}

Grafo::Jugador::Jugador(pair<int, int> n)
{
    this->posicion = n;
    this->turno = JUGADOR1;
    this->gano = false;
    this->empate = false;
}

Grafo::~Grafo()
{
    delete jugador;
}

/** \brief Asocia una ficha con un pair, lo que hace es asignar al azar un lugar para el inicio y la meta
 *
 * \param esJugador bool especifica si es la ficha de inicio o la de fin
 * \param min int rango minimo
 * \param max int rango maximo
 * \return pair<int, int> pair correspondiente al nodo del tablero
 *
 */
pair<int, int> Grafo::asociarFicha(bool esJugador, int min, int max)
{
    int r;
    srand (time(NULL));
    if(esJugador)
    {
        r = rand() % max;
        return make_pair(0, r);
    }
    else
    {
        r = rand() % min + (max-min);
        return make_pair(max-1, r);
    }
}


/** \brief Devuelve uno de los ejes del nodo correspondiente a la posicion actual del usuario
*
* \param eje char identificador del eje a devolver
* \return int entero correspondiente a la posicion actual de jugador
*/
int Grafo::Jugador::getPosicion(char eje)
{
    int pos = 0;
    if(eje == 'x')
        pos = posicion.first;
    else if(eje == 'y')
        pos = posicion.second;
    return pos;
}

/** \brief cambia el turno entre ambos jugadores
*
*/
void Grafo::Jugador::cambiarTurno()
{
    turno = (turno==JUGADOR1)?JUGADOR2:JUGADOR1;
}

/** \brief Busca en el map un nodo
*
* \param coordX int coordenada x a buscar
* \param coordY int coordenada y a buscar
* \return map<pair<int, int>, vector<pair<int, int> > >::iterator iterador que contiene el nodo
*/
map<pair<int, int>, vector<pair<int, int> > >::iterator Grafo::buscarNodo(int coordX, int coordY)
{
    map<pair<int, int>, vector<pair<int, int> > >::iterator it = r.begin();
    pair<int, int> buscar = make_pair(coordX, coordY);
    bool encontrado = false;
    while(!encontrado)
    {
        if(it->first == buscar)
        {
            encontrado = true;
        }
        else if(it == r.end())
        {
            encontrado = true;
        }
        else
        {
            it++;
        }
    }
    return it;
}


/** \brief Busca en el vector del nodo una arista
 *
 * \param nodoIt map<pair<int, int>, vector<pair<int, int> > >::iterator iterador del nodo
 * \param coordX int coord x de la arista
 * \param coordY int coord y de la arista
 * \return bool true si lo encontro
 *
 */
bool Grafo::buscarArista(map<pair<int, int>, vector<pair<int, int> > >::iterator nodoIt, int coordX, int coordY)
{
    vector<pair<int, int> > v = nodoIt->second;
    pair<int, int> buscar = make_pair(coordX, coordY);
    vector<pair<int, int> >::iterator it = v.begin();
    bool encontrado = false;
    while(it != v.end())
    {
        if(*it == buscar)
        {
            encontrado = true;
            it == v.end();
        }
            it++;
    }
    return encontrado;
}

/** \brief Crea el tablero mediante las clases estandar
*
* \param n int tama;o del tablero
*
*/
void Grafo::asociarMatriz(int n)
{
    for(int j = 0; j < n; j++)
    {
        for(int i = 0; i < n; i++)
        {
            /// Crea el nodo
            pair<int, int> nodo(i, j);
            vector<pair<int, int> >listaAristas;

            /// Estos tres if buscan posibles aristas del nodo y las agregan
            if(j < (n-1))
            {
                listaAristas.push_back(make_pair(i, (j+1)));
                //cout << "agregue arista a la derecha" << endl;
                if(1 < (n-1))
                {
                    //cout << "agregue arista diagonal" << endl;
                    listaAristas.push_back(make_pair((i+1), (j+1)));
                }
            }
            if(i < (n-1))
            {
                //cout << "agregue arista abajo" << endl;
                listaAristas.push_back(make_pair((i+1), j));
            }
            r.insert( pair<pair<int, int>, vector<pair<int, int> > >(nodo, listaAristas) );
        }
    }
}

/** \brief Campia el pair de la ficha del jugador. Valida si hay un gane o empate y si no cambia de turno
*
* \param coord int* puntero con la posicion a mover la ficha
*
*/
void Grafo::moverJugador(int * coord)
{
    jugador->posicion =  buscarNodo(coord[0], coord[1])->first;

    if(jugador->posicion == meta)
    {
        jugador->gano = true;
    }
    else
    {
        if(jugador->getPosicion('y') > (meta.second) && jugador->getPosicion('x') == (meta.first-1))
        {
            jugador->empate = true;
        }
        else{
            jugador->cambiarTurno();
        }
    }
}

/** \brief Consulta si el juego ha terminado
*
* \return bool true si ya hay un gane o un empate
*/
bool Grafo::finDelJuego()
{
    return jugador->gano || jugador->empate;
}

/** \brief Itera para imprimir el grafo
 *
 * \return void
 *
 */
void Grafo::imprimir()
{
    map<pair<int, int>, vector<pair<int, int> > >::iterator it = r.begin();
    for(it; it!=r.end(); it++){
        cout << "Nodo: " << it->first.first << " , " << it->first.second;
    }
}
