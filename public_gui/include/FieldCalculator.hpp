// === File: include/FieldCalculator.hpp ===
#pragma once
#include <vector>
#include "Charge.hpp"
#include "Vector2D.hpp"

class FieldCalculator {
public:
    static Vector2D calculateFieldAt(const Vector2D& point, const std::vector<Charge>& charges);
    static float calculateVoltageAt(const Vector2D& point, const std::vector<Charge>& charges);
};