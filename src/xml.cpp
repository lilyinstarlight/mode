#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

#include "xml.h"

XML::XML(const std::string& filename) : parser(NULL), tags(), data() {
	// create parser
	parser = XML_ParserCreate(NULL);

	if (!parser) {
		throw std::runtime_error("Could not create parser");
	}

	// set handlers
	XML_SetUserData(parser, this);
	XML_SetElementHandler(parser, wrap_start, wrap_end);
	XML_SetCharacterDataHandler(parser, wrap_chars);

	// load file
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("Could not open xml file: " + filename);
	}

	// send file line by line into parser
	file.getline(buf, BUF_SIZE);
	while (true) {
		if (!XML_Parse(parser, buf, std::strlen(buf), 0)) {
			std::cerr << "Parse error at line " << XML_GetCurrentLineNumber(parser) << ": " << XML_ErrorString(XML_GetErrorCode(parser)) << std::endl;
			throw std::runtime_error("Could not parse file: " + filename);
		}

		if (file.eof())
			break;
		else
			file.getline(buf, BUF_SIZE);
	}
}

std::string XML::make_tag(const std::string & name) const {
	// make tag adding in /'s to separate hierarchy
	std::string tag_name;

	for (unsigned int i = 1; i < tags.size() - 1; ++i) {
		tag_name += tags[i] + "/";
	}

	tag_name += tags[tags.size() - 1];
	tag_name += std::string("/") + name;

	return tag_name;
}

std::string XML::make_tag() const {
	// make tag adding in /'s to separate hierarchy
	std::string tag_name;

	for (unsigned int i = 1; i < tags.size() - 1; ++i) {
		tag_name += tags[i] + "/";
	}

	tag_name += tags[tags.size() - 1];

	return tag_name;
}

void XML::start(const char * el, const char * attr[]) {
	// add tag start
	tags.push_back(el);
	for (int i = 0; attr[i]; i += 2) {
		data[make_tag(attr[i])] = attr[i + 1];
	}
}

void XML::end(const char * end_tag) {
	// make sure tag end matches
	if (end_tag != tags.back())
		throw std::runtime_error(std::string("Tags ") + end_tag + " and " + tags.back() + " do not match");

	// remove tag from stack
	tags.pop_back();
}

std::string XML::strip_whitespace(const std::string & str) const {
	// remove unnecessary whitespace from string
	return std::regex_replace(str, std::regex("^\\s+|\\s+$"), "");
}

void XML::chars(const char * text, int len) {
	// make tag contents
	std::string str(text, len);

	str = strip_whitespace(str);

	if (str.size()) {
		std::string tag = make_tag();

		if (data.find(tag) != data.end())
			data[tag] += "\n" + str;
		else
			data[tag] = str;
	}
}

void XML::wrap_start(void * data, const char * el, const char ** attr) {
	// perform static cast of data back to XML
	XML * parser = static_cast<XML *>(data);
	parser->start(el, attr);
}

void XML::wrap_end(void * data, const char * el) {
	// perform static cast of data back to XML
	XML * parser = static_cast<XML *>(data);
	parser->end(el);
}

void XML::wrap_chars(void * data, const char * text, int len) {
	// perform static cast of data back to XML
	XML * parser = static_cast<XML *>(data);
	parser->chars(text, len);
}
