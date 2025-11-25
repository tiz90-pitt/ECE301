#include "Resistor.hpp"
#include <stdexcept> // to use invvalid_arg and logic_err

void Resistor::set_value(double r)
{
    // TODO
    // Setter function
    // r = resistance (ohms)
    if (r < 0)
    {
        throw std::invalid_argument("r is not positive");
    }
    else
    {
        Component::set_value(r);
    }
}

double Resistor::get_current() const
{
    // TODO
    // Getter function - R = V / I, then I = V / R
    double r = Component::get_value();
    double v = Component::get_voltage();

    if (r == 0)
    {
        throw std::logic_error("Invalid division by zero");
    }
    double I = v / r;

    return I;
}
