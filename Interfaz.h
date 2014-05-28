/** \class Interfaz
 * \brief Muestra al usuario las ventanas para que pueda jugar.

 * \version 1.0
 * \author Emmanuel Arias
 *
 */
#ifndef INTERFAZ_H_
#define INTERFAZ_H_
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "Grafo.h"

using namespace std;

class Interfaz
{
public:

    Interfaz(sf::RenderWindow&);
    ~Interfaz();

    /** \class Cuadro
     * \brief Mantiene un arbol con un contenedor para las instancias de sf::RectangleShape que son los cuadros del tablero.

     * \version 0.1
     * \author Emmanuel Arias, Geovanny Zu;iga
     *
     */
    class Cuadro
    {
    public:
        Cuadro();
        Cuadro(int, int, sf::RectangleShape);
        ~Cuadro();

        int                coordX;
        int                coordY;
        bool               usado;
        Cuadro *           hijoIzq;
        Cuadro *           hijoDer;
        sf::RectangleShape rectangulo;

        void     agregarCuadro(Cuadro *);
        void     dibujarCuadros(sf::RenderWindow&);
        Cuadro * buscarCuadro(int, int);
        int *    buscarPixelEnCuadro(int, int);
    };

    int    iniciarPantallaInicio();
    void   iniciarPantallaFinal();
    void   iniciarPantallaTablero();
    void   dibujarPantallaTablero();
    void   setElementosGrafo(Grafo *, pair<int, int>);
    void   setPosicionMouse(int, int);
    bool   puedeMover(int, int);
    int *  getCoordNodo(int, int);

private:
    int               cantCuadros;
    int               mousex;
    int               mousey;
    sf::RenderWindow& ventana;
    sf::Font          fuente;
    Cuadro *          raiz;
    Grafo::Jugador*   jugador;
    Grafo *           grafo;
    pair<int, int>    meta;

    int *     calcularEsquinaCuadro(int, int);
    int       calcularEscalaCirculo();
    int *     calcularCoordEscala(int, int);
    sf::Color getColorCuadro(int, int);
    sf::Color getColorFicha(bool);
    void      dibujarFicha(int, int, sf::Color);
    void      dibujarFichas();
    void      dibujarTexto(string, int, int);
    void      dibujarPantallaInicio();
    void      dibujarPantallaFinal();
    std::string itos(int);
};

#endif

