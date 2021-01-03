#include "framework.h"
#include <iostream>
#include <regex>
#include <tuple>
#include <vector>
#include <iomanip>
#include <ostream>


BOOL CALLBACK my_enum_proc(HMONITOR, HDC, LPRECT, LPARAM);

std::vector<HMONITOR> hmonitor_vec;

constexpr const char* vcp_enum_to_string(const MC_VCP_CODE_TYPE e)
{
	switch (e)
	{
	case MC_MOMENTARY: return "MC_MOMENTARY";
	case MC_SET_PARAMETER: return "MC_SET_PARAMETER";
	}
	return "UNKNOWN";
}

constexpr const char* vcp_code_desc(const BYTE vcp_code)
{
	switch (vcp_code)
	{
	case 0x01: return "DEGAUSS";
	case 0x02: return "NEW CONTROL VALUE";
	case 0x03: return "SOFT CONTROLS";
	case 0x04: return "RESTORE FACTORY DEFAULTS";
	case 0x05: return "RESTORE FACTORY BRIGHTNESS/CONTRAST SETTINGS";
	case 0x06: return "RESTORE FACTORY GEOMETRY DEFAULTS";
	case 0x08: return "RESTORE COLOR DEFAULTS";
	case 0x0A: return "RESTORE FACTORY TV DEFAULTS";
	case 0x0B: return "COLOR TEMPERATURE INCREMENT";
	case 0x0C: return "COLOR TEMPERATURE REQUEST";
	case 0x0E: return "CLOCK";
	case 0x10: return "BRIGHTNESS";
	case 0x11: return "FLESH TONE ENHANCEMENT";
	case 0x12: return "CONTRAST";
	case 0x13: return "BACKLIGHT CONTROL";
	case 0x14: return "SELECT COLOR PRESET";
	case 0x16: return "VIDEO GAIN: RED";
	case 0x17: return "USER COLOR VISION COMPENSATION";
	case 0x18: return "VIDEO GAIN: GREEN";
	case 0x1A: return "VIDEO GAIN: BLUE";
	case 0x1C: return "FOCUS";
	case 0x1E: return "AUTO SETUP";
	case 0x1F: return "AUTO COLOR SETUP";
	case 0x20: return "HORIZONTAL POSITION (PHASE)";
	case 0x22: return "HORIZONTAL SIZE";
	case 0x24: return "HORIZONTAL PINCUSHION";
	case 0x26: return "HORIZONTAL PINCUSHION BALANCE";
	case 0x28: return "HORIZONTAL CONVERGENCE R/B";
	case 0x29: return "HORIZONTAL CONVERGENCE M/G";
	case 0x2A: return "HORIZONTAL LINEARITY";
	case 0x2C: return "HORIZONTAL LINEARITY BALANCE";
	case 0x2E: return "GRAY SCALE COMPENSATION";
	case 0x30: return "VERTICAL POSITION (PHASE)";
	case 0x32: return "VERTICAL SIZE";
	case 0x34: return "VERTICAL PINCUSHION";
	case 0x36: return "VERTICAL PINCUSHION BALANCE";
	case 0x38: return "VERTICAL CONVERGENCE R/B";
	case 0x39: return "VERTICAL CONVERGENCE M/G";
	case 0x3A: return "VERTICAL LINEARITY";
	case 0x3C: return "VERTICAL LINEARITY BALANCE";
	case 0x3E: return "CLOCK PHASE";
	case 0x40: return "HORIZONTAL PARALLELOGRAM";
	case 0x41: return "VERTICAL PARALLELOGRAM";
	case 0x42: return "HORIZONTAL KEYSTONE";
	case 0x43: return "VERTICAL KEYSTONE";
	case 0x44: return "ROTATION";
	case 0x46: return "TOP CORNER FLARE";
	case 0x48: return "TOP CORNER HOOK";
	case 0x4A: return "BOTTOM CORNER FLARE";
	case 0X4C: return "BOTTOM CORNER HOOK";
	case 0x52: return "ACTIVE CONTROL";
	case 0x54: return "PERFORMANCE PRESERVATION";
	case 0x56: return "HORIZONTAL MOIRE";
	case 0x58: return "VERTICAL MOIRE";
	case 0x59: return "6 AXIS SATURATION: RED";
	case 0x5A: return "6 AXIS SATURATION: YELLOW";
	case 0x5B: return "6 AXIS SATURATION: GREEN";
	case 0x5C: return "6 AXIS SATURATION: CYAN";
	case 0x5D: return "6 AXIS SATURATION: BLUE";
	case 0x5E: return "6 AXIS SATURATION: MAGENTA";
	case 0x60: return "INPUT SOURCE";
	case 0x62: return "AUDIO SPEAKER VOLUME";
	case 0x63: return "SPEAKER SELECT";
	case 0x64: return "AUDIO: MICROPHONE VOLUME";
	case 0x66: return "AMBIENT LIGHT SENSOR";
	case 0x6B: return "BACKLIGHT LEVEL: WHITE";
	case 0x6C: return "VIDEO BLACK LEVEL: RED";
	case 0x6D: return "BACKLIGHT LEVEL: RED";
	case 0x6E: return "VIDEO BLACK LEVEL: GREEN";
	case 0x6F: return "BACKLIGHT LEVEL: GREEN";
	case 0x70: return "VIDEO BLACK LEVEL: BLUE";
	case 0x71: return "BACKLIGHT LEVEL: BLUE";
	case 0x72: return "GAMMA";
	case 0x73: return "LUT SIZE";
	case 0x74: return "SINGLE POINT LUT OPERATION";
	case 0x75: return "BLOCK LUT OPERATION";
	case 0x76: return "REMOTE PROCEDURE CALL";
	case 0x78: return "DISPLAY IDENTIFICATION OPERATION";
	case 0x7A: return "ADJUST FOCAL PLANE";
	case 0x7C: return "ADJUST ZOOM";
	case 0x7E: return "TRAPEZOID";
	case 0x80: return "KEYSTONE";
	case 0x82: return "HORIZONTAL MIRROR (FLIP)";
	case 0x84: return "VERTICAL MIRROR (FLIP)";
	case 0x86: return "DISPLAY SCALING";
	case 0x87: return "SHARPNESS";
	case 0x88: return "VELOCITY SCAN MODULATION";
	case 0x8A: return "COLOR SATURATION";
	case 0x8B: return "TV CHANNEL UP/DOWN";
	case 0x8C: return "TV SHARPNESS";
	case 0x8D: return "AUDIO MUTE/SCREEN BLANK";
	case 0x8E: return "TV CONTRAST";
	case 0x8F: return "AUDIO TREBLE";
	case 0x90: return "HUE";
	case 0x91: return "AUDIO BASS";
	case 0x92: return "TV BLACK LEVEL/LUMINESCENCE";
	case 0x93: return "AUDIO BALANCE L/R";
	case 0x94: return "AUDIO PROCESSOR MODE";
	case 0x95: return "WINDOW POSITION (TL_X)";
	case 0x96: return "WINDOW POSITION (TL_Y)";
	case 0x97: return "WINDOW POSITION (BR_X)";
	case 0x98: return "WINDOW POSITION (BR_Y)";
	case 0x99: return "WINDOW CONTROL ON/OFF";
	case 0x9A: return "WINDOW BACKGROUND";
	case 0x9B: return "6 AXIS HUE CONTROL: RED";
	case 0x9C: return "6 AXIS HUE CONTROL: YELLOW";
	case 0x9D: return "6 AXIS HUE CONTROL: GREEN";
	case 0x9E: return "6 AXIS HUE CONTROL: CYAN";
	case 0x9F: return "6 AXIS HUE CONTROL: BLUE";
	case 0xA0: return "6 AXIS HUE CONTROL: MAGENTA";
	case 0xA2: return "AUTO SETUP ON/OFF";
	case 0xA4: return "WINDOW MASK CONTROL";
	case 0xA5: return "CHANGE THE SELECTED WINDOW";
	case 0xAA: return "SCREEN ORIENTATION";
	case 0xAC: return "HORIZONTAL FREQUENCY";
	case 0xAE: return "VERTICAL FREQUENCY";
	case 0xB0: return "SETTINGS";
	case 0xB2: return "FLAT PANEL SUB-PIXEL LAYOUT";
	case 0xB4: return "SOURCE TIMING MODE";
	case 0xB6: return "DISPLAY TECHNOLOGY TYPE";
	case 0xB7: return "MONITOR STATUS";
	case 0xB8: return "PACKET COUNT";
	case 0xB9: return "MONITOR X ORIGIN";
	case 0xBA: return "MONITOR Y ORIGIN";
	case 0xBB: return "HEADER ERROR COUNT";
	case 0xBC: return "BODY CRC ERROR COUNT";
	case 0xBD: return "CLIENT ID";
	case 0xBE: return "LINK CONTROL";
	case 0xC0: return "DISPLAY USAGE TIME";
	case 0xC2: return "DISPLAY DESCRIPTOR LENGTH";
	case 0xC3: return "TRANSMIT DISPLAY DESCRIPTOR";
	case 0xC4: return "ENABLE DISPLAY OF 'DISPLAY DESCRIPTOR'";
	case 0xC6: return "APPLICATION ENABLE KEY";
	case 0xC8: return "DISPLAY CONTROLLER TYPE";
	case 0xC9: return "DISPLAY FIRMWARE LEVEL";
	case 0xCA: return "OSD/BUTTON CONTROL";
	case 0xCC: return "OSD LANGUAGE";
	case 0xCD: return "STATUS INDICATORS";
	case 0xCE: return "AUXILIARY DISPLAY SIZE";
	case 0xCF: return "AUXILIARY DISPLAY DATA";
	case 0xD0: return "OUTPUT SELECT";
	case 0xD2: return "ASSET TAG";
	case 0xD4: return "STEREO VIDEO MODE";
	case 0xD6: return "POWER MODE";
	case 0xD7: return "AUXILIARY POWER OUTPUT";
	case 0xDA: return "SCAN MODE";
	case 0xDB: return "IMAGE MODE";
	case 0xDC: return "DISPLAY MODE";
	case 0xDE: return "SCRATCH PAD";
	case 0xDF: return "VCP VERSION";
	default: return "UNKNOWN";
	}
}

constexpr const char* vcp_feature_desc(const BYTE vcp_code, const BYTE feature_code)
{
	switch (vcp_code)
	{
	case 0xB0:
		switch (feature_code)
		{
		case 0x01: return "STORE CURRENT SETTINGS";
		case 0x02: return "RESET FACTORY DEAFULTS";
		}
		break;
	case 0x13:
		switch (feature_code)
		{
		case 0x00: return "WHITE ADJUSTEMENT";
		case 0x01: return "RED ADJUSTEMENT";
		case 0x02: return "GREEN ADJUSTEMENT";
		case 0x03: return "BLUE ADJUSTEMENT";
		}
		break;
	case 0x14:
		switch (feature_code)
		{
		case 0x01: return "SRGB";
		case 0x02: return "DISPLAY NATIVE";
		case 0x03: return "4000K";
		case 0x04: return "5000K";
		case 0x05: return "6500K";
		case 0x06: return "7500K";
		case 0x07: return "8200K";
		case 0x08: return "9300K";
		case 0x09: return "10000K";
		case 0x0A: return "11500K";
		case 0x0B: return "USER 1";
		}
		break;
	case 0x1E:
		switch (feature_code)
		{
		case 0x00: return "AUTO SETUP IS NOT ACTIVE";
		case 0x01: return "PERFORM AUTO SETUP";
		case 0x02: return "ENABLE CONTINUOUS / PERIODIC AUTO SETUP";
		}
		break;
	case 0x1F:
		switch (feature_code)
		{
		case 0x00: return "AUTO COLOR SETUP IS NOT ACTIVE";
		case 0x01: return "PERFORM / PERFORMING AUTO COLOR SETUP";
		case 0x02: return "ENABLE CONTINUOUS / PERIODIC AUTO COLOR SETUP";
		}
		break;
	case 0x2E:
		switch (feature_code)
		{
		case 0x00: return "NO WHITE REGION EXPANSION";
		case 0x01: return "FIRST LEVEL EXPANSION";
		case 0x02: return "SECOND LEVEL EXPANSION";
		case 0x03: return "THIRD LEVEL EXPANSION";
		}
		break;
	case 0x59:
		if (feature_code > 0x7F)
		{
			return "CAUSE AN INCREASE IN RED SATURATION";
		}
		if (feature_code == 0x7F)
		{
			return "NOMINAL VALUE RED";
		}
		if (feature_code < 0x7F)
		{
			return "CAUSE A DECREASE IN RED SATURATION";
		}
		break;
	case 0x5A:
		if (feature_code > 0x7F)
		{
			return "CAUSE AN INCREASE IN YELLOW SATURATION";
		}
		if (feature_code == 0x7F)
		{
			return "NOMINAL VALUE YELLOW";
		}
		if (feature_code < 0x7F)
		{
			return "CAUSE A DECREASE IN YELLOW SATURATION";
		}
		break;
	case 0x5B:
		if (feature_code > 0x7F)
		{
			return "CAUSE AN INCREASE IN GREEN SATURATION";
		}
		if (feature_code == 0x7F)
		{
			return "NOMINAL VALUE GREEN";
		}
		if (feature_code < 0x7F)
		{
			return "CAUSE A DECREASE IN GREEN SATURATION";
		}
		break;
	case 0x5C:
		if (feature_code > 0x7F)
		{
			return "CAUSE AN INCREASE IN CYAN SATURATION";
		}
		if (feature_code == 0x7F)
		{
			return "NOMINAL VALUE CYAN";
		}
		if (feature_code < 0x7F)
		{
			return "CAUSE A DECREASE IN CYAN SATURATION";
		}
		break;
	case 0x5E:
		if (feature_code > 0x7F)
		{
			return "CAUSE AN INCREASE IN MAGENTA SATURATION";
		}
		if (feature_code == 0x7F)
		{
			return "NOMINAL VALUE MAGENTA";
		}
		if (feature_code < 0x7F)
		{
			return "CAUSE A DECREASE IN MAGENTA SATURATION";
		}
		break;
	case 0x9B:
		if (feature_code > 0x7F)
		{
			return "CAUSE AN INCREASE IN RED HUE";
		}
		if (feature_code == 0x7F)
		{
			return "NOMINAL VALUE RED HUE";
		}
		if (feature_code < 0x7F)
		{
			return "CAUSE A DECREASE IN RED HUE";
		}
		break;
	case 0x9C:
		if (feature_code > 0x7F)
		{
			return "CAUSE AN INCREASE IN YELLOW HUE";
		}
		if (feature_code == 0x7F)
		{
			return "NOMINAL VALUE YELLOW HUE";
		}
		if (feature_code < 0x7F)
		{
			return "CAUSE A DECREASE IN YELLOW HUE";
		}
		break;
	case 0x9D:
		if (feature_code > 0x7F)
		{
			return "CAUSE AN INCREASE IN GREEN HUE";
		}
		if (feature_code == 0x7F)
		{
			return "NOMINAL VALUE GREEN HUE";
		}
		if (feature_code < 0x7F)
		{
			return "CAUSE A DECREASE IN GREEN HUE";
		}
		break;
	case 0x9E:
		if (feature_code > 0x7F)
		{
			return "CAUSE AN INCREASE IN CYAN HUE";
		}
		if (feature_code == 0x7F)
		{
			return "NOMINAL VALUE CYAN HUE";
		}
		if (feature_code < 0x7F)
		{
			return "CAUSE A DECREASE IN CYAN HUE";
		}
		break;
	case 0x9F:
		if (feature_code > 0x7F)
		{
			return "CAUSE AN INCREASE IN BLUE HUE";
		}
		if (feature_code == 0x7F)
		{
			return "NOMINAL VALUE BLUE HUE";
		}
		if (feature_code < 0x7F)
		{
			return "CAUSE A DECREASE IN BLUE HUE";
		}
		break;
	case 0xA0:
		if (feature_code > 0x7F)
		{
			return "CAUSE AN INCREASE IN MAGENTA HUE";
		}
		if (feature_code == 0x7F)
		{
			return "NOMINAL VALUE MAGENTA HUE";
		}
		if (feature_code < 0x7F)
		{
			return "CAUSE A DECREASE IN MAGENTA HUE";
		}
		break;
	case 0xA2:
		switch (feature_code)
		{
		case 0x01: return "TURN AUTO SETUP OFF";
		case 0x02: return "TURN AUTO SETUP ON";
		}
		break;
	case 0xA5:
		switch (feature_code)
		{
		case 0x00: return "FULL DISPLAY IMAGE IS SELECTED";
		case 0x01: return "WINDOW 1 IS SELECTED";
		case 0x02: return "WINDOW 2 IS SELECTED";
		case 0x03: return "WINDOW 3 IS SELECTED";
		case 0x04: return "WINDOW 4 IS SELECTED";
		case 0x05: return "WINDOW 5 IS SELECTED";
		case 0x06: return "WINDOW 6 IS SELECTED";
		case 0x07: return "WINDOW 7 IS SELECTED";
		}
		break;
	case 0xAA:
		switch (feature_code)
		{
		case 0x01: return "0 DEGREES - NORMAL LANDSCAPE MODE";
		case 0x02: return "90 DEGREES - PORTRAIT MODE CLOCKWISE ROTATION BY 90 DEGREES";
		case 0x03: return "180 DEGREES - LANDSCAPE MODE ROTATION BY 180 DEGREES";
		case 0x04: return "270 DEGREES - PORTRAIT MODE CLOCKWISE ROTATION BY 270 DEGREES";
		case 0xFF: return "THE DISPLAY CANNOT SUPPLY THE CURRENT ORIENTATION";
		}
		break;
	case 0xDC:
		switch (feature_code)
		{
		case 0x00: return "DEFAULT MODE";
		case 0x01: return "PRODUCTIVITY";
		case 0x02: return "MIXED";
		case 0x03: return "MOVIE";
		case 0x04: return "USER DEFINED";
		case 0x05: return "GAMES";
		case 0x06: return "SPORTS";
		case 0x07: return "PROFESSIONAL";
		case 0x08: return "STANDARD WITH INTERMEDIATE POWER CONSUMPTION";
		case 0x09: return "STANDARD WITH LOW POWER CONSUMPTION";
		case 0x0A: return "DEMONSTRATION";
		case 0xF0: return "DYNAMIC CONTRAST";
		}
		break;
	case 0xC8:
		switch (feature_code)
		{
		case 0x01: return "CONEXANT";
		case 0x02: return "GENESIS MICROCHIP";
		case 0x03: return "MACRONIX";
		case 0x04: return "MRT (MEDIA REALITY TECHNOLOGIES)";
		case 0x05: return "MSTAR SEMICONDUCTOR";
		case 0x06: return "MYSON";
		case 0x07: return "PHILIPS";
		case 0x08: return "PIXELWORKS";
		case 0x09: return "REALTEK SEMICONDUCTOR";
		case 0x0A: return "SAGE";
		case 0x0B: return "SILICON IMAGE";
		case 0x0C: return "SMARTASIC";
		case 0x0D: return "STMICROELECTRONICS";
		case 0x0E: return "TOPRO";
		case 0x0F: return "TRUMPION";
		case 0x10: return "WELLTREND";
		case 0x11: return "SAMSUNG";
		case 0x12: return "NOVATEK MICROELECTRONICS";
		case 0x13: return "STK";
		}
		break;
	case 0xCA:
		switch (feature_code)
		{
		case 0x01: return "OSD IS DISABLED";
		case 0x02: return "OSD IS ENABLED";
		case 0xFF: return "THE DISPLAY CANNOT SUPPLY THIS INFORMATION";
		}
		break;
	case 0xCC:
		switch (feature_code)
		{
		case 0x01: return "CHINESE";
		case 0x02: return "ENGLISH";
		case 0x03: return "FRENCH";
		case 0x04: return "GERMAN";
		case 0x05: return "ITALIAN";
		case 0x06: return "JAPANESE";
		case 0x07: return "KOREAN";
		case 0x08: return "PORTOGUESE";
		case 0x09: return "RUSSIAN";
		case 0x0A: return "SPANISH";
		case 0x0B: return "SWEDISH";
		case 0x0C: return "TURKISH";
		case 0x0D: return "CHINESE (SIMPLIFIED)";
		case 0x0E: return "PORTOGUESE (BRAZIL)";
		case 0x0F: return "ARABIC";
		case 0x10: return "BULGARIAN";
		case 0x11: return "CROATIAN";
		case 0x12: return "CZECH";
		case 0x13: return "DANISH";
		case 0x14: return "DUTCH";
		case 0x15: return "ESTONIAN";
		case 0x16: return "FINNISH";
		case 0x17: return "GREEK";
		case 0x18: return "HEBREW";
		case 0x19: return "HINDI";
		case 0x1A: return "HUNGARIAN";
		case 0x1B: return "LATVIAN";
		case 0x1C: return "LITHUANIAN";
		case 0x1D: return "NORWEGIAN";
		case 0x1E: return "POLISH";
		case 0x1F: return "ROMANIAN";
		case 0x20: return "SERBIAN";
		case 0x21: return "SLOVAK";
		case 0x22: return "SLOVENIAN";
		case 0x23: return "THAI";
		case 0x24: return "UKRANIAN";
		case 0x25: return "VIETNAMESE";
		}
		break;
	case 0xD6:
		switch (feature_code)
		{
		case 0x01: return "ON";
		case 0x02: return "OFF";
		case 0x03: return "OFF OR SUSPEND";
		case 0x04: return "OFF";
		}
		break;
	}
	return "UNKNOWN OPTION";
}

int main()
{
	EnumDisplayMonitors(nullptr, nullptr, my_enum_proc, 0);
	std::cout << "Number of adapters: " << hmonitor_vec.size() << std::endl;
	if (hmonitor_vec.empty()) return 1;

	const auto selected_monitor_handle = hmonitor_vec.front();

	DWORD phys_mon_number;

	GetNumberOfPhysicalMonitorsFromHMONITOR(selected_monitor_handle, &phys_mon_number);

	auto* phys_monitor_array = static_cast<LPPHYSICAL_MONITOR>(malloc(
		phys_mon_number * sizeof(PHYSICAL_MONITOR)));

	if (phys_monitor_array == nullptr) return 1;

	GetPhysicalMonitorsFromHMONITOR(selected_monitor_handle, phys_mon_number, phys_monitor_array);

	DWORD capabilities_length;

	const auto selected_phys_mon = phys_monitor_array[0].hPhysicalMonitor;

	std::wcout << phys_monitor_array[0].szPhysicalMonitorDescription << std::endl;

	GetCapabilitiesStringLength(selected_phys_mon, &capabilities_length);

	auto* capabilities_str = static_cast<LPSTR>(malloc(capabilities_length));

	if (capabilities_str == nullptr) return 1;

	CapabilitiesRequestAndCapabilitiesReply(selected_phys_mon, capabilities_str, capabilities_length);

	std::cout << capabilities_str << std::endl;

	std::regex reg(R"((vcp\()(.*?)\)mccs)");
	std::cmatch matches;

	std::regex_search(capabilities_str, matches, reg);

	std::string vcps(matches[2]);
	std::smatch second_matches;

	reg = R"(([\w\d]+)(\((.+?)\))*)";

	std::vector<std::tuple<BYTE, std::vector<BYTE>>> vcp_codes;


	while (std::regex_search(vcps, second_matches, reg))
	{
		std::vector<BYTE> sec;
		if (second_matches[3].matched)
		{
			std::regex third_reg(R"(\S+)");
			std::string third_str(second_matches[3]);
			std::smatch third_matches;

			while (std::regex_search(third_str, third_matches, third_reg))
			{
				sec.push_back(static_cast<BYTE>(std::stoi(third_matches[0], nullptr, 16)));
				third_str = third_matches.suffix();
			}
		}
		vcp_codes.emplace_back(std::stoi(second_matches[1], nullptr, 16), sec);
		vcps = second_matches.suffix();
	}

	for (const auto& vcp_code : vcp_codes)
	{
		std::ios state(nullptr);
		state.copyfmt(std::cout);

		std::cout << "VCP code: " << std::uppercase << std::hex << std::setw(2) << std::setfill('0') <<
			static_cast<unsigned int>(std::get<0>(vcp_code)) << " -- Description: " << vcp_code_desc(
				std::get<0>(vcp_code)) << " -- ";
		std::cout.copyfmt(state);
		std::cout << "Features: ";
		for (auto feature : std::get<1>(vcp_code))
		{
			std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(
				feature) << ": \"" << vcp_feature_desc(std::get<0>(vcp_code), feature) << "\"  ";
		}
		std::cout << std::endl;
		std::cout.copyfmt(state);
	}

	BYTE chosen_vcp_code;

	while (true)
	{
		std::string inp_str;
		std::cout << "Choose a vcp code: ";
		std::cin >> inp_str;
		if (inp_str == "-1")
		{
			break;
		}
		chosen_vcp_code = static_cast<BYTE>(std::stoi(inp_str, nullptr, 16));


		if (!std::any_of(vcp_codes.begin(), vcp_codes.end(), [chosen_vcp_code](std::tuple<BYTE, std::vector<BYTE>> elem)
		{
			return std::get<0>(elem) == chosen_vcp_code;
		}))
		{
			std::cout << "Not a valid vcp code!" << std::endl;
			continue;
		}

		MC_VCP_CODE_TYPE code_type;
		DWORD current_value;
		DWORD maximum_value;

		GetVCPFeatureAndVCPFeatureReply(selected_phys_mon, chosen_vcp_code, &code_type, &current_value, &maximum_value);

		std::ios state(nullptr);
		state.copyfmt(std::cout);
		std::cout << "Code type: " << vcp_enum_to_string(code_type) << std::endl;
		std::cout << "Current value: " << current_value << " - hex: " << std::hex << std::setfill('0') << std::setw(2)
			<< current_value << " - " <<
			vcp_feature_desc(chosen_vcp_code, static_cast<BYTE>(current_value)) << std::endl;
		std::cout.copyfmt(state);
		std::cout << "Maximum value: " << maximum_value << " - hex: " << std::hex << std::setfill('0') << std::setw(2)
			<< maximum_value << std::endl;
		std::cout.copyfmt(state);
	}

	DestroyPhysicalMonitors(phys_mon_number, phys_monitor_array);

	free(capabilities_str);

	free(phys_monitor_array);

	return 0;
}

BOOL CALLBACK my_enum_proc(HMONITOR hmonitor, HDC hdc, LPRECT lprect, LPARAM lparam)
{
	hmonitor_vec.push_back(hmonitor);
	return TRUE;
}
