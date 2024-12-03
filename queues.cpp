#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>

void queues(sf::RenderWindow &window) {
    sf::Font font;
    font.loadFromFile("GOTHIC.ttf");

    // Configuración del título
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Queues");
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

    std::vector<int> queue; // Representación de la cola
    std::string inputBuffer = ""; // Entrada para modo enqueue
    bool isEnqueueMode = true; // Modo inicial (enqueue)
    const int blockWidth = 100; // Ancho de cada bloque
    const int blockHeight = 50; // Altura de cada bloque
    const int baseX = 200; // Base X de la cola
    const int baseY = 400; // Base Y de la cola

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            // Manejar el cambio de modo
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P) {
                    isEnqueueMode = true;
                    inputBuffer = ""; // Reiniciar entrada
                } else if (event.key.code == sf::Keyboard::D) {
                    isEnqueueMode = false;
                } else if (event.key.code == sf::Keyboard::S) {
                    return; // Salir al menú principal
                } else if (event.key.code == sf::Keyboard::Enter && !isEnqueueMode) {
                    if (!queue.empty()) {
                        queue.erase(queue.begin()); // Eliminar el primer elemento en modo dequeue
                    }
                }
            }

            // Manejar la entrada en modo enqueue
            if (isEnqueueMode && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') { // Borrar
                    if (!inputBuffer.empty()) {
                        inputBuffer.pop_back();
                    }
                } else if (event.text.unicode == '\r') { // Enter
                    try {
                        int value = std::stoi(inputBuffer);
                        queue.push_back(value); // Agregar elemento a la cola
                        inputBuffer.clear();
                    } catch (...) {
                        inputBuffer.clear(); // Limpiar entrada inválida
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
        modeText.setString("Modo actual: " + std::string(isEnqueueMode ? "Enqueue (P)" : "Dequeue (D)"));
        window.draw(modeText);

        // Mostrar instrucciones para cambiar de modo
        if (isEnqueueMode) {
            changeModeText.setString("Presione D para ir a modo Dequeue");
        } else {
            changeModeText.setString("Presione P para ir a modo Enqueue");
        }
        window.draw(changeModeText);

        // Mostrar instrucciones para salir
        window.draw(exitText);

        // Mostrar entrada y mensaje en modo enqueue
        if (isEnqueueMode) {
            sf::Text inputText;
            inputText.setFont(font);
            inputText.setCharacterSize(24);
            inputText.setFillColor(sf::Color::White);
            inputText.setString("Ingrese un valor: " + inputBuffer);
            inputText.setPosition(10, 180);
            window.draw(inputText);
        } else {
            sf::Text dequeueInstructionText;
            dequeueInstructionText.setFont(font);
            dequeueInstructionText.setCharacterSize(24);
            dequeueInstructionText.setFillColor(sf::Color::White);
            dequeueInstructionText.setString("Presione Enter para quitar un elemento.");
            dequeueInstructionText.setPosition(10, 180);
            window.draw(dequeueInstructionText);
        }

        // Dibujar la cola
        for (size_t i = 0; i < queue.size(); ++i) {
            sf::RectangleShape block(sf::Vector2f(blockWidth, blockHeight));
            block.setFillColor(sf::Color::Red); // Relleno rojo
            block.setOutlineThickness(3);
            block.setOutlineColor(sf::Color::White); // Bordes blancos
            block.setPosition(baseX + (i * (blockWidth + 10)), baseY); // Posición basada en el índice

            // Mostrar valor dentro del bloque
            sf::Text blockText;
            blockText.setFont(font);
            blockText.setCharacterSize(20);
            blockText.setFillColor(sf::Color::White);
            blockText.setString(std::to_string(queue[i]));
            blockText.setPosition(baseX + (i * (blockWidth + 10)) + 25, baseY + 10); // Centrar el texto dentro del bloque

            window.draw(block);
            window.draw(blockText);
        }

        // Dibujar flechas de front y rear
        if (!queue.empty()) {
            sf::Text frontText, rearText;
            frontText.setFont(font);
            rearText.setFont(font);
            frontText.setCharacterSize(18);
            rearText.setCharacterSize(18);
            frontText.setFillColor(sf::Color::Green);
            rearText.setFillColor(sf::Color::Cyan);

            frontText.setString("Front");
            rearText.setString("Rear");

            frontText.setPosition(baseX - 50, baseY);
            rearText.setPosition(baseX + (queue.size() - 1) * (blockWidth + 10) + blockWidth + 10, baseY);

            window.draw(frontText);
            window.draw(rearText);
        }

        window.display();
    }
}
