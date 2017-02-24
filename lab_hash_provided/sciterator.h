#ifndef _SCITERATOR_H_
#define _SCITERATOR_H_
template <class K, class V>
class SCHashTable<K,V>::SCIteratorImpl : public HashTable<K,V>::HTIteratorImpl {
	public:
		friend class SCHashTable<K,V>;

		typedef typename HashTable<K,V>::HTIteratorImpl HTIteratorImpl;

		virtual void operator++();
		virtual bool operator==( const HTIteratorImpl & other ) const;
		virtual bool operator==( const SCIteratorImpl & other ) const;
		virtual const std::pair<K,V> & operator*();
		
		virtual HTIteratorImpl * clone() const;

	private:
		const SCHashTable & table;
		int bucket;
		typename std::list< std::pair<K,V> >::iterator bucket_iterator;
		bool end;

		SCIteratorImpl( const SCHashTable & ht, int i, bool ed );
};
#include "sciterator.cpp"
#endif
