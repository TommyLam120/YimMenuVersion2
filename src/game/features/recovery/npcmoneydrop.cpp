#include "core/commands/LoopedCommand.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Natives.hpp"
#include "game/gta/Pools.hpp"

namespace YimMenu::Features
{
    class PedDropMoney : public LoopedCommand 
    {
        using LoopedCommand::LoopedCommand; 
       virtual void OnTick() override
    {
        for (auto ped : Pools::GetPeds())
        {
            int handle = ped.GetHandle();
            if (!ENTITY::DOES_ENTITY_EXIST(handle))
                continue;
            if (PED::IS_PED_A_PLAYER(handle))
                continue;
            if (!PED::IS_PED_DEAD_OR_DYING(handle, true))
                continue;
            PED::SET_PED_MONEY(handle, 2000);
        }
    }
    virtual void OnDisable() override
    {
        for (auto ped : Pools::GetPeds())
        {
            int handle = ped.GetHandle();
            if (ENTITY::DOES_ENTITY_EXIST(handle))
                PED::SET_PED_MONEY(handle, 0);
        }
    }
};

    static PedDropMoney _PedDropMoney{"peddropcash", "Make NPC's drop 2k Money", "be carefull with it too much for it you get transactions errors and can brake the game"};
}
