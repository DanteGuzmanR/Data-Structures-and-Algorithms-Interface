#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// Nodo para representar cada elemento de la lista enlazada
struct Node {
    int value;
    sf::Vector2f position; // Posición del nodo en la pantalla
    bool isHighlighted = false; // Indica si el nodo está resaltado (para búsqueda)
};

// Función principal de las listas enlazadas
void linkedLists(sf::RenderWindow &window) {
    sf::Font font;
    font.loadFromFile("GOTHIC.ttf");

    // Configuración del título
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Linked Lists");
    titleText.setCharacterSize(36);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(10, 10);

    // Texto del modo actual
    sf::Text modeText;
    modeText.setFont(font);
    modeText.setCharacterSize(24);
    modeText.setStyle(sf::Text::Bold);
    modeText.setFillColor(sf::Color::White);
    modeText.setPosition(10, 60);

    // Texto para las instrucciones de cambio de modo
    sf::Text changeModeText;
    changeModeText.setFont(font);
    changeModeText.setCharacterSize(18);
    changeModeText.setFillColor(sf::Color::White);
    changeModeText.setPosition(10, 100);

    // Texto para salir
    sf::Text exitText;
    exitText.setFont(font);
    exitText.setCharacterSize(18);
    exitText.setFillColor(sf::Color::White);
    exitText.setString("Presione 'S' para salir.");
    exitText.setPosition(10, 140);

    std::vector<Node> linkedList; // Representación de la lista enlazada
    std::string inputBuffer = ""; // Entrada para las operaciones
    std::string currentMode = "Insertion"; // Modo inicial: Inserción
    const int nodeSize = 50; // Tamaño de cada nodo
    const int baseX = 200; // Posición inicial X
    const int baseY = 400; // Posición Y fija

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            // Manejar cambio de modo
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::I) {
                    currentMode = "Insertion";
                    inputBuffer = ""; // Reiniciar entrada
                    for (auto &node : linkedList) {
                        node.isHighlighted = false; // Reiniciar colores
                    }
                } else if (event.key.code == sf::Keyboard::E) {
                    currentMode = "Deletion";
                    inputBuffer = ""; // Reiniciar entrada
                    for (auto &node : linkedList) {
                        node.isHighlighted = false; // Reiniciar colores
                    }
                } else if (event.key.code == sf::Keyboard::B) {
                    currentMode = "Search";
                    inputBuffer = ""; // Reiniciar entrada
                } else if (event.key.code == sf::Keyboard::S) {
                    return; // Salir al menú principal
                }
            }

            // Manejar entrada del teclado
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') { // Borrar
                    if (!inputBuffer.empty()) {
                        inputBuffer.pop_back();
                    }
                } else if (event.text.unicode == '\r') { // Enter
                    if (currentMode == "Insertion") {
                        try {
                            int value = std::stoi(inputBuffer);
                            Node newNode = {value, sf::Vector2f(baseX + linkedList.size() * (nodeSize + 20), baseY)};
                            linkedList.push_back(newNode); // Insertar nodo al final
                            inputBuffer.clear();
                        } catch (...) {
                            inputBuffer.clear(); // Limpiar entrada inválida
                        }
                    } else if (currentMode == "Deletion") {
                        try {
                            int value = std::stoi(inputBuffer);
                            auto it = std::find_if(linkedList.begin(), linkedList.end(),
                                [&](const Node &node) { return node.value == value; });
                            if (it != linkedList.end()) {
                                linkedList.erase(it); // Eliminar nodo si se encuentra
                            }
                            inputBuffer.clear();
                        } catch (...) {
                            inputBuffer.clear(); // Limpiar entrada inválida
                        }
                    } else if (currentMode == "Search") {
                        try {
                            int value = std::stoi(inputBuffer);
                            for (auto &node : linkedList) {
                                if (node.value == value) {
                                    node.isHighlighted = true; // Resaltar nodo encontrado
                                } else {
                                    node.isHighlighted = false; // Reiniciar otros nodos
                                }
                            }
                            inputBuffer.clear();
                        } catch (...) {
                            inputBuffer.clear(); // Limpiar entrada inválida
                        }
                    }
                } else if (event.text.unicode < 128) { // Agregar caracteres
                    inputBuffer += static_cast<char>(event.text.unicode);
                }
            }
        }

        // Dibujar en la ventana
        window.clear(sf::Color::Black);

        // Mostrar título y modo
        window.draw(titleText);
        modeText.setString("Modo actual: " + currentMode + " (I/E/B)");
        window.draw(modeText);

        // Mostrar instrucciones para cambiar de modo
        if (currentMode == "Insertion") {
            changeModeText.setString("Presione E para ir a modo Eliminacion, B para Busqueda");
        } else if (currentMode == "Deletion") {
            changeModeText.setString("Presione I para ir a modo Insercion, B para Busqueda");
        } else {
            changeModeText.setString("Presione I para ir a modo Insercion, E para Eliminacion");
        }
        window.draw(changeModeText);

        // Mostrar instrucciones para salir
        window.draw(exitText);

        // Mostrar entrada del usuario
        sf::Text inputText;
        inputText.setFont(font);
        inputText.setCharacterSize(24);
        inputText.setFillColor(sf::Color::White);
        inputText.setString("Entrada: " + inputBuffer);
        inputText.setPosition(10, 180);
        window.draw(inputText);

        // Dibujar la lista enlazada
        for (size_t i = 0; i < linkedList.size(); ++i) {
            // Dibujar nodo
            sf::RectangleShape node(sf::Vector2f(nodeSize, nodeSize));
            node.setFillColor(linkedList[i].isHighlighted ? sf::Color::Green : sf::Color::Red);
            node.setOutlineThickness(3);
            node.setOutlineColor(sf::Color::White);
            node.setPosition(linkedList[i].position);
            window.draw(node);

            // Mostrar valor del nodo
            sf::Text nodeText;
            nodeText.setFont(font);
            nodeText.setCharacterSize(20);
            nodeText.setFillColor(sf::Color::White);
            nodeText.setString(std::to_string(linkedList[i].value));
            nodeText.setPosition(linkedList[i].position.x + 10, linkedList[i].position.y + 10);
            window.draw(nodeText);

            // Dibujar flecha hacia el siguiente nodo
            if (i < linkedList.size() - 1) {
                sf::RectangleShape arrow(sf::Vector2f(30, 5));
                arrow.setFillColor(sf::Color::Cyan);
                arrow.setPosition(linkedList[i].position.x + nodeSize, linkedList[i].position.y + nodeSize / 2 - 2);
                window.draw(arrow);
            }
        }

        // Dibujar puntero NULL al final
        if (!linkedList.empty()) {
            sf::Text nullText;
            nullText.setFont(font);
            nullText.setCharacterSize(20);
            nullText.setFillColor(sf::Color::White);
            nullText.setString("NULL");
            nullText.setPosition(linkedList.back().position.x + nodeSize + 40, baseY + 10);
            window.draw(nullText);
        }

        window.display();
    }
}
