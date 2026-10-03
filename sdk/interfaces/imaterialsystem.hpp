#pragma once
#include "../structs/materials.hpp"

#define TEXTURE_GROUP_LIGHTMAP						        "Lightmaps"
#define TEXTURE_GROUP_WORLD							          "World textures"
#define TEXTURE_GROUP_MODEL							          "Model textures"
#define TEXTURE_GROUP_VGUI							          "VGUI textures"
#define TEXTURE_GROUP_PARTICLE						        "Particle textures"
#define TEXTURE_GROUP_DECAL							          "Decal textures"
#define TEXTURE_GROUP_SKYBOX						          "SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				      "ClientEffect textures"
#define TEXTURE_GROUP_OTHER							          "Other textures"
#define TEXTURE_GROUP_PRECACHED						        "Precached"
#define TEXTURE_GROUP_CUBE_MAP						        "CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					      "RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED					        "Unaccounted textures"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			    "Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			  "Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			  "Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					      "DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					        "ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					      "Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				      "Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			  "RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					      "Morph Targets"

class matrix3x4_t;
class i_material_render_context
{
public:
	void get_render_target_dimensions(int* width, int* height)
	{
		using fn = void(__thiscall*)(i_material_render_context*, int*, int*);
		return (*(fn**)this)[8](this, width, height);
	}

	void draw_screen_space_rectangle(i_material* material, int dest_x, int dest_y, int width, int height, float texture_x0, float texture_y0,
		float texture_x1, float texture_y1, int texture_width, int texture_height, void* client_renderable = nullptr,
		int x_dice = 1, int y_dice = 1)
	{
		using fn = void(__thiscall*)(i_material_render_context*, i_material*, int, int, int, int, float, float, float, float, int, int, void*, int, int);
		return (*(fn**)this)[114](this, material, dest_x, dest_y, width, height, texture_x0, texture_y0,
			texture_x1, texture_y1, texture_width, texture_height, client_renderable,
			x_dice, y_dice);
	}

	void get_view_port(int* x, int* y, int* width, int* height)
	{
		using fn = void(__thiscall*)(i_material_render_context*, int*, int*, int*, int*);
		return (*(fn**)this)[41](this, x, y, width, height);
	}

	void copy_render_target_to_texture_ex(i_texture* texture, int unk, rect_t* dst, rect_t* src)
	{
		using fn = void(__thiscall*)(i_material_render_context*, i_texture*, int, rect_t*, rect_t*);
		return (*(fn**)this)[122](this, texture, unk, dst, src);
	}

	void set_frame_buffer_copy_texture(i_texture* texture, int texture_index)
	{
		using fn = void(__thiscall*)(i_material_render_context*, i_texture*, int);
		return (*(fn**)this)[20](this, texture, texture_index);
	}
};

enum {
	MATERIAL_ADAPTER_NAME_LENGTH = 1 << 9
};

struct material_adapter_info_t {
	char driver_name[MATERIAL_ADAPTER_NAME_LENGTH];
	unsigned int vendor_id;
	unsigned int device_id;
	unsigned int sub_sys_id;
	unsigned int revision;
	int dx_support_level;
	int min_dx_support_level;
	int max_dx_support_level;
	unsigned int driver_version_high;
	unsigned int driver_version_low;
};

class i_material_system {
public:
	i_material* find_material(char const* material_name, const char* group_name, bool complain = true, const char* complain_prefix = 0) {
		using fn = i_material * (__thiscall*)(i_material_system*, char const*, const char*, bool, const char*);
		return (*(fn**)this)[84](this, material_name, group_name, complain, complain_prefix);
	}
	i_texture* find_texture(char const* texture_name, const char* texture_group_name, bool complain = true, int additional_creation_flags = 0)
	{
		using fn = i_texture*(__thiscall*)(i_material_system*, char const*, const char*, bool, int);
		return (*(fn**)this)[91](this, texture_name, texture_group_name, complain, additional_creation_flags);
	}
	i_material* create_material(char const* material_name, c_key_values* kv) {
		using fn = i_material * (__thiscall*)(i_material_system*, char const*, c_key_values*);
		return (*(fn**)this)[83](this, material_name, kv);
	}
	material_handle_t first_material() {
		using fn = material_handle_t(__thiscall*)(i_material_system*);
		return (*(fn**)this)[86](this);
	}
	material_handle_t next_material(material_handle_t handle) {
		using fn = material_handle_t(__thiscall*)(i_material_system*, material_handle_t);
		return (*(fn**)this)[87](this, handle);
	}
	material_handle_t invalid_material_handle() {
		using fn = material_handle_t(__thiscall*)(i_material_system*);
		return (*(fn**)this)[88](this);
	}
	i_material* get_material(material_handle_t handle) {
		using fn = i_material * (__thiscall*)(i_material_system*, material_handle_t);
		return (*(fn**)this)[89](this, handle);
	}
	int	get_materials_count() {
		using fn = int(__thiscall*)(i_material_system*);
		return (*(fn**)this)[90](this);
	}
	int get_current_adapter() {
		using fn = int(__thiscall*)(void*);
		return (*(fn**)this)[25](this);
	}
	void get_display_adapter_info(int adapter, material_adapter_info_t* adapter_info) {
		using fn = void(__thiscall*)(void*, int, material_adapter_info_t*);
		return (*(fn**)this)[26](this, adapter, adapter_info);
	}
	i_material_render_context* get_render_context() {
		using fn = i_material_render_context*(__thiscall*)(i_material_system*);
		return (*(fn**)this)[115](this);
	}
};