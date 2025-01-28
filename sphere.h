#pragma once

#include "98_raytracer/raytracer-geom/vector.h"
#include "vector.h"

class Sphere {
   public:
    Sphere(const Vector& center, double radius)
        : center_(center), radius_(radius) {

        };

    [[nodiscard]] const Vector& GetCenter() const {
        return center_;
    }

    [[nodiscard]] double GetRadius() const {
        return radius_;
    }

   private:
    // data
    Vector center_;
    double radius_;
};
