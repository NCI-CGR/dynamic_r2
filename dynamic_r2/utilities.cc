#include "dynamic_r2/utilities.h"
/*
unsigned dynamic_r2::compute_index(double f) {
  if (f > 0.05) return 0;
  if (f > 0.03) return 1;
  if (f > 0.01) return 2;
  if (f > 0.005) return 3;
  if (f > 0.001) return 4;
  return 5;
}

std::string dynamic_r2::compute_binlabel(unsigned index) {
  if (index == 0) return "(0.05, 1]";
  if (index == 1) return "(0.03, 0.05]";
  if (index == 2) return "(0.01, 0.03]";
  if (index == 3) return "(0.005, 0.01]";
  if (index == 4) return "(0.001, 0.005]";
  return "[0, 0.001]";
}
*/
void dynamic_r2::resolve_frequency_bins(const std::vector<double> &breaks,
					const double &hard_min,
					std::vector<std::pair<double, double> > &target) {
  target.clear();
  if (breaks.empty())
    throw std::domain_error("resolve_frequency_bins: no breaks provided");
  target.push_back(std::make_pair(hard_min, breaks.at(0)));
  for (unsigned i = 1; i < breaks.size(); ++i) {
    target.push_back(std::make_pair(breaks.at(i - 1), breaks.at(i)));
  }
  target.push_back(std::make_pair(*breaks.rbegin(), 1.0));
}
