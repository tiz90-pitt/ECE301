#include "PracticalVsCircuit.hpp"
#include <cmath> // For the abs

// Constructor needs to call VoltageDivider, which also needs 2 res (rs & rl)
PracticalVsCircuit::PracticalVsCircuit(double vs, double rs, double rl) : VoltageDivider(vs, new double[2]{rs, rl}, 2)
{
}
double PracticalVsCircuit::get_source_voltage() const
{
  return VoltageDivider::get_node_voltage(2);
}
void PracticalVsCircuit::set_load(double rl)
{
  // Changing the load of rl @ index 1
  set_resistance(1, rl);
}
double PracticalVsCircuit::get_load_power() const
{
  // Pl = vl^2 / rl
  double vl = get_resistor_voltage(1);
  double rl = get_resistance(1);
  double pl = (vl * vl) / rl;
  return pl;
}
bool PracticalVsCircuit::maximum_power_transfer(double rl)
{
  // can r give the max pow travel from source and = to theoretical max pow by
  // Pmax = vs^2 / (4 * rs)? True = if the given rl works, otherwise false
  // Avoid using rl == rs, so we use Threhold & abs
  const double THRESHOLD = 1e-9;

  double vs = get_component_voltage(0);   // @ comp 0
  double rs = get_resistance(0);          // index 0
  double thpmax = (vs * vs) / (4.0 * rs); // Theoretical max power
  // Pmax with given rl
  double totalres = rs + rl;
  double vc = vs / totalres;
  double pmax = (vc * vc) * rl;
  return (std::abs(thpmax - pmax) <= THRESHOLD); // If its in the threshold, it should return true
}