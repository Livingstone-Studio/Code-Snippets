#include "Input.h"

const Uint8* Input::m_keyboardState;
const Uint8* Input::m_previousKeyboardState;

// Keyboard Input Functions

bool Input::GetKey(SDL_Scancode key)
{
    // Returns true if the key is pressed, false if the key is idle.
    return m_keyboardState[key];
}

bool Input::GetKeyDown(SDL_Scancode key)
{
    // Returns true if key is pressed this frame but was idle last frame.
    return m_keyboardState[key] && !(m_previousKeyboardState[key]);
}

bool Input::GetKeyUp(SDL_Scancode key)
{
    // Returns true if key was pressed last frame but is idle this frame.
    return !(m_keyboardState[key]) && m_previousKeyboardState[key];
}

// Functions used for class maintence

void Input::Initialize()
{
    // Returns a pointer to an internal SDL array that uses SDL_Scancodes to identify keyboard keys.
    m_keyboardState = SDL_GetKeyboardState(NULL);
    // Allocate memory to m_previousKeyboardState to match the same amount of space as m_keyboardState.
    m_previousKeyboardState = (Uint8*)malloc(SDL_NUM_SCANCODES);
}

bool Input::EventHandler()
{
    // Local declaration of an sdl event.
    SDL_Event m_event;

    // Loops until all input events are handled.
    while (SDL_PollEvent(&m_event))
    {
        // Checks if the event type is the 'X' button in the top right of the window, if so - return false.
        switch (m_event.type)
        {
        case SDL_QUIT:
            return false;
            break;
        }
    }

    return true;
}

void Input::Update()
{
    // Copies the current data from m_keyboardState into m_previousKeyboardState.
    SDL_memcpy((void*)m_previousKeyboardState, m_keyboardState, SDL_NUM_SCANCODES);
}

void Input::Cleanup()
{
    // Frees the allocated space dedicated to m_previousKeyboardState in Initalize.
    free((void*)m_previousKeyboardState);
}