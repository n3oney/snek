#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <future>

#define kFPS 15 // The more fps, the faster the game goes.
#define kFullscreen false // Self-explanatory
#define kSolidWalls false // If set to false, you won't die from impact with the window's border, but teleport to the other side

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

[[maybe_unused]] bool isOutOfBounds(Vector2<float> position, Vector2<float> size) {
    return position.x < 0 || position.x >g_window.getSize().x - size.x || position.y < 0 || position.y > g_window.getSize().y - size.y;
}

[[maybe_unused]] bool isOutOfBounds(Vector2<int> position, Vector2<int> size) {
    return position.x < 0 || position.x > g_window.getSize().x - size.x || position.y < 0 || position.y > g_window.getSize().y - size.y;
}

[[maybe_unused]] bool isOutOfBounds(Vector2<unsigned int> position, Vector2<unsigned int> size) {
    return position.x < 0 || position.x > g_window.getSize().x - size.x || position.y < 0 || position.y > g_window.getSize().y - size.y;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"
#pragma ide diagnostic ignored "UnreachableCode"
[[maybe_unused]] Vector2<float> moveBy(Vector2<float> vector2, Direction direction, float amount = 0.0f) {
    Vector2 newVector = vector2;
    switch(direction) {
        case Up:
            newVector.y -= amount;
            break;
        case Down:
            newVector.y += amount;
            break;
        case Left:
            newVector.x -= amount;
            break;
        case Right:
            newVector.x += amount;
            break;
    }
    if(isOutOfBounds(newVector, g_playerShapes[0].getSize())) {
        if(kSolidWalls) return vector2;
        else {
            Vector2 size = g_window.getSize();
            if(direction == Right) return Vector2(0.0f, vector2.y);
            else if(direction == Left) return Vector2(size.x - 20.0f, vector2.y);
            else if(direction == Down) return Vector2(vector2.x, 0.0f);
            else if(direction == Up) return Vector2(vector2.x, size.y - 20.0f);
        }
    }
    return newVector;
}
#pragma clang diagnostic pop

Vector2<float> getNewApplePosition() {
    float x = rand() % (g_window.getSize().x / 20) * 20; // NOLINT(cppcoreguidelines-narrowing-conversions)
    float y = rand() % (g_window.getSize().y / 20) * 20; // NOLINT(cppcoreguidelines-narrowing-conversions)

    for(std::vector<int>::size_type i = 0; i != g_playerShapes.size(); i++) {
        Vector2 position = g_playerShapes[i].getPosition();
        if(position.x == x && position.y == y) {
            return getNewApplePosition();
        }
    }

    return Vector2(x, y);
}


int main() {
    Vector2 windowSize = g_window.getSize();
    unsigned int amountOfSquares = (windowSize.x / 20) * (windowSize.y / 20);
    Direction lastDirection = Left;
    Direction nextDirection = Left;
    Vector2 playerPosition((float)windowSize.x - 20, (float)windowSize.y - 20);
    g_window.setFramerateLimit(kFPS);
    g_window.setKeyRepeatEnabled(false);
    g_playerShapes[0].setFillColor(Color(50, 255, 50));
    g_playerShapes[0].setPosition(playerPosition);

    g_apple.setFillColor(Color::Red);
    g_apple.setPosition((float)(floor(windowSize.x / 40.0) * 20.0), (float)(floor(windowSize.y / 40.0) * 20.0));

    RectangleShape background(Vector2(20.0f, 20.0f));
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
                        newRectangle.setFillColor(Color(rand() % 55 + 200, rand() % 55 + 200, rand() % 55 + 200));
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

        playerPosition = moveBy(playerPosition, nextDirection, 20);
        lastDirection = nextDirection;

        if(playerPosition == g_apple.getPosition()) {
            g_apple.setPosition(getNewApplePosition());
            RectangleShape newRectangle(Vector2(20.0f, 20.0f));
            newRectangle.setFillColor(Color(rand() % 55 + 200, rand() % 55 + 200, rand() % 55 + 200));
            newRectangle.setPosition(g_playerShapes.size() == 1 ? oldPosition : g_playerShapes.back().getPosition());
            g_playerShapes.push_back(newRectangle);
            progressText.setString(std::to_string(g_playerShapes.size()) + "/" + std::to_string(amountOfSquares));
        } else {
            for(std::vector<int>::size_type i = 1; i != g_playerShapes.size(); i++) {
                if(g_playerShapes[0].getPosition() == g_playerShapes[i].getPosition()) {
                    g_playerShapes = { g_playerShapes[0] };
                    g_apple.setPosition((float)(floor(g_window.getSize().x / 40.0) * 20.0), (float)(floor(g_window.getSize().y / 40.0) * 20.0));
                    progressText.setString("1/" + std::to_string(amountOfSquares));
                    break;
                }
            }
        }

        g_playerShapes[0].setPosition(playerPosition);
        g_window.clear();

        for(short x = 0; x <= g_window.getSize().x; x+=20) { // NOLINT(bugprone-too-small-loop-variable)
            for(short y = 0; y <= g_window.getSize().y; y+=20) { // NOLINT(bugprone-too-small-loop-variable)
//                int a = rand() % 2 + 10;
                background.setFillColor((x / 20 + y / 20) % 2 == 1 ? Color(0, 0, 0) : Color(10, 10, 10));
                background.setPosition(x, y);
                g_window.draw(background);
            }
        }

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