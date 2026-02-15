#include "CayoPericoHeist.hpp"
#include "game/gta/Natives.hpp"
#include "core/util/utils.hpp"

namespace YimMenu::Submenus
{
	static int g_SelectedTeleport = 0;
	static const char* g_TeleportItems[] = {
		//Just a test
	    "Kosatka",
	    "Drainage Pipe",
	    "Drainage Pipe Checkpoint",
	};


	std::shared_ptr<TabItem> RenderCayoPericoHeistMenu()
	{
		auto tab = std::make_shared<TabItem>("Cayo Perico Heist");

		auto cuts = std::make_shared<Group>("Heist Cuts", 2);
		auto setups = std::make_shared<Group>("Heist Setups");
		auto loots = std::make_shared<Group>("Loots", 2);
		auto misc = std::make_shared<Group>("Misc", 1);
		auto cayo_tp = std::make_shared<Group>("Teleport");


		cuts->AddItem(std::make_shared<IntCommandItem>("cayopericoheistcut1"_J));
		cuts->AddItem(std::make_shared<IntCommandItem>("cayopericoheistcut3"_J));
		cuts->AddItem(std::make_shared<IntCommandItem>("cayopericoheistcut2"_J));
		cuts->AddItem(std::make_shared<IntCommandItem>("cayopericoheistcut4"_J));
		cuts->AddItem(std::make_shared<CommandItem>("cayopericoheistforceready"_J));
		cuts->AddItem(std::make_shared<CommandItem>("cayopericoheistsetcuts"_J));

		setups->AddItem(std::make_shared<ListCommandItem>("cayopericoheistdifficulty"_J));
		setups->AddItem(std::make_shared<ListCommandItem>("cayopericoheistprimarytarget"_J));
		setups->AddItem(std::make_shared<ListCommandItem>("cayopericoheistweapon"_J));
		setups->AddItem(std::make_shared<CommandItem>("cayopericoheistsetup"_J));

		loots->AddItem(std::make_shared<IntCommandItem>("cayopericoheistprimarytargetvalue"_J));
		loots->AddItem(std::make_shared<IntCommandItem>("cayopericoheistsecondarytakevalue"_J));
		loots->AddItem(std::make_shared<CommandItem>("cayopericoheistsetprimarytargetvalue"_J, "Set##primarytargetvalue"));
		loots->AddItem(std::make_shared<CommandItem>("cayopericoheistsetsecondarytakevalue"_J, "Set##secondarytakevalue"));

		misc->AddItem(std::make_shared<CommandItem>("cayopericoheistskiphacking"_J));
		misc->AddItem(std::make_shared<CommandItem>("cayopericoheistcutsewer"_J));
		misc->AddItem(std::make_shared<CommandItem>("cayopericoheistcutglass"_J));
		misc->AddItem(std::make_shared<CommandItem>("cayopericoheisttakeprimarytarget"_J));
		misc->AddItem(std::make_shared<CommandItem>("cayopericoheistinstantfinish"_J));
		
		cayo_tp->AddItem(std::make_unique<ImGuiItem>([] {
			ImGui::SetNextItemWidth(140.f);
			ImGui::Combo("Teleport To", &g_SelectedTeleport, g_TeleportItems, IM_ARRAYSIZE(g_TeleportItems));
			if (ImGui::Button("Teleport", ImVec2(150, 30))) {
				 Vector3 pos{};
				 switch (g_SelectedTeleport) {
					  case 0: // Kosatka
						  pos = { 1561.2369, 385.8831, -49.689915 };
					      Utils::SetHeading(175);
					  break;
					  case 1: // Drainage Pipe
						   pos = { 5044.001, -5815.6426, -11.808871 };
					  break;
					  case 2: // Drainage Pipe Checkpoint
						   pos = { 5053.773, -5773.2266, -5.40778 };
					  break;

					}
					   Ped ped = PLAYER::PLAYER_PED_ID();
					    if (PED::IS_PED_IN_ANY_VEHICLE(ped, false)) {
							Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
							ENTITY::SET_ENTITY_COORDS(veh, pos.x, pos.y, pos.z, false, false, false, true);
							}
						else {
							 ENTITY::SET_ENTITY_COORDS(ped, pos.x, pos.y, pos.z, false, false, false, true);
						}
					}
			}));
		

		tab->AddItem(cuts);
		tab->AddItem(setups);
		tab->AddItem(loots);
		tab->AddItem(misc);
		tab->AddItem(cayo_tp);
		return tab;
	}
}
