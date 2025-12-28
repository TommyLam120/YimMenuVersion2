#include "core/commands/Command.hpp"
#include "game/gta/Natives.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/gta/Stats.hpp"

namespace YimMenu::Features
{
	class Unlockpaints : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{
				Stats::SetInt("MPPLY_XMASLIVERIES0", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES1", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES2", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES3", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES4", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES5", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES6", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES7", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES8", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES9", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES10", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES11", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES12", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES13", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES14", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES15", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES16", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES17", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES18", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES19", -1);
				Stats::SetInt("MPPLY_XMASLIVERIES20", -1);
				Stats::SetInt("MPPLY_XMAS22CPAINT0", -1);
				Stats::SetInt("MPPLY_XMAS22CPAINT1", -1);
				Stats::SetInt("MPPLY_SUM23WHEELCPAINT0", -1);
				Stats::SetInt("MPPLY_SUM23WHEELCPAINT1", -1);
				STATS::STAT_SAVE(0, 0, 3, 0);
			}
		}
	};
	static Unlockpaints _Unlockpaints{
	    "unlock_paints",
	    "Unlock Paints",
	    "Unlock Paints"};

}
