#include <stdexcept>
#include <sstream>

#include "clock.h"
#include "context.h"
#include "script.h"
#include "spec.h"
#include "text.h"
#include "util.h"

#include "editor.h"

Editor::Editor() : Drawable("editor", "editor", Vector2f(0, 0), 0, Vector2f(0, 0), 1, 9002), script(nullptr), result(""), view(""), buffers{std::deque<char>{}}, buffer(buffers.begin()), pos(buffer->begin()), columns(Spec::get_instance().get_int("editor/columns")), rows(Spec::get_instance().get_int("editor/rows")), top(0), padding_top(4), padding_left(5), padding_font(2) {}

Editor::Editor(const Editor & editor) : Drawable(editor), script(editor.script), result(editor.result), view(editor.view), buffers(editor.buffers), buffer(editor.buffer), pos(editor.pos), columns(editor.columns), rows(editor.rows), top(editor.top), padding_top(4), padding_left(5), padding_font(2) {}

void Editor::dispatch(const SDL_Event & event) {
	if (script) {
		// grab keyboard focus
		if (!Input::get_instance().check("editor")) {
			Input::get_instance().grab("editor");
			SDL_StartTextInput();
		}

		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_BACKSPACE) {
				if (pos == buffer->begin()) {
					if (buffer != buffers.begin()) {
						pos = (buffer - 1)->insert((buffer - 1)->end(), buffer->begin(), buffer->end());
						buffer = buffers.erase(buffer) - 1;
					}
				}
				else {
					pos = buffer->erase(pos - 1);
				}
			}
			else if (event.key.keysym.sym == SDLK_RETURN) {
				std::deque<char> line(pos, buffer->end());
				buffer->erase(pos, buffer->end());

				buffer = buffers.insert(buffer + 1, line);
				pos = buffer->begin();
			}
			else if (event.key.keysym.sym == SDLK_TAB) {
				pos = buffer->insert(pos, '\t') + 1;
			}
			else if (event.key.keysym.sym == SDLK_LEFT) {
				if (pos != buffer->begin())
					--pos;
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				if (buffer != buffers.begin()) {
					unsigned int p = pos - buffer->begin();
					--buffer;
					pos = buffer->begin() + Util::min(p, buffer->size());
				}
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				if (pos != buffer->end())
					++pos;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				if (buffer != buffers.end() - 1) {
					unsigned int p = pos - buffer->begin();
					++buffer;
					pos = buffer->begin() + Util::min(p, buffer->size());
				}
			}

			refresh();
		}
		else if (event.type == SDL_TEXTINPUT) {
			// record text
			std::string text(event.text.text);
			pos = buffer->insert(pos, text.begin(), text.end()) + text.length();

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
	if (script) {
		// drawing rectable
		SDL_Rect rect = {padding_left, padding_top, viewport.get_width() - padding_left*2, viewport.get_height() - padding_top*2};

		// draw box
		SDL_SetRenderDrawColor(Context::get_instance().get_renderer(), Spec::get_instance().get_int("editor/box/r"), Spec::get_instance().get_int("editor/box/g"), Spec::get_instance().get_int("editor/box/b"), Spec::get_instance().get_int("editor/box/a"));
		SDL_RenderFillRect(Context::get_instance().get_renderer(), &rect);

		// draw text
		SDL_Color color = {static_cast<Uint8>(Spec::get_instance().get_int("editor/text/r")), static_cast<Uint8>(Spec::get_instance().get_int("editor/text/g")), static_cast<Uint8>(Spec::get_instance().get_int("editor/text/b")), 255};
		Text::get_instance().write(Context::get_instance().get_renderer(), Spec::get_instance().check("editor/font") ? "editor/font" : "font/default", view, rect.x + padding_font, rect.y + padding_font, 0, color);
	}
}

void Editor::open(Script & s) {
	Clock::get_instance().pause();

	script = &s;

	const std::string & text = script->get_script();
	std::string::const_iterator chr = text.begin();

	buffers.clear();

	buffers.emplace_back();
	buffer = buffers.end() - 1;
	while(chr != text.end()) {
		if (*chr == '\n') {
			buffers.emplace_back();
			buffer = buffers.end() - 1;
		}
		else {
			buffer->push_back(*chr);
		}

		++chr;
	}

	buffer = buffers.begin();
	pos = buffer->begin();

	refresh();
}

void Editor::close() {
	std::stringstream ss;

	std::deque<std::deque<char>>::iterator buf = buffers.begin();
	std::deque<char>::iterator iter = buf->begin();

	while (buf != buffers.end()) {
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
		script->set_script(ss.str());
	}
	catch (std::runtime_error & err) {
		//TODO: actually put this error in the interface
		result = err.what();

		return;
	}

	script = nullptr;

	Clock::get_instance().start();
}

void Editor::refresh() {
	std::stringstream ss;

	top = Util::min(Util::max(buffer - buffers.begin() - rows/2, 0), Util::max(buffers.size() - rows, 0));

	std::deque<std::deque<char>>::iterator buf = buffers.begin() + top;
	std::deque<char>::iterator iter = buf->begin();

	int col = 0;
	int row = 0;

	while (buf != buffers.end()) {
		if (row > rows)
			break;

		if (iter == buf->end()) {
			if (buf == buffer && iter == pos)
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

		if (col > columns) {
			ss << "\n▶";
			col = 0;
			++row;
		}

		if (*iter == '\t') {
			if (buf == buffer && iter == pos)
				ss << "_ ";
			else
				ss << "  ";
		}
		else {
			if (buf == buffer && iter == pos)
				ss << "_";
			else
				ss << *iter;
		}

		++iter;
	}

	view = ss.str();
}
