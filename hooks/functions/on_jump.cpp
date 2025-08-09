#include "../hooks.hpp"
#include "../../menu/config/config.hpp"
#include "../../features/misc/misc.hpp"

void __stdcall sdk::hooks::on_jump::on_jump(float stamina) {
    ofunc(stamina);

    static int counter = 0;
    counter++;
    //we have jumpped
    if (counter % 2 == 0 && counter != 0) {
        counter = 0;
        features::misc::jumpstats::gotjump();
    }

}