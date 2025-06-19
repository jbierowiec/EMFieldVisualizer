#include "../include/FieldCalculator.hpp"
#include <cmath>

const float k = 8.99e5f;

Vector2D FieldCalculator::calculateFieldAt(const Vector2D& point, const std::vector<Charge>& charges) {
    Vector2D totalField(0, 0);

    for (const auto& c : charges) {
        Vector2D r = point - c.pos;
        float dist = r.magnitude();

        if (dist < 1.0f) continue;

        float e = k * c.magnitude / (dist * dist);
        Vector2D eVec = r.normalized() * e;

        if (c.type == POSITIVE)
            totalField += eVec;
        else
            totalField += Vector2D(-eVec.x, -eVec.y);
    }

    return totalField;
}

float FieldCalculator::calculateVoltageAt(const Vector2D& point, const std::vector<Charge>& charges) {
    float totalVoltage = 0.0f;

    for (const auto& c : charges) {
        Vector2D r = point - c.pos;
        float dist = r.magnitude();

        if (dist < 1.0f) continue;

        float v = k * c.magnitude / dist;
        totalVoltage += (c.type == POSITIVE ? v : -v);
    }

    return totalVoltage;
}
