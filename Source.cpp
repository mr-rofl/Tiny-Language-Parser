#include "iostream"
#include "fstream"
#include "string"
#include <map>
#include <vector>
#include <stdlib.h>
#include "source.h"

using namespace std;

Node::Node(){
		value="";type="";
        isdashed=false;
		sibiling = nullptr;
	}
Node::Node(string val, string typ){
		value=val;type=typ;
		sibiling = nullptr;
	}
void Node::add_children(Node *child){
		childern.push_back(*child);
	}
void Node::set_sibiling(Node *s){
		sibiling =  s;
	}
void Node::set_data(string val, string typ){
		value=val;type=typ;
		sibiling = nullptr;
	}
string Node::get_value(){return value;}
string Node::get_type(){return type;}
vector<Node> Node::get_children(){return childern;}
Node *Node::get_sibiling(){return sibiling;}
void Node:: clear_children(){childern.clear();}

vector<vector<string>> result(0,vector<string>(2));
int cnt;
string script="graph D { \n";
int ids=0;
vector<Node> Nodes;


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


bool stmt_seq(Node* parent){
//Node temp;
if(!statement(parent)){
    cout<<"ERROR\n";
    return false;
}
bool f=true;
while(cnt<result.size()){
    if (result[cnt][1]=="SEMICOLON")
    {
        Node *N = new Node();
        cnt++;if (cnt>=result.size())return true;
        f=statement(N);
        if(!f){
            cout<<"ERROR\n";
            return false;
        }
        parent->set_sibiling(N);
        parent=N;
    }
    else
    {
        return true;
    }

}


if(f)return true;
else return false;
}

bool statement(Node* parent){
Node N;
int temp=cnt;
if (result[cnt][1]=="IF")
{
    parent->set_data(result[temp][0],"square");
    cnt++;if (cnt>=result.size())return false;
    if(if_stmt(parent)){
        return true;
    }
    return false;

}
else if (result[cnt][1]=="REPEAT")
{
    parent->set_data(result[temp][0],"square");
    cnt++;if (cnt>=result.size())return false;
    if(repeat_stmt(parent))
    {
        return true;
    }
    return false;
}
else if (result[cnt][1]=="READ")
{
    cnt++;if (cnt>=result.size())return false;
    if(read_stmt(&N))
    {
        parent->set_data(result[temp][0]+"\n("+N.get_value()+")","square");
        return true;
    }
    return false;
}
else if (result[cnt][1]=="WRITE")
{
    cnt++;if (cnt>=result.size())return false;
    if(write_stmt(parent))
    {
        parent->set_data(result[temp][0],"square");
        //(*parent).add_children(&N);
        return true;
    }
    return false;
}
else if (result[cnt][1]=="IDENTIFIER")
{
    cnt++;if (cnt>=result.size())return false;
    if(assign_stmt(&N))
    {
        parent->set_data("Assign\n" + result[temp][0],"square");
        (*parent).add_children(&N);
        return true;
    }
    return false;
}
else
{
    cout<<"ERROR\n";
    return false;
}

}

bool if_stmt(Node* parent){
Node N;
if(!exp(&N)){
    cout<<"ERROR\n";
    return false;
}

parent->add_children(&N);

if (result[cnt][1]!="THEN"){
    cout<<"ERROR\n";
    return false;
}
cnt++;if (cnt>=result.size())return false;
Node p;
if(!stmt_seq(&p)){

    cout<<"ERROR\n";
    return false;
}
parent->add_children(&p);
if (result[cnt][1]=="ELSE")
{
    Node N2;
    cnt++;if (cnt>=result.size())return false;
    if(!stmt_seq(&N2)){
        cout<<"ERROR\n";
        return false;
    }
    N.add_children(&N2);
    parent->add_children(&N2);
}

if (result[cnt][1]=="END")
{
    cnt++;if (cnt>=result.size())return true;
    return true;
}
return false;
}

bool repeat_stmt(Node* parent){
Node N;
if (!stmt_seq(&N))
{
    cout<<"ERROR\n";
    return false;
}
parent->add_children(&N);

if (result[cnt][1]!="UNTIL"){
    cout<<"ERROR\n";
    return false;
}
cnt++;if (cnt>=result.size())return true;
Node p;
if (!exp(&p))
{
    return false;
}
(*parent).add_children(&p);
return true;
}

bool assign_stmt(Node* parent){
if (result[cnt][1]!="ASSIGN"){
    cout<<"ERROR\n";
    return false;
}
cnt++;if (cnt>=result.size())return true;
if (!exp(parent))
{
    return false;
}
return true;
}
bool read_stmt(Node* parent){
if (result[cnt][1]!="IDENTIFIER"){
    cout<<"ERROR\n";
    return false;
}
parent->set_data(result[cnt][0],"");
cnt++;if (cnt>=result.size())return true;
return true;
}
bool write_stmt(Node* parent){
Node N;
if (!exp(&N)){
    cout<<"ERROR\n";
    return false;
}
(*parent).add_children(&N);
return true;
}
bool exp(Node* parent){
Node N;
if(!simple_exp(parent)){
    cout<<"ERROR\n";
    return false;
}
if (cnt>=result.size())return true;
if (result[cnt][1]=="LESSTHAN"||result[cnt][1]=="EQUAL"){
    N=*parent;
    parent->set_data("op\n"+result[cnt][0],"circle");
    parent->clear_children();
    parent->add_children(&N);
    cnt++;if (cnt>=result.size())return false;
    N.clear_children();
    if(!simple_exp(&N)){
        cout<<"ERROR\n";
        return false;
    }
    parent->add_children(&N);
    if (cnt>=result.size())return true;
}
return true;
}

bool simple_exp(Node* parent){
Node N;
if(!term(parent)){
    cout<<"ERROR\n";
    return false;
}
if (cnt>=result.size())return true;
while(result[cnt][1]=="ADD"||result[cnt][1]=="MINUS"){
    N= *parent;
    parent->set_data("op\n"+result[cnt][0],"circle");
    parent->clear_children();
    parent->add_children(&N);
    cnt++;if (cnt>=result.size())return false;
    N.clear_children();
    if(!term(&N)){
        cout<<"ERROR\n";
        return false;
    }
    parent->add_children(&N);
    if (cnt>=result.size())return true;
}
return true;
}
bool term(Node* parent){
Node N;
if(!factor(parent)){
    cout<<"ERROR\n";
    return false;
}
if (cnt>=result.size())return true;
while(result[cnt][1]=="MULT"||result[cnt][1]=="DIV")
{
    N = *parent;
    parent->set_data("op\n"+result[cnt][0],"circle");
    parent->clear_children();
    parent->add_children(&N);
    cnt++;if (cnt>=result.size())return false;
    N.clear_children();
    if(!factor(&N)){
        cout<<"ERROR\n";
        return false;
    }
    parent->add_children(&N);
    if (cnt>=result.size())return true;
}
return true;
}

bool factor(Node* parent){
Node N;
if (result[cnt][1]=="IDENTIFIER")
{
    parent->set_data("id\n"+result[cnt][0],"circle");
    cnt++;if (cnt>=result.size())return true;
    return true;
}
if (result[cnt][1]=="NUMBER")
{
    parent->set_data("no\n"+result[cnt][0],"circle");
    cnt++;if (cnt>=result.size())return true;
    return true;
}
if (result[cnt][1]=="OPENBRACKET")
{
    cnt++;if (cnt>=result.size())return false;
    if (!exp(parent))
    {
        cout<<"ERROR\n";
        return false;
    }
    if (cnt>=result.size())return false;
    if (result[cnt][1]=="CLOSEBRACKET")
    {
        cnt++;if (cnt>=result.size())return true;
        return true;
    }
    return false;
}
return false;
}

int traverse(Node n ){
	vector<Node> c= n.get_children();
	script= script + to_string(ids)+ "[ shape = "+n.get_type()+" label = \" " + n.get_value() + "\" ] \n";
	int myid=ids;
	ids++;
	//script+=n.get_value();

	for(auto i =0;i<c.size();i++){
		int t=traverse(c[i]);
		script = script + to_string(myid)+ "--" +to_string(t)+"\n";
	}

	Node *s=n.get_sibiling();
	//cout<<s->get_value();
	if (s!=nullptr)
	{
		int t=traverse(*s);
		script = script + "{ rank=same "+ to_string(myid) + " " + to_string(t)+"}\n";
		script = script + to_string(myid)+ "--" +to_string(t)+"\n";
	}
	return myid;
}

void write(string text,string path){
      ofstream myfile;
      myfile.open (path);
      myfile << text;
      myfile.close();
}

bool myinit(string path ,string *error){
	cnt = 0;
    if(path.substr(path.size()-3 ) != "txt"){
        *error = "File Not Supported";
        return false;
    }
	ifstream tokens(path);
	vector<string> tempv(2);
	char c;
	state s=VALUE;
	if(tokens.is_open()){
		while(tokens.get(c))
		{
			switch (s)
			{
			case VALUE:
				if (c!=',' && c!=' ')
				{
					tempv[0]+=c;
				}
				else
				{
					s=TYPE;
				}
				
				break;
			case TYPE:
				if (c!='\n' && c!=' ' && c !=',')
				{
					tempv[1]+=c;
					if (tokens.peek() == EOF)
					{
						s=VALUE;
						result.push_back(tempv);
					}
				}
				else if(!tempv[1].empty() )
				{
					s=VALUE;
					result.push_back(tempv);
					tempv[0]="", tempv[1]="";
				}
				
				break;
			default:
				break;
			}

		}
	}
    else {
        *error = "File Not Found";
        return false;
    }
    tokens.close();
    if(result[0][1].empty()){
        *error = "Syntax Error";
        cnt=0;
        result.clear();
        return false;
    }
    Node n;
    if(!stmt_seq(&n)){
        *error = "Syntax Error";
        cnt=0;
        result.clear();
        return false;
    }
    traverse(n);
    ofstream myfile;
     myfile.open ("example.gv");
     myfile << script<<"}";
     myfile.close();
     script="graph D { \n";

     ids=0;
     system("dot -Tjpg example.gv -o graph1.jpg\n");
     system("graph1.jpg\n");

     cnt=0;
     result.clear();

    return true;
}
