#include "UI.hpp"
#include "ImGuiFix.hpp" //Adds std::vector support to the legacy Combo API
#include "../ImGui/imgui_impl_win32.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_dx9.h"
#include "../Invoker/Invoker.hpp"
#include "../Core/Core.hpp"
#include "../SDK/SDK.hpp"
#include <filesystem>

//Disable annoying ImGui warnings
#pragma warning(disable : 26451) //Arithmetic overflow: Using operator '+'
#pragma warning(disable : 6031) //Return value ignored: sscanf
#pragma warning(disable : 26812) //Enum 'x' is unscoped. Prefer 'enum class'.

void UI::SetStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(10, 10);
    style.FramePadding = ImVec2(4, 4);
    style.CellPadding = ImVec2(4, 2);
    style.ItemSpacing = ImVec2(10, 4);
    style.ItemInnerSpacing = ImVec2(4, 4);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 21;
    style.ScrollbarSize = 14;
    style.GrabMinSize = 8;

    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = 0;
    style.TabBorderSize = 0;

    style.WindowRounding = 4;
    style.ChildRounding = 1;
    style.FrameRounding = 8;
    style.PopupRounding = 0;
    style.ScrollbarRounding = 8;
    style.GrabRounding = 12;
    style.LogSliderDeadzone = 4;

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.07f, 0.27f, 0.75f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.67f, 0.00f, 0.75f, 0.46f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.74f, 0.00f, 0.73f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.30f, 0.17f, 0.60f, 0.29f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.30f, 0.17f, 0.60f, 0.67f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.25f, 0.09f, 0.73f, 0.55f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.10f, 0.10f, 0.36f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.11f, 0.07f, 0.56f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.07f, 0.23f, 0.60f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.37f, 0.81f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.37f, 0.81f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.60f, 0.13f, 1.00f, 0.61f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.51f, 0.00f, 0.86f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.23f, 1.00f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.15f, 0.12f, 0.34f, 0.58f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.09f, 0.18f, 0.48f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.10f, 0.31f, 0.56f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.79f, 0.34f, 0.82f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.31f, 0.19f, 0.51f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.37f, 0.20f, 0.91f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.66f, 0.17f, 0.82f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void UI::Render()
{
    SetStyle();

    if (ImGui::BeginMainMenuBar())
    {
#ifdef _DEBUG
        ImGui::Text("Project DELTA v2.xx | Debug Version");
#else
        ImGui::Text("Project DELTA v2.02 by Archie");
#endif
        ImGui::Checkbox("Invoker Window", &UI::bInvokerWnd);

        ImGui::EndMainMenuBar();
    }

    SDK::Structs::Read();

    switch (Core::CurrentGame)
    {
    case Core::GameType::Deltarune:
        RenderDeltarune();
        break;
    case Core::GameType::Undertale:
        RenderUndertale();
        break;
    case Core::GameType::Underswap:
        RenderUnderswap();
        break;
    default:
        RenderDefault();
        break;
    }

    if (bInvokerWnd)
        RenderInvoker();

    SDK::Structs::Write();
}

void UI::RenderDefault()
{
    ImGui::SetNextWindowSize(ImVec2(flWindowX, flWindowY));
    if (ImGui::Begin("Project DELTA", NULL,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings
    ))
    {
        ImGui::Text("Could not detect the current game.");
        ImGui::Text("If the automatic detection failed, you may select the game here.");
        ImGui::Text("If you are not running a supported game, do not select anything.");

        if (ImGui::Button("Force Undertale", ImVec2(140, 30)))
        {
            Core::CurrentGame = Core::GameType::Undertale;
            SDK::Structs::Read();
        }

        ImGui::SameLine();

        if (ImGui::Button("Force Deltarune", ImVec2(140, 30)))
        {
            Core::CurrentGame = Core::GameType::Deltarune;
            SDK::Structs::Read();
        }

        ImGui::SameLine();

        if (ImGui::Button("Force Underswap", ImVec2(140, 30)))
        {
            Core::CurrentGame = Core::GameType::Underswap;
            SDK::Structs::Read();
        }

        ImGui::NewLine();

        if (!bInvokerWnd)
            RenderInvoker();

        ImGui::End();
    }
}

void UI::RenderInvoker()
{
    static char funcName[64] = { 0 }; //Don't overflow me please :(
    static std::vector<RValue> vArgs;

    ImGui::SetNextWindowSize(ImVec2(480, 540));

    if (UI::bInvokerWnd)
        ImGui::Begin("Invoker", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
    else
        ImGui::BeginChild("ch_Invoker", ImVec2(460, 280), true);

    {
        static int nRoom = 0;

        ImGui::Combo("Target room", &nRoom, Core::RoomList);

        if (ImGui::Button("Teleport!", ImVec2(75, 30)))
        {
            Invoker::invoke("room_goto", { RValue((double)(nRoom)) });
        }

        if (ImGui::BeginChild("ch_Invoker2", ImVec2(450, bInvokerWnd ? 420 : 250), true))
        {
            ImGui::Text("Custom Invoker");

            ImGui::InputText("Function Name", funcName, 64);

            if (ImGui::Button("Add argument", ImVec2(140, 30)))
                vArgs.push_back(RValue(nullptr));

            ImGui::SameLine();

            if (ImGui::Button("Remove argument", ImVec2(140, 30)) && !vArgs.empty())
                vArgs.pop_back();

            for (int i = 0; i < vArgs.size(); i++)
            {
                if (ImGui::BeginChild(std::string("Arg " + std::to_string(i)).c_str() , ImVec2(440, 140), true, ImGuiWindowFlags_NoScrollWithMouse))
                {
                    static const char* szKinds[] = { 
                        "Real", 
                        "String",
                        "Array", 
                        "Pointer", 
                        "Vector (3D)", 
                        "Undefined",
                        "Object",
                        "32-bit Integer",
                        "Vector (4D)",
                        "Matrix (4x4)",
                        "64-bit Integer"
                    };
                    ImGui::Text("Argument %i", i);

                    ImGui::InputDouble("Real Value", &vArgs.at(i).dValue, 0, 0, "%.2f");
                    ImGui::InputInt("Integer Value", &vArgs.at(i).iValue);
                    ImGui::Combo("Kind", &vArgs.at(i).nKind, szKinds, IM_ARRAYSIZE(szKinds));
                }
                ImGui::EndChild();
            }

            if (ImGui::Button("Invoke!", ImVec2(75, 30)))
            {
                Invoker::invoke(funcName, vArgs);
            }
        }
        ImGui::EndChild();
    }

    if (UI::bInvokerWnd)
        ImGui::End();
    else
        ImGui::EndChild();
}

void UI::Init(PDIRECT3DDEVICE9 pDX9Device)
{
    namespace fs = std::filesystem;

	if (!bInit)
	{
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Memory::GetCurrentWindow());
		ImGui_ImplDX9_Init(pDX9Device);

		ImGuiIO& curIO = ImGui::GetIO();

        auto SystemRoot = std::string(getenv("SystemRoot"));
        if (fs::exists(SystemRoot + "\\Fonts\\bahnschrift.ttf"))
            curIO.Fonts->AddFontFromFileTTF(std::string(SystemRoot + "\\Fonts\\bahnschrift.ttf").c_str(), 18.0F);
        else
            curIO.Fonts->AddFontFromFileTTF(std::string(SystemRoot + "\\Fonts\\tahoma.ttf").c_str(), 16.0F);

		bInit = true;
	}
}

void UI::Init(ID3D11Device* pDX11Device, ID3D11DeviceContext* pContext)
{
    namespace fs = std::filesystem;

	if (!bInit)
	{
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Memory::GetCurrentWindow());
		ImGui_ImplDX11_Init(pDX11Device, pContext);

		ImGuiIO& curIO = ImGui::GetIO();
        
        auto SystemRoot = std::string(getenv("SystemRoot"));
        if (fs::exists(SystemRoot + "\\Fonts\\bahnschrift.ttf"))
            curIO.Fonts->AddFontFromFileTTF(std::string(SystemRoot + "\\Fonts\\bahnschrift.ttf").c_str(), 18.0F);
        else
            curIO.Fonts->AddFontFromFileTTF(std::string(SystemRoot + "\\Fonts\\tahoma.ttf").c_str(), 16.0F);

		bInit = true;
	}
}