#pragma once
#include "../../../sdk/sdk.hpp"

namespace features::drawmodel {
    void instance();
    itexture* get_texture();
    void set_chams_settings(C_ChamsSettings settings);
    void set_glow(int glow);
    void set_glow_color(Color glow);

    namespace detail
    {
        ITexture* preview_texture = nullptr;
        ITexture* cubemap_texture = nullptr;
        C_MergedMDL* preview_model = nullptr;
        C_ViewSetup view_setup = { };
        C_ChamsSettings chams_settings = { };
        int32_t glow = 0;
        Color glow_color = Color(255, 255, 255, 255);
    }
}
