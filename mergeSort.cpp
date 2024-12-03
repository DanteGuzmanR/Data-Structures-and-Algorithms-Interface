#include "MergeSort.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <iomanip>

void drawBars(sf::RenderWindow &window, const std::vector<int> &arr, int barWidth, int barSpacing, int baseY, const sf::Font &font, const std::vector<bool> &isSorted = {}, int highlight1 = -1, int highlight2 = -1) {
    sf::Text numberText;
    numberText.setFont(font);
    numberText.setCharacterSize(18);
    numberText.setFillColor(sf::Color::White);

    for (size_t i = 0; i < arr.size(); ++i) {
        sf::RectangleShape bar;
        bar.setSize(sf::Vector2f(barWidth, -arr[i] * 10)); // Altura proporcional al valor
        bar.setPosition(i * (barWidth + barSpacing), baseY);

        // Colorear las barras
        if (isSorted.size() > i && isSorted[i]) {
            bar.setFillColor(sf::Color::Green); // Barras ordenadas
        } else if (i == highlight1 || i == highlight2) {
            bar.setFillColor(sf::Color::Yellow); // Comparaciones activas
        } else {
            bar.setFillColor(sf::Color::White); // Barras normales
        }

        window.draw(bar);

        // Dibujar el número debajo de la barra
        numberText.setString(std::to_string(arr[i]));
        numberText.setPosition(i * (barWidth + barSpacing) + barWidth / 4, baseY + 10);
        window.draw(numberText);
    }
}

void mergeSortVisual(sf::RenderWindow &window, std::vector<int> &arr, int left, int right, int barWidth, int barSpacing, int baseY, const sf::Font &font, std::vector<bool> &isSorted) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSortVisual(window, arr, left, mid, barWidth, barSpacing, baseY, font, isSorted);
        mergeSortVisual(window, arr, mid + 1, right, barWidth, barSpacing, baseY, font, isSorted);

        std::vector<int> temp(arr.begin() + left, arr.begin() + right + 1);
        int i = 0, j = mid + 1 - left, k = left;

        while (i <= mid - left && j <= right - left) {
            window.clear(sf::Color::Black);
            drawBars(window, arr, barWidth, barSpacing, baseY, font, isSorted, left + i, mid + 1 + j);
            window.display();

            std::this_thread::sleep_for(std::chrono::milliseconds(300));

            if (temp[i] <= temp[j]) {
                arr[k++] = temp[i++];
            } else {
                arr[k++] = temp[j++];
            }
        }

        while (i <= mid - left) {
            window.clear(sf::Color::Black);
            drawBars(window, arr, barWidth, barSpacing, baseY, font, isSorted, left + i);
            window.display();

            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            arr[k++] = temp[i++];
        }

        while (j <= right - left) {
            window.clear(sf::Color::Black);
            drawBars(window, arr, barWidth, barSpacing, baseY, font, isSorted, mid + 1 + j);
            window.display();

            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            arr[k++] = temp[j++];
        }

        // Marcar como ordenados
        for (int x = left; x <= right; ++x) {
            isSorted[x] = true;
        }
    }
}

void mergeSort(sf::RenderWindow &window) {
    std::vector<int> arr;
    sf::Font font;
    font.loadFromFile("GOTHIC.ttf");

    // Configuración del título
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Merge Sort");
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
    double executionTime = 0.0;

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
                            originalArray = inputBuffer;
                            inputBuffer.clear();
                            enteringArray = false;

                            // Calcular tiempo de ejecución real del algoritmo
                            auto startTime = std::chrono::high_resolution_clock::now();
                            std::vector<bool> isSorted(arr.size(), false);
                            mergeSortVisual(window, arr, 0, arr.size() - 1, 90, 15, 750, font, isSorted);
                            auto endTime = std::chrono::high_resolution_clock::now();
                            executionTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

                            startAnimation = true;
                        }
                    }
                } else if (event.text.unicode < 128) { // Agregar caracteres
                    inputBuffer += static_cast<char>(event.text.unicode);
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(titleText);

        if (enteringArray) {
            infoText.setString("Ingrese los elementos del array separados por comas:\n" + inputBuffer);
        } else if (startAnimation) {
            std::string sortedArray = "Vector ordenado: ";
            for (int num : arr) {
                sortedArray += std::to_string(num) + " ";
            }
            float tiempo;
            tiempo = executionTime/1000000;
            infoText.setString(
                "Complejidad temporal: O(n log n)\n"
                "Vector introducido: " + originalArray +
                "\n" + sortedArray +
                "\nTiempo de ejecucion: " + std::to_string(tiempo) + " ms\n"
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
