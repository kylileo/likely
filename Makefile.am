# add path to boost.m4
ACLOCAL_AMFLAGS = -I .

# global compile and link options
AM_CPPFLAGS = $(BOOST_CPPFLAGS) $(SIMD_FLAGS)

# targets to build and install
lib_LTLIBRARIES = liblikely.la
bin_PROGRAMS = likelytest likelymc likelyinterp likelywsum likelyinteg likelyrand \
	likelybicubic likelytricubic likelycov likelydata likelyfitpar resamplingtest likelyquantile

# extra targets that should not be installed
noinst_PROGRAMS = demo1 demo2

# targets that contain unit tests
check_PROGRAMS = likelycheck

# add our pkgconfig file to the install target
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = likely.pc

# any library dependencies not already added by configure can be added here
liblikely_la_LIBADD = $(BOOST_REGEX_LDFLAGS) $(BOOST_REGEX_LIBS)

# instructions for building the library
liblikely_la_SOURCES = \
	likely/FitParameter.cc \
	likely/FitModel.cc \
	likely/FitParameterStatistics.cc \
	likely/FunctionMinimum.cc \
	likely/AbsEngine.cc \
	likely/EngineRegistry.cc \
	likely/MarkovChainEngine.cc \
	likely/Interpolator.cc \
	likely/Integrator.cc \
	likely/Random.cc \
	likely/AbsAccumulator.cc \
	likely/WeightedAccumulator.cc \
	likely/WeightedCombiner.cc \
	likely/QuantileAccumulator.cc \
	likely/ExactQuantileAccumulator.cc \
	likely/BiCubicInterpolator.cc \
	likely/TriCubicInterpolator.cc \
	likely/AbsBinning.cc \
	likely/UniformBinning.cc \
	likely/NonUniformBinning.cc \
	likely/UniformSampling.cc \
	likely/NonUniformSampling.cc \
	likely/CovarianceMatrix.cc \
	likely/CovarianceAccumulator.cc \
	likely/BinnedGrid.cc \
	likely/BinnedData.cc \
	likely/BinnedDataResampler.cc \
	likely/test/TestLikelihood.cc

# library headers to install (nobase prefix preserves directories under bosslya)
# Anything that includes config.h should *not* be listed here.
# and is not part of the package public API.
nobase_include_HEADERS = \
	likely/likely.h \
	likely/types.h \
	likely/function.h \
	likely/function_impl.h \
	likely/RuntimeError.h \
	likely/FitParameter.h \
	likely/FitModel.h \
	likely/FitParameterStatistics.h \
	likely/FunctionMinimum.h \
	likely/AbsEngine.h \
	likely/EngineRegistry.h \
	likely/MarkovChainEngine.h \
	likely/Interpolator.h \
	likely/Integrator.h \
	likely/Random.h \
	likely/AbsAccumulator.h \
	likely/WeightedAccumulator.h \
	likely/WeightedCombiner.h \
	likely/QuantileAccumulator.h \
	likely/ExactQuantileAccumulator.h \
	likely/BiCubicInterpolator.h \
	likely/TriCubicInterpolator.h \
	likely/AbsBinning.h \
	likely/BinningError.h \
	likely/UniformBinning.h \
	likely/NonUniformBinning.h \
	likely/UniformSampling.h \
	likely/NonUniformSampling.h \
	likely/CovarianceMatrix.h \
	likely/CovarianceAccumulator.h \
	likely/BinnedGrid.h \
	likely/BinnedData.h \
	likely/BinnedDataResampler.h \
	likely/test/TestLikelihood.h

# add GSL features when libgsl is available
if USE_GSL
liblikely_la_SOURCES += likely/GslEngine.cc likely/GslErrorHandler.cc
nobase_include_HEADERS += likely/GslEngine.h likely/GslErrorHandler.h
endif

# add Minuit2 features when libMinuit2 is available
if USE_MINUIT2
liblikely_la_SOURCES += likely/MinuitEngine.cc
nobase_include_HEADERS += likely/MinuitEngine.h
endif

# instructions for building each program

likelytest_SOURCES = src/likelytest.cc
likelytest_DEPENDENCIES = $(lib_LIBRARIES)
likelytest_LDADD = liblikely.la $(BOOST_PROGRAM_OPTIONS_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIBS)

likelymc_SOURCES = src/likelymc.cc
likelymc_DEPENDENCIES = $(lib_LIBRARIES)
likelymc_LDADD = liblikely.la $(BOOST_PROGRAM_OPTIONS_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIBS)

likelyinterp_SOURCES = src/likelyinterp.cc
likelyinterp_DEPENDENCIES = $(lib_LIBRARIES)
likelyinterp_LDADD = liblikely.la $(BOOST_PROGRAM_OPTIONS_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIBS)

likelywsum_SOURCES = src/likelywsum.cc
likelywsum_DEPENDENCIES = $(lib_LIBRARIES)
likelywsum_LDADD = liblikely.la $(BOOST_PROGRAM_OPTIONS_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIBS)

likelyinteg_SOURCES = src/likelyinteg.cc
likelyinteg_DEPENDENCIES = $(lib_LIBRARIES)
likelyinteg_LDADD = liblikely.la

likelyrand_SOURCES = src/likelyrand.cc
likelyrand_DEPENDENCIES = $(lib_LIBRARIES)
likelyrand_LDADD = liblikely.la

likelybicubic_SOURCES = src/likelybicubic.cc
likelybicubic_DEPENDENCIES = $(lib_LIBRARIES)
likelybicubic_LDADD = liblikely.la $(BOOST_PROGRAM_OPTIONS_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIBS)

likelytricubic_SOURCES = src/likelytricubic.cc
likelytricubic_DEPENDENCIES = $(lib_LIBRARIES)
likelytricubic_LDADD = liblikely.la $(BOOST_PROGRAM_OPTIONS_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIBS)

likelycov_SOURCES = src/likelycov.cc
likelycov_DEPENDENCIES = $(lib_LIBRARIES)
likelycov_LDADD = liblikely.la

likelydata_SOURCES = src/likelydata.cc
likelydata_DEPENDENCIES = $(lib_LIBRARIES)
likelydata_LDADD = liblikely.la

likelyfitpar_SOURCES = src/likelyfitpar.cc
likelyfitpar_DEPENDENCIES = $(lib_LIBRARIES)
likelyfitpar_LDADD = liblikely.la

resamplingtest_SOURCES = src/resamplingtest.cc
resamplingtest_DEPENDENCIES = $(lib_LIBRARIES)
resamplingtest_LDADD = liblikely.la $(BOOST_PROGRAM_OPTIONS_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIBS)

likelyquantile_SOURCES = src/likelyquantile.cc
likelyquantile_DEPENDENCIES = $(lib_LIBRARIES)
likelyquantile_LDADD = liblikely.la

demo1_SOURCES = src/demo1.cc
demo1_DEPENDENCIES = $(lib_LIBRARIES)
demo1_LDADD = liblikely.la

demo2_SOURCES = src/demo2.cc
demo2_DEPENDENCIES = $(lib_LIBRARIES)
demo2_LDADD = liblikely.la

likelycheck_SOURCES = \
  test/likelycheck.cc \
	test/CovarianceMatrixTest.cc \
	test/UniformBinningTest.cc \
	test/UniformSamplingTest.cc \
	test/NonUniformBinningTest.cc \
	test/NonUniformSamplingTest.cc \
	test/BinnedDataTest.cc \
	test/FitParameterTest.cc \
	test/ExactQuantileAccumulatorTest.cc
likelycheck_DEPENDENCIES = $(lib_LIBRARIES)
likelycheck_LDADD = liblikely.la $(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS) $(BOOST_UNIT_TEST_FRAMEWORK_LIBS)
