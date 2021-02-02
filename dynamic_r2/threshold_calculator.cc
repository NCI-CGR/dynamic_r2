#include "dynamic_r2/threshold_calculator.h"

bool dynamic_r2::threshold_calculator::get_bin_index(const double &f,
						     unsigned &target) const {
  for (unsigned i = 0; i < _frequency_bins.size(); ++i) {
    if (!i && _frequency_bins.at(i).first < DBL_EPSILON) {
      if (f >= _frequency_bins.at(i).first && f <= _frequency_bins.at(i).second) {
	target = i;
	return true;
      }
    } else {
      if (f > _frequency_bins.at(i).first && f <= _frequency_bins.at(i).second) {
	target = i;
	return true;
      }
    }
  }
  return false;
}

void dynamic_r2::threshold_calculator::report_data(std::ostream &out) const {
  for (unsigned i = 0; i < _frequency_bins.size(); ++i) {
    std::ostringstream bin_label;
    if (_frequency_bins.at(0).first < DBL_EPSILON) {
      bin_label << "[";
    } else {
      bin_label << "(";
    }
    bin_label << _frequency_bins.at(i).first << "," << _frequency_bins.at(i).second << "]";
    
    if (!(out << "for bin " << bin_label.str() << ", started with " << _calculators.at(i).total_count() << " variants"
	  << " and retained " << _calculators.at(i).maintained_count() << " (" << _calculators.at(i).proportion_maintained() << ") with an rsq filter of "
	  << _calculators.at(i).max_included_value() << " and a mean remaining rsq of " << _calculators.at(i).current_mean() << std::endl)) {
    }
  }
}

void dynamic_r2::threshold_calculator::filter_to_target(const double &target_r2,
							const double &minimum_r2) {
  for (std::vector<bin_calculator>::iterator iter = _calculators.begin();
       iter != _calculators.end();
       ++iter) {
    iter->filter_to_target(target_r2, minimum_r2);
  }
}

void dynamic_r2::threshold_calculator::sort() {
  for (std::vector<bin_calculator>::iterator iter = _calculators.begin();
       iter != _calculators.end();
       ++iter) {
    iter->sort();
  }
}

void dynamic_r2::threshold_calculator::add_file(const std::string &filename) {
  std::string line = "", variant_id = "", reference_allele = "", alternate_allele = "";
  double alternate_allele_frequency = 0.0, maf = 0.0;
  std::string avgcall = "", rsq = "";
  unsigned bin_index = 0;
  finter::finter_reader *input = 0;
  try {
    input = finter::reconcile_reader(filename);
    input->getline(&line);
    while (input->getline(&line)) {
      std::istringstream strm1(line);
      if (!(strm1 >> variant_id >> reference_allele >> alternate_allele
	    >> alternate_allele_frequency >> maf >> avgcall >> rsq))
	throw std::domain_error("threshold_calculator::add_file: cannot parse info file \"" + filename + "\" line \"" + line + "\"");
      if (rsq.compare("-") && get_bin_index(maf, bin_index)) {
	_calculators.at(bin_index).add_value(from_string<double>(rsq));
      }
    }
    input->close();
    delete input;
    input = 0;
  } catch (...) {
    if (input) {
      input->close();
      delete input;
      input = 0;
    }
    throw;
  }
}

void dynamic_r2::threshold_calculator::report_variant_data(const std::string &filename,
							   finter::finter_writer *output_inclusion,
							   finter::finter_writer *output_exclusion) const {
  if (!output_inclusion && !output_exclusion)
    return;
  std::string line = "", variant_id = "", reference_allele = "", alternate_allele = "";
  double alternate_allele_frequency = 0.0, maf = 0.0;
  std::string avgcall = "", rsq = "";
  unsigned bin_index = 0;
  finter::finter_reader *input = 0;
  try {
    input = finter::reconcile_reader(filename);
    input->getline(&line);
    while (input->getline(&line)) {
      std::istringstream strm1(line);
      if (!(strm1 >> variant_id >> reference_allele >> alternate_allele
	    >> alternate_allele_frequency >> maf >> avgcall >> rsq))
	throw std::domain_error("threshold_calculator::add_file: cannot parse info file \"" + filename + "\" line \"" + line + "\"");
      if (rsq.compare("-") && get_bin_index(maf, bin_index)) {
	if (output_inclusion && from_string<double>(rsq) >= _calculators.at(bin_index).max_included_value()) {
	  output_inclusion->writeline(variant_id);
	}
	if (output_exclusion && from_string<double>(rsq) < _calculators.at(bin_index).max_included_value()) {
	  output_exclusion->writeline(variant_id);
	}
      } else if (!rsq.compare("-") && output_inclusion) {
	output_inclusion->writeline(variant_id);
      }
    }
    input->close();
    delete input;
    input = 0;
  } catch (...) {
    if (input) {
      delete input;
    }
    throw;
  }
}
