#include "Player.h"
#include "Move.h"
class CPUL : public Player
{

	void generatePossiblePer(std::string prefix, std::string input, Dictionary & dictionary, std::set<std::string> & per, Board & board);

	std::string scanVert(Board & board, bool up, int x, int y);

	std::string scanHori(Board & board, bool left, int x, int y);

public:
	/* Constructor giving the player the given name, and setting their points to 0.
		Does not give the player any tiles.
	*/
	CPUL(size_t maxTiles);

	/* Destructor for a player. Deletes all the tiles the player still has. */
	virtual ~CPUL ();

	virtual void performCPUMove(Board & board, Bag & bag, Dictionary & dictionary);

};

class CPUS : public Player
{

	void generatePossiblePer(std::string prefix, std::string input, Dictionary & dictionary, std::set<std::string> & per, Board & board);

	std::string scanVert(Board & board, bool up, int x, int y);

	std::string scanHori(Board & board, bool left, int x, int y);

public:
	/* Constructor giving the player the given name, and setting their points to 0.
		Does not give the player any tiles.
	*/
	CPUS(size_t maxTiles);

	/* Destructor for a player. Deletes all the tiles the player still has. */
	virtual ~CPUS ();

	virtual void performCPUMove(Board & board, Bag & bag, Dictionary & dictionary);

};