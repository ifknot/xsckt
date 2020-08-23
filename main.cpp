#include <iostream>

#include "libxsckt/xsckt.h"

int main() {

	std::cout << "xsckt\n";

	std::cout << xsckt::startup();

	xsckt::cleanup();

	return 0;
  
}
