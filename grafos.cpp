#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <thread>
#include <stdexcept>

// Estructura para las aristas del grafo
struct Connection {
    int destination;
};

// Función para dibujar texto
void renderText(sf::RenderWindow& window, const std::string& text, float x, float y, unsigned int size, const sf::Font& font, sf::Color color = sf::Color::White) {
    sf::Text sfText(text, font, size);
    sfText.setPosition(x, y);
    sfText.setFillColor(color);
    sfText.setStyle(sf::Text::Bold);
    window.draw(sfText);
}

// Función para dibujar nodos
void renderNode(sf::RenderWindow& window, const sf::Vector2f& position, const std::string& label, const sf::Font& font, sf::Color color) {
    sf::CircleShape circle(25);
    circle.setFillColor(color);
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(3);
    circle.setPosition(position - sf::Vector2f(25, 25));
    window.draw(circle);

    sf::Text text(label, font, 20);
    text.setPosition(position - sf::Vector2f(10, 10));
    text.setFillColor(sf::Color::White);
    window.draw(text);
}

// Función para dibujar aristas
void renderConnection(sf::RenderWindow& window, const sf::Vector2f& from, const sf::Vector2f& to) {
    sf::Vertex line[] = {
        sf::Vertex(from, sf::Color::White),
        sf::Vertex(to, sf::Color::White)
    };
    window.draw(line, 2, sf::Lines);
}

// Función para verificar existencia en un unordered_map
bool existsInMap(const std::unordered_map<int, sf::Vector2f>& positions, int node) {
    return positions.find(node) != positions.end();
}

// Función para animar BFS
void bfsTraversal(sf::RenderWindow& window, const std::unordered_map<int, sf::Vector2f>& positions,
                  const std::unordered_map<int, std::vector<Connection>>& graph, int start, int target, const sf::Font& font) {
    std::queue<int> queue;
    std::unordered_map<int, bool> visited;
    queue.push(start);
    visited[start] = true;

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        // Dibujar estado actual
        window.clear(sf::Color::Black);
        for (const auto& [node, edges] : graph) {
            for (const auto& edge : edges) {
                if (existsInMap(positions, node) && existsInMap(positions, edge.destination)) {
                    renderConnection(window, positions.at(node), positions.at(edge.destination));
                }
            }
        }

        for (const auto& [node, position] : positions) {
            if (node == current) {
                renderNode(window, position, std::to_string(node), font, sf::Color::Yellow);
            } else if (visited[node]) {
                renderNode(window, position, std::to_string(node), font, sf::Color::Green);
            } else {
                renderNode(window, position, std::to_string(node), font, sf::Color::Blue);
            }
        }

        renderText(window, "Modo: BFS", window.getSize().x / 2 - 100, 10, 30, font, sf::Color::White);
        renderText(window, "Buscando nodo: " + std::to_string(target), 10, 50, 20, font);
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        if (current == target) {
            renderText(window, "Nodo encontrado!", window.getSize().x / 2 - 100, window.getSize().y - 50, 25, font, sf::Color::Green);
            window.display();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return;
        }

        if (graph.find(current) != graph.end()) {
            for (const auto& edge : graph.at(current)) {
                if (!visited[edge.destination]) {
                    queue.push(edge.destination);
                    visited[edge.destination] = true;
                }
            }
        }
    }

    renderText(window, "Nodo no encontrado!", window.getSize().x / 2 - 100, window.getSize().y - 50, 25, font, sf::Color::Red);
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

// Función para animar DFS
void dfsTraversal(sf::RenderWindow& window, const std::unordered_map<int, sf::Vector2f>& positions,
                  const std::unordered_map<int, std::vector<Connection>>& graph, int current, int target, const sf::Font& font,
                  std::unordered_map<int, bool>& visited) {
    if (!existsInMap(positions, current) || !existsInMap(positions, target)) {
        throw std::runtime_error("Nodo inicial o final no existe en el grafo.");
    }

    visited[current] = true;

    // Dibujar estado actual
    window.clear(sf::Color::Black);
    for (const auto& [node, edges] : graph) {
        for (const auto& edge : edges) {
            if (existsInMap(positions, node) && existsInMap(positions, edge.destination)) {
                renderConnection(window, positions.at(node), positions.at(edge.destination));
            }
        }
    }

    for (const auto& [node, position] : positions) {
        if (node == current) {
            renderNode(window, position, std::to_string(node), font, sf::Color::Yellow);
        } else if (visited[node]) {
            renderNode(window, position, std::to_string(node), font, sf::Color::Green);
        } else {
            renderNode(window, position, std::to_string(node), font, sf::Color::Blue);
        }
    }

    renderText(window, "Modo: DFS", window.getSize().x / 2 - 100, 10, 30, font, sf::Color::White);
    renderText(window, "Buscando nodo: " + std::to_string(target), 10, 50, 20, font);
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    if (current == target) {
        renderText(window, "Nodo encontrado!", window.getSize().x / 2 - 100, window.getSize().y - 50, 25, font, sf::Color::Green);
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return;
    }

    if (graph.find(current) != graph.end()) {
        for (const auto& edge : graph.at(current)) {
            if (!visited[edge.destination]) {
                dfsTraversal(window, positions, graph, edge.destination, target, font, visited);
                if (visited[target]) return; // Terminar si ya se encontró el nodo
            }
        }
    }
}

// Función principal
void graphs(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("GOTHIC.ttf")) {
        throw std::runtime_error("No se pudo cargar la fuente GOTHIC.ttf.");
    }

    std::unordered_map<int, sf::Vector2f> positions = {
        {0, {200, 400}}, {1, {400, 300}}, {2, {600, 400}},
        {3, {200, 500}}, {4, {400, 500}}, {5, {600, 500}}
    };

    std::unordered_map<int, std::vector<Connection>> graph = {
        {0, {{1}, {3}}}, {1, {{0}, {2}, {4}}}, {2, {{1}, {5}}},
        {3, {{0}, {4}}}, {4, {{1}, {3}, {5}}}, {5, {{2}, {4}}}
    };

    bool isBFSMode = true;
    std::string userInput;
    sf::Event event;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::TextEntered) {
                if (std::isdigit(static_cast<unsigned char>(event.text.unicode))) userInput += static_cast<char>(event.text.unicode);
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Backspace) {
                if (!userInput.empty()) userInput.pop_back();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                if (!userInput.empty()) {
                    int targetNode = std::stoi(userInput);
                    userInput.clear();
                    if (positions.find(targetNode) != positions.end()) {
                        if (isBFSMode) {
                            bfsTraversal(window, positions, graph, 0, targetNode, font);
                        } else {
                            std::unordered_map<int, bool> visited;
                            dfsTraversal(window, positions, graph, 0, targetNode, font, visited);
                        }
                    } else {
                        window.clear(sf::Color::Black);
                        renderText(window, "Nodo no valido. Intente de nuevo.", 10, 10, 20, font, sf::Color::Red);
                        window.display();
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) isBFSMode = !isBFSMode;
        }

        window.clear(sf::Color::Black);
        renderText(window, "Grafos", window.getSize().x / 2 - 100, 10, 40, font);
        renderText(window, isBFSMode ? "Modo: BFS" : "Modo: DFS", window.getSize().x / 2 - 100, 60, 30, font);
        renderText(window, "Ingrese el nodo objetivo y presione Enter", 10, 100, 20, font);
        renderText(window, "Presione Espacio para cambiar de modo", 10, 130, 20, font);
        renderText(window, "Entrada actual: " + userInput, 10, 160, 20, font);

        for (const auto& [node, edges] : graph) {
            for (const auto& edge : edges) {
                if (existsInMap(positions, node) && existsInMap(positions, edge.destination)) {
                    renderConnection(window, positions.at(node), positions.at(edge.destination));
                }
            }
        }

        for (const auto& [node, position] : positions) {
            renderNode(window, position, std::to_string(node), font, sf::Color::Blue);
        }

        window.display();
    }
}
