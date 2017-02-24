#include <iostream>
#include <string>

using namespace std;

/*************************************************************************
* This is the list class which is used for organizing 
* the list of student names.
* This is a singly linked list.
* 
* ListNode class is the structure of ever node in 
* the linked list. 
*
* next is the pointer to the next node of the list
* name is the name of every student
* netid is the unique netid of every student
*
**************************************************************************/

class List
{
	private:
	class ListNode
	{
		public:

		//Constructs list node with given data
		ListNode(std::string a_name,int a_numberTicket);
		
		ListNode* next;
        std::string m_sName;
		int m_nNumberTicket;
        
        //Returns NumberTicket value
        int getNumberTicket();

	};

	ListNode* head;
    ListNode* tail;
	int length;

	public:

	// Default constructor 
	// Empty List with head pointing to NULL
	List();

    //Prints list
    void print();
    
	// Return length
	int getLength();

	// Inserts a new node at the front of the list. 
	void insertBack(std::string a_name, int a_numberTicket);

	// Reorganizes the list of student names according
	// to given pattern in question
	void reorganizeList();
};

//Constructor for list node
List::ListNode::ListNode(std::string a_name, int a_numberTicket)
{
    m_sName = a_name;
    m_nNumberTicket = a_numberTicket;
    next = NULL;
}

//Default constructor for list
List::List()
{
    length = 0;
    head = NULL;
    tail = NULL;
}

//Return length
int List::getLength()
{
    return length;
}

//Return number ticket
int List::ListNode::getNumberTicket()
{
    return m_nNumberTicket;
}

//Insert Back
void List::insertBack(std::string a_name, int a_numberTicket)
{
    ListNode * temp = new ListNode(a_name, a_numberTicket);
    if(head == NULL)
    {
        head = temp;
        tail = temp;
    }
    
    else
    {
        tail->next = temp;
        tail = tail->next;
    }
    
    length ++;
}

//Reorganize the list
//Example (displaying ticket values instead of names for clarity)
// 0 -> 1 -> 0 -> 0 -> 1 -> 1 -> NULL becomes
// 0 -> 0 -> 0 -> 1 -> 1 -> 1 -> NULL
void List::reorganizeList() {
    if (length > 1) {
        ListNode * prev = NULL;
        ListNode * iter = head;
        ListNode * next;
        
        for (int i = 0; i < length; i++) {
            next = iter->next;
            if (iter->getNumberTicket() == 1) {
                
                if (iter == head) {
                    head = next;
                }
                
                if (prev != NULL) {
                    prev->next = next;
                }
                tail->next = iter;
                iter->next = NULL;
                tail = iter;    
                
            }
            prev = iter;
            iter = next;
        }
    }
}

//Use this function to display the list
void List::print()
{
    cout<<"----------------Printing List----------"<<endl;
    ListNode * curr = head;
    
    while(curr != tail)
    {
        cout<<curr->m_sName<<" -> ";
        curr = curr->next;
    }
    
    cout<<curr->m_sName<<endl;
    cout<<"--------------------------------------"<<endl;
}

int main()
{
    List studentNames;
    studentNames.insertBack("a", 0);
    studentNames.insertBack("b", 1);
    studentNames.insertBack("c", 1);
    studentNames.insertBack("d", 0);
    
    studentNames.print();
    
    studentNames.reorganizeList();
    
    //Expected output after reorganize = a -> d -> b -> c
    
    studentNames.print();
}
