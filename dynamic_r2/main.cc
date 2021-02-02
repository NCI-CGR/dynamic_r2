#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <utility>
#include <stdexcept>
#include <algorithm>

#include "dynamic_r2/cargs.h"
#include "dynamic_r2/bin_calculator.h"
#include "dynamic_r2/threshold_calculator.h"
#include "dynamic_r2/utilities.h"

#include "finter/finter.h"

int main(int argc, char **argv) {
  finter::finter_writer *output_inclusion = 0, *output_exclusion = 0;
  try {
    dynamic_r2::cargs ap(argc, argv);
    if (ap.help() || argc == 1) {
      ap.print_help(std::cout);
      return 0;
    }

    std::vector<std::string> input_files = ap.get_input_files();;
    std::vector<double> frequency_breakpoints = ap.get_frequency_bounds();
    std::string output_exclusion_filename = ap.get_excluded_variant_output_filename(), output_inclusion_filename = ap.get_included_variant_output_filename();
    double hard_frequency_limit = ap.get_hard_lower_limit(), r2_target = ap.get_r2_target(), minimum_r2 = ap.get_minimum_r2();
    bool yun_li_override = ap.get_yun_li_mode();

    if (yun_li_override) {
      hard_frequency_limit = 0.0;
      frequency_breakpoints.clear();
      frequency_breakpoints.push_back(0.0005);
      frequency_breakpoints.push_back(0.002);
      frequency_breakpoints.push_back(0.005);
      frequency_breakpoints.push_back(0.01);
      frequency_breakpoints.push_back(0.03);
      frequency_breakpoints.push_back(0.05);
      r2_target = 0.9;
    }
    
    std::vector<std::pair<double, double> > frequency_bins;
    dynamic_r2::resolve_frequency_bins(frequency_breakpoints, hard_frequency_limit, frequency_bins);
    
    dynamic_r2::threshold_calculator thresh(frequency_bins);
    for (std::vector<std::string>::const_iterator input_file = input_files.begin(); input_file != input_files.end(); ++input_file) {
      thresh.add_file(*input_file);
    }
    thresh.sort();
    thresh.filter_to_target(r2_target, minimum_r2);
    thresh.report_data(std::cout);
    if (!output_exclusion_filename.empty() ||
	!output_inclusion_filename.empty()) {
      if (!output_inclusion_filename.empty()) {
	output_inclusion = finter::reconcile_writer(output_inclusion_filename);
      }
      if (!output_exclusion_filename.empty()) {
	output_exclusion = finter::reconcile_writer(output_exclusion_filename);
      }
      for (std::vector<std::string>::const_iterator input_file = input_files.begin(); input_file != input_files.end(); ++input_file) {
	thresh.report_variant_data(*input_file, output_inclusion, output_exclusion);
      }
    }
    if (output_inclusion) {
      output_inclusion->close();
      delete output_inclusion;
      output_inclusion = 0;
    }
    if (output_exclusion) {
      output_exclusion->close();
      delete output_exclusion;
      output_exclusion = 0;
    }
    return 0;
  } catch (const std::domain_error &e) {
    if (output_inclusion) {
      delete output_inclusion;
    }
    if (output_exclusion) {
      delete output_exclusion;
    }
    std::cerr << "error: " << e.what() << std::endl;
    return 1;
  } catch (...) {
    if (output_inclusion) {
      delete output_inclusion;
    }
    if (output_exclusion) {
      delete output_exclusion;
    }
    throw;
  }
}
