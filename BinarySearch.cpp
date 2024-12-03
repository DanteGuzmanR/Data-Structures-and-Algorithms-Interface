#include "BinarySearch.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>
#include <algorithm> // Para std::sort

void binarySearch(sf::RenderWindow &window) {
    std::vector<int> arr;
    int target = -1;

    sf::Font font;
    font.loadFromFile("GOTHIC.ttf");

    // Configuración del título
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Binary Search");
    titleText.setCharacterSize(36);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(10, 10);

    // Texto para la entrada del usuario
    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(24);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition(10, 60);

    std::string inputBuffer = "";
    bool enteringArray = true;
    bool enteringTarget = false;
    bool startAnimation = false;

    std::vector<sf::RectangleShape> finalBars; // Almacena las barras finales
    sf::Text numberText;                      // Texto para los números debajo de las barras
    numberText.setFont(font);
    numberText.setCharacterSize(18);
    numberText.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            // Manejar la entrada del teclado
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') { // Borrar
                    if (!inputBuffer.empty()) {
                        inputBuffer.pop_back();
                    }
                } else if (event.text.unicode == '\r') { // Enter
                    if (enteringArray) {
                        // Procesar el array ingresado
                        inputBuffer.erase(std::remove_if(inputBuffer.begin(), inputBuffer.end(), isspace), inputBuffer.end());
                        std::istringstream iss(inputBuffer);
                        std::string token;
                        while (std::getline(iss, token, ',')) {
                            try {
                                arr.push_back(std::stoi(token));
                            } catch (...) {
                                infoText.setString("Entrada no valida. Intente nuevamente.");
                                inputBuffer.clear();
                                break;
                            }
                        }
                        if (!arr.empty()) {
                            std::sort(arr.begin(), arr.end()); // Ordenar el array
                            inputBuffer.clear();
                            enteringArray = false;
                            enteringTarget = true;
                        }
                    } else if (enteringTarget) {
                        try {
                            target = std::stoi(inputBuffer);
                            inputBuffer.clear();
                            enteringTarget = false;
                            startAnimation = true;
                        } catch (...) {
                            infoText.setString("Entrada no valida. Intente nuevamente.");
                            inputBuffer.clear();
                        }
                    }
                } else if (event.text.unicode < 128) { // Agregar caracteres
                    inputBuffer += static_cast<char>(event.text.unicode);
                }
            }
        }

        // Dibujar en la ventana
        window.clear(sf::Color::Black);

        // Mostrar título
        window.draw(titleText);

        if (enteringArray) {
            infoText.setString("Ingrese los elementos del array separados por comas:\n" + inputBuffer);
        } else if (enteringTarget) {
            infoText.setString("Ingrese el elemento a buscar:\n" + inputBuffer);
        } else if (startAnimation) {
            // Animación de Binary Search
            const int barWidth = 90;  // Ancho de cada barra (más grande)
            const int barSpacing = 15;  // Espaciado entre barras
            const int baseY = 750;  // Posición Y de las barras (más abajo)

            int left = 0;
            int right = arr.size() - 1;
            int middle;
            bool found = false;

            finalBars.clear(); // Limpia las barras finales antes de empezar

            while (left <= right) {
                window.clear(sf::Color::Black);
                window.draw(titleText);

                finalBars.clear(); // Actualiza las barras en cada iteración

                // Calcular el medio
                middle = left + (right - left) / 2;

                // Dibujar las barras
                for (size_t j = 0; j < arr.size(); ++j) {
                    sf::RectangleShape bar;
                    bar.setSize(sf::Vector2f(barWidth, -arr[j] * 10)); // Altura proporcional al valor
                    bar.setPosition(j * (barWidth + barSpacing), baseY);

                    if (j == middle && found) {
                        bar.setFillColor(sf::Color::Green); // Elemento encontrado
                    } else if (j == middle) {
                        bar.setFillColor(sf::Color::Yellow); // Elemento medio
                    } else if (j >= left && j <= right) {
                        bar.setFillColor(sf::Color::White); // Elementos en el rango
                    } else {
                        bar.setFillColor(sf::Color::Red); // Fuera del rango actual
                    }

                    finalBars.push_back(bar); // Guarda la barra final
                    window.draw(bar);

                    // Mostrar número debajo de la barra
                    numberText.setString(std::to_string(arr[j]));
                    numberText.setPosition(j * (barWidth + barSpacing) + barWidth / 4, baseY + 10);
                    window.draw(numberText);
                }

                // Mostrar información
                infoText.setString("Buscando: " + std::to_string(target));
                window.draw(infoText);

                window.display();

                // Pausa para animación
                std::this_thread::sleep_for(std::chrono::milliseconds(500));

                // Comparar
                if (arr[middle] == target) {
                    found = true;
                    break;
                } else if (arr[middle] < target) {
                    left = middle + 1;
                } else {
                    right = middle - 1;
                }
            }

            // Mostrar resultado con barras finales
            window.clear(sf::Color::Black);
            window.draw(titleText);

            // Dibujar las barras finales almacenadas
            for (auto& bar : finalBars) {
                window.draw(bar);
            }

            std::string resultMessage = found ? "Elemento encontrado en el indice: " + std::to_string(middle)
                                              : "Elemento no encontrado.";
            infoText.setString(resultMessage + "\nComplejidad temporal: O(log n)\nPresione 'S' para volver al menu principal.");
            window.draw(infoText);

            window.display();
            startAnimation = false;
        }

        window.draw(infoText);
        window.display();

        // Volver al menú principal
        if (!enteringArray && !enteringTarget && !startAnimation) {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
                    return;
                }
            }
        }
    }
}
