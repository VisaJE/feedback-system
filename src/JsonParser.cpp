/*
 * JsonParser.cpp
 *
 *  Created on: 18 Jun 2018
 *      Author: visakoe1
 */
#include <iostream>
#include "JsonParser.h"
#include <string>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include <cstdio>
#include <fstream>

using namespace feedback;
using namespace std;
using namespace rapidjson;

#include <sys/stat.h>
// Function: fileExists
/**
    Check if a file exists
@param[in] filename - the name of the file to check

@return    true if the file exists, else false

*/
static bool fileExists(const string filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

JsonParser::JsonParser(std::string file, std::string subject, bool freshStart): filename(file), subject(subject) {
	if (!fileExists(filename)) {

	}
}
JsonParser::~JsonParser() {

}

void JsonParser::refreshFromData() {
	remove (filename.c_str());
	Document jsonDoc;
	string info = prototype;
	jsonDoc.Parse(prototype.c_str());

	Value::MemberIterator subjectN = jsonDoc.FindMember("subject");
	subjectN->value = this->subject;

	Value::MemberIterator values = jsonDoc.FindMember("score");
	for (int i = 0; i < 5; i++) {
		values->value[i] = data[i];
	}
	// Rapidjson document has to be written onto a buffer to be parsed as a string.
	rapidjson::StringBuffer buffer;
	buffer.Clear();
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	jsonDoc.Accept(writer);

	ofstream out(filename.c_str());
	out << buffer.GetString();
	out.close();
}

void JsonParser::

void JsonParser::addData(short value) {
	assert (value >= 1 && value <= 5);
	data[value-1] += 1;
}



