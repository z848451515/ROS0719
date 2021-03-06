/*
* Ros D3D 1.1c by n7
How to compile:
- compile with visual studio community 2017 (..\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe)
- select Release x86
- click: project -> properties -> configuration properties -> general -> character set -> change to "not set"
- compile with CTRL + Shift + B
Optional: remove dependecy on vs runtime:
- click: project -> properties -> configuration properties -> C/C++ -> code generation -> runtime library: Multi-threaded (/MT)
@abuali for offset
last edit by       @danielrumasondi
*/

#include <windows.h>
#include "Main.h" //less important stuff & helper funcs here

using namespace std;



typedef HRESULT(__stdcall* SetTexture_t)(IDirect3DDevice9*, DWORD, IDirect3DBaseTexture9*);
SetTexture_t SetTexture_orig;


typedef HRESULT(__stdcall* tPresent)(IDirect3DDevice9*, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
tPresent	pPresent = 0;

typedef HRESULT(__stdcall* Reset_t)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
Reset_t OnReset = NULL;

//==========================================================================================================================

HRESULT __stdcall SetTexture_hook(LPDIRECT3DDEVICE9 pDevice, DWORD Sampler, IDirect3DBaseTexture9 *pTexture)
{
	if (InitOnce)
	{
		InitOnce = false;

		//get viewport
		pDevice->GetViewport(&Viewport);
		ScreenCX = (float)Viewport.Width / 2.0f;
		ScreenCY = (float)Viewport.Height / 2.0f;

		//GenerateTexture(pDevice, &Red, D3DCOLOR_ARGB(255, 255, 0, 0));
		//GenerateTexture(pDevice, &Green, D3DCOLOR_RGBA(0, 255, 0, 255));
		//GenerateTexture(pDevice, &Blue, D3DCOLOR_ARGB(255, 0, 0, 255));
		//GenerateTexture(pDevice, &Yellow, D3DCOLOR_ARGB(255, 255, 255, 0));

		//LoadCfg();
	}

	//get vSize
	if (SUCCEEDED(pDevice->GetVertexShader(&vShader)))
		if (vShader != NULL)
			if (SUCCEEDED(vShader->GetFunction(NULL, &vSize)))
				if (vShader != NULL) { vShader->Release(); vShader = NULL; }

	//model rec sample
	//Stride == 20 && vSize == 2008
	//Stride == 36 && vSize == 2356

	if (wallhack>0)
	{
		pDevice->SetRenderState(D3DRS_DEPTHBIAS, 0);
		if ((vSize == 2356 || vSize == 900 || vSize == 1004 || vSize == 2008 || vSize == 640) || (Stride == 36 && vSize == 1436) || (Stride == 48 && vSize == 1436))
			//if (vSize == 2008|| vSize == 2356|| vSize == 640 || vSize == 1436)
		{
			if (wallhack == 2 && vSize != 1436)
			{
				float sColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f };//yellow
				pDevice->SetPixelShaderConstantF(0, sColor, 1);
				//SetTexture_orig(pDevice, 0, Red);
				//SetTexture_orig(pDevice, 1, Red);
			}
			if (wallhack == 3 && vSize != 1436)
			{
				float sColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };//red
				pDevice->SetPixelShaderConstantF(0, sColor, 1);
				//SetTexture_orig(pDevice, 0, Red);
				//SetTexture_orig(pDevice, 1, Red);
			}
			if (wallhack == 4 && vSize != 1436)
			{
				float sColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };//blue
				pDevice->SetPixelShaderConstantF(0, sColor, 1);
				//SetTexture_orig(pDevice, 0, Red);
				//SetTexture_orig(pDevice, 1, Red);
			}
			if (wallhack == 5 && vSize != 1436)
			{
				float sColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };//green
				pDevice->SetPixelShaderConstantF(0, sColor, 1);
				//SetTexture_orig(pDevice, 0, Red);
				//SetTexture_orig(pDevice, 1, Red);
			}
			if (wallhack == 6 && vSize != 1436)
			{
				float sColor[4] = { 1.0f, 0.0f, 1.0f, 1.0f };//purple
				pDevice->SetPixelShaderConstantF(0, sColor, 1);
				//SetTexture_orig(pDevice, 0, Red);
				//SetTexture_orig(pDevice, 1, Red);
			}
			if (wallhack == 7 && vSize != 1436)
			{
				float sColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };//white
				pDevice->SetPixelShaderConstantF(0, sColor, 1);
				//SetTexture_orig(pDevice, 0, Red);
				//SetTexture_orig(pDevice, 1, Red);
			}
			if (wallhack == 8 && vSize != 1436)
			{
				float sColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f };//full yellow
				pDevice->SetPixelShaderConstantF(0, sColor, 1);
				//SetTexture_orig(pDevice, 0, Red);
				//SetTexture_orig(pDevice, 1, Red);
			}

			float bias = 1000.0f;
			float bias_float = static_cast<float>(-bias);
			bias_float /= 10000.0f;
			pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&bias_float);
		}
	}

	//worldtoscreen weapons in hand

	if (aimbot == 1 || esp > 0)
	{
		if ((Stride == 48 && vSize > 1328) || (vSize == 2356 || vSize == 2008 || vSize == 1552 || vSize == 1004))//1040crap,1328crap
			AddWeapons(pDevice);
	}




	return SetTexture_orig(pDevice, Sampler, pTexture);
}

//==========================================================================================================================

HRESULT __stdcall Reset_hook(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	if (Font)
		Font->OnLostDevice();

	HRESULT ResetReturn = OnReset(pDevice, pPresentationParameters);

	if (SUCCEEDED(ResetReturn))
	{
		if (Font)
			Font->OnResetDevice();

		InitOnce = true;
	}

	return ResetReturn;
}

HRESULT __stdcall hPresent(IDirect3DDevice9* pDevice, RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	if (GetAsyncKeyState(VK_ESCAPE) & 1 || GetAsyncKeyState(VK_INSERT))
	{
		//get viewport
		pDevice->GetViewport(&Viewport);
		ScreenCX = (float)Viewport.Width / 2.0f;
		ScreenCY = (float)Viewport.Height / 2.0f;
	}

	//create font
	if (Font == NULL)
		D3DXCreateFont(pDevice, 14, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Italic"), &Font);

	if (ShowMenu)
		//draw background
		DrawBox(pDevice, 71.0f, 86.0f, 200.0f, 380.0f, D3DCOLOR_ARGB(200, 20, 20, 20));//180 = up/down, 200 = left/right
	if (Font)
		DrawMenu(pDevice);

	//call feature
	//if (*Verify == 1) {
	//	CallMe();
	//}


	//Shift|RMouse|LMouse|Ctrl|Alt|Space|X|C
	if (aimkey == 0) Daimkey = 0;
	if (aimkey == 1) Daimkey = VK_SHIFT;
	if (aimkey == 2) Daimkey = VK_RBUTTON;
	if (aimkey == 3) Daimkey = VK_LBUTTON;
	if (aimkey == 4) Daimkey = VK_CONTROL;
	if (aimkey == 5) Daimkey = VK_MENU;
	if (aimkey == 6) Daimkey = VK_SPACE;
	if (aimkey == 7) Daimkey = 0x58; //X
	if (aimkey == 8) Daimkey = 0x43; //C




									 //do esp
	if (esp > 0 && WeaponEspInfo.size() != NULL)
	{
		for (unsigned int i = 0; i < WeaponEspInfo.size(); i++)
		{

			//box esp
			if (WeaponEspInfo[i].pOutX > 1.0f && WeaponEspInfo[i].pOutY > 1.0f && (float)WeaponEspInfo[i].RealDistance > 4.0f)
				DrawCornerBox(pDevice, (int)WeaponEspInfo[i].pOutX, (int)WeaponEspInfo[i].pOutY + 20, 20, 30, 1, D3DCOLOR_ARGB(255, 255, 255, 255));

			//line esp
			if (WeaponEspInfo[i].pOutX > 1.0f && WeaponEspInfo[i].pOutY > 1.0f && (float)WeaponEspInfo[i].RealDistance > 4.0f)//&& (float)WeaponEspInfo[i].vSizeod == 2008)//long range weapon
				DrawLine(pDevice, (int)WeaponEspInfo[i].pOutX, (int)WeaponEspInfo[i].pOutY, ScreenCX, ScreenCY * ((float)esp * 0.2f), 20, D3DCOLOR_ARGB(255, 255, 255, 255), 1);//0.1up, 1.0middle, 2.0down
																																												//else if (WeaponEspInfo[i].pOutX > 1.0f && WeaponEspInfo[i].pOutY > 1.0f && (float)WeaponEspInfo[i].RealDistance > 4.0f && (float)WeaponEspInfo[i].vSizeod != 2008)//short/mid range weapon
																																												//DrawLine(pDevice, (int)WeaponEspInfo[i].pOutX, (int)WeaponEspInfo[i].pOutY, ScreenCX, ScreenCY * ((float)esp * 0.2f), 20, D3DCOLOR_ARGB(255, 0, 255, 0), 1);//0.1up, 1.0middle, 2.0down

																																												//distance esp
			if (WeaponEspInfo[i].pOutX > 1.0f && WeaponEspInfo[i].pOutY > 1.0f && (float)WeaponEspInfo[i].RealDistance > 200.0f)
				DrawCenteredString(Font, (int)WeaponEspInfo[i].pOutX, (int)WeaponEspInfo[i].pOutY - 20.0f, D3DCOLOR_ARGB(255, 255, 255, 255), (PCHAR)"%.f", (float)WeaponEspInfo[i].RealDistance);


			else if (WeaponEspInfo[i].pOutX > 1.0f && WeaponEspInfo[i].pOutY > 1.0f && (float)WeaponEspInfo[i].RealDistance > 4.0f && (float)WeaponEspInfo[i].RealDistance <= 200.0f)
				DrawCenteredString(Font, (int)WeaponEspInfo[i].pOutX, (int)WeaponEspInfo[i].pOutY - 20.0f, D3DCOLOR_ARGB(255, 255, 255, 0), (PCHAR)"%.f", (float)WeaponEspInfo[i].RealDistance);

			//text esp
			//if (WeaponEspInfo[i].pOutX > 1.0f && WeaponEspInfo[i].pOutY > 1.0f && (float)WeaponEspInfo[i].distance > 4.0f)
			//DrawString(Font, (int)WeaponEspInfo[i].pOutX, (int)WeaponEspInfo[i].pOutY, D3DCOLOR_ARGB(255, 255, 255, 255), "o");
		}
	}


	//do aim
	if (aimbot == 1 && WeaponEspInfo.size() != NULL)
	{
		UINT BestTarget = -1;
		DOUBLE fClosestPos = 99999;

		for (unsigned int i = 0; i < WeaponEspInfo.size(); i++)
		{
			//aimfov
			float radiusx = (aimfov*5.0f) * (ScreenCX / 100.0f);
			float radiusy = (aimfov*5.0f) * (ScreenCY / 100.0f);

			if (aimfov == 0)
			{
				radiusx = 5.0f * (ScreenCX / 100.0f);
				radiusy = 5.0f * (ScreenCY / 100.0f);
			}

			//get crosshairdistance
			WeaponEspInfo[i].CrosshairDistance = GetDistance(WeaponEspInfo[i].pOutX, WeaponEspInfo[i].pOutY, ScreenCX, ScreenCY);

			//if in fov
			if (WeaponEspInfo[i].pOutX >= ScreenCX - radiusx && WeaponEspInfo[i].pOutX


				<= ScreenCX + radiusx && WeaponEspInfo[i].pOutY >=



				ScreenCY - radiusy && WeaponEspInfo[i].pOutY <= ScreenCY + radiusy)

				//get closest/nearest target to crosshair
				if (WeaponEspInfo[i].CrosshairDistance < fClosestPos)
				{
					fClosestPos = WeaponEspInfo[i].CrosshairDistance;
					BestTarget = i;
				}
		}


		//if nearest target to crosshair

		if (BestTarget != -1 && WeaponEspInfo[BestTarget].RealDistance > 4.0f)//do not aim at self
		{
			double DistX1 = WeaponEspInfo[BestTarget].pOutX - ScreenCX;
			double DistY1 = WeaponEspInfo[BestTarget].pOutY - ScreenCY;
			/////

			double DistX2 = WeaponEspInfo[BestTarget].pOutX - ScreenCX;
			double DistY2 = WeaponEspInfo[BestTarget].pOutY - ScreenCY;
			////

			double DistX3 = WeaponEspInfo[BestTarget].pOutX - ScreenCX;
			double DistY3 = WeaponEspInfo[BestTarget].pOutY - ScreenCY;

			////
			DistX1 /= (0.6f + (float)aimsens*0.6f / 2);
			DistY1 /= (0.6f + (float)aimsens*0.6f / 2);


			DistX2 /= (0.6f + (float)aimsens*0.6f / 1);
			DistY2 /= (0.6f + (float)aimsens*0.6f / 1);

			DistX3 /= (0.6f + (float)aimsens*4.0f / 2);
			DistY3 /= (0.6f + (float)aimsens*4.0f / 2);



			//aim
			if (GetAsyncKeyState(Daimkey) & 0x8000)
			{
				mouse_event(MOUSEEVENTF_MOVE, (float)DistX1 / 2, (float)DistY1 / 2, 0, NULL);
				Sleep(6);
				mouse_event(MOUSEEVENTF_MOVE, (float)DistX2 / 12, (float)DistY2 / 12, 0, NULL);
				Sleep(6);
				mouse_event(MOUSEEVENTF_MOVE, (float)DistX3 / 2, (float)DistY3 / 2, 0, NULL);

			}


			//autoshoot on
			if ((!GetAsyncKeyState(VK_LBUTTON) && (autoshoot == 1) && (GetAsyncKeyState(Daimkey) & 0x8000))) //
			{
				if (autoshoot == 1 && !IsPressed)
				{
					IsPressed = true;
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				}
			}
		}
	}
	WeaponEspInfo.clear();


	//croshair
	if (crosshair == 1)
	{
		CrossHair(pDevice, D3DCOLOR_ARGB(255, 255, 255, 0)); //yellow
	}
	return pPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}


DWORD __stdcall ThreadD3D(__in LPVOID lpParameter)
{

	HMODULE dDll = NULL;
	while (!dDll)
	{
		dDll = GetModuleHandleA("d3d9.dll");
		Sleep(100);
	}
	CloseHandle(dDll);

	IDirect3D9* d3d = NULL;
	IDirect3DDevice9* d3ddev = NULL;

	HWND tmpWnd = CreateWindowA("BUTTON", "RosD3D", WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, Hand, NULL);
	if (tmpWnd == NULL)
	{

		return 0;
	}

	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL)
	{
		DestroyWindow(tmpWnd);

		return 0;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = tmpWnd;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	HRESULT result = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, tmpWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
	if (result != D3D_OK)
	{
		d3d->Release();
		DestroyWindow(tmpWnd);

		return 0;
	}

	//ros.exe+9E645C - \x0F\x5E\xD8 - off
	//ros.exe+9E645C - \x10\x5E\xD8 - on


#if defined _M_X64
	DWORD64* dVtable = (DWORD64*)d3ddev;
	dVtable = (DWORD64*)dVtable[0];
#elif defined _M_IX86
	DWORD* dVtable = (DWORD*)d3ddev;
	dVtable = (DWORD*)dVtable[0];
#endif


	//pPresent = (tPresent)DetourFunction((PBYTE)Table[45], (PBYTE)Present_hook);
	//DetourFunction((PBYTE)Table[45], (PBYTE)Present_hook);
	pPresent = (tPresent)DetourFunction((PBYTE)dVtable[17], (PBYTE)hPresent);
	SetTexture_orig = (SetTexture_t)DetourFunction((PBYTE)dVtable[65], (PBYTE)SetTexture_hook);
	OnReset = (Reset_t)DetourFunction((PBYTE)dVtable[16], (PBYTE)Reset_hook);


	return 1;
}

//to check ya ben shel zuna sharmuta lama at israeloli ya zuna sharmota lol lech timitzoch li ta tachchat ya koksinel homo
//ros.exe+1B57D76 - 69 72 65 64206675     - imul esi,[edx+65],iertutil.Ordinal701+5E994 { ["ings\SO\ACTIVE_CONTENT\VALIDATION\ENABLE!HKeyRoot"] }
//
void WriteProtection(void *adress, void *bytes, int size)
{
	DWORD dwProtect;
	VirtualProtect((void*)adress, size, PAGE_EXECUTE_READWRITE, &dwProtect);
	memcpy((void *)adress, (PBYTE)bytes, size);
	VirtualProtect((void*)adress, size, dwProtect, &dwProtect);
}

DWORD __stdcall OverwriteValues()
{
	for (;; Sleep(20))
	{
		//No fall 
		if (nofalldmg == 1)
		{
			PatchHook((void *)(MODULEROS + 0x56EECA), (int*)(PBYTE)"\xF6\xC2\x06", 3);
		}
		if (nofalldmg == 0)
		{
			PatchHook((void *)(MODULEROS + 0x56EECA), (int*)(PBYTE)"\xF6\xC2\x02", 3);
		}
		//************************************
		//make you under the fucking water YEET make your teamates crying
		if (underwater == 0)
		{
			PatchHook((void *)(MODULEROS + 0x3B748C), (int*)(PBYTE)"\x91\x00", 2);
		}
		if (underwater == 1)
		{
			PatchHook((void *)(MODULEROS + 0x3B748C), (int*)(PBYTE)"\x91\x00", 2);
		}
		//************************************
		//No fog :D
		BYTE FogOn[5] = { 0xF3,0x0F,0x11,0x4D,0xE8 };
		BYTE FogOFF[5] = { 0xF3,0x0F,0x11,0x45,0xE8 };
		if (nofog == 1)
		{
			WriteProtection((void*)(MODULEROS + 0xC37FFE), (void*)FogOn, 5);
		}
		if (nofog == 0)
		{
			WriteProtection((void*)(MODULEROS + 0xC37FFE), (void*)FogOFF, 5);
		}
		//************************************
		//No recoil but do not use it becuase it kindda ez 3 days ban
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
			PatchHook((void*)(recoil), (int*)(PBYTE)"\xF2\x0F\x5E\x0D", 4);//No recoil
		}
		//************************************
		//LUL make your teamates crying
		if (walkthrough == 1)
		{
			PatchHook((void *)(MODULEROS + 0x179B338), (int*)(PBYTE)"\x66\x66\x66\xBF", 4);
		}
		if (walkthrough == 0)
		{
			PatchHook((void *)(MODULEROS + 0x179B338), (int*)(PBYTE)"\x00\x00\x00\xBF", 4);
		}
		//************************************
		//	//jumphack - fly like huge boobs in the sky (if you short like me)
		//if (jumphack == 1)
		//{
		//	PatchHook((void *)(MODULEROS + 0x5452D7), (void *)(PBYTE) "\xC7\x82\x9C\x00\x00\x00\x00\x00\x7A\x43", 10);
		//}
		//if (jumphack == 0)
		//{
		//	PatchHook((void *)(MODULEROS + 0x5452D7), (void *)(PBYTE) "\xF3\x0F\x11\x82\x9C\x00\x00\x00\x8B\x06", 10);
		//}
		//************************************
		//not really work .. Just hide the spectecshitors for few sec util the server send you it again maybe for some it does work but for me it isnt working lul
		if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
		{
			*(byte*)(MODULEROS + 0x159986) = 2; //ON HOTKEY : NUMPAD1
		}
		/* reconnect */
		if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
		{
			*(byte*)(MODULEROS + 0x159986) = 1; //OFF HOTKEY : NUMPAD2
		}
		//************************************
		//speed hacks to become THE FLASH :D
		bool PressingR = (bool)(GetAsyncKeyState(VK_XBUTTON2) & 0x8000); // true if R is currently held down
		static bool IsPatched = false; // to know whether already patched or not
		if (PressingR && !IsPatched) {
			PatchHook((void *)(MODULEROS + 0x1E6E48C), (int*)(PBYTE)"\x00\x00\xC6\x42", 4);//Telekill
			PatchHook((void *)(MODULEROS + 0x17A551C), (int*)(PBYTE)"\x00\x00\xFA\x43", 4);//speedhack
			IsPatched = true;
		}
		else if (!PressingR && IsPatched) {
			PatchHook((void *)(MODULEROS + 0x1E6E48C), (int*)(PBYTE)"\x0A\xD7\xA3\x3C", 4);//Telekill
			if (fastrun == 1)
			{
				PatchHook((void *)(MODULEROS + 0x17A551C), (int*)(PBYTE)"\x00\x00\xD2\x42", 4);
			}
			if (fastrun == 0)
			{
				PatchHook((void *)(MODULEROS + 0x17A551C), (int*)(PBYTE)"\x00\x00\x70\x42", 4);
			}
			IsPatched = false;
		}
		//Car speed
		if (superCarSuperSpeed == 1)
		{
			PatchHook((void*)(MODULEROS + 0x172FA70), (void*)(PBYTE)"\x6F\x12\xFA", 3);// search via CE 0.001000000047f;
		}
		if (superCarSuperSpeed == 0)
		{
			PatchHook((void*)(MODULEROS + 0x172FA70), (void*)(PBYTE)"\x6F\x12\x83", 3);// search via CE 0.001000000047f;
		}
		//************************************
		//		//colors
		//if (GlowItems == 0)//Green
		//{
		//	PatchHook((void *)(MODULEROS + 0x3FB3B9), (void *)(PBYTE) "\x66\x0F\x5E\xD1", 4);//turn it to Green
		//	PatchHook((void *)(MODULEROS + 0x3FB3C1), (void *)(PBYTE) "\x66\x0F\x11\x45", 4);//turn it to Purple
		//}
		//if (GlowItems == 1)//Purple
		//{
		//	PatchHook((void *)(MODULEROS + 0x3FB3C1), (void *)(PBYTE) "\x66\x0F\x11\x4D", 4);//turn it to Purple
		//	PatchHook((void *)(MODULEROS + 0x3FB3B9), (void *)(PBYTE) "\x66\x0F\x5E\xC1", 4);//turn it to Green
		//}
		//if (WhiteItems == 1)//on
		//{
		//	PatchHook((void *)(MODULEROS + 0x3FB3DF), (void *)(PBYTE) "\x66\x0F\x6E\xC8", 4);
		//}
		//if (WhiteItems == 0)//off
		//{
		//	PatchHook((void *)(MODULEROS + 0x3FB3DF), (void *)(PBYTE) "\x66\x0F\x6E\xC0", 4);
		//}
		//************************************
		//MasterSpeed on XBUTTON1 ez fast reload when you need
		bool PressingRsss = (bool)(GetAsyncKeyState(VK_XBUTTON1) & 0x8000);
		static bool IsPatchedsass = false;

		if (PressingRsss && !IsPatchedsass) {
			PatchHook((void *)(MODULEROS + 0x739CC), (void *)(PBYTE) "\xC3\x66", 2);
			IsPatchedsass = true;
		}
		else if (!PressingRsss && IsPatchedsass) {
			PatchHook((void *)(MODULEROS + 0x739CC), (void *)(PBYTE) "\xC2\x66", 2);
			IsPatchedsass = false;
		}
		//************************************
		//	//gravemode to feel a fucking god 
		//if (gravemode == 1)
		//{
		//	PatchHook((void *)(MODULEROS + 0x5467B8), (void *)(PBYTE) "\x50\x58\x50\x58\x50\x58\x50\x58", 8);
		//}
		//if (gravemode == 0)
		//{
		//	PatchHook((void *)(MODULEROS + 0x5467B8), (void *)(PBYTE) "\xF3\x0F\x11\x87\xF4\x00\x00\x00", 8);
		//}
		//************************************

		//Anti screenshot
		PatchHook((void*)(MODULEROS + 0x172AD8E), (int*)(PBYTE)"\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58\x50\x58", 40);
	}

}
void _stdcall CallMe()
{
	for (;; Sleep(0))
	{
		if (GetAsyncKeyState(VK_F1) & 0x8000)
		{
			DWORD Val = FloatToDword(400.0);
			__try {
				//*(PDWORD)PatchPTR((MODULEROS + 0x21A600C), 0x0, 0x40, 0x4, 0x10, 0x9C, 0, 0) = Val;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {}
		}

		if (GetAsyncKeyState(VK_F2) & 0x8000)
		{
			DWORD Val = FloatToDword(-804.0);
			__try {
				//*(PDWORD)PatchPTR((MODULEROS + 0x21A600C), 0x0, 0x40, 0x4, 0x10, 0xAC, 0, 0) = Val;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {}
		}
	}
}

//==========================================================================================================================
BOOL __stdcall DllMain(HMODULE hDll, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH: // A process is loading the DLL.
		Hand = hDll;
		DisableThreadLibraryCalls(hDll); // disable unwanted thread notifications to reduce overhead
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SearchPatterns, hDll, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadD3D, hDll, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OverwriteValues, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CallMe, NULL, NULL, NULL);
		//shit();
		//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Present_hook, hDll, NULL, NULL);

		GetModuleFileNameA(hDll, dlldir, 512);
		for (int i = (int)strlen(dlldir); i > 0; i--)
		{
			if (dlldir[i] == '\\')
			{
				dlldir[i + 1] = 0;
				break;
			}
		}
		break;

	case DLL_PROCESS_DETACH: // A process unloads the DLL.
		break;
	}
	return TRUE;
}
