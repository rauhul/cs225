/**
 * @file htiterator.h
 * Definition for an iterator class for HashTables.
 *
 * @author Chase Geigle
 * @date Summer 2012
 */
#ifndef _HASHTABLE_ITERATOR_H_
#define _HASHTABLE_ITERATOR_H_

/**
 * Iterator for iterating over a hashtable.
 *
 * This example will print out all of the key, value pairs in the
 * HashTable variable `ht`:
 *
 *     for( HashTable::iterator it = ht.begin(); it != ht.end(); ++it )
 *         cout << "key: " << it->first << " value: " << it->second << endl;
 */
template <class K, class V>
class HashTable<K,V>::iterator : public std::iterator< std::forward_iterator_tag, std::pair<K, V> > {
	public:
		// hash table can build iterators with our private constructor
		friend class HashTable<K,V>;

		iterator();
		iterator( const iterator & other );
		const iterator & operator=( const iterator & rhs );
		~iterator();
		iterator & operator++();
		iterator operator++(int);
		bool operator==( const iterator & rhs ) const;
		bool operator!=( const iterator & rhs ) const;
		const std::pair<K,V> & operator*();
		const std::pair<K,V> * operator->();

	private:
		iterator( HTIteratorImpl * ptr );

		HTIteratorImpl * impl;

		void copy( const iterator & other );
		void clear();
};

template <class K, class V>
class HashTable<K,V>::HTIteratorImpl {
	public:
		virtual ~HTIteratorImpl() { /* nothing */ }
		virtual HTIteratorImpl * clone() const = 0;
		virtual void operator++() = 0;
		virtual bool operator==( const HTIteratorImpl & other ) const = 0;
		virtual bool operator!=( const HTIteratorImpl & other ) const {
			return !(operator==( other ));
		}
		virtual const std::pair<K,V> & operator*() = 0;
};
#include "htiterator.cpp"
#endif
