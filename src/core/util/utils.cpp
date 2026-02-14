#include "utils.hpp"
#include "game/gta/Natives.hpp"
namespace Utils {
	void ApplyForceToEntity(Entity entity, float x, float y, float z, float offX, float offY, float offZ) {
		ENTITY::APPLY_FORCE_TO_ENTITY(entity, 1, x, y, z, offX, offY, offZ, 0, 1, 1, 1, 0, 1);
	}
   void SetHeading(float heading) {
       Ped ped = PLAYER::PLAYER_PED_ID();
       if (PED::IS_PED_IN_ANY_VEHICLE(ped, false))
       {
           Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
           ENTITY::SET_ENTITY_HEADING(veh, heading);
       }
       else 
       {
            ENTITY::SET_ENTITY_HEADING(ped, heading);
       }
    }
}
