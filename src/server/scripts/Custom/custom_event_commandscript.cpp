/*
 * Fake
 */

/* ScriptData
Name: custom_event_commandscript
%Complete: 100
Category: Custom Scripts
EndScriptData */

#include "ScriptPCH.h"
#include "Chat.h"
#include "Player.h"


#define NO_EVENT_ATM "There isn't any event atm."
#define CLOSED_EVENT  "You just closed event: %s"
#define CURRENT_EVENT "The current event is : %s"



class custom_event_commandscript : public CommandScript
{
    public:
        custom_event_commandscript() : CommandScript("custom_event_commandscript") { }

		static bool HandleEventOpenCommand(ChatHandler* handler, const char* args)
        {
			Player* player = handler->GetSession()->GetPlayer();
			std::string name = handler->GetSession()->GetPlayer()->GetName();
			QueryResult cx = LoginDatabase.PQuery("SELECT `x` FROM custom_events");
			uint32 map = player->GetMapId();
			float x,y,z,o;
			x = player->GetPositionX();
			y = player->GetPositionY();
			z = player->GetPositionZ();
			o = player->GetOrientation();
			if (!*args)
			{
				handler->SendSysMessage("You must set a name of the event.");
				return false;
			}
			
			if (handler->HasLowerSecurity(player, 0))
				return false;
				
			if (player->isInCombat())
			{
				handler->SendSysMessage(LANG_YOU_IN_COMBAT);
				handler->SetSentErrorMessage(true);
				return false;
			}
			if(cx)
			{
				handler->SendSysMessage("There is already 1 opened event.Please close it first.");
				return false;
			}
			
			LoginDatabase.PExecute("INSERT INTO `custom_events` (`x`, `y`, `z`, `o`, `m`, `name`) VALUES (%f, %f, %f, %f, %u, '%s')", x, y, z, o, map, args);
			
			
			sWorld->SendWorldText(LANG_EVENT_CUSTOM, name.c_str(), args);
			handler->SendSysMessage("You've just opened an event.");
            return true;
        }
		
		static bool HandleEventViewCommand(ChatHandler* handler, const char* /*args*/)
        {	
			Player* player = handler->GetSession()->GetPlayer();
			uint32 zoneId = player->GetZoneId();
			
			if(QueryResult event = LoginDatabase.PQuery("SELECT `name` FROM `custom_events`"))
			{
				Field* fields = event->Fetch();	
				const char* namee = fields[0].GetCString();
				handler->PSendSysMessage("There is an event atm - ' %s '", namee);
				return true;
			}
			
			else
			{
				handler->SendSysMessage(NO_EVENT_ATM);
				return true;
			}
		}
		
		static bool HandleEventCloseCommand(ChatHandler* handler, const char* args)
        {
			if (!*args)
				return false;
			
			std::string name = handler->GetSession()->GetPlayer()->GetName();
			
			if(QueryResult event = LoginDatabase.PQuery("SELECT `name` FROM `custom_events`"))
			{
				Field* fields = event->Fetch();	
				const char* namee = fields[0].GetCString();			
				handler->PSendSysMessage("You just closed event: %s ", namee);
				LoginDatabase.PQuery("delete FROM custom_events");
				return true;
			}
			
			else
			{
				handler->SendSysMessage(NO_EVENT_ATM);
				return true;
			}
		}
		
		// Display the list of VIPs
		static bool HandleEventJoinCommand(ChatHandler* handler, char const* /*args*/)
		{
			Player* player = handler->GetSession()->GetPlayer();
			
			if (player->isInCombat())
			{
				handler->SendSysMessage(LANG_YOU_IN_COMBAT);
				handler->SetSentErrorMessage(true);
				return false;
			}	
			
			if(QueryResult event = LoginDatabase.PQuery("SELECT x, y, z, m, o  FROM custom_events"))
			{
				Field* fields = event->Fetch();
				float x = fields[0].GetFloat();
				float y = fields[1].GetFloat();
				float z = fields[2].GetFloat();
				uint32 m = fields[3].GetUInt32();
				uint32 o = fields[4].GetUInt32();
				player->TeleportTo(m, x, y, z, o);
				player->Yell("Hey all, I just came to the event!", 1);			
				return true;
			}
			
			else
			{
				handler->SendSysMessage("No events atm.");
				return false;
			}
		}
		
        ChatCommand* GetCommands() const
        {
			static ChatCommand EvvCommandTable[] =
			{
				{	"open",			SEC_ADMINISTRATOR,	true,   &HandleEventOpenCommand,       		"", NULL },
				{	"close",		SEC_ADMINISTRATOR,	true,   &HandleEventCloseCommand,       		"", NULL },
				{	"view",			SEC_PLAYER,	true,   &HandleEventViewCommand,       	"", NULL },
				{	"join",			SEC_PLAYER,	true,   &HandleEventJoinCommand,       	"", NULL },
				{ 	NULL, 	  		  0,	false,	NULL,								"", NULL }
			};
			
            static ChatCommand EvCommandTable[] =
            {
				{	"cevent",	SEC_PLAYER,      false,	NULL,	"", EvvCommandTable },
                {	NULL,	0,            false,	NULL,   "", NULL }
            };
            return EvCommandTable;
        }
};

void AddSC_custom_event_commandscript()
{
    new custom_event_commandscript();
}
