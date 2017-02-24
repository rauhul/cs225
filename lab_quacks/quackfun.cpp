/**
 * @file quackfun.cpp
 * This is where you will implement the required functions for the
 *  stacks and queues portion of the lab.
 */

namespace QuackFun {

/**
 * Sums items in a stack.
 * @param s A stack holding values to sum.
 * @return The sum of all the elements in the stack, leaving the original
 *  stack in the same state (unchanged).
 *
 * @note You may modify the stack as long as you restore it to its original
 *  values.
 * @note You may use only two local variables of type T in your function.
 *  Note that this function is templatized on the stack's type, so stacks of
 *  objects overloading the + operator can be summed.
 * @note We are using the Standard Template Library (STL) stack in this
 *  problem. Its pop function works a bit differently from the stack we
 *  built. Try searching for "stl stack" to learn how to use it.
 * @hint Think recursively!
 */
template <typename T>
T sum(stack<T>& s) {
    if (s.size() == 0 ) {
        return T();
    } else {
        T temp1 = s.top(), total = s.top();
	s.pop();
	total += sum(s);
        s.push(temp1);
	return total;
    }
}

/**
 * Reverses even sized blocks of items in the queue. Blocks start at size
 * one and increase for each subsequent block.
 * @param q A queue of items to be scrambled
 *
 * @note Any "leftover" numbers should be handled as if their block was
 *  complete.
 * @note We are using the Standard Template Library (STL) queue in this
 *  problem. Its pop function works a bit differently from the stack we
 *  built. Try searching for "stl stack" to learn how to use it.
 * @hint You'll want to make a local stack variable.
 */
template <typename T>
void scramble(queue<T>& q) {
    stack<T> s;
    queue<T> q2;

    queue<T> output;

    int counter = 1;
    int length  = 1;

    T temp;
    while (q.size() > 0) {
	temp = q.front();
	q.pop();
	counter--;
	if (length%2 == 0) {
            s.push(temp);
        } else {
            q2.push(temp);    
        }
	if (counter == 0 || q.size() == 0) {
	    if (length%2 == 0) {
		while (s.size() > 0) {
		    output.push(s.top());
		    s.pop();
		}
	    } else {
                while (q2.size() > 0) {
                    output.push(q2.front());
                    q2.pop();
                }
            }
	    length++;
            counter = length;
	}
    }
    while (output.size() >0) {
        q.push(output.front());
        output.pop();
    }
}

/**
 * @return true if the parameter stack and queue contain only elements of
 *  exactly the same values in exactly the same order; false, otherwise.
 *
 * @note You may assume the stack and queue contain the same number of items!
 * @note There are restrictions for writing this function.
 * - Your function may not use any loops
 * - In your function you may only declare ONE local boolean variable to use in
 *   your return statement, and you may only declare TWO local variables of
 *   parametrized type T to use however you wish.
 * - No other local variables can be used.
 * - After execution of verifySame, the stack and queue must be unchanged. Be
 *   sure to comment your code VERY well.
 */
template <typename T>
bool verifySame(stack<T>& s, queue<T>& q) {
    if (s.size() == 0) {
        return true;
    } else {
        T stackValue = s.top();
	T queueValue;
        s.pop();

	bool pass = verifySame(s,q) ? q.front() == stackValue  : false;

        s.push(stackValue);
        queueValue = q.front();
        q.pop();
        q.push(queueValue);
	return pass;


        if (verifySame(s,q)) {
	
        }




    } 
}

}
