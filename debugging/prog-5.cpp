#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct node
{
    node * prev;
    node * next;
    std::string data;

    node(node * p, node * n, const std::string& s)
        :prev(p), next(n), data(s)
    {
    }
};

struct list
{
    node * head;
    node * tail;
    size_t n;
    
    list()
        :head(0), tail(0), n(0)
    {
    }

    ~list()
    {
        for(node * n = head; n; n = n->next)
            delete n;
    }

    size_t size() const
    {
        return n;
    }

    node * append(const std::string& str)
    {
        if(tail)
        {
            tail = tail->next = new node(tail, 0, str);
        }
        else
        {
            tail = head = new node(0, 0, str);
        }
        return tail;
    }

    node * add_after(node* n, const std::string& str)
    {
        // test if there is a node 
        if (!n) {
            return nullptr;
        }

        // If n is the tail, create a new node and update the tail.
        if (n == tail) {
            tail = tail->next = new node(n, nullptr, str);
        } 
        else {
            // Otherwise, create a new node and update the links.
            node * a = new node(n, n->next, str);
            n->next->prev = a;
            n->next = a;
        }

        // Return the newly added node.
        return n->next;
    }

    void remove(node* n)
    {
        // tests if there is a node 
        if (!n) {
            return;
        }

        // test if n isn't a head
        if (n->prev) {
            n->prev->next = n->next;
        } else {
            // update the head pointer.
            head = n->next;
        }

        // test if n isn't a tail
        if (n->next) {
            n->next->prev = n->prev;
        } else {
            // update the tail pointer.
            tail = n->prev;
        }

        // Delete the node n pointer
        // delete n;
    }

    void print()
    {
        if (!head) {
        return;
        }

        for(node * n = head; n; n = n->next)
            std::cout << n->data << std::endl;
    }
};

int main()
{
    list L;

    node * a = L.append("A");
    node * c = L.append("C");
    node * e = L.append("E");
    L.print();

    node * b = L.add_after(a, "B");
    node * d = L.add_after(c, "D");
    node * f = L.add_after(e, "F");
    L.print();

    L.remove(a);
    L.remove(d);
    
    L.print();

    return 0;
}

