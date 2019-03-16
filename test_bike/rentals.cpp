#include "rentals.h"

double									bik::rentalCalculatePrice					(::bikl::view_array<::bik::SRental> rentals, const ::bikl::view_array<const ::bik::SPrice>& priceList, ::std::vector<::bik::SRentalPrices>& individualPrices, bool enableDiscount) {
	double										totalPrice								= 0;
	for(uint32_t iRental = 0, countRentals = (uint32_t)rentals.size(); iRental < countRentals; ++iRental) {
		const ::bik::SRental						& rental								= rentals[iRental];
		::bik::SRentalPrices						rentalPrices							= {};
		const char									* textType								= priceList[rental.Type].Text;
		const bool									discountQualified						= (rental.Bikes > 2);	//
		const bool									discountApplied							= enableDiscount && discountQualified;
		rentalPrices.Raw						= priceList[rental.Type].Price * rental.Bikes * rental.Time;
		rentalPrices.Discount					= rentalPrices.Raw * .3;
		rentalPrices.Final						= rentalPrices.Raw - rentalPrices.Discount;
		printf("\n--- Rental #%.2u ---"
			"\nType: %s"
			"\n%ss: %u" 
			"\nBikes: %u" 
			"\nPrice: %2.2f" 
			"\nDiscount: %s", 1 + iRental, textType, textType, rental.Time, rental.Bikes, rentalPrices.Raw, discountApplied ? "Yes" : "No");
		if(discountApplied) {
			printf("\nFamily Package Discount: %2.2f"
				"\nPrice with discount: %2.2f"
				, rentalPrices.Discount, rentalPrices.Final);
		}
		individualPrices.push_back(rentalPrices);
		totalPrice								+= individualPrices[individualPrices.size() - 1].Final;
	}
	return totalPrice;
}

double									bik::rentalCalculatePriceOriginal			(::bikl::view_array<::bik::SRental> rentals, const ::bikl::view_array<const ::bik::SPrice>& priceList, ::std::vector<::bik::SRentalPrices>& individualPrices, bool enableDiscount) {
	double										totalPrice								= 0;
	for(uint32_t iRental = 0, countRentals = (uint32_t)rentals.size(); iRental < countRentals; ++iRental) {
		const ::bik::SRental						& rental								= rentals[iRental];
		const char									* textType								= priceList[rental.Type].Text;
		::bik::SRentalPrices						rentalPrices							= {};
		rentalPrices.Raw						= priceList[rental.Type].Price * rental.Time;
		printf("\n- Rental #%.2u: Type: %s, %ss: %u, Price: %2.2f.", 1 + iRental, textType, textType, rental.Time, rentalPrices.Raw);
		individualPrices.push_back(rentalPrices);
		totalPrice									+= rentalPrices.Raw;
	}
	const bool									discountQualified						= (individualPrices.size() >= 3 && individualPrices.size() <= 5);
	const bool									discountApplied							= enableDiscount && discountQualified;
	if(discountApplied) {
		const double								discountValue							= totalPrice * .3;
		const double								priceWithDiscount						= totalPrice - discountValue;
		printf("\n-- Family Package Discount --"
			"\nRental Price: %2.2f"
			"\nDiscount: %2.2f"
			"\nFinal price: %2.2f\n"
			, totalPrice
			, discountValue
			, priceWithDiscount
		);
		totalPrice								= priceWithDiscount;
	}
	return totalPrice;
}
