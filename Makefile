#   ============================================================================
#   Youhua Xu Aug-27-2018
#   ============================================================================
#   Make a build dir for compilation
MDIR := $(shell pwd)
WRKDIR = $(MDIR)/build
BINDIR = $(MDIR)/bin

#	make build & binary dirs
.base:
	if ! [ -e $(WRKDIR) ]; then mkdir $(WRKDIR) ; fi;
	touch $(WRKDIR)/.base;
	if ! [ -e $(BINDIR) ]; then mkdir $(BINDIR) ; fi;
	touch $(BINDIR)/.base;

#   ============================================================================
#   Set the source file path
vpath %.cpp main:src:src_test:tools:iniparser
vpath %.c main:src:src_test:tools:iniparser
vpath %.hpp include:iniparser
vpath %.h include:iniparser
vpath %.o build
vpath .base build

INCLUDES = -I $(MDIR)/include

CC          = g++
# CC          = g++-4.8
# CC          = clang++
CCFLAG  	= -Wall -DHAVE_INLINE

# CCFLAG += -D_ENABLE_DEBUG_INFO_
CCFLAG 		+= -D_USE_LAGRANGE_INTERP_
CCFLAG		+= -D_INCLUDE_VELOCITY_

OPTFLAG		= -O2 #-ffast-math #( not recognized by intel compiler )
# OPTFLAG += -pg



LDFLAG      =
#   http://www.tuicool.com/articles/NBfeYj
ifeq ($(shell uname -s),Linux)
	LDFLAG	+= -Wl,--no-as-needed
	LDFLAG  += -L /home/xyh/Tools/lib/
	# ifeq ($(CC),g++)
	# 	CCFLAG += -fno-aggressive-loop-optimizations
	# endif
endif

LIBS	= -lgsl -lgslcblas -lm #-larmadillo

ifeq ($(shell uname -s),Darwin)
	LDFLAG	+= -framework Accelerate #(-framework Accelerate is for Mac OSX)
endif

%.o: %.cpp .base
	cd $(WRKDIR);$(CC) $(OPTFLAG) $(CCFLAG) $(INCLUDES) -c ../$< -o $*.o

%.o: %.c .base
	cd $(WRKDIR);$(CC) $(OPTFLAG) $(CCFLAG) $(INCLUDES) -c ../$< -o $*.o

MAIN = inspect_main.o
DEPS = container.o saa.o sun.o moon.o satellite.o solar_panel.o cmg.o rotation.o \
	ephemeris_api.o	gsl_func.o coordinates.o gnulliver.o ephcom.o mock_orbit.o misc.o \
	dictionary.o iniparser.o \
	inspector.o inspect_sun.o inspect_moon.o inspect_earth.o inspect_cmg.o inspect_saa.o combine_drift.o

# INI_DICTIONARY = dictionary.o
# INI_PARSER = iniparser.o

TOOL_GEN_ORBIT = gen_orbit.o
TOOL_GET_SAT_POS = get_sat_pos.o
TOOL_GET_SUN_ORBIT = get_sun_orbit.o
TOOL_GET_SUN_POSITION = get_sun_position.o
TOOL_COMBINE_DRIFT = combine_drift_main.o

TEST_GSL_FUNC = test_gsl_func.o
TEST_CONTAINER = test_container.o
TEST_CMG_USE = test_CMG_use.o
TEST_JPL405 = test_jpl405.o
TEST_SAT = test_satellite.o

TEST_ORBIT_INTERP = test_orbit_interp.o
TEST_ORBIT_TIME_COST = test_orbit_time_cost.o
TEST_COORDINATES = test_coordinates.o

TEST_SAA = test_saa.o
TEST_SUBASTRAL = test_subastral.o
TEST_SUBASTRAL2 = test_subastral2.o
TEST_SUBASTRAL3 = test_subastral3.o

all:inspect tools tests
tools:gen_orbit get_sat_pos get_sun_orbit get_sun_position combine_drift
tests:test_container test_gsl_func test_cmg_use test_jpl405 test_sat gen_orbit \
test_orbit_interp test_orbit_time_cost test_coordinates test_saa test_subastral \
test_subastral2 test_subastral3

####
inspect:${MAIN} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@

#### the following are small tools
gen_orbit:${TOOL_GEN_ORBIT} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
get_sat_pos:${TOOL_GET_SAT_POS} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
get_sun_orbit:${TOOL_GET_SUN_ORBIT} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
get_sun_position:${TOOL_GET_SUN_POSITION} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
combine_drift:${TOOL_COMBINE_DRIFT} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@

# the following are tests
test_gsl_func:${TEST_GSL_FUNC} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
test_container:${TEST_CONTAINER} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
test_cmg_use:${TEST_CMG_USE} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
test_jpl405:${TEST_JPL405} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
test_sat:${TEST_SAT} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
test_orbit_interp:${TEST_ORBIT_INTERP} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
test_orbit_time_cost:${TEST_ORBIT_TIME_COST} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
test_coordinates:${TEST_COORDINATES} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
test_saa:${TEST_SAA} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
test_subastral:${TEST_SUBASTRAL} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
test_subastral2:${TEST_SUBASTRAL2} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
test_subastral3:${TEST_SUBASTRAL3} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@
#   ================================================================================================
.PHONY:clean tidy run
clean: .base
	rm -rf $(WRKDIR);
tidy:
	make clean; rm -rf $(BINDIR);
