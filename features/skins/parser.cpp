#include "skins.hpp"

void* get_export(const char* module_name, const char* export_name) {
	HMODULE mod;
	while (!((mod = GetModuleHandleA(module_name))))
		Sleep(100);
	return reinterpret_cast<void*>(GetProcAddress(mod, export_name));
}

void features::skins::init_parser() {
	std::ifstream items{ "csgo/scripts/items/items_game_cdn.txt" };
	const std::string gameItems{ std::istreambuf_iterator<char>{ items }, std::istreambuf_iterator<char>{ } };
	items.close();
	const auto V_UCS2ToUTF8 = static_cast<int(*)(const wchar_t* ucs2, char* utf8, int len)>(get_export("vstdlib.dll", "V_UCS2ToUTF8"));
	const auto sig_address = find_pattern("client.dll", "E8 ? ? ? ? FF 76 0C 8D 48 04 E8");
	const auto item_system_offset = *reinterpret_cast<std::int32_t*>(sig_address + 1);
	const auto item_system_fn = reinterpret_cast<CCStrike15ItemSystem * (*)()>(sig_address + 5 + item_system_offset);
	const auto item_schema = reinterpret_cast<CCStrike15ItemSchema*>(std::uint32_t(item_system_fn()) + sizeof(void*));
	{
		const auto get_paint_kit_definition_offset = *reinterpret_cast<std::int32_t*>(sig_address + 11 + 1);
		const auto get_paint_kit_definition_fn = reinterpret_cast<paint_kit_t * (__thiscall*)(CCStrike15ItemSchema*, int)>(sig_address + 11 + 5 + get_paint_kit_definition_offset);
		const auto start_element_offset = *reinterpret_cast<std::int32_t*>(std::uint32_t(get_paint_kit_definition_fn) + 8 + 2);
		const auto head_offset = start_element_offset - 12;
		const auto map_head = reinterpret_cast<head_t<int, paint_kit_t*>*>(std::uint32_t(item_schema) + head_offset);
		for (auto i = 0; i <= map_head->last_element; ++i) {
			const auto paint_kit = map_head->memory[i].value;
			if (paint_kit->id == 9001)
				continue;
			const auto wide_name = interfaces::localize->find(paint_kit->item_name.buffer + 1);
			char name[256];

			V_UCS2ToUTF8(wide_name, name, sizeof(name));

			const int itemNameLength = WideCharToMultiByte(CP_UTF8, 0, wide_name, -1, nullptr, 0, nullptr, nullptr);
			if (std::string name(itemNameLength, 0); WideCharToMultiByte(CP_UTF8, 0, wide_name, -1, &name[0], itemNameLength, nullptr, nullptr)) {
				if (paint_kit->id < 10000) {

					if (auto pos = gameItems.find('_' + std::string{ paint_kit->name.buffer } + '='); pos != std::string::npos && gameItems.substr(pos + paint_kit->name.length).find('_' + std::string{ paint_kit->name.buffer } + '=') == std::string::npos) {
						if (auto weaponName = gameItems.rfind("weapon_", pos); weaponName != std::string::npos) {
							name.back() = ' ';
							name += '(' + gameItems.substr(weaponName + 7, pos - weaponName - 7) + ')';
						}
					}
					parser_skins.push_back({ paint_kit->id, name, paint_kit });
				}
				else {

					parser_gloves.push_back({ paint_kit->id, name, paint_kit });

				}
			}

			std::sort(parser_skins.begin(), parser_skins.end());
			std::sort(parser_gloves.begin(), parser_gloves.end());

		}

	}

	printf("delusional | kit parser initialized\n");
}