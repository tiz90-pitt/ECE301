#include "CurrentSource.hpp"

double CurrentSource::get_current() const
{
    // TODO
    // Similar to VoltageSource, but CurrentSource only provides constant source current value
    return Component::get_value();
}