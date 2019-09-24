#include "Game.h"
#include <iostream>
using namespace std;

Game::Game(const Board &b, Player* south, Player* north): m_Board(b) //construct the board
{
	m_South = south;
    m_North = north; //point to the approprieate player
    turn = 0;
}

void Game::display() const
{
	cout << "		" << m_North-> name() << endl; //just a couple tabs to center the name
	cout << "  ";
	for(int i = 1; i <= m_Board.holes(); i++)
	{
		cout << m_Board.beans(NORTH, i) << " "; //cout NORTH
	}
	cout << endl;
	cout << m_Board.beans(NORTH, 0);
	for(int i = 1; i <= m_Board.holes(); i++)
	{
		cout << "  ";  // cout the spaces between pots
	}
	cout << " " << m_Board.beans(SOUTH, 0);
	cout << endl;
	cout << "  ";
	for(int i = 1; i <= m_Board.holes(); i++)
	{
		cout << m_Board.beans(SOUTH, i) << " "; //cout south
	}
	cout << endl;
	cout <<"		" << m_South->name() << endl; //end with the name
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
	if(m_Board.beansInPlay(NORTH) != 0 && m_Board.beansInPlay(SOUTH) != 0) //if there are still beans
	{
		over = false;
		return;
	}
	else if(m_Board.beansInPlay(NORTH) == 0) //if north has no beans
	{
		over = true; //game is over
		int totalSouth = 0;
		for(int i = 0; i <= m_Board.holes(); i++)
		{
			totalSouth += m_Board.beans(SOUTH, i); //sweep the beans to the pot, without actually doing so as it is const
		}
		if(totalSouth > m_Board.beans(NORTH, 0)) //if south ends up being greater than north's pot
		{
			hasWinner = true; //we have a winner
			winner = SOUTH; //who is south
		}
		else if(totalSouth < m_Board.beans(NORTH, 0))
		{
			hasWinner = true;
			winner = NORTH; //the winner is north
		}
		else
		{
			hasWinner = false; //its a tie
		}
		return;
	}
	else //same thing but with south
	{
		over = true;
		int totalNorth = 0;
		for(int i = 0; i <= m_Board.holes(); i++)
		{
			totalNorth += m_Board.beans(NORTH, i);
		}
		if(totalNorth < m_Board.beans(SOUTH, 0))
		{
			hasWinner = true;
			winner = SOUTH;
		}
		else if(totalNorth > m_Board.beans(SOUTH, 0))
		{
			hasWinner = true;
			winner = NORTH;
		}
		else
		{
			hasWinner = false;
		}
		return;
	}
}

bool Game::move()
{
	bool over;
	bool hasWinner;
	Side s;
	int endHole;
	int startHole;
	Side endSide;
	status(over, hasWinner, s); //if its already over
	if(over == true)
	{
        return false; // end
	}
	if(m_North -> isInteractive() || m_South -> isInteractive()) //if one is a human player
	{
		if(turn%2 == 0) //turn will moderate whose turn it is, even South, odd North
		{
			startHole = m_South -> chooseMove(m_Board, SOUTH); //find the hole
			m_Board.sow(SOUTH, startHole, endSide, endHole); //sow it
            if(!(m_South -> isInteractive()))
            {
                cout << m_South->name() << " chose hole " << startHole << endl; //if its a computer tell the hole chosen
            }
            status(over, hasWinner, s); //check if over
            if(over == true)
            {
                if(m_Board.beansInPlay(NORTH) != 0)
                {
                    cout << "Moving remaining beans to pot." << endl; //sweep the remaining beans to pot
                    for(int i = 1; i <= m_Board.holes(); i++)
                    {
                        m_Board.moveToPot(NORTH, i, NORTH);
                    }
                }
                if(m_Board.beansInPlay(SOUTH) != 0)
                {
                    cout << "Moving remeaining beans to pot." << endl;
                    for(int i = 1; i <= m_Board.holes(); i++)
                    {
                        m_Board.moveToPot(SOUTH, i, SOUTH);
                    }
                }
                if(hasWinner == false)
                {
                    display();
                    cout << "The game results in a tie!" << endl; //if has winner is false its a tie
                }
                if(s == NORTH)
                {
                    display();
                    cout << m_North -> name() << " wins!" << endl; //inform the user about who won
                }
                if(s == SOUTH)
                {
                    display();
                    cout << m_South -> name() << " wins!" << endl;
                }
                return false;
            }
			while(endHole == 0) //if endhole is 0
			{
				display();
				status(over, hasWinner, s);
				if(over == true) //check if over
				{
                    if(m_Board.beansInPlay(NORTH) != 0)
                    {
                        cout << "Moving remaining beans to pot." << endl; //sweep the remaining beans to pot
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(NORTH, i, NORTH);
                        }
                    }
                    if(m_Board.beansInPlay(SOUTH) != 0)
                    {
                        cout << "Moving remeaining beans to pot." << endl;
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(SOUTH, i, SOUTH);
                        }
                    }
                    if(hasWinner == false)
                    {
                        display();
                        cout << "The game results in a tie!" << endl; //if has winner is false its a tie
                    }
                    if(s == NORTH)
                    {
                        display();
                        cout << m_North -> name() << " wins!" << endl; //inform the user about who won
                    }
                    if(s == SOUTH)
                    {
                        display();
                        cout << m_South -> name() << " wins!" << endl;
                    }
					return false;
					break;
				}
				else
				{
					cout << m_South -> name() << " landed on their pot, they get another turn." << endl; //inform user that they landed on pot
				}
				startHole = m_South -> chooseMove(m_Board, SOUTH); //choose a hole
				m_Board.sow(SOUTH, startHole, endSide, endHole); //sow it
                if(!(m_South -> isInteractive()))
                {
                    cout << m_South->name() << " chose hole " << startHole << endl; //tell the user
                }
                status(over, hasWinner, s); //check if over
                if(over == true)
                {
                    if(m_Board.beansInPlay(NORTH) != 0)
                    {
                        cout << "Moving remaining beans to pot." << endl; //sweep the remaining beans to pot
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(NORTH, i, NORTH);
                        }
                    }
                    if(m_Board.beansInPlay(SOUTH) != 0)
                    {
                        cout << "Moving remeaining beans to pot." << endl;
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(SOUTH, i, SOUTH);
                        }
                    }
                    if(hasWinner == false)
                    {
                        display();
                        cout << "The game results in a tie!" << endl; //if has winner is false its a tie
                    }
                    if(s == NORTH)
                    {
                        display();
                        cout << m_North -> name() << " wins!" << endl; //inform the user about who won
                    }
                    if(s == SOUTH)
                    {
                        display();
                        cout << m_South -> name() << " wins!" << endl;
                    }
                    return false;
                }
                if(m_Board.beans(SOUTH, endHole) == 1 && endSide == SOUTH && endHole != 0) //see if beans will be captured
                {
                    if(m_Board.beans(NORTH, endHole) != 0)
                    {
                        cout << m_North -> name() << " beans will be captured!" << endl; //inform
                        m_Board.moveToPot(NORTH, endHole, SOUTH); //move the appropriate beans to pot
                        m_Board.moveToPot(SOUTH, endHole, SOUTH);
                    }
                }
			}
			if(m_Board.beans(SOUTH, endHole) == 1 && endSide == SOUTH && endHole != 0) //see if beans will be captured
			{
				if(m_Board.beans(NORTH, endHole) != 0)
				{
					cout << m_North -> name() << " beans will be captured!" << endl;//inform
					m_Board.moveToPot(NORTH, endHole, SOUTH); //same thing
					m_Board.moveToPot(SOUTH, endHole, SOUTH);
				}
			}
			turn++;
			return true;
		}
		else //its South's turn, do the same thing
		{
			startHole = m_North -> chooseMove(m_Board, NORTH);
			m_Board.sow(NORTH, startHole, endSide, endHole);
            if(!(m_North -> isInteractive()))
            {
                cout << m_North->name() << " chose hole " << startHole << endl;
            }
            status(over, hasWinner, s);
            if(over == true)
            {
                if(m_Board.beansInPlay(NORTH) != 0)
                {
                    cout << "Moving remaining beans to pot." << endl; //sweep the remaining beans to pot
                    for(int i = 1; i <= m_Board.holes(); i++)
                    {
                        m_Board.moveToPot(NORTH, i, NORTH);
                    }
                }
                if(m_Board.beansInPlay(SOUTH) != 0)
                {
                    cout << "Moving remeaining beans to pot." << endl;
                    for(int i = 1; i <= m_Board.holes(); i++)
                    {
                        m_Board.moveToPot(SOUTH, i, SOUTH);
                    }
                }
                if(hasWinner == false)
                {
                    display();
                    cout << "The game results in a tie!" << endl; //if has winner is false its a tie
                }
                if(s == NORTH)
                {
                    display();
                    cout << m_North -> name() << " wins!" << endl; //inform the user about who won
                }
                if(s == SOUTH)
                {
                    display();
                    cout << m_South -> name() << " wins!" << endl;
                }
                return false;
            }
			while(endHole == 0)
			{
				display();
				status(over, hasWinner, s);
				if(over == true)
				{
                    if(m_Board.beansInPlay(NORTH) != 0)
                    {
                        cout << "Moving remaining beans to pot." << endl; //sweep the remaining beans to pot
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(NORTH, i, NORTH);
                        }
                    }
                    if(m_Board.beansInPlay(SOUTH) != 0)
                    {
                        cout << "Moving remeaining beans to pot." << endl;
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(SOUTH, i, SOUTH);
                        }
                    }
                    if(hasWinner == false)
                    {
                        display();
                        cout << "The game results in a tie!" << endl; //if has winner is false its a tie
                    }
                    if(s == NORTH)
                    {
                        display();
                        cout << m_North -> name() << " wins!" << endl; //inform the user about who won
                    }
                    if(s == SOUTH)
                    {
                        display();
                        cout << m_South -> name() << " wins!" << endl;
                    }
					return false;
					break;
				}
				else
				{
				cout << m_North -> name() << " landed on their pot, they get another turn." << endl;
				}
                startHole = m_North -> chooseMove(m_Board, NORTH);
				m_Board.sow(NORTH, startHole, endSide, endHole);
                if(!(m_North -> isInteractive()))
                {
                    cout << m_North->name() << " chose hole " << startHole << endl;
                }
                status(over, hasWinner, s);
                if(over == true)
                {
                    if(m_Board.beansInPlay(NORTH) != 0)
                    {
                        cout << "Moving remaining beans to pot." << endl; //sweep the remaining beans to pot
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(NORTH, i, NORTH);
                        }
                    }
                    if(m_Board.beansInPlay(SOUTH) != 0)
                    {
                        cout << "Moving remeaining beans to pot." << endl;
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(SOUTH, i, SOUTH);
                        }
                    }
                    if(hasWinner == false)
                    {
                        display();
                        cout << "The game results in a tie!" << endl; //if has winner is false its a tie
                    }
                    if(s == NORTH)
                    {
                        display();
                        cout << m_North -> name() << " wins!" << endl; //inform the user about who won
                    }
                    if(s == SOUTH)
                    {
                        display();
                        cout << m_South -> name() << " wins!" << endl;
                    }
                    return false;
                }
                if(m_Board.beans(NORTH, endHole) == 1 && endSide == NORTH && endHole != 0)
                {
                    if(m_Board.beans(SOUTH, endHole) != 0)
                    {
                        cout << m_South -> name() << " beans will be captured!" << endl;
                        m_Board.moveToPot(SOUTH, endHole, NORTH);
                        m_Board.moveToPot(NORTH, endHole, NORTH);
                    }
                }
			}
			if(m_Board.beans(NORTH, endHole) == 1 && endSide == NORTH && endHole != 0)
			{
				if(m_Board.beans(SOUTH, endHole) != 0)
				{
					cout << m_South -> name() << " beans will be captured!" << endl;
					m_Board.moveToPot(SOUTH, endHole, NORTH);
					m_Board.moveToPot(NORTH, endHole, NORTH);
				}
			}
			turn++;
			return true;
		}
	}
	else //they are both computers
	{
		if(turn%2 == 0)
		{
			startHole = m_South -> chooseMove(m_Board, SOUTH);
			m_Board.sow(SOUTH, startHole, endSide, endHole);
            cout << "Press enter to continue" << endl; //add the press enter to continue function
            cin.ignore(1000, '\n');
            cout << m_South ->name() << " chose Hole " << startHole << endl; //inform
            status(over, hasWinner, s);
            if(over == true) //the rest is the same
            {
                if(m_Board.beansInPlay(NORTH) != 0)
                {
                    cout << "Moving remaining beans to pot." << endl; //sweep the remaining beans to pot
                    for(int i = 1; i <= m_Board.holes(); i++)
                    {
                        m_Board.moveToPot(NORTH, i, NORTH);
                    }
                }
                if(m_Board.beansInPlay(SOUTH) != 0)
                {
                    cout << "Moving remeaining beans to pot." << endl;
                    for(int i = 1; i <= m_Board.holes(); i++)
                    {
                        m_Board.moveToPot(SOUTH, i, SOUTH);
                    }
                }
                if(hasWinner == false)
                {
                    display();
                    cout << "The game results in a tie!" << endl; //if has winner is false its a tie
                }
                if(s == NORTH)
                {
                    display();
                    cout << m_North -> name() << " wins!" << endl; //inform the user about who won
                }
                if(s == SOUTH)
                {
                    display();
                    cout << m_South -> name() << " wins!" << endl;
                }
                return false;
            }
			while(endHole == 0)
            {
                display();
				status(over, hasWinner, s);
				if(over == true)
                {
                    if(m_Board.beansInPlay(NORTH) != 0)
                    {
                        cout << "Moving remaining beans to pot." << endl; //sweep the remaining beans to pot
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(NORTH, i, NORTH);
                        }
                    }
                    if(m_Board.beansInPlay(SOUTH) != 0)
                    {
                        cout << "Moving remeaining beans to pot." << endl;
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(SOUTH, i, SOUTH);
                        }
                    }
                    if(hasWinner == false)
                    {
                        display();
                        cout << "The game results in a tie!" << endl; //if has winner is false its a tie
                    }
                    if(s == NORTH)
                    {
                        display();
                        cout << m_North -> name() << " wins!" << endl; //inform the user about who won
                    }
                    if(s == SOUTH)
                    {
                        display();
                        cout << m_South -> name() << " wins!" << endl;
                    }
					return false;
					break;
				}
				else
				{
					cout << m_South -> name() << " landed on their pot, they get another turn." << endl;
				}
				startHole = m_South -> chooseMove(m_Board, SOUTH);
				m_Board.sow(SOUTH, startHole, endSide, endHole);
                cout << "Press enter to continue" << endl;
                cin.ignore(1000, '\n');
                cout << m_South ->name() << " chose Hole " << startHole << endl;
                status(over, hasWinner, s);
                if(over == true)
                {
                    if(m_Board.beansInPlay(NORTH) != 0)
                    {
                        cout << "Moving remaining beans to pot." << endl; //sweep the remaining beans to pot
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(NORTH, i, NORTH);
                        }
                    }
                    if(m_Board.beansInPlay(SOUTH) != 0)
                    {
                        cout << "Moving remeaining beans to pot." << endl;
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(SOUTH, i, SOUTH);
                        }
                    }
                    if(hasWinner == false)
                    {
                        display();
                        cout << "The game results in a tie!" << endl; //if has winner is false its a tie
                    }
                    if(s == NORTH)
                    {
                        display();
                        cout << m_North -> name() << " wins!" << endl; //inform the user about who won
                    }
                    if(s == SOUTH)
                    {
                        display();
                        cout << m_South -> name() << " wins!" << endl;
                    }
                    return false;
                }
                if(m_Board.beans(SOUTH, endHole) == 1 && endSide == SOUTH && endHole != 0)
                {
                    if(m_Board.beans(NORTH, endHole) != 0)
                    {
                        display();
                        cout << m_North -> name() << " beans will be captured!" << endl;
                        m_Board.moveToPot(NORTH, endHole, SOUTH);
                        m_Board.moveToPot(SOUTH, endHole, SOUTH);
                    }
                }
			}
			if(m_Board.beans(SOUTH, endHole) == 1 && endSide == SOUTH && endHole != 0)
			{
				if(m_Board.beans(NORTH, endHole) != 0)
				{
                    display();
					cout << m_North -> name() << " beans will be captured!" << endl;
					m_Board.moveToPot(NORTH, endHole, SOUTH);
					m_Board.moveToPot(SOUTH, endHole, SOUTH);
				}
			}
			turn++;
			return true;
		}
		else
		{
			startHole = m_North -> chooseMove(m_Board, NORTH);
			m_Board.sow(NORTH, startHole, endSide, endHole);
            status(over, hasWinner, s);
            cout << "Press enter to continue" << endl;
            cin.ignore(1000, '\n');
            cout << m_North ->name() << " chose Hole " << startHole << endl;
            {
                if(over == true)
                {
                    if(m_Board.beansInPlay(NORTH) != 0)
                    {
                        cout << "Moving remaining beans to pot." << endl; //sweep the remaining beans to pot
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(NORTH, i, NORTH);
                        }
                    }
                    if(m_Board.beansInPlay(SOUTH) != 0)
                    {
                        cout << "Moving remeaining beans to pot." << endl;
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(SOUTH, i, SOUTH);
                        }
                    }
                    if(hasWinner == false)
                    {
                        display();
                        cout << "The game results in a tie!" << endl; //if has winner is false its a tie
                    }
                    if(s == NORTH)
                    {
                        display();
                        cout << m_North -> name() << " wins!" << endl; //inform the user about who won
                    }
                    if(s == SOUTH)
                    {
                        display();
                        cout << m_South -> name() << " wins!" << endl;
                    }
                    return false;
                }
            }
			while(endHole == 0)
			{
                display();
				status(over, hasWinner, s);
				if(over == true)
				{
                    if(m_Board.beansInPlay(NORTH) != 0)
                    {
                        cout << "Moving remaining beans to pot." << endl; //sweep the remaining beans to pot
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(NORTH, i, NORTH);
                        }
                    }
                    if(m_Board.beansInPlay(SOUTH) != 0)
                    {
                        cout << "Moving remeaining beans to pot." << endl;
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(SOUTH, i, SOUTH);
                        }
                    }
                    if(hasWinner == false)
                    {
                        display();
                        cout << "The game results in a tie!" << endl; //if has winner is false its a tie
                    }
                    if(s == NORTH)
                    {
                        display();
                        cout << m_North -> name() << " wins!" << endl; //inform the user about who won
                    }
                    if(s == SOUTH)
                    {
                        display();
                        cout << m_South -> name() << " wins!" << endl;
                    }
					return false;
					break;
				}
				else
				{
					cout << m_North -> name() << " landed on their pot, they get another turn." << endl;
				}
				startHole = m_North -> chooseMove(m_Board, NORTH);
				m_Board.sow(NORTH, startHole, endSide, endHole);
                cout << "Press enter to continue" << endl;
                cin.ignore(1000, '\n');
                cout << m_North ->name() << " chose Hole " << startHole << endl;
                status(over, hasWinner, s);
                if(over == true)
                {
                    if(m_Board.beansInPlay(NORTH) != 0)
                    {
                        cout << "Moving remaining beans to pot." << endl; //sweep the remaining beans to pot
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(NORTH, i, NORTH);
                        }
                    }
                    if(m_Board.beansInPlay(SOUTH) != 0)
                    {
                        cout << "Moving remeaining beans to pot." << endl;
                        for(int i = 1; i <= m_Board.holes(); i++)
                        {
                            m_Board.moveToPot(SOUTH, i, SOUTH);
                        }
                    }
                    if(hasWinner == false)
                    {
                        display();
                        cout << "The game results in a tie!" << endl; //if has winner is false its a tie
                    }
                    if(s == NORTH)
                    {
                        display();
                        cout << m_North -> name() << " wins!" << endl; //inform the user about who won
                    }
                    if(s == SOUTH)
                    {
                        display();
                        cout << m_South -> name() << " wins!" << endl;
                    }
                    return false;
                }
                if(m_Board.beans(NORTH, endHole) == 1 && endSide == NORTH && endHole != 0)
                {
                    if(m_Board.beans(SOUTH, endHole) != 0)
                    {
                        display();
                        cout << m_South -> name() << " beans will be captured!" << endl;
                        m_Board.moveToPot(SOUTH, endHole, NORTH);
                        m_Board.moveToPot(NORTH, endHole, NORTH);
                    }
                }
			}
            status(over, hasWinner, s);
            if(over == true)
            {
                if(m_Board.beansInPlay(NORTH) != 0)
                {
                    cout << "Moving remaining beans to pot." << endl; //sweep the remaining beans to pot
                    for(int i = 1; i <= m_Board.holes(); i++)
                    {
                        m_Board.moveToPot(NORTH, i, NORTH);
                    }
                }
                if(m_Board.beansInPlay(SOUTH) != 0)
                {
                    cout << "Moving remeaining beans to pot." << endl;
                    for(int i = 1; i <= m_Board.holes(); i++)
                    {
                        m_Board.moveToPot(SOUTH, i, SOUTH);
                    }
                }
                if(hasWinner == false)
                {
                    display();
                    cout << "The game results in a tie!" << endl; //if has winner is false its a tie
                }
                if(s == NORTH)
                {
                    display();
                    cout << m_North -> name() << " wins!" << endl; //inform the user about who won
                }
                if(s == SOUTH)
                {
                    display();
                    cout << m_South -> name() << " wins!" << endl;
                }
                return false;
            }
            if(m_Board.beans(NORTH, endHole) == 1 && endSide == NORTH && endHole != 0)
			{
				if(m_Board.beans(SOUTH, endHole) != 0)
				{
                    display();
					cout << m_South -> name() << " beans will be captured!" << endl;
					m_Board.moveToPot(SOUTH, endHole, NORTH);
					m_Board.moveToPot(NORTH, endHole, NORTH);
				}
			}
			turn++;
			return true;
		}
	}
}

void Game::play()
{
	display(); //display the board
	if(move() == false) //if move is false
	{
		return;
	}
	else
	{
		return play(); //otherwise we keep playing
	}
}

int Game::beans(Side s, int hole) const
{
	if(hole > m_Board.holes() || hole < 0) //bounds checking
	{
		return -1;
	}
	else
	{
		return m_Board.beans(s, hole); //return the appropriate hole
	}
}
