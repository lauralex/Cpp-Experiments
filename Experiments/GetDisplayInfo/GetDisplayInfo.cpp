#include "framework.h"
#include <iostream>
#include <regex>
#include <tuple>
#include <vector>
#include <iomanip>
#include <ostream>


BOOL CALLBACK my_enum_proc(HMONITOR, HDC, LPRECT, LPARAM);

std::vector<HMONITOR> hmonitor_vec;

const char* vcp_enum_to_string(const MC_VCP_CODE_TYPE e)
{
	switch (e)
	{
	case MC_MOMENTARY: return "MC_MOMENTARY";
	case MC_SET_PARAMETER: return "MC_SET_PARAMETER";
	}
	return "UNKNOWN";
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
			static_cast<unsigned int>(std::get<0>(vcp_code)) << " -- ";
		std::cout.copyfmt(state);
		std::cout << "Features: ";
		for (auto feature : std::get<1>(vcp_code))
		{
			std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(
				feature) << " ";
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

		std::cout << "Code type: " << vcp_enum_to_string(code_type) << std::endl;
		std::cout << "Current value: " << current_value << std::endl;
		std::cout << "Maximum value: " << maximum_value << std::endl;
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
