#include "CurrentDivider.hpp"
#include <stdexcept>

CurrentDivider::CurrentDivider(double c, double res[], int num_res) : Network(num_res + 1, num_res + 1), num_res(num_res)
{
  if (num_res < 1) // at least 1 Resistor
  {
    throw std::invalid_argument("Need at least one resistor.");
  }
  Network::set_component(0, new CurrentSource(c));

  for (int i = 0; i < num_res; i++)
  {
    Network::set_component(i + 1, new Resistor(res[i]));
  }
  for (int i = 0; i < get_num_components(); i++)
  {
    Network::connect_component_terminal_a(i, 0); // Ground
    Network::connect_component_terminal_b(i, 1); // Node 1
  }
  // Voltage Divider: Google Search for the formula
  // 1) Find the eq_r: 1/req = 1/r_1+1/r_2+...+ 1/r_n
  double req = 0.0;
  double r;

  for (int i = 0; i < num_res; i++)
  {
    r = get_resistance(i);
    req += 1.0 / r;
  }
  if (req == 0.0)
  {
    throw std::logic_error("Invalid resistance.");
  }
  req = 1.0 / req;
  // 2) Find voltage: V=I*r
  // Starting with comp 0
  double volt = req * (get_component_value(0));
  // Setter voltage @ Ground and Node 1
  Network::set_node_voltage(0, 0.0);
  Network::set_node_voltage(1, volt);
}
int CurrentDivider::get_num_resistors() const
{
  return num_res;
}
double CurrentDivider::get_source_current() const
{
  return get_component_value(0);
}
double CurrentDivider::get_resistance(int index) const
{
  if (index < 0 || index >= num_res)
  {
    throw std::out_of_range("The index is invalid");
  }
  return get_component_value(index + 1);
}
double CurrentDivider::get_resistor_voltage() const
{
  return Network::get_component_voltage(1);
}
double CurrentDivider::get_resistor_current(int index) const
{
  if (index < 0 || index >= num_res)
  {
    throw std::out_of_range("The index is invalid");
  }
  // R starts from comp 1
  return Network::get_component_current(index + 1);
}
void CurrentDivider::set_source_current(double c)
{
  Network::set_component_value(0, c);
  // Updating the nodes
  double req = 0.0;
  double r;
  for (int i = 0; i < num_res; i++)
  {
    r = get_resistance(i);
    req += 1.0 / r;
  }
  if (req == 0.0)
  {
    throw std::logic_error("Invalid resistance.");
  }
  req = 1.0 / req;
  double volt = req * (get_component_value(0));
  Network::set_node_voltage(0, 0.0);
  Network::set_node_voltage(1, volt);
}
void CurrentDivider::set_resistance(int index, double r)
{
  if (index == 0 || index >= num_res)
  {
    throw std::out_of_range("The index is invalid");
  }
  Network::set_component_value(index + 1, r);
}