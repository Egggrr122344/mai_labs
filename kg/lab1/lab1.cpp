#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glu.h>
#include <sstream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void initOpenGL() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawRectangle() {
    glBegin(GL_QUADS);
    glVertex2f(-50.f, -25.f);
    glVertex2f(50.f, -25.f);
    glVertex2f(50.f, 25.f);
    glVertex2f(-50.f, 25.f);
    glEnd();
}

sf::Color getColorByDirection(bool up, bool down, bool left, bool right, bool rotateLeft, bool rotateRight) {
    if (up) return sf::Color(255, 182, 193);
    if (down) return sf::Color(144, 238, 144);
    if (left) return sf::Color(173, 216, 230);
    if (right) return sf::Color(255, 255, 224);
    if (rotateLeft) return sf::Color(255, 255, 120);
    if (rotateRight) return sf::Color(120, 255, 100);
    return sf::Color(211, 211, 211);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Rectangle with Transformations", sf::Style::Default, sf::ContextSettings(24));
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        return -1;
    }

    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(20);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition(10, 10);

    initOpenGL();

    float posX = 400.f, posY = 300.f;
    float scale = 1.f;
    float angle = 0.f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) posY -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) posY += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) posX -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) posX += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) angle -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) angle += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) scale *= 0.99f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) scale *= 1.01f;

        sf::Color color = getColorByDirection(
            sf::Keyboard::isKeyPressed(sf::Keyboard::W),
            sf::Keyboard::isKeyPressed(sf::Keyboard::S),
            sf::Keyboard::isKeyPressed(sf::Keyboard::A),
            sf::Keyboard::isKeyPressed(sf::Keyboard::D),
            sf::Keyboard::isKeyPressed(sf::Keyboard::Q),
            sf::Keyboard::isKeyPressed(sf::Keyboard::E)
        );

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(posX, posY, 0.f);
        glRotatef(angle, 0.f, 0.f, 1.f);
        glScalef(scale, scale, 1.f);

        glColor3f(color.r / 255.f, color.g / 255.f, color.b / 255.f);
        drawRectangle();

        std::stringstream ss;
        ss << "Position: (" << posX << ", " << posY << ")\n";
        ss << "Scale: " << scale << "\n";
        ss << "Rotation: " << angle << " degrees";
        infoText.setString(ss.str());

        window.pushGLStates();
        window.draw(infoText);
        window.popGLStates();

        window.display();
    }

    return 0;
}
