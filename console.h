#ifndef CONSOLE_H
#define CONSOLE_H
#include <string>

#include "input.h"

class Console : public Drawable {
	public:
		static Console & get_instance();

		Console(const Console & console) = delete;
		const Console & operator=(const Console & console) = delete;

		virtual void draw() const;
		virtual void update(unsigned int ticks);

	private:
		Console();
		~Console() {}

		bool open;
		std::string command;
};
#endif
