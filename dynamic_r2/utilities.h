#ifndef __DYNAMIC_R2_UTILITIES_H__
#define __DYNAMIC_R2_UTILITIES_H__

#include <string>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dynamic_r2 {
  template <class value_type>
    value_type from_string(const std::string &str) {
    std::istringstream strm1(str);
    value_type res;
    if (!(strm1 >> res))
      throw std::domain_error("from_string: cannot convert string to object: \"" + str + "\"");
    return res;
  }

  inline bool reverse_sort(const double &d1,
		    const double &d2) {
    return d2 < d1;
  }
  
  //unsigned compute_index(double f);
  
  //std::string compute_binlabel(unsigned index);

  void resolve_frequency_bins(const std::vector<double> &breaks,
			      const double &hard_min,
			      std::vector<std::pair<double, double> > &target);
}

#endif //__DYNAMIC_R2_UTILITIES_H__
