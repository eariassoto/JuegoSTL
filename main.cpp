#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include "Grafo.h"
#include "Interfaz.h"

using namespace std;

/** \brief Espera una cantidad de segundos
 *
 * \param segundos int segundos para que programa espere
 *
 */
void esperar(int segundos)
{
    clock_t finEspera;
    finEspera = clock () + segundos * CLOCKS_PER_SEC ;
    while (clock() < finEspera) {}
}

/** \brief Controlador del programa.
 *
 * \return int
 *
 */
int main()
{
    /// Se crea la ventana y se esconde.
    sf::RenderWindow ventana(sf::VideoMode(600, 600), "Meta");
    ventana.setFramerateLimit(30);
    ventana.setVisible(false);

    /// Instancia de interfaz
    Interfaz * interfaz = new Interfaz(ventana);

    /// Se muestra la ventana inicial y se espera a la decision del usuario
    int cantCuadros = interfaz->iniciarPantallaInicio();

    /// Instancia del grafo
    Grafo * grafo = new Grafo(cantCuadros);

    /// Se le manda a interfaz los punteros que necesita e inicia el tablero de juego.
    interfaz->setElementosGrafo(grafo, grafo->meta);
    interfaz->iniciarPantallaTablero();

    bool ventanaCerrada = (ventana.isOpen())?false:true;

    /// Loop principal del juego
    while (!grafo->finDelJuego() && !ventanaCerrada)
    {
        sf::Event event;
        while (ventana.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                ventanaCerrada = true;
                ventana.close();
                break;
            case sf::Event::Resized:
                ventana.setSize(sf::Vector2u(600, 600));
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if(interfaz->puedeMover(event.mouseButton.x, event.mouseButton.y))
                        grafo->moverJugador(interfaz->getCoordNodo(event.mouseButton.x, event.mouseButton.y));
                }
                break;
            case sf::Event::MouseMoved:
                interfaz->setPosicionMouse(event.mouseMove.x, event.mouseMove.y);
                break;
            default:
                break;
            }
        }

        interfaz->dibujarPantallaTablero();
        ventana.display();

        if(grafo->finDelJuego())
        {
            esperar(1);
            interfaz->iniciarPantallaFinal();
        }
    }
    return 0;
}
