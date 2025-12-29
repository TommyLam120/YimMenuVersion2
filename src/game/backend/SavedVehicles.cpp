#include "SavedVehicles.hpp"
#include "core/util/Joaat.hpp"
#include "core/backend/ScriptMgr.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/gta/VehicleModel.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Natives.hpp"
#include "game/gta/Vehicle.hpp"
#include "game/gta/data/VehicleValues.hpp"

namespace YimMenu
{
	Folder SavedVehicles::CheckFolder(std::string folderName)
	{
		return FileMgr::GetProjectFolder("./saved_json_vehicles/" + folderName);
	}
	Folder SavedVehicles::CheckIniFolder(std::string folderName)
	{
		return FileMgr::GetProjectFolder("./saved_ini_vehicles/" + folderName);
	}
    void SavedVehicles::RefreshList(std::string folderName,std::vector<std::string>& folders,std::vector<std::string>& files)
	{
		folders.clear();
		const auto jsonRoot = CheckFolder();
		if (std::filesystem::exists(jsonRoot.Path()))
		{
			for (const auto& entry : std::filesystem::directory_iterator(jsonRoot.Path()))
			{
				if (entry.is_directory())
					folders.push_back(entry.path().filename().generic_string());
			}
		}
		files.clear();
		auto scanFiles = [&](const Folder& folder) {
			if (!std::filesystem::exists(folder.Path()))
				return;
			for (const auto& entry : std::filesystem::directory_iterator(folder.Path()))
			{
				const auto ext = entry.path().extension();
				if (ext == ".json" || ext == ".ini")
					files.push_back(entry.path().filename().generic_string());
			}
		};
		scanFiles(CheckFolder(folderName));    // JSON
		scanFiles(CheckIniFolder(folderName)); // INI
	}
	nlohmann::json SavedVehicles::GetJson(Vehicle veh)
	{
		nlohmann::json vehicle_json;
		int primary_color, secondary_color;
		int pearlescent_color, wheel_color;
		auto vehicle = veh.GetHandle();
		Hash vehicle_hash = veh.GetModel();
		std::map<int, int> vehicle_extras;
		auto is_bennys = VehicleModel::IsBennys(vehicle);
		for (int slot = (int)VehicleModType::MOD_SPOILERS; slot <= (int)VehicleModType::MOD_LIGHTBAR; slot++)
			if (VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, slot) > 0)
			{
				auto wheel_custom = VEHICLE::GET_VEHICLE_MOD_VARIATION(vehicle, slot);

				if (wheel_custom && is_bennys)
					wheel_custom = 0;

				int vehicle_mod[2] = {VEHICLE::GET_VEHICLE_MOD(vehicle, slot), wheel_custom};
				vehicle_json[mod_names[slot]] = vehicle_mod;
			}
			else if (VEHICLE::IS_TOGGLE_MOD_ON(vehicle, slot))
			{
				vehicle_json[mod_names[slot]] = "TOGGLE";

				if (slot == (int)VehicleModType::MOD_TYRE_SMOKE)
				{
					int tire_smoke_color[3]{};
					VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(vehicle, &tire_smoke_color[0], &tire_smoke_color[1], &tire_smoke_color[2]);
					vehicle_json[tire_smoke_color_key] = tire_smoke_color;
				}
				else if (slot == (int)VehicleModType::MOD_XENON_LIGHTS)
					vehicle_json[headlight_color_key] = VEHICLE::GET_VEHICLE_XENON_LIGHT_COLOR_INDEX(vehicle);
			}

		for (int extra = (int)CustomVehicleModType::MOD_EXTRA_14; extra <= (int)CustomVehicleModType::MOD_EXTRA_1; ++extra)
			if (auto id = (extra - (int)CustomVehicleModType::MOD_EXTRA_1) * -1; VEHICLE::DOES_EXTRA_EXIST(vehicle, id))
				vehicle_extras[id] = VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(vehicle, id);

		VEHICLE::GET_VEHICLE_COLOURS(vehicle, &primary_color, &secondary_color);
		VEHICLE::GET_VEHICLE_EXTRA_COLOURS(vehicle, &pearlescent_color, &wheel_color);

		vehicle_json[vehicle_model_hash_key] = vehicle_hash;
		vehicle_json[primary_color_key] = primary_color;
		vehicle_json[secondary_color_key] = secondary_color;

		if (VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(vehicle))
		{
			int custom_primary_color[3]{};
			VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle, &custom_primary_color[0], &custom_primary_color[1], &custom_primary_color[2]);
			vehicle_json[custom_primary_color_key] = custom_primary_color;
		}

		if (VEHICLE::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(vehicle))
		{
			int custom_secondary_color[3]{};
			VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle, &custom_secondary_color[0], &custom_secondary_color[1], &custom_secondary_color[2]);
			vehicle_json[custom_secondary_color_key] = custom_secondary_color;
		}

		vehicle_json[vehicle_window_tint_key] = VEHICLE::GET_VEHICLE_WINDOW_TINT(vehicle);
		vehicle_json[pearlescent_color_key] = pearlescent_color;
		vehicle_json[wheel_color_key] = wheel_color;
		vehicle_json[tire_can_burst] = VEHICLE::GET_VEHICLE_TYRES_CAN_BURST(vehicle);
		vehicle_json[vehicle_extras_key] = vehicle_extras;
		vehicle_json[wheel_type_key] = VEHICLE::GET_VEHICLE_WHEEL_TYPE(vehicle);

		if ((VEHICLE::GET_VEHICLE_LIVERY_COUNT(vehicle) > 1) && VEHICLE::GET_VEHICLE_LIVERY(vehicle) >= 0)
			vehicle_json[vehicle_livery_key] = VEHICLE::GET_VEHICLE_LIVERY(vehicle);

		if (VEHICLE::IS_THIS_MODEL_A_CAR(vehicle_hash) || VEHICLE::IS_THIS_MODEL_A_BIKE(vehicle_hash))
		{
			bool neon_lights[4]{};
			for (int i = (int)NeonLightLocations::NEON_LEFT; i <= (int)NeonLightLocations::NEON_BACK; i++)
				neon_lights[i] = VEHICLE::GET_VEHICLE_NEON_ENABLED(vehicle, i);

			int neon_color[3]{};
			VEHICLE::GET_VEHICLE_NEON_COLOUR(vehicle, &neon_color[0], &neon_color[1], &neon_color[2]);

			int interior_color, dashboard_color;
			VEHICLE::GET_VEHICLE_EXTRA_COLOUR_5(vehicle, &interior_color);
			VEHICLE::GET_VEHICLE_EXTRA_COLOUR_6(vehicle, &dashboard_color);

			vehicle_json[plate_text_key] = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(vehicle);
			vehicle_json[plate_text_index_key] = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehicle);
			vehicle_json[drift_tires] = VEHICLE::GET_DRIFT_TYRES_SET(vehicle);
			vehicle_json[neon_color_key] = neon_color;
			vehicle_json[neon_lights_key] = neon_lights;
			vehicle_json[interior_color_key] = interior_color;
			vehicle_json[dash_color_key] = dashboard_color;
		}

		return vehicle_json;
	}
	static std::unordered_map<std::string, std::string> ParseIni(const std::filesystem::path& path)
	{
		std::unordered_map<std::string, std::string> out;
		std::ifstream file(path);
		std::string line;
		std::string section;
		while (std::getline(file, line))
		{
			if (line.empty() || line[0] == ';')
				continue;
			if (line.front() == '[' && line.back() == ']')
			{
				section = line.substr(1, line.size() - 2);
				continue;
			}
			auto pos = line.find('=');
			if (pos == std::string::npos)
				continue;
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
			if (!section.empty())
				key = section + "." + key;

			out[key] = value;
		}
		return out;
	}
	std::optional<Vehicle> SpawnFromIni(const std::filesystem::path& path)
	{
		auto ini = ParseIni(path);
		auto getStr = [&](const char* key, const char* section = "Vehicle") -> std::optional<std::string> {
			const std::string k1 = std::string(section) + "." + key;
			if (ini.contains(k1))
				return ini[k1];
			if (ini.contains(key))
				return ini[key];
			return std::nullopt;
		};
		auto getInt = [&](const char* key, int def = 0, const char* section = "Vehicle") -> int {
			if (auto s = getStr(key, section))
				return std::stoi(*s);
			return def;
		};
		auto modelStr = getStr("Model");
		if (!modelStr)
			return std::nullopt;
		Hash model = Joaat(modelStr->c_str());
		auto veh = Vehicle::Create(
		    model,
		    Vehicle::GetSpawnLocRelToPed(Self::GetPed().GetHandle(), model),
		    Self::GetPed().GetHeading());
		if (!veh)
			return std::nullopt;
		ScriptMgr::Yield();
		auto v = veh.GetHandle();
		VEHICLE::SET_VEHICLE_MOD_KIT(v, 0);
		VEHICLE::SET_VEHICLE_COLOURS(v, getInt("PrimaryColor"), getInt("SecondaryColor"));
		VEHICLE::SET_VEHICLE_EXTRA_COLOURS(v, getInt("PearlescentColor"), getInt("WheelColor"));
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(v, getInt("WheelType"));
		VEHICLE::SET_VEHICLE_WINDOW_TINT(v, getInt("WindowTint", -1));
		if (auto plate = getStr("PlateText"))
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(v, plate->c_str());
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(v, getInt("PlateIndex", 0));
		VEHICLE::SET_DRIFT_TYRES(v, getInt("DriftTires", 0));
		VEHICLE::SET_VEHICLE_NEON_ENABLED(v, 0, getInt("EnabledLeft", 0, "Neon"));
		VEHICLE::SET_VEHICLE_NEON_ENABLED(v, 1, getInt("EnabledRight", 0, "Neon"));
		VEHICLE::SET_VEHICLE_NEON_ENABLED(v, 2, getInt("EnabledFront", 0, "Neon"));
		VEHICLE::SET_VEHICLE_NEON_ENABLED(v, 3, getInt("EnabledBack", 0, "Neon"));
		VEHICLE::SET_VEHICLE_NEON_COLOUR(v, getInt("ColorR", 0, "Neon"), getInt("ColorG", 0, "Neon"), getInt("ColorB", 0, "Neon"));
		auto setMod = [&](VehicleModType mod, const char* key) {
			if (int val = getInt(key, -1, "Mods"); val >= 0)
				VEHICLE::SET_VEHICLE_MOD(v, (int)mod, val, false);
		};
		setMod(VehicleModType::MOD_SPOILERS, "Spoilers");
		setMod(VehicleModType::MOD_ENGINE, "Engine");
		setMod(VehicleModType::MOD_BRAKES, "Brakes");
		setMod(VehicleModType::MOD_TRANSMISSION, "Transmission");
		setMod(VehicleModType::MOD_SUSPENSION, "Suspension");
		if (getInt("Turbo", 0, "Mods"))
			VEHICLE::TOGGLE_VEHICLE_MOD(v, (int)VehicleModType::MOD_TURBO, TRUE);
		auto getF = [&](const std::string& key, float def = 0.f) -> float {
			if (ini.contains(key))
				return std::stof(ini[key]);
			return def;
		};
		for (int i = 0; i < 32; i++)
		{
			const std::string base = std::to_string(i);
			const std::string modelKey = base + ".model";
			if (!ini.contains(modelKey))
				continue;
			Hash propHash = Joaat(ini[modelKey].c_str());
			STREAMING::REQUEST_MODEL(propHash);
			int tries = 0;
			while (!STREAMING::HAS_MODEL_LOADED(propHash))
			{
				ScriptMgr::Yield();
				if (++tries > 200)
					break;
			}
			if (!STREAMING::HAS_MODEL_LOADED(propHash))
			{
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(propHash);
				continue;
			}

			ScriptMgr::Yield();
			Object obj = OBJECT::CREATE_OBJECT(propHash, 0.f, 0.f, 0.f, true, true, false);
			float x = getF(base + ".x");
			float y = getF(base + ".y");
			float z = getF(base + ".z");
			float rx = getF(base + ".RotX");
			float ry = getF(base + ".RotY");
			float rz = getF(base + ".RotZ");
			ENTITY::ATTACH_ENTITY_TO_ENTITY(obj, v, 0, x, y, z, rx, ry, rz, false, false, false, false, 2, true, true);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(propHash);
		}
		return veh;
	}
	void SavedVehicles::Save(std::string folderName, std::string fileName)
	{
		if (auto veh = Self::GetVehicle(); veh && veh.IsValid())
		{
			const auto file = SavedVehicles::CheckFolder(folderName).GetFile(fileName);
			std::ofstream file_stream(file.Path(), std::ios::out | std::ios::trunc);
			file_stream << SavedVehicles::GetJson(veh).dump(4);
			file_stream.close();
		}
		else
			Notifications::Show("Persist Car", "Tried to save a vehicle which does not exist", NotificationType::Warning);
	}

	void SavedVehicles::Load(std::string folderName, std::string fileName, bool spawnInside)
	{
		if (fileName.empty())
		{
			Notifications::Show("Persist Car", "Select a file first", NotificationType::Warning);
			return;
		}

		const auto ext = std::filesystem::path(fileName).extension();

		const auto file =
		    (ext == ".ini") ? CheckIniFolder(folderName).GetFile(fileName).Path() : CheckFolder(folderName).GetFile(fileName).Path();

		if (!std::filesystem::exists(file))
		{
			Notifications::Show("Persist Car", "File does not exist.", NotificationType::Error);
			return;
		}
		try
		{
			if (ext == ".json")
			{
				std::ifstream file_stream(file);
				nlohmann::json vehicle_json;
				file_stream >> vehicle_json;

				Vehicle veh = SpawnFromJson(vehicle_json);

				if (veh)
				{
					if (spawnInside)
						Self::GetPed().SetInVehicle(veh.GetHandle());

					Notifications::Show("Persist Car", std::format("Spawned {}", fileName), NotificationType::Success);
				}
				else
				{
					Notifications::Show("Persist Car", std::format("Unable to spawn {}", fileName), NotificationType::Error);
				}
			}
			else if (ext == ".ini")
			{
				auto vehOpt = SpawnFromIni(file);

				if (vehOpt)
				{
					auto& veh = *vehOpt;

					if (spawnInside)
						Self::GetPed().SetInVehicle(veh.GetHandle());

				Notifications::Show("Persist Car", std::format("Spawned {}", fileName), NotificationType::Success);
				}
				else
				{
					Notifications::Show("Persist Car", std::format("Unable to spawn {}", fileName), NotificationType::Error);
				}
			}
			else
			{
				Notifications::Show("Persist Car", "Unsupported vehicle file type", NotificationType::Error);
			}
		}
		catch (const std::exception& e)
		{
			Notifications::Show("Persist Car", "Failed to load vehicle file", NotificationType::Error);
		}
	}


	Vehicle SavedVehicles::SpawnFromJson(nlohmann::json vehicle_json)
	{
		const Hash vehicle_hash = vehicle_json[vehicle_model_hash_key];
		auto veh = Vehicle::Create(vehicle_hash, Vehicle::GetSpawnLocRelToPed(Self::GetPed().GetHandle(), vehicle_hash), Self::GetPed().GetHeading());

		if (veh != nullptr)
		{
			ScriptMgr::Yield();

			auto vehicle = veh.GetHandle();

			VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);

			if (!(vehicle_json[primary_color_key].is_null() || vehicle_json[secondary_color_key].is_null()))
				VEHICLE::SET_VEHICLE_COLOURS(vehicle, vehicle_json[primary_color_key], vehicle_json[secondary_color_key]);

			if (!vehicle_json[custom_primary_color_key].is_null())
			{
				std::vector<int> primary_custom_color = vehicle_json[custom_primary_color_key];
				VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle, primary_custom_color[0], primary_custom_color[1], primary_custom_color[2]);
			}
			if (!vehicle_json[custom_secondary_color_key].is_null())
			{
				std::vector<int> secondary_custom_color = vehicle_json[custom_secondary_color_key];
				VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle, secondary_custom_color[0], secondary_custom_color[1], secondary_custom_color[2]);
			}

			if (!vehicle_json[vehicle_window_tint_key].is_null())
				VEHICLE::SET_VEHICLE_WINDOW_TINT(vehicle, vehicle_json[vehicle_window_tint_key]);
			if (!(vehicle_json[pearlescent_color_key].is_null() || vehicle_json[wheel_color_key].is_null()))
				VEHICLE::SET_VEHICLE_EXTRA_COLOURS(vehicle, vehicle_json[pearlescent_color_key], vehicle_json[wheel_color_key]);
			if (!vehicle_json[tire_can_burst].is_null())
				VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(vehicle, vehicle_json[tire_can_burst]);

			if (!vehicle_json[wheel_type_key].is_null())
				VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, vehicle_json[wheel_type_key]);

			if (!vehicle_json[vehicle_livery_key].is_null())
				VEHICLE::SET_VEHICLE_LIVERY(vehicle, vehicle_json[vehicle_livery_key]);

			if (VEHICLE::IS_THIS_MODEL_A_CAR(vehicle_hash) || VEHICLE::IS_THIS_MODEL_A_BIKE(vehicle_hash))
			{
				if (!vehicle_json[neon_color_key].is_null())
				{
					std::vector<int> neon_color = vehicle_json[neon_color_key];
					VEHICLE::SET_VEHICLE_NEON_COLOUR(vehicle, neon_color[0], neon_color[1], neon_color[2]);
				}
				if (!vehicle_json[neon_lights_key].is_null())
				{
					std::vector<bool> neon_lights = vehicle_json[neon_lights_key];
					for (int i = (int)NeonLightLocations::NEON_LEFT; i <= (int)NeonLightLocations::NEON_BACK; i++)
						VEHICLE::SET_VEHICLE_NEON_ENABLED(vehicle, i, neon_lights[i]);
				}
				if (!vehicle_json[plate_text_key].is_null())
					VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(vehicle, vehicle_json[plate_text_key].get<std::string>().c_str());
				if (!vehicle_json[plate_text_index_key].is_null())
					VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehicle, vehicle_json[plate_text_index_key]);
				if (!vehicle_json[drift_tires].is_null())
					VEHICLE::SET_DRIFT_TYRES(vehicle, vehicle_json[drift_tires]);

				if (!vehicle_json[interior_color_key].is_null())
					VEHICLE::SET_VEHICLE_EXTRA_COLOUR_5(vehicle, vehicle_json[interior_color_key]);
				if (!vehicle_json[dash_color_key].is_null())
					VEHICLE::SET_VEHICLE_EXTRA_COLOUR_6(vehicle, vehicle_json[dash_color_key]);
			}

			for (int i = (int)VehicleModType::MOD_SPOILERS; i < (int)VehicleModType::MOD_LIGHTBAR; i++)
				if (!vehicle_json[mod_names[i]].is_null())
				{
					if (vehicle_json[mod_names[i]].is_array())
					{
						std::vector<int> mod = vehicle_json[mod_names[i]];
						VEHICLE::SET_VEHICLE_MOD(vehicle, i, mod[0], mod[1]);
					}
					else
					{
						if (i == (int)VehicleModType::MOD_TYRE_SMOKE && !vehicle_json[tire_smoke_color_key].is_null())
						{
							std::vector<int> tire_smoke_color = vehicle_json[tire_smoke_color_key];
							VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(vehicle, tire_smoke_color[0], tire_smoke_color[1], tire_smoke_color[2]);
						}
						else if (i == (int)VehicleModType::MOD_XENON_LIGHTS && !vehicle_json[headlight_color_key].is_null())
							VEHICLE::SET_VEHICLE_XENON_LIGHT_COLOR_INDEX(vehicle, vehicle_json[headlight_color_key]);

						VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, i, TRUE);
					}
				}

			if (!vehicle_json[vehicle_extras_key].is_null())
			{
				std::map<int, int> vehicle_extras = vehicle_json[vehicle_extras_key];
				for (const auto& [extra, extra_enabled] : vehicle_extras)
					VEHICLE::SET_VEHICLE_EXTRA(vehicle, extra, extra_enabled ? 0 : 1);
			}
		}

		return veh;
	}
}
