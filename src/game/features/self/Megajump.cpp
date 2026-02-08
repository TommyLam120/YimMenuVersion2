#include "core/commands/LoopedCommand.hpp"
#include "game/gta/Natives.hpp"
#include "core/util/utils.hpp"
namespace YimMenu::Features
{
	class MegaJump : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;
		
		virtual void OnTick() override
		{
		    static bool was_active;
			static int is_timer;
			bool is_active = PED::IS_PED_JUMPING(PLAYER::PLAYER_PED_ID());
			if (is_active && was_active) {
				was_active = true;
				if (is_timer > 0) {
					is_timer--;
					PED::SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), false);
					WEAPON::REMOVE_WEAPON_FROM_PED(PLAYER::PLAYER_PED_ID(), MISC::GET_HASH_KEY("GADGET_PARACHUTE"));
					Utils::ApplyForceToEntity(PLAYER::PLAYER_PED_ID(), 0, 2.5, 20,true,true,true);
				}
			} else if (is_active && !was_active) {
				was_active = true;
				is_timer = 10;
			} else if (!is_active) {
				was_active = false;
			}
		}
	};
	static MegaJump _MegaJump{"megajump", "Mega Jump", "Jump Super High"};
}
