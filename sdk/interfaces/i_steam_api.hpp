#pragma once
#include <cstdint>
#include "../../utils/steamtypes.h"

using h_steam_pipe = std::int32_t;
using h_steam_user = std::int32_t;
using h_steam_api_call = std::int64_t;

class i_steam_client;
class i_steam_user;
class i_steam_friends;
class i_steam_utils;

struct steam_api_context_t
{
	i_steam_client* steam_client;
	i_steam_user* steam_user;
	i_steam_friends* steam_friends;
	i_steam_utils* steam_utils;
};

enum e_account_type
{
	account_type_invalid = 0,
	account_type_individual = 1,		// single user account
	account_type_multiseat = 2,		// multiseat (e.g. cybercafe) account
	account_type_game_server = 3,		// game server account
	account_type_anon_game_server = 4,	// anonymous game server account
	account_type_pending = 5,		// pending
	account_type_content_server = 6,	// content server
	account_type_clan = 7,
	account_type_chat = 8,
	account_type_console_user = 9,	// Fake SteamID for local PSN account on PS3 or Live account on 360, etc.
	account_type_anon_user = 10,
	account_type_max
};


using steam_api_warning_message_hook_t = void(__cdecl*)(int, const char*);
using steam_api_post_api_result_in_process_t = void(__cdecl*)(std::int64_t call, void*, std::uint32_t callback_size, int callbacks);
using steam_api_check_callback_registered_t = std::uint32_t(__cdecl*)(int callbacks);

struct steam_api_call_t;
enum e_persona_state;
enum e_friend_relationship;
struct friend_game_info_t;
struct friends_group_id_t;

class i_steam_friends
{
private:
	// returns the local players name - guaranteed to not be NULL.
	// this is the same name as on the users community profile page
	// this is stored in UTF-8 format
	// like all the other interface functions that return a char *, it's important that this pointer is not saved
	// off; it will eventually be free'd or re-allocated
	virtual const char* unused0() = 0;

	// Sets the player name, stores it on the server and publishes the changes to all friends who are online.
	// Changes take place locally immediately, and a PersonaStateChange_t is posted, presuming success.
	//
	// The final results are available through the return value SteamAPICall_t, using SetPersonaNameResponse_t.
	//
	// If the name change fails to happen on the server, then an additional global PersonaStateChange_t will be posted
	// to change the name back, in addition to the SetPersonaNameResponse_t callback.
	virtual void unused1() = 0;

	// gets the status of the current user
	virtual void unused2() = 0;

	// friend iteration
	// takes a set of k_EFriendFlags, and returns the number of users the client knows about who meet that criteria
	// then GetFriendByIndex() can then be used to return the id's of each of those users
	virtual int unused3() = 0;

	// returns the steamID of a user
	// iFriend is a index of range [0, GetFriendCount())
	// iFriendsFlags must be the same value as used in GetFriendCount()
	// the returned CSteamID can then be used by all the functions below to access details about the user
	virtual void unused4() = 0;

	// returns a relationship to a user
	virtual void unused5() = 0;

	// returns the current status of the specified user
	// this will only be known by the local user if steamIDFriend is in their friends list; on the same game server; in a chat room or lobby; or in a small group with the local user
	virtual void unused6() = 0;

	// returns the name another user - guaranteed to not be NULL.
	// same rules as GetFriendPersonaState() apply as to whether or not the user knowns the name of the other user
	// note that on first joining a lobby, chat room or game server the local user will not known the name of the other users automatically; that information will arrive asyncronously
	// 
	virtual const char* unused7() = 0;

	// returns true if the friend is actually in a game, and fills in pFriendGameInfo with an extra details 
	virtual bool unused8() = 0;
	// accesses old friends names - returns an empty string when their are no more items in the history
	virtual const char* unused9() = 0;
	// friends steam level
	virtual int unused10() = 0;

	// Returns nickname the current user has set for the specified player. Returns NULL if the no nickname has been set for that player.
	virtual const char* unused11() = 0;

	// friend grouping (tag) apis
	// returns the number of friends groups
	virtual int unused12() = 0;
	// returns the friends group ID for the given index (invalid indices return k_FriendsGroupID_Invalid)
	virtual void unused13() = 0;
	// returns the name for the given friends group (NULL in the case of invalid friends group IDs)
	virtual const char* unused14() = 0;
	// returns the number of members in a given friends group
	virtual int unused15() = 0;
	// gets up to nMembersCount members of the given friends group, if fewer exist than requested those positions' SteamIDs will be invalid
	virtual void unused16() = 0;

	// returns true if the specified user meets any of the criteria specified in iFriendFlags
	// iFriendFlags can be the union (binary or, |) of one or more k_EFriendFlags values
	virtual bool unused17() = 0;

	// clan (group) iteration and access functions
	virtual int unused18() = 0;
	virtual void unused19() = 0;
	virtual const char* unused20() = 0;
	virtual const char* unused21() = 0;
	// returns the most recent information we have about what's happening in a clan
	virtual bool unused22() = 0;
	// for clans a user is a member of, they will have reasonably up-to-date information, but for others you'll have to download the info to have the latest
	virtual void unused23() = 0;

	// iterators for getting users in a chat room, lobby, game server or clan
	// note that large clans that cannot be iterated by the local user
	// note that the current user must be in a lobby to retrieve CSteamIDs of other users in that lobby
	// steamIDSource can be the steamID of a group, game server, lobby or chat room
	virtual int unused24() = 0;
	virtual void unused25() = 0;

	// returns true if the local user can see that steamIDUser is a member or in steamIDSource
	virtual bool  unused26() = 0;

	// User is in a game pressing the talk button (will suppress the microphone for all voice comms from the Steam friends UI)
	virtual void  unused27() = 0;

	// activates the game overlay, with an optional dialog to open 
	// valid options are "Friends", "Community", "Players", "Settings", "OfficialGameGroup", "Stats", "Achievements"
	virtual void  unused28() = 0;

	// activates game overlay to a specific place
	// valid options are
	//		"steamid" - opens the overlay web browser to the specified user or groups profile
	//		"chat" - opens a chat window to the specified user, or joins the group chat 
	//		"jointrade" - opens a window to a Steam Trading session that was started with the ISteamEconomy/StartTrade Web API
	//		"stats" - opens the overlay web browser to the specified user's stats
	//		"achievements" - opens the overlay web browser to the specified user's achievements
	//		"friendadd" - opens the overlay in minimal mode prompting the user to add the target user as a friend
	//		"friendremove" - opens the overlay in minimal mode prompting the user to remove the target friend
	//		"friendrequestaccept" - opens the overlay in minimal mode prompting the user to accept an incoming friend invite
	//		"friendrequestignore" - opens the overlay in minimal mode prompting the user to ignore an incoming friend invite
	virtual void  unused29() = 0;

	// activates game overlay web browser directly to the specified URL
	// full address with protocol type is required, e.g. http://www.steamgames.com/
	virtual void  unused30() = 0;

	// activates game overlay to store page for app
	virtual void unused31() = 0;

	// Mark a target user as 'played with'. This is a client-side only feature that requires that the calling user is 
	// in game 
	virtual void unused32() = 0;

	// activates game overlay to open the invite dialog. Invitations will be sent for the provided lobby.
	virtual void unused33() = 0;
public:
	// gets the small (32x32) avatar of the current user, which is a handle to be used in IClientUtils::GetImageRGBA(), or 0 if none set
	virtual int get_small_friend_avatar(std::uint64_t steam_id_friend) = 0;
	// gets the small (64x64) avatar of the current user, which is a handle to be used in IClientUtils::GetImageRGBA(), or 0 if none set
	virtual int get_medium_friend_avatar(std::uint64_t steam_id_friend) = 0;
	// gets the small (128x128) avatar of the current user, which is a handle to be used in IClientUtils::GetImageRGBA(), or 0 if none set
	virtual int get_large_friend_avatar(std::uint64_t steam_id_friend) = 0;

};

class i_steam_user
{
public:
	// returns the h_steam_user this interface represents
	// this is only used internally by the API, and by a few select interfaces that support multi-user
	virtual h_steam_user get_h_steam_user() = 0;

	// returns true if the Steam client current has a live connection to the Steam servers. 
	// If false, it means there is no active connection due to either a networking issue on the local machine, or the Steam server is down/busy.
	// The Steam client will automatically be trying to recreate the connection as often as possible.
	virtual bool logged_on() = 0;

	// returns the CSteamID of the account currently logged into the Steam client
	// a CSteamId is a unique identifier for an account, and used to differentiate users in all parts of the Steamworks API
	virtual std::uint64_t get_steam_id() = 0;
};


class i_steam_utils
{
private:
	virtual void fn01() = 0;
	virtual void fn02() = 0;
	virtual void fn03() = 0;
	virtual void fn04() = 0;
	virtual void fn05() = 0;
public:
	// returns true if the image exists, and valid sizes were filled out
	virtual bool get_image_size(int image, uint32* width, uint32* height) = 0;
	virtual bool get_image_rgba(int image, uint8* ub_dest, int dest_buffer_size) = 0;
};

class i_steam_client
{
public:
	virtual h_steam_pipe create_steam_pipe() = 0;
	virtual bool release_steam_pipe(h_steam_pipe steam_pipe) = 0;
	virtual h_steam_user connect_to_global_user(h_steam_pipe steam_pipe) = 0;
	virtual h_steam_user create_local_user(h_steam_pipe* psteam_pipe, e_account_type account_type) = 0;
	virtual void release_user(h_steam_pipe steam_pipe, h_steam_user hUser) = 0;
	virtual i_steam_user* get_i_steam_user(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_game_server(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void set_local_ip_binding(uint32_t unIP, uint16_t usPort) = 0;
	virtual i_steam_friends* get_i_steam_friends(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual i_steam_utils* get_i_steam_utils(h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_matchmaking(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_matchmaking_servers(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_generic_interface(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_user_stats(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_game_server_stats(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_apps(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_networking(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_remote_storage(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_screenshots(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void run_frame() = 0;
	virtual uint32_t get_ipc_call_count() = 0;
	virtual void set_warning_message_hook(steam_api_warning_message_hook_t pFunction) = 0;
	virtual bool shutdown_if_all_pipes_closed() = 0;
	virtual void* get_i_steam_http(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_unified_messages(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_controller(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_ugc(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_app_list(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_music(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_music_remote(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_html_surface(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void set_steam_api_c_post_api_result_in_process(steam_api_post_api_result_in_process_t func) = 0;
	virtual void remove_steam_api_c_post_api_result_in_process(steam_api_post_api_result_in_process_t func) = 0;
	virtual void set_steam_api_c_check_callback_registered_in_process(steam_api_check_callback_registered_t func) = 0;
	virtual void* get_i_steam_inventory(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
	virtual void* get_i_steam_video(h_steam_user steam_user_handle, h_steam_pipe steam_pipe, const char* version) = 0;
};