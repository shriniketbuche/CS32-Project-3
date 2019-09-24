#include "Board.h"
#include <iostream>
using namespace std;

void Board::insertNodeforGame(int val) //inserts nodes including both the north and south nodes
{
	if(north == nullptr && south == nullptr) //construct north and south
	{
		north = new Node;
		north -> beans = 0;
		north -> m_next = nullptr;
		north -> m_prev = nullptr;

		south = new Node;
		south -> beans = 0;
		south -> m_next = nullptr;
		south -> m_prev = nullptr;

		Node *a = new Node;
		a -> beans = val;
		a -> m_next = south;
		a -> m_prev = north;
		north -> m_next = a;
		south -> m_prev = a;

		Node *b = new Node;
		b -> beans = val;
		b -> m_next = north;
		b -> m_prev = south;
		north -> m_prev = b;
		south -> m_next = b;
	}
	else //construct two nodes, one on the south which is north's next
	{
		Node *s = new Node;
		s -> beans = val;
		s -> m_prev = north;
		s -> m_next = north -> m_next;
		north -> m_next -> m_prev = s;
		north -> m_next = s;
        //and one on the north which is south's next
		Node *t = new Node;
		t -> beans = val;
		t -> m_prev = south;
		t -> m_next = south -> m_next;
		south -> m_next -> m_prev = t;
		south -> m_next = t;
	}
}
Board::~Board() //delete all the nodes in a circular order
{
    if(north == nullptr && south == nullptr){}
    Node* temp = north;
    temp = temp -> m_next;
    north -> m_prev -> m_next = nullptr;
    while(temp -> m_next != nullptr)
    {
        north -> m_next = temp -> m_next; //move north's pointer over to the next next node
        temp -> m_prev = nullptr;
        delete temp; //delete the node next to north
        temp = north;
        temp = temp -> m_next;
    }
    delete north;
}
Board::Board(const Board& other) //copy constructor
{
	north = new Node; //create north
	north -> beans = other.north ->beans;

	south = new Node; //create south
	south -> beans = other.south -> beans;

	m_holes = other.m_holes; //initlaize holes

	north -> m_next = nullptr; //set all links to null at first, to avoid weird pointer problems
	north -> m_prev = nullptr;
	south -> m_next = nullptr;
	south -> m_prev = nullptr;

	Node * p = new Node; //make a new node to connect north and south
	p -> beans = other.north -> m_next -> beans;
	p -> m_next = south;
	p -> m_prev = north;
	north -> m_next = p;
	south -> m_prev = p;

	Node * q = new Node; //repeat for the north side
	q -> beans = other.north -> m_prev -> beans;
	q -> m_next = north;
	q -> m_prev = south;
	south -> m_next = q;
	north -> m_prev = q;

	//south half
	Node* tempval1 = other.north -> m_next -> m_next;
	Node* temp1 = north -> m_next;
	for(int i = 0; i < other.m_holes -1; i++)
	{
		Node *s = new Node;
		s -> beans = tempval1 -> beans;
		s -> m_prev = temp1;
		s -> m_next = temp1 -> m_next;
		temp1 -> m_next -> m_prev = s;
		temp1 -> m_next = s;
		temp1 = temp1 -> m_next;
		tempval1 = tempval1 -> m_next;
	}

	//north half
	Node *tempval2 = other.north -> m_prev -> m_prev;
	Node *temp2 = north -> m_prev;
	for(int j = 0; j < other.m_holes -1; j++)
	{
		Node *t = new Node;
		t -> beans = tempval2 -> beans;
		t -> m_next = temp2;
		t -> m_prev = temp2 -> m_prev;
		temp2 -> m_prev -> m_next = t;
		temp2 -> m_prev = t;
		temp2 = temp2 -> m_prev;
		tempval2 = tempval2 -> m_prev;
	}

}
Board& Board::operator=(const Board& other) //delete everything and then copy
{
    if(this != & other)
    {
        Node* temp = north;
        temp = temp -> m_next;
        north -> m_prev -> m_next = nullptr;
        while(temp -> m_next != nullptr)
        {
            north -> m_next = temp -> m_next; //move north's pointer over to the next next node
            temp -> m_prev = nullptr;
            delete temp; //delete the node next to north
            temp = north;
            temp = temp -> m_next;
        }
        delete north;
        north = nullptr;
        south = nullptr;
        
        north = new Node; //create north
        north -> beans = other.north ->beans;
        
        south = new Node; //create south
        south -> beans = other.south -> beans;
        
        m_holes = other.m_holes; //initlaize holes
        
        north -> m_next = nullptr; //set all links to null at first, to avoid weird pointer problems
        north -> m_prev = nullptr;
        south -> m_next = nullptr;
        south -> m_prev = nullptr;
        
        Node * p = new Node; //make a new node to connect north and south
        p -> beans = other.north -> m_next -> beans;
        p -> m_next = south;
        p -> m_prev = north;
        north -> m_next = p;
        south -> m_prev = p;
        
        Node * q = new Node; //repeat for the north side
        q -> beans = other.north -> m_prev -> beans;
        q -> m_next = north;
        q -> m_prev = south;
        south -> m_next = q;
        north -> m_prev = q;
        
        //south half
        Node* tempval1 = other.north -> m_next -> m_next;
        Node* temp1 = north -> m_next;
        for(int i = 0; i < other.m_holes -1; i++)
        {
            Node *s = new Node;
            s -> beans = tempval1 -> beans;
            s -> m_prev = temp1;
            s -> m_next = temp1 -> m_next;
            temp1 -> m_next -> m_prev = s;
            temp1 -> m_next = s;
            temp1 = temp1 -> m_next;
            tempval1 = tempval1 -> m_next;
        }
        
        //north half
        Node *tempval2 = other.north -> m_prev -> m_prev;
        Node *temp2 = north -> m_prev;
        for(int j = 0; j < other.m_holes -1; j++)
        {
            Node *t = new Node;
            t -> beans = tempval2 -> beans;
            t -> m_next = temp2;
            t -> m_prev = temp2 -> m_prev;
            temp2 -> m_prev -> m_next = t;
            temp2 -> m_prev = t;
            temp2 = temp2 -> m_prev;
            tempval2 = tempval2 -> m_prev;
        }
    }
    return *this;
}
Board::Board(int nHoles, int nInitialBeansPerHole) //constructor
{
	north = nullptr;  //set north to null
	south = nullptr;  //set south to null as it will be checked in the insert function
	if(nHoles <= 0)
	{
        nHoles = 1; //as per the spec if holes is not positive act as if it were 1;
	}
	if(nInitialBeansPerHole < 0)
	{
		nInitialBeansPerHole = 0; //same idea
	}
	for(int i = 0; i < nHoles; i++)
	{
		insertNodeforGame(nInitialBeansPerHole); //call insert and create the board
	}
	m_holes = nHoles;
}

int Board::holes() const
{
	return m_holes; //simple getter
}

int Board::beans(Side s, int hole) const
{
	Node* temp = north;
	if(hole > m_holes) //bounds checking
	{
		return -1;
	}
	if(hole < 0)
	{
		return -1;
	}
	else
	{
		if(s == SOUTH) //if we're at the south
		{
			if(hole == 0)
			{
				return(south -> beans); //if at pot
			}
			for(int i = 0; i < hole; i++)
			{
				temp = temp->m_next; //move forward till you get to the desired node
			}
			return(temp -> beans); //return its beans
		}
		else
		{
			if(hole == 0)
			{
				return(north -> beans);
			}
			for(int i = 0; i < hole; i++)
			{
				temp = temp-> m_prev; //in this case move backward as it follows the numbering system more intuitively
			}
		}	return(temp -> beans);
	}
}

int Board::beansInPlay(Side s) const
{
	int sum = 0; //increment a sum and return it
	if(s == SOUTH)
	{
		Node *temp = north -> m_next;
		for(int i = 0; i < m_holes; i++)
		{
			sum += temp -> beans; //just add the beans in every node
			temp = temp -> m_next;
		}
	}
	else
	{
		Node *temp = north -> m_prev;
		for(int i = 0; i < m_holes; i++)
		{
			sum += temp -> beans;
			temp = temp -> m_prev;
		}
	}
	return sum;
}

int Board::totalBeans() const
{
	int sum = 0;
	Node *temp = north;
	while(temp->m_next != north) //while we haven't made a full circle
	{
		sum += temp -> beans; //add all the beans
		temp = temp -> m_next;
	}
	sum += temp->beans; //return the sum
	return sum;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
	if(hole > m_holes) //bounds checking
	{
		return false;
	}
	if(hole <= 0)
	{
		return false;
	}
    endSide = s; //set endside to startSideis
	if(s == SOUTH) //if we are at the south node
	{
		int pos = hole; //our position is our hole
		Node *temp = north; //start from north
		for(int i = 0; i < hole; i++)
		{
			temp = temp -> m_next; //move to desired hole
		}
		int possibleBeans = temp -> beans; //get our possibleBeans
		temp -> beans -= possibleBeans; //subtract from the hole
		temp = temp -> m_next; //move on
		pos++;
		if(pos == m_holes +1) //if pos is next to a pot
		{
			pos = 0; //increment the pos
		}
		for(int j = possibleBeans; j > 0; j--)
		{
			if(temp == south) //if we are at a pot
			{
				endSide = NORTH; //change side
				pos = 0; //start pos from 0
			}
			if(temp == north) //we need to skip this pot
			{
				endSide = SOUTH; //switch side
				pos = 0; //start from 0
				j++; //incrememt our beans to maintain the turn
				temp = temp -> m_next; //move on to the next node
                pos++; //increment position
				continue;
			}
			temp-> beans += 1; //otherwise add 1 to the hole
			temp = temp-> m_next; //move on
			pos++; //increment pos
			if(j == 1)
			{
                pos--; //if we're at the end, decrement otherwise we will go one too far
			}
		}
		if(pos == 0) //if pos is 0
		{
			if(endSide == NORTH)
			{
				endSide = SOUTH; //we must end at a pot oppposite the one we assigned in the loop
			}
			else
			{
				endSide = NORTH;
			}
		}
		if(endSide == NORTH && pos != 0)
		{
			endHole = m_holes - pos + 1; //due to the north naming convention, the holes are not named CCW so we must reverse order
            return true;
		}
		else
		{
			endHole = pos; //naming is fine
		}
		return true;
	}
	if(s == NORTH) //repeat for north
	{
		int pos = m_holes - hole + 1; //but our pos must be converted to our CCW notation
		Node *temp = south;
		for(int i = 0; i < pos; i++)
		{
			temp = temp -> m_next;
		}
		int possibleBeans = temp -> beans;
		temp -> beans -= possibleBeans;
        temp = temp -> m_next;
		pos++;
        if(pos == m_holes +1)
        {
            pos = 0;
        }
		for(int j = possibleBeans; j > 0; j--)
		{
			if(temp == north)
			{
				endSide = SOUTH;
				pos = 0;
			}
			if(temp == south)
			{
				endSide = NORTH;
				pos = 0;
				j++;
				temp = temp -> m_next;
                pos++;
				continue;
			}
			temp -> beans += 1;
			temp = temp-> m_next;
			pos++;
			if(j == 1)
			{
				pos--;
			}
		}
        if(pos == 0)
        {
            if(endSide == NORTH)
            {
                endSide = SOUTH;
            }
            else
            {
                endSide = NORTH;
            }
        }
		if(endSide == NORTH && pos != 0)
		{
			endHole = m_holes - pos +1; //and later reconverted if we are on the north side
            return true;
		}
		else
		{
			endHole = pos; //South follows our naming convention
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool Board::moveToPot(Side s, int hole, Side potOwner)
{
	if(hole > m_holes) //bounds checking
	{
		return false;
	}
	if(hole <= 0)
	{
		return false;
	}
	if(s == SOUTH)
	{
		Node* temp = north; //if we're at the south
		for(int i = 0; i < hole; i++)
		{
			temp = temp -> m_next; //move to desired hole
		}
		if(potOwner == NORTH)
		{
			north -> beans += temp -> beans; //add it to the appropriate pot
			temp -> beans = 0;
		}
		else
		{
			south -> beans += temp -> beans;
			temp -> beans = 0;
		}
		return true;
	}
	else
	{
		Node* temp = north;
		for(int i = 0; i < hole; i++)
		{
			temp = temp -> m_prev; //same thing
		}
		if(potOwner == NORTH)
		{
			north -> beans += temp -> beans;
			temp -> beans = 0;
		}
		else
		{
			south -> beans += temp -> beans;
			temp -> beans = 0;
		}
		return true;
	}
	return false;
}

bool Board::setBeans(Side s, int hole, int beans)
{
	if(hole > m_holes) //bounds checking
	{
		return false;
	}
	if(hole < 0)
	{
		return false;
	}
	if(hole == 0) //if we are at a pot
	{
		if(s == NORTH)
		{
			north -> beans = beans; //set the north/south nodes appropriately
		}
		else
		{
			south -> beans = beans;
		}
	}
	Node* temp = north;
	if(s == SOUTH)
	{
		for(int i = 0; i < hole; i++)
		{
			temp = temp -> m_next; //get to the desired hole
		}
		temp -> beans = beans; //set the beans
		return true;
	}
	else //same thing
	{
		for(int i = 0; i < hole; i++)
		{
			temp = temp -> m_prev;
		}
		temp -> beans = beans;
		return true;
	}
}

