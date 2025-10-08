#include "visuals.hpp"
#include "vcollide_t.h"

static vcollide_t precipitation_collideable{};
int m_timer = -1;

void features::weather::reset_weather(const bool cleanup) {
    created_rain = false;

    if (rain_entity && cleanup) {
        rain_entity->net_pre_data_update(0);
        rain_entity->pre_data_change(0);

        *(int*)((uintptr_t)rain_entity + 0xA00) = -1;

        rain_entity->collideable()->mins() = { 0, 0, 0 };
        rain_entity->collideable()->maxs() = { 0, 0, 0 };

        interfaces::physics_collision->v_collide_unload(&precipitation_collideable);

        rain_entity->on_data_changed(0);
        rain_entity->post_data_update(0);

        if (rain_entity->networkable())
            rain_entity->net_release();
    }

    rain_entity = nullptr;
    m_timer = -1;
}

void* features::weather::getv_collideble()
{
    return &precipitation_collideable;
}

void features::weather::update_weather() {
    if (!precipitation) {
        for (auto client_class = interfaces::client->get_all_classes(); client_class && !precipitation;
            client_class = client_class->next_ptr) {
            if (client_class->class_id == cprecipitation)
                precipitation = client_class;
        }
    }
    static int weather_type = 0;
    switch (c::visuals::weather_type) {
        case 0: {
            weather_type = 4;
            break;
        }
        case 1: {
            weather_type = 5;
            break;
        }
        case 2: {
            weather_type = 6;
            break;
        }
        case 3: {
            weather_type = 7;
            break;
        }
    }
    if (m_timer > -1) {
        --m_timer;
        if (m_timer == 0) {
            reset_weather();
        }
    }
    if (!precipitation)
        return;

    static std::optional< int > last_type{};

    if (last_type.has_value() && last_type.value() != weather_type)
        reset_weather();

    last_type = weather_type;

    memset(&precipitation_collideable, 0, sizeof(precipitation_collideable));

    if (!created_rain && c::visuals::enable_weather || c::visuals::enable_weather && !last_state) {
        if (created_rain && rain_entity)
            reset_weather();

        if (precipitation->create_fn && precipitation->create_fn(MAX_EDICTS - 1, 0)) {

            rain_entity = reinterpret_cast<precipitation_t*>(interfaces::ent_list->get_client_entity(MAX_EDICTS - 1));

            rain_entity->net_pre_data_update(0);
            rain_entity->pre_data_change(0);
            rain_entity->get_index() = -1;

            rain_entity->precip_type() = (precipitation_type_t) * &weather_type;

            rain_entity->collideable()->mins() = { -32768.0f, -32768.0f, -32768.0f };
            rain_entity->collideable()->maxs() = { 32768.0f, 32768.0f, 32768.0f };

            interfaces::physics_collision->v_collide_load(&precipitation_collideable, 1, (const char*)collide_data, sizeof(collide_data));

            rain_entity->get_model_index() = -1;
            rain_entity->on_data_changed(0);
            rain_entity->post_data_update(0);

            created_rain = true;
        }
    }
    else if (created_rain && !c::visuals::enable_weather && last_state) {
        reset_weather();
    }

    last_state = c::visuals::enable_weather;
}