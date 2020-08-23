#include <iostream>

#include "libxsckt/xsckt.h"

int main() {

	std::cout << "xsckt\n";

	std::cout << xsckt::startup() << "\n";

	xsckt::cleanup();

	system("pause");
	return 0;
  
}
