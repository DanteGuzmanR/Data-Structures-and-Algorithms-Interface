#include "BubbleSort.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <iomanip> // Para manejar precisión del tiempo

void bubbleSort(sf::RenderWindow &window) {
    std::vector<int> arr;
    sf::Font font;
    font.loadFromFile("GOTHIC.ttf");

    // Configuración del título
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Bubble Sort");
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
    bool startAnimation = false;
    std::string originalArray;
    double executionTime = 0.0; // Tiempo real de ejecución del algoritmo

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
                            originalArray = inputBuffer; // Guardar el array original
                            inputBuffer.clear();
                            enteringArray = false;

                            // Calcular el tiempo de ejecución del algoritmo sin afectar la animación
                            auto startTime = std::chrono::high_resolution_clock::now();
                            std::vector<int> tempArray = arr; // Copia para calcular el tiempo
                            for (size_t i = 0; i < tempArray.size(); ++i) {
                                for (size_t j = 0; j < tempArray.size() - i - 1; ++j) {
                                    if (tempArray[j] > tempArray[j + 1]) {
                                        std::swap(tempArray[j], tempArray[j + 1]);
                                    }
                                }
                            }
                            auto endTime = std::chrono::high_resolution_clock::now();
                            executionTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

                            startAnimation = true; // Comenzar la animación
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
        } else if (startAnimation) {
            // Animación de Bubble Sort
            const int barWidth = 90;  // Ancho de cada barra
            const int barSpacing = 15;  // Espaciado entre barras
            const int baseY = 750;  // Posición Y de las barras

            for (size_t i = 0; i < arr.size(); ++i) {
                for (size_t j = 0; j < arr.size() - i - 1; ++j) {
                    window.clear(sf::Color::Black);
                    window.draw(titleText);

                    finalBars.clear(); // Actualiza las barras en cada iteración

                    // Dibujar las barras
                    for (size_t k = 0; k < arr.size(); ++k) {
                        sf::RectangleShape bar;
                        bar.setSize(sf::Vector2f(barWidth, -arr[k] * 10)); // Altura proporcional al valor
                        bar.setPosition(k * (barWidth + barSpacing), baseY);

                        if (k == j || k == j + 1) {
                            bar.setFillColor(sf::Color::Yellow); // Elementos en comparación
                        } else if (k >= arr.size() - i) {
                            bar.setFillColor(sf::Color::Green); // Elementos ya ordenados
                        } else {
                            bar.setFillColor(sf::Color::White); // Elementos no comparados
                        }

                        finalBars.push_back(bar); // Guarda la barra final
                        window.draw(bar);

                        // Mostrar número debajo de la barra
                        numberText.setString(std::to_string(arr[k]));
                        numberText.setPosition(k * (barWidth + barSpacing) + barWidth / 4, baseY + 10);
                        window.draw(numberText);
                    }

                    // Mostrar información
                    infoText.setString("Ordenando...");
                    window.draw(infoText);

                    window.display();

                    // Pausa para animación
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));

                    // Intercambiar si están en orden incorrecto
                    if (arr[j] > arr[j + 1]) {
                        std::swap(arr[j], arr[j + 1]);
                    }
                }
            }

            // Mostrar resultado con barras finales
            window.clear(sf::Color::Black);
            window.draw(titleText);

            // Dibujar las barras finales almacenadas
            for (auto& bar : finalBars) {
                window.draw(bar);
            }

            std::string sortedArray = "Vector ordenado: ";
            for (int num : arr) {
                sortedArray += std::to_string(num) + " ";
            }

            infoText.setString(
                "Complejidad temporal: O(n^2)\n"
                "Vector introducido: " + originalArray +
                "\n" + sortedArray +
                "\nTiempo de ejecucion: " + std::to_string(executionTime) + " ms\n"
                "Presione 'S' para volver al menu principal."
            );
            window.draw(infoText);

            window.display();
            startAnimation = false;
        }

        window.draw(infoText);
        window.display();

        // Volver al menú principal
        if (!enteringArray && !startAnimation) {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
                    return;
                }
            }
        }
    }
}
