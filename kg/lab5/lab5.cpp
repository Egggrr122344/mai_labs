#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <random>

const float PI = 3.14159265359f;
const float EPSILON = 1e-4f;
const int IMAGE_WIDTH = 800;
const int IMAGE_HEIGHT = 400;
const int SAMPLES = 40;

struct Vec3 {
    float x, y, z;

    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3 &v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3 &v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
    Vec3 operator/(float s) const { return Vec3(x / s, y / s, z / s); }

    float dot(const Vec3 &v) const { return x * v.x + y * v.y + z * v.z; }

    Vec3 normalize() const {
        float len = std::sqrt(x * x + y * y + z * z);
        return *this / len;
    }
};

struct Ray {
    Vec3 origin, direction;
    Ray(const Vec3 &origin, const Vec3 &direction) : origin(origin), direction(direction.normalize()) {}
};

struct Sphere {
    Vec3 center;
    float radius;

    Sphere(const Vec3 &center, float radius) : center(center), radius(radius) {}

    bool intersect(const Ray &ray, float &t) const {
        Vec3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) return false;
        float t0 = (-b - std::sqrt(discriminant)) / (2.0f * a);
        float t1 = (-b + std::sqrt(discriminant)) / (2.0f * a);
        t = (t0 > EPSILON) ? t0 : t1;
        return t > EPSILON;
    }
};

struct Plane {
    Vec3 point, normal;

    Plane(const Vec3 &point, const Vec3 &normal) : point(point), normal(normal.normalize()) {}

    bool intersect(const Ray &ray, float &t) const {
        float denom = normal.dot(ray.direction);
        if (std::fabs(denom) < EPSILON) return false;
        t = (point - ray.origin).dot(normal) / denom;
        return t > EPSILON;
    }
};

struct Light {
    Vec3 position;
    float radius;

    Light(const Vec3 &position, float radius) : position(position), radius(radius) {}

    Vec3 sampleLight(std::mt19937 &gen, std::uniform_real_distribution<float> &dist) const {
        float theta = dist(gen) * 2.0f * PI;
        float phi = std::acos(1.0f - 2.0f * dist(gen));
        float x = radius * std::sin(phi) * std::cos(theta);
        float y = radius * std::sin(phi) * std::sin(theta);
        float z = radius * std::cos(phi);
        return position + Vec3(x, y, z);
    }
};

Vec3 trace(const Ray &ray, const Sphere &sphere, const Plane &plane, const Light &light, std::mt19937 &gen, std::uniform_real_distribution<float> &dist) {
    float t_sphere = std::numeric_limits<float>::max();
    float t_plane = std::numeric_limits<float>::max();
    Vec3 hit_color(0, 0, 0);

    bool hit_sphere = sphere.intersect(ray, t_sphere);
    bool hit_plane = plane.intersect(ray, t_plane);

    if (!hit_sphere && !hit_plane) return Vec3(0.2, 0.7, 0.8);

    Vec3 hit_point;
    Vec3 normal;

    if (hit_sphere && (t_sphere < t_plane)) {
        hit_point = ray.origin + ray.direction * t_sphere;
        normal = (hit_point - sphere.center).normalize();
        hit_color = Vec3(1, 0, 0);
    } else {
        hit_point = ray.origin + ray.direction * t_plane;
        normal = plane.normal;
        hit_color = Vec3(0.5, 0.5, 0.5);
    }

    Vec3 light_contrib(0, 0, 0);
    for (int i = 0; i < SAMPLES; ++i) {
        Vec3 light_pos = light.sampleLight(gen, dist);
        Vec3 light_dir = (light_pos - hit_point).normalize();
        Ray shadow_ray(hit_point + normal * EPSILON, light_dir);

        float t_shadow_sphere, t_shadow_plane;
        if (!sphere.intersect(shadow_ray, t_shadow_sphere) && !plane.intersect(shadow_ray, t_shadow_plane)) {
            float light_intensity = std::max(0.0f, normal.dot(light_dir));
            light_contrib = light_contrib + Vec3(1, 1, 1) * light_intensity;
        }
    }

    light_contrib = light_contrib * (1.0f / SAMPLES);
    return hit_color * 0.5f + light_contrib * 0.5f;
}

void renderToTexture(sf::Image &image, const Sphere &sphere, const Plane &plane, const Light &light) {
    float aspect_ratio = float(IMAGE_WIDTH) / float(IMAGE_HEIGHT);
    float fov = PI / 3.0f;

    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (int y = 0; y < IMAGE_HEIGHT; ++y) {
        for (int x = 0; x < IMAGE_WIDTH; ++x) {
            float u = (2.0f * (x + 0.5f) / IMAGE_WIDTH - 1.0f) * aspect_ratio * std::tan(fov / 2.0f);
            float v = (1.0f - 2.0f * (y + 0.5f) / IMAGE_HEIGHT) * std::tan(fov / 2.0f);

            Ray ray(Vec3(0, 1, 3), Vec3(u, v, -1).normalize());
            Vec3 color = trace(ray, sphere, plane, light, gen, dist);

            image.setPixel(x, y, sf::Color(
                (unsigned char)(std::min(1.0f, color.x) * 255),
                (unsigned char)(std::min(1.0f, color.y) * 255),
                (unsigned char)(std::min(1.0f, color.z) * 255)));
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(IMAGE_WIDTH, IMAGE_HEIGHT), "Ray Tracing with Soft Shadows");
    window.setFramerateLimit(60);

    sf::Image image;
    image.create(IMAGE_WIDTH, IMAGE_HEIGHT);

    sf::Texture texture;
    texture.create(IMAGE_WIDTH, IMAGE_HEIGHT);

    sf::Sprite sprite;
    sprite.setTexture(texture);

    Sphere sphere(Vec3(0, 1, -5), 1.0f);
    Plane plane(Vec3(0, 0, 0), Vec3(0, 1, 0));
    Light light(Vec3(2, 4, -3), 1.0f);

    renderToTexture(image, sphere, plane, light);

    texture.update(image);
    sprite.setTexture(texture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
