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
/*
 * The parser can update an existing file only if the subject name and the filenames remain the same. If subject changes,
 * the earlier file will be overwritten. The files are updated at the destructor.
 */

JsonParser::JsonParser(std::string file, std::string subject, bool freshStart): filename(file), subject(subject) {
	char extension[5] = {'.', 'j', 's', 'o', 'n'};
	if (filename.length() < 5) filename = filename.append(".json");
	else {
		for (int i = 4; i >= 0; i--) {
			if (filename.at(filename.length() - i-1) != extension[4-i]) {
				filename = filename.append(".json");
			}
		}
	}
	if (!fileExists(filename) || freshStart) {
		refreshFromData();
	}
	else if (!refreshFromFile()) {
		refreshFromData();
	}
}
JsonParser::~JsonParser() {
	refreshFromData();
}

void JsonParser::refreshFromData() {
	remove (filename.c_str());
	Document jsonDoc;
	string info = prototype;
	jsonDoc.Parse(prototype.c_str());

	Value::MemberIterator subjectN = jsonDoc.FindMember("subject");
	subjectN->value = StringRef(subject.c_str());

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

bool JsonParser::refreshFromFile() {
	  std::ifstream ifs (filename, std::ifstream::in);
	  char c = ifs.get();
	  stringstream s;
	  while (ifs.good()) {
	    if (c != '\n' ) s << c;
	    c = ifs.get();
	  }
	  Document jsonDoc;
	  string st = s.str();
	  try {
		  jsonDoc.Parse(st.c_str());
	  } catch (int e) {
		  return false;
	  }

	  Value::MemberIterator subjectN = jsonDoc.FindMember("subject");
	  if (subjectN->value.GetString() != subject) return false;
	  Value::MemberIterator valuesN = jsonDoc.FindMember("score");
	  for (int i = 0; i < 5; i++) {
		  if (valuesN->value[i].IsNumber() ) {
			  data[i] = valuesN->value[i].GetInt();
		  }
		  else return false;
	  }
	  ifs.close();
	  return true;
}

void JsonParser::addData(short value) {
	assert (value >= 1 && value <= 5);
	data[value-1] += 1;
}

void JsonParser::getData(int values[5]) {
	for (int i = 0; i < 5; i++) {
		values[i] = data[i];
	}
}



