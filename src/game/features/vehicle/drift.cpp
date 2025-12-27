#include "core/commands/LoopedCommand.hpp"
#include "core/commands/ListCommand.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Natives.hpp"
#include "types/pad/ControllerInputs.hpp"

namespace YimMenu::Features
{
	static ListCommand DriftVariant{
	    "driftvariant",
	    "Drift Variant",
	    "Select drift behavior",
	    {{0, "Soft"},
	    {1, "Balanced"},
	    {2, "Aggressive"},
	    {3, "Grip Only"}},
		 1 
	};
	class VehicleDrift : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			auto veh = Self::GetVehicle();
			if (!veh)
				return;

			const bool driftHeld = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0 || PAD::IS_CONTROL_PRESSED(2, 21);
			if (!driftHeld)
			{
				VEHICLE::SET_VEHICLE_REDUCE_GRIP(veh.GetHandle(), false);
				return;
			}
			VEHICLE::SET_VEHICLE_REDUCE_GRIP(veh.GetHandle(), true);
			const float speed = ENTITY::GET_ENTITY_SPEED(veh.GetHandle());
			if (speed < 5.0f)
				return;
			float force = 0.0f;
			float maxForce = 0.0f;
			switch (DriftVariant.GetState())
			{
			case 0: // Soft
				force = speed * 0.0025f;
				maxForce = 0.06f;
				break;

			case 1: // Balanced
				force = speed * 0.0040f;
				maxForce = 0.12f;
				break;

			case 2: // Aggressive
				force = speed * 0.0060f;
				maxForce = 0.18f;
				break;

			case 3: // Grip Only
			default:
				return;
			}
			if (force > maxForce)
				force = maxForce;
			ENTITY::APPLY_FORCE_TO_ENTITY(veh.GetHandle(), 1, 0.0f, force, 0.0f, 0.0f, 0.0f, 0.0f, 0, true, true, true, true, false);
		}

		virtual void OnDisable() override
		{
			auto veh = Self::GetVehicle();
			if (veh)
				VEHICLE::SET_VEHICLE_REDUCE_GRIP(veh.GetHandle(), false);
		}
	};

	static VehicleDrift _VehicleDrift{
	    "vehicledrift",
	    "Drift Mode",
	    "Hold SHIFT to drift"};
}

