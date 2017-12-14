#include <sstream>

#include "context.h"
#include "script.h"
#include "spec.h"
#include "text.h"
#include "util.h"

#include "editor.h"

Editor::Editor() : Drawable("editor", "editor", Vector2f(0, 0), 0, Vector2f(0, 0), 1, 9002), script(nullptr), view(""), buffers{std::deque<char>{}}, buffer(buffers.begin()), pos(buffer->begin()), surface(nullptr), padding_top(4), padding_left(5), padding_font(2) {}

Editor::Editor(const Editor & editor) : Drawable(editor), script(editor.script), view(editor.view), buffers(editor.buffers), buffer(editor.buffer), pos(editor.pos), surface(nullptr), padding_top(4), padding_left(5), padding_font(2) {}

void Editor::dispatch(const SDL_Event & event) {
	if (script) {
		// grab keyboard focus
		if (!Input::get_instance().check("editor")) {
			Input::get_instance().grab("editor");
			SDL_StartTextInput();
		}

		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_BACKSPACE) {
				// backspace
			}
			else if (event.key.keysym.sym == SDLK_RETURN) {
				std::deque<char> line(pos, buffer->end());
				buffer->erase(pos, buffer->end());

				buffer = buffers.insert(buffer, line);
				pos = buffer->begin();
			}
			else if (event.key.keysym.sym == SDLK_LEFT) {
				if (pos != buffer->begin())
					--pos;
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				int p = pos - buffer->begin();
				--buffer;
				pos = buffer->begin() + Util::min(p, buffer->size());
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				if (pos != buffer->end())
					++pos;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				int p = pos - buffer->begin();
				++buffer;
				pos = buffer->begin() + Util::min(p, buffer->size());
			}

			reload();
		}
		else if (event.type == SDL_TEXTINPUT) {
			// record text
			std::string text(event.text.text);
			buffer->insert(pos, text.begin(), text.end());
		}
	}
	else {
		// release keyboard focus
		if (Input::get_instance().check("editor")) {
			Input::get_instance().release("editor");
			SDL_StopTextInput();

			view.clear();
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
		Text::get_instance().write(Context::get_instance().get_renderer(), view, rect.x + padding_font, rect.y + padding_font, color);
	}
}

void Editor::open(Script & s) {
	script = &s;

	const std::string & text = script->get_script();
	std::string::const_iterator chr = text.begin();

	buffers.clear();

	buffers.emplace_back();
	buffer = buffers.begin();
	while(chr != text.end()) {
		if (*chr == '\n') {
			buffers.emplace_back();
			++buffer;
		}

		buffer->push_back(*chr);
	}

	buffer = buffers.begin();
	pos = buffer->begin();
}

void Editor::close() {
	std::stringstream ss;

	std::deque<std::deque<char>>::iterator buf = buffers.begin();
	std::deque<char>::iterator iter = buf->begin();

	while (buf != buffers.end()) {
		if (iter == buf->end()) {
			++buf;
			iter = buf->begin();
			continue;
		}

		ss << *iter;

		++iter;
	}

	script->set_script(ss.str());

	script = nullptr;
}

void Editor::reload() {
	std::stringstream ss;

	std::deque<std::deque<char>>::iterator buf = buffers.begin();
	std::deque<char>::iterator iter = buf->begin();
	int columns = 0;

	while (buf != buffers.end()) {
		if (iter == buf->end()) {
			++buf;
			iter = buf->begin();
			continue;
		}

		if (*iter == '\n')
			columns = 0;
		else
			++columns;

		if (columns > Editor::LINE_LENGTH) {
			ss << "\n↳";
			columns = 0;
		}

		if (iter == pos)
			ss << "█";
		else
			ss << *iter;

		++iter;
	}

	view = ss.str();
}
