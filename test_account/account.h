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

		void									Deposit					(int64_t amount)						{ this->Balance += amount; }
		void									Withdraw				(int64_t amount)						{
			if(::acc::ACCOUNT_TYPE_CTA_CORRIENTE == this->Balance && amount > (this->Balance + this->AgreedOverdraft))
				throw ::std::runtime_error("No hay dinero suficiente.");
			else if (amount > this->Balance)
				throw ::std::runtime_error("No hay dinero suficiente.");
			this->Balance					-= amount;
		}
	};
#pragma pack(pop)

	struct SAccount {
		int64_t									AccountNumber			;
		::std::string							Titular					;
	};

	class Bank {
		::std::vector<::acc::SAccount>			Accounts;
		::std::vector<::acc::SAccountBalance>	Balances;

		int32_t									CreateAccount			(ACCOUNT_TYPE type, int64_t nCuenta, const ::std::string& titular, long descAcordado = 0)					{
			if(descAcordado && type != ::acc::ACCOUNT_TYPE_CTA_CORRIENTE)
				throw ::std::runtime_error("El descubierto acordado debe ser igual a cero para cuentas que no sean de tipo CTA_CORRIENTE.");
			Accounts.push_back({nCuenta, titular});
			Balances.push_back(type);
			Balances[Balances.size() - 1].AgreedOverdraft	= descAcordado;
		}
	};
}

#endif // ACCOUNT_H_20190315
