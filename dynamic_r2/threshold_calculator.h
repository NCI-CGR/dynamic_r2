#ifndef __DYNAMIC_R2_THRESHOLD_CALCULATOR_H__
#define __DYNAMIC_R2_THRESHOLD_CALCULATOR_H__

#include <vector>
#include <utility>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cfloat>

#include "dynamic_r2/utilities.h"
#include "dynamic_r2/bin_calculator.h"
#include "finter/finter.h"

namespace dynamic_r2 {
  class threshold_calculator {
  public:
    threshold_calculator(const std::vector<std::pair<double, double> > &frequency_bins)
      : _frequency_bins(frequency_bins) {
      _calculators.resize(frequency_bins.size());
    }
    ~threshold_calculator() throw() {}

    void add_file(const std::string &filename);
    void sort();
    void filter_to_target(const double &target_r2,
			  const double &minimum_r2);
    void report_data(std::ostream &out) const;
    bool get_bin_index(const double &f,
		       unsigned &target) const;
    void report_variant_data(const std::string &filename,
			     finter::finter_writer *output_inclusion,
			     finter::finter_writer *output_exclusion) const;
  private:
    threshold_calculator() {}
    std::vector<std::pair<double, double> > _frequency_bins;
    std::vector<bin_calculator> _calculators;
  };
}

#endif //__DYNAMIC_R2_THRESHOLD_CALCULATOR_H__
