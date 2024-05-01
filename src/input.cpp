module;
#include <SDL3/SDL.h>
#include <functional>

export module fae:input;

import :application;
import :sdl;
import :windowing;

export namespace fae
{
	struct application;
	struct update_step;

	enum class key
	{
		unknown,
		enter,
		escape,
		backspace,
		tab,
		space,
		exclaim,
		quotedbl,
		hash,
		percent,
		dollar,
		ampersand,
		quote,
		leftparen,
		rightparen,
		asterisk,
		plus,
		comma,
		minus,
		period,
		slash,
		num_0,
		num_1,
		num_2,
		num_3,
		num_4,
		num_5,
		num_6,
		num_7,
		num_8,
		num_9,
		colon,
		semicolon,
		less,
		equals,
		greater,
		question,
		at,

		/*
			Skip uppercase letters
		*/

		leftbracket,
		backslash,
		rightbracket,
		caret,
		underscore,
		backquote,
		a,
		b,
		c,
		d,
		e,
		f,
		g,
		h,
		i,
		j,
		k,
		l,
		m,
		n,
		o,
		p,
		q,
		r,
		s,
		t,
		u,
		v,
		w,
		x,
		y,
		z,

		capslock,

		f1,
		f2,
		f3,
		f4,
		f5,
		f6,
		f7,
		f8,
		f9,
		f10,
		f11,
		f12,

		printscreen,
		scrolllock,
		pause,
		insert,
		home,
		pageup,
		del,
		end,
		pagedown,
		right,
		left,
		down,
		up,

		numlockclear,
		kp_divide,
		kp_multiply,
		kp_minus,
		kp_plus,
		kp_enter,
		kp_1,
		kp_2,
		kp_3,
		kp_4,
		kp_5,
		kp_6,
		kp_7,
		kp_8,
		kp_9,
		kp_0,
		kp_period,

		application,
		power,
		kp_equals,
		f13,
		f14,
		f15,
		f16,
		f17,
		f18,
		f19,
		f20,
		f21,
		f22,
		f23,
		f24,
		execute,
		help,
		menu,
		select,
		stop,
		again,
		undo,
		cut,
		copy,
		paste,
		find,
		mute,
		volumeup,
		volumedown,
		kp_comma,
		kp_equalsas400,

		alterase,
		sysreq,
		cancel,
		clear,
		prior,
		return2,
		separator,
		out,
		oper,
		clearagain,
		crsel,
		exsel,

		kp_00,
		kp_000,
		thousandsseparator,
		decimalseparator,
		currencyunit,
		currencysubunit,
		kp_leftparen,
		kp_rightparen,
		kp_leftbrace,
		kp_rightbrace,
		kp_tab,
		kp_backspace,
		kp_a,
		kp_b,
		kp_c,
		kp_d,
		kp_e,
		kp_f,
		kp_xor,
		kp_power,
		kp_percent,
		kp_less,
		kp_greater,
		kp_ampersand,
		kp_dblampersand,
		kp_verticalbar,
		kp_dblverticalbar,
		kp_colon,
		kp_hash,
		kp_space,
		kp_at,
		kp_exclam,
		kp_memstore,
		kp_memrecall,
		kp_memclear,
		kp_memadd,
		kp_memsubtract,
		kp_memmultiply,
		kp_memdivide,
		kp_plusminus,
		kp_clear,
		kp_clearentry,
		kp_binary,
		kp_octal,
		kp_decimal,
		kp_hexadecimal,

		lctrl,
		lshift,
		lalt,
		lgui,
		rctrl,
		rshift,
		ralt,
		rgui,

		mode,

		audionext,
		audioprev,
		audiostop,
		audioplay,
		audiomute,
		mediaselect,
		www,
		mail,
		calculator,
		computer,
		ac_search,
		ac_home,
		ac_back,
		ac_forward,
		ac_stop,
		ac_refresh,
		ac_bookmarks,

		brightnessdown,
		brightnessup,
		displayswitch,
		kbdillumtoggle,
		kbdillumdown,
		kbdillumup,
		eject,
		sleep,
		app1,
		app2,

		audiorewind,
		audiofastforward,

		softleft,
		softright,
		call,
		endcall,

		count, // does not represent a key, used to get the number of key types
	};

	struct input
	{
		std::function<void(key)> press_key;
		std::function<void(key)> release_key;
		std::function<bool(key)> is_key_pressed;
		std::function<bool(key)> is_key_released;
		std::function<bool(key)> is_key_just_pressed;
		std::function<bool(key)> is_key_just_released;
	};

	auto quit_on_esc(const update_step &step) noexcept -> void
	{
		step.resources.use_resource<input>([&](input &input)
			{
			if (input.is_key_just_pressed(key::escape))
			{
				step.scheduler.invoke(application_quit{});
			} });
	}

	auto toggle_fullscreen_on_alt_enter(const update_step &step) noexcept -> void
	{
		step.resources.use_resource<input>([&](input &input)
			{
			if (input.is_key_pressed(key::lalt) && input.is_key_just_pressed(key::enter))
			{
				step.resources.use_resource<fae::primary_window>([&](fae::primary_window& primary)
					{
						primary.window().toggle_fullscreen();
					});
			} });
	}

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
		case fae::key::quotedbl:
			return SDLK_QUOTEDBL;
		case fae::key::hash:
			return SDLK_HASH;
		case fae::key::percent:
			return SDLK_PERCENT;
		case fae::key::dollar:
			return SDLK_DOLLAR;
		case fae::key::ampersand:
			return SDLK_AMPERSAND;
		case fae::key::quote:
			return SDLK_QUOTE;
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
		case fae::key::backquote:
			return SDLK_BACKQUOTE;
		case fae::key::a:
			return SDLK_a;
		case fae::key::b:
			return SDLK_b;
		case fae::key::c:
			return SDLK_c;
		case fae::key::d:
			return SDLK_d;
		case fae::key::e:
			return SDLK_e;
		case fae::key::f:
			return SDLK_f;
		case fae::key::g:
			return SDLK_g;
		case fae::key::h:
			return SDLK_h;
		case fae::key::i:
			return SDLK_i;
		case fae::key::j:
			return SDLK_j;
		case fae::key::k:
			return SDLK_k;
		case fae::key::l:
			return SDLK_l;
		case fae::key::m:
			return SDLK_m;
		case fae::key::n:
			return SDLK_n;
		case fae::key::o:
			return SDLK_o;
		case fae::key::p:
			return SDLK_p;
		case fae::key::q:
			return SDLK_q;
		case fae::key::r:
			return SDLK_r;
		case fae::key::s:
			return SDLK_s;
		case fae::key::t:
			return SDLK_t;
		case fae::key::u:
			return SDLK_u;
		case fae::key::v:
			return SDLK_v;
		case fae::key::w:
			return SDLK_w;
		case fae::key::x:
			return SDLK_x;
		case fae::key::y:
			return SDLK_y;
		case fae::key::z:
			return SDLK_z;
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
			return SDLK_AUDIONEXT;
		case fae::key::audioprev:
			return SDLK_AUDIOPREV;
		case fae::key::audiostop:
			return SDLK_AUDIOSTOP;
		case fae::key::audioplay:
			return SDLK_AUDIOPLAY;
		case fae::key::audiomute:
			return SDLK_AUDIOMUTE;
		case fae::key::mediaselect:
			return SDLK_MEDIASELECT;
		case fae::key::www:
			return SDLK_WWW;
		case fae::key::mail:
			return SDLK_MAIL;
		case fae::key::calculator:
			return SDLK_CALCULATOR;
		case fae::key::computer:
			return SDLK_COMPUTER;
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
		case fae::key::brightnessdown:
			return SDLK_BRIGHTNESSDOWN;
		case fae::key::brightnessup:
			return SDLK_BRIGHTNESSUP;
		case fae::key::displayswitch:
			return SDLK_DISPLAYSWITCH;
		case fae::key::kbdillumtoggle:
			return SDLK_KBDILLUMTOGGLE;
		case fae::key::kbdillumdown:
			return SDLK_KBDILLUMDOWN;
		case fae::key::kbdillumup:
			return SDLK_KBDILLUMUP;
		case fae::key::eject:
			return SDLK_EJECT;
		case fae::key::sleep:
			return SDLK_SLEEP;
		case fae::key::app1:
			return SDLK_APP1;
		case fae::key::app2:
			return SDLK_APP2;
		case fae::key::audiorewind:
			return SDLK_AUDIOREWIND;
		case fae::key::audiofastforward:
			return SDLK_AUDIOFASTFORWARD;
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

	auto input_from(fae::sdl_input &sdl_input) noexcept -> fae::input
	{
		return fae::input{
			.press_key = [&](fae::key k)
			{ sdl_input.press_key(to_sdl_keycode(k)); },
			.release_key = [&](fae::key k)
			{ sdl_input.release_key(to_sdl_keycode(k)); },
			.is_key_pressed = [&](fae::key k)
			{ return sdl_input.is_key_pressed(to_sdl_keycode(k)); },
			.is_key_released = [&](fae::key k)
			{ return sdl_input.is_key_released(to_sdl_keycode(k)); },
			.is_key_just_pressed = [&](fae::key k)
			{ return sdl_input.is_key_just_pressed(to_sdl_keycode(k)); },
			.is_key_just_released = [&](fae::key k)
			{ return sdl_input.is_key_just_released(to_sdl_keycode(k)); },
		};
	}

	struct input_plugin
	{
		auto init(application &app) const noexcept -> void
		{
			app.add_plugin(sdl_plugin{});

			auto &input = app.resources.get_or_emplace<sdl_input>(sdl_input{});
			app
				.emplace_resource<fae::input>(input_from(input))
				.add_system<update_step>(toggle_fullscreen_on_alt_enter);
		}
	};
} // namespace fae
