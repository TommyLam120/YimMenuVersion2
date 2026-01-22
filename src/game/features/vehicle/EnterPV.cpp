#include "core/commands/Command.hpp"
#include "core/backend/FiberPool.hpp"
#include "game/backend/PersonalVehicles.hpp"
#include "game/backend/Self.hpp"
#include "core/frontend/Notifications.hpp"

namespace YimMenu::Features
{
	class EnterPV : Command
	{
	  using Command::Command;

	  virtual void OnCall() override
	  {
	    FiberPool::Push([] {
		if (auto veh = PersonalVehicles::GetCurrentHandle(); veh.IsValid())
		{
			Self::GetPed().SetInVehicle(veh);
		}
		else
		{
			Notifications::Show("YimMenuV2","No personal vehicle found",NotificationType::Error);
		}
	  });
    }
};
	static EnterPV _EnterPV{"enterpvvehicle", "Enter PV Vehicle", "Enter Your PV Vehicle"};
};
