#pragma once
#include "movement.hpp"

namespace lobotomy_eb {

    inline bool EdgeBug_Founded;

    inline std::vector<vec3_t> lbebpos;

    void PrePredictionEdgeBug(c_usercmd* cmd);

    bool EdgeBugCheck(c_usercmd* cmd);

    void ReStorePrediction();

    void EdgeBugPostPredict(c_usercmd* cmd);

    void edgebug_detect(c_usercmd* cmd);

    void edgebug_lock(float& x, float& y);

    void frame_stage(int stage);
}