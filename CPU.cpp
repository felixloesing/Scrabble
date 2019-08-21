#include "CPU.h"
#include <iostream>
#include "Exceptions.h"

//CPUL implementation
//play longest word

CPUL::CPUL(size_t maxTiles) : Player("CPUL", maxTiles) {

}

CPUL::~CPUL() {

}

void CPUL::performCPUMove(Board & board, Bag & bag, Dictionary & dictionary) {
	std::set<Tile*> hand = getHandTiles();
	std::string handString = "";
	std::vector<Tile*> tiles;
	int numBlanks = 0;

	for (std::set<Tile*>::iterator i = hand.begin(); i != hand.end(); ++i)
	{
		if ((*i)->getLetter() == '?') {
			numBlanks++;
		} else {
			handString += (*i)->getLetter();
			tiles.push_back((*i));
		}
		
	}
	int highestLength = -1;
	int highestScore = -1;
	int xPos = -1;
	int yPos = -1;
	int hori = -1;
	std::string bestString = "";


	for (unsigned int x = 0; x < board.getColumns(); ++x)
	{

		for (unsigned int y = 0; y < board.getRows(); ++y)
		{

			if (board.getSquare(x+1, y+1)->isOccupied()) {;
				continue;
			}

			for (int h = 0; h < 2; ++h)
			{
				std::vector<std::pair<std::string, unsigned int>> results;

				std::set<std::string> prefix;
				

				std::string a;
				if (h == 1) {
					a = scanVert(board, true, x-1, y);

				} else {
					a = scanHori(board, true, x, y-1);
					
				}

				if (numBlanks > 0) {
					//resolve blanks
				} else {
					generatePossiblePer(a, handString, dictionary, prefix, board);
				}

				for (std::set<std::string>::iterator it = prefix.begin(); it != prefix.end(); ++it) {

					try {
						std::vector<Tile*> t;
						t = this->takeTiles((*it), true);
						this->addTiles(t);
						PlaceMove pm(x, y, (bool)h, (*it), this);
						this->addTiles(t);

						try {
							results = board.getPlaceMoveResults(pm);
						} catch (const std::exception& e) {
							continue;
						}

					} catch (const std::exception& e) {
						continue;
					}


					int length = -1;
					int score = -1;

					//check if valid word
					std::string wordFormed = results.back().first;
					if (dictionary.isLegalWord(wordFormed)) {
						//calculate score

						for (unsigned int i = 0; i < results.size(); ++i)
						{
							
							if (dictionary.isLegalWord(results[i].first)) {
								length = wordFormed.length();
								score += results[i].second;
							} else {
								length = -1;
								break;
							}
							
						}

						if (length > highestLength) {
							highestLength = length;
							bestString = (*it);
							xPos = x;
							yPos = y;
							hori = h;
							highestScore = score;
						}
					} else {
						//discard
						continue;
					}

				}

			}
		}
	}


	//execute best move
	if (highestScore > -1) {
		PlaceMove pm(xPos, yPos, (bool)hori, bestString, this);
		board.executePlaceMove(pm);
		if (bestString.length() == 7) {
			this->addPoints(50);
		}
		this->addPoints(highestScore+1);
	} else {
		//pass
		std::cout << "CPU passed" << std::endl;
		PassMove pass(this);
		pass.execute(board, bag, dictionary);
	}
}

std::string CPUL::scanHori(Board & board, bool left, int x, int y) {
	
	std::string s = "";
	if (left) {
		while (y > 0 && board.getSquare(x+1, y+1)->isOccupied()) {
			s += board.getSquare(x+1, y+1)->getLetter();
			y--;
		}
		std::reverse(s.begin(), s.end());
		return s;
	} else {
		//right
		while (y < (int)board.getColumns() && board.getSquare(x+1, y+1)->isOccupied()) {
			s += board.getSquare(x+1, y+1)->getLetter();
			y++;
		}
		return s;
	}
}

std::string CPUL::scanVert(Board & board, bool up, int x, int y) {
	
	std::string s = "";
	if (up) {
		while (x > 0 && board.getSquare(x+1, y+1)->isOccupied()) {
			s += board.getSquare(x+1, y+1)->getLetter();
			x--;
		}
		std::reverse(s.begin(), s.end());
		return s;
	} else {
		//right
		while (x < (int)board.getRows() && board.getSquare(x+1, y+1)->isOccupied()) {
			s += board.getSquare(x+1, y+1)->getLetter();
			x++;
		}
		return s;
	}
}

void CPUL::generatePossiblePer(std::string prefix, std::string input, Dictionary & dictionary, std::set<std::string> & per, Board & board) {
	if (!dictionary.isPrefixWord(prefix)) {
		return;
	}

	//use legal words
	if (dictionary.isPrefixWord(prefix)) {
		per.insert(prefix);
	}

	for (unsigned int i = 0; i < input.length(); ++i)
	{
		std::string p = prefix + input.at(i);
		std::string in = input;
		in.erase(i, 1);
		//increment index
		generatePossiblePer(p, in, dictionary, per, board);
	}
}



//CPUS implementation
//get highest score

CPUS::CPUS(size_t maxTiles) : Player("CPUS", maxTiles) {
}

CPUS::~CPUS() {
}

void CPUS::performCPUMove(Board & board, Bag & bag, Dictionary & dictionary) {

	std::set<Tile*> hand = getHandTiles();
	std::string handString = "";
	std::vector<Tile*> tiles;
	int numBlanks = 0;

	for (std::set<Tile*>::iterator i = hand.begin(); i != hand.end(); ++i)
	{
		if ((*i)->getLetter() == '?') {
			numBlanks++;
		} else {
			handString += (*i)->getLetter();
			tiles.push_back((*i));
		}
		
	}

	int highestScore = -1;
	int xPos = -1;
	int yPos = -1;
	int hori = -1;
	std::string bestString = "";


	for (unsigned int x = 0; x < board.getColumns(); ++x)
	{

		for (unsigned int y = 0; y < board.getRows(); ++y)
		{

			if (board.getSquare(x+1, y+1)->isOccupied()) {;
				continue;
			}

			for (int h = 0; h < 2; ++h)
			{
				std::vector<std::pair<std::string, unsigned int>> results;

				std::set<std::string> prefix;

				std::string a;
				if (h == 1) {
					a = scanVert(board, true, x-1, y);
					
				} else {
					a = scanHori(board, true, x, y-1);
					
				}

				if (numBlanks > 0) {
					//resolve blanks
					
				} else {
					generatePossiblePer(a, handString, dictionary, prefix, board);
				}

				for (std::set<std::string>::iterator it = prefix.begin(); it != prefix.end(); ++it) {

					try {
						std::vector<Tile*> t;
						t = this->takeTiles((*it), true);
						this->addTiles(t);
						PlaceMove pm(x, y, (bool)h, (*it), this);
						this->addTiles(t);

						try {
							results = board.getPlaceMoveResults(pm);
						} catch (const std::exception& e) {
							continue;
						}

					} catch (const std::exception& e) {
						continue;
					}

					int score = -1;

					//check if valid word
					std::string wordFormed = results.back().first;
					if (dictionary.isLegalWord(wordFormed)) {
						//calculate score

						for (unsigned int i = 0; i < results.size(); ++i)
						{
							if (dictionary.isLegalWord(results[i].first)) {
								score += results[i].second;
							} else {
								//discard move
								score = -1;
								break;
							}
							
						}

						if (score > highestScore) {
							highestScore = score;
							bestString = (*it);
							xPos = x;
							yPos = y;
							hori = h;
						}
					} else {
						//discard
						continue;
					}

				}

			}
		}
	}

	//execute best move
	if (highestScore > -1) {
		PlaceMove pm(xPos, yPos, (bool)hori, bestString, this);
		board.executePlaceMove(pm);
		if (bestString.length() == 7) {
			this->addPoints(50);
		}
		this->addPoints(highestScore+1);
	} else {
		//pass
		PassMove pass(this);
		std::cout << "CPU passed" << std::endl;
		pass.execute(board, bag, dictionary);
	}
}


void CPUS::generatePossiblePer(std::string prefix, std::string input, Dictionary & dictionary, std::set<std::string> & per, Board & board) {
	if (!dictionary.isPrefixWord(prefix)) {
		return;
	}

	//use legal words
	if (dictionary.isPrefixWord(prefix)) {
		per.insert(prefix);
	}

	for (unsigned int i = 0; i < input.length(); ++i)
	{
		std::string p = prefix + input.at(i);
		std::string in = input;
		in.erase(i, 1);
		//increment index
		generatePossiblePer(p, in, dictionary, per, board);
	}
}

std::string CPUS::scanHori(Board & board, bool left, int x, int y) {
	
	std::string s = "";
	if (left) {
		while (y > 0 && board.getSquare(x+1, y+1)->isOccupied()) {
			s += board.getSquare(x+1, y+1)->getLetter();
			y--;
		}
		std::reverse(s.begin(), s.end());
		return s;
	} else {
		//right
		while (y < (int)board.getColumns()) {
			s += board.getSquare(x+1, y+1)->getLetter();
			y++;
		}
		return s;
	}
}

std::string CPUS::scanVert(Board & board, bool up, int x, int y) {
	
	std::string s = "";
	if (up) {
		while (x > 0 && board.getSquare(x+1, y+1)->isOccupied()) {
			s += board.getSquare(x+1, y+1)->getLetter();
			x--;
		}
		std::reverse(s.begin(), s.end());
		return s;
	} else {
		//right
		while (x < (int)board.getRows()) {
			s += board.getSquare(x+1, y+1)->getLetter();
			x++;
		}
		return s;
	}
}