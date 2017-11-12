#include <iostream>
#include <sstream>

#include "xml.h"

XML::XML(const std::string& filename) : parser(NULL), tags(), data() {
	parser = XML_ParserCreate(NULL);

	if (!parser) {
		throw std::string("Could not create parser");
	}

	XML_SetUserData(parser, this);
	XML_SetElementHandler(parser, wrap_start, wrap_end);
	XML_SetCharacterDataHandler(parser, wrap_chars);

	std::ifstream file(filename);
	if (!file) {
		throw string("Could not open xml file: ") + filename;
	}

	in.getline(buff, BUF_SIZE);
	while ( true ) {
		if (!XML_Parse(parser, buf, strlen(buf), 0)) {
			std::cerr << "Parse error at line " << XML_GetCurrentLineNumber(parser) << ": " << XML_ErrorString(XML_GetErrorCode(parser)) << std::endl;
			throw string("Could not parse file: ") + filename;
		}

		if (in.eof())
			break;
		else
			in.getline(buf, BUF_SIZE);
	}
}

void XML::display() const {
	std::map<string, string>::const_iterator ptr = data.begin();
	while (ptr != data.end()) {
		std::cout << "(" << ptr->first << ", " << ptr->second << ")" << std::endl;
		++ptr;
	}
}

std::string XML::make_tag(const std::string & name) const {
	std::string name;

	for (unsigned int i = 1; i < tags.size() - 1; ++i) {
		name += tags[i] + "/";
	}

	name += tags[tags.size() - 1];
	name += std::string("/") + name;

	return name;
}

std::string XML::make_tag() const {
	std::string name;

	for (unsigned int i = 1; i < tags.size() - 1; ++i) {
		name += tags[i] + "/";
	}

	name += tags[tags.size() - 1];

	return name;
}

void XML::start(const char * el, const char * attr[]) {
	tags.push_back(el);
	for (int i = 0; attr[i]; i += 2) {
		data[make_tag(attr[i])] = attr[i + 1];
	}
}

void XML::end(const char * end_tag) {
	if ( end_tag != tags.back() ) {
		throw std::string("Tags ") + end_tag + " and " + tags.back() + std::string(" do not match");
	}

	tags.pop_back();
}

void XML::strip_whitespace(std::string & str) const {
	int length = str.size();

	int i = length - 1;
	while (i >= 0) {
		if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
			str.erase(i, 1);
			i--;
		}
		else {
			break;
		}
	}
}

void XML::chars(const char * text, int len) {
	std::string str(text, len);

	strip_whitespace(str);

	if (str.size()) {
		data[make_tag()] = str;
	}
}

void XML::wrap_start(void * data, const char * el, const char ** attr) {
	XML * parser = static_cast<XML *>(data);
	parser->start(el, attr);
}

void XML::wrap_end(void * data, const char * el) {
	XML * parser = static_cast<XML *>(data);
	parser->end(el);
}

void XML::wrap_chars(void * data, const char * text, int len) {
	XML * parser = static_cast<XML *>(data);
	parser->chars(text, textlen);
}
