#pragma once

#include "98_raytracer/raytracer-geom/vector.h"
#include "vector.h"

class Ray {
   public:
    Ray(const Vector& origin, const Vector& direction) : origin_(origin), direction_(direction) {
        direction_.Normalize();
    }

    [[nodiscard]] const Vector& GetOrigin() const {
        return origin_;
    }

    [[nodiscard]] const Vector& GetDirection() const {
        return direction_;
    }

   private:
    // data
    Vector origin_;
    Vector direction_;
};
