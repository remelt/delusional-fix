#pragma once
#include <cstdint>
#include "../math/vec3.hpp"

#pragma pack(push, 1)

class i_net_channel {
public:
	uint8_t pad_0x0000[0x17];
	bool should_delete;
	int out_sequence_nr;
	int in_sequence_nr;
	int out_sequence_nr_ack;
	int out_reliable_state;
	int in_reliable_state;
	int choked_packets;
};

class c_event_info
{
public:
	enum {
		EVENT_INDEX_BITS = 8,
		EVENT_DATA_LEN_BITS = 11,
		MAX_EVENT_DATA = 192,
	};

	short m_class_id;
	PAD(0x2);
	float m_fire_delay;
	const void* m_send_table;
	const void* m_client_class;
	int m_packed;
	int m_flags;
	int m_filter[8];
	c_event_info* m_next;
};

class i_client_state
{
public:
	unsigned char pad0[0x9c];
	i_net_channel* net_channel;
	int m_challenge_nr;
	unsigned char pad1[0x64];
	int signon_state_count;
	unsigned char pad2[0x8];
	float next_cmd_time;
	int server_count;
	int current_sequence;
	unsigned char pad3[0x54];
	int delta_tick;
	bool paused;
	unsigned char pad4[0x7];
	int view_entity;
	int player_slot;
	char level_name[260 /* MAX_PATH */];
	char level_name_short[80];
	char map_group_name[80];
	char mast_level_name_short[80];
	unsigned char pad5[0xc];
	int max_clients;
	unsigned char pad6[0x498c];
	float last_server_tick_time;
	bool in_simulation;
	unsigned char pad7[0x3];
	int old_tickcount;
	float tick_remainder;
	float frame_time;
	int last_outgoing_command;
	int choked_commands;
	int last_command_ack;
	int command_ack;
	int sound_sequence;
	unsigned char pad8[0x50];
	vec3_t view_angles;
	unsigned char pad9[0xd0];
	c_event_info* m_events;
};

#pragma pack(pop)