#include "Player.h"
#include <iostream>
#include <chrono>
#include <future>
#include <atomic>

class AlarmClock //alarm clock class given to us
{
public:
    AlarmClock(int ms)
    {
        m_timedOut = false;
        m_isRunning = true;
        m_alarmClockFuture = std::async([=]() {
            for (int k = 0; k < ms  &&  m_isRunning; k++)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (m_isRunning)
                m_timedOut = true;
        });
    }
    
    ~AlarmClock()
    {
        m_isRunning = false;
        m_alarmClockFuture.get();
    }
    
    bool timedOut() const
    {
        return m_timedOut;
    }
    
    AlarmClock(const AlarmClock&) = delete;
    AlarmClock& operator=(const AlarmClock&) = delete;
private:
    std::atomic<bool> m_isRunning;
    std::atomic<bool> m_timedOut;
    std::future<void> m_alarmClockFuture;
};



Player::Player(std::string name):m_name(name){} //just set the name

Player::~Player(){}

bool Player::isInteractive() const //normally they will be false
{
	return false;
}
std::string Player::name() const
{
	return m_name; //simple getter
}
bool HumanPlayer::isInteractive() const
{
	return true; //for human it will be true
}

HumanPlayer::HumanPlayer(std::string name):Player(name) //call player's constructor
{}

int HumanPlayer::chooseMove(const Board& b, Side s) const
{
	if(b.beansInPlay(s) == 0) //no move possible
	{
		return -1;
	}
	int hole;
	bool looper = true;
	std::cout << "Enter a hole: " << std::endl;
	std::cin >> hole;
	if(b.beans(s, hole) == 0 || hole == 0 || hole > b.holes() || hole < 0) //check if hole is valid
	{
        while(looper) //if it isn;t loop
		{
			if(hole == 0)
			{
				std::cout << "You cannot choose a pot, enter a valid hole: " << std::endl;
			}
            else if(hole > b.holes() || hole < 0)
            {
                std::cout << "Invalid hole, enter a valid hole: " << std::endl;
            }
			else
			{
				std::cout << "There are no beans in this hole, enter a valid hole: " << std::endl;
			}
			std::cin >> hole;
			if(b.beans(s, hole) == 0 || hole == 0 || hole > b.holes() || hole < 0)
			{
				looper = true;
			}
			else
			{
				looper = false; //until the input finally is valid
			}
		}
	}
	return hole;
}

BadPlayer::BadPlayer(std::string name): Player(name) //call player's constructor
{}

int BadPlayer::chooseMove(const Board& b, Side s) const
{
	if(b.beansInPlay(s) == 0) //bounds checking
	{
		return -1;
	}
	int hole = 1; //start from hole 1
	while(hole <= b.holes())
	{
		if(b.beans(s, hole) == 0) //if it isn't a valid hole
		{
			hole++;
		}
		else
		{
			break; //choose that hole
		}
	}
	return hole;
}

SmartPlayer::SmartPlayer(std::string name): Player(name)
{}

bool checkOver(const Board &b, int &value) //this will be used to ascertain whether the game is over or not
{
    if(b.beansInPlay(NORTH) != 0 && b.beansInPlay(SOUTH) != 0) //this is basically the game status function
    {
        return false;
    }
    else if(b.beansInPlay(NORTH) == 0)
    {
        int totalSouth = 0;
        for(int i = 0; i <= b.holes(); i++)
        {
            totalSouth += b.beans(SOUTH, i);
        }
        if(totalSouth > b.beans(NORTH, 0))
        {
            value = 32768; //if south wins, this is my +inf
        }
        else if(totalSouth < b.beans(NORTH, 0))
        {
            value = -32768; //this is my -inf value
        }
        else
        {
            value = 0; //tie
        }
        return true;
    }
    else //same thing
    {
        int totalNorth = 0;
        for(int i = 0; i <= b.holes(); i++)
        {
            totalNorth += b.beans(NORTH, i);
        }
        if(totalNorth < b.beans(SOUTH, 0))
        {
            value = 32768;
        }
        else if(totalNorth > b.beans(SOUTH, 0))
        {
            value = -32768;
        }
        else
        {
            value = 0;
        }
        return true;
    }
}

void chooseMoveHelper(Side s, const Board& b, int& bestHole, int& value, int depth, AlarmClock& ac) //we will use both to maximise breadth and depth
{
    if(ac.timedOut()) //if we have timed out, stop
    {
        if(checkOver(b, value)) //check if over and adjust value
        {
            bestHole = -1; //as per spec
            return;
        }
        value = b.beans(SOUTH, 0) - b.beans(NORTH, 0); //use heuristic
        bestHole = -1;
        return;
    }
    if(checkOver(b, value)) //check if over
    {
        bestHole = -1;
        return;
    }
    if(depth == 0) //if we have reached the end of depth
    {
        if(checkOver(b, value)) //check if over
        {
            bestHole = -1;
            return;
        }
        value = b.beans(SOUTH, 0) - b.beans(NORTH, 0); //heuristic
        bestHole = -1;
        return;
    }
    if(s == SOUTH) //Maximizer
    {
        for(int possibleHole = 1; possibleHole <= b.holes(); possibleHole++) //loop through holes
        {
            Board copy(b); //make copy
            Side endSide;
            int endHole;
            if(copy.beans(SOUTH, possibleHole) == 0) //if its not valid move on
            {
                continue;
            }
            copy.sow(SOUTH, possibleHole, endSide, endHole); //sow the hole
            value = copy.beans(SOUTH, 0) - copy.beans(NORTH, 0); //calculate heuristic
            if(endHole != 0 && endSide == SOUTH && copy.beans(SOUTH, endHole) == 1 && copy.beans(NORTH, endHole) != 0 && (!checkOver(copy, value))) //check for capture
            {
                copy.moveToPot(SOUTH, endHole, SOUTH);
                copy.moveToPot(NORTH, endHole, SOUTH);
            }
            value = copy.beans(SOUTH, 0) - copy.beans(NORTH, 0);
            checkOver(copy, value);
            if(endHole == 0) //if endhole is 0
            {
                int h2;
                int val2;
                chooseMoveHelper(SOUTH, copy, h2, val2, depth, ac); //call again but wtih same side and depth remains the same
                if(val2 >= value) //if the value is the biggest we've seen
                {
                    value = val2; //choose that whole and that value
                    bestHole = possibleHole;
                }
            }
            value = copy.beans(SOUTH, 0) - copy.beans(NORTH, 0); //heuristic
            checkOver(copy, value);
            int h2;
            int val2;
            chooseMoveHelper(NORTH, copy, h2, val2, depth-1, ac); //call the opponent side and decrement depth
            if(val2 >= value) //if the value is the biggest we;ve seen
            {
                value = val2; //choose that value
                bestHole = possibleHole;
            }
        }
    }
    else //do the same thing
    {
        for(int possibleHole = 1; possibleHole <= b.holes(); possibleHole++)
        {
            Board copy(b);
            Side endSide;
            int endHole;
            if(copy.beans(NORTH, possibleHole) == 0)
            {
                continue;
            }
            copy.sow(NORTH, possibleHole, endSide, endHole);
            value = copy.beans(SOUTH, 0) - copy.beans(NORTH, 0);
            if(endHole != 0 && endSide == NORTH && copy.beans(NORTH, endHole) == 1 && copy.beans(SOUTH, endHole) != 0 && !checkOver(copy, value))
            {
                copy.moveToPot(NORTH, endHole, NORTH);
                copy.moveToPot(SOUTH, endHole, SOUTH);
            }
            value = copy.beans(SOUTH, 0) - copy.beans(NORTH, 0);
            checkOver(copy, value);
            if(endHole == 0)
            {
                int h2;
                int val2;
                chooseMoveHelper(NORTH, copy, h2, val2, depth, ac);
                if(val2 <= value) // if its the smalles we have ever seen
                {
                    value = val2;
                    bestHole = possibleHole;
                }
            }
            value = copy.beans(SOUTH, 0) - copy.beans(NORTH, 0);
            checkOver(copy, value);
            int h2;
            int val2;
            chooseMoveHelper(SOUTH, copy, h2, val2, depth-1, ac);
            if(val2 <= value) //if its the smallest we have ever seen
            {
                value = val2;
                bestHole = possibleHole;
            }
        }
    }
    return;
}

int SmartPlayer::chooseMove(const Board &b, Side s) const
{
    AlarmClock ac(3000); //set the alarm clock, testing revealed this as the best value as checkover is slow
    int hole = 0;
    int value = 0;
    chooseMoveHelper(s, b, hole, value, 5, ac); //call the helper function
    if(hole == 0) //if no move results in a win, hole will be unchanged
    {
        for(int i = 1; i <= b.holes(); i++) //choose a valid hole regardless
        {
            if(b.beans(s, i) != 0)
            {
                hole = i;
                break;
            }
        }
    }
    return hole;
}
