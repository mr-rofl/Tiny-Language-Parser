#ifndef SOURCE_H
#define SOURCE_H

#include "iostream"
#include "fstream"
#include "string"
#include <map>
#include <vector>
#include <stdlib.h>

using namespace std;
enum state{VALUE,TYPE};

class Node{
private:
    Node *sibiling;
    vector<Node> childern;
    string value,type;
    bool isdashed;
public:
    Node();
    Node(string val, string typ);
    void add_children(Node *child);
    void set_sibiling(Node *s);
    void set_data(string val, string typ);
    string get_value();
    string get_type();
    vector<Node> get_children();
    Node *get_sibiling();
    void clear_children();
};
bool stmt_seq(Node* parent);
bool statement(Node* parent);
bool if_stmt(Node* parent);
bool repeat_stmt(Node* parent);
bool assign_stmt(Node* parent);
bool read_stmt(Node* parent);
bool write_stmt(Node* parent);
bool exp(Node* parent);
bool simple_exp(Node* parent);
bool term(Node* parent);
bool factor(Node* parent);



int traverse(Node n );


bool myinit(string path, string *error);
void write(string text,string path);



#endif // SOURCE_H
