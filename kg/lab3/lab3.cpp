#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glu.h>
#include <cmath>

// Положение камеры
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 5.0f;
// Углы камеры
float pitch = 0.0f, yaw = -90.0f; // в горизонтальной и вертикалной плоскости 
// Скорость камеры
float cameraSpeed = 0.05f, mouseSensitivity = 0.1f;

// Параметры для обработки мыши
bool firstMouse = true;
float lastX = 400, lastY = 300;

// Конвертация градусов в радианы
float toRadians(float degrees) {
    return degrees * M_PI / 180.0f;
}

// Вектор направления камеры
void getDirection(float &dirX, float &dirY, float &dirZ) {
    dirX = cosf(toRadians(yaw)) * cosf(toRadians(pitch));
    dirY = sinf(toRadians(pitch));
    dirZ = sinf(toRadians(yaw)) * cosf(toRadians(pitch));
}

// Отрисовка куба 
void drawCube(float x, float y, float z, float size) {
    float half = size / 2.0f;
    glBegin(GL_QUADS);

    // Цвета для каждой грани
    glColor3f(0.95f, 0.64f, 0.37f); 
    glVertex3f(x - half, y - half, z + half);
    glVertex3f(x + half, y - half, z + half);
    glVertex3f(x + half, y + half, z + half);
    glVertex3f(x - half, y + half, z + half);

    glColor3f(0.44f, 0.76f, 0.96f); 
    glVertex3f(x - half, y - half, z - half);
    glVertex3f(x - half, y + half, z - half);
    glVertex3f(x + half, y + half, z - half);
    glVertex3f(x + half, y - half, z - half);

    glColor3f(0.58f, 0.88f, 0.57f); 
    glVertex3f(x - half, y - half, z - half);
    glVertex3f(x - half, y - half, z + half);
    glVertex3f(x - half, y + half, z + half);
    glVertex3f(x - half, y + half, z - half);

    glColor3f(0.98f, 0.74f, 0.44f); 
    glVertex3f(x + half, y - half, z - half);
    glVertex3f(x + half, y + half, z - half);
    glVertex3f(x + half, y + half, z + half);
    glVertex3f(x + half, y - half, z + half);

    glColor3f(0.73f, 0.47f, 0.85f);
    glVertex3f(x - half, y - half, z - half);
    glVertex3f(x + half, y - half, z - half);
    glVertex3f(x + half, y - half, z + half);
    glVertex3f(x - half, y - half, z + half);

    glColor3f(0.96f, 0.83f, 0.56f); 
    glVertex3f(x - half, y + half, z - half);
    glVertex3f(x - half, y + half, z + half);
    glVertex3f(x + half, y + half, z + half);
    glVertex3f(x + half, y + half, z - half);

    glEnd();
}

// Отрисовка сферы с  градиентами
// Сфера отрисовывается с использованием GL_QUAD_STRIP,
//  где каждая лента квадов формирует часть поверхности сферы.
//   Цвет меняется плавно по градиенту.
void drawSphere(float x, float y, float z, float radius, int segments) {
    glColor3f(0.67f, 0.84f, 0.90f); 
    for (int i = 0; i <= segments; ++i) {
        float lat0 = M_PI * (-0.5f + (float)(i - 1) / segments);
        float z0 = sinf(lat0);
        float zr0 = cosf(lat0);

        float lat1 = M_PI * (-0.5f + (float)i / segments);
        float z1 = sinf(lat1);
        float zr1 = cosf(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= segments; ++j) {
            float lng = 2 * M_PI * (float)(j - 1) / segments;
            float xSegment = cosf(lng);
            float ySegment = sinf(lng);

            // Плавное изменение цвета
            float factor = (float)j / segments;
            glColor3f(0.67f * factor, 0.84f * factor, 0.90f);

            glVertex3f(x + radius * xSegment * zr0, y + radius * ySegment * zr0, z + radius * z0);
            glVertex3f(x + radius * xSegment * zr1, y + radius * ySegment * zr1, z + radius * z1);
        }
        glEnd();
    }
}

// Отрисовка пирамиды
void drawPyramid(float x, float y, float z, float size) {
    float half = size / 2.0f;

    // Верхняя вершина пирамиды
    float apexX = x;
    float apexY = y + half;
    float apexZ = z;

    // Нижние вершины пирамиды
    float base1X = x - half, base1Y = y - half, base1Z = z - half;
    float base2X = x + half, base2Y = y - half, base2Z = z - half;
    float base3X = x + half, base3Y = y - half, base3Z = z + half;
    float base4X = x - half, base4Y = y - half, base4Z = z + half;

    glBegin(GL_TRIANGLES);



    // Передняя грань (оранжевый)
    glColor3f(0.9f, 0.5f, 0.3f);
    glVertex3f(apexX, apexY, apexZ);
    glVertex3f(base1X, base1Y, base1Z);
    glVertex3f(base2X, base2Y, base2Z);

    // Правая грань (желто-оранжевый)
    glColor3f(0.9f, 0.7f, 0.4f);
    glVertex3f(apexX, apexY, apexZ);
    glVertex3f(base2X, base2Y, base2Z);
    glVertex3f(base3X, base3Y, base3Z);

    // Задняя грань (розовый)
    glColor3f(0.8f, 0.4f, 0.5f);
    glVertex3f(apexX, apexY, apexZ);
    glVertex3f(base3X, base3Y, base3Z);
    glVertex3f(base4X, base4Y, base4Z);

    // Левая грань (голубой)
    glColor3f(0.7f, 0.7f, 0.9f);
    glVertex3f(apexX, apexY, apexZ);
    glVertex3f(base4X, base4Y, base4Z);
    glVertex3f(base1X, base1Y, base1Z);

    glEnd();

    // Отрисовка основания пирамиды (светло-серый)
    glBegin(GL_QUADS);
    glColor3f(0.8f, 0.8f, 0.8f);
    glVertex3f(base1X, base1Y, base1Z);
    glVertex3f(base2X, base2Y, base2Z);
    glVertex3f(base3X, base3Y, base3Z);
    glVertex3f(base4X, base4Y, base4Z);
    glEnd();
}



// Установка перспективы
void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspectRatio, nearPlane, farPlane);
    glMatrixMode(GL_MODELVIEW);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "3D Shapes with OpenGL and SFML", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    glEnable(GL_DEPTH_TEST); // Включение теста глубины для корректного отрисовывания объектов ближе к камере
    setPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f); 

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseMoved) {
                float xpos = static_cast<float>(event.mouseMove.x);
                float ypos = static_cast<float>(event.mouseMove.y);

                if (firstMouse) {
                    lastX = xpos;
                    lastY = ypos;
                    firstMouse = false;
                }

                float xOffset = xpos - lastX;
                float yOffset = lastY - ypos;
                lastX = xpos;
                lastY = ypos;

                yaw += xOffset * mouseSensitivity; // горизонтальное движение мыши
                pitch += yOffset * mouseSensitivity; // Вертикальное движение мыши

                if (pitch > 89.0f)
                    pitch = 89.0f;
                if (pitch < -89.0f)
                    pitch = -89.0f;
            }
        }

        float dirX, dirY, dirZ;
        getDirection(dirX, dirY, dirZ);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            cameraX += dirX * cameraSpeed;
            cameraY += dirY * cameraSpeed;
            cameraZ += dirZ * cameraSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            cameraX -= dirX * cameraSpeed;
            cameraY -= dirY * cameraSpeed;
            cameraZ -= dirZ * cameraSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            cameraX -= cameraSpeed * sinf(toRadians(yaw));
            cameraZ += cameraSpeed * cosf(toRadians(yaw));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            cameraX += cameraSpeed * sinf(toRadians(yaw));
            cameraZ -= cameraSpeed * cosf(toRadians(yaw));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            cameraY += cameraSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            cameraY -= cameraSpeed;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();


        float centerX = cameraX + dirX;
        float centerY = cameraY + dirY;
        float centerZ = cameraZ + dirZ;
        gluLookAt(cameraX, cameraY, cameraZ, centerX, centerY, centerZ, 0.0f, 1.0f, 0.0f); // функция задания камеры положение камеры, точка на которую смотрит камера, вектор для ориентации камеры

        drawCube(-1.5f, 0.0f, 0.0f, 1.0f);  // Куб
        drawSphere(0.0f, 0.0f, 0.0f, 0.5f, 16); // Сфера
        drawPyramid(1.5f, 0.0f, 0.0f, 1.0f);   // Пирамида

        window.display();
    }

    return 0;
}
