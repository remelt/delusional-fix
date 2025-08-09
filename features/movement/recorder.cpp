#include "movement.hpp"
#include "../../menu/menu.hpp"
#include "../../menu/config/config.hpp"
#include "../movement/prediction/prediction.hpp"
#include "../../includes/imgui/imgui_internal.h"
#include "../../includes/json/json.h"

bool world_to_screen(const vec3_t& in, vec3_t& out) {
	const auto result = math::screen_transform(in, out);
	int w, h;
	interfaces::engine->get_screen_size(w, h);

	out.x = (w / 2.0f) + (out.x * w) / 2.0f;
	out.y = (h / 2.0f) - (out.y * h) / 2.0f;

	return result;
}

static auto world_circle = [](vec3_t location, float radius, color_t col = color_t(255, 255, 255, 255), bool filled = false) {
	static constexpr float Step = m_pi * 2.0f / 60;
	std::vector<ImVec2> points;
	for (float lat = 0.f; lat <= m_pi * 2.0f; lat += Step) {
		const auto& point3d = vec3_t(sin(lat), cos(lat), 0.f) * radius;
		vec3_t point2d;

		if (world_to_screen(location + point3d, point2d))
			points.push_back(ImVec2(point2d.x, point2d.y));
	}

	im_render.drawpolyline(points, col, true, 2.f);
};

struct frame {
	float viewangles[2];
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	short mousedx;
	short mousedy;
	vec3_t position;

	frame(c_usercmd* cmd, vec3_t pos) {
		this->viewangles[0] = cmd->view_angles.x;
		this->viewangles[1] = cmd->view_angles.y;
		this->forwardmove = cmd->forward_move;
		this->sidemove = cmd->side_move;
		this->upmove = cmd->up_move;
		this->buttons = cmd->buttons;
		this->mousedx = cmd->mouse_dx;
		this->mousedy = cmd->mouse_dy;
		this->position = pos;
	}

	void replay(c_usercmd* cmd) {
		cmd->view_angles.x = this->viewangles[0];
		cmd->view_angles.y = this->viewangles[1];
		cmd->forward_move = this->forwardmove;
		cmd->side_move = this->sidemove;
		cmd->up_move = this->upmove;
		cmd->buttons = this->buttons;
		cmd->mouse_dx = this->mousedx;
		cmd->mouse_dy = this->mousedy;
	}
};

typedef std::vector<frame> framecontainer;

struct fullroute {
	std::string routename;
	std::string startwpn;
	framecontainer frames;
};

std::vector<fullroute> mapWithRoutes;

class recorder_t {
private:
	bool is_recording_active = false;
	framecontainer recording_frames;

public:
	void startrecording() {
		this->is_recording_active = true;
	}

	void stoprecording() {
		this->is_recording_active = false;
	}

	bool isrecordingactive() const {
		return this->is_recording_active;
	}

	framecontainer& getactiverecording() {
		return this->recording_frames;
	}

	void setrecording(framecontainer frames) {
		this->recording_frames = frames;
	}
};

class playback_t {
private:
	bool is_playback_active = false;
	size_t current_frame = 0;
	const framecontainer& active_demo = framecontainer();

public:
	void startplayback(framecontainer& frames) {
		this->is_playback_active = true;

		uintptr_t active_demoptr = (uintptr_t)&this->active_demo;
		*(framecontainer*)active_demoptr = frames;
	};

	void stopplayback() {
		this->is_playback_active = false;
		this->current_frame = 0;
	};

	bool isplaybackactive() const {
		return this->is_playback_active;
	}

	size_t getcurrentframe() const {
		return this->current_frame;
	};

	void setcurrentframe(size_t frame) {
		this->current_frame = frame;
	};

	const framecontainer& getactivedemo() {
		return this->active_demo;
	}
};

playback_t playback;
recorder_t rec;

char filename[32] = "";

int curroute = -1;

static char buffer[32];
static char buffer1[32];

static int delselection;

std::string routestodelete = "";
std::string routelist = "";

void mrecorder::forcestop() {
	playback.stopplayback();
}

void mrecorder::draw() {

	int flags;
	flags = ImGuiInputTextFlags_EnterReturnsTrue;

	auto& routeItems = route->getRoutes();

	static int currentRoute = -1;

	if (static_cast<std::size_t>(currentRoute) >= routeItems.size())
		currentRoute = -1;

	ImGui::Text(("map files"));

	ImGui::ListBoxHeader("", routeItems.size(), 5);
	for (int i = 0; i < routeItems.size(); i++) {
		bool isSelected = (currentRoute == i);
		if (ImGui::Selectable(routeItems[i].c_str(), isSelected))
			currentRoute = i;
	}
	ImGui::ListBoxFooter();

	ImGui::InputTextWithHint(("##route name"), ("route name"), buffer, sizeof(buffer));

	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("press enter to create a new record");
	}

	//for buffer
	if (currentRoute != -1) {

		if (ImGui::Button("save route", ImVec2(-1, 15))) {
			route->save(currentRoute, buffer);
			ZeroMemory(buffer, 32);
		}
	}

	if (interfaces::engine->is_in_game()) {

		if (ImGui::Button("create file map", ImVec2(-1, 15))) {
			std::string routname = interfaces::engine->get_level_name() + std::to_string(int(1.0f / interfaces::globals->interval_per_tick));
			route->add(routname.c_str());
			ZeroMemory(buffer, 32);
			ZeroMemory(buffer1, 32);
		}
	}

	if (currentRoute != -1) {

		if (ImGui::Button("load selected map file", ImVec2(-1, 15))) {
			route->load(currentRoute);
			ZeroMemory(buffer, 32);
			ZeroMemory(buffer1, 32);
		}


		if (ImGui::Button("delete selected map file", ImVec2(-1, 15))) {
			route->remove(currentRoute);
			currentRoute = -1;
			ZeroMemory(buffer, 32);
			ZeroMemory(buffer1, 32);
		}

		if (ImGui::Button("clear routes from map file", ImVec2(-1, 15))) {
			if (!mapWithRoutes.empty())
				mapWithRoutes.clear();
		}

		if (ImGui::Button("reload records", ImVec2(-1, 15)))
		route->listroutes();

		if (!mapWithRoutes.empty()) {

			std::string text = ("records (") + std::to_string(mapWithRoutes.size()) + ("):");

			routelist.clear();

			for (int i = 0; i < mapWithRoutes.size(); i++) {
				std::string item = std::to_string(i) + " - " + mapWithRoutes[i].routename + "\n";
				routelist += item;
			}

			ImGui::Text(text.c_str());

			ImGui::Text(routelist.c_str());

			if (ImGui::Button("delete route")) {
				if (delselection >= 0 && delselection < mapWithRoutes.size()) {
					mapWithRoutes.erase(mapWithRoutes.begin() + delselection);
				}
			}

			std::clamp(delselection, 0, (int)mapWithRoutes.size() - 1);
			ImGui::SetNextItemWidth(55);
			ImGui::SameLine();
			ImGui::InputInt(("record to delete"), &delselection);

		}
	}

	framecontainer& recording = rec.getactiverecording();
	int currentFrame = playback.getcurrentframe();
	int maxFrames = recording.size();
	ImGui::Text(std::string(("frames: ") + std::to_string(currentFrame) + (" / ") + std::to_string(maxFrames)).c_str());
	if (rec.isrecordingactive())
		ImGui::Text(("recording is active"));
	if (playback.isplaybackactive())
		ImGui::Text(("playback is active"));;
}

void mrecorder::drawroute() {
	if (!c::misc::movement_rec)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));

	if (playback.isplaybackactive() || rec.isrecordingactive()) {
		if (rec.getactiverecording().size() > 2) {
			if (interfaces::engine->is_connected()) {
				if (g::local) {
					if (g::local->is_alive()) {
						auto& recording = rec.getactiverecording();
						for (int i = 1; i < recording.size(); i++) {
							vec3_t from; vec3_t to;
							if (interfaces::debug_overlay->world_to_screen(recording.at(i).position, from) && interfaces::debug_overlay->world_to_screen(recording.at(i - 1).position, to) && c::misc::movement_rec_show_line) {
								//draw the recording line
								im_render.drawline(from.x, from.y, to.x, to.y, color_t(255, 255, 255, 255), 0.5);
								//circle @ start
								if (i == 1) {

									interfaces::surface->draw_outlined_circle(from.x, from.y, 0, 15);
									if (prediction::origin.distance_to(recording.at(i - 1).position) < 0.5f) {
										world_circle(recording.at(i - 1).position, 7.f, color_t(255, 255, 255), true);

									}
									else {
										world_circle(recording.at(i - 1).position, 7.f, color_t(255, 255, 255), true);
									}
								}
								//circle @ end
								else if (i == recording.size() - 1 && !rec.isrecordingactive()) {
									//world_circle(recording.at(i).position, 15.f, color_t(255, 255, 255), true);
								}
							}
						}
					}
				}
			}
		}
	}
	else {
		if (!mapWithRoutes.empty()) {
			if (g::local->is_alive()) {
				for (size_t i = 0; i < mapWithRoutes.size(); i++) {

					if (c::misc::movement_rec_maxrender != 0) {
						if (prediction::origin.distance_to(mapWithRoutes.at(i).frames.at(0).position) > c::misc::movement_rec_maxrender) {
							continue;
						}
					}

					vec3_t scrpos;
					if (interfaces::debug_overlay->world_to_screen(vec3_t{ mapWithRoutes.at(i).frames.at(0).position.x, mapWithRoutes.at(i).frames.at(0).position.y, mapWithRoutes.at(i).frames.at(0).position.z }, scrpos)) {

						if (prediction::origin.distance_to(mapWithRoutes.at(i).frames.at(0).position) < 0.5f) {
							im_render.drawcircle(scrpos.x, scrpos.y + 3, 5.f, 32, color_t(255, 255, 255));
						}
						else {
							im_render.drawcircle(scrpos.x, scrpos.y + 3, 5.f, 32, color_t(255, 255, 255));
						}

						im_render.text(scrpos.x, scrpos.y - 20, 12.f, fonts::esp_misc, mapWithRoutes.at(i).routename.c_str(), true, color_t(255, 255, 255, 255));
					}

					if (prediction::origin.distance_to(mapWithRoutes.at(i).frames.at(0).position) < c::misc::movement_rec_ringsize) {
						vec3_t o = g::local->get_eye_pos();

						vec3_t angles = { mapWithRoutes.at(i).frames.at(0).viewangles[0], mapWithRoutes.at(i).frames.at(0).viewangles[1], 0.f };

						vec3_t forward;
						math::angle_vectors(angles, &forward);

						vec3_t out;
						if (interfaces::debug_overlay->world_to_screen(o + (forward * 100.f), out)) {
							auto get_text_size = [=](const std::string& text, ImFont* font = fonts::esp_misc) {
								return font->CalcTextSizeA(12.f, FLT_MAX, 0.0f, text.c_str());
							};

							auto text_size = get_text_size("aim here", fonts::esp_misc);

							im_render.text(out.x + 7, out.y - (text_size.y / 2), 12.f, fonts::esp_misc, ("aim here"), false, color_t(255, 255, 255, 255));
							im_render.drawcircle(out.x, out.y, 4, 32, color_t(255, 255, 255, 255));
						}

					}
				}
			}
		}
	}
}


bool goingtostart = false;
bool smoothingtostart = false;
bool wishtostart = false;
void mrecorder::create_move(c_usercmd* cmd) {
	if (!c::misc::movement_rec)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
	bool isplaybackactive = playback.isplaybackactive();
	bool isRecordingActive = rec.isrecordingactive();

	if (g::local->is_alive()) {

		if (menu::checkkey(c::misc::movement_rec_keystartrecord, c::misc::movement_rec_keystartrecord_s) && !rec.isrecordingactive() && !playback.isplaybackactive() && !menu::open)
			rec.startrecording();

		if (menu::checkkey(c::misc::movement_rec_keystoprecord, c::misc::movement_rec_keystoprecord_s) && !playback.isplaybackactive()) {
			rec.stoprecording();
			cmd->view_angles.normalize3();
		}

		if (menu::checkkey(c::misc::movement_rec_keystartplayback, c::misc::movement_rec_keystartplayback_s) && !rec.isrecordingactive() && !playback.isplaybackactive() && !menu::open) {
			if (mapWithRoutes.size() > 0) {

				size_t closestroute = -1;
				float bestdist = 999999;

				for (size_t i = 0; i < mapWithRoutes.size(); i++) {
					framecontainer& currec = mapWithRoutes.at(i).frames;
					if (currec.at(0).position.distance_to(prediction_backup::origin) < bestdist) {
						closestroute = i;
						bestdist = currec.at(0).position.distance_to(prediction_backup::origin);
					}
				}

				if (closestroute == -1)
					return;

				if (mapWithRoutes.at(closestroute).frames.size() > 0) {
					if (mapWithRoutes.at(closestroute).frames.at(0).position.distance_to(prediction_backup::origin) < c::misc::movement_rec_ringsize) {
						rec.setrecording(mapWithRoutes.at(closestroute).frames);
						goingtostart = true;
						if (c::misc::movement_rec_lockva)
							smoothingtostart = true;
						wishtostart = true;
					}
				}
			}
		}

		if (goingtostart) {

			float dist = prediction_backup::origin.distance_to(rec.getactiverecording().at(0).position);

			if (dist <= 0.1f) {
				goingtostart = false;
			}

			auto curPos = prediction_backup::origin;
			auto difference = curPos - rec.getactiverecording().at(0).position;

			float cos_flt, sin_flt;
			cos_flt = cos(cmd->view_angles.y / 180.f * m_pi);
			sin_flt = sin(cmd->view_angles.y / 180.f * m_pi);
			auto velocity = vec3_t {
				difference.x * cos_flt + difference.y * sin_flt,
				difference.y * cos_flt - difference.x * sin_flt,
				difference.z
			};

			cmd->forward_move = -velocity.x * 20.f;
			cmd->side_move = velocity.y * 20.f;
		}

		if (smoothingtostart) {
			cmd->mouse_dx = 0;
			cmd->mouse_dy = 0;

			vec2_t targetviewangles = { rec.getactiverecording().at(0).viewangles[0], rec.getactiverecording().at(0).viewangles[1] };

			vec3_t deltaang = vec3_t{ targetviewangles.x - cmd->view_angles.x, targetviewangles.y - cmd->view_angles.y, 0.f };

			if (deltaang.length_r() < 0.1f || !c::misc::movement_rec_lockva) {
				smoothingtostart = false;
			}

			deltaang.normalize3();

			deltaang.x /= c::misc::movement_rec_smoothing;
			deltaang.y /= c::misc::movement_rec_smoothing;
			cmd->view_angles.x += deltaang.x;
			cmd->view_angles.y += deltaang.y;
			interfaces::engine->set_view_angles(cmd->view_angles);
		}

		if (wishtostart) {
			if (!smoothingtostart && !goingtostart) {
				if (!rec.getactiverecording().empty()) {
					if (prediction_backup::origin.distance_to(rec.getactiverecording().at(0).position) < 0.1f) {
						if ((vec3_t{ vec3_t{ rec.getactiverecording().at(0).viewangles[0], rec.getactiverecording().at(0).viewangles[1], 0.f } - cmd->view_angles }.length_2d() < 0.1f) || !c::misc::movement_rec_lockva) {
							playback.startplayback(rec.getactiverecording());
							wishtostart = false;
							return;
						}
					}
				}
			}
		}

		if (menu::checkkey(c::misc::movement_rec_keystopplayback, c::misc::movement_rec_keystopplayback_s) && playback.isplaybackactive() && !rec.isrecordingactive()) {
			goingtostart = false;
			smoothingtostart = false;
			cmd->view_angles = { playback.getactivedemo().at(playback.getcurrentframe()).viewangles[0], playback.getactivedemo().at(playback.getcurrentframe()).viewangles[1], 0.f };
			playback.stopplayback();
		}

		if (isRecordingActive) {
			prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
			rec.getactiverecording().push_back(frame(cmd, g::local->origin()));
		}

		if (isplaybackactive) {
			const size_t current_playback_frame = playback.getcurrentframe();
			try {
				vec3_t orang = cmd->view_angles;
				rec.getactiverecording().at(current_playback_frame).replay(cmd);
				vec3_t tomvfix = cmd->view_angles;
				if (c::misc::movement_rec_lockva) {
					interfaces::engine->set_view_angles(cmd->view_angles);
				}
				else {
					cmd->view_angles = orang;
					features::movement::fix_movement(cmd, tomvfix);
				}

				if (current_playback_frame + 1 == rec.getactiverecording().size()) {
					playback.stopplayback();
				}
				else {
					playback.setcurrentframe(current_playback_frame + 1);
				}
			}
			catch (std::out_of_range) {
				playback.stopplayback();
			}
		}
	}

	if (menu::checkkey(c::misc::movement_rec_keyclearrecord, c::misc::movement_rec_keyclearrecord_s) && !menu::open) {
		if (!rec.isrecordingactive() && !playback.isplaybackactive()) {
			rec.getactiverecording().clear();
		}
	}

	cmd->view_angles.normalize3();
}

savingroute::savingroute(const char* name) {
	path /= "C:/delusional";
	path /= name;
	std::error_code ec;
	if (!std::filesystem::exists(path))
		std::filesystem::create_directory(path, ec);
	listroutes();
}

bool is_empty_file(std::ifstream& file) {
	return file.peek() == std::ifstream::traits_type::eof();
}

void savingroute::load(size_t id) {

	playback.stopplayback();

	mapWithRoutes.clear();

	Json::Value json;

	std::ifstream in(path.string() + "\\" + routes[id].c_str());

	if (in && in.good()) {
		if (!is_empty_file(in)) {
			in >> json;
		}
		else {
			return;
		}
	}
	else {
		return;
	}

	//std::vector<FullRoute> topush;
	auto& mainjson = json[("main")];

	size_t routeamount = 0;
	if (mainjson.isMember(("records"))) routeamount = mainjson[("records")].asInt();

	for (size_t i = 0; i < routeamount; i++) {
		fullroute thisroute = {};
		//memset(&thisroute, 0, sizeof(thisroute));

		auto& globaljson = json[("globals")][i];
		if (globaljson.isMember(("routename"))) thisroute.routename = globaljson[("routename")].asCString();
		if (globaljson.isMember(("startwpn"))) thisroute.startwpn = globaljson[("startwpn")].asCString();
		size_t framecount = 0;
		if (globaljson.isMember(("framecount"))) framecount = globaljson[("framecount")].asInt();

		for (size_t j = 0; j < framecount; j++) {
			auto& framejson = json[("frame")][i][j];

			c_usercmd curframe = c_usercmd();
			vec3_t pos = {};

			if (framejson.isMember(("viewanglex"))) curframe.view_angles.x = framejson[("viewanglex")].asFloat();
			if (framejson.isMember(("viewangley"))) curframe.view_angles.y = framejson[("viewangley")].asFloat();
			if (framejson.isMember(("forwardmove"))) curframe.forward_move = framejson[("forwardmove")].asFloat();
			if (framejson.isMember(("sidemove"))) curframe.side_move = framejson[("sidemove")].asFloat();
			if (framejson.isMember(("upmove"))) curframe.up_move = framejson[("upmove")].asFloat();
			if (framejson.isMember(("buttons"))) curframe.buttons = framejson[("buttons")].asInt();
			if (framejson.isMember(("mousedx"))) curframe.mouse_dx = framejson[("mousedx")].asInt();
			if (framejson.isMember(("mousedy"))) curframe.mouse_dy = framejson[("mousedy")].asInt();
			if (framejson.isMember(("posx"))) pos.x = framejson[("posx")].asFloat();
			if (framejson.isMember(("posy"))) pos.y = framejson[("posy")].asFloat();
			if (framejson.isMember(("posz"))) pos.z = framejson[("posz")].asFloat();

			thisroute.frames.push_back(frame(&curframe, pos));
		}
		mapWithRoutes.push_back(thisroute);
	}
	curroute = id;
}

void savingroute::save(size_t id, std::string name) const {

	if (rec.getactiverecording().size() < 1)
		return;

	fullroute routetosave;
	routetosave.frames = rec.getactiverecording();
	routetosave.routename = name.c_str();

	mapWithRoutes.push_back(routetosave);

	Json::Value json;
	auto& mainjson = json[("main")];
	mainjson[("records")] = mapWithRoutes.size();

	for (size_t i = 0; i < mapWithRoutes.size(); i++) {
		fullroute thisroute = mapWithRoutes[i];

		auto& globaljson = json[("globals")][i];
		globaljson[("routename")] = thisroute.routename;
		globaljson[("startwpn")] = thisroute.startwpn;
		globaljson[("framecount")] = thisroute.frames.size();
		for (size_t j = 0; j < thisroute.frames.size(); j++) {
			auto& framejson = json[("frame")][i][j];

			framejson[("viewanglex")] = thisroute.frames.at(j).viewangles[0];
			framejson[("viewangley")] = thisroute.frames.at(j).viewangles[1];
			framejson[("forwardmove")] = thisroute.frames.at(j).forwardmove;
			framejson[("sidemove")] = thisroute.frames.at(j).sidemove;
			framejson[("upmove")] = thisroute.frames.at(j).upmove;
			framejson[("buttons")] = thisroute.frames.at(j).buttons;
			framejson[("mousedx")] = thisroute.frames.at(j).mousedx;
			framejson[("mousedy")] = thisroute.frames.at(j).mousedy;
			framejson[("posx")] = thisroute.frames.at(j).position.x;
			framejson[("posy")] = thisroute.frames.at(j).position.y;
			framejson[("posz")] = thisroute.frames.at(j).position.z;
		}
	}

	std::ofstream out(path.string() + "\\" + routes[id].c_str());

	if (out && out.good()) {
		out << json;
		curroute = id;
	}
}

void savingroute::add(const char* name) {
	if (*name && std::find(routes.cbegin(), routes.cend(), name) == routes.cend()) {
		routes.emplace_back(name);
		if (routes.size() == 0) {
			if (std::ofstream out{ path.string() + "\\" + routes[0].c_str() }; out.good())
				out << "";
		}
		else {
			if (std::ofstream out{ path.string() + "\\" + routes[routes.size() - 1].c_str() }; out.good())
				out << "";
		}
	}
}

void savingroute::remove(size_t id) {
	std::error_code ec;
	std::filesystem::remove(path.string() + "\\" + routes[id].c_str(), ec);
	routes.erase(routes.cbegin() + id);
}

void savingroute::rename(size_t item, const char* newName) {
	std::error_code ec;
	std::filesystem::rename(path.string() + "\\" + routes[item].c_str(), path.string() + "\\" + newName, ec);
	routes[item] = newName;
}


void savingroute::listroutes() {
	routes.clear();

	std::error_code ec;
	std::transform(std::filesystem::directory_iterator{ path, ec },
		std::filesystem::directory_iterator{ },
		std::back_inserter(routes),
		[](const auto& entry) { return std::string{ (const char*)entry.path().filename().u8string().c_str() }; });
}