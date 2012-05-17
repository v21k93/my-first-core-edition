/*
Custom CommandScript [Cast All Players Online..!!] 
By Fake.
*/

#include "ScriptPCH.h"
#include "Chat.h"

class CastAll_Comandscript : public CommandScript
{
    public:
        CastAll_Comandscript() : CommandScript("CastAll_Comandscript") { }

		static bool HandleCastAllCommand(ChatHandler* handler, const char* args)
        {
		
			WorldSession* m_session = handler->GetSession();
			Player* target1 = handler->GetSession()->GetPlayer();
			Unit* target = handler->GetSession()->GetPlayer();

			if (!target)
			{
				return false;
			}

			// number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
			uint32 spell = handler->extractSpellIdFromLink((char*)args);
			if (!spell)
				return false;

			SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell);
			if (!spellInfo)
			{
				return false;
			}

			if (!SpellMgr::IsSpellValid(spellInfo, m_session->GetPlayer()))
			{
				return false;
			}

			char* trig_str = strtok(NULL, " ");
			if (trig_str)
			{
				int l = strlen(trig_str);
				if (strncmp(trig_str, "triggered", l) != 0)
					return false;
			}

			bool triggered = (trig_str != NULL);
			sObjectAccessor->CastAllPlayers(spell);

            return true;
        }

		static bool HandleAuraAllCommand(ChatHandler* handler, const char* args)
        {
		
			WorldSession* m_session = handler->GetSession();
			Player* target1 = handler->GetSession()->GetPlayer();
			Unit* target = handler->GetSession()->GetPlayer();

			if (!target)
			{
				return false;
			}

			// number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
			uint32 spell = handler->extractSpellIdFromLink((char*)args);
			if (!spell)
				return false;

			SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell);
			if (!spellInfo)
			{
				return false;
			}

			if (!SpellMgr::IsSpellValid(spellInfo, m_session->GetPlayer()))
			{
				return false;
			}

			char* trig_str = strtok(NULL, " ");
			if (trig_str)
			{
				int l = strlen(trig_str);
				if (strncmp(trig_str, "triggered", l) != 0)
				return false;
			}

			bool triggered = (trig_str != NULL);
			sObjectAccessor->AuraAllPlayers(spell);
            return true;
        }
        ChatCommand* GetCommands() const
        {
            static ChatCommand CastAllCommandTable[] =
            {
                { "castall",          SEC_MODERATOR,         true,   &HandleCastAllCommand,        "", NULL },				
                { "auraall",          SEC_MODERATOR,         true,   &HandleAuraAllCommand,        "", NULL },
                { NULL,             0,                  false,  NULL,                            "", NULL }
            };
            return CastAllCommandTable;
        }
};

void AddSC_CastAll_Comandscript()
{
    new CastAll_Comandscript();
}
