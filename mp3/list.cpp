/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 *
 * @author Chase Geigle
 * @date (created) Fall 2011
 * @date (modified) Spring 2012, Fall 2012
 *
 * @author Jack Toole
 * @date (modified) Fall 2011
 */

/**
 * Destroys the current List. This function should ensure that
 * memory does not leak on destruction of a list.
 */
template <class T>
List<T>::~List()
{
    /// @todo Graded in MP3.1
    clear();
}

/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <class T>
void List<T>::clear() {
	/// @todo Graded in MP3.1
	if (head != NULL) {
        ListNode* prev = NULL;
        ListNode* curr = head;

        while(curr->next != NULL) {
            prev = curr;
            curr = curr->next;
            delete prev;
        }
        delete curr;
    }
    length = 0;
    head = NULL;
    tail = NULL;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <class T>
void List<T>::insertFront(T const& ndata) {
    /// @todo Graded in MP3.1
    ListNode* node = new ListNode(ndata);
    node->prev = NULL;
    node->next = head;
    if (length == 0) {
        head = node;
        tail = node;
    } else {
        head->prev = node;
        head = node;
    }
    
    length++;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <class T>
void List<T>::insertBack(const T& ndata) {
    /// @todo Graded in MP3.1
    ListNode* node = new ListNode(ndata);
    node->next = NULL;
    node->prev = tail;
    if (length == 0) {
        head = node;
        tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    
    length++;
}

/**
 * Reverses the current List.
 */
template <class T>
void List<T>::reverse() {
    reverse(head, tail);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <class T>
void List<T>::reverse(ListNode*& startPoint, ListNode*& endPoint) {
	if (startPoint != endPoint) {
		ListNode * curr  = startPoint;
		ListNode * post  = endPoint->next, * pre = startPoint->prev;
		ListNode * start = startPoint,     * end = endPoint;
	  
		while (curr != post) {
			swap(curr->prev, curr->next);
			curr = curr->prev;
		}

		if (pre == NULL) {
			end->prev = NULL;
			head = end;
		} else {
			pre->next = end;
			end->prev = pre;
		}

		if (post == NULL) {
			start->next = NULL;
			tail = start;
		} else {
			post->prev = start;
			start->next = post;
		}
		startPoint = end;
		endPoint = start;
	}
}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <class T>
void List<T>::reverseNth(int n) {
    /// @todo Graded in MP3.1
    if (n > 1 && length != 0) {
        ListNode* start = head;
        ListNode* end   = head;
        while(start != NULL) {
            for(int i = 1; i < n; i++)
                if (end->next != NULL)
                    end = end->next;
            reverse(start, end);
            start = end = end->next;
        }
    }
}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <class T>
void List<T>::waterfall() {
	/// @todo Graded in MP3.1
	if (length > 1) {
		ListNode* curr = head->next;
		ListNode* next = curr->next;
		while (curr != NULL) {
			next = curr->next;
			moveToTail(curr);

			if (next != NULL && next->next != NULL) {
				curr = next->next;
			} else {
				return;
			}
		}
	}
}

template <class T>
void List<T>::moveToTail(ListNode*& node) {
	if (node != tail) {
		if (node->prev != NULL) {
			node->prev->next = node->next;
		}
		if (node->next != NULL) {
			node->next->prev = node->prev;
		}

		tail->next = node;
		node->prev = tail;
		node->next = NULL;
		tail = node;
	}
}
/**
 * Splits the given list into two parts by dividing it at the splitPoint.
 *
 * @param splitPoint Point at which the list should be split into two.
 * @return The second list created from the split.
 */
template <class T>
List<T> List<T>::split(int splitPoint) {
    if (splitPoint > length)
        return List<T>();

    if (splitPoint < 0)
        splitPoint = 0;

    ListNode* secondHead = split(head, splitPoint);

    int oldLength = length;
    if (secondHead == head) {
        // current list is going to be empty
        head = NULL;
        tail = NULL;
        length = 0;
    } else {
        // set up current list
        tail = head;
        while (tail->next != NULL)
            tail = tail->next;
        length = splitPoint;
    }

    // set up the returned list
    List<T> ret;
    ret.head = secondHead;
    ret.tail = secondHead;
    if (ret.tail != NULL) {
        while (ret.tail->next != NULL)
            ret.tail = ret.tail->next;
    }
    ret.length = oldLength - splitPoint;
    return ret;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List objects!
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <class T>
typename List<T>::ListNode* List<T>::split(ListNode* start, int splitPoint) {
	/// @todo Graded in MP3.2

	ListNode* aStart = NULL, * bStart = NULL;

	while (splitPoint > 0) {
		if (aStart == NULL) {
			aStart = start;
		} else {
			aStart = aStart->next;
		}
		splitPoint--;
	}

	if (aStart == NULL) {
		return start;
	} else {
		bStart = aStart->next;
		if (bStart != NULL) {
			bStart->prev = NULL;
			aStart->next = NULL;
		}
		return bStart;
	}
}

/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <class T>
void List<T>::mergeWith(List<T>& otherList) {
    // set up the current list
    head = merge(head, otherList.head);
    tail = head;

    // make sure there is a node in the new list
    if (tail != NULL) {
        while (tail->next != NULL)
            tail = tail->next;
    }
    length = length + otherList.length;

    // empty out the parameter list
    otherList.head = NULL;
    otherList.tail = NULL;
    otherList.length = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */

template <class T>
typename List<T>::ListNode* List<T>::merge(ListNode* first, ListNode* second) {
	/// @todo Graded in MP3.2
	ListNode* head = NULL,
			* tail = NULL,
			* firstNext,
			* secondNext;

	while(first != NULL || second != NULL) {
		firstNext  = first  != NULL ? first->next  : NULL;
		secondNext = second != NULL ? second->next : NULL;

		if (first != NULL && second != NULL) {
			if (first->data < second->data ) {
				appendNodeTo(head, first, tail);
				first = firstNext;
			} else {
				appendNodeTo(head, second, tail);
				second = secondNext;
			}
		} else {
			if (first != NULL) {
				appendNodeTo(head, first, tail);
				first = firstNext;
			} else {
				appendNodeTo(head, second, tail);
				second = secondNext;
			}
		}
	}
    return head;
}

template <class T>
void List<T>::appendNodeTo(ListNode*& head, ListNode*& node, ListNode*& tail) {
	if (node != NULL) {
		if (head == NULL) {
			node->next = NULL;
			node->prev = NULL;
			head = tail = node;
		} else {
			node->next = NULL;
			node->prev = tail;
			tail->next = node;
			tail = node;
		}
	}
}


/**
 * Sorts the current list by applying the Mergesort algorithm.
 */
template <class T>
void List<T>::sort() {
    if (empty())
        return;
    head = mergesort(head, length);
    tail = head;
    while (tail->next != NULL)
        tail = tail->next;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <class T>
typename List<T>::ListNode* List<T>::mergesort(ListNode* start, int chainLength) {
	/// @todo Graded in MP3.2
	if (chainLength == 1 || start == NULL) {
		return start;
	} else {
		ListNode* headB = split(start, chainLength/2);
		headB = mergesort(headB, chainLength/2 + chainLength%2);
		start = mergesort(start, chainLength/2);
		return merge(start, headB);
	}
}
