// Created 22-May-2011 by David Kirkby (University of California, Irvine) <dkirkby@uci.edu>

#ifndef LIKELY_MINUIT_ENGINE
#define LIKELY_MINUIT_ENGINE

#include "likely/types.h"
#include "likely/AbsEngine.h"

#include "Minuit2/FCNGradientBase.h"

#include "boost/smart_ptr.hpp"

#include <vector>

namespace ROOT {
namespace Minuit2 {
    class MnUserParameterState;
}} // ROOT::Minuit2

namespace likely {
    // Implements minimization and error analysis using the Minuit2 library.
	class MinuitEngine : public ROOT::Minuit2::FCNGradientBase, public AbsEngine {
	public:
	    // Creates a new engine for the specified function of the specified number
	    // of parameters.
		MinuitEngine(FunctionPtr f, GradientCalculatorPtr gc, FitParameters const &parameters,
		    std::string const &algorithm);
		virtual ~MinuitEngine();
		// Evaluates the engine's function for the specified input parameter values.
        virtual double operator()(Parameters const& pValues) const;
        // Evaluates the function gradient for the specified input parameters.
        // (Use likely::Gradient below to distinguish from the method name)
        virtual likely::Gradient Gradient(Parameters const& pValues) const;
        virtual bool CheckGradient() const;
        // Returns the change in function value corresponding to one unit of error.
        // Can be changed to calculate different confidence intervals. For 1-sigma errors,
        // this value should be 1 for both chi-square and -2log(L) functions.
        virtual double Up() const;
        // Returns a smart pointer to the initial parameter state.
        typedef boost::shared_ptr<ROOT::Minuit2::MnUserParameterState> StatePtr;
        StatePtr getInitialState();
        // Runs a stateless minimization templated on a ROOT::Minuit2::FunctionMinimizer subclass.
        template<class T>
        void minimize(FunctionMinimumPtr fmin, double prec, int maxfcn, int strategy);
	private:
        int _nPar;
        FunctionPtr _f;
        GradientCalculatorPtr _gc;
        StatePtr _initialState;
        void _setInitialState(FunctionMinimumPtr fmin);
	}; // MinuitEngine
	
	inline MinuitEngine::StatePtr MinuitEngine::getInitialState() {
        return _initialState;
	}
    inline bool MinuitEngine::CheckGradient() const { return false; }

    // Registers our named methods.
    void registerMinuitEngineMethods();
	
} // likely

#endif // LIKELY_MINUIT_ENGINE
