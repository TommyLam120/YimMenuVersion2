#include "utils.hpp"
#include "game/gta/Natives.hpp"
namespace Utils {
	void ApplyForceToEntity(Entity entity, float x, float y, float z, float offX, float offY, float offZ) {
		ENTITY::APPLY_FORCE_TO_ENTITY(entity, 1, x, y, z, offX, offY, offZ, 0, 1, 1, 1, 0, 1);
	}
}
