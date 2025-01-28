#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>

class Vector {
   public:
    Vector() = default;

    Vector(double x, double y, double z) {
        v_ = {x, y, z};
    }

    double& operator[](size_t ind) {
        return v_.at(ind);
    }

    double operator[](const size_t ind) const {
        return v_.at(ind);
    }

    Vector operator+(const Vector& b) const {
        return {v_[0] + b[0], v_[1] + b[1], v_[2] + b[2]};
    }

    Vector operator-(const Vector& b) const {
        return {v_[0] - b[0], v_[1] - b[1], v_[2] - b[2]};
    }

    Vector operator*(const double& a) const {
        return {v_[0] * a, v_[1] * a, v_[2] * a};
    }

    void Normalize() {
        const double len = sqrt(v_[0] * v_[0] + v_[1] * v_[1] + v_[2] * v_[2]);
        v_[0] /= len;
        v_[1] /= len;
        v_[2] /= len;
    }

   private:
    // data
    std::array<double, 3> v_{};
};

inline double DotProduct(const Vector& a, const Vector& b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline Vector CrossProduct(const Vector& a, const Vector& b) {
    return {a[1] * b[2] - b[1] * a[2], -(a[0] * b[2] - b[0] * a[2]), a[0] * b[1] - b[0] * a[1]};
}

inline double Length(const Vector& v) {
    return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}
