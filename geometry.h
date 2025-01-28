#pragma once

#include "98_raytracer/raytracer-geom/intersection.h"
#include "98_raytracer/raytracer-geom/ray.h"
#include "98_raytracer/raytracer-geom/sphere.h"
#include "98_raytracer/raytracer-geom/triangle.h"
#include "98_raytracer/raytracer-geom/vector.h"
#include "intersection.h"
#include "ray.h"
#include "sphere.h"
#include "triangle.h"
#include "vector.h"

#include <algorithm>
#include <cmath>
#include <optional>

inline bool IsPointInTriangle(const Triangle& t, const Vector& inter) {
    const Vector side1 = {t[1][0] - t[0][0], t[1][1] - t[0][1], t[1][2] - t[0][2]};
    const Vector side2 = {t[2][0] - t[1][0], t[2][1] - t[1][1], t[2][2] - t[1][2]};
    const Vector side3 = {t[0][0] - t[2][0], t[0][1] - t[2][1], t[0][2] - t[2][2]};

    const Vector p1 = {inter[0] - t[0][0], inter[1] - t[0][1], inter[2] - t[0][2]};
    const Vector p2 = {inter[0] - t[1][0], inter[1] - t[1][1], inter[2] - t[1][2]};
    const Vector p3 = {inter[0] - t[2][0], inter[1] - t[2][1], inter[2] - t[2][2]};

    const Vector v1 = CrossProduct(side1, p1);
    const Vector v2 = CrossProduct(side2, p2);
    const Vector v3 = CrossProduct(side3, p3);

    return (DotProduct(v1, v2) >= 0 && DotProduct(v1, v3) >= 0);
}

inline std::optional<Intersection> GetIntersection(const Ray& ray, const Sphere& sphere) {
    const double a = std::pow(ray.GetDirection()[0], 2) + std::pow(ray.GetDirection()[1], 2) +
                     std::pow(ray.GetDirection()[2], 2);
    const double b = 2 * ((ray.GetOrigin()[0] - sphere.GetCenter()[0]) * ray.GetDirection()[0] +
                          (ray.GetOrigin()[1] - sphere.GetCenter()[1]) * ray.GetDirection()[1] +
                          (ray.GetOrigin()[2] - sphere.GetCenter()[2]) * ray.GetDirection()[2]);
    const double c =
        std::pow(ray.GetOrigin()[0] - sphere.GetCenter()[0], 2) +
        std::pow(ray.GetOrigin()[1] - sphere.GetCenter()[1], 2) +
        std::pow(ray.GetOrigin()[2] - sphere.GetCenter()[2], 2) - std::pow(sphere.GetRadius(), 2);

    const double d = b * b - 4 * a * c;
    if (d < 0) {
        return std::nullopt;
    }

    const double t1 = (-b + sqrt(d)) / (2 * a);
    const double t2 = (-b - sqrt(d)) / (2 * a);
    double t = 0;
    if (t1 > 0 && t2 > 0) {
        t = std::min(t1, t2);
    } else if (t1 > 0) {
        t = t1;
    } else if (t2 > 0) {
        t = t2;
    } else {
        return std::nullopt;
    }

    const Vector position = {
      ray.GetOrigin()[0] + t * ray.GetDirection()[0],
      ray.GetOrigin()[1] + t * ray.GetDirection()[1],
      ray.GetOrigin()[2] + t * ray.GetDirection()[2]};
    Vector normal = position - sphere.GetCenter();
    normal.Normalize();
    if (DotProduct(ray.GetDirection(), normal) > 0) {
        normal = {-normal[0], -normal[1], -normal[2]};
    }
    const Vector l = position - ray.GetOrigin();
    const double distance = Length(l);

    return Intersection{position, normal, distance};
}

inline Vector Reflect(const Vector& ray, const Vector& normal) {
    const Vector n = normal;
    return ray - (n * DotProduct(ray, normal)) * 2;
}

inline std::optional<Intersection> GetIntersection(const Ray& ray, const Triangle& triangle) {
    const Vector a1 = {
      triangle[1][0] - triangle[0][0], triangle[1][1] - triangle[0][1],
      triangle[1][2] - triangle[0][2]};
    const Vector a2 = {
      triangle[2][0] - triangle[0][0], triangle[2][1] - triangle[0][1],
      triangle[2][2] - triangle[0][2]};

    const Vector b1 = triangle[0];

    Vector n = CrossProduct(a1, a2);
    n.Normalize();
    if (DotProduct(ray.GetDirection(), n) > 0) {
        n = {-n[0], -n[1], -n[2]};
    }
    if (DotProduct(ray.GetDirection(), n) == 0) {
        return std::nullopt;
    }

    const double t =
        DotProduct(
            Vector{
              b1[0] - ray.GetOrigin()[0], b1[1] - ray.GetOrigin()[1], b1[2] - ray.GetOrigin()[2]},
            n) /
        DotProduct(ray.GetDirection(), n);
    if (t < 0) {
        return std::nullopt;
    }

    const Vector position = {
      ray.GetOrigin()[0] + ray.GetDirection()[0] * t,
      ray.GetOrigin()[1] + ray.GetDirection()[1] * t,
      ray.GetOrigin()[2] + ray.GetDirection()[2] * t};
    if (IsPointInTriangle(triangle, position)) {
        const double distance = std::sqrt(
            std::pow(position[0] - ray.GetOrigin()[0], 2) +
            std::pow(position[1] - ray.GetOrigin()[1], 2) +
            std::pow(position[2] - ray.GetOrigin()[2], 2));
        return Intersection{position, n, distance};
    }
    return std::nullopt;
}

inline std::optional<Vector> Refract(const Vector& ray, const Vector& normal, double eta) {
    Vector d = ray;
    d.Normalize();  // Нормализуем входящий вектор луча

    const double cos_theta_i = -DotProduct(d, normal);
    const double sin_theta_t_squared =
        eta * eta * (1 - cos_theta_i * cos_theta_i);  // Применяем закон Снеллиуса
    if (sin_theta_t_squared > 1) {
        return std::nullopt;
    }

    const double cos_theta_t = std::sqrt(1 - sin_theta_t_squared);

    // Формула для преломленного вектора
    return (d * eta) + (normal * (eta * cos_theta_i - cos_theta_t));
}

inline Vector GetBarycentricCoords(const Triangle& triangle, const Vector& point) {
    const double area = triangle.Area();
    const Triangle t_mbc = {point, triangle[1], triangle[2]};
    const Triangle t_mca = {point, triangle[2], triangle[0]};
    const Triangle t_mab = {point, triangle[0], triangle[1]};

    const double alpha = t_mbc.Area() / area;
    const double beta = t_mca.Area() / area;
    const double gamma = t_mab.Area() / area;

    // Проверка, что сумма координат равна 1 (если точка внутри или на границе)
    if (alpha < 0 || beta < 0 || gamma < 0) {
        return {0, 0, 0};  // Точка находится вне треугольника
    }
    return {alpha, beta, gamma};
}
