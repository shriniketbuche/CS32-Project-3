#ifndef BOARD_H
#define BOARD_H

#include "Side.h"

class Board
{
public:
	Board(int nHoles, int nInitialBeansPerHole);
	~Board();
	Board(const Board& other);
    Board& operator=(const Board& other);
	int holes() const;
	int beans(Side s, int hole) const;
	int beansInPlay(Side s) const;
	int totalBeans() const;
	bool sow(Side s, int hole, Side& endSide, int& endHole);
	bool moveToPot(Side s, int hole, Side potOwner);
	bool setBeans(Side s, int hole, int beans);
private:
	struct Node //Doubly Linked List implementation
	{
		int beans;
		Node* m_next;
		Node* m_prev;
	};
	Node* north;
	Node* south;
	int m_holes; //Keep track of the number of holes
	void insertNodeforGame(int val); //Used to insert nodes for constructor
};
#endif
