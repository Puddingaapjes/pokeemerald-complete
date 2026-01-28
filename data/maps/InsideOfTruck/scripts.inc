InsideOfTruck_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, InsideOfTruck_OnLoad
	map_script MAP_SCRIPT_ON_RESUME, InsideOfTruck_OnResume
	.byte 0

InsideOfTruck_OnLoad:
	setmetatile 4, 1, METATILE_InsideOfTruck_ExitLight_Top, FALSE
	setmetatile 4, 2, METATILE_InsideOfTruck_ExitLight_Mid, FALSE
	setmetatile 4, 3, METATILE_InsideOfTruck_ExitLight_Bottom, FALSE
	end

InsideOfTruck_OnResume:
	setstepcallback STEP_CB_TRUCK
	end

InsideOfTruck_EventScript_SetIntroFlags::
	lockall
	setflag FLAG_HIDE_MAP_NAME_POPUP
	setrespawn HEAL_LOCATION_OLDALE_TOWN
	setdynamicwarp MAP_OLDALE_TOWN, 12, 18
	releaseall
	end

InsideOfTruck_EventScript_MovingBox::
	msgbox InsideOfTruck_Text_BoxPrintedWithMonLogo, MSGBOX_SIGN
	givemon SPECIES_BULBASAUR, 5
	givemon SPECIES_BULBASAUR, 5
	givemon SPECIES_BULBASAUR, 5
	givemon SPECIES_BULBASAUR, 5
	givemon SPECIES_BULBASAUR, 5
	givemon SPECIES_BULBASAUR, 5
	givemon SPECIES_BULBASAUR, 5
	givemon SPECIES_BULBASAUR, 5
	givemon SPECIES_BULBASAUR, 5
	givemon SPECIES_BULBASAUR, 5
	givemon SPECIES_BULBASAUR, 5
	givemon SPECIES_BULBASAUR, 5
	givemon SPECIES_BULBASAUR, 5
	givemon SPECIES_BULBASAUR, 5
	end

InsideOfTruck_Text_BoxPrintedWithMonLogo:
	.string "The box is printed with a POKéMON logo.\p"
	.string "It's a POKéMON brand moving and\n"
	.string "delivery service.$"

