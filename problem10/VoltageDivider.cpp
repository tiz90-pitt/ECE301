#include "VoltageDivider.hpp"
#include <stdexcept> // to use invalid_arg

// A constructor that takes a double voltage source value, a double res[], and an int num_res
// Create a network with the specified # of components and sets the source and resistor values
VoltageDivider::VoltageDivider(double v, double res[], int num_res) : Network(num_res + 1, num_res + 1), num_res(num_res)
{
  if (num_res < 1) // at least 1 Resistor
  {
    throw std::invalid_argument("Need at least one resistor.");
  }
  // Source always Connector 0, between ground and Node 1
  Network::set_component(0, new VoltageSource(v)); // Need 1 VoltageSource
  // Connecting Ground
  Network::connect_component_terminal_a(0, 0);
  // Connect Node 1 - Similar to Ground, but node is 1
  Network::connect_component_terminal_b(0, 1);
  // for loop to account all r and connecting them, which is similar to lines 10-14
  for (int i = 0; i < num_res; i++)
  {
    int compindex = i + 1;
    // Terminal B
    int nodeb = i + 1;
    // Terminal A
    int nodea;
    if (i == num_res - 1)
    {
      nodea = 0; // last r goes to ground
    }
    else
    {
      nodea = i + 2; // else to the next node
    }
    // Setting and connecting comp
    Network::set_component(compindex, new Resistor(res[i])); // But with Resistors
    Network::connect_component_terminal_a(compindex, nodea);
    Network::connect_component_terminal_b(compindex, nodeb);
  }
  // Division
  // Finding the total res
  double totalres = 0.0;
  for (int i = 0; i < num_res; i++)
  {
    // r starts @ comp 1
    totalres += get_component_value(i + 1);
  }
  // Finding I
  double vs = get_component_value(0); // Voltage source
  double curr = vs / totalres;
  // Setters
  set_node_voltage(0, 0.0); // Ground
  double vcurr = vs;
  set_node_voltage(1, vcurr);       // Node 1
  for (int i = 1; i < num_res; i++) // Setting the rest of the nodes
  {
    double r = get_component_value(i); // r_i @ comp i
    // V = IR
    double oldv = curr * r;

    if (i < num_res)
    {
      vcurr -= oldv;
      set_node_voltage(i + 1, vcurr);
    }
    else
    {
      break;
    }
  }
}
// Getters
int VoltageDivider::get_num_resistors() const
{
  return num_res;
}
double VoltageDivider::get_source_voltage() const
{
  // vs is always component 0 or Ground
  return get_component_value(0);
}
double VoltageDivider::get_resistor_voltage(int index) const
{
  if (index < 0 || index >= num_res)
  {
    throw std::out_of_range("The index is invalid");
  }
  // r starts from comp 1
  return get_component_voltage(index + 1);
}
double VoltageDivider::get_resistance(int index) const
{
  // Similar to get_resistor_voltage
  if (index < 0 || index >= num_res)
  {
    throw std::out_of_range("The index is invalid");
  }
  return get_component_value(index + 1);
}
double VoltageDivider::get_loop_current() const
{
  double vs = get_component_value(0);
  double totalres = 0.0;

  for (int i = 0; i < num_res; i++)
  {
    totalres += get_component_value(i + 1);
  }
  if (totalres == 0)
  {
    throw std::invalid_argument("Invalid total resistance.");
  }
  return vs / totalres;
}
// Setters
void VoltageDivider::set_source_voltage(double v)
{
  set_component_value(0, v);
}
void VoltageDivider::set_resistance(int index, double r)
{
  if (index < 0 || index >= num_res)
  {
    throw std::out_of_range("The index is invalid");
  }
  // r starts from comp 1
  set_component_value(index + 1, r);
}