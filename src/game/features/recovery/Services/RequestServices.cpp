#include "core/commands/Command.hpp"
#include "game/gta/ScriptGlobal.hpp"
#include "core/backend/ScriptMgr.hpp"

namespace YimMenu::Features
{
	static constexpr int SERVICE_GLOBAL_BASE = 2733138;
	inline void RequestService(int index)
	{
		*ScriptGlobal(SERVICE_GLOBAL_BASE).At(index).As<int*>() = 1;
	}
	class RequestMOC : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			RequestService(577);
		}
	};

	class RequestAvenger : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			RequestService(585);
		}
	};

	class RequestTerrorbyte : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			RequestService(591);
		}
	};

	class RequestKosatka : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			RequestService(613);
		}
	};

	class RequestAcidLab : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			RequestService(592);
		}
	};

	class RequestAcidBike : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			RequestService(648);
		}
	};

	class RequestAmmoDrop : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			RequestService(538);
		}
	};

	class RequestBullShark : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			RequestService(546);
		}
	};

	class RequestAirstrike : public Command
	{
		using Command::Command;
		void OnCall() override
		{
			RequestService(3580);
		}
	};


	static RequestMOC _reqMoc{
		"requestmoc",
		"Request MOC",
		"Requests the Mobile Operations Center"};

	static RequestAvenger _reqAvenger{
		"requestavenger",
		"Request Avenger",
		"Requests the Avenger aircraft"};

	static RequestTerrorbyte _reqTb{
		"requestterrorbyte",
		"Request Terrorbyte",
		"Requests the Terrorbyte"};

	static RequestKosatka _reqKosatka{
		"requestkosatka",
		"Request Kosatka",
		"Requests the Kosatka submarine"};

	static RequestAcidLab _reqAcid{
		"requestacidlab",
		"Request Acid Lab",
		"Requests the Acid Lab"};

	static RequestAcidBike _reqBike{
		"requestacidbike",
		"Request Acid Lab Bike",
		"Requests the Acid Lab delivery bike"};

	static RequestAmmoDrop _reqAmmo{
		"requestammodrop",
		"Ammo Drop",
		"Requests an ammo drop"};

	static RequestBullShark _reqBull{
		"requestbullshark",
		"Bull Shark",
		"Requests Bull Shark Testosterone"};

	static RequestAirstrike _reqStrike{
		"requestairstrike",
		"Airstrike",
		"Requests an airstrike"};
}
