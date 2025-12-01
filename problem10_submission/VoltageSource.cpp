#include "VoltageSource.hpp"

double VoltageSource::get_voltage() const
{
    // TODO
    // Same functionality as Component (two terminals, can connect to Node, etc.), but the voltage should always be constant, according to the source voltage
    // So, we need source voltage from Component and override it
    // This is a getter function, so we can just call the value from Component
    return get_value();
}
