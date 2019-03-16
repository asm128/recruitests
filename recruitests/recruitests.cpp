#include "recruitests.h"

#include <iostream>
#include <string>

int64_t										rec::getNumericInput					()									{
	// Get user input
	::std::string									userChoice;
	int64_t											selectedOption							= -1;
	while(0 == userChoice.size()) {
		::std::getline(::std::cin, userChoice);
		if(userChoice.size() >= 4 && (userChoice.substr(0, 4) == "exit" || userChoice.substr(0, 4) == "back"))
			return -2; // return exit code
		// Convert the input string to a menuitem index.
		selectedOption								= -1;
		try { selectedOption = ::std::stoll(userChoice); }
		catch(::std::invalid_argument) {}	// this try/catch bullshit is required because std::stoi() throws exceptions if the input can't be converted to a number.
	}
	return selectedOption;
}
