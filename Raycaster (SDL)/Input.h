#pragma once

#include <SDL.h>

/// <summary>
/// A static class that handles SDL inputs.
/// </summary>
static class Input
{
public:
	// Keyboard Input Functions
	/// <summary>
	/// Checks if the key is being pressed.
	/// </summary>
	/// <param name="key">The key you want to check is being pressed.</param>
	/// <returns>true if the key is currently pressed.</returns>
	static bool GetKey(SDL_Scancode key);
	/// <summary>
	/// Checks if the key has just been pressed (shoots on the first frame a press is detected in the keys state).
	/// </summary>
	/// <param name="key">The key you want to check is being pressed.</param>
	/// <returns>true if the key has been pressed this frame.</returns>
	static bool GetKeyDown(SDL_Scancode key);
	/// <summary>
	/// Checks if the key has just been released (shoots on the first frame a release is detected in the keys state).
	/// </summary>
	/// <param name="key">The key you want to check is being pressed.</param>
	/// <returns>true if the key has been released this frame.</returns>
	static bool GetKeyUp(SDL_Scancode key);

	// Functions used for class maintence
	/// <summary>
	/// Initialize the Input class.
	/// </summary>
	static void Initialize();
	/// <summary>
	/// Polling Event Handler (used for the window exit, minimize, fullsceen buttons mainly) - runs every frame.
	/// </summary>
	/// <returns>true if the quit button has NOT been pressed.</returns>
	static bool EventHandler();
	/// <summary>
	/// Updates the current keyboard state and previous keyboard state - runs every frame.
	/// </summary>
	static void Update();
	/// <summary>
	/// Frees up memory on application end.
	/// </summary>
	static void Cleanup();

private:
	// The current state of the keyboard this frame.
	static const Uint8* m_keyboardState;
	// The state of the keyboard last frame.
	static const Uint8* m_previousKeyboardState;
};
