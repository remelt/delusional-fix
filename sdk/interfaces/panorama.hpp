#pragma once
#include <array>

template <typename T>
struct utl_vector_t
{
    T* mem;
    int alloc_count;
    int grow_size;
    int size;
    T* els;
};

class c_ui_panel {
public:
    const char* get_panel_id() {
        using original_fn = const char* (__thiscall*)(c_ui_panel*);
        return (*(original_fn**)this)[9](this);
    }

    c_ui_panel* get_parent() {
        using original_fn = c_ui_panel * (__thiscall*)(c_ui_panel*);
        return (*(original_fn**)this)[25](this);
    }

    void set_visible(bool visible) {
        using original_fn = void(__thiscall*)(c_ui_panel*, bool);
        return (*(original_fn**)this)[27](this, visible);
    }

    c_ui_panel* find_child_traverse(const char* name) {
        using original_fn = c_ui_panel * (__thiscall*)(c_ui_panel*, const char*);
        return (*(original_fn**)this)[40](this, name);
    }

    void find_children_with_class_traverse(const char* class_name, utl_vector_t<c_ui_panel*>* out)
    {
        return ((void(__thiscall*)(c_ui_panel*, const char*, utl_vector_t<c_ui_panel*> *))((*(uintptr_t**)(this))[302]))(this, class_name, out);
    }
};

class c_ui_engine {
public:
    bool is_valid_panel_pointer(c_ui_panel const* panel)
    {
        return ((bool(__thiscall*)(c_ui_engine*, c_ui_panel const*))((*(uintptr_t**)(this))[36]))(this, panel);
    }

    c_ui_panel* get_last_dispatched_event_target_panel()
    {
        return ((c_ui_panel * (__thiscall*)(c_ui_engine*))((*(uintptr_t**)(this))[56]))(this);
    }

    void run_script(c_ui_panel* panel, const char* js, const char* schema_path, int a5 = 8, int a6 = 10, bool a7 = false, bool a8 = false)
    {
        return ((void(__thiscall*)(c_ui_engine*, c_ui_panel*, const char*, const char*, int, int, bool, bool))((*(uintptr_t**)(this))[113]))(this, panel, js, schema_path, a5, a6, a7, a8);
    }
};

class i_panoramaui_engine {
public:
    c_ui_engine* access_ui_engine()
    {
        return ((c_ui_engine * (__thiscall*)(i_panoramaui_engine*))((*(uintptr_t**)(this))[11]))(this);
    }
};