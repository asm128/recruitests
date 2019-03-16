#include "rentals.h"

static	int64_t								inputRental								(::bik::SRental & rental)			{
	int												selection								= -1;
	while(selection < 0) {
		printf("\n\nSelect rental type: "
			"\n1: Rental by %s"
			"\n2: Rental by %s"
			"\n3: Rental by %s"
			"\n0: %s\n"
			, ::bik::g_Prices[0].Text
			, ::bik::g_Prices[1].Text
			, ::bik::g_Prices[2].Text
			, "Back"
			);
		selection									= (int)::rec::getNumericInput();
		char											output[512]								= {};
		switch(selection) { 
		case -2	: 
		case 0	: return -2;
		default: 
			sprintf_s(output, "Error: Invalid rental type: '%c'.\n", selection);
			break;
		case 1 + ::bik::RENTAL_TYPE_HOUR	: 
		case 1 + ::bik::RENTAL_TYPE_DAY		: 
		case 1 + ::bik::RENTAL_TYPE_MONTH	: 
			rental.Type									= (::bik::RENTAL_TYPE)(selection - 1); 
			sprintf_s(output, "Selected rental type: %s.\n", ::bik::g_Prices[rental.Type].Text);
			break;
		}
		printf("\n- %s", output);
	}
	int												bikes									= -1;
	while(bikes < 0) {
		printf("\n\nType bike count or \"exit\" (without quotes) to exit:\n");
		bikes										= (int)::rec::getNumericInput();
		if(bikes == -2) 
			return -2;
		char											output[512]								= {};
		if(bikes <= 0 || bikes > 0x7FFF)
			sprintf_s(output, "Error: Invalid bike count: '%c'.\n", bikes);
		else {
			rental.Bikes								= (int16_t)bikes; 
			sprintf_s(output, "Selected bikes: %u.\n", rental.Bikes);
		}
		printf("\n- %s", output);
	}
	const char										* textTypeSelected						= ::bik::g_Prices[rental.Type].Text;
	int												hours									= -1;
	while(hours <= 0) {
		printf("\n\nType rental time (in %ss) or \"exit\" (without quotes) to exit:\n", textTypeSelected);
		hours										= (int)::rec::getNumericInput();
		if(hours == -2)
			return -2;
		char											output[512]								= {};
		if(hours <= 0 || hours > 0x7FFF)
			sprintf_s(output, "Error: Invalid time: '%c'.\n", hours);
		else {
			rental.Time									= (int16_t)hours; 
			sprintf_s(output, "Selected time: %u %s%s.\n", rental.Time, textTypeSelected, (rental.Time > 1) ? "s" : "");
		}
		printf("\n- %s", output);
	}
	return 0;
}

static	int64_t								inputRentalOriginal						(::bik::SRental & rental)			{
	bool											reset									= false;
	int												selection								= -1;
	while(selection < 0) {
		printf("\n\nSelect rental type: "
			"\n1: Rental by %s"
			"\n2: Rental by %s"
			"\n3: Rental by %s"
			"\n0: %s\n"
			, ::bik::g_Prices[0].Text
			, ::bik::g_Prices[1].Text
			, ::bik::g_Prices[2].Text
			, "Back"
			);
		selection									= (int)::rec::getNumericInput();
		char											output[512]								= {};
		switch(selection) { 
		case -2	: 
		case 0	: return -2;
		default	: 
			sprintf_s(output, "Error: Invalid rental type: '%c'.", selection);
			reset										= true;
			break;
		case 1 + ::bik::RENTAL_TYPE_HOUR	: 
		case 1 + ::bik::RENTAL_TYPE_DAY		: 
		case 1 + ::bik::RENTAL_TYPE_MONTH	: 
			rental.Type									= (::bik::RENTAL_TYPE)(selection - 1); 
			sprintf_s(output, "Selected rental type: %s.", ::bik::g_Prices[rental.Type].Text);
			break;
		}
		printf("\n- %s", output);
		if(reset)
			continue;
	}

	const char										* textTypeSelected						= ::bik::g_Prices[rental.Type].Text;
	int32_t												hours									= -1;
	while(hours < 0) {
		printf("\n\nType rental time (in %ss) or \"exit\" (without quotes) to exit:\n", textTypeSelected);
		hours										= (int)::rec::getNumericInput();
		if(hours == -2)
			return -2;
		char											output[512]								= {};
		if(hours <= 0 || hours > 0x7FFF) {
			sprintf_s(output, "Error: Invalid time: '%c'.", hours);
			reset										= true;
		}
		else {
			rental.Time									= (int16_t)hours; 
			sprintf_s(output, "Selected time: %u %s%s.", rental.Time, textTypeSelected, (rental.Time > 1) ? "s" : "");
		}
		printf("\n- %s", output);
		if(reset)
			continue;
	}
	return 0;
}

struct SCart {
	::std::vector<::bik::SRental>				Rentals;
};

		int									main									()									{
	::SCart											cart									= {};
	int												version									= -1;
	while(version < 0) {
		cart.Rentals.clear();
		printf("\n\nSelect implementation type: "
			"\n- 1: %s"
			"\n- 2: %s"
			"\n- 0: %s\n"
			, "Discount by rental type (custom)."
			, "Discount by total rentals (original)."
			, "Exit."
			);
		version										= (int)::rec::getNumericInput();
		switch(version) {
		default: 
			printf("\nEnter a valid option.");
			version										= -1;
			break;
		case 0:
			break;
		case 1:
			while(true) {
				printf("\n----- Current rentals (%u) -----", (uint32_t)cart.Rentals.size());
				::std::vector<::bik::SRentalPrices>				individualPrices						= {};
				double											totalPrice								= ::bik::rentalCalculatePrice({&cart.Rentals[0], (uint32_t)cart.Rentals.size()}, ::bik::g_Prices, individualPrices, true);
				printf("\n- Final price: %2.2f.", totalPrice);
				::bik::SRental									rental									= {};
				int32_t											rentalResult							= (int32_t)::inputRental(rental);
				bool											skip									= rentalResult == -1;
				if(skip)
					continue;
				if(rentalResult == -2) {
					version										= -1;
					break;
				}
				cart.Rentals.push_back(rental);
			}
			break;
		case 2:
			while(true) {
				printf("\n----- Current rentals (%u) -----", (uint32_t)cart.Rentals.size());
				::std::vector<::bik::SRentalPrices>				individualPrices						= {};
				double											totalPrice								= ::bik::rentalCalculatePriceOriginal({&cart.Rentals[0], (uint32_t)cart.Rentals.size()}, ::bik::g_Prices, individualPrices, true);
				printf("\n----- Final price: %2.2f -----", totalPrice);
				::bik::SRental									rental									= {};
				int32_t											rentalResult							= (int32_t)::inputRentalOriginal(rental);
				bool											skip									= rentalResult == -1;
				if(skip)
					continue;
				else if(rentalResult == -2) {
					version										= -1;
					break;
				}
				else
					cart.Rentals.push_back(rental);
			}
			break;
		}
	}
	printf("\nBye bye.");
	return 0;
}