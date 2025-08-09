#include "visuals.hpp"

void features::weather::reset_weather(const bool cleanup) {
    created_rain = false;

    if (rain_entity && !IsBadReadPtr(rain_entity, 4) && cleanup) {
        if (rain_entity->networkable())
            rain_entity->net_release();
    }

    rain_entity = nullptr;
}

void features::weather::update_weather() {
    if (!precipitation) {
        for (auto client_class = interfaces::client->get_all_classes(); client_class && !precipitation;
            client_class = client_class->next_ptr) {
            if (client_class->class_id == cprecipitation)
                precipitation = client_class;
            break;
        }
    }

    static int weather_type = 4;

    if (!precipitation)
        return;

    if (!created_rain && c::visuals::enable_weather || c::visuals::enable_weather && !last_state) {
        if (created_rain && rain_entity)
            reset_weather();

        if (precipitation->create_fn && precipitation->create_fn(2048 - 1, 0)) {
            rain_entity = reinterpret_cast<precipitation_t*>(interfaces::ent_list->get_client_entity(2048 - 1));

            if (!rain_entity)
                return;

            rain_entity->net_pre_data_update(0);
            rain_entity->pre_data_change(0);
            rain_entity->get_entity_by_index(-1);

            *(int*)((uintptr_t)rain_entity + 0xA00) = weather_type;

            const auto collideable = rain_entity->collideable();
            if (!collideable)
                return;

            collideable->mins() = { -32768.f, -32768.f, -32768.f };
            collideable->maxs() = { 32768.f, 32768.f, 32768.f };

            rain_entity->on_data_changed(0);
            rain_entity->post_data_update(0);

            rain_entity->set_model_index(-1);

            created_rain = true;
        }
    }
    else if (created_rain && !c::visuals::enable_weather && last_state) {
        reset_weather();
    }

    last_state = c::visuals::enable_weather;
}

