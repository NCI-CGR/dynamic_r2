#include "dynamic_r2/bin_calculator.h"

void dynamic_r2::bin_calculator::remove_value() {
  if (_current_inclusion_size) {
    --_current_inclusion_size;
    _running_total -= _data.at(_current_inclusion_size);
  }
}
double dynamic_r2::bin_calculator::current_mean() const {
  return _running_total / _current_inclusion_size;
}

void dynamic_r2::bin_calculator::filter_to_target(const double &target,
						  const double &minimum) {
  if (_data.size() > 1) {
    std::cout << "filtering from " << _data.size() << " variants. maximum r2 is " << _data.at(0) << "; minimum is " << _data.at(_data.size() - 1) << std::endl;
  } else {
    std::cout << "attempting filter, but data only contains " << _data.size() << " entries womp womp" << std::endl;
  }
  while (_current_inclusion_size && (current_mean() < target || current_smallest() < minimum)) {
    // quickly check if sort order is ok
    if (_current_inclusion_size > 1) {
      if (_data.at(_current_inclusion_size - 1) > _data.at(_current_inclusion_size - 2))
	throw std::domain_error("sorting apparently failed");
    }
    remove_value();
  }
  //if (!_current_inclusion_size) {
  //throw std::domain_error("target too strict!  everything was removed!");
  //}
}

const double &dynamic_r2::bin_calculator::current_smallest() const {
  if (!_current_inclusion_size)
    throw std::domain_error("bin_calculator::current_smallest: called with no valid entries");
  return _data.at(_current_inclusion_size - 1);
}

void dynamic_r2::bin_calculator::add_value(const double &d) {
  _data.push_back(d);
  _running_total += d;
  ++_current_inclusion_size;
}

void dynamic_r2::bin_calculator::sort() {
  std::sort(_data.begin(), _data.end(), reverse_sort);
}

double dynamic_r2::bin_calculator::proportion_maintained() const {
  return static_cast<double>(_current_inclusion_size) / static_cast<double>(_data.size());
}
unsigned dynamic_r2::bin_calculator::maintained_count() const {
  return _current_inclusion_size;
}

double dynamic_r2::bin_calculator::max_included_value() const {
  if (_current_inclusion_size)
    return _data.at(_current_inclusion_size - 1);
  return 69.0;
}

unsigned dynamic_r2::bin_calculator::total_count() const {
  return _data.size();
}
