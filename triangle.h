#pragma once

#include "vector.h"

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>

class Triangle {
   public:
    Triangle(const Vector& a, const Vector& b, const Vector& c)
        : triangle_{a, b, c} {

        };

    const Vector& operator[](size_t ind) const {
        assert(ind < 3);
        return triangle_.at(ind);
    }

    [[nodiscard]] double Area() const {
        const Vector side1 = {
          triangle_[0][0] - triangle_[1][0], triangle_[0][1] - triangle_[1][1],
          triangle_[0][2] - triangle_[1][2]};
        const Vector side2 = {
          triangle_[2][0] - triangle_[1][0], triangle_[2][1] - triangle_[1][1],
          triangle_[2][2] - triangle_[1][2]};
        Vector result = CrossProduct(side1, side2);
        return std::sqrt(result[0] * result[0] + result[1] * result[1] + result[2] * result[2]) / 2;
    }

   private:
    // data
    std::array<Vector, 3> triangle_;
};
