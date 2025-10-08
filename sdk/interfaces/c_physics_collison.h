#pragma once

struct polyhedron_t;
struct convertconvexparams_t;
struct truncatedcone_t;
struct vcollide_t;
class vec3_t;


class c_physics_collison
{
public:
	virtual ~c_physics_collison(void) {}

	// produce a convex element from verts (convex hull around verts)
	virtual void* convex_from_verts(vec3_t** p_verts, int vert_count) = 0;
	// produce a convex element from planes (csg of planes)
	virtual void* convex_from_planes(float* p_planes, int plane_count, float merge_distance) = 0;
	// calculate volume of a convex element
	virtual float convex_volume(void* p_convex) = 0;

	virtual float convex_surface_area(void* p_convex) = 0;
	// store game-specific data in a convex solid
	virtual void set_convex_game_data(void* p_convex, unsigned int game_data) = 0;
	// if not converted, free the convex elements with this call
	virtual void convex_free(void* p_convex) = 0;
	virtual void* b_box_to_convex(const vec3_t& mins, const vec3_t& maxs) = 0;
	// produce a convex element from a convex polyhedron_t
	virtual void* convex_from_convexpolyhedron_t(const polyhedron_t& convexpolyhedron_t) = 0;
	// produce a set of convex triangles from a convex polygon, normal is assumed to be on the side with forward point ordering, which should be
	// clockwise, output will need to be able to hold exactly (i_point_count-2) convexes
	virtual void convexes_from_convex_polygon(const vec3_t& v_poly_normal, const vec3_t* p_points, int i_point_count, void** p_output) = 0;

	// concave objects
	// create a triangle soup
	virtual void* polysoup_create(void) = 0;
	// destroy the container and memory
	virtual void polysoup_destroy(void* p_soup) = 0;
	// add a triangle to the soup
	virtual void polysoup_add_triangle(void* p_soup, const vec3_t& a, const vec3_t& b, const vec3_t& c, int material_index7bits) = 0;
	// convert the convex into a compiled collision model
	virtual void* convert_polysoup_to_collide(void* p_soup, bool use_mopp) = 0;

	// convert an array of convex elements to a compiled collision model (this deletes the convex elements)
	virtual void* convert_convex_to_collide(void** p_convex, int convex_count) = 0;
	virtual void* convert_convex_to_collide_params(void** p_convex, int convex_count, const convertconvexparams_t& convert_params) = 0;
	// free a collide that was created with convert_convex_to_collide()
	virtual void destroy_collide(void* p_collide) = 0;

	// get the memory size in bytes of the collision model for serialization
	virtual int collide_size(void* p_collide) = 0;
	// serialize the collide to a block of memory
	virtual int collide_write(char* p_dest, void* p_collide, bool b_swap = false) = 0;
	// unserialize the collide from a block of memory
	virtual void* unserialize_collide(char* p_buffer, int size, int index) = 0;

	// compute the volume of a collide
	virtual float collide_volume(void* p_collide) = 0;
	// compute surface area for tools
	virtual float collide_surface_area(void* p_collide) = 0;

	// get the support map for a collide in the given direction
	virtual vec3_t collide_get_extent(const void* p_collide, const vec3_t& collide_origin, const vec3_t& collide_angles,
		const vec3_t& direction) = 0;

	// get an aabb for an oriented collision model
	virtual void collide_get_aabb(vec3_t* p_mins, vec3_t* p_maxs, const void* p_collide, const vec3_t& collide_origin,
		const vec3_t& collide_angles) = 0;

	virtual void collide_get_mass_center(void* p_collide, vec3_t* p_out_mass_center) = 0;
	virtual void collide_set_mass_center(void* p_collide, const vec3_t& mass_center) = 0;
	// get the approximate cross-sectional area projected orthographically on the bbox of the collide
	// note: these are fractional areas - unitless.  basically this is the fraction of the obb on each axis that
	// would be visible if the object were rendered orthographically.
	// note: this has been precomputed when the collide was built or this function will return 1,1,1
	virtual vec3_t collide_get_orthographic_areas(const void* p_collide) = 0;
	virtual void collide_set_orthographic_areas(void* p_collide, const vec3_t& areas) = 0;

	// query the vcollide index in the physics model for the instance
	virtual int collide_index(const void* p_collide) = 0;

	// convert a bbox to a collide
	virtual void* b_box_to_collide(const vec3_t& mins, const vec3_t& maxs) = 0;
	virtual int get_convexes_used_in_collideable(const void* p_collideable, void** p_output_array, int i_output_array_limit) = 0;

	// trace an aabb against a collide
	virtual void pad_1() = 0;
	virtual void pad_2() = 0;
	virtual void pad_3() = 0;

	// trace one collide against another
	virtual void trace_collide(const vec3_t& start, const vec3_t& end, const void* p_sweep_collide, const vec3_t& sweep_angles,
		const void* p_collide, const vec3_t& collide_origin, const vec3_t& collide_angles, void* ptr) = 0;

	// relatively slow test for box vs. truncated cone
	virtual bool is_box_intersecting_cone(const vec3_t& box_abs_mins, const vec3_t& box_abs_maxs, const truncatedcone_t& cone) = 0;

	// loads a set of solids into a vcollide_t
	virtual void v_collide_load(vcollide_t* p_output, int solid_count, const char* p_buffer, int size, bool swap = false) = 0;
	// destroyts the set of solids created by v_collide_load
	virtual void v_collide_unload(vcollide_t* p_v_collide) = 0;
};