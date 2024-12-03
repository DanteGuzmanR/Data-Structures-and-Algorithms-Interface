#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <cmath>
#include <climits>

// Estructura para las aristas del grafo
struct Edge {
    int target;
    int weight;
};

// Función para dibujar texto
void drawText(sf::RenderWindow& window, const std::string& text, float x, float y, unsigned int size, const sf::Font& font, sf::Color color = sf::Color::White) {
    sf::Text sfText(text, font, size);
    sfText.setPosition(x, y);
    sfText.setFillColor(color);
    sfText.setStyle(sf::Text::Bold);
    window.draw(sfText);
}

// Función para dibujar un nodo
void drawNode(sf::RenderWindow& window, const sf::Vector2f& position, const std::string& label, const sf::Font& font) {
    sf::CircleShape circle(25);
    circle.setFillColor(sf::Color(0, 128, 255)); // Azul claro
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(3);
    circle.setPosition(position - sf::Vector2f(25, 25));
    window.draw(circle);

    sf::Text text(label, font, 20);
    text.setPosition(position - sf::Vector2f(10, 10));
    text.setFillColor(sf::Color::White);
    window.draw(text);
}

// Función para dibujar una arista con pesos visibles
void drawEdge(sf::RenderWindow& window, const sf::Vector2f& from, const sf::Vector2f& to, int weight, const sf::Font& font, std::set<std::pair<int, int>>& drawnEdges, int nodeFrom, int nodeTo) {
    if (drawnEdges.count({nodeFrom, nodeTo}) || drawnEdges.count({nodeTo, nodeFrom})) return;
    drawnEdges.insert({nodeFrom, nodeTo});

    sf::Vertex line[] = {
        sf::Vertex(from, sf::Color::White),
        sf::Vertex(to, sf::Color::White)
    };
    window.draw(line, 2, sf::Lines);

    sf::Vector2f midPoint = (from + to) / 2.0f;
    sf::Text weightText(std::to_string(weight), font, 18);
    weightText.setPosition(midPoint.x - 10, midPoint.y - 10);
    weightText.setFillColor(sf::Color::Yellow);
    window.draw(weightText);
}

// Función para ejecutar Dijkstra
std::vector<int> dijkstra(const std::unordered_map<int, std::vector<Edge>>& graph, int start, int end) {
    std::unordered_map<int, int> distances;
    std::unordered_map<int, int> previous;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;

    for (const auto& [node, _] : graph) {
        distances[node] = INT_MAX;
        previous[node] = -1;
    }
    distances[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int current = pq.top().second;
        pq.pop();

        if (current == end) break;

        for (const auto& edge : graph.at(current)) {
            int newDist = distances[current] + edge.weight;
            if (newDist < distances[edge.target]) {
                distances[edge.target] = newDist;
                previous[edge.target] = current;
                pq.push({newDist, edge.target});
            }
        }
    }

    std::vector<int> path;
    for (int at = end; at != -1; at = previous[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

// Función principal para visualizar Dijkstra
void dijkstraVisualization(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("GOTHIC.TTF")) {
        throw std::runtime_error("No se pudo cargar la fuente GOTHIC.TTF.");
    }

    std::unordered_map<int, sf::Vector2f> positions = {
        {0, {150, 500}}, {1, {300, 400}}, {2, {450, 500}},
        {3, {150, 700}}, {4, {450, 700}}, {5, {600, 600}},
        {6, {300, 300}}, {7, {700, 500}}
    };

    std::unordered_map<int, std::vector<Edge>> graph = {
        {0, {{1, 2}, {3, 4}}},
        {1, {{0, 2}, {2, 5}, {4, 3}}},
        {2, {{1, 5}, {5, 2}}},
        {3, {{0, 4}, {4, 3}}},
        {4, {{1, 3}, {3, 3}, {5, 2}}},
        {5, {{2, 2}, {4, 2}, {7, 5}}},
        {6, {{1, 1}, {7, 6}}},
        {7, {{5, 5}, {6, 6}}}
    };

    int startNode = -1, endNode = -1;
    std::vector<int> path;
    std::string infoText = "Ingrese nodo inicial";
    std::string inputText = "";
    bool enteringStart = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) return;
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Backspace && !inputText.empty()) inputText.pop_back();
            if (event.type == sf::Event::TextEntered && std::isdigit(static_cast<unsigned char>(event.text.unicode))) inputText += static_cast<char>(event.text.unicode);

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                if (enteringStart) {
                    startNode = std::stoi(inputText);
                    infoText = "Ingrese nodo final";
                    inputText.clear();
                    enteringStart = false;
                } else {
                    endNode = std::stoi(inputText);
                    path = dijkstra(graph, startNode, endNode);
                    inputText.clear();
                }
            }
        }

        window.clear(sf::Color::Black);
        drawText(window, "Dijkstra", 20, 20, 36, font, sf::Color::White);
        drawText(window, infoText, 20, 80, 20, font, sf::Color::White);
        drawText(window, "Entrada: " + inputText, 20, 120, 20, font, sf::Color::Cyan);
        drawText(window, "Presione S para salir", 20, 160, 18, font, sf::Color::Yellow);

        std::set<std::pair<int, int>> drawnEdges;
        for (const auto& [node, edges] : graph) {
            for (const auto& edge : edges) {
                drawEdge(window, positions[node], positions[edge.target], edge.weight, font, drawnEdges, node, edge.target);
            }
        }

        for (const auto& [node, position] : positions) {
            drawNode(window, position, std::to_string(node), font);
        }

        if (!path.empty()) {
            std::string result = "Camino mas corto: ";
            for (int node : path) {
                result += std::to_string(node) + " -> ";
            }
            result = result.substr(0, result.size() - 4); // Quitar la última flecha
            drawText(window, result, 10, 750, 20, font);
        }

        window.display();
    }
}
