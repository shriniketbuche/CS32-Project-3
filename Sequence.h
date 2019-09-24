#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <string>

using ItemType = std::string;


class Sequence
{
public:
    Sequence();
    ~Sequence();
    Sequence(const Sequence& aSequence);
    Sequence& operator=(const Sequence& aSequence);
    bool empty() const;
    int size() const;
    int insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);
    void dump() const;
private:
    struct Node
    {
        ItemType val;
        Node* m_next;
        Node* m_previous;
    };
    Node* head;
    Node* tail;
    int m_size;
};

int subsequence(const Sequence& seq1, const Sequence& seq2);
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);



#endif
