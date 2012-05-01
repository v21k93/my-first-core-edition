/*
 * Ac0
 */

/* ScriptData
Name: custom_duel_commandscript
%Complete: 100
Comment: Short custom command scripts for duel watching
Category: Custom Scripts
EndScriptData */

#include "ScriptPCH.h"
#include "Chat.h"
#include "Player.h"
#include "SocialMgr.h"

class custom_duel_commandscript : public CommandScript
{
    public:
        custom_duel_commandscript() : CommandScript("custom_duel_commandscript") { }
		
		static bool HandleDuelViewCommand(ChatHandler* handler, char const* args)
		{
			Player* target;
			uint64 target_guid;
			std::string target_name;
			if (!handler->extractPlayerTarget((char*)args, &target, &target_guid, &target_name))
				return false;
				
			Player* _player = handler->GetSession()->GetPlayer();
			
			if (target == _player || target_guid == _player->GetGUID())
			{
				handler->PSendSysMessage(LANG_CANT_TELEPORT_SELF);
				handler->SetSentErrorMessage(true);
				return false;
			}
			
			if (target)
			{
				// check online security
				if (handler->HasLowerSecurity(target, 0))
					return false;

				std::string chrNameLink = handler->playerLink(target_name);
				
				if (target->GetSocial()->HasIgnore(_player->GetGUIDLow()))
				{
					handler->PSendSysMessage("Player %s forbided you to watch his duel match.", chrNameLink.c_str());
					handler->SetSentErrorMessage(true);
					return false;
				}
				
				if (!target->duel)
				{
					handler->PSendSysMessage("Target is not dueling.");
					handler->SetSentErrorMessage(true);
					return false;
				}
				
				if (target->GetPhaseMask() == _player->GetPhaseMask())
				{
					handler->PSendSysMessage("You are in the same phase.");
					handler->SetSentErrorMessage(true);
					return false;
				}
				
				Map* cMap_target = target->GetMap();
				
				if(cMap_target != _player->GetMap())
				{
					handler->PSendSysMessage("Must be in the same map.");
					handler->SetSentErrorMessage(true);
					return false;
				}
				
				if (cMap_target->IsBattlegroundOrArena() || cMap_target->IsDungeon() || cMap_target->IsRaid())
				{
					handler->PSendSysMessage(LANG_CANNOT_GO_TO_INST_PARTY, chrNameLink.c_str());
					handler->SetSentErrorMessage(true);
					return false;
				}
				
				// stop flight if need
				if (_player->isInFlight())
				{
					_player->GetMotionMaster()->MovementExpired();
					_player->CleanupAfterTaxiFlight();
				}
				// save only in non-flight case
				else
					_player->SaveRecallPosition();
					
				// to point to see at target with same orientation
				float x, y, z;
				target->GetContactPoint(_player, x, y, z);

				_player->setFaction(35);
				_player->CombatStop();
				
				if (_player->IsNonMeleeSpellCasted(true))
					_player->InterruptNonMeleeSpells(true);
					
				//if player class = hunter || warlock remove pet if alive
				if ((_player->getClass() == CLASS_HUNTER) || (_player->getClass() == CLASS_WARLOCK))
				{
					if (Pet* pet = _player->GetPet())
					{
						pet->SavePetToDB(PET_SAVE_AS_CURRENT);
						// not let dismiss dead pet
						if (pet && pet->isAlive())
							_player->RemovePet(pet, PET_SAVE_NOT_IN_SLOT);
					}
				}
				
				_player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
				
				if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(8326))
					Aura::TryRefreshStackOrCreate(spellInfo, MAX_EFFECT_MASK, _player, _player);
					
				_player->TeleportTo(target->GetMapId(), x, y, z, _player->GetAngle(target));
				_player->SetPhaseMask(target->GetPhaseMask(), true);
				
				_player->SaveToDB();
			}
			
			return true;
		}
		
		static bool HandleDuelLeaveCommand(ChatHandler* handler, char const* /*args*/)
		{
			Player* _player = handler->GetSession()->GetPlayer();

			_player->setFactionForRace(_player->getRace());
			_player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

			_player->RemoveAurasDueToSpell(8326);

			_player->SetPhaseMask(1, true);
				
			_player->SaveToDB();
			
			return true;
		}
		
        ChatCommand* GetCommands() const
        {
			static ChatCommand DuelSubCommandsTable[] =
			{
				{	"view",		SEC_PLAYER,	true,   &HandleDuelViewCommand,       		"", NULL },
				{	"leave",	SEC_PLAYER,	true,   &HandleDuelLeaveCommand,       		"", NULL },
				{ 	NULL, 	  		  0,	false,	NULL,								"", NULL }
			};
			
            static ChatCommand DuelCommandTable[] =
            {
				{	"duel",	SEC_PLAYER,      false,	NULL,	"", DuelSubCommandsTable },
                {	NULL,	0,           	 false,	NULL,   "", NULL }
            };
            return DuelCommandTable;
        }
};

void AddSC_custom_duel_commandscript()
{
    new custom_duel_commandscript();
}
