/*!
 \file cargs.h
 \brief command line argument handling
 \author Cameron Palmer
 
 Thanks to https://www.boost.org/doc/libs/1_70_0/doc/html/program_options/tutorial.html#id-1.3.32.4.3
 */
 
#ifndef __DYNAMIC_R2_CARGS_H__
#define __DYNAMIC_R2_CARGS_H__

#include <string>
#include <stdexcept>
#include <boost/program_options.hpp>

namespace dynamic_r2 {
  /*!
    \class cargs
    \brief command line argument parser using boost::program_options
   */
  class cargs {
  public:
    /*!
      \brief constructor with program arguments
      @param argc number of arguments including program name
      @param argv string array containing actual arguments
     */
    cargs(int argc, char **argv) : _desc("Recognized options") {
      initialize_options();
      boost::program_options::store(boost::program_options::parse_command_line(argc, argv, _desc), _vm);
      boost::program_options::notify(_vm);
    }
    /*!
      \brief copy constructor
      @param obj existing cargs object
     */
    cargs(const cargs &obj)
      : _desc(obj._desc),
	_vm(obj._vm) {}
    /*!
      \brief destructor
     */
    ~cargs() throw() {}

    /*!
      \brief set user help documentation and default values for parameters as needed
      
      Note the weird syntax with overloaded () operators. The lists are not separated by commas.
     */
    void initialize_options();

    /*!
      \brief get optional name of file in which to store included variant IDs
      \return included variant ID output filename, if specified, or "" otherwise
     */
    std::string get_included_variant_output_filename() const {
      return compute_parameter<std::string>("output-included-variants");
    }

    /*!
      \brief get optional name of file in which to store excluded variant IDs
      \return excluded variant ID output filename, if specified, or "" otherwise
     */
    std::string get_excluded_variant_output_filename() const {
      return compute_parameter<std::string>("output-excluded-variants");
    }
    
    /*!
      \brief get a user-specified hard lower limit on frequencies included in analysis. anything lower than this
      limit will be filtered entirely
      \return hard lower limit on inclusion frequency
      
      note that the default of 0.0 means that the bottom frequency bin contains everything <= its breakpoint
     */
    double get_hard_lower_limit() const {
      try {
	return compute_parameter<double>("use-hard-lower-limit");
      } catch (const std::domain_error &e) {
	return 0.0;
      }
    }
    /*!
      \brief get user-specified breakpoints for frequency bins
      \return user-specified breakpoints for frequency bins in a vector

      the behavior of the lowest bin depends on the flag --use-hard-lower-limit
     */
    std::vector<double> get_frequency_bounds() const {
      return compute_parameter<std::vector<double> >("frequency-bounds");
    }
    
    /*!
      \brief get target per-bin average r2 after filtering
      \return target r2
     */
    double get_r2_target() const {
      return compute_parameter<double>("r2-target");
    }
    double get_minimum_r2() const {
      return compute_parameter<double>("minimum-r2");
    }
    /*!
      \brief determine whether the user has requested help documentation
      \return whether the user has requested help documentation

      This test is separate from whether the user has run the software with no flags
     */
    bool help() const {
      return compute_flag("help");
    }

    /*!
      \brief determine whether the user is a boss and wants to use Yun Li's parameters
      \return whether the user is a boss
     */
    bool get_yun_li_mode() const {
      return compute_flag("yun-li-mode");
    }

    /*!
      \brief get user-specified list of minimac info files to parse
      \return vector of files from command line
     */
    std::vector<std::string> get_input_files() const {
      return compute_parameter<std::vector<std::string> >("imputed-info");
    }
    /*!
      \brief find status of arbitrary flag
      @param tag name of flag
      \return whether the flag is set

      This is the underlying accessor function used by the custom flag accessors,
      and can be used for arbitrary flag additions if you don't want to type out the
      custom access functions or want to allow dynamic specification from a config file.
     */
    bool compute_flag(const std::string &tag) const {
      return _vm.count(tag);
    }
    /*!
      \brief find value of arbitrary parameter
      @tparam value_type class to which the value should be cast
      @param tag name of parameter
      \return value of parameter if specified

      \warning throws exception if parameter was not set and had no default
     */
    template <class value_type>
    value_type compute_parameter(const std::string &tag) const {
      if (_vm.count(tag)) {
	return _vm[tag].as<value_type>();
      }
      throw std::domain_error("cargs: requested parameter \"" + tag + "\" unset");
    }

    /*!
      \brief report help documentation to arbitrary output stream
      @param out stream to which to write help documentation

      Parameter should probably be std::cout or std::cerr at your preference.
     */
    void print_help(std::ostream &out) {
      if (!(out << _desc))
	throw std::domain_error("cargs::print_help: unable to write to stream");
    }
  private:
    /*!
      \brief default constructor
      \warning disabled
     */
    cargs() {throw std::domain_error("cargs: do not use default constructor");}
    boost::program_options::options_description _desc; //!< help documentation string
    boost::program_options::variables_map _vm; //!< storage of parsed command line settings
  };
}
 
#endif //__DYNAMIC_R2_CARGS_H__
