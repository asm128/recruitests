#include <vector>
#include <string>

#ifndef ACCOUNT_H_20190315
#define ACCOUNT_H_20190315

namespace acc
{
#pragma pack(push, 1)
	enum ACCOUNT_TYPE : int64_t
		{ ACCOUNT_TYPE_CTA_CORRIENTE	
		, ACCOUNT_TYPE_CAJA_AHORRO		
		};

	struct SAccountBalance {
		ACCOUNT_TYPE							Type					= ::acc::ACCOUNT_TYPE_CTA_CORRIENTE;
		int64_t									Balance					= 0;
		int64_t									AgreedOverdraft			= 0;

												SAccountBalance			(ACCOUNT_TYPE type)	: Type(type)		{}

		void									SetAgreedOverDraft		(int64_t agreedOverdraft)				{
			if(agreedOverdraft < 0)
				throw ::std::runtime_error("The overdraft value cannot be negative.");
			else if(agreedOverdraft > 0 && Type != ::acc::ACCOUNT_TYPE_CTA_CORRIENTE)
				throw ::std::runtime_error("The overdraft value must be zero for accounts of types other than \"CTA_CORRIENTE\".");
			AgreedOverdraft							= agreedOverdraft;
		}

		void									Deposit					(uint64_t amount)						{ 
			if(amount > 0x7FFFFFFFFFFFFFFF) 
				throw ::std::runtime_error("Invalid deposit amount: value overflow.");
			const int64_t								resultingValue			= this->Balance + amount;
			if(resultingValue < this->Balance || resultingValue < (int64_t)amount)
				throw ::std::runtime_error("Invalid deposit amount: value overflow.");
			this->Balance							= resultingValue; 
		}

		void									Withdraw				(uint64_t amount)						{
			if(amount > 0x7FFFFFFFFFFFFFFF) 
				throw ::std::runtime_error("Invalid withdraw amount: value overflow.");
			const int64_t								resultingValue			= this->Balance - amount;
			const bool									enoughMoney				= (int64_t)amount <= this->Balance + (::acc::ACCOUNT_TYPE_CTA_CORRIENTE == this->Type) ? this->AgreedOverdraft : 0;
			if(false == enoughMoney)
				throw ::std::runtime_error("No hay dinero suficiente.");
			this->Balance							-= amount;
		}
	};
#pragma pack(pop)

	class Bank {
		::std::vector<uint64_t				>	AccountNumbers	;
		::std::vector<::std::string			>	Holders			;
		::std::vector<::acc::SAccountBalance>	Balances;

		int32_t									CreateAccount			(ACCOUNT_TYPE type, uint64_t nCuenta, const ::std::string& holder, long agreedOverdraft = 0)					{
			if(agreedOverdraft && type != ::acc::ACCOUNT_TYPE_CTA_CORRIENTE)
				throw ::std::runtime_error("El descubierto acordado debe ser igual a cero para cuentas que no sean de tipo CTA_CORRIENTE.");
			const size_t								oldSize					= Balances.size();
			try {
				AccountNumbers	.push_back(nCuenta);
				Holders			.push_back(holder);
				Balances		.push_back(type);
			}
			catch (...) { // Out of memory?
				AccountNumbers	.resize(oldSize);
				Holders			.resize(oldSize);
				return -1;
			}
			const size_t								index					= Balances.size() - 1;
			Balances[index].AgreedOverdraft			= agreedOverdraft;
			return (int32_t)index;
		}
	};
}

#endif // ACCOUNT_H_20190315
