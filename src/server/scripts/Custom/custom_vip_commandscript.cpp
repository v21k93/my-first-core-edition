/*
 * Ac0
 */

/* ScriptData
Name: custom_vip_commandscript
%Complete: 100
Comment: Short custom command scripts for VIP users
Category: Custom Scripts
EndScriptData */

#include "ScriptPCH.h"
#include "Chat.h"
#include "Player.h"

class custom_vip_commandscript : public CommandScript
{
    public:
        custom_vip_commandscript() : CommandScript("custom_vip_commandscript") { }

		static bool HandleVipBuffCommand(ChatHandler* handler, const char* /*args*/)
        {
			Player* player = handler->GetSession()->GetPlayer();
			uint8 playerClass = player->getClass();
			
			if (handler->HasLowerSecurity(player, 0))
				return false;
				
			if (player->isInCombat())
			{
				handler->SendSysMessage(LANG_YOU_IN_COMBAT);
				handler->SetSentErrorMessage(true);
				return false;
			}
			
			player->CastSpell(player, 48161, 1); // Power Word: Fortitude
			
			player->CastSpell(player, 48073, 1); // Divine Spirit
			
			player->CastSpell(player, 48169, 1); // Shadow Protection
			
			player->CastSpell(player, 61024, 1); // Dalaran Inellect
			
			player->CastSpell(player, 48469, 1); // Mark of the Wild
			
			if (playerClass == CLASS_WARRIOR)
				player->CastSpell(player, 25899, 1); // Blessing of Santuary
			else
				player->CastSpell(player, 25898, 1); // Blessing of Kings
		
            return true;
        }
		
		static bool HandleVipMallCommand(ChatHandler* handler, const char* /*args*/)
        {
			Player* player = handler->GetSession()->GetPlayer();
			uint32 zoneId = player->GetZoneId();
			
			if (zoneId != 1519 && zoneId != 1537 && zoneId != 1 && zoneId != 1657 && zoneId != 3557 && zoneId != 1497 && zoneId != 85 && zoneId != 1638 && zoneId != 1637 && zoneId != 3487)
			{
				handler->SendSysMessage("You're not in capital city.");
				handler->SetSentErrorMessage(true);
				return false;
			}

			if (player->isInCombat())
			{
				handler->SendSysMessage(LANG_YOU_IN_COMBAT);
				handler->SetSentErrorMessage(true);
				return false;
			}

			// Stop flight if need
			if (player->isInFlight())
			{
				player->GetMotionMaster()->MovementExpired();
				player->CleanupAfterTaxiFlight();
			}
			// Save only in non-flight case
			else
				player->SaveRecallPosition();

			player->TeleportTo(0,	23.8378f,	-1588.85f,	195.419f,	4.54306f); // MapId, X, Y, Z, O
            
			return true;
		}
		
		static bool HandleVipAnnounceCommand(ChatHandler* handler, const char* args)
        {
			if (!*args)
				return false;
			
			std::string name = handler->GetSession()->GetPlayer()->GetName();
			
			sWorld->SendWorldText(LANG_VIP_ANNOUNCE_COLOR, name.c_str(), args);
			return true;
		}
		
		// Display the list of VIPs
		static bool HandleVipListCommand(ChatHandler* handler, char const* /*args*/)
		{
			// Get the accounts with VIP Level >0
			QueryResult result = LoginDatabase.PQuery("SELECT account.username FROM account, account_access WHERE account.id=account_access.id AND account_access.gmlevel = 1 AND (account_access.realmid = -1 OR account_access.realmid = %u)", realmID);
			if (result)
			{
				handler->PSendSysMessage(LANG_VIPLIST);
				handler->PSendSysMessage("%s", "----------------------------");
				// Cycle through them. Display username and VIP level
				do
				{
					Field* fields = result->Fetch();
					char const* name = fields[0].GetCString();
					handler->PSendSysMessage("|     %s ", name);
				} while (result->NextRow());
				handler->PSendSysMessage("%s", "----------------------------");
			}
			else
				handler->PSendSysMessage(LANG_VIPLIST_EMPTY);
			return true;
		}
		
        ChatCommand* GetCommands() const
        {
			static ChatCommand vipCommandTable[] =
			{
				{	"buff",		SEC_VIP,	true,   &HandleVipBuffCommand,       		"", NULL },
				{	"mall",		SEC_VIP,	true,   &HandleVipMallCommand,       		"", NULL },
				{	"announce",	SEC_VIP,	true,   &HandleVipAnnounceCommand,       	"", NULL },
				{	"list",		SEC_VIP,	true,   &HandleVipListCommand,       		"", NULL },
				{ 	NULL, 	  		  0,	false,	NULL,								"", NULL }
			};
			
            static ChatCommand BuffCommandTable[] =
            {
				{	"vip",	SEC_VIP,      false,	NULL,	"", vipCommandTable },
                {	NULL,	0,            false,	NULL,   "", NULL }
            };
            return BuffCommandTable;
        }
};

void AddSC_custom_vip_commandscript()
{
    new custom_vip_commandscript();
}
