#include "core/commands/Command.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Natives.hpp"

namespace YimMenu::Features
{
	class TpLeft : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			auto ped = Self::GetPed();
			Vector3 pos = ped.GetPosition();
			float heading = ENTITY::GET_ENTITY_HEADING(ped.GetHandle());
			float headingRad = heading * 0.01745329251f;
			float distance = 5.0f;
			Vector3 left;
			left.x = -cosf(headingRad) * distance;
			left.y = -sinf(headingRad) * distance;
			left.z = 0.0f;
			Vector3 newPos;
			newPos.x = pos.x + left.x;
			newPos.y = pos.y + left.y;
			newPos.z = pos.z;
			ped.TeleportTo(newPos);
		}
	};

	static TpLeft _TpLeft{"tpleft", "Teleport Left", "Teleports you left"};
}
