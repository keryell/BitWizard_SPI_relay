TARGET = main bw_spi_relay_control

# There is a compilation issue with fmt and C++20
CXXFLAGS = -std=c++17 -Wall -g
CPPFLAGS = -I ../include

# Installing the libboost-all-dev package may help for this library
LDLIBS = -lboost_program_options -lfmt -lrt -lpthread

all: $(TARGET)

clean:
	$(RM) $(TARGET)
