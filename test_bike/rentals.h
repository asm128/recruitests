#include "rentalib.h"

#include <vector>

#ifndef RENTALS_H_20190314
#define RENTALS_H_20190314

namespace bik
{
	enum RENTAL_TYPE : int32_t
		{ RENTAL_TYPE_HOUR		= 0
		, RENTAL_TYPE_DAY	
		, RENTAL_TYPE_MONTH	
		, RENTAL_TYPE_COUNT
		};

#pragma pack(push, 1)
	struct SPrice {
		RENTAL_TYPE									Type;
		int32_t										Price;
		const char									* Text;
	};

	struct SRentalPrices {
		double										Raw;
		double										Discount;
		double										Final;
	};

	struct SRental {
		RENTAL_TYPE									Type;
		int16_t										Time;
		int16_t										Bikes;
	};
#pragma pack(pop)

	double										rentalCalculatePrice				(::bikl::view_array<::bik::SRental> rentals, const ::bikl::view_array<const ::bik::SPrice>& priceList, ::std::vector<::bik::SRentalPrices>& individualPrices, bool enableDiscount);
	double										rentalCalculatePriceOriginal		(::bikl::view_array<::bik::SRental> rentals, const ::bikl::view_array<const ::bik::SPrice>& priceList, ::std::vector<::bik::SRentalPrices>& individualPrices, bool enableDiscount);

	static constexpr	const ::bik::SPrice		g_Prices[RENTAL_TYPE_COUNT]			= 
		{ { RENTAL_TYPE_HOUR	,  5, "Hour"	}
		, { RENTAL_TYPE_DAY		, 20, "Day"		}
		, { RENTAL_TYPE_MONTH	, 60, "Month"	}
		};
} // namespace


#endif // RENTALS_H_20190314
