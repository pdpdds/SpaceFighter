//=============================================================================
// DirectInput.cpp by Frank Luna (C) 2005 All Rights Reserved.
//=============================================================================

#include "stdafx.h"
#include "DirectInput.h"
#include "DirectXUtil.h"

DirectInput::DirectInput(HINSTANCE hInstance, HWND hHwnd, DWORD keyboardCoopFlags, DWORD mouseCoopFlags)
{
	ZeroMemory(mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&mMouseState, sizeof(mMouseState));

	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&mDInput, 0);

	mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0);
	mKeyboard->SetDataFormat(&c_dfDIKeyboard);
	mKeyboard->SetCooperativeLevel(hHwnd, keyboardCoopFlags);
	mKeyboard->Acquire();

	mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0);
	mMouse->SetDataFormat(&c_dfDIMouse2);
	mMouse->SetCooperativeLevel(hHwnd, mouseCoopFlags);
	mMouse->Acquire();
}

DirectInput::~DirectInput()
{
	SAFE_DELETE(mDInput);
	mKeyboard->Unacquire();
	mMouse->Unacquire();
	SAFE_DELETE(mKeyboard);
	SAFE_DELETE(mMouse);
}

void DirectInput::poll()
{
	// Poll keyboard.
	HRESULT hr = mKeyboard->GetDeviceState(sizeof(mKeyboardState), (void**)&mKeyboardState); 
	if( FAILED(hr) )
	{
		// Keyboard lost, zero out keyboard data structure.
		ZeroMemory(mKeyboardState, sizeof(mKeyboardState));

		 // Try to acquire for next time we poll.
		hr = mKeyboard->Acquire();
	}

	// Poll mouse.
	hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mMouseState); 
	if( FAILED(hr) )
	{
		// Mouse lost, zero out mouse data structure.
		ZeroMemory(&mMouseState, sizeof(mMouseState));

		// Try to acquire for next time we poll.
		hr = mMouse->Acquire(); 
	}
}

bool DirectInput::keyDown(char key)
{
	return (mKeyboardState[key] & 0x80) != 0;
}

bool DirectInput::mouseButtonDown(int button)
{
	return (mMouseState.rgbButtons[button] & 0x80) != 0;
}

float DirectInput::mouseDX()
{
	return (float)mMouseState.lX;
}

float DirectInput::mouseDY()
{
	return (float)mMouseState.lY;
}

float DirectInput::mouseDZ()
{
	return (float)mMouseState.lZ;
}