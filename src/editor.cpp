#include <stdexcept>
#include <sstream>

#include "clock.h"
#include "context.h"
#include "script.h"
#include "spec.h"
#include "text.h"
#include "util.h"

#include "editor.h"

Editor::Editor() : Drawable("editor", "editor", Vector2f(0, 0), 0, Vector2f(0, 0), 1, 9002),
		_script(nullptr),
		_result(""),
		_view(""),
		_buffers{std::deque<char>{}},
		_buffer(_buffers.begin()),
		_pos(_buffer->begin()),
		_columns(Spec::get_instance().get_int("editor/columns")),
		_rows(Spec::get_instance().get_int("editor/rows")),
		_top(0),
		_font(Spec::get_instance().check("editor/font") ? "editor/font" : "font/default"),
		_color{static_cast<Uint8>(Spec::get_instance().get_int("editor/text/r")), static_cast<Uint8>(Spec::get_instance().get_int("editor/text/g")), static_cast<Uint8>(Spec::get_instance().get_int("editor/text/b")), static_cast<Uint8>(Spec::get_instance().get_int("editor/text/a"))},
		_box{static_cast<Uint8>(Spec::get_instance().get_int("editor/box/r")), static_cast<Uint8>(Spec::get_instance().get_int("editor/box/g")), static_cast<Uint8>(Spec::get_instance().get_int("editor/box/b")), static_cast<Uint8>(Spec::get_instance().get_int("editor/box/a"))},
		_padding_top(4), _padding_left(5), _padding_font(2) {}

Editor::Editor(const Editor & editor) : Drawable(editor), _script(editor._script), _result(editor._result), _view(editor._view), _buffers(editor._buffers), _buffer(editor._buffer), _pos(editor._pos), _columns(editor._columns), _rows(editor._rows), _top(editor._top), _font(editor._font), _color(editor._color), _box(editor._box), _padding_top(editor._padding_top), _padding_left(editor._padding_left), _padding_font(editor._padding_font) {}

void Editor::dispatch(const SDL_Event & event) {
	if (_script) {
		// grab keyboard focus
		if (!Input::get_instance().check("editor")) {
			Input::get_instance().grab("editor");
			SDL_StartTextInput();
		}

		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_BACKSPACE) {
				if (_pos == _buffer->begin()) {
					if (_buffer != _buffers.begin()) {
						_pos = (_buffer - 1)->insert((_buffer - 1)->end(), _buffer->begin(), _buffer->end());
						_buffer = _buffers.erase(_buffer) - 1;
					}
				}
				else {
					_pos = _buffer->erase(_pos - 1);
				}
			}
			else if (event.key.keysym.sym == SDLK_RETURN) {
				std::deque<char> line(_pos, _buffer->end());
				_buffer->erase(_pos, _buffer->end());

				_buffer = _buffers.insert(_buffer + 1, line);
				_pos = _buffer->begin();
			}
			else if (event.key.keysym.sym == SDLK_TAB) {
				_pos = _buffer->insert(_pos, '\t') + 1;
			}
			else if (event.key.keysym.sym == SDLK_LEFT) {
				if (_pos != _buffer->begin())
					--_pos;
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				if (_buffer != _buffers.begin()) {
					unsigned int p = _pos - _buffer->begin();
					--_buffer;
					_pos = _buffer->begin() + Util::min(p, _buffer->size());
				}
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				if (_pos != _buffer->end())
					++_pos;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				if (_buffer != _buffers.end() - 1) {
					unsigned int p = _pos - _buffer->begin();
					++_buffer;
					_pos = _buffer->begin() + Util::min(p, _buffer->size());
				}
			}

			refresh();
		}
		else if (event.type == SDL_TEXTINPUT) {
			// record text
			std::string text(event.text.text);
			_pos = _buffer->insert(_pos, text.begin(), text.end()) + text.length();

			refresh();
		}
	}
	else {
		// release keyboard focus
		if (Input::get_instance().check("editor")) {
			Input::get_instance().release("editor");
			SDL_StopTextInput();
		}
	}
}

void Editor::draw(const Viewport & viewport) const {
	if (_script) {
		// drawing rectable
		SDL_Rect rect = {_padding_left, _padding_top, viewport.get_width() - _padding_left*2, viewport.get_height() - _padding_top*2};

		// draw box
		SDL_SetRenderDrawColor(Context::get_instance().get_renderer(), _box.r, _box.g, _box.b, _box.a);
		SDL_RenderFillRect(Context::get_instance().get_renderer(), &rect);

		// draw text
		Text::get_instance().write(Context::get_instance().get_renderer(), _font, _view, rect.x + _padding_font, rect.y + _padding_font, 0, _color);
	}
}

void Editor::open(Script & s) {
	Clock::get_instance().pause();

	_script = &s;

	const std::string & text = _script->get_script();
	std::string::const_iterator chr = text.begin();

	_buffers.clear();

	_buffers.emplace_back();
	_buffer = _buffers.end() - 1;
	while (chr != text.end()) {
		if (*chr == '\n') {
			_buffers.emplace_back();
			_buffer = _buffers.end() - 1;
		}
		else {
			_buffer->push_back(*chr);
		}

		++chr;
	}

	_buffer = _buffers.begin();
	_pos = _buffer->begin();

	refresh();
}

void Editor::close() {
	std::stringstream ss;

	std::deque<std::deque<char>>::iterator buf = _buffers.begin();
	std::deque<char>::iterator iter = buf->begin();

	while (buf != _buffers.end()) {
		if (iter == buf->end()) {
			ss << "\n";

			++buf;
			iter = buf->begin();
			continue;
		}

		ss << *iter;

		++iter;
	}

	try {
		_script->set_script(ss.str());
	}
	catch (std::runtime_error & err) {
		//TODO: actually put this error in the interface
		//TODO: what about errors caught later? - should editor automatically be shown, enemy automatically killed, or enemy basically paused until edited again (any way showing error hopefully)?
		_result = err.what();

		return;
	}

	_script = nullptr;

	Clock::get_instance().start();
}

void Editor::refresh() {
	std::stringstream ss;

	_top = Util::min(Util::max(_buffer - _buffers.begin() - _rows/2, 0), Util::max(_buffers.size() - _rows, 0));

	std::deque<std::deque<char>>::iterator buf = _buffers.begin() + _top;
	std::deque<char>::iterator iter = buf->begin();

	int col = 0;
	int row = 0;

	while (buf != _buffers.end()) {
		if (row > _rows)
			break;

		if (iter == buf->end()) {
			if (buf == _buffer && iter == _pos)
				ss << "_\n";
			else
				ss << " \n";

			col = 0;
			++row;

			++buf;
			iter = buf->begin();
			continue;
		}
		else {
			++col;
		}

		if (col > _columns) {
			ss << "\n▶";
			col = 0;
			++row;
		}

		if (*iter == '\t') {
			if (buf == _buffer && iter == _pos)
				ss << "_ ";
			else
				ss << "  ";
		}
		else {
			if (buf == _buffer && iter == _pos)
				ss << "_";
			else
				ss << *iter;
		}

		++iter;
	}

	_view = ss.str();
}
