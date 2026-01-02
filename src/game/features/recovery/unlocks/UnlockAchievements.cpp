#include "core/commands/Command.hpp"
#include "game/gta/Natives.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/gta/ScriptGlobal.hpp"
#include "core/backend/ScriptMgr.hpp"

namespace YimMenu::Features
{
	enum class AchievementId : int
	{
		WelcomeToLosSantos = 1,
		AFriendshipResurrected = 2,
		AFairDaysPay = 3,
		TheMomentOfTruth = 4,
		ToLiveOrDieInLosSantos = 5,
		DiamondHard = 6,
		Subversive = 7,
		Blitzed = 8,
		SmallTownBigJob = 9,
		TheGovernmentGimps = 10,
		TheBigOne = 11,
		SolidGoldBaby = 12,
		CareerCriminal = 13,
		SanAndreasSightseer = 14,
		AllsFareInLoveAndWar = 15,
		TPIndustriesArmsRace = 16,
		MultiDisciplined = 17,
		FromBeyondTheStars = 18,
		AMysterySolved = 19,
		WasteManagement = 20,
		RedMist = 21,
		ShowOff = 22,
		Kifflom = 23,
		ThreeManArmy = 24,
		OutOfYourDepth = 25,
		AltruistAcolyte = 26,
		ALotOfCheddar = 27,
		TradingPureAlpha = 28,
		PimpMySidearm = 29,
		WantedAliveOrAlive = 30,
		LosSantosCustoms = 31,
		CloseShave = 32,
		OffThePlane = 33,
		ThreeBitGangster = 34,
		MakingMoves = 35,
		AboveTheLaw = 36,
		NumeroUno = 37,
		TheMidnightClub = 38,
		UnnaturalSelection = 39,
		BackseatDriver = 40,
		RunLikeTheWind = 41,
		CleanSweep = 42,
		Decorated = 43,
		StickUpKid = 44,
		EnjoyYourStay = 45,
		CrewCut = 46,
		FullRefund = 47,
		DiallingDigits = 48,
		AmericanDream = 49,
		ANewPerspective = 50,
		BePrepared = 51,
		InTheNameOfScience = 52,
		DeadPresidents = 53,
		ParoleDay = 54,
		ShotCaller = 55,
		FourWay = 56,
		LiveALittle = 57,
		CantTouchThis = 58,
		Mastermind = 59,
		VinewoodVisionary = 60,
		Majestic = 61,
		HumansOfLosSantos = 62,
		FirstTimeDirector = 63,
		AnimalLover = 64,
		EnsemblePiece = 65,
		CultMovie = 66,
		LocationScout = 67,
		MethodActor = 68,
		Cryptozoologist = 69,
		GettingStarted = 70,
		TheDataBreaches = 71,
		TheBogdanProblem = 72,
		TheDoomsdayScenario = 73,
		AWorldWorthSaving = 74,
		OrbitalObliteration = 75,
		Elitist = 76,
		Masterminds = 77
	};
	inline void UnlockAchievement(AchievementId id)
	{
		auto base = ScriptGlobal(4525144);
		*base.At(1).As<int*>() = static_cast<int>(id);
		*base.At(2).As<int*>() = 1;
	}
	class Unlockachievements : public Command
	{
		using Command::Command;
		virtual void OnCall() override
		{
			if (*Pointers.IsSessionStarted)
			{

				for (int id = 1; id <= 77; ++id)
				{
					auto base = ScriptGlobal(4525144);
					*base.At(1).As<int*>() = id;
					*base.At(2).As<int*>() = 1;

					ScriptMgr::Yield(150ms);
				}
				STATS::STAT_SAVE(0, 0, 3, 0);
			}
			else
			{
				Notifications::Show("YimMenuV2", "You Must be Online", NotificationType::Error);
			}
		}
	};
	static Unlockachievements _Unlockachievements{
	    "unlock_achievements",
	    "unlock Achievements",
	    "unlock All Achievements"};

}

