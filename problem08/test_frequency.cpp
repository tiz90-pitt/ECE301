#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <stdexcept>
#include <cmath>

#include "Frequency.hpp"

const double THRESHOLD = 1e-9;

/* Provided test cases */

TEST_CASE("Export example", "[frequency]")
{
    Frequency fdef;
    fdef.gaussian(12.0, 9 * M_PI);
    fdef.export_csv("output");
}

TEST_CASE("Test default construction", "[frequency]")
{
    Frequency fdef;

    REQUIRE(fdef.get_component() == 301);
    REQUIRE(std::abs(fdef.get_increment() - 0.1) <= THRESHOLD);
}

TEST_CASE("Test construction with arguments", "[frequency]")
{
    int comp = 201;
    double incr = 0.02;
    Frequency farg(comp, incr);

    REQUIRE(farg.get_component() == comp);
    REQUIRE(std::abs(farg.get_increment() - incr) <= THRESHOLD);

    // must call the frequency constructor directly to check throws in constructor
    // invalid number of components
    REQUIRE_THROWS_AS(Frequency(-1, incr), std::invalid_argument);
    REQUIRE_THROWS_AS(Frequency(Frequency::MAX_COMPONENTS_NUM + 1, incr), std::invalid_argument);
    // invalid incr
    REQUIRE_THROWS_AS(Frequency(comp, -1), std::invalid_argument);
    REQUIRE_THROWS_AS(Frequency(comp, 0), std::invalid_argument);
}

TEST_CASE("Test default construction zeros", "[frequency]")
{
    Frequency zero;

    const int comp = zero.get_component();
    double incr = zero.get_increment();

    // arrays to hold exported data
    double freqs[comp];
    double amps[comp];

    zero.export_frequency(freqs, amps);

    for (int i = 0; i < comp; i++)
    {
        // check freq component according to equation
        double freq_diff = std::abs(freqs[i] - static_cast<double>(i) * incr);
        REQUIRE(freq_diff <= THRESHOLD);
        // all amps should be zero
        double amp_diff = std::abs(amps[i] - 0);
        REQUIRE(amp_diff <= THRESHOLD);
    }
}
/* Add your test cases below */
TEST_CASE("Test constant", "[frequency]")
{
    Frequency fconst(15, .1);
    const double val = 1.2;
    // Checking if val is valid
    fconst.constant(val);
    double freqs[15];
    double amps[15];
    fconst.export_frequency(freqs, amps);

    for (int i = 0; i < 15; i++)
    {
        // Testing the amp
        REQUIRE(std::abs(amps[i] - val) <= THRESHOLD);
        // Testing if the calc is correct
        REQUIRE(std::abs(freqs[i] - static_cast<double>(i) * 0.1) <= THRESHOLD);
    }
    // Test Invalid
    REQUIRE_THROWS_AS(fconst.constant(-6.9), std::invalid_argument);
}
TEST_CASE("Test gaussian", "[frequency]")
{
    Frequency fgau(10, 0.3);
    const double mag = 5.0;
    const double width = 0.6;
    // Invalid check with magnitude being at the top and width at the line below it
    REQUIRE_THROWS_AS(fgau.gaussian(-1.7, width), std::invalid_argument);
    REQUIRE_THROWS_AS(fgau.gaussian(mag, -1), std::invalid_argument);
    // Testing if it works
    fgau.gaussian(mag, width);
    double freqs[10];
    double amps[10];
    fgau.export_frequency(freqs, amps);
    // First comp and freq should be 0
    REQUIRE(std::abs(amps[0] - mag) <= THRESHOLD);
    // Testing the other comps
    for (int i = 1; i < 10; i++)
    {
        double alpha = freqs[i];
        double expcalc = alpha / width;
        double expected_amp = mag * std::exp(-0.5 * expcalc * expcalc);
        REQUIRE(std::abs(amps[i] - expected_amp) <= THRESHOLD);
    }
    // Checking the calculations for freqs
    for (int i = 0; i < 10; i++)
    {
        REQUIRE(std::abs(freqs[i] - (i * 0.3)) <= THRESHOLD);
    }
}
TEST_CASE("Test operator==", "[frequency]")
{
    const double amp = 5;
    Frequency f1(5, 0.1); // f1 and f2 should be all 0
    Frequency f2(5, 0.1);
    f1.constant(amp);
    f2.constant(amp);
    Frequency f3(5, 0.3);  // Different increment
    Frequency f4(10, 0.1); // Different num_components
    // Test True
    REQUIRE(f1 == f2);
    // Test False
    REQUIRE_FALSE(f1 == f3); // Different increment
    REQUIRE_FALSE(f1 == f4); // Different num_components
    f2.constant(amp * THRESHOLD * 2);
    REQUIRE_FALSE(f1 == f2); // Different amplitudes
}
TEST_CASE("Test operator<=", "[frequency]")
{
    Frequency f1(4, 0.2);
    Frequency f2(4, 0.2);
    f2.constant(3.0);
    // Checking for Logic Error
    Frequency f3(1, 0.2); // Different num_components
    REQUIRE_THROWS_AS(f1 <= f3, std::logic_error);
    // Test True
    REQUIRE(f1 <= f2);
    // Test False
    f1.constant(1.0);
    REQUIRE_FALSE(f1 <= f2);
}