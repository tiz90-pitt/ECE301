#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <stdexcept>
#include <cmath>

#include "Node.hpp"
#include "Component.hpp"
#include "VoltageSource.hpp"
#include "CurrentSource.hpp"
#include "Resistor.hpp"

#include "Network.hpp"
#include "VoltageDivider.hpp"
#include "CurrentDivider.hpp"
#include "PracticalVsCircuit.hpp"

const double THRESHOLD = 1e-9;

bool compare_double(double val1, double val2)
{
    double diff = std::abs(val1 - val2);
    return (diff <= THRESHOLD);
}

// VoltageSource class unit tests
TEST_CASE("Network: Test VoltageSource", "[network]")
{
    VoltageSource vs;

    // test inherited behavior
    REQUIRE(vs.get_node_a() == nullptr);
    REQUIRE(vs.get_node_b() == nullptr);

    Node *node1ptr = new Node();
    Node *node2ptr = new Node();
    REQUIRE_NOTHROW(vs.set_node_a(node1ptr));
    REQUIRE_NOTHROW(vs.set_node_b(node2ptr));
    REQUIRE(vs.get_node_a() == node1ptr);
    REQUIRE(vs.get_node_b() == node2ptr);
    REQUIRE(compare_double(vs.get_voltage(), 0));

    // test source voltage, should start as 0 from Component default
    double voltage = 3.3;
    REQUIRE(compare_double(vs.get_voltage(), 0));
    vs.set_value(voltage);
    // source voltage should be independent of node voltages
    REQUIRE(compare_double(vs.get_voltage(), voltage));

    // construct with pointers, test that source voltage is separate from terminal voltages
    double v1 = 5.2, v2 = 1.3;
    node1ptr->set_voltage(v1);
    node2ptr->set_voltage(v2);
    VoltageSource vs2(node1ptr, node2ptr);
    REQUIRE(compare_double(vs2.get_voltage(), 0));
    vs2.set_value(-voltage);
    REQUIRE(compare_double(vs2.get_voltage(), -voltage));

    delete node1ptr;
    delete node2ptr;
}

// CurrentSource class unit tests
TEST_CASE("Network: Test CurrentSource", "[network]")
{
    // test inherited behavior
    CurrentSource cs;

    REQUIRE(cs.get_node_a() == nullptr);
    REQUIRE(cs.get_node_b() == nullptr);
    REQUIRE_THROWS_AS(cs.get_voltage(), std::logic_error);

    Node *node1ptr = new Node();
    Node *node2ptr = new Node();
    REQUIRE_NOTHROW(cs.set_node_a(node1ptr));
    REQUIRE_NOTHROW(cs.set_node_b(node2ptr));
    REQUIRE(cs.get_node_a() == node1ptr);
    REQUIRE(cs.get_node_b() == node2ptr);
    REQUIRE(compare_double(cs.get_voltage(), 0));

    // test source current, should start as 0 from Component default
    double current = 3.3;
    REQUIRE(compare_double(cs.get_current(), 0));
    cs.set_value(current);
    REQUIRE(compare_double(cs.get_current(), current));

    // construct with pointers, test current and voltage
    double v1 = 5.2, v2 = 1.3;
    node1ptr->set_voltage(v1);
    node2ptr->set_voltage(v2);
    CurrentSource cs2(node1ptr, node2ptr);
    REQUIRE(compare_double(cs2.get_current(), 0));
    cs2.set_value(-current);
    REQUIRE(compare_double(cs2.get_voltage(), v2 - v1));
    REQUIRE(compare_double(cs2.get_current(), -current));

    delete node1ptr;
    delete node2ptr;
}

// Resistor class unit tests
TEST_CASE("Network: Test Resistor", "[network]")
{
    // test inheritance
    Resistor r;

    REQUIRE(r.get_node_a() == nullptr);
    REQUIRE(r.get_node_b() == nullptr);
    REQUIRE_THROWS_AS(r.get_voltage(), std::logic_error);

    Node *node1ptr = new Node();
    Node *node2ptr = new Node();
    REQUIRE_NOTHROW(r.set_node_a(node1ptr));
    REQUIRE_NOTHROW(r.set_node_b(node2ptr));
    REQUIRE(r.get_node_a() == node1ptr);
    REQUIRE(r.get_node_b() == node2ptr);
    REQUIRE(compare_double(r.get_voltage(), 0));

    // test resistance, should start as 0 from Component default
    double resistance = 590;
    REQUIRE(compare_double(r.get_value(), 0));
    r.set_value(0); // 0 resistance is allowed
    r.set_value(resistance);
    REQUIRE(compare_double(r.get_value(), resistance));

    // test negative resistance
    REQUIRE_THROWS_AS(r.set_value(-resistance), std::invalid_argument);

    // test current
    double v1 = 5.2, v2 = 1.3;
    node1ptr->set_voltage(v1);
    node2ptr->set_voltage(v2);
    REQUIRE(compare_double(r.get_current(), (v2 - v1) / resistance));

    // test get_current throws
    r.set_node_a(nullptr);
    // node a nullptr
    REQUIRE_THROWS_AS(r.get_current(), std::logic_error);
    r.set_node_b(nullptr);
    // both nodes nullptr
    REQUIRE_THROWS_AS(r.get_current(), std::logic_error);
    r.set_node_a(node1ptr);
    // node b still nullptr
    REQUIRE_THROWS_AS(r.get_current(), std::logic_error);
    r.set_node_b(node2ptr);
    // valid
    REQUIRE(compare_double(r.get_current(), (v2 - v1) / resistance));
    // div by 0
    r.set_value(0);
    REQUIRE_THROWS_AS(r.get_current(), std::logic_error);

    delete node1ptr;
    delete node2ptr;
}

// General unit tests of overriding and dynamic binding
TEST_CASE("Network: Test overriding and dynamic binding", "[network]")
{

    Component c;
    Resistor r;
    Component *c_ptr = new Resistor();

    REQUIRE_NOTHROW(c.set_value(-1));
    REQUIRE_THROWS_AS(r.set_value(-1), std::invalid_argument);
    REQUIRE_THROWS_AS(c_ptr->set_value(-1), std::invalid_argument);

    c.set_value(0);
    r.set_value(0);
    c_ptr->set_value(0);
    REQUIRE_NOTHROW(c.get_current());
    REQUIRE_THROWS_AS(r.get_current(), std::logic_error);
    REQUIRE_THROWS_AS(c_ptr->get_current(), std::logic_error);

    double val = 0.1;
    Component c_base(val);
    VoltageSource vs(val);
    CurrentSource cs(val);
    Component *c_ptr1 = new VoltageSource(val);
    Component *c_ptr2 = new CurrentSource(val);

    REQUIRE(compare_double(c_base.get_current(), 0.0));
    REQUIRE(compare_double(vs.get_current(), 0.0));
    REQUIRE(compare_double(cs.get_current(), val));
    REQUIRE(compare_double(c_ptr1->get_current(), 0.0));
    REQUIRE(compare_double(c_ptr2->get_current(), val));

    REQUIRE_THROWS_AS(c_base.get_voltage(), std::logic_error);
    REQUIRE(compare_double(vs.get_voltage(), val));
    REQUIRE_THROWS_AS(cs.get_voltage(), std::logic_error);
    REQUIRE(compare_double(c_ptr1->get_voltage(), val));
    REQUIRE_THROWS_AS(c_ptr2->get_voltage(), std::logic_error);

    delete c_ptr;
    delete c_ptr1;
    delete c_ptr2;
}

/* Your test cases here */
// Had help to resolve failed test cases from ChatGPT(The chatbox in VSCode?)
TEST_CASE("Test PracticalVsCircuit", "[network]")
{
    double vs = 15.0;
    double rs = 2.5;
    double rl = 2.75;
    // 1) Constructor - Pass
    PracticalVsCircuit pvc(vs, rs, rl);
    // 2) Correct overriding of get_source_voltage()
    double expvolt = vs * (rl / (rs + rl));
    REQUIRE(compare_double(pvc.get_source_voltage(), expvolt)); // Pass
    // 3) Proper Power Calculation
    double vl = pvc.get_resistor_voltage(1);
    double r_l = pvc.get_resistance(1);
    double pl = (vl * vl) / r_l;
    REQUIRE(compare_double(pvc.get_load_power(), pl)); // Pass
    // 4) Maximum Power Transfer
    bool transfer = pvc.maximum_power_transfer(2.5);
    REQUIRE(transfer); // Pass
}
/* Uncomment the following partial test cases when you are ready to start testing the dividers */

TEST_CASE("Network: Test VoltageDivider", "[network]")
{
    double voltage = 5.0;
    const int num = 2;
    double res[num] = {200, 400};
    VoltageDivider div(voltage, res, num); // Pass

    // TODO
    // B. Test get_num_resistors()
    REQUIRE(div.get_num_resistors() == num); // Pass
    // C. Test get_source_voltage() & get_resistor_voltage()
    REQUIRE(compare_double(div.get_source_voltage(), voltage)); // Pass
    // R1
    double gdiv = (voltage * res[0]) / (res[0] + res[1]);
    REQUIRE(compare_double(div.get_resistor_voltage(0), gdiv));
    // R2
    double div2 = (voltage * res[1]) / (res[0] + res[1]);
    REQUIRE(compare_double(div.get_resistor_voltage(1), div2)); // Pass
    // D. get_resistance()
    REQUIRE(compare_double(div.get_resistance(0), res[0]));
    REQUIRE(compare_double(div.get_resistance(1), res[1])); // Pass
    // E. get_loop_current()
    double totr = res[0] + res[1];
    double curr = voltage / totr;
    REQUIRE(compare_double(div.get_loop_current(), curr)); // Pass
    // F. set_source_voltage & set_resistance
    double newvolt = 12.0;
    div.set_source_voltage(newvolt);
    REQUIRE(compare_double(div.get_source_voltage(), newvolt));

    double newr = 250.0;
    div.set_resistance(0, newr);
    REQUIRE(compare_double(div.get_resistance(0), newr)); // Pass
}

TEST_CASE("Network: Test CurrentDivider", "[network]")
{
    double current = 0.006;
    const int num = 2;
    double res[num] = {15000, 75000};
    CurrentDivider div(current, res, num); // Pass

    // TODO
    // B. Test get_num_resistors()
    REQUIRE(div.get_num_resistors() == num); // Pass
    // C. get_source_current
    REQUIRE(compare_double(div.get_source_current(), current)); // Pass
    // D. get_resistance
    REQUIRE(compare_double(div.get_resistance(0), res[0]));
    REQUIRE(compare_double(div.get_resistance(1), res[1])); // Pass
    // E. get_resistor_voltage
    double req = 1.0 / ((1.0 / res[0]) + (1.0 / res[1]));
    double volt = req * current;

    REQUIRE(compare_double(div.get_resistor_voltage(), volt)); // Pass
    // F. get_resistor_current
    double i1 = volt / res[0];
    double i2 = volt / res[1];
    double rc1 = div.get_resistor_current(0);
    double rc2 = div.get_resistor_current(1);

    REQUIRE(compare_double(rc1, i1));
    REQUIRE(compare_double(rc2, i2)); // Pass
    // G. set_source_current & set_resistance
    double newcurr = 0.01;
    div.set_source_current(newcurr);
    REQUIRE(compare_double(div.get_source_current(), newcurr));

    double newr = 1500.0;
    div.set_resistance(1, newr);
    REQUIRE(compare_double(div.get_resistance(1), newr)); // Pass
}