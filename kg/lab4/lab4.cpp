#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glu.h>
#include <cmath>

// Положение камеры
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 5.0f;
// Углы камеры
float pitch = 0.0f, yaw = -90.0f;
// Скорость камеры
float cameraSpeed = 0.05f, mouseSensitivity = 0.1f;

// Параметры для источника света
float lightPosX = 0.0f, lightPosY = 2.0f, lightPosZ = 2.0f;
float lightSpeed = 0.1f;

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

// Отрисовка сферы с улучшенной раскраской для отслеживания освещения
void drawSphere(float x, float y, float z, float radius, int segments) {
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

            // Плавное изменение цвета в оттенках серого
            float factor = (float)j / segments;
            float gray = factor * 0.8f + 0.2f;  // Оттенки серого (от темного к светлому)

            glColor3f(gray, gray, gray);  // Серый цвет

            glVertex3f(x + radius * xSegment * zr0, y + radius * ySegment * zr0, z + radius * z0);
            glVertex3f(x + radius * xSegment * zr1, y + radius * ySegment * zr1, z + radius * z1);
        }
        glEnd();
    }
}

// Установка перспективы
void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspectRatio, nearPlane, farPlane);
    glMatrixMode(GL_MODELVIEW);
}

// Инициализация освещения
void setupLighting() {
    glEnable(GL_LIGHTING);  // Включить освещение
    glEnable(GL_LIGHT0);    // Включить источник света 0

    // Позиция точечного источника света
    GLfloat light_position[] = { lightPosX, lightPosY, lightPosZ, 1.0f };  // Последний параметр 1.0f означает точечный свет
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Диффузное освещение (цвет света)
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    // Спекулярное освещение (для бликов)
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // Амбиентное освещение (общая подсветка)
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    // Настройка материала объекта
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Цвет бликов
    GLfloat mat_shininess[] = { 50.0f };  // Интенсивность бликов (чем больше, тем ярче и меньше блики)
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_COLOR_MATERIAL);  // Включаем использование материалов для цвета
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "3D Shapes with Lighting", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    glEnable(GL_DEPTH_TEST);  // Включить тест глубины для 3D-объектов
    setPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);  // Установить перспективу
    setupLighting();  // Настройка освещения



    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Управление движением камеры
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

        // Управление источником света
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            lightPosY += lightSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            lightPosY -= lightSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            lightPosX -= lightSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            lightPosX += lightSpeed;
        }

        // Обновление экрана
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Очистка буферов
        glLoadIdentity();
        glTranslatef(-cameraX, -cameraY, -cameraZ);  // Перемещение камеры

        setupLighting();  // Обновляем освещение
        drawSphere(0.0f, 0.0f, 0.0f, 1.0f, 20);  // Отрисовка сферы

        window.display();
    }

    return 0;
}
