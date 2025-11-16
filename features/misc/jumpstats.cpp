#include "../../features/movement/movement.hpp"
#include "../movement/prediction/prediction.hpp"
#include "../../menu/config/config.hpp"
#include "../../menu/menu.hpp"
#include "../misc/misc.hpp"
#include <iostream>
#include <cmath>
#include "jumpstats.hpp"

//TODO: IM SRY FOR EVERYTTHING IVE DONE TO THAT CODE

struct JumpStats
{
	vec3_t StartJump{};
	vec3_t EndJump{};
	float distance{};
	std::string JumpType{};
	float sync{};
	float JumpHeight;
	int strafes;
	int pre;
	int max;
};

bool edgebugged = false;
void features::misc::jumpstats::jumpstats(c_usercmd* cmd)
{
	if (!c::misc::jumpstats)
		return;
	if (!g::local)
		return;
	if (!g::local->is_alive())
		return;

	static bool OnJump = false;
	static bool OnLand = false;
	static bool RecordDuck = false;
	static int DuckTicks = 0;
	static int ticksonGround = 0;
	static int BhopCount = 0;
	static bool JumpIsJb = false;
	static int perfectStrafeTicks = 0;
	static int strafeTicks = 0;
	static JumpStats js;
	static bool strafe_right = false;
	static bool strafe_left = false;
	if (prediction_backup::flags & 1)
	{
		ticksonGround += 1;
		perfectStrafeTicks = 0;
		strafeTicks = 0;
		js.strafes = 0;
		js.max = 0;
		js.pre = std::round(prediction_backup::velocity.length_2d());
	}
	else
	{
		ticksonGround = 0;
		if (js.max < g::local->velocity().length_2d())
			js.max = std::round(g::local->velocity().length_2d());
		if (g::local->velocity().length_2d() > 50.f)
		{
			float deltaSpeed = g::local->velocity().length_2d() - prediction_backup::velocity.length_2d();
			bool gained = deltaSpeed > 0.000001f;
			bool lost = deltaSpeed < -0.000001f;
			if (gained)
			{
				perfectStrafeTicks++;
			}
			strafeTicks++;
		}
	}
	if (strafeTicks > 0)
		js.sync = (perfectStrafeTicks / static_cast<float>(strafeTicks)) * 100.f;
	if (ticksonGround > 2)
		BhopCount = 0;

	if (features::movement::detect_data.ticks_left == 1) {
		edgebugged = true;
	}
	if (prediction_backup::flags & 1 && !(g::local->flags() & 1))
	{
		OnJump = true;
	}
	if (cmd->mouse_dx < 0 && !strafe_right && !strafe_left)
		strafe_right = true;
	else if (cmd->mouse_dx > 0 && !strafe_right && !strafe_left)
		strafe_left = true;
	if (cmd->mouse_dx < 0 && !strafe_right && strafe_left)
	{
		strafe_right = true;
		strafe_left = false;
		js.strafes++;
	}
	else if (cmd->mouse_dx > 0 && strafe_right && !strafe_left) {
		strafe_right = false;
		strafe_left = true;
		js.strafes++;
	}
	if (features::movement::detected_normal_jump_bug && !JumpIsJb)
	{
		js.pre = std::round(prediction_backup::velocity.length_2d());
		JumpIsJb = true;
		OnJump = true;
	}
	if (g::local->flags() & 1 && !(prediction_backup::flags & 1))
	{
		OnLand = true;
	}
	if (prediction_backup::velocity.z > 0 && g::local->velocity().z < 0)
		js.JumpHeight = abs(prediction_backup::origin.z) - abs(js.StartJump.z);

	if (OnJump)
	{
		if (cmd->buttons & in_duck)
			RecordDuck = true;
		js.StartJump = prediction_backup::origin;
		if (ticksonGround == 1)
			BhopCount += 1;
		js.JumpType = "J: ";
		if (BhopCount == 1)
			js.JumpType = "BH: ";
		if (BhopCount > 1)
			js.JumpType = "MBH: ";

		OnJump = false;
	}
	if (RecordDuck && cmd->buttons & in_duck)
	{
		DuckTicks += 1;
	}
	else
	{
		DuckTicks = 0;
		RecordDuck = false;
	}

	if (BhopCount == 0)
	{
		if (RecordDuck)
		{
			if (DuckTicks == 1)
			{
				js.JumpType = "MJ: ";
			}
			if (DuckTicks == 2)
			{
				js.JumpType = "LJ: ";
			}
		}
	}
	if (JumpIsJb)
		js.JumpType = "JB: ";


	if (OnLand)
	{
		float razn;
		if (JumpIsJb)
			razn = 16.f;
		else
			razn = 2.f;
		JumpIsJb = false;
		js.EndJump = g::local->origin();
		float absh = abs(js.StartJump.z) - abs(js.EndJump.z);

		//isnt fixed, should be recoded properly
		if (edgebugged) {
			edgebugged = false;
			OnLand = false;
			return;
		}

		if (absh <razn && absh >-razn)
		{
			std::ostringstream distanceStream;
			distanceStream << std::fixed << std::setprecision(2) << js.StartJump.dist_to_2d(js.EndJump) + 32.0f;
			std::string distanceStr = distanceStream.str();

			std::ostringstream HeightStream;
			HeightStream << std::fixed << std::setprecision(2) << js.JumpHeight * -1;
			std::string HeightStr = HeightStream.str();
			int syncPercentage = static_cast<int>(js.sync);

			float dist = js.StartJump.dist_to_2d(js.EndJump) + 32.0f;
			if (js.JumpType == "LJ: ")
			{
				if (dist < 230)
				{
					jumpstating::JumpInfo = "\x08 " + js.JumpType + distanceStr + " units | " + std::to_string(js.strafes) + " strafe | " + std::to_string(js.pre) + " pre | " + std::to_string(js.max) + " max | " + HeightStr + " height | " + std::to_string(syncPercentage) + " sync";
				}
				if (dist >= 230)
				{
					jumpstating::JumpInfo = "\x0C " + js.JumpType + distanceStr + " units \x08| \x0C" + std::to_string(js.strafes) + "\x08 strafe | \x0C" + std::to_string(js.pre) + "\x08 pre | \x0C" + std::to_string(js.max) + "\x08 max | \x0C" + HeightStr + "\x08 height | \x0C" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 235)
				{
					jumpstating::JumpInfo = "\x04 " + js.JumpType + distanceStr + " units \x08| \x04" + std::to_string(js.strafes) + "\x08 strafe | \x04" + std::to_string(js.pre) + "\x08 pre | \x04" + std::to_string(js.max) + "\x08 max | \x04" + HeightStr + "\x08 height | \x04" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 240)
				{
					jumpstating::JumpInfo = "\x02 " + js.JumpType + distanceStr + " units \x08| \x02" + std::to_string(js.strafes) + "\x08 strafe | \x02" + std::to_string(js.pre) + "\x08 pre | \x02" + std::to_string(js.max) + "\x08 max | \x02" + HeightStr + "\x08 height | \x02" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 244)
				{
					jumpstating::JumpInfo = "\x09 " + js.JumpType + distanceStr + " units \x08| \x09" + std::to_string(js.strafes) + "\x08 strafe | \x09" + std::to_string(js.pre) + "\x08 pre | \x09" + std::to_string(js.max) + "\x08 max | \x09" + HeightStr + "\x08 height | \x09" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 246)
				{
					jumpstating::JumpInfo = "\x0D " + js.JumpType + distanceStr + " units \x08| \x0D" + std::to_string(js.strafes) + "\x08 strafe | \x0D" + std::to_string(js.pre) + "\x08 pre | \x0D" + std::to_string(js.max) + "\x08 max | \x0D" + HeightStr + "\x08 height | \x0D" + std::to_string(syncPercentage) + "\x08 sync";

				}
			}
			if (js.JumpType == "BH: ")
			{
				if (dist < 230)
				{
					jumpstating::JumpInfo = "\x08 " + js.JumpType + distanceStr + " units | " + std::to_string(js.strafes) + " strafe | " + std::to_string(js.pre) + " pre | " + std::to_string(js.max) + " max | " + HeightStr + " height | " + std::to_string(syncPercentage) + " sync";
				}
				if (dist >= 230)
				{
					jumpstating::JumpInfo = "\x0C " + js.JumpType + distanceStr + " units \x08| \x0C" + std::to_string(js.strafes) + "\x08 strafe | \x0C" + std::to_string(js.pre) + "\x08 pre | \x0C" + std::to_string(js.max) + "\x08 max | \x0C" + HeightStr + "\x08 height | \x0C" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 234)
				{
					jumpstating::JumpInfo = "\x04 " + js.JumpType + distanceStr + " units \x08| \x04" + std::to_string(js.strafes) + "\x08 strafe | \x04" + std::to_string(js.pre) + "\x08 pre | \x04" + std::to_string(js.max) + "\x08 max | \x04" + HeightStr + "\x08 height | \x04" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 235)
				{
					jumpstating::JumpInfo = "\x02 " + js.JumpType + distanceStr + " units \x08| \x02" + std::to_string(js.strafes) + "\x08 strafe | \x02" + std::to_string(js.pre) + "\x08 pre | \x02" + std::to_string(js.max) + "\x08 max | \x02" + HeightStr + "\x08 height | \x02" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 238)
				{
					jumpstating::JumpInfo = "\x09 " + js.JumpType + distanceStr + " units \x08| \x09" + std::to_string(js.strafes) + "\x08 strafe | \x09" + std::to_string(js.pre) + "\x08 pre | \x09" + std::to_string(js.max) + "\x08 max | \x09" + HeightStr + "\x08 height | \x09" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 240)
				{
					jumpstating::JumpInfo = "\x0D " + js.JumpType + distanceStr + " units \x08| \x0D" + std::to_string(js.strafes) + "\x08 strafe | \x0D" + std::to_string(js.pre) + "\x08 pre | \x0D" + std::to_string(js.max) + "\x08 max | \x0D" + HeightStr + "\x08 height | \x0D" + std::to_string(syncPercentage) + "\x08 sync";

				}
			}
			if (js.JumpType == "MBH: ")
			{
				if (dist < 235)
				{
					jumpstating::JumpInfo = "\x08 " + js.JumpType + distanceStr + " units | " + std::to_string(js.strafes) + " strafe | " + std::to_string(js.pre) + " pre | " + std::to_string(js.max) + " max | " + HeightStr + " height | " + std::to_string(syncPercentage) + " sync";
				}
				if (dist >= 235)
				{
					jumpstating::JumpInfo = "\x0C " + js.JumpType + distanceStr + " units \x08| \x0C" + std::to_string(js.strafes) + "\x08 strafe | \x0C" + std::to_string(js.pre) + "\x08 pre | \x0C" + std::to_string(js.max) + "\x08 max | \x0C" + HeightStr + "\x08 height | \x0C" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 237)
				{
					jumpstating::JumpInfo = "\x04 " + js.JumpType + distanceStr + " units \x08| \x04" + std::to_string(js.strafes) + "\x08 strafe | \x04" + std::to_string(js.pre) + "\x08 pre | \x04" + std::to_string(js.max) + "\x08 max | \x04" + HeightStr + "\x08 height | \x04" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 242)
				{
					jumpstating::JumpInfo = "\x02 " + js.JumpType + distanceStr + " units \x08| \x02" + std::to_string(js.strafes) + "\x08 strafe | \x02" + std::to_string(js.pre) + "\x08 pre | \x02" + std::to_string(js.max) + "\x08 max | \x02" + HeightStr + "\x08 height | \x02" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 245)
				{
					jumpstating::JumpInfo = "\x09 " + js.JumpType + distanceStr + " units \x08| \x09" + std::to_string(js.strafes) + "\x08 strafe | \x09" + std::to_string(js.pre) + "\x08 pre | \x09" + std::to_string(js.max) + "\x08 max | \x09" + HeightStr + "\x08 height | \x09" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 247)
				{
					jumpstating::JumpInfo = "\x0D " + js.JumpType + distanceStr + " units \x08| \x0D" + std::to_string(js.strafes) + "\x08 strafe | \x0D" + std::to_string(js.pre) + "\x08 pre | \x0D" + std::to_string(js.max) + "\x08 max | \x0D" + HeightStr + "\x08 height | \x0D" + std::to_string(syncPercentage) + "\x08 sync";

				}
			}
			if (js.JumpType == "JB: ")
			{
				if (dist < 250)
				{
					jumpstating::JumpInfo = "\x08 " + js.JumpType + distanceStr + " units | " + std::to_string(js.strafes) + " strafe | " + std::to_string(js.pre) + " pre | " + std::to_string(js.max) + " max | " + HeightStr + " height | " + std::to_string(syncPercentage) + " sync";
				}
				if (dist >= 250)
				{
					jumpstating::JumpInfo = "\x0C " + js.JumpType + distanceStr + " units \x08| \x0C" + std::to_string(js.strafes) + "\x08 strafe | \x0C" + std::to_string(js.pre) + "\x08 pre | \x0C" + std::to_string(js.max) + "\x08 max | \x0C" + HeightStr + "\x08 height | \x0C" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 255)
				{
					jumpstating::JumpInfo = "\x04 " + js.JumpType + distanceStr + " units \x08| \x04" + std::to_string(js.strafes) + "\x08 strafe | \x04" + std::to_string(js.pre) + "\x08 pre | \x04" + std::to_string(js.max) + "\x08 max | \x04" + HeightStr + "\x08 height | \x04" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 260)
				{
					jumpstating::JumpInfo = "\x02 " + js.JumpType + distanceStr + " units \x08| \x02" + std::to_string(js.strafes) + "\x08 strafe | \x02" + std::to_string(js.pre) + "\x08 pre | \x02" + std::to_string(js.max) + "\x08 max | \x02" + HeightStr + "\x08 height | \x02" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 265)
				{
					jumpstating::JumpInfo = "\x09 " + js.JumpType + distanceStr + " units \x08| \x09" + std::to_string(js.strafes) + "\x08 strafe | \x09" + std::to_string(js.pre) + "\x08 pre | \x09" + std::to_string(js.max) + "\x08 max | \x09" + HeightStr + "\x08 height | \x09" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 268)
				{
					jumpstating::JumpInfo = "\x0D " + js.JumpType + distanceStr + " units \x08| \x0D" + std::to_string(js.strafes) + "\x08 strafe | \x0D" + std::to_string(js.pre) + "\x08 pre | \x0D" + std::to_string(js.max) + "\x08 max | \x0D" + HeightStr + "\x08 height | \x0D" + std::to_string(syncPercentage) + "\x08 sync";

				}
			}
			if (js.JumpType == "J: ")
			{
				if (dist < 227)
				{
					jumpstating::JumpInfo = "\x08 " + js.JumpType + distanceStr + " units | " + std::to_string(js.strafes) + " strafe | " + std::to_string(js.pre) + " pre | " + std::to_string(js.max) + " max | " + HeightStr + " height | " + std::to_string(syncPercentage) + " sync";
				}
				if (dist >= 227)
				{
					jumpstating::JumpInfo = "\x0C " + js.JumpType + distanceStr + " units \x08| \x0C" + std::to_string(js.strafes) + "\x08 strafe | \x0C" + std::to_string(js.pre) + "\x08 pre | \x0C" + std::to_string(js.max) + "\x08 max | \x0C" + HeightStr + "\x08 height | \x0C" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 232)
				{
					jumpstating::JumpInfo = "\x04 " + js.JumpType + distanceStr + " units \x08| \x04" + std::to_string(js.strafes) + "\x08 strafe | \x04" + std::to_string(js.pre) + "\x08 pre | \x04" + std::to_string(js.max) + "\x08 max | \x04" + HeightStr + "\x08 height | \x04" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 237)
				{
					jumpstating::JumpInfo = "\x02 " + js.JumpType + distanceStr + " units \x08| \x02" + std::to_string(js.strafes) + "\x08 strafe | \x02" + std::to_string(js.pre) + "\x08 pre | \x02" + std::to_string(js.max) + "\x08 max | \x02" + HeightStr + "\x08 height | \x02" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 241)
				{
					jumpstating::JumpInfo = "\x09 " + js.JumpType + distanceStr + " units \x08| \x09" + std::to_string(js.strafes) + "\x08 strafe | \x09" + std::to_string(js.pre) + "\x08 pre | \x09" + std::to_string(js.max) + "\x08 max | \x09" + HeightStr + "\x08 height | \x09" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 243)
				{
					jumpstating::JumpInfo = "\x0D " + js.JumpType + distanceStr + " units \x08| \x0D" + std::to_string(js.strafes) + "\x08 strafe | \x0D" + std::to_string(js.pre) + "\x08 pre | \x0D" + std::to_string(js.max) + "\x08 max | \x0D" + HeightStr + "\x08 height | \x0D" + std::to_string(syncPercentage) + "\x08 sync";

				}
			}
			if (js.JumpType == "MJ: ")
			{
				if (dist < 220)
				{
					jumpstating::JumpInfo = "\x08 " + js.JumpType + distanceStr + " units | " + std::to_string(js.strafes) + " strafe | " + std::to_string(js.pre) + " pre | " + std::to_string(js.max) + " max | " + HeightStr + " height | " + std::to_string(syncPercentage) + " sync";
				}
				if (dist >= 220)
				{
					jumpstating::JumpInfo = "\x0C " + js.JumpType + distanceStr + " units \x08| \x0C" + std::to_string(js.strafes) + "\x08 strafe | \x0C" + std::to_string(js.pre) + "\x08 pre | \x0C" + std::to_string(js.max) + "\x08 max | \x0C" + HeightStr + "\x08 height | \x0C" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 225)
				{
					jumpstating::JumpInfo = "\x04 " + js.JumpType + distanceStr + " units \x08| \x04" + std::to_string(js.strafes) + "\x08 strafe | \x04" + std::to_string(js.pre) + "\x08 pre | \x04" + std::to_string(js.max) + "\x08 max | \x04" + HeightStr + "\x08 height | \x04" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 230)
				{
					jumpstating::JumpInfo = "\x02 " + js.JumpType + distanceStr + " units \x08| \x02" + std::to_string(js.strafes) + "\x08 strafe | \x02" + std::to_string(js.pre) + "\x08 pre | \x02" + std::to_string(js.max) + "\x08 max | \x02" + HeightStr + "\x08 height | \x02" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 234)
				{
					jumpstating::JumpInfo = "\x09 " + js.JumpType + distanceStr + " units \x08| \x09" + std::to_string(js.strafes) + "\x08 strafe | \x09" + std::to_string(js.pre) + "\x08 pre | \x09" + std::to_string(js.max) + "\x08 max | \x09" + HeightStr + "\x08 height | \x09" + std::to_string(syncPercentage) + "\x08 sync";

				}
				if (dist >= 236)
				{
					jumpstating::JumpInfo = "\x0D " + js.JumpType + distanceStr + " units \x08| \x0D" + std::to_string(js.strafes) + "\x08 strafe | \x0D" + std::to_string(js.pre) + "\x08 pre | \x0D" + std::to_string(js.max) + "\x08 max | \x0D" + HeightStr + "\x08 height | \x0D" + std::to_string(syncPercentage) + "\x08 sync";

				}
			}

			//recode it if u want idc :D
			if (dist < 280.f && dist > 220.f && !c::misc::jumpstats_show_fail)
				interfaces::chat_element->chatprintf("#delusional#_jumpstats");
			else if (dist < 280.f && dist > 150.f && c::misc::jumpstats_show_fail)
				interfaces::chat_element->chatprintf("#delusional#_jumpstats");
		}
		OnLand = false;
	}

}