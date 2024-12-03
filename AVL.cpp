#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <memory>
#include <algorithm>

// Nodo del árbol AVL
struct AVLNode {
    int value;
    int height;
    std::shared_ptr<AVLNode> left;
    std::shared_ptr<AVLNode> right;

    AVLNode(int v) : value(v), height(1), left(nullptr), right(nullptr) {}
};

// Obtener la altura del nodo
int getHeight(std::shared_ptr<AVLNode> node) {
    return node ? node->height : 0;
}

// Calcular el factor de balance
int getBalance(std::shared_ptr<AVLNode> node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

// Rotación hacia la derecha
std::shared_ptr<AVLNode> rotateRight(std::shared_ptr<AVLNode> y) {
    auto x = y->left;
    auto T = x->right;

    x->right = y;
    y->left = T;

    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// Rotación hacia la izquierda
std::shared_ptr<AVLNode> rotateLeft(std::shared_ptr<AVLNode> x) {
    auto y = x->right;
    auto T = y->left;

    y->left = x;
    x->right = T;

    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Insertar un nodo en el árbol AVL
std::shared_ptr<AVLNode> insert(std::shared_ptr<AVLNode> node, int value) {
    if (!node) return std::make_shared<AVLNode>(value);

    if (value < node->value) {
        node->left = insert(node->left, value);
    } else if (value > node->value) {
        node->right = insert(node->right, value);
    } else {
        return node; // No se permiten duplicados
    }

    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    int balance = getBalance(node);

    if (balance > 1 && value < node->left->value) {
        return rotateRight(node);
    }
    if (balance < -1 && value > node->right->value) {
        return rotateLeft(node);
    }
    if (balance > 1 && value > node->left->value) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && value < node->right->value) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// Buscar el nodo con el valor mínimo
std::shared_ptr<AVLNode> findMin(std::shared_ptr<AVLNode> node) {
    while (node->left) {
        node = node->left;
    }
    return node;
}

// Eliminar un nodo del árbol AVL
std::shared_ptr<AVLNode> remove(std::shared_ptr<AVLNode> node, int value) {
    if (!node) return nullptr;

    if (value < node->value) {
        node->left = remove(node->left, value);
    } else if (value > node->value) {
        node->right = remove(node->right, value);
    } else {
        if (!node->left || !node->right) {
            node = node->left ? node->left : node->right;
        } else {
            auto minNode = findMin(node->right);
            node->value = minNode->value;
            node->right = remove(node->right, minNode->value);
        }
    }

    if (!node) return node;

    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->left) >= 0) {
        return rotateRight(node);
    }
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(node);
    }
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// Dibujar los nodos del árbol AVL
void drawTree(sf::RenderWindow &window, const std::shared_ptr<AVLNode> &node, const sf::Font &font, float x, float y, float offsetX) {
    if (!node) return;

    if (node->left) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, y), sf::Color::White),
            sf::Vertex(sf::Vector2f(x - offsetX, y + 120), sf::Color::White)};
        window.draw(line, 2, sf::Lines);
        drawTree(window, node->left, font, x - offsetX, y + 120, offsetX / 1.5f);
    }

    if (node->right) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, y), sf::Color::White),
            sf::Vertex(sf::Vector2f(x + offsetX, y + 120), sf::Color::White)};
        window.draw(line, 2, sf::Lines);
        drawTree(window, node->right, font, x + offsetX, y + 120, offsetX / 1.5f);
    }

    sf::CircleShape circle(25);
    circle.setFillColor(sf::Color::Blue);
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(2);
    circle.setPosition(x - 25, y - 25);
    window.draw(circle);

    sf::Text text;
    text.setFont(font);
    text.setString(std::to_string(node->value));
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(x - 10, y - 15);
    window.draw(text);
}

// Función principal para manejar el árbol AVL
void avlTree(sf::RenderWindow &window) {
    sf::Font font;
    font.loadFromFile("GOTHIC.ttf");

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("AVL Tree");
    titleText.setCharacterSize(36);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(10, 10);

    sf::Text modeText;
    modeText.setFont(font);
    modeText.setCharacterSize(24);
    modeText.setFillColor(sf::Color::White);
    modeText.setStyle(sf::Text::Bold);
    modeText.setPosition(10, 60);

    sf::Text exitText;
    exitText.setFont(font);
    exitText.setCharacterSize(18);
    exitText.setFillColor(sf::Color::White);
    exitText.setString("Presione 'S' para salir.");
    exitText.setPosition(10, 140);

    std::shared_ptr<AVLNode> root = nullptr;
    std::string inputBuffer = "";
    bool isInsertMode = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::S) {
                    return;
                } else if (event.key.code == sf::Keyboard::I) {
                    isInsertMode = true;
                } else if (event.key.code == sf::Keyboard::D) {
                    isInsertMode = false;
                }
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b') { // Borrar
                    if (!inputBuffer.empty()) {
                        inputBuffer.pop_back();
                    }
                } else if (event.text.unicode == '\r') { // Enter
                    try {
                        int value = std::stoi(inputBuffer);
                        if (isInsertMode) {
                            root = insert(root, value);
                        } else {
                            root = remove(root, value);
                        }
                        inputBuffer.clear();
                    } catch (...) {
                        inputBuffer.clear();
                    }
                } else if (event.text.unicode < 128) { // Agregar caracteres
                    inputBuffer += static_cast<char>(event.text.unicode);
                }
            }
        }

        window.clear(sf::Color::Black);

        window.draw(titleText);
        modeText.setString("Modo actual: " + std::string(isInsertMode ? "Insercion (I)" : "Eliminacion (D)"));
        window.draw(modeText);

        sf::Text instructionText;
        instructionText.setFont(font);
        instructionText.setCharacterSize(18);
        instructionText.setFillColor(sf::Color::White);
        instructionText.setPosition(10, 100);

        if (isInsertMode) {
            instructionText.setString("Presione D para cambiar al modo de eliminacion");
        } else {
            instructionText.setString("Presione I para cambiar al modo de insercion");
        }
        window.draw(instructionText);

        window.draw(exitText);

        sf::Text inputText;
        inputText.setFont(font);
        inputText.setCharacterSize(24);
        inputText.setFillColor(sf::Color::White);
        inputText.setString("Ingrese un valor: " + inputBuffer);
        inputText.setPosition(10, 180);
        window.draw(inputText);

        drawTree(window, root, font, 600, 200, 200);

        window.display();
    }
}
