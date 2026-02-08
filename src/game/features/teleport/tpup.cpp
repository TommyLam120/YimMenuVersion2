#include "core/commands/Command.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Natives.hpp"

namespace YimMenu::Features
{
	class TpUp : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			auto ped = Self::GetPed();
			Vector3 pos = ped.GetPosition();
			float distance = 5.0f;
			Vector3 newPos;
			newPos.x = pos.x;
			newPos.y = pos.y;
			newPos.z = pos.z + distance;
			ped.TeleportTo(newPos);
		}
	};

	static TpUp _TpUp{"tpup", "Teleport Up", "Teleports you upward"};
}
