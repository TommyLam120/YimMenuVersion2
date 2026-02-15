#include "core/commands/Command.hpp"
#include "game/gta/ScriptGlobal.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/pointers/Pointers.hpp"

namespace YimMenu::Features
{
	class Dev_Tshirts : public Command
	{
		using Command::Command;

		void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				*ScriptGlobal(262145).At(12584).As<bool*>() = true;//Vinewood Zombie
				*ScriptGlobal(262145).At(12591).As<bool*>() = true;//Cannibal Clown
				*ScriptGlobal(262145).At(12592).As<bool*>() = true;//Hot Serial Killer Stepmom
				*ScriptGlobal(262145).At(12594).As<bool*>() = true;//Meathook For Mommy
			}
			else
			{
				Notifications::Show("YimMenuV2","You must be online",NotificationType::Error);
			}
		}
	};

	static Dev_Tshirts _Dev_Tshirts{
	    "Rockstar_dev_tshirt",
	    "Rockstar Developer Tshirts",
	    "Rockstar Developer Tshirts"};
}
