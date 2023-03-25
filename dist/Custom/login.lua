-- runs on login after setting up the AccountLogin frame

local username = GetEnvString("wow_username")
local password = GetEnvString("wow_password")
local character = GetEnvString("wow_character")

AccountLoginAccountEdit:SetText(username)
AccountLoginPasswordEdit:SetText(password)
AccountLogin_Login()

local function CharacterSelect_SelectByName(desired)
	local numChars = GetNumCharacters();

	for i=1, numChars, 1 do
		local name, race, class, level, zone, sex, ghost, PCC, PRC, PFC = GetCharacterInfo(i);
		if name == desired then
			SelectCharacter(i)
			break
		end
	end
end

local frame = CreateFrame("Frame")

local total = 0
frame:SetScript("OnUpdate", function(self, elapsed)
	if CharSelectCharacterName:GetText() ~= nil then
		total = total + elapsed
	end

	if total >= 0.1 then		
		self:SetScript("OnUpdate", nil) -- done

		CharacterSelect_SelectByName(character)
		CharacterSelect_EnterWorld()
	end
end)

frame:Show()
