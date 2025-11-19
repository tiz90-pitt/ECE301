#include "Frequency.hpp"

#include <stdexcept>
#include <cmath>

const double THRESHOLD = 1e-9; // For the operators

// TODO: default constructor and constructor with args
Frequency::Frequency() : Frequency(301, 0.1)
{
    // The default constructor that sets num_components to 301, and increment to 0.1.
}
Frequency::Frequency(int num_comp, double incr)
{ // Constructor with args
    // Checking
    if (num_comp <= 0 || num_comp > MAX_COMPONENTS_NUM)
    {
        throw std::invalid_argument("Invalid number.");
    }
    if (incr <= 0)
    {
        throw std::invalid_argument("Invalid increment.");
    }
    num_components = num_comp;
    increment = incr;
    zeros();
}
// TODO: accessors
int Frequency::get_component() const
{
    return num_components;
}
double Frequency::get_increment() const
{
    return increment;
}
// provided: assign zero amplitudes to frequencies
void Frequency::zeros()
{
    for (int i = 0; i < num_components; i++)
    {
        freq_data[i].frequencies = static_cast<double>(i) * increment;
        freq_data[i].amplitudes = 0.0;
    }
}
// TODO: assign constant amplitudes to all frequencies
void Frequency::constant(double val)
{
    // Checking
    if (val < 0)
    {
        throw std::invalid_argument("Invalid amplitude.");
    }
    for (int i = 0; i < num_components; i++)
    {
        freq_data[i].frequencies = static_cast<double>(i) * increment;
        freq_data[i].amplitudes = val;
    }
}
// TODO: gaussian build-up
void Frequency::gaussian(double mag, double width)
{
    // Checking
    if (mag <= 0 || width <= 0)
    {
        throw std::invalid_argument("Invalid magnitude or width.");
    }
    // The equation for gaussian
    for (int i = 0; i < num_components; i++)
    {
        double alpha = freq_data[i].frequencies;
        double expcalc = alpha / width;
        freq_data[i].amplitudes = mag * std::exp(-0.5 * expcalc * expcalc);
    }
}
bool Frequency::operator==(const Frequency &fcomp) const
{
    // TODO: overloaded operators
    // Comparsion between num_comp, incr, and amp
    if (num_components != fcomp.num_components || std::abs(increment - fcomp.increment) > THRESHOLD)
    {
        return false;
    }
    for (int i = 0; i < num_components; i++)
    {
        if (std::abs(freq_data[i].amplitudes - fcomp.freq_data[i].amplitudes) > THRESHOLD)
        {
            return false;
        }
    }
    return true; // All loops are checked and passed
}
bool Frequency::operator<=(const Frequency &fcomp) const
{
    // Similar to ==
    if (num_components != fcomp.num_components || std::abs(increment - fcomp.increment) > THRESHOLD)
    {
        throw std::logic_error("Comparison failed.");
    }
    // Check the amps are <= to Freq comp
    for (int i = 0; i < num_components; i++)
    {
        if (std::abs(freq_data[i].amplitudes - fcomp.freq_data[i].amplitudes) > THRESHOLD)
        {
            return false;
        }
    }
    return true; // All loops are checked and passed
}
bool Frequency::operator>>(const Frequency &fcomp) const
{
    // Need to calc the equation first
    // Avg Power for the current obj
    double sos = 0.0; // Sum of squares
    for (int i = 0; i < num_components; i++)
    {
        double amp = freq_data[i].amplitudes;
        sos += amp * amp;
    }
    double avgpow = 0.0;
    if (num_components > 0)
    {
        avgpow = sos / num_components;
    }
    // Avg Power for the fcomp obj
    double fcompsos = 0.0; // Sum of squares for fcomp
    for (int i = 0; i < num_components; i++)
    {
        double amp = fcomp.freq_data[i].amplitudes;
        fcompsos += amp * amp;
    }
    double fcompavgpow = 0.0;
    if (fcomp.num_components > 0)
    {
        fcompavgpow = fcompsos / fcomp.num_components;
    }
    // Comparsion
    return avgpow - (100.0 * fcompavgpow) >= THRESHOLD;
}
Frequency Frequency::operator^(const Frequency &fcomp) const
{
    // Checking for increment
    if (std::abs(increment - fcomp.increment) > THRESHOLD)
    {
        throw std::logic_error("Multiplication on two spectrums failed.");
    }
    // Resulting spectrum length
    int spect = std::max(num_components, fcomp.num_components);
    Frequency result(spect, increment);
    // Shorter spectrum length
    int shortspect = std::min(num_components, fcomp.num_components);
    for (int i = 0; i < shortspect; i++)
    {
        result.freq_data[i].amplitudes = freq_data[i].amplitudes * fcomp.freq_data[i].amplitudes;
    }
    // 0 amplitudes are already set
    return result;
}
// provided: exports
void Frequency::export_frequency(double freq[], double amps[]) const
{
    for (int i = 0; i < num_components; i++)
    {
        freq[i] = freq_data[i].frequencies;
        amps[i] = freq_data[i].amplitudes;
    }
}

bool Frequency::export_csv(const std::string &outfile) const
{
    std::ofstream filewrite(outfile + ".csv");
    if (!filewrite.fail())
    {
        filewrite << "w, |F(w)|" << std::endl;
        for (int i = 0; i < num_components; i++)
        {
            filewrite << freq_data[i].frequencies << ", " << freq_data[i].amplitudes << std::endl;
        }
        // Close file
        filewrite.close();
        return true;
    }
    return false;
}