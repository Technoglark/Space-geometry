#pragma once

#include "98_raytracer/raytracer-geom/vector.h"
#include "vector.h"

class Intersection {
   public:
    Intersection() = default;

    Intersection(const Vector& position, const Vector& normal, double distance)
        : position_(position), normal_(normal), distance_(distance) {
        normal_.Normalize();
    }

    [[nodiscard]] const Vector& GetPosition() const {
        return position_;
    }

    [[nodiscard]] const Vector& GetNormal() const {
        return normal_;
    }

    [[nodiscard]] double GetDistance() const {
        return distance_;
    }

   private:
    // data
    Vector position_;
    Vector normal_;
    double distance_{};
};
