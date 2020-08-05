#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <SFML/Graphics.hpp>
#include <vector>

#define kFPS 15
#define kFullscreen false

using namespace sf;

enum Direction {
    Right = 1,
    Left = 2,
    Up = 3,
    Down = 4
};

RenderWindow g_window(VideoMode(1000, 500), "snek", kFullscreen ? sf::Style::Fullscreen : sf::Style::Close, ContextSettings(0, 0, 8));
std::vector<RectangleShape> g_playerShapes = { RectangleShape(Vector2(20.0f, 20.0f)) };
CircleShape g_apple(10.0f, 20);

[[maybe_unused]] bool isOutOfBounds(Vector2<float> position, Vector2<float> size, RenderWindow *window) {
    return position.x < 0 || position.x > window->getSize().x - size.x || position.y < 0 || position.y > window->getSize().y - size.y;
}

[[maybe_unused]] bool isOutOfBounds(Vector2<int> position, Vector2<int> size, RenderWindow *window) {
    return position.x < 0 || position.x > window->getSize().x - size.x || position.y < 0 || position.y > window->getSize().y - size.y;
}

[[maybe_unused]] bool isOutOfBounds(Vector2<unsigned int> position, Vector2<unsigned int> size, RenderWindow *window) {
    return position.x < 0 || position.x > window->getSize().x - size.x || position.y < 0 || position.y > window->getSize().y - size.y;
}



[[maybe_unused]] Vector2<float> moveBy(Vector2<float> vector2, float x = 0.0f, float y = 0.0f) {
    Vector2 newVector(vector2.x + x, vector2.y + y);
    if(isOutOfBounds(newVector, g_playerShapes[0].getSize(), &g_window)) return vector2;
    else return newVector;
}

[[maybe_unused]] Vector2<int> moveBy(Vector2<int> vector2, int x = 0, int y = 0) {
    return Vector2(vector2.x + x, vector2.y + y);
}

[[maybe_unused]] Vector2<unsigned int> moveBy(Vector2<unsigned int> vector2, unsigned int x = 0, unsigned int y = 0) {
    return Vector2(vector2.x + x, vector2.y + y);
}

Vector2<float> getNewApplePosition() {
    float x = rand() % 50 * 20; // NOLINT(cppcoreguidelines-narrowing-conversions)
    float y = rand() % 25 * 20; // NOLINT(cppcoreguidelines-narrowing-conversions)

    for(std::vector<int>::size_type i = 0; i != g_playerShapes.size(); i++) {
        Vector2 position = g_playerShapes[i].getPosition();
        if(position.x == x && position.y == y) {
            return getNewApplePosition();
        }
    }

    return Vector2(x, y);
}

int main() {
    unsigned int amountOfSquares = (g_window.getSize().x / 20) * (g_window.getSize().y / 20);
    Direction lastDirection = Left;
    Direction nextDirection = Left;
    Vector2 playerPosition(980.0f, 480.0f);
    g_window.setFramerateLimit(kFPS);
    g_window.setKeyRepeatEnabled(false);
    g_playerShapes[0].setFillColor(Color::Red);
    g_playerShapes[0].setPosition(playerPosition);

    g_apple.setFillColor(Color::Green);
    g_apple.setPosition(480, 240);

    RectangleShape background(Vector2((float)g_window.getSize().x, (float)g_window.getSize().y));
    background.setFillColor(Color(30, 40, 50));
    Font font{};
    font.loadFromFile("./PressStart2P-Regular.ttf");
    Text progressText("1/" + std::to_string(amountOfSquares), font);
    progressText.setCharacterSize(16);
    progressText.setOutlineColor(Color::White);
    progressText.setPosition(10, 10);

    while (g_window.isOpen())
    {
        Vector2 oldPosition = playerPosition;
        Event event{};

        while (g_window.pollEvent(event))
        {
            if(event.type == Event::KeyPressed)
                switch (event.key.code) {
                    #ifndef NDEBUG
                    case Keyboard::Key::Enter: {
                        RectangleShape newRectangle(Vector2(20.0f, 20.0f));
                        newRectangle.setFillColor(Color::White);
                        newRectangle.setPosition(g_playerShapes.size() == 1 ? oldPosition : g_playerShapes.back().getPosition());
                        g_playerShapes.push_back(newRectangle);
                        progressText.setString(std::to_string(g_playerShapes.size()) + "/" + std::to_string(amountOfSquares));
                        break;
                    }
                    #endif
                    case Keyboard::Key::Escape: {
                        g_window.close();
                        break;
                    }
                    case Keyboard::Key::Left: {
                        if(lastDirection == Right && g_playerShapes.size() > 1) break;
                        nextDirection = Left;
                        break;
                    }
                    case Keyboard::Key::Right: {
                        if(lastDirection == Left && g_playerShapes.size() > 1) break;
                        nextDirection = Right;
                        break;
                    }
                    case Keyboard::Key::Up: {
                        if(lastDirection == Down && g_playerShapes.size() > 1) break;
                        nextDirection = Up;
                        break;
                    }
                    case Keyboard::Key::Down: {
                        if(lastDirection == Up && g_playerShapes.size() > 1) break;
                        nextDirection = Down;
                        break;
                    }
                    default:
                        break;
                }
            else if (event.type == Event::Closed)
                g_window.close();
        }

        switch(nextDirection) {
            case Left:
                playerPosition = moveBy(playerPosition, -20);
                break;
            case Right:
                playerPosition = moveBy(playerPosition, 20);
                break;
            case Up:
                playerPosition = moveBy(playerPosition, 0, -20);
                break;
            case Down:
                playerPosition = moveBy(playerPosition, 0, 20);
                break;
        }
        lastDirection = nextDirection;

        if(playerPosition == g_apple.getPosition()) {
            g_apple.setPosition(getNewApplePosition());
            RectangleShape newRectangle(Vector2(20.0f, 20.0f));
            newRectangle.setFillColor(Color::White);
            newRectangle.setPosition(g_playerShapes.size() == 1 ? oldPosition : g_playerShapes.back().getPosition());
            g_playerShapes.push_back(newRectangle);
            progressText.setString(std::to_string(g_playerShapes.size()) + "/" + std::to_string(amountOfSquares));
        } else {
            for(std::vector<int>::size_type i = 1; i != g_playerShapes.size(); i++) {
                if(g_playerShapes[0].getPosition() == g_playerShapes[i].getPosition()) {
                    g_playerShapes = { RectangleShape(Vector2(20.0f, 20.0f)) };
                    g_playerShapes[0].setFillColor(Color::Red);
                    g_playerShapes[0].setPosition(playerPosition);

                    g_apple.setPosition(480, 240);
                    progressText.setString("1/" + std::to_string(amountOfSquares));
                    break;
                }
            }
        }

        g_playerShapes[0].setPosition(playerPosition);
        g_window.clear();
        g_window.draw(background);
        for(std::vector<int>::size_type i = g_playerShapes.size() - 1; i >= 1; i--) {
            if(i == 1) g_playerShapes[i].setPosition(oldPosition);
            else g_playerShapes[i].setPosition(g_playerShapes[i-1].getPosition());
            g_window.draw(g_playerShapes[i]);
        }
        g_window.draw(g_playerShapes[0]);
        g_window.draw(g_apple);
        g_window.draw(progressText);
        g_window.display();
    }

    return 0;
}