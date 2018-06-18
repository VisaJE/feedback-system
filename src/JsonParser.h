/*
 * JsonParser.h
 *
 *  Created on: 18 Jun 2018
 *      Author: visakoe1
 */

#ifndef JSONPARSER_H_
#define JSONPARSER_H_
#include <iostream>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"

namespace feedback {
class JsonParser {
public:
	JsonParser(std::string file, std::string subject, bool freshStart);
	~JsonParser();
	std::string filename;
	std::string subject;
	void addData(short value);
	bool getData(short values[5]);
	void refreshFromData();
	void refreshFromFile();
private:
	const std::string prototype = "{\"subject\" : \"%s\", \"score\": [0, 0, 0, 0, 0]}";
	short data[5] = {0, 0, 0, 0, 0};
};

}




#endif /* JSONPARSER_H_ */
