// Ŭnicode please
#include "stdafx.h"
#include "common_string.h"


namespace sora {;
int Split(const std::string &str,
		  char ch, std::vector<std::string> *retval) 
{
	using std::string;
	//SR_ASSERT(retval->empty() == true);

	// if no delimeter
	if (str.length() == 0) {
		retval->push_back(str);
		return retval->size();
	}

	// simple impl
	string tmp_token;
	for (size_t i = 0 ; i < str.length() ; i++) {
		char str_ch = str[i];
		if (str_ch == ch) {
			// split point
			retval->push_back(tmp_token);
			tmp_token.clear();
		} else {
			tmp_token.push_back(str_ch);
		}
	}
	retval->push_back(tmp_token);
	return retval->size();
}

int StringToInt(const std::string &str) 
{
	int value = 0;
	value = atoi(str.c_str());
	return value;
}
float StringToFloat(const std::string &str) 
{
	float value = 0;
	value = static_cast<float>(atof(str.c_str()));
	return value;
}
}
