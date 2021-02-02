#ifndef __DYNAMIC_R2_BIN_CALCULATOR_H__
#define __DYNAMIC_R2_BIN_CALCULATOR_H__

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#include "dynamic_r2/utilities.h"

namespace dynamic_r2 {
  class bin_calculator {
  public:
    bin_calculator()
      : _running_total(0.0),
	_current_inclusion_size(0) {}
    bin_calculator(const bin_calculator &obj)
      : _data(obj._data),
	_running_total(obj._running_total),
	_current_inclusion_size(obj._current_inclusion_size) {}
    ~bin_calculator() throw() {}

    void add_value(const double &d);

    void sort();

    double proportion_maintained() const;

    void remove_value();

    double current_mean() const;

    const double &current_smallest() const;
  
    void filter_to_target(const double &target,
			  const double &minimum);

    unsigned maintained_count() const;

    double max_included_value() const;

    unsigned total_count() const;
  
  private:
    std::vector<double> _data;
    double _running_total;
    unsigned _current_inclusion_size;
  };
}

#endif //__DYNAMIC_R2_BIN_CALCULATOR_H__
