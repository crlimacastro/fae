#include "fae/sdl.hpp"

#include <bitset>
#include <memory>

#include <SDL3/SDL.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>

#include "fae/logging.hpp"
#include "fae/input.hpp"
#include "fae/windowing.hpp"
#include "fae/application/application.hpp"

[[nodiscard]] auto to_sdl_keycode(const fae::key key) noexcept -> SDL_Keycode
{
    switch (key)
    {
    case fae::key::unknown:
        return SDLK_UNKNOWN;
    case fae::key::enter:
        return SDLK_RETURN;
    case fae::key::escape:
        return SDLK_ESCAPE;
    case fae::key::backspace:
        return SDLK_BACKSPACE;
    case fae::key::tab:
        return SDLK_TAB;
    case fae::key::space:
        return SDLK_SPACE;
    case fae::key::exclaim:
        return SDLK_EXCLAIM;
    case fae::key::dblapostrophe:
        return SDLK_DBLAPOSTROPHE;
    case fae::key::hash:
        return SDLK_HASH;
    case fae::key::percent:
        return SDLK_PERCENT;
    case fae::key::dollar:
        return SDLK_DOLLAR;
    case fae::key::ampersand:
        return SDLK_AMPERSAND;
    case fae::key::apostrophe:
        return SDLK_APOSTROPHE;
    case fae::key::leftparen:
        return SDLK_LEFTPAREN;
    case fae::key::rightparen:
        return SDLK_RIGHTPAREN;
    case fae::key::asterisk:
        return SDLK_ASTERISK;
    case fae::key::plus:
        return SDLK_PLUS;
    case fae::key::comma:
        return SDLK_COMMA;
    case fae::key::minus:
        return SDLK_MINUS;
    case fae::key::period:
        return SDLK_PERIOD;
    case fae::key::slash:
        return SDLK_SLASH;
    case fae::key::num_0:
        return SDLK_0;
    case fae::key::num_1:
        return SDLK_1;
    case fae::key::num_2:
        return SDLK_2;
    case fae::key::num_3:
        return SDLK_3;
    case fae::key::num_4:
        return SDLK_4;
    case fae::key::num_5:
        return SDLK_5;
    case fae::key::num_6:
        return SDLK_6;
    case fae::key::num_7:
        return SDLK_7;
    case fae::key::num_8:
        return SDLK_8;
    case fae::key::num_9:
        return SDLK_9;
    case fae::key::colon:
        return SDLK_COLON;
    case fae::key::semicolon:
        return SDLK_SEMICOLON;
    case fae::key::less:
        return SDLK_LESS;
    case fae::key::equals:
        return SDLK_EQUALS;
    case fae::key::greater:
        return SDLK_GREATER;
    case fae::key::question:
        return SDLK_QUESTION;
    case fae::key::at:
        return SDLK_AT;
    case fae::key::leftbracket:
        return SDLK_LEFTBRACKET;
    case fae::key::backslash:
        return SDLK_BACKSLASH;
    case fae::key::rightbracket:
        return SDLK_RIGHTBRACKET;
    case fae::key::caret:
        return SDLK_CARET;
    case fae::key::underscore:
        return SDLK_UNDERSCORE;
    case fae::key::grave:
        return SDLK_GRAVE;
    case fae::key::a:
        return SDLK_A;
    case fae::key::b:
        return SDLK_B;
    case fae::key::c:
        return SDLK_C;
    case fae::key::d:
        return SDLK_D;
    case fae::key::e:
        return SDLK_E;
    case fae::key::f:
        return SDLK_F;
    case fae::key::g:
        return SDLK_G;
    case fae::key::h:
        return SDLK_H;
    case fae::key::i:
        return SDLK_I;
    case fae::key::j:
        return SDLK_J;
    case fae::key::k:
        return SDLK_K;
    case fae::key::l:
        return SDLK_L;
    case fae::key::m:
        return SDLK_M;
    case fae::key::n:
        return SDLK_N;
    case fae::key::o:
        return SDLK_O;
    case fae::key::p:
        return SDLK_P;
    case fae::key::q:
        return SDLK_Q;
    case fae::key::r:
        return SDLK_R;
    case fae::key::s:
        return SDLK_S;
    case fae::key::t:
        return SDLK_T;
    case fae::key::u:
        return SDLK_U;
    case fae::key::v:
        return SDLK_V;
    case fae::key::w:
        return SDLK_W;
    case fae::key::x:
        return SDLK_X;
    case fae::key::y:
        return SDLK_Y;
    case fae::key::z:
        return SDLK_Z;
    case fae::key::capslock:
        return SDLK_CAPSLOCK;
    case fae::key::f1:
        return SDLK_F1;
    case fae::key::f2:
        return SDLK_F2;
    case fae::key::f3:
        return SDLK_F3;
    case fae::key::f4:
        return SDLK_F4;
    case fae::key::f5:
        return SDLK_F5;
    case fae::key::f6:
        return SDLK_F6;
    case fae::key::f7:
        return SDLK_F7;
    case fae::key::f8:
        return SDLK_F8;
    case fae::key::f9:
        return SDLK_F9;
    case fae::key::f10:
        return SDLK_F10;
    case fae::key::f11:
        return SDLK_F11;
    case fae::key::f12:
        return SDLK_F12;
    case fae::key::printscreen:
        return SDLK_PRINTSCREEN;
    case fae::key::scrolllock:
        return SDLK_SCROLLLOCK;
    case fae::key::pause:
        return SDLK_PAUSE;
    case fae::key::insert:
        return SDLK_INSERT;
    case fae::key::home:
        return SDLK_HOME;
    case fae::key::pageup:
        return SDLK_PAGEUP;
    case fae::key::del:
        return SDLK_DELETE;
    case fae::key::end:
        return SDLK_END;
    case fae::key::pagedown:
        return SDLK_PAGEDOWN;
    case fae::key::right:
        return SDLK_RIGHT;
    case fae::key::left:
        return SDLK_LEFT;
    case fae::key::down:
        return SDLK_DOWN;
    case fae::key::up:
        return SDLK_UP;
    case fae::key::numlockclear:
        return SDLK_NUMLOCKCLEAR;
    case fae::key::kp_divide:
        return SDLK_KP_DIVIDE;
    case fae::key::kp_multiply:
        return SDLK_KP_MULTIPLY;
    case fae::key::kp_minus:
        return SDLK_KP_MINUS;
    case fae::key::kp_plus:
        return SDLK_KP_PLUS;
    case fae::key::kp_enter:
        return SDLK_KP_ENTER;
    case fae::key::kp_1:
        return SDLK_KP_1;
    case fae::key::kp_2:
        return SDLK_KP_2;
    case fae::key::kp_3:
        return SDLK_KP_3;
    case fae::key::kp_4:
        return SDLK_KP_4;
    case fae::key::kp_5:
        return SDLK_KP_5;
    case fae::key::kp_6:
        return SDLK_KP_6;
    case fae::key::kp_7:
        return SDLK_KP_7;
    case fae::key::kp_8:
        return SDLK_KP_8;
    case fae::key::kp_9:
        return SDLK_KP_9;
    case fae::key::kp_0:
        return SDLK_KP_0;
    case fae::key::kp_period:
        return SDLK_KP_PERIOD;
    case fae::key::application:
        return SDLK_APPLICATION;
    case fae::key::power:
        return SDLK_POWER;
    case fae::key::kp_equals:
        return SDLK_KP_EQUALS;
    case fae::key::f13:
        return SDLK_F13;
    case fae::key::f14:
        return SDLK_F14;
    case fae::key::f15:
        return SDLK_F15;
    case fae::key::f16:
        return SDLK_F16;
    case fae::key::f17:
        return SDLK_F17;
    case fae::key::f18:
        return SDLK_F18;
    case fae::key::f19:
        return SDLK_F19;
    case fae::key::f20:
        return SDLK_F20;
    case fae::key::f21:
        return SDLK_F21;
    case fae::key::f22:
        return SDLK_F22;
    case fae::key::f23:
        return SDLK_F23;
    case fae::key::f24:
        return SDLK_F24;
    case fae::key::execute:
        return SDLK_EXECUTE;
    case fae::key::help:
        return SDLK_HELP;
    case fae::key::menu:
        return SDLK_MENU;
    case fae::key::select:
        return SDLK_SELECT;
    case fae::key::stop:
        return SDLK_STOP;
    case fae::key::again:
        return SDLK_AGAIN;
    case fae::key::undo:
        return SDLK_UNDO;
    case fae::key::cut:
        return SDLK_CUT;
    case fae::key::copy:
        return SDLK_COPY;
    case fae::key::paste:
        return SDLK_PASTE;
    case fae::key::find:
        return SDLK_FIND;
    case fae::key::mute:
        return SDLK_MUTE;
    case fae::key::volumeup:
        return SDLK_VOLUMEUP;
    case fae::key::volumedown:
        return SDLK_VOLUMEDOWN;
    case fae::key::kp_comma:
        return SDLK_KP_COMMA;
    case fae::key::kp_equalsas400:
        return SDLK_KP_EQUALSAS400;
    case fae::key::alterase:
        return SDLK_ALTERASE;
    case fae::key::sysreq:
        return SDLK_SYSREQ;
    case fae::key::cancel:
        return SDLK_CANCEL;
    case fae::key::clear:
        return SDLK_CLEAR;
    case fae::key::prior:
        return SDLK_PRIOR;
    case fae::key::return2:
        return SDLK_RETURN2;
    case fae::key::separator:
        return SDLK_SEPARATOR;
    case fae::key::out:
        return SDLK_OUT;
    case fae::key::oper:
        return SDLK_OPER;
    case fae::key::clearagain:
        return SDLK_CLEARAGAIN;
    case fae::key::crsel:
        return SDLK_CRSEL;
    case fae::key::exsel:
        return SDLK_EXSEL;
    case fae::key::kp_00:
        return SDLK_KP_00;
    case fae::key::kp_000:
        return SDLK_KP_000;
    case fae::key::thousandsseparator:
        return SDLK_THOUSANDSSEPARATOR;
    case fae::key::decimalseparator:
        return SDLK_DECIMALSEPARATOR;
    case fae::key::currencyunit:
        return SDLK_CURRENCYUNIT;
    case fae::key::currencysubunit:
        return SDLK_CURRENCYSUBUNIT;
    case fae::key::kp_leftparen:
        return SDLK_KP_LEFTPAREN;
    case fae::key::kp_rightparen:
        return SDLK_KP_RIGHTPAREN;
    case fae::key::kp_leftbrace:
        return SDLK_KP_LEFTBRACE;
    case fae::key::kp_rightbrace:
        return SDLK_KP_RIGHTBRACE;
    case fae::key::kp_tab:
        return SDLK_KP_TAB;
    case fae::key::kp_backspace:
        return SDLK_KP_BACKSPACE;
    case fae::key::kp_a:
        return SDLK_KP_A;
    case fae::key::kp_b:
        return SDLK_KP_B;
    case fae::key::kp_c:
        return SDLK_KP_C;
    case fae::key::kp_d:
        return SDLK_KP_D;
    case fae::key::kp_e:
        return SDLK_KP_E;
    case fae::key::kp_f:
        return SDLK_KP_F;
    case fae::key::kp_xor:
        return SDLK_KP_XOR;
    case fae::key::kp_power:
        return SDLK_KP_POWER;
    case fae::key::kp_percent:
        return SDLK_KP_PERCENT;
    case fae::key::kp_less:
        return SDLK_KP_LESS;
    case fae::key::kp_greater:
        return SDLK_KP_GREATER;
    case fae::key::kp_ampersand:
        return SDLK_KP_AMPERSAND;
    case fae::key::kp_dblampersand:
        return SDLK_KP_DBLAMPERSAND;
    case fae::key::kp_verticalbar:
        return SDLK_KP_VERTICALBAR;
    case fae::key::kp_dblverticalbar:
        return SDLK_KP_DBLVERTICALBAR;
    case fae::key::kp_colon:
        return SDLK_KP_COLON;
    case fae::key::kp_hash:
        return SDLK_KP_HASH;
    case fae::key::kp_space:
        return SDLK_KP_SPACE;
    case fae::key::kp_at:
        return SDLK_KP_AT;
    case fae::key::kp_exclam:
        return SDLK_KP_EXCLAM;
    case fae::key::kp_memstore:
        return SDLK_KP_MEMSTORE;
    case fae::key::kp_memrecall:
        return SDLK_KP_MEMRECALL;
    case fae::key::kp_memclear:
        return SDLK_KP_MEMCLEAR;
    case fae::key::kp_memadd:
        return SDLK_KP_MEMADD;
    case fae::key::kp_memsubtract:
        return SDLK_KP_MEMSUBTRACT;
    case fae::key::kp_memmultiply:
        return SDLK_KP_MEMMULTIPLY;
    case fae::key::kp_memdivide:
        return SDLK_KP_MEMDIVIDE;
    case fae::key::kp_plusminus:
        return SDLK_KP_PLUSMINUS;
    case fae::key::kp_clear:
        return SDLK_KP_CLEAR;
    case fae::key::kp_clearentry:
        return SDLK_KP_CLEARENTRY;
    case fae::key::kp_binary:
        return SDLK_KP_BINARY;
    case fae::key::kp_octal:
        return SDLK_KP_OCTAL;
    case fae::key::kp_decimal:
        return SDLK_KP_DECIMAL;
    case fae::key::kp_hexadecimal:
        return SDLK_KP_HEXADECIMAL;
    case fae::key::lctrl:
        return SDLK_LCTRL;
    case fae::key::lshift:
        return SDLK_LSHIFT;
    case fae::key::lalt:
        return SDLK_LALT;
    case fae::key::lgui:
        return SDLK_LGUI;
    case fae::key::rctrl:
        return SDLK_RCTRL;
    case fae::key::rshift:
        return SDLK_RSHIFT;
    case fae::key::ralt:
        return SDLK_RALT;
    case fae::key::rgui:
        return SDLK_RGUI;
    case fae::key::mode:
        return SDLK_MODE;
    case fae::key::audionext:
        return SDLK_MEDIA_NEXT_TRACK;
    case fae::key::audioprev:
        return SDLK_MEDIA_PREVIOUS_TRACK;
    case fae::key::audiostop:
        return SDLK_MEDIA_STOP;
    case fae::key::audioplay:
        return SDLK_MEDIA_PLAY;
    case fae::key::audiomute:
        return SDLK_MUTE;
    case fae::key::mediaselect:
        return SDLK_MEDIA_SELECT;
    case fae::key::ac_search:
        return SDLK_AC_SEARCH;
    case fae::key::ac_home:
        return SDLK_AC_HOME;
    case fae::key::ac_back:
        return SDLK_AC_BACK;
    case fae::key::ac_forward:
        return SDLK_AC_FORWARD;
    case fae::key::ac_stop:
        return SDLK_AC_STOP;
    case fae::key::ac_refresh:
        return SDLK_AC_REFRESH;
    case fae::key::ac_bookmarks:
        return SDLK_AC_BOOKMARKS;
    case fae::key::eject:
        return SDLK_MEDIA_EJECT;
    case fae::key::sleep:
        return SDLK_SLEEP;
    case fae::key::audiorewind:
        return SDLK_MEDIA_REWIND;
    case fae::key::audiofastforward:
        return SDLK_MEDIA_FAST_FORWARD;
    case fae::key::softleft:
        return SDLK_SOFTLEFT;
    case fae::key::softright:
        return SDLK_SOFTRIGHT;
    case fae::key::call:
        return SDLK_CALL;
    case fae::key::endcall:
        return SDLK_ENDCALL;
    case fae::key::count:
        return SDLK_UNKNOWN;
    default:
        return SDLK_UNKNOWN;
    }
}

struct sdl_input
{
    auto press_key(const SDL_Keycode key) noexcept -> void
    {
        m_is_key_pressed->set(static_cast<std::size_t>(key), true);
    }

    auto press_key(const SDL_Scancode key) noexcept -> void
    {
        press_key(SDL_SCANCODE_TO_KEYCODE(key));
    }

    auto release_key(const SDL_Keycode key) noexcept -> void
    {
        m_is_key_pressed->set(static_cast<std::size_t>(key), false);
    }

    auto release_key(const SDL_Scancode key) noexcept -> void
    {
        release_key(SDL_SCANCODE_TO_KEYCODE(key));
    }

    [[nodiscard]] auto is_key_pressed(const SDL_Keycode key) const noexcept -> bool
    {
        return m_is_key_pressed->test(static_cast<std::size_t>(key));
    }

    [[nodiscard]] auto is_key_pressed(const SDL_Scancode key) const noexcept -> bool
    {
        return is_key_pressed(SDL_SCANCODE_TO_KEYCODE(key));
    }

    [[nodiscard]] auto is_key_released(const SDL_Keycode key) const noexcept -> bool
    {
        return !is_key_pressed(key);
    }

    [[nodiscard]] auto is_key_released(const SDL_Scancode key) const noexcept -> bool
    {
        return !is_key_pressed(key);
    }

    [[nodiscard]] auto is_key_just_pressed(const SDL_Keycode key) const noexcept -> bool
    {
        return is_key_pressed(key) && !was_key_pressed(key);
    }

    [[nodiscard]] auto is_key_just_pressed(const SDL_Scancode key) const noexcept -> bool
    {
        return is_key_pressed(key) && !was_key_pressed(key);
    }

    [[nodiscard]] auto is_key_just_released(const SDL_Keycode key) const noexcept -> bool
    {
        return !is_key_pressed(key) && was_key_pressed(key);
    }

    [[nodiscard]] auto is_key_just_released(const SDL_Scancode key) const noexcept -> bool
    {
        return !is_key_pressed(key) && was_key_pressed(key);
    }

    [[nodiscard]] auto was_key_pressed(const SDL_Keycode key) const noexcept -> bool
    {
        return m_was_key_pressed->test(static_cast<std::size_t>(key));
    }

    [[nodiscard]] auto was_key_pressed(const SDL_Scancode key) const noexcept -> bool
    {
        return was_key_pressed(SDL_SCANCODE_TO_KEYCODE(key));
    }

    struct mouse_delta
    {
        float x = 0.f;
        float y = 0.f;
    };

    [[nodiscard]] auto get_mouse_delta() const noexcept -> mouse_delta
    {
        static float previousX = 0, previousY = 0;
        float x, y;
        SDL_GetGlobalMouseState(&x, &y);
        auto delta = mouse_delta{ .x = x - previousX, .y = y - previousY };
        previousX = x;
        previousY = y;
        return delta;
    }

    auto update() noexcept -> void
    {
        *m_was_key_pressed = *m_is_key_pressed;
    }

  private:
    std::shared_ptr<std::bitset<static_cast<std::size_t>(SDLK_ENDCALL)>> m_is_key_pressed = std::make_shared<std::bitset<static_cast<std::size_t>(SDLK_ENDCALL)>>();
    std::shared_ptr<std::bitset<static_cast<std::size_t>(SDLK_ENDCALL)>> m_was_key_pressed = std::make_shared<std::bitset<static_cast<std::size_t>(SDLK_ENDCALL)>>();
};
sdl_input global_input;

auto fae::input_from_sdl() noexcept -> fae::input
{
    return fae::input{
        .press_key = [&](fae::key k)
        { global_input.press_key(to_sdl_keycode(k)); },
        .release_key = [&](fae::key k)
        { global_input.release_key(to_sdl_keycode(k)); },
        .is_key_pressed = [&](fae::key k)
        { return global_input.is_key_pressed(to_sdl_keycode(k)); },
        .is_key_released = [&](fae::key k)
        { return global_input.is_key_released(to_sdl_keycode(k)); },
        .is_key_just_pressed = [&](fae::key k)
        { return global_input.is_key_just_pressed(to_sdl_keycode(k)); },
        .is_key_just_released = [&](fae::key k)
        { return global_input.is_key_just_released(to_sdl_keycode(k)); },
        .get_mouse_delta = [&]()
        {
                auto mouse_delta = global_input.get_mouse_delta();
                return fae::input::mouse_delta{ mouse_delta.x, mouse_delta.y }; },
        .get_global_mouse_position = [&]()
        {
                    float x, y;
                    SDL_GetGlobalMouseState(&x, &y);
                    return fae::input::mouse_position{ x, y }; },
        .get_local_mouse_position = [&]()
        {
                    float x, y;
                    SDL_GetMouseState(&x, &y);
                    return fae::input::mouse_position{ x, y }; },
        .set_global_mouse_position = [&](float x, float y)
        { SDL_WarpMouseGlobal(x, y); },
        .set_local_mouse_position = [&](entity_commands& window_entity, float x, float y)
        {
                    auto maybe_window = window_entity.get_component<SDL_Window*>();
                    if (!maybe_window)
                    {
                        log_error("entity does not have a sdl_window component", log_options{.show_stacktrace = true});
                        return;
                    }
                    auto &window = *maybe_window;
                    SDL_WarpMouseInWindow(window, x, y); },
    };
}

[[nodiscard]] auto fae::window_from_sdl_window(SDL_Window* sdl_window) noexcept -> fae::window
{
    return fae::window{
        .get_title = [sdl_window]()
        { return SDL_GetWindowTitle(sdl_window); },
        .set_title = [sdl_window](std::string_view value)
        { SDL_SetWindowTitle(sdl_window, value.data()); },
        .get_size = [sdl_window]()
        {
				int width{}, height{};
				SDL_GetWindowSize(sdl_window, &width, &height);
				return fae::window::size
				{
					.width = static_cast<std::size_t>(width),
					.height = static_cast<std::size_t>(height),
				}; },
        .set_size = [sdl_window](std::size_t width, std::size_t height)
        { SDL_SetWindowSize(sdl_window, static_cast<int>(width), static_cast<int>(height)); },
        .get_position = [sdl_window]()
        {
                int x, y;
                SDL_GetWindowPosition(sdl_window, &x, &y);
                return fae::window::position{ .x = x, .y = y }; },
        .set_position = [sdl_window](int x, int y)
        { SDL_SetWindowPosition(sdl_window, x, y); },
        .show = [sdl_window]()
        { SDL_ShowWindow(sdl_window); },
        .hide = [sdl_window]()
        { SDL_HideWindow(sdl_window); },
        .update = [sdl_window]()
        {
            // do nothing
        },
        .should_close = [sdl_window]()
        { 
            // TODO
            return false; },
        .close = [sdl_window]()
        {
            // TODO
        },
        .is_fullscreen = [sdl_window]()
        { return SDL_GetWindowFlags(sdl_window) & SDL_WINDOW_FULLSCREEN; },
        .set_fullscreen = [sdl_window](bool value)
        { SDL_SetWindowFullscreen(sdl_window, value ? true : false); },
        .is_focused = [sdl_window]()
        { return SDL_GetWindowFlags(sdl_window) & SDL_WINDOW_INPUT_FOCUS; },
    };
}

auto fae::sdl_plugin::init(application& app) const noexcept -> void
{
    Uint32 flags = SDL_INIT_EVENTS;
    if (init_audio)
    {
        flags |= SDL_INIT_AUDIO;
    }
    if (init_video)
    {
        flags |= SDL_INIT_VIDEO;
    }
    if (init_haptic)
    {
        flags |= SDL_INIT_HAPTIC;
    }
    if (init_sensor)
    {
        flags |= SDL_INIT_SENSOR;
    }

    if (!SDL_Init(flags))
    {
        fae::log_error(std::format("could not initialize SDL: {}", SDL_GetError()));
        return;
    }

    app
        .add_system<update_step>(update_sdl)
        .add_system<deinit_step>(deinit_sdl);
}

auto fae::update_sdl(const fae::update_step& step) noexcept -> void
{
    global_input.update();
    for (SDL_Event event; SDL_PollEvent(&event);)
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
        {
            step.scheduler.invoke(fae::application_quit{});
            break;
        }
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        {
            auto* window = SDL_GetWindowFromID(event.window.windowID);

            for (auto [entity, entity_window] : step.ecs_world.query<SDL_Window*>())
            {
                if (window == entity_window)
                {
                    entity.destroy();
                }
            }
            break;
        }
        case SDL_EVENT_KEY_DOWN:
        {
            global_input.press_key(event.key.key);
            break;
        }
        case SDL_EVENT_KEY_UP:
        {
            global_input.release_key(event.key.key);
            break;
        }
        case SDL_EVENT_WINDOW_RESIZED:
        {
            auto width = static_cast<std::size_t>(event.window.data1);
            auto height = static_cast<std::size_t>(event.window.data2);
            step.scheduler.invoke(fae::window_resized{
                .width = width,
                .height = height,
                .global_entity = step.global_entity,
                .assets = step.assets,
                .scheduler = step.scheduler,
                .ecs_world = step.ecs_world,
            });
        }
        }
        ImGui_ImplSDL3_ProcessEvent(&event);
    }
}

auto fae::deinit_sdl(const deinit_step& step) noexcept -> void
{
    SDL_Quit();
}
