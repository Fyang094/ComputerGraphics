#include "common.h"

// Evaluate curve c at t and return the result.  If sub_a or sub_b are not null,
// perform subdivision on curve c at t as well.  If sub_a is not null, assign it
// the first piece (so that sub_a and c agree at their endpoint A.)  If sub_b is
// not null, assign it the second piece (so that sub_b and c agree at their
// endpoint D.)
vec2 Evaluate(const Curve& c, double t, Curve* sub_a, Curve* sub_b)
{
    const double u = 1.0 - t;

    // following: de Casteljau's algorithm
    const vec2 Q1 = c.A * u + c.B * t;
    const vec2 Q2 = c.B * u + c.C * t;
    const vec2 Q3 = c.C * u + c.D * t;

    if (sub_a != nullptr)
    {
        sub_a->A = c.A;
        sub_a->B = Q1;
        sub_a->C = Q1 * u + Q2 * t;
        sub_a->D = (Q1 * u + Q2 * t) * u + (Q2 * u + Q3 * t) * t;
    }

    if (sub_b != nullptr)
    {
        sub_b->A = (Q1 * u + Q2 * t) * u + (Q2 * u + Q3 * t) * t;
        sub_b->B = Q2 * u + Q3 * t;
        sub_b->C = Q3;
        sub_b->D = c.D;
    }

    return (Q1 * u + Q2 * t) * u + (Q2 * u + Q3 * t) * t;
}

