#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "LinearSearch.h" // Incluir el encabezado de Linear Search
#include "BinarySearch.h" // Incluir el encabezado de Binary Search
#include "mergeSort.h"

// Declaraciones de otras funciones
void binarySearch(sf::RenderWindow &window);
void mergeSort(sf::RenderWindow &window);
void insertionSort(sf::RenderWindow &window);
void quickSort(sf::RenderWindow &window);
void selectionSort(sf::RenderWindow &window);
void bubbleSort(sf::RenderWindow &window);
void stacks(sf::RenderWindow &window);
void queues(sf::RenderWindow &window);
void linkedLists(sf::RenderWindow &window);
void dijkstraVisualization(sf::RenderWindow &window);
void avlTree(sf::RenderWindow &window);
void graphs(sf::RenderWindow &window);

int main() {
    // Crear la ventana principal
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Menu Principal - Algoritmos y Estructuras de Datos");
    window.setFramerateLimit(60);

    // Configuración de fuente
    sf::Font font;
    if (!font.loadFromFile("GOTHIC.TTF")) { // Asegúrate de que la fuente GOTHIC.TTF esté en tu directorio
        return -1;
    }

    // Títulos y subtítulos del menú
    std::vector<std::string> sections = {
        "Menu de algoritmos",
        "Algoritmos de busqueda",
        "Algoritmos de ordenamiento",
        "Estructuras de datos lineales",
        "Estructuras de datos no lineales"
    };

    // Opciones del menú
    std::vector<std::string> menuOptions = {
        "Linear Search", "Binary Search",
        "Merge Sort", "Insertion Sort", "Quick Sort", "Selection Sort", "Bubble Sort",
        "Stacks", "Queues", "Linked lists",
        "Dijkstra", "AVL", "Grafos"
    };

    // Configuración de los textos
    std::vector<sf::Text> sectionTexts(sections.size());
    for (size_t i = 0; i < sections.size(); ++i) {
        sectionTexts[i].setFont(font);
        sectionTexts[i].setString(sections[i]);
        sectionTexts[i].setCharacterSize(i == 0 ? 36 : 30); // Título principal más grande
        sectionTexts[i].setFillColor(sf::Color::White);
        sectionTexts[i].setStyle(sf::Text::Bold);
    }

    std::vector<sf::Text> menuTexts(menuOptions.size());
    for (size_t i = 0; i < menuOptions.size(); ++i) {
        menuTexts[i].setFont(font);
        menuTexts[i].setString(menuOptions[i]);
        menuTexts[i].setCharacterSize(22); // Reducir tamaño del texto de las opciones
        menuTexts[i].setFillColor(sf::Color::White);
    }

    // Posiciones iniciales y espaciado ajustado
    float startX = 500; // Centrado horizontalmente
    float startY = 50;
    float sectionSpacing = 60; // Espaciado entre secciones
    float optionSpacing = 25; // Espaciado entre opciones
    float sectionToOptionsSpacing = 10; // Espaciado entre subtítulo y opciones

    // Ajustar las posiciones
    float currentY = startY;
    for (size_t i = 0; i < sections.size(); ++i) {
        sectionTexts[i].setPosition(startX - sectionTexts[i].getLocalBounds().width / 2, currentY);
        currentY += sectionSpacing;
        for (size_t j = 0; j < menuOptions.size(); ++j) {
            if ((i == 1 && j < 2) || (i == 2 && j >= 2 && j < 7) || (i == 3 && j >= 7 && j < 10) || (i == 4 && j >= 10)) {
                if (j == (i == 1 ? 0 : (i == 2 ? 2 : (i == 3 ? 7 : 10)))) {
                    currentY += sectionToOptionsSpacing; // Reducir espacio entre subtítulo y opciones
                }
                menuTexts[j].setPosition(startX - menuTexts[j].getLocalBounds().width / 2, currentY);
                currentY += optionSpacing;
            }
        }
    }

    // Índice para rastrear la opción seleccionada
    int selectedOption = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Manejo de eventos del teclado
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    // Mover selección hacia arriba
                    if (selectedOption > 0) {
                        selectedOption--;
                    }
                } else if (event.key.code == sf::Keyboard::Down) {
                    // Mover selección hacia abajo
                    if (selectedOption < static_cast<int>(menuOptions.size()) - 1) {
                        selectedOption++;
                    }
                } else if (event.key.code == sf::Keyboard::Enter) {
                    // Ejecutar acción para la opción seleccionada
                    if (menuOptions[selectedOption] == "Linear Search") {
                        linearSearch(window);
                    } else if (menuOptions[selectedOption] == "Binary Search") {
                        binarySearch(window);
                    } else if (menuOptions[selectedOption] == "Merge Sort") {
                        mergeSort(window);
                    } else if (menuOptions[selectedOption] == "Insertion Sort") {
                        insertionSort(window);
                    } else if (menuOptions[selectedOption] == "Quick Sort") {
                        quickSort(window);
                    } else if (menuOptions[selectedOption] == "Selection Sort") {
                        selectionSort(window);
                    } else if (menuOptions[selectedOption] == "Bubble Sort") {
                        bubbleSort(window);
                    } else if (menuOptions[selectedOption] == "Stacks") {
                        stacks(window);
                    } else if (menuOptions[selectedOption] == "Queues") {
                        queues(window);
                    } else if (menuOptions[selectedOption] == "Linked lists") {
                        linkedLists(window);
                    } else if (menuOptions[selectedOption] == "Dijkstra") {
                        dijkstraVisualization(window);
                    } else if (menuOptions[selectedOption] == "AVL") {
                        avlTree(window);
                    } else if (menuOptions[selectedOption] == "Grafos") {
                        graphs(window);
                    }
                }
            }
        }

        // Actualizar colores de las opciones del menú
        for (size_t i = 0; i < menuTexts.size(); ++i) {
            if (static_cast<int>(i) == selectedOption) {
                menuTexts[i].setFillColor(sf::Color::Yellow); // Resalta la opción seleccionada
            } else {
                menuTexts[i].setFillColor(sf::Color::White); // Opciones no seleccionadas en blanco
            }
        }

        // Dibujar en la ventana
        window.clear(sf::Color::Black); // Fondo negro
        for (const auto& text : sectionTexts) {
            window.draw(text);
        }
        for (const auto& text : menuTexts) {
            window.draw(text);
        }
        window.display();
    }

    return 0;
}
