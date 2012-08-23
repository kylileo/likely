// Created 28-May-2012 by David Kirkby (University of California, Irvine) <dkirkby@uci.edu>

#include "likely/FitParameter.h"
#include "likely/RuntimeError.h"

#include "boost/format.hpp"
#include "boost/function.hpp"
#include "boost/bind.hpp"
#include "boost/regex.hpp"
#include "boost/foreach.hpp"
#include "boost/spirit/include/qi.hpp"
#include "boost/spirit/include/phoenix_core.hpp"
#include "boost/spirit/include/phoenix_operator.hpp"
#include "boost/spirit/include/phoenix_stl.hpp"

#include <iterator>
#include <iostream>

namespace local = likely;

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

std::string const &local::FitParameter::getValidNameCharacters() {
    // The hyphen must appear as the last char below for correct interpretation.
    static std::string charset(" a-zA-Z0-9()*/+-");
    return charset;
}

local::FitParameter::FitParameter(std::string const &name, double value, double error)
: _name(name)
{
    boost::cmatch what;
    boost::regex pattern(std::string("[") + getValidNameCharacters() + "]+");
    if(!boost::regex_match(name,pattern)) {
        throw RuntimeError("FitParameter: name uses invalid characters \"" + name + "\"");
    }
    if('*' == name[name.size()-1]) {
        throw RuntimeError("FitParameter: name cannot end with asterisk.");
    }
    setValue(value);
    setError(error);
}

void local::FitParameter::setError(double error) {
    if(error < 0) {
        throw RuntimeError("FitParameter::setError: error must be >= 0.");
    }
    _error = error;
}

void local::getFitParameterValues(FitParameters const &parameters, Parameters &values, bool onlyFloating) {
    values.resize(0);
    if(!onlyFloating) values.reserve(parameters.size());
    for(FitParameters::const_iterator iter = parameters.begin(); iter != parameters.end(); ++iter) {
        if(!onlyFloating || iter->isFloating()) values.push_back(iter->getValue());
    }
}

void local::getFitParameterErrors(FitParameters const &parameters, Parameters &errors, bool onlyFloating) {
    errors.resize(0);
    if(!onlyFloating) errors.reserve(parameters.size());
    for(FitParameters::const_iterator iter = parameters.begin(); iter != parameters.end(); ++iter) {
        if(!onlyFloating || iter->isFloating()) errors.push_back(iter->getError());
    }
}

void local::getFitParameterNames(FitParameters const &parameters, std::vector<std::string> &names,
bool onlyFloating) {
    names.resize(0);
    if(!onlyFloating) names.reserve(parameters.size());
    for(FitParameters::const_iterator iter = parameters.begin(); iter != parameters.end(); ++iter) {
        if(!onlyFloating || iter->isFloating()) names.push_back(iter->getName());
    }
}

int local::countFitParameters(FitParameters const &parameters, bool onlyFloating) {
    if(!onlyFloating) return parameters.size();
    int count(0);
    for(FitParameters::const_iterator iter = parameters.begin(); iter != parameters.end(); ++iter) {
        if(iter->isFloating()) count++;
    }
    return count;
}

void local::printFitParametersToStream(FitParameters const &parameters, std::ostream &out,
std::string const &formatSpec) {
    boost::format formatter(formatSpec), label("%20s = ");
    for(FitParameters::const_iterator iter = parameters.begin(); iter != parameters.end(); ++iter) {
        out << (label % iter->getName()) << (formatter % iter->getValue());
        if(iter->isFloating()) {
            out << " +/- " << formatter % iter->getError();
        }
        out << std::endl;
    }
}

int local::findFitParameterByName(FitParameters const &parameters, std::string const &name) {
    for(FitParameters::const_iterator iter = parameters.begin(); iter != parameters.end(); ++iter) {
        if(name == iter->getName()) return std::distance(parameters.begin(),iter);
    }
    throw RuntimeError("findFitParameterByName: name not found: '" + name + "'");
}

void local::setFitParameterValues(FitParameters &parameters, Parameters::const_iterator first,
Parameters::const_iterator last, bool onlyFloating) {
    if(std::distance(first,last) != countFitParameters(parameters, onlyFloating)) {
        throw RuntimeError("setFitParameterValues: wrong number of values provided.");
    }
    Parameters::const_iterator nextValue(first);
    for(FitParameters::iterator iter = parameters.begin(); iter != parameters.end(); ++iter) {
        if(!onlyFloating || iter->isFloating()) iter->setValue(*nextValue++);
    }
}

void local::setFitParameterValues(FitParameters &parameters, Parameters const &values,
bool onlyFloating) {
    setFitParameterValues(parameters,values.begin(),values.end(),onlyFloating);
}

namespace likely {
namespace fitpar {
    // Declare our script grammar.
    struct Grammar : qi::grammar<std::string::const_iterator,ascii::space_type> {

        Grammar(FitParameters &_params) : params(_params), base_type(script) {

            using qi::double_;
            using qi::lit;
            using qi::no_skip;
            using qi::char_;

            // Commands are separated by semicolons. A final semicolon is optional.
            script = ( command % ';' ) >> -lit(';');

            // Declare the supported commands and their semantic actions.
            command =
                ( "value" >> name >> '=' >> double_ )   [boost::bind(&Grammar::setValue,this,::_1)] |
                ( "error" >> name >> '=' >> double_ )   [boost::bind(&Grammar::setError,this,::_1)] |
                ( "fix" >> name >> '=' >> double_ )     [boost::bind(&Grammar::fixat,this,::_1)] |
                ( "fix" >> name )                       [boost::bind(&Grammar::fix,this)] |
                ( "release" >> name )                   [boost::bind(&Grammar::release,this)];
            
            // Commands share a common name parser.
            name = lit('[')[boost::bind(&Grammar::beginName,this)]
                >> no_skip[+char_(FitParameter::getValidNameCharacters())[
                    boost::bind(&Grammar::addToName,this,::_1)]]
                >> lit(']')[boost::bind(&Grammar::endName,this)];
            
        }
        
        // Any space_type in this template must match the grammar template above.
        qi::rule<std::string::const_iterator, ascii::space_type> script, command, name;        

        FitParameters &params;
        std::string theName;
        std::vector<int> selected;
        
        void beginName() {
            theName.clear();
        }
        void addToName(char c) {
            theName += c;
        }
        void endName() {
            selected.resize(0);
            if('*' == theName[theName.size()-1]) {
                theName.erase(theName.end()-1);
                boost::regex pattern(std::string("^\\Q")+theName+"\\E");
                for(int index = 0; index < params.size(); ++index) {
                    if(boost::regex_search(params[index].getName(),pattern)) {
                        selected.push_back(index);
                    }
                }
                if(selected.empty()) {
                    throw RuntimeError("FitParameter: wildcard pattern must match at least one name.");
                }
            }
            else {
                selected.push_back(findFitParameterByName(params,theName));
            }
        }
        void setValue(double value) {
            BOOST_FOREACH(int index, selected) params[index].setValue(value);
        }
        void setError(double error) {
            BOOST_FOREACH(int index, selected) params[index].setError(error);
        }
        void fix() {
            BOOST_FOREACH(int index, selected) params[index].fix();
        }
        void fixat(double value) {
            BOOST_FOREACH(int index, selected) {
                params[index].setValue(value);
                params[index].fix();
            }
        }
        void release() {
            BOOST_FOREACH(int index, selected) params[index].release();
        }
    };
} // grammar
} // likely

void local::modifyFitParameters(FitParameters &parameters, std::string const &script) {
    FitParameters modified(parameters);
    fitpar::Grammar grammar(modified);
    std::string::const_iterator iter = script.begin();
    bool ok = qi::phrase_parse(iter, script.end(), grammar, ascii::space);
    if(!ok || iter != script.end()) {
        throw RuntimeError("modifyFitParameters: syntax error in script.");
    }
    parameters = modified;
}

std::string local::roundValueWithError(double value, std::vector<double> const &errors, std::string const &seperator) {
    if(!errors.size()) return (boost::format("%f") % value).str();
    double largestOffset;
    for(int i = 0; i < errors.size(); ++i){
        if(errors[i] <= 0) {
            throw RuntimeError("FitParameter::roundValueWithError: error must be > 0.");
        }
        // What order of magnitude are we dealing with here?
        int order = std::ceil(std::log10(errors[i]));
        // Check the three highest order digits
        int power = 3 - order;
        double offset = std::pow(10., power);
        int firstThreeDigits = std::floor(errors[i]*offset);
        // Adjust offset to first significant digit
        offset /= 100.;
        if(firstThreeDigits >= 100 && firstThreeDigits <= 355) {
            // Adjust offset to save second significant digit
            offset *= 10.;
        }
        // Save the offset to the smallest error
        if(i == 0 || offset > largestOffset) {
            largestOffset = offset;
        }
    }
    // Configure floating point output format
    int precision = int(std::log10(largestOffset));
    std::string format = (boost::format("%s%df") % "%." % (precision < 0 ? 0 : precision)).str();
    std::string formattedValueWithError;
    // Round value and apply floating point format
    long shiftedValue = std::floor(value*largestOffset+.5);
    formattedValueWithError += (boost::format(format) % (shiftedValue/largestOffset)).str();
    // Round errors and apply floating point format
    for(int i = 0; i < errors.size(); ++i){
        long shiftedError = std::floor(errors[i]*largestOffset + .5);
        formattedValueWithError += (boost::format(" %s " + format) % seperator % (shiftedError/largestOffset)).str();
    }
    // Return formatted value with error(s)
    return formattedValueWithError;
}