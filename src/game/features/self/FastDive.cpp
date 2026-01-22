#include "core/commands/LoopedCommand.hpp"
#include "game/gta/Natives.hpp"

namespace YimMenu::Features
{
	class FastDive : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		// Config values (easy to tweak)
		static constexpr float NORMAL_SWIM = 1.0f;
		static constexpr float FAST_SWIM = 2.5f; // 2.0f – 2.8f ? smooth and >3.0f ? animation glitches
		static constexpr float FAST_DIVE = -3.0f; // Downward force | -2.0f ? realistic dive and -4.0f ? dolphin-style dive ??

		virtual void OnTick() override
		{
			Ped ped = PLAYER::PLAYER_PED_ID();
			Player player = PLAYER::PLAYER_ID();

			if (!PED::IS_PED_SWIMMING(ped))
			{
				PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, NORMAL_SWIM);
				return;
			}

			// Sprint = Fast Swim
			if (PAD::IS_CONTROL_PRESSED(0, 21)) // Sprint (SHIFT / A)
			{
				PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, FAST_SWIM);
			}
			else
			{
				PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, NORMAL_SWIM);
			}

			// Dive faster when looking down / underwater
			if (PED::IS_PED_SWIMMING_UNDER_WATER(ped) && PAD::IS_CONTROL_PRESSED(0, 32)) // Move Forward (W / LS Up)
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(
				    ped,
				    1,
				    0.0f,
				    0.0f,
				    FAST_DIVE,
				    0.0f,
				    0.0f,
				    0.0f,
				    0,
				    false,
				    true,
				    true,
				    false,
				    true);
			}
		}

		virtual void OnDisable() override
		{
			PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(
			    PLAYER::PLAYER_ID(),
			    NORMAL_SWIM);
		}
	};

	static FastDive _FastDive{"fastdive", "Fast Dive", "Swim faster with sprint and dive quicker underwater"};
}
