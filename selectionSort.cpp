#include "SelectionSort.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <iomanip> // Para manejar precisión del tiempo

void dibujarBarras(sf::RenderWindow &window, const std::vector<int> &arr, int barWidth, int barSpacing, int baseY, const sf::Font &font, int current = -1, int minIdx = -1, int sorted = -1) {
    sf::Text numberText;
    numberText.setFont(font);
    numberText.setCharacterSize(18);
    numberText.setFillColor(sf::Color::White);

    for (size_t i = 0; i < arr.size(); ++i) {
        sf::RectangleShape bar;
        bar.setSize(sf::Vector2f(barWidth, -arr[i] * 10)); // Altura proporcional al valor
        bar.setPosition(i * (barWidth + barSpacing), baseY);

        if (i == current) {
            bar.setFillColor(sf::Color::Yellow); // Elemento en comparación
        } else if (i == minIdx) {
            bar.setFillColor(sf::Color::Blue); // Elemento mínimo encontrado
        } else if (i <= sorted) {
            bar.setFillColor(sf::Color::Green); // Elementos ya ordenados
        } else {
            bar.setFillColor(sf::Color::White); // Elementos no comparados
        }

        window.draw(bar);

        // Mostrar número debajo de la barra
        numberText.setString(std::to_string(arr[i]));
        numberText.setPosition(i * (barWidth + barSpacing) + barWidth / 4, baseY + 10);
        window.draw(numberText);
    }
}

void selectionSortVisual(sf::RenderWindow &window, std::vector<int> &arr, int barWidth, int barSpacing, int baseY, const sf::Font &font, sf::Text &titleText) {
    for (size_t i = 0; i < arr.size(); ++i) {
        size_t minIdx = i;

        for (size_t j = i + 1; j < arr.size(); ++j) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }

            // Dibujar las barras durante la comparación
            window.clear(sf::Color::Black);
            window.draw(titleText); // Título siempre visible
            dibujarBarras(window, arr, barWidth, barSpacing, baseY, font, j, minIdx, i - 1);
            window.display();

            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }

        // Intercambiar el elemento mínimo encontrado
        std::swap(arr[i], arr[minIdx]);

        // Dibujar las barras después del intercambio
        window.clear(sf::Color::Black);
        window.draw(titleText); // Título siempre visible
        dibujarBarras(window, arr, barWidth, barSpacing, baseY, font, -1, -1, i);
        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

void selectionSort(sf::RenderWindow &window) {
    std::vector<int> arr;
    sf::Font font;
    font.loadFromFile("GOTHIC.ttf");

    // Configuración del título
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Selection Sort");
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

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') { // Borrar
                    if (!inputBuffer.empty()) {
                        inputBuffer.pop_back();
                    }
                } else if (event.text.unicode == '\r') { // Enter
                    if (enteringArray) {
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

                            // Calcular el tiempo de ejecución del algoritmo
                            auto startTime = std::chrono::high_resolution_clock::now();
                            std::vector<int> tempArray = arr;
                            for (size_t i = 0; i < tempArray.size(); ++i) {
                                size_t minIdx = i;
                                for (size_t j = i + 1; j < tempArray.size(); ++j) {
                                    if (tempArray[j] < tempArray[minIdx]) {
                                        minIdx = j;
                                    }
                                }
                                std::swap(tempArray[i], tempArray[minIdx]);
                            }
                            auto endTime = std::chrono::high_resolution_clock::now();
                            executionTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

                            startAnimation = true;
                        }
                    }
                } else if (event.text.unicode < 128) {
                    inputBuffer += static_cast<char>(event.text.unicode);
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(titleText);

        if (enteringArray) {
            infoText.setString("Ingrese los elementos del array separados por comas:\n" + inputBuffer);
        } else if (startAnimation) {
            window.clear(sf::Color::Black);
            selectionSortVisual(window, arr, 90, 15, 750, font, titleText);

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

            while (true) {
                window.pollEvent(event);
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
                    return;
                }
            }
        }

        window.draw(infoText);
        window.display();
    }
}
