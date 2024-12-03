#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>

void stacks(sf::RenderWindow &window) {
    sf::Font font;
    font.loadFromFile("GOTHIC.ttf");

    // Configuración del título
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Stacks");
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

    std::vector<int> stack; // Representación de la pila
    std::string inputBuffer = ""; // Entrada para modo Push
    bool isPushMode = true; // Modo inicial (Push)
    const int blockHeight = 50; // Altura de cada bloque
    const int baseY = 700; // Base de la pila

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
                    isPushMode = true;
                    inputBuffer = ""; // Reiniciar entrada
                } else if (event.key.code == sf::Keyboard::D) {
                    isPushMode = false;
                } else if (event.key.code == sf::Keyboard::S) {
                    return; // Salir al menú principal
                } else if (event.key.code == sf::Keyboard::Enter && !isPushMode) {
                    if (!stack.empty()) {
                        stack.pop_back(); // Eliminar el último elemento en modo Pop
                    }
                }
            }

            // Manejar la entrada en modo Push
            if (isPushMode && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') { // Borrar
                    if (!inputBuffer.empty()) {
                        inputBuffer.pop_back();
                    }
                } else if (event.text.unicode == '\r') { // Enter
                    try {
                        int value = std::stoi(inputBuffer);
                        stack.push_back(value); // Agregar elemento a la pila
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
        modeText.setString("Modo actual: " + std::string(isPushMode ? "Push (P)" : "Pop (D)"));
        window.draw(modeText);

        // Mostrar instrucciones para cambiar de modo
        if (isPushMode) {
            changeModeText.setString("Presione D para ir a modo Pop");
        } else {
            changeModeText.setString("Presione P para ir a modo Push");
        }
        window.draw(changeModeText);

        // Mostrar instrucciones para salir
        window.draw(exitText);

        // Mostrar entrada y mensaje en modo Push
        if (isPushMode) {
            sf::Text inputText;
            inputText.setFont(font);
            inputText.setCharacterSize(24);
            inputText.setFillColor(sf::Color::White);
            inputText.setString("Ingrese un valor: " + inputBuffer);
            inputText.setPosition(10, 180);
            window.draw(inputText);
        } else {
            sf::Text popInstructionText;
            popInstructionText.setFont(font);
            popInstructionText.setCharacterSize(24);
            popInstructionText.setFillColor(sf::Color::White);
            popInstructionText.setString("Presione Enter para desapilar un elemento.");
            popInstructionText.setPosition(10, 180);
            window.draw(popInstructionText);
        }

        // Dibujar la pila
        for (size_t i = 0; i < stack.size(); ++i) {
            sf::RectangleShape block(sf::Vector2f(200, blockHeight));
            block.setFillColor(sf::Color::Red); // Relleno rojo
            block.setOutlineThickness(3);
            block.setOutlineColor(sf::Color::White); // Bordes blancos
            block.setPosition(400, baseY - (i * blockHeight)); // Posición basada en el índice

            // Mostrar valor dentro del bloque
            sf::Text blockText;
            blockText.setFont(font);
            blockText.setCharacterSize(20);
            blockText.setFillColor(sf::Color::White);
            blockText.setString(std::to_string(stack[i]));
            blockText.setPosition(450, baseY - (i * blockHeight) + 10); // Centrar el texto dentro del bloque

            window.draw(block);
            window.draw(blockText);
        }

        window.display();
    }
}
