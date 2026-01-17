#include "core/commands/Command.hpp"
#include "game/gta/Stats.hpp"
#include "game/gta/Network.hpp"
#include <game/gta/Natives.hpp>
#include "core/frontend/Notifications.hpp"
#include "core/backend/ScriptMgr.hpp"

namespace YimMenu::Features
{
	class OpenBunkerLaptop : public Command
	{
		using Command::Command;

		void OnCall() override
		{
			const Hash script = Joaat("appBunkerBusiness");
            if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(script) > 0)
            {
                Notifications::Show("YimMenuV2", "Bunker app already open", NotificationType::Info);
                return;
            }
            SCRIPT::REQUEST_SCRIPT_WITH_NAME_HASH(script);
            while (!SCRIPT::HAS_SCRIPT_WITH_NAME_HASH_LOADED(script))
                ScriptMgr::Yield();
            BUILTIN::START_NEW_SCRIPT_WITH_NAME_HASH(script, 1424);
            SCRIPT::SET_SCRIPT_WITH_NAME_HASH_AS_NO_LONGER_NEEDED(script);
        
		}
	};

	static OpenBunkerLaptop _OpenBunkerLaptop{"bunkerlaptop", "Open Bunker Laptop", "Open Laptop Bunker the see what need be Unlocked etc."};
}
