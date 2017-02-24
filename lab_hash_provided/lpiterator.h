#ifndef _LPITERATOR_H_
#define _LPITERATOR_H_
template <class K, class V>
class LPHashTable<K,V>::LPIteratorImpl : public HashTable<K,V>::HTIteratorImpl {
	public:
		friend class LPHashTable<K,V>;
		friend class HashTable<K,V>;

		typedef typename HashTable<K,V>::HTIteratorImpl HTIteratorImpl;

		virtual void operator++();
		virtual bool operator==( const HTIteratorImpl & other ) const;
		virtual bool operator==( const LPIteratorImpl & other ) const;
		virtual const std::pair<K,V> & operator*();

		virtual HTIteratorImpl * clone() const;

	private:
		int bucket;
		const LPHashTable<K,V> & table;

		LPIteratorImpl( const LPHashTable & ht, int i );
};
#include "lpiterator.cpp"
#endif
