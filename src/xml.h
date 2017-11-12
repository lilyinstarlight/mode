#ifndef XML_H
#define XML_H
#include <deque>
#include <map>
#include <string>

#include <expat.h>

const int BUF_SIZE = 1024;

class XML {
	public:
		XML();

		virtual ~XML() {
			XML_ParserFree(parser);
		}

		XML(const XML &) = delete;
		const XML & operator=(const XML &) = delete;

		const std::map<std::string, std::string> & get_data() const { return data; }

		static void wrap_start(void * data, const char * el, const char ** attr);
		static void wrap_end(void * data, const char * el);
		static void wrap_chars(void * data, const char * text, int len);

	private:
		XML_Parser parser;

		char buf[BUF_SIZE];

		std::deque<std::string> tags;
		std::map<std::string, std::string> data;

		void display() const;

		void start(const char * el, const char * attr[]);
		void end(const char * end_tag);
		void chars(const char * text, int len);

		std::string make_tag() const;
		std::string make_tag(const std::string & name) const;

		void strip_whitespace(std::string & str) const;
};
#endif
