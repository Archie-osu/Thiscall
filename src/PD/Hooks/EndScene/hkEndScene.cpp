#include "hkEndScene.hpp"
#include "../../SDK/Void.hpp"
#include "../../SDK/Hook System/Hook System.hpp"
#include <mutex>

#include <ImGui/imgui_impl_dx9.h>
#include <ImGui/imgui_impl_win32.h>

#include "../../SDK/Callback Manager/Callback Manager.hpp"

inline std::once_flag Init;
HRESULT WINAPI Hooks::EndScene::Hook(LPDIRECT3DDEVICE9 lpDevice)
{
	Void.CallbackManager->Call(CCallbackManager::Types::FRAME_BEGIN, {});

	std::call_once(Init, [&]() {
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Void.GetGameWindow());
		ImGui_ImplDX9_Init(lpDevice);
	});

	auto Return = Void.HookSystem->GetOriginal<FN>("EndScene")(lpDevice);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	
	Void.CallbackManager->Call(CCallbackManager::Types::FRAME_RENDER, {});

	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	Void.CallbackManager->Call(CCallbackManager::Types::FRAME_END, {});
	return Return;
}

void* Hooks::EndScene::GetTargetAddress()
{
	void* ppTable[119]; //Array of void pointers (void**)

	auto Device = ReCa<LPDIRECT3DDEVICE9>(Void.GetGameDevice());

	if (!Device)
		Void.Error("D3DDevice was nullptr!");

	memcpy(ppTable, *ReCa<void***>(Device), sizeof(ppTable));

	return ppTable[42];
}