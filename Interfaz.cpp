#include "Interfaz.h"
#define MIN 5
#define MAX 20

Interfaz::Cuadro::Cuadro(int coordX, int coordY, sf::RectangleShape rectangulo)
{
    this->rectangulo = rectangulo;
    this->coordX = coordX;
    this->coordY = coordY;
    usado = true;
    hijoIzq = 0;
    hijoDer = 0;
}

Interfaz::Cuadro::Cuadro()
{
    coordX = 0;
    coordY = 0;
    usado = false;
    hijoIzq = 0;
    hijoDer = 0;
}

Interfaz::Cuadro::~Cuadro()
{
    if(hijoDer)
        delete hijoDer;
    if(hijoIzq)
        delete hijoIzq;
}

Interfaz::Interfaz(sf::RenderWindow& ven): ventana(ven)
{
    this->mousex = -1;
    this->mousey = -1;
    this->cantCuadros = 5;

    raiz = new Cuadro();

    if (!fuente.loadFromFile("PWPerspective.ttf"))
        cout << "error al cargar la fuente" << endl;
}

Interfaz::~Interfaz()
{
    delete grafo;
    delete raiz;
}

/** \brief Agrega un nodo al arbol de cuadros
*
* \param cuadro Cuadro* Instancia de Codo que se debe agregar al arbol
*
*/
void Interfaz::Cuadro::agregarCuadro(Cuadro * cuadro)
{
    if (!usado) // caso cabeza vacio
    {
        this->coordX = cuadro->coordX;
        this->coordY = cuadro->coordY;
        this->rectangulo = cuadro->rectangulo;
        usado = true;
    }
    else
    {
        if(cuadro->coordX < this->coordX) //x menor
        {
            if(!this->hijoIzq)
                hijoIzq = cuadro;
            else
                this->hijoIzq->agregarCuadro(cuadro);
        }
        else
        {
            if(cuadro->coordX == this->coordX) // x iguales
            {
                if(cuadro->coordY < this->coordY) // y menor
                {
                    if(!this->hijoIzq)
                        hijoIzq = cuadro;
                    else
                        this->hijoIzq->agregarCuadro(cuadro);
                }
                else
                {
                    if(!this->hijoDer)
                        hijoDer = cuadro;
                    else
                        this->hijoDer->agregarCuadro(cuadro);
                }
            }
            else
            {
                if(!this->hijoDer)
                    hijoDer = cuadro;
                else
                    this->hijoDer->agregarCuadro(cuadro);
            }
        }
    }
}

/** \brief Busca en el arbol un cuadro
*
* \param coordX int coordenada x a buscar
* \param coordY int coordenada y a buscar
* \return Interfaz::Cuadro* si se encontro retorna el puntero al nodo, de lo contrario 0
*/
Interfaz::Cuadro * Interfaz::Cuadro::buscarCuadro(int coordX, int coordY)
{
    Cuadro * resultado = 0;
    if(this->coordX == coordX)
    {
        if(this->coordY == coordY)
            resultado = this;
        else
        {
            if(coordY < this->coordY && hijoIzq)
                resultado = hijoIzq->buscarCuadro(coordX, coordY);
            else if(coordY > this->coordY && hijoDer)
                resultado = hijoDer->buscarCuadro(coordX, coordY);
        }
    }
    else
    {
        if(coordX < this->coordX && hijoIzq)
            resultado = hijoIzq->buscarCuadro(coordX, coordY);
        else if(coordX > this->coordX && hijoDer)
            resultado = hijoDer->buscarCuadro(coordX, coordY);
    }
    return resultado;
}

/** \brief De acuerdo a unas coordenas busca el cuadro que contenga ese punto
 *
 * \param x int coordenada x
 * \param y int coordenada y
 * \return int* puntero con las coordenadas del cuadro que contiene ese punto
 *
 */
int * Interfaz::Cuadro::buscarPixelEnCuadro(int x, int y)
{
    int * coord = new int[2];
    coord[0] = -1;
    if(this->rectangulo.getGlobalBounds().contains(x, y))
    {
        coord[0] = this->coordX;
        coord[1] = this->coordY;
    }
    else
    {
        if(hijoIzq && coord[0] == -1)
            coord = hijoIzq->buscarPixelEnCuadro(x, y);
        if(hijoDer && coord[0] == -1)
            coord = hijoDer->buscarPixelEnCuadro(x, y);
    }
    return coord;
}

/** \brief Dibuja recursivamente los cuadros del tablero
 *
 * \param ventana sf::RenderWindow&  ventana donde se va a dibujar
 *
 */
void Interfaz::Cuadro::dibujarCuadros(sf::RenderWindow& ventana)
{
    ventana.draw(this->rectangulo);
    if(this->hijoIzq)
        hijoIzq->dibujarCuadros(ventana);
    if(this->hijoDer)
        hijoDer->dibujarCuadros(ventana);
}

/** \brief Pasa los punteros a las fichas del tablero.
 *
 * \param jugador Puntero a la instancia del jugador, que contiene, entre otras cosas la ficha.
 * \param meta La ficha de la meta.
 *
 */
void Interfaz::setElementosGrafo(Grafo * grafo, pair<int, int> meta)
{
    this->grafo = grafo;
    this->jugador = grafo->jugador;
    this->meta = meta;
}

/** \brief Cambia las coordenadas actuales del mouse
 *
 * \param mousex int coordenada x
 * \param mousey int coordenada y
 *
 */
void Interfaz::setPosicionMouse(int mousex, int mousey)
{
    this->mousex = mousex;
    this->mousey = mousey;
}

/** \brief Calcula la coordenada de la esquina superior izquierda del cuadro
 *
 * \param coordX int coordenada x del cuadro
 * \param coordY int coordenada y del cuadro
 * \return int* puntero con las coordenadas
 *
 */
int * Interfaz::calcularEsquinaCuadro(int coordX, int coordY)
{
    int * coord = new int[2];
    Interfaz::Cuadro * cuadro = raiz->buscarCuadro(coordX, coordY);
    coord[0] = cuadro->rectangulo.getGlobalBounds().left;
    coord[1] = cuadro->rectangulo.getGlobalBounds().top;
    return coord;
}

/** \brief Calcula el tama;o en escala del circulo, basicamente la diagonal de cuadrado
 *
 * \return int tama;o en el que se debe dibujar la ficha
 *
 */
int Interfaz::calcularEscalaCirculo()
{
    return (raiz->buscarCuadro(0, 0)->rectangulo.getGlobalBounds().height/2) * sqrt(2);
}

/** \brief Calcula la coordenada en la que se debe colocar la ficha
*
* \return int* puntero con las coordenadas para dibujar
*
*/
int * Interfaz::calcularCoordEscala(int coordX, int coordY)
{
    int * coord = calcularEsquinaCuadro(coordX, coordY);
    int escala = this->calcularEscalaCirculo();
    coord[0] += ((raiz->buscarCuadro(0, 0)->rectangulo.getGlobalBounds().height)-escala)/2;
    coord[1] += ((raiz->buscarCuadro(0, 0)->rectangulo.getGlobalBounds().height)-escala)/2;
    return coord;
}

/** \brief Calcula el color respectivo del cuadro de acuerdo a sus coordenadas
 *
 * \param coordX int coordenada x
 * \param coordY int coordenada y
 * \return sf::Color color a utilizar
 *
 */
sf::Color Interfaz::getColorCuadro(int coordX, int coordY)
{
    if (coordX % 2 == 0)
    {
        if (coordY % 2 == 0)
            return sf::Color(0,54,107);
        else
            return sf::Color(0,148,255);
    }
    else
    {
        if (coordY % 2 == 0)
            return sf::Color(0,148,255);
        else
            return sf::Color(0,54,107);
    }
}

/** \brief Devuelve los colores de la fichas
 *
 * \param esJugador bool Indica si la ficha es del jugador o si es la meta
 * \return sf::Color color de la ficha
 *
 */
sf::Color Interfaz::getColorFicha(bool esJugador)
{
    return (esJugador)?sf::Color(255, 0, 0):sf::Color(0, 255, 0);
}

/** \brief Dibuja una hilera de texto en la ventana
 *
 * \param msj string
 * \param coordX int coordenada x
 * \param coordY int coordenada y
 *
 */
void Interfaz::dibujarTexto(string msj, int coordX, int coordY)
{
    sf::Text texto;
    texto.setFont(fuente);
    texto.setColor(sf::Color::Black);
    texto.setString(msj);
    texto.setCharacterSize(this->calcularEscalaCirculo());
    texto.setPosition(coordX,coordY);
    ventana.draw(texto);
}

/** \brief Dibuja una ficha en la ventana
 *
 * \param coordX int coordenada x
 * \param coordY int coordenada y
 * \param color sf::Color color para pintar
 *
 */
void Interfaz::dibujarFicha(int coordX, int coordY, sf::Color color)
{
    sf::CircleShape ficha(floor(ventana.getSize().x/this->cantCuadros)/2);
    int * coord = calcularEsquinaCuadro(coordX, coordY);
    ficha.setFillColor(color);
    ficha.setPosition(coord[0], coord[1]);
    ventana.draw(ficha);
    delete coord;
}

/** \brief Dibuja las fichas del tablero con sus respectivos textos
 *
 */
void Interfaz::dibujarFichas()
{
    dibujarFicha(meta.first, meta.second, this->getColorFicha(false));
    int * coord = calcularCoordEscala(meta.first, meta.second);
    dibujarTexto("M", coord[0], coord[1]);

    dibujarFicha(jugador->getPosicion('x'), jugador->getPosicion('y'), this->getColorFicha(true));
    coord = calcularEsquinaCuadro(jugador->getPosicion('x'), jugador->getPosicion('y'));

    string textoFicha;
    if(jugador->turno == Grafo::Jugador::JUGADOR1)
        textoFicha = "J1";
    else
        textoFicha = "J2";
    dibujarTexto(textoFicha, coord[0]+3, coord[1]);

    if(mousex > -1)
    {
        coord = raiz->buscarPixelEnCuadro(mousex, mousey);
        if(grafo->buscarArista(grafo->buscarNodo(jugador->getPosicion('x'), jugador->getPosicion('y')),coord[0], coord[1]))
        {
            sf::Color c = this->getColorFicha(true);
            dibujarFicha(coord[0], coord[1], sf::Color(c.r, c.g, c.b, 125));
        }
    }
    delete coord;
}

/** \brief Inicia dibujando en la ventana la pantalla inicial. Le pide al usuario la cantidad de cuadros que quiere en su tablero
 *
 * \return int Cantidad de cuadros solicitados por el usuario
 *
 */
int Interfaz::iniciarPantallaInicio()
{
    bool decision = false;
    ventana.setVisible(true);
    this->dibujarPantallaInicio();
    while (!decision)
    {
        sf::Event event;
        while (ventana.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                ventana.close();
                decision = true;
                break;
            case sf::Event::Resized:
                ventana.setSize(sf::Vector2u(600, 600));
                break;
            case sf::Event::KeyPressed:
                if(event.key.code == sf::Keyboard::Return)
                {
                    decision = true;
                }
                else if(event.key.code == sf::Keyboard::Up)
                {
                    if(cantCuadros < MAX)
                    {
                        this->cantCuadros++;
                    }
                }
                else if(event.key.code == sf::Keyboard::Down)
                {
                    if(cantCuadros > MIN)
                    {
                        this->cantCuadros--;
                    }
                }
                break;
            default:
                break;
            }
            this->dibujarPantallaInicio();

            ventana.display();
        }
    }
    return this->cantCuadros;
}

/** \brief Inicia dibujando en la ventana la pantalla inicial. Le pide al usuario la cantidad de cuadros que quiere en su tablero
 *
 * \return int Cantidad de cuadros solicitados por el usuario
 *
 */
bool Interfaz::iniciarPantallaSesion()
{
    bool decision = false, continuar = false;
    ventana.setVisible(true);
    this->dibujarPantallaSesion(continuar);
    while (!decision)
    {
        sf::Event event;
        while (ventana.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                ventana.close();
                decision = true;
                break;
            case sf::Event::Resized:
                ventana.setSize(sf::Vector2u(600, 600));
                break;
            case sf::Event::KeyPressed:
                if(event.key.code == sf::Keyboard::Return)
                {
                    decision = true;
                }
                else if(event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down)
                {
                    continuar = !continuar;
                }

                break;
            default:
                break;
            }
            this->dibujarPantallaSesion(continuar);

            ventana.display();
        }
    }
    return this->cantCuadros;
}

/** \brief Hace los calculos de tama;os entre otros y crea el arbol que contiene los cuadros del tablero.
 *
 */
void Interfaz::iniciarPantallaTablero()
{
    int tamCuadro = floor(ventana.getSize().x/this->cantCuadros);
    int tamVentana = tamCuadro*this->cantCuadros;
    ventana.setSize(sf::Vector2u(tamVentana, tamVentana));

    for(int j = 0; j < this->cantCuadros; j++)
    {
        for(int i = 0; i < this->cantCuadros; i++)
        {
            int * coord = new int[2];
            coord[0] = j * tamCuadro;
            coord[1] = i * tamCuadro;
            sf::RectangleShape rectangulo(sf::Vector2f(tamCuadro, tamCuadro));
            rectangulo.setFillColor(this->getColorCuadro(i, j));
            rectangulo.setPosition(coord[0], coord[1]);
            delete coord;

            Cuadro * cuadro = new Cuadro(i, j, rectangulo);
            raiz->agregarCuadro(cuadro);

        }
    }

    this->dibujarPantallaTablero();
}

/** \brief Dibuja la pantalla que se muestra luego de que el juego haya terminado.
 *
 */
void Interfaz::iniciarPantallaFinal()
{
    this->dibujarPantallaFinal();

    while (ventana.isOpen())
    {
        sf::Event event;
        while (ventana.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                ventana.close();
            case sf::Event::Resized:
                ventana.setSize(sf::Vector2u(600, 600));
                break;
            default:
                break;
            case sf::Event::KeyPressed:
                ventana.close();
                break;
            }
            this->dibujarPantallaFinal();

            ventana.display();
        }
    }
}

/** \brief Dibuja en la ventana los componentes referentes a la pantalla de inicio.
 *
 */
void Interfaz::dibujarPantallaInicio()
{
    ventana.clear(sf::Color(0,148,255));
    sf::Text titulo;
    sf::Text instrucciones;
    sf::Text cantCuadros;
    sf::Text textoInicio;

    titulo.setFont(fuente);
    instrucciones.setFont(fuente);
    cantCuadros.setFont(fuente);
    textoInicio.setFont(fuente);

    titulo.setColor(sf::Color::Black);
    instrucciones.setColor(sf::Color::Black);
    cantCuadros.setColor(sf::Color::Black);
    textoInicio.setColor(sf::Color::Black);

    titulo.setString("META");
    instrucciones.setString("Selecciona con las flechas la\ncantidad  de cuadros para el\n                        tablero");
    cantCuadros.setString(itos(this->cantCuadros));
    textoInicio.setString("Presiona Enter para iniciar");

    titulo.setCharacterSize(floor(ventana.getSize().x * 0.3));
    instrucciones.setCharacterSize(floor(ventana.getSize().x * 0.05));
    cantCuadros.setCharacterSize(floor(ventana.getSize().x * 0.35));
    textoInicio.setCharacterSize(floor(ventana.getSize().x * 0.05));

    titulo.setPosition(0,0);
    instrucciones.setPosition(0,(ventana.getSize().x * 0.375));
    cantCuadros.setPosition(floor(floor(ventana.getSize().x - cantCuadros.getGlobalBounds().width) / 2), (ventana.getSize().x * 0.475));
    textoInicio.setPosition(floor(floor(ventana.getSize().x - textoInicio.getGlobalBounds().width) / 2), (ventana.getSize().x * 0.875));

    ventana.draw(titulo);
    ventana.draw(instrucciones);
    ventana.draw(cantCuadros);
    ventana.draw(textoInicio);
}

/** \brief Dibuja en la ventana los componentes referentes a la pantalla de inicio.
 *
 */
void Interfaz::dibujarPantallaSesion(bool)
{
    ventana.clear(sf::Color(0,148,255));
    sf::Text titulo;
    sf::Text instrucciones;
    sf::Text cantCuadros;
    sf::Text textoInicio;

    titulo.setFont(fuente);
    instrucciones.setFont(fuente);
    cantCuadros.setFont(fuente);
    textoInicio.setFont(fuente);

    titulo.setColor(sf::Color::Black);
    instrucciones.setColor(sf::Color::Black);
    cantCuadros.setColor(sf::Color::Black);
    textoInicio.setColor(sf::Color::Black);

    titulo.setString("META");
    instrucciones.setString("Seleccigxjhona con las flechas la\ncantidad  de cuadros para el\n                        tablero");
    cantCuadros.setString(itos(44));
    textoInicio.setString("Presiona Enter para iniciar");

    titulo.setCharacterSize(floor(ventana.getSize().x * 0.3));
    instrucciones.setCharacterSize(floor(ventana.getSize().x * 0.05));
    cantCuadros.setCharacterSize(floor(ventana.getSize().x * 0.35));
    textoInicio.setCharacterSize(floor(ventana.getSize().x * 0.05));

    titulo.setPosition(0,0);
    instrucciones.setPosition(0,(ventana.getSize().x * 0.375));
    cantCuadros.setPosition(floor(floor(ventana.getSize().x - cantCuadros.getGlobalBounds().width) / 2), (ventana.getSize().x * 0.475));
    textoInicio.setPosition(floor(floor(ventana.getSize().x - textoInicio.getGlobalBounds().width) / 2), (ventana.getSize().x * 0.875));

    ventana.draw(titulo);
    ventana.draw(instrucciones);
    ventana.draw(cantCuadros);
    ventana.draw(textoInicio);
}

/** \brief Dibuja en la ventana los componentes referentes a la pantalla del tablero.
 *
 */
void Interfaz::dibujarPantallaTablero()
{
    ventana.clear(sf::Color::White);
    raiz->dibujarCuadros(this->ventana);
    this->dibujarFichas();
}

/** \brief Dibuja en la ventana los componentes referentes a la pantalla final.
 *
 */
void Interfaz::dibujarPantallaFinal()
{
    ventana.clear(sf::Color(0,148,255));
    sf::Text mensajeGanador;
    sf::Text mensajeEnter;
    string mensaje;

    mensajeGanador.setFont(fuente);
    mensajeEnter.setFont(fuente);

    mensajeGanador.setColor(sf::Color::Black);
    mensajeEnter.setColor(sf::Color::Black);
    if(jugador->empate)
    {
        mensajeGanador.setCharacterSize(floor(ventana.getSize().x * 0.2));
        mensaje = "Empate";
    }
    else if(jugador->gano)
    {
        mensaje = "Ha ganado el ";
        if(jugador->turno == Grafo::Jugador::JUGADOR1)
        {
            mensaje += "\n   Jugador 1.\n  Felicidades";
        }
        else
        {
            mensaje += "\n   Jugador 2.\n   Felicidades";
        }
        mensajeGanador.setCharacterSize(floor(ventana.getSize().x * 0.1));
    }

    mensajeGanador.setString(mensaje);
    mensajeEnter.setString("Presiona cualquier\n  tecla para salir.");

    mensajeEnter.setCharacterSize(floor(ventana.getSize().x * 0.075));

    mensajeGanador.setPosition(floor(floor(ventana.getSize().x - mensajeGanador.getGlobalBounds().width) / 2),  (ventana.getSize().x * 0.1));
    mensajeEnter.setPosition(floor(floor(ventana.getSize().x - mensajeEnter.getGlobalBounds().width) / 2), (ventana.getSize().x * 0.5));

    ventana.draw(mensajeGanador);
    ventana.draw(mensajeEnter);
}

/** \brief Comprueba si el jugador puede moverse en determinada posicion del tablero
 *
 * \param mousex int coordenada x
 * \param mousey int coordenada y
 * \return bool true si el jugador se puede mover
 *
 */
bool Interfaz::puedeMover(int mousex, int mousey)
{
    bool b = false;
    int * coord = raiz->buscarPixelEnCuadro(mousex, mousey);

    if(grafo->buscarArista(grafo->buscarNodo(jugador->getPosicion('x'), jugador->getPosicion('y')),coord[0], coord[1]))
        b = true;
    delete coord;
    return b;
}

/** \brief Devuelve las coordenas del un nodo
 *
 * \param mousex int coordenada x del mouse
 * \param mousey int coordenada y del mouse
 * \return int* puntero con las coordenadas que se encontraron
 *
 */
int * Interfaz::getCoordNodo(int mousex, int mousey)
{
    return raiz->buscarPixelEnCuadro(mousex, mousey);
}

/** \brief Convierte enteros en hileras
 *
 * \param i int entero para convertir
 * \return std::string resultado
 *
 */
std::string Interfaz::itos(int i)
{
    std::stringstream ss;
    ss<<i;
    return ss.str();
}

