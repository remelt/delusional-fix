#pragma once

struct vcollide_t {
	unsigned short m_solid_count : 15;
	unsigned short m_is_packed : 1;
	unsigned short m_desc_size;
	void** m_solids;
	char* m_key_values;
	void* m_user_data;
};