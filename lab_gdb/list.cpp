/**
 * @file list.cpp
 * Singly Linked List (lab_gdb).
 *
 * @author Chase Geigle
 * @date (created) Fall 2011
 * @date (modified) Spring 2012, Fall 2012
 *
 * @author Jack Toole
 * @date (modified) Fall 2011
 *
 * @author Veer Dedhia
 * @date (modified) Spring 2014
 */

/**
 * Destroys the current List. This function should ensure that
 * memory does not leak on destruction of a list.
 */
template <class T>
List<T>::~List() {
    clear();
}

/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <class T>
void List<T>::clear() {
    if (head != NULL) {
        ListNode* prev = NULL;
        ListNode* curr = head;

        while(curr->next != NULL) {
            prev = curr;
            curr = curr->next;
            delete prev;
        }
        delete curr;
        head = NULL;
    }
    length = 0;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <class T>
void List<T>::insertFront(T const& ndata) {
    // @todo Graded in lab_gdb

    ListNode* node = new ListNode(ndata);
    node->next = head;
    if (length == 0) {
        head = node;
    } else {
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
    // @todo Graded in lab_gdb
    // NOTE: Do not use this implementation for MP3!
    ListNode* node = new ListNode(ndata);
    ListNode* curr = head;
    if (curr == NULL) {
        head = node;
    } else {
        while (curr->next != NULL)
            curr = curr->next;
        curr->next = node;
    }
    length++;
}

/**
 * Reverses the current List.
 */
template <class T>
void List<T>::reverse() {
    if (length != 0) {
        head = reverse(head, NULL, 0);
    }
}

/**
 * Helper function to recursively reverse the enitre sequence of
 * linked memory inside a List.
 *
 * @param curr The current node we are reversing
 * @param prev The node that should be placed before the current node in the
 * final reversed list
 * @param len The length of the remaining list to be reversed
 */
template <class T>
typename List<T>::ListNode* List<T>::reverse(ListNode* curr, ListNode* prev, int len) {
    // @todo Graded in lab_gdb
    if (curr->next != NULL) {
        ListNode* node = reverse(curr->next, curr, 0);
        curr->next = prev;
        return node;
    } else {
        curr->next = prev;
        return curr;
    }
}

/**
 * Shuffles the current list by applying a perfect shuffle once. This entails
 * splitting the list in half, then interleaving the two halves.
 * For example:
 * start : < 1, 2, 3, 4, 5 >
 * split : < 1, 2, 3 >    < 4, 5 >
 * final : < 1, 4, 2, 5, 3 >
 */
template <class T>
void List<T>::shuffle() {

    // @todo Graded in lab_gdb

    // one should point at the start of the first half-list
    // two should point at the start of the second half-list
    if (length != 0) {
        ListNode *one, *two, *prev, *temp;
        one = two = prev = head;


        for (int i = 0; i < length / 2 + length % 2; i++) {
            prev = two;
            two = two->next;
        }
        prev->next = NULL;

        // interleave
        while (two != NULL) {
            temp = one->next;
            one->next = two;
            two = two->next;
            one->next->next = temp;
            one = one->next->next;
        }
    }
}
