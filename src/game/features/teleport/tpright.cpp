#include "core/commands/Command.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Natives.hpp"

namespace YimMenu::Features
{
	class TpRight : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			auto ped = Self::GetPed();
			Vector3 pos = ped.GetPosition();
			float heading = ENTITY::GET_ENTITY_HEADING(ped.GetHandle());
			float headingRad = heading * 0.01745329251f;
			float distance = 5.0f;
			Vector3 right;
			right.x = cosf(headingRad) * distance;
			right.y = sinf(headingRad) * distance;
			right.z = 0.0f;
			Vector3 newPos;
			newPos.x = pos.x + right.x;
			newPos.y = pos.y + right.y;
			newPos.z = pos.z;
			ped.TeleportTo(newPos);
		}
	};

	static TpRight _TpRight{"tpright", "Teleport Right", "Teleports you right"};
}
