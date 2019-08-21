//
// Dictionary class implementation from Jamie's solution
//

#include <string>
#include <fstream>

#include "Dictionary.h"
#include "Exceptions.h"
#include "Util.h"

Dictionary::Dictionary(std::string dictionary_file_name):
wordsTrie()
{
	std::ifstream dictFileStream(dictionary_file_name);

	if(!dictFileStream)
	{
		throw FileException("DICTIONARY");
	}

	while(!dictFileStream.eof())
	{
		std::string word;
		dictFileStream >> word;

		if (dictFileStream.eof() && word.empty())
		{
			break;
		}

		makeLowercase(word);
		wordsTrie.insert(word);
	}
}

Dictionary::~Dictionary()
{
	// default destructor is OK
}

bool Dictionary::isLegalWord(std::string const &word) const
{
	std::string lowercaseWord(word);
	makeLowercase(lowercaseWord);
	if (wordsTrie.search(lowercaseWord) == NULL) {
		return false;
	} else {
		return true;
	}
}

bool Dictionary::isPrefixWord(std::string const &word) const {
	std::string lowercaseWord(word);
	makeLowercase(lowercaseWord);
	if (wordsTrie.prefix(lowercaseWord) == NULL) {
		return false;
	} else {
		return true;
	}
}
