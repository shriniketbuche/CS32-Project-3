#include "Sequence.h"
#include <iostream>

Sequence::Sequence()
{
    head = nullptr; //set both pointers to null for empty sequence
    tail = nullptr;
    m_size = 0;
}
Sequence::~Sequence()
{
    while(m_size >0)
    {
        erase(0); //keep erasing until nothing is left, also works if sequence is empty
    }
}
Sequence::Sequence(const Sequence& aSequence):m_size(0)
{
    if(aSequence.m_size == 0) //if its empty
    {
        head = nullptr;
        tail = nullptr;
    }
    else if(aSequence.m_size == 1) //if there is a single one
    {
        Node* p = new Node;
        head = p;
        tail = p;
        p->val = aSequence.head->val;
    }
    else
    {
        Node* temp = aSequence.head;
        for(int i = 0; i < aSequence.m_size; i++)
        {
            if(temp != nullptr)
            {
                insert(i, temp->val); //put the work on the insert function, this will take care of head and tail.
                temp = temp->m_next;
            }
            else
            {
                break;
            }
        }
    }
}
Sequence& Sequence::operator=(const Sequence& aSequence)
{
    if(this != &aSequence) //basic copy constructor implementation
    {
        while(m_size >0)
        {
            erase(0); // erase all our sequence
        }
        Node* temp = aSequence.head;
        for(int i = 0; i < aSequence.m_size; i++)
        {
            if(temp != nullptr)
            {
                insert(i, temp->val); //reassign
                temp = temp->m_next;
            }
            else
            {
                break;
            }
        }
    }
    return *this;
}
bool Sequence::empty() const
{
    return(m_size == 0); //check for 0 size
}

int Sequence::size() const
{
    return m_size; //simple getter
}

int Sequence::insert(int pos, const ItemType &value)
{
    if(pos < 0) //error bounds checking
    {
        return -1;
    }
    else if(pos == 0 && m_size ==0) //create a new list with 1 element
    {
        Node *p = new Node;
        p->m_next = nullptr;
        p->m_previous = nullptr;
        p->val = value;
        head = p;
        tail = p;
        m_size++;
        return pos;
    }
    else if(pos == 0 && m_size > 0) //create a new head node
    {
        Node *p = new Node;
        p->m_next = head;
        head->m_previous = p;
        p->m_previous = nullptr;
        p->val = value;
        head = p;
        m_size++;
        return pos;
    }
    else if(pos == m_size && m_size >0) //create a new tail pointer
    {
        Node *p = new Node;
        tail->m_next = p;
        p->m_previous = tail;
        p->m_next = nullptr;
        p->val = value;
        tail = p;
        m_size++;
        return pos;
    }
    else //create a new node in the middle
    {
        Node *temp = head;
        Node *p = new Node;
        for(int i = 1; i < pos; i++)
        {
            temp = temp ->m_next;
        }
        p->m_next = temp->m_next;
        p->m_previous = temp->m_previous;
        Node *temp2 = temp->m_next;
        temp2->m_previous = p;
        temp->m_next = p;
        p->val = value;
        m_size++;
        return pos;
    }
}

int Sequence::insert(const ItemType &value)
{
    if(m_size == 0) //if its empty
    {
        return(insert(0, value));
    }
    int pos = 0;
    for(Node* loop = head; loop != nullptr; loop = loop -> m_next) //loop through until we hit a place where one value is less than the next
    {
        if(value >= loop->val)
        {
            pos++;
        }
        else
        {
            break;
        }
    }
    return(insert(pos, value));
}

bool Sequence::erase(int pos)
{
    if(pos < 0) //bounds checking
    {
        return false;
    }
    else if(m_size == 0) //can't erase something that is not there
    {
        return false;
    }
    else if(pos >= m_size) //bounds checking
    {
        return false;
    }
    else if(pos == 0 && m_size > 1) //getting rid of head
    {
        Node*temp = head->m_next;
        temp->m_previous = nullptr;
        delete head;
        head = temp;
        m_size--;
        
        return true;
    }
    else if(m_size ==1) //if only 1 element
    {
        delete head;
        head = nullptr;
        tail = nullptr;
        m_size--;
        return true;
    }
    else if(pos == m_size-1) //getting rid of tail
    {
        Node*temp = tail->m_previous;
        temp->m_next = nullptr;
        delete tail;
        tail = temp;
        m_size--;
        return true;
    }
    else //deleting something in the middle
    {
        Node* temp = head;
        for(int i= 0; i< pos; i++)
        {
            temp = temp->m_next;
        }
        Node*temp2 = temp->m_previous;
        Node*temp3 = temp->m_next;
        temp2->m_next = temp->m_next;
        temp3->m_previous = temp->m_previous;
        temp->m_previous = nullptr;
        temp->m_next= nullptr;
        delete temp;
        m_size--;
        return true;
    }
}
int Sequence::remove(const ItemType &value)
{
    int counter = 0; //keep track of a counter
    Node*loop = head;
    for(int i = 0; i < m_size; i++)
    {
        if(loop!= nullptr)
        {
            if(loop->val == value)
            {
                loop = loop->m_next;
                counter++;
                erase(i);
                i = 0; //reset the loop as the sequence has changed
            }
            else
            {
                loop = loop->m_next;
            }
        }
        else
        {
            break;
        }
    }
    return counter;
}
bool Sequence::get(int pos, ItemType &value) const
{
    if(pos <0) //bounds checking
    {
        return false;
    }
    else if(pos == 0) //get head
    {
        value = head->val;
        return true;
    }
    else if(pos == m_size-1) //get tail
    {
        value = tail->val;
        return true;
    }
    else if(pos >= m_size) //bounds checking
    {
        return false;
    }
    Node*temp = head;
    for(int i = 0; i < pos; i++)
    {
        temp = temp->m_next;
    }
    value = temp->val; //loop to the right position
    return true;
}
bool Sequence::set(int pos, const ItemType &value)
{
    if(pos <0) //bounds checking
    {
        return false;
    }
    else if(pos == 0) //set head
    {
        head->val = value;
        return true;
    }
    else if(pos == m_size-1) //set tail
    {
        tail->val = value;
        return true;
    }
    else if(pos >= m_size) //bounds checking
    {
        return false;
    }
    Node*temp = head;
    for(int i = 0; i < pos; i++)
    {
        temp = temp->m_next; // get to the right position
    }
    temp->val = value; //set it
    return true;
}
int Sequence::find(const ItemType &value) const
{
    if(m_size == 0) //can't find something if its empty
    {
        return -1;
    }
    int pos = -1; //set it -1 to return -1 if it can't be found
    Node * temp = head;
    for(int i = 0; i < m_size; i++)
    {
        if(temp != nullptr)
        {
            if(temp->val == value)
            {
                pos = i; //loop till found
                break;
            }
            else
            {
                temp = temp->m_next;
            }
        }
        else
        {
            break;
        }
    }
    if(pos == -1)
    {
        return -1;
    }
    else
    {
        return pos;
    }
}
void Sequence::swap(Sequence &other)
{   //simple swapper
    Node* tempHead = head;
    Node* tempTail = tail;
    int tempSize = m_size;
    head = other.head;
    tail = other.tail;
    other.head = tempHead;
    other.tail = tempTail;
    m_size = other.m_size;
    other.m_size = tempSize;
}
void Sequence::dump() const
{
    for(Node *p = head; p != nullptr; p = p->m_next)
    {
        std::cerr << p->val << std::endl; //just cerr the values
    }
}

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
    if(seq2.size() > seq1.size()) //seq2 can't be bigger than seq1
    {
        return -1;
    }
    else if(seq2.empty()) //also can't be empty
    {
        return -1;
    }
    else if(seq1.empty())// same thing
    {
        return -1;
    }
    else
    {
        bool isinSequence = false;
        ItemType value1;
        ItemType value2;
        int answer = -1;
        for(int i = 0; i < seq1.size(); i++)
        {
            seq1.get(i, value1);
            seq2.get(0, value2);
            if(value1 == value2)
            {
                isinSequence = true;
                answer = i; //put answer as this and loop, if it isn't true, answer will change value back to -1
                int seq1index = i;
                int seq2index = 0;
                while(isinSequence)
                {
                    if((seq1index < seq1.size())&&(seq2index < seq2.size()))
                    {
                        seq1.get(seq1index, value1);
                        seq2.get(seq2index, value2);
                        if(value1 != value2)
                        {
                            isinSequence = false;
                        }
                        else
                        {
                            seq1index++;
                            seq2index++;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            if(isinSequence == true)
            {
                break;
            }
        }
        if(isinSequence != true)
        {
            answer = -1;
        }
        return answer;
    }
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
    if((&seq1 == &result) && (&seq1 == &seq2)) //if they're all the same
    {
        Sequence s = seq1;
        Sequence a = seq2;
        interleave(s, a, result);
        return;
    }
    if((&seq1 == &result) && (&seq1 != &seq2)) //if two are the same
    {
        Sequence s = seq1;
        interleave(s, seq2, result);
        return;
    }
    if((&seq2 == &result) && (&seq2 != &seq1)) //if two are the same
    {
        Sequence s = seq2;
        interleave(seq1, s, result);
        return;
    }
    while(result.size() > 0) //get rid of stuff in result
    {
        result.erase(0);
    }
    if(seq1.empty() && seq2.empty()) //if they're both empty so is result
    {
        return;
    }
    if(seq1.empty() && !seq2.empty()) //if one is empty return the other
    {
        result = seq2;
        return;
    }
    if(seq2.empty() && !seq1.empty()) //same
    {
        result = seq1;
        return;
    }
    ItemType value1;
    ItemType value2;
    if(seq1.size() == seq2.size()) //if they are of the same size
    {
        int k = 0;
        for(int i = 0; i < seq1.size(); i++)
        {
            seq1.get(i, value1);
            seq2.get(i, value2);
            result.insert(k, value1);
            result.insert(k+1, value2);
            k+=2;
        }
    }
    else if(seq1.size() < seq2.size()) //if one is less than the other...
    {
        int k = 0;
        for(int i = 0; i < seq1.size(); i++)
        {
            seq1.get(i, value1);
            seq2.get(i, value2);
            result.insert(k, value1);
            result.insert(k+1, value2);
            k+=2;
        }
        for(int j = seq1.size(); j < seq2.size(); j++) //fill result in with the remaining values
        {
            seq2.get(j, value2);
            result.insert(seq1.size() + j, value2);
        }
    }
    else if(seq2.size() < seq1.size()) //same thing
    {
        int k = 0;
        for(int i = 0; i < seq2.size(); i++)
        {
            seq1.get(i, value1);
            seq2.get(i, value2);
            result.insert(k, value1);
            result.insert(k+1, value2);
            k+=2;
        }
        for(int j = seq2.size(); j < seq1.size(); j++)
        {
            seq1.get(j, value2);
            result.insert(seq2.size() + j, value2);
        }
    }
}
