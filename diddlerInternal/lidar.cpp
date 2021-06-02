#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Raycaster.h"
#include "drawCube.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"

namespace lidar {
	ImGuiWindowFlags infoBoxFlags;

	float currentOffset = 0.f;
	int currentPtr = 0;
	float records[125] = {};
	float zoom = 10.f;

	const float pi = 3.1415926535f;

	int resolution = 500;

	ImVec2 getPoint(ImVec2 midpoint, float angle, float rot, float length) {
		float delta_X = cos(angle + rot);
		float delta_Y = sin(angle + rot);
		return { midpoint.x + (delta_X * length), midpoint.y + (delta_Y * length) };
	}

	void drawLidarWindow(td::Vec2 size) {

		static ImVec4 white = ImVec4(1.0f, 1.0f, 1.f, 1.0f);
		ImU32 white32 = ImColor(white);

		static ImVec4 red = ImVec4(1.0f, 0.0f, 0.f, 1.0f);
		ImU32 red32 = ImColor(red);

		td::Color redT{ 1.f , 0.f, 0.f, 1.f };

		infoBoxFlags = 0;
		infoBoxFlags |= ImGuiWindowFlags_NoMove;
		infoBoxFlags |= ImGuiWindowFlags_NoResize;
		infoBoxFlags |= ImGuiWindowFlags_NoCollapse;
		infoBoxFlags |= ImGuiWindowFlags_NoTitleBar;

		ImGui::SetNextWindowPos(ImVec2(5, size.y + 10));
		ImGui::SetNextWindowSize({ size.x, size.x });
		ImGui::SetNextWindowBgAlpha(0.35f);

		ImGui::Begin("Lidar", (bool*)false, infoBoxFlags);
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImVec2 midpoint = { ImGui::GetWindowPos().x + (ImGui::GetWindowWidth() / 2), ImGui::GetWindowPos().y + (ImGui::GetWindowHeight() / 2) };

		currentOffset = 0.f;

		for (int i = 0; i < resolution; i++) {
			RaycastFilter filter{ 0 };
			filter.m_Mask = -1;
			glm::quat camera_rotation_tl = glm::quat(glm::vec3(0, (currentOffset + glb::player->camYaw), 0));
			glm::vec3 raycast_dir_tl = camera_rotation_tl * glm::vec3(0, 0, -1);

			raycaster::rayData rd = raycaster::castRayManual(glb::player->cameraPosition, { raycast_dir_tl.x, raycast_dir_tl.y, raycast_dir_tl.z }, &filter);
			td::Vec3 target = rd.worldPos;


			float step = ((pi * 2) / resolution);

			currentOffset += step;

			ImVec2 endPoint = getPoint(midpoint, (1.5 * pi) - (step * i), 0.f, rd.distance * zoom);
			 
			drawList->AddCircleFilled(endPoint, 2, white32);
		}

		drawList->AddRectFilled({ midpoint.x - 1, midpoint.y - 1 }, { midpoint.x + 2, midpoint.y + 2 }, white32);

		ImGui::SliderFloat("Zoom", &zoom, 1.f, 100.f, "%2.f");

		ImGui::End();
	}
}
