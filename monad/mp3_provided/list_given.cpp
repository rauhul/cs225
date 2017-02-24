/**
 * Doubly Linked List (MP 3)
 * Created by Chase Geigle for CS225, Fall 2011
 * Modified by Jack Toole, Fall 2011
 */

// These are given functions---you do not need to (and should not!) modify them

/**
 * List() -- Default Constructor (GIVEN)
 *  - Creates the empty list, defined to have null head and tail pointers and no
 *    sentinels.
 */
template <class T>
List<T>::List()
	: head(NULL), tail(NULL), length(0)
{ /* nothing */ }



template <class T>
int List<T>::size() const
{
	return length;
}



/**
 * List() -- Copy Constructor (GIVEN)
 *  - Creates a copy of the parameter list
 */
template <class T>
List<T>::List(List<T> const & other)
{
	copy(other);
}



/**
 * operator= -- (GIVEN)
 *  - Assignment operator for Lists
 */
template <class T>
List<T> & List<T>::operator=(List<T> const & rhs)
{
	if (this != &rhs)
	{
		clear();
		copy(rhs);
	}
	return *this;
}



/**
 * empty() -- (GIVEN)
 *  - Returns a boolean value: whether or not the list is empty
 */
template <class T>
bool List<T>::empty() const {
	return head == NULL && tail == NULL && length == 0;
}



/**
 * copy() -- Copy helper (GIVEN)
 *  - Copies the parameter list into the current list
 */
template <class T>
void List<T>::copy(List<T> const & other) {
	// set up the default, empty list
	head = NULL;
	tail = NULL;
	length = 0;

	// if we have things to copy
	if (!other.empty())  {
		ListNode * curr = other.head;
		ListNode * prev = NULL;
		// iterate down the parameter list
		while (curr != NULL)  {
			ListNode * node = new ListNode(curr->data);

			// set the head of the new list
			if (head == NULL) 
				head = node;

			// correct pointer of the previous node if it exists
			if (prev != NULL) 
				prev->next = node;
			
			node->prev = prev;

			prev = node;
			curr = curr->next;
		}

		// prev will contain our new tail---set it up accordingly
		tail = prev;
		tail->next = NULL;

		length = other.length;
	}
}



/**
 * print -- Printing Operator (GIVEN)
 *  - Used for printing the list
 */
template <class T>
void List<T>::print(ostream & os) const
{
	os << "<";
	ListNode * curr = head;
	while (curr != NULL) {
		os << " " << curr->data;
		curr = curr->next;
	}
	os << " >";
}



// overloaded operator<<
template <class T>
ostream & operator<<(ostream & os, const List<T> & list)
{
	list.print(os);
	return os;
}



// ListNode constructors
template <class T>
List<T>::ListNode::ListNode()
	: next(NULL), prev(NULL), data(T())
{ /* nothing */ }



template <class T>
List<T>::ListNode::ListNode(const T & ndata)
	: next(NULL), prev(NULL), data(ndata)
{ /* nothing */ }



template <class T>
template <class Iter>
List<T>::List(const Iter & begin_iterator, const Iter & end_iterator)
	: head(NULL), tail(NULL), length(0)
{
	for (Iter i = begin_iterator; i != end_iterator; ++i)
		insertBack(*i);
}

