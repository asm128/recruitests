#include <cstdint>
#include <cstdio>

#ifndef RENTALIB_H_20190314
#define RENTALIB_H_20190314

namespace bikl	// what's found in this namespace could be moved to some .h as it's useful everywhere.
{
// This macro simplifies throwing generic exceptions with custom messages.
#define throw_if(condition, format, ...) do { 	\
	if(condition) {								\
		static	char		buf[512]	= {};	\
		sprintf_s(buf, format, __VA_ARGS__);	\
		throw(::std::exception(buf));			\
	}											\
} while(0)

	// This class is used to hold a pointer to an array and the element count in order to check for invalid array accesses when using the operator[].
	template <typename _tElement>
	class view_array {
	protected:
		// Properties / Member Variables
							_tElement				* Data						= 0;
							uint32_t				Count						= 0;
	public:
		typedef				_tElement				TElement;

		// Constructors
		inline constexpr							view_array					()																			noexcept	= default;
		inline										view_array					(_tElement* dataElements, uint32_t elementCount)										: Data(dataElements), Count(elementCount)										{
			throw_if(0 == dataElements && 0 != elementCount, "Invalid parameters: %p, %u.", dataElements, elementCount);	// Crash if we received invalid parameters in order to prevent further malfunctioning.
		}

		template <size_t _elementCount>
		inline constexpr							view_array					(_tElement (&_dataElements)[_elementCount])									noexcept	: Data(_dataElements), Count(_elementCount)										{}

		template <size_t _elementCount>
		inline										view_array					(_tElement (&_dataElements)[_elementCount], uint32_t elementCount)						: Data(_dataElements), Count(::gpk::min((uint32_t)_elementCount, elementCount))			{
			throw_if(elementCount > _elementCount, "Element count out of range. Max count: %u. Requested: %u.", (uint32_t)_elementCount, elementCount);
		}

		// Operators
		inline constexpr	operator				view_array<const _tElement>	()																			noexcept	{ return {Data, Count}; }
							_tElement&				operator[]					(uint32_t index)																		{ throw_if(0 == Data, "Uninitialized array pointer. Count: %u.", Count); throw_if(index >= Count, "Invalid index: %u.", index); return Data[index]; }
							const _tElement&		operator[]					(uint32_t index)													const				{ throw_if(0 == Data, "Uninitialized array pointer. Count: %u.", Count); throw_if(index >= Count, "Invalid index: %u.", index); return Data[index]; }

		// Methods
		inline				_tElement*				begin						()																			noexcept	{ return Data;			}
		inline				_tElement*				end							()																			noexcept	{ return Data + Count;	}

		inline constexpr	const _tElement*		begin						()																	const	noexcept	{ return Data;			}
		inline constexpr	const _tElement*		end							()																	const	noexcept	{ return Data + Count;	}

		inline constexpr	const uint32_t&			size						()																	const	noexcept	{ return Count;			}
	};
}

#endif // RENTALIB_H_20190314
