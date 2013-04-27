// Ŭnicode please

namespace sora {;
int StringToInt(const std::string &str);
float StringToFloat(const std::string &str);

template<typename T>
T Trim(const T &str) {
	using std::size_t;

	// do no use boost to compile speed + dependenty
	const char *whitespace = "\t\n\x0b\x0c\r ";

	size_t leftFound = str.find_first_not_of(whitespace);
	size_t rightFound = str.find_last_not_of(whitespace);

	if (leftFound == T::npos) {
		leftFound = 0;
	}
	if (rightFound == T::npos) {
		rightFound = str.length();
	}
	T result = str.substr(leftFound, rightFound-leftFound+1);
	return result;
}

template<typename T>
T LeftTrim(const T &str) {
	using std::size_t;
	size_t n = str.find_first_not_of("\t\n\x0b\x0c\r ");
	T left(n == T::npos ? str : str.substr(n, str.length()));
	return left;
}

template<typename T>
T RightTrim(const T &str) {
	using std::size_t;
	size_t n = str.find_last_not_of("\t\n\x0b\x0c\r ");
	T right(n == T::npos ? str : str.substr(0, n + 1));
	return right;
}

int Split(const std::string &str, char ch, std::vector<std::string> *retval);

template<typename T>
T Join(const T &str, const std::vector<T> &tokenlist) {
	std::ostringstream oss;
	for (size_t i = 0 ; i < tokenlist.size() ; i++) {
		oss << tokenlist[i];
		if (i != tokenlist.size() - 1) {
			oss << str;
		}
	}
	return oss.str();
}
}
