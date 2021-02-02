/*!
  \file cargs.cc
  \brief method implementation for command line argument parser class
*/

#include "dynamic_r2/cargs.h"

void dynamic_r2::cargs::initialize_options() {
  _desc.add_options()
    ("help,h", "emit this help message")
    ("output-included-variants", boost::program_options::value<std::string>()->default_value(""), "if specified, write variants included in the final set to this file")
    ("output-excluded-variants", boost::program_options::value<std::string>()->default_value(""), "if specified, write variants excluded from the final set to this file")
    ("imputed-info", boost::program_options::value<std::vector<std::string> >()->multitoken(), "minimac3 info file(s) requiring thresholding")
    ("frequency-bounds", boost::program_options::value<std::vector<double> >()->multitoken()->default_value(std::vector<double>{0.005, 0.01, 0.03, 0.05}, "0.005, 0.01, 0.03, 0.05"), "breaks between frequency bins")
    ("use-hard-lower-limit", boost::program_options::value<double>(), "lowest bin should not include everything but rather terminate at this lower bound")
    ("r2-target", boost::program_options::value<double>()->default_value(0.8), "target average r2 for each bin")
    ("minimum-r2", boost::program_options::value<double>()->default_value(0.3), "standard r2 threshold to apply uniformly across all variants")
    ("yun-li-mode", "stand on the shoulders of giants. hide behind people better at imputation than you. use the settings in doi:10.1002/gepi.21603")
    ;
}
