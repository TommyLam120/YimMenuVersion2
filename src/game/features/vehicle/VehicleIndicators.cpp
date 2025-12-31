#include "core/commands/LoopedCommand.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Natives.hpp"
#include <windows.h>


namespace YimMenu::Features
{
	class VehicleIndicators : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;
		bool m_Left = false;
		bool m_Right = false;
		bool m_EWasPressed = false;
		bool m_RWasPressed = false;
		bool m_PWasPressed = false;
		using Vehicle = int;
        using Entity = int;
        using Ped = int;

		virtual void OnTick() override
		{
			static Vehicle veh = 0;
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			if (!ENTITY::DOES_ENTITY_EXIST(playerPed))
			{
				veh = 0;
				return;
			}

			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false))
			{
				veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
			}
			else
			{
				veh = 0;
			}

			if (veh == 0)
				return;

	       	// ← LEFT ARROW → toggle left signal
			if (GetAsyncKeyState(VK_LEFT) & 1)
			{
				m_Left = !m_Left;
				m_Right = false;
			}

			// → RIGHT ARROW → toggle right signal
			if (GetAsyncKeyState(VK_RIGHT) & 1)
			{
				m_Right = !m_Right;
				m_Left = false;
			}

			// ↑ UP ARROW → Hazard lights
			if (GetAsyncKeyState(VK_UP) & 1)
			{
				m_Left = true;
				m_Right = true;
			}
			//cancel all
			if (GetAsyncKeyState(VK_DOWN) & 1)
			{
				m_Left = false;
				m_Right = false;
			}
			VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(veh, 0, m_Left ? TRUE : FALSE);
			VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(veh, 1, m_Right ? TRUE : FALSE);
		}
	};
	static VehicleIndicators _VehicleIndicators{
	    "vehicle_indicators",
	    "Vehicle Indicators",
	    "  ← LEFT ARROW | → RIGHT ARROW | ↑ UP ARROW Hazard Lights | Cancel All Arrow Down ↓"};
}
