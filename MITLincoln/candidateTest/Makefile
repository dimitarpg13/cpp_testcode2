SRC_DIR := src
BUILD_DIR := build
LIB_DIR := lib
CXXFLAGS := -g -O0 -Wall -no-pie -std=c++11 $(LDFLAGS)

SRCS := \
$(SRC_DIR)/OrderedStatCfarImpl.cpp

OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

.PHONY : \
all \
test \
clean

test : $(BUILD_DIR)/DimitarsTest

all : \
test

$(BUILD_DIR)/DimitarsTest : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ 

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.d : $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@set -e; rm -f $@; \
	$(CXX) -MM $(CXXFLAGS) $(INCLUDES) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(BUILD_DIR)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(BUILD_DIR) :
	@mkdir -p $@

clean :
	@rm -rf $(BUILD_DIR)

-include $(OBJS:.o=.d)
