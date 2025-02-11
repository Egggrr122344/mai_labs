#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glu.h>
#include <cmath>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

float fov = 45.0f;
float rotateX = 0.0f;
float rotateY = 0.0f;

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void drawPyramid() {
    glBegin(GL_TRIANGLES);

    // Front face
    glColor3f(0.8f, 0.4f, 0.4f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);

    // Right face
    glColor3f(0.4f, 0.8f, 0.4f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);

    // Back face
    glColor3f(0.4f, 0.4f, 0.8f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);

    // Left face
    glColor3f(0.8f, 0.8f, 0.4f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);

    glEnd();

    // Base of the pyramid
    glBegin(GL_QUADS);
    glColor3f(0.7f, 0.7f, 0.7f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glEnd();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                            "3D Pyramid with Perspective Projection",
                            sf::Style::Default, sf::ContextSettings(24));
    window.setFramerateLimit(60);

    initOpenGL();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) rotateX += 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) rotateX -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) rotateY -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) rotateY += 1.0f;

        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) fov -= 0.1f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) fov += 0.1f;

        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fov, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1, 100.0);

        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        
        glTranslatef(0.0f, 0.0f, -5.0f);

        
        glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotateY, 0.0f, 1.0f, 0.0f);

        
        drawPyramid();

        
        window.display();
    }

    return 0;
}
