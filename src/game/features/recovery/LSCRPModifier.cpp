#include "core/commands/IntCommand.hpp"
#include "core/commands/Command.hpp"
#include "game/gta/Stats.hpp"
#include <algorithm>
namespace YimMenu::Features
{
	static IntCommand LSCarMeetRankSlider{
	    "lscarmeetrank",
	    "LS Car Meet Rank",
	    "Sets LS Car Meet rank (writes CAR_CLUB_REP).",
	    1,
	    1000,
	    1};

	class ApplyLSCarMeetRank : public Command
	{
		using Command::Command;

		void OnCall() override
		{
			int rank = std::clamp(LSCarMeetRankSlider.GetState(), 1, 1000);
			int rep = rank * 1100;
			Stats::SetInt("MPX_CAR_CLUB_REP", rep);
		}
	};

	static ApplyLSCarMeetRank _ApplyLSCarMeetRank{
	    "applylscarmeetrank",
	    "Apply LS Car Meet Rank",
	    "Applies the selected LS Car Meet rank."};
}
