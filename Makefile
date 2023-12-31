SRCDIR    = src
BUILDDIR  = build
TARGET    = ltl-solver

SRCEXT    = cpp
ALLSRCS   = $(wildcard $(SRCDIR)/*.$(SRCEXT))
SATSRCS   = $(wildcard $(SRCDIR)/*CryptoMiniSatSolver.$(SRCEXT))
SOURCES   = $(filter-out $(SATSRCS),  $(ALLSRCS))
OBJECTS   = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

CXX       = g++
CFLAGS    = -Wall -Wno-parentheses -Wno-sign-compare -std=c++11
COPTIMIZE = -O3
LFLAGS    = -Wall
IFLAGS    = -I include

CFLAGS   += $(COPTIMIZE)
CFLAGS   += -D __STDC_LIMIT_MACROS -D __STDC_FORMAT_MACROS
CFLAGS   += -D CONE_OF_INFLUENCE
#CFLAGS   += -D DEBUG_MODE

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	@echo "$(CXX) $(OBJECTS) -o $(TARGET) $(LFLAGS)" $(LDFLAGS); $(CXX) $(OBJECTS) -o $(TARGET) $(LFLAGS) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo "Compiling..."
	@mkdir -p $(BUILDDIR)
	@echo "$(CXX) $(CFLAGS) $(IFLAGS) -c -o $@ $<"; $(CXX) $(CFLAGS) $(IFLAGS) -c -o $@ $<

clean:
	@echo "Cleaning..."
	@echo "rm -rf $(BUILDDIR) $(TARGET)"; rm -rf $(BUILDDIR) $(TARGET)
