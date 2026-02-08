#include "core/commands/LoopedCommand.hpp"
#include "game/gta/Natives.hpp"

namespace YimMenu::Features
{
	class FastSwim : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		static constexpr float NORMAL_SWIM = 1.0f;
		static constexpr float FAST_SWIM = 1.49f; // GTA cap
		static constexpr float SPEED_BOOST = 1.35f;

		virtual void OnTick() override
		{
			Player player = PLAYER::PLAYER_ID();
			Ped ped = PLAYER::PLAYER_PED_ID();

			if (!PED::IS_PED_SWIMMING(ped))
			{
				PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, NORMAL_SWIM);
				return;
			}

			bool sprintPressed =
			    PAD::IS_CONTROL_PRESSED(0, 21) || PAD::IS_DISABLED_CONTROL_PRESSED(0, 21);

			// Animation speed (GTA clamps > ~1.5)
			PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(
			    player,
			    sprintPressed ? FAST_SWIM : NORMAL_SWIM);

			// Real movement speed
			if (sprintPressed)
			{
				Vector3 vel = ENTITY::GET_ENTITY_VELOCITY(ped);

				ENTITY::SET_ENTITY_VELOCITY(
				    ped,
				    vel.x * SPEED_BOOST,
				    vel.y * SPEED_BOOST,
				    vel.z);
			}
		}

		virtual void OnDisable() override
		{
			PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(
			    PLAYER::PLAYER_ID(),
			    NORMAL_SWIM);
		}
	};

	static FastSwim _FastSwim{"fastswim", "Fast Swim", "Swim significantly faster while holding sprint"};
}
