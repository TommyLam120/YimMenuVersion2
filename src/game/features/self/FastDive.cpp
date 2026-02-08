#include "core/commands/LoopedCommand.hpp"
#include "game/gta/Natives.hpp"

namespace YimMenu::Features
{
	class FastDive : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		// Swim multipliers
		static constexpr float NORMAL_SWIM = 1.0f;
		static constexpr float FAST_SWIM   = 3.0f;

		// Vertical swim speeds
		static constexpr float DIVE_SPEED  = -6.0f; // Down
		static constexpr float RISE_SPEED  =  4.0f; // Up

		virtual void OnTick() override
		{
			Player player = PLAYER::PLAYER_ID();
			Ped ped = PLAYER::PLAYER_PED_ID();

			// Safety checks
			if (!ENTITY::DOES_ENTITY_EXIST(ped) ||
			    PED::IS_PED_IN_ANY_VEHICLE(ped, false) ||
			    PED::IS_PED_RAGDOLL(ped))
			{
				return;
			}

			// Not swimming → reset multiplier
			if (!PED::IS_PED_SWIMMING(ped))
			{
				PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, NORMAL_SWIM);
				return;
			}

			// Sprint (Shift)
			bool sprintPressed =
			    PAD::IS_CONTROL_PRESSED(0, 21) ||
			    PAD::IS_DISABLED_CONTROL_PRESSED(0, 21);

			PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(
			    player,
			    sprintPressed ? FAST_SWIM : NORMAL_SWIM);

			// Only dive/rise underwater
			if (!PED::IS_PED_SWIMMING_UNDER_WATER(ped))
				return;

			// Controls
			bool forwardPressed =
			    PAD::IS_CONTROL_PRESSED(0, 32) ||
			    PAD::IS_DISABLED_CONTROL_PRESSED(0, 32); // W

			bool divePressed =
			    PAD::IS_CONTROL_PRESSED(0, 36) ||
			    PAD::IS_DISABLED_CONTROL_PRESSED(0, 36); // CTRL

			bool risePressed =
			    PAD::IS_CONTROL_PRESSED(0, 22) ||
			    PAD::IS_DISABLED_CONTROL_PRESSED(0, 22); // SPACE

			if (!forwardPressed)
				return;

			Vector3 velocity = ENTITY::GET_ENTITY_VELOCITY(ped);

			// Dive down
			if (divePressed)
			{
				velocity.z = DIVE_SPEED;
			}
			// Rise up
			else if (risePressed)
			{
				velocity.z = RISE_SPEED;
			}

			ENTITY::SET_ENTITY_VELOCITY(
			    ped,
			    velocity.x,
			    velocity.y,
			    velocity.z);
		}

		virtual void OnDisable() override
		{
			// Reset swim speed on disable
			PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(
			    PLAYER::PLAYER_ID(),
			    NORMAL_SWIM);
		}
	};

	static FastDive _FastDive{
	    "fastdive",
	    "Fast Dive",
	    "Sprint = faster swimming | CTRL = dive | SPACE = rise"
	};
}
