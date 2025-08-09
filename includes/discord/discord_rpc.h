#pragma once
#include "Includes/discord_register.h"
#include <Windows.h>
#include "Includes/discord_rpc.h"
#include "../../utils/singleton.hpp"

class c_discord : public singleton<c_discord>
{
public:
    void initialize();
    void update();
    void shutdown();

};