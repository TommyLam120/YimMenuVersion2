#include "core/commands/LoopedCommand.hpp"
#include "game/gta/ScriptGlobal.hpp"
namespace YimMenu::Features
{
	class MansionBusinessBoost : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;
		virtual void OnTick() override
		{
			*ScriptGlobal(262145).At(37613).As<int*>() = 86400;
			*ScriptGlobal(262145).At(37614).As<float*>() = 3.0f; // Meth
			*ScriptGlobal(262145).At(37615).As<float*>() = 3.0f; // Weed
			*ScriptGlobal(262145).At(37616).As<float*>() = 3.0f; // Cocaine
			*ScriptGlobal(262145).At(37617).As<float*>() = 3.0f; // Cash
			*ScriptGlobal(262145).At(37618).As<float*>() = 3.0f; // Docs
			*ScriptGlobal(262145).At(37619).As<float*>() = 3.0f; // Bunker
			*ScriptGlobal(262145).At(37620).As<float*>() = 3.0f; // Acid Lab
		}
		virtual void OnDisable() override
		{
			*ScriptGlobal(262145).At(37613).As<int*>() = 86400;
			*ScriptGlobal(262145).At(37614).As<float*>() = 1.0f;
			*ScriptGlobal(262145).At(37615).As<float*>() = 1.0f;
			*ScriptGlobal(262145).At(37616).As<float*>() = 1.0f;
			*ScriptGlobal(262145).At(37617).As<float*>() = 1.0f;
			*ScriptGlobal(262145).At(37618).As<float*>() = 1.0f;
			*ScriptGlobal(262145).At(37619).As<float*>() = 1.0f;
			*ScriptGlobal(262145).At(37620).As<float*>() = 1.0f;
		}
	};
	static MansionBusinessBoost _MansionBusinessBoost{
	    "mansion_business_boost",
	    "Mansion Business Boost",
	    "Boosts all business production"};
}
