/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Log.h"
#include "Opcodes.h"
#include "UpdateData.h"
#include "Player.h"
#include "UpdateFields.h"
#include "config.h"

void WorldSession::HandleDuelAcceptedOpcode(WorldPacket& recvPacket)
{
    uint32 zone1 = ConfigMgr::GetIntDefault("Duel.Zone1", NULL);
	uint32 zone2 = ConfigMgr::GetIntDefault("Duel.Zone2", NULL);
	uint32 zone3 = ConfigMgr::GetIntDefault("Duel.Zone3", NULL);
	uint32 zone4 = ConfigMgr::GetIntDefault("Duel.Zone4", NULL);
	
    uint64 guid;
    Player* player;
    Player* plTarget;

    recvPacket >> guid;

    if (!GetPlayer()->duel)                                  // ignore accept from duel-sender
        return;

    player       = GetPlayer();
    plTarget = player->duel->opponent;

    if (player == player->duel->initiator || !plTarget || player == plTarget || player->duel->startTime != 0 || plTarget->duel->startTime != 0)
        return;
	
    //sLog->outDebug(LOG_FILTER_PACKETIO, "WORLD: Received CMSG_DUEL_ACCEPTED");
    sLog->outStaticDebug("Player 1 is: %u (%s)", player->GetGUIDLow(), player->GetName());
    sLog->outStaticDebug("Player 2 is: %u (%s)", plTarget->GetGUIDLow(), plTarget->GetName());

    time_t now = time(NULL);
    player->duel->startTimer = now;
    plTarget->duel->startTimer = now;

    player->SendDuelCountdown(3000);
    plTarget->SendDuelCountdown(3000);	
	plTarget->MonsterWhisper("Prepare to fight...!",plTarget->GetGUID(),true);
	player->MonsterWhisper("Prepare to fight...!",player->GetGUID(),true);
	
	if (player->GetZoneId() == zone1 || player->GetZoneId() == zone2 || player->GetZoneId() == zone3 || player->GetZoneId() == zone4)
    {
		player->RemoveAllSpellCooldown();
		plTarget->RemoveAllSpellCooldown();
		player->SetHealth(player->GetMaxHealth());
		player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
		plTarget->SetHealth(plTarget->GetMaxHealth());
		plTarget->SetPower(POWER_MANA,  plTarget->GetMaxPower(POWER_MANA));
	}
}

void WorldSession::HandleDuelCancelledOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_DUEL_CANCELLED");
    uint64 guid;
    recvPacket >> guid;
    // no duel requested
    if (!GetPlayer()->duel)
        return;
		
	GetPlayer()->duel->opponent->SetPhaseMask(1, true);
	GetPlayer()->duel->opponent2->SetPhaseMask(1, true);

    // player surrendered in a duel using /forfeit
    if (GetPlayer()->duel->startTime != 0)
    {
        GetPlayer()->CombatStopWithPets(true);
        if (GetPlayer()->duel->opponent)
            GetPlayer()->duel->opponent->CombatStopWithPets(true);

        GetPlayer()->CastSpell(GetPlayer(), 7267, true);    // beg
        GetPlayer()->DuelComplete(DUEL_WON);
        return;
    }

    GetPlayer()->DuelComplete(DUEL_INTERRUPTED);
}
