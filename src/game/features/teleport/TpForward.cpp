#include "core/commands/Command.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Natives.hpp"

namespace YimMenu::Features
{
	class TpForward : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			auto ped = Self::GetPed();

			// Current position
			Vector3 pos = ped.GetPosition();

			// Heading (yaw) in radians
			float heading = ENTITY::GET_ENTITY_HEADING(ped.GetHandle());
			float headingRad = heading * 0.01745329251f; // DEG2RAD

			// Distance to teleport forward (adjustable)
			float distance = 5.0f;

			// Calculate forward vector
			Vector3 forward;
			forward.x = -sinf(headingRad) * distance;
			forward.y = cosf(headingRad) * distance;
			forward.z = 0.0f;

			// New position
			Vector3 newPos;
			newPos.x = pos.x + forward.x;
			newPos.y = pos.y + forward.y;
			newPos.z = pos.z;

			// Teleport
			ped.TeleportTo(newPos);
		}
	};

	static TpForward _TpForward{"tpforward", "Teleport Forward", "Teleports you a short distance forward"};
}
