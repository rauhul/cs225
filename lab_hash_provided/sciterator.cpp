using std::pair;
using std::list;

#include <iostream>
using std::cout;
using std::endl;

template <class K, class V>
SCHashTable<K,V>::SCIteratorImpl::SCIteratorImpl( const SCHashTable<K,V> & ht, int i, bool en ) : table( ht ), bucket( i ), end( en ) {
	if( bucket < table.size ) {
		bucket_iterator = table.table[bucket].begin();
		if( bucket_iterator == table.table[bucket].end() )
			operator++();
	}
}

template <class K, class V>
void SCHashTable<K,V>::SCIteratorImpl::operator++() {
    if( ++bucket_iterator == table.table[bucket].end() ) {
        while( ++bucket < table.size && table.table[bucket].empty() );
        if( bucket < table.size ) {
            bucket_iterator = table.table[bucket].begin();
        } else
            end = true;
    }
}

template <class K, class V>
bool SCHashTable<K,V>::SCIteratorImpl::operator==( const HTIteratorImpl & rhs ) const {
	const HTIteratorImpl * tmp = &rhs;
	const SCIteratorImpl * other = dynamic_cast<const SCIteratorImpl *>( tmp );
	if( other == NULL )
		return false;
	else
		return *other == *this;
}

template <class K, class V>
bool SCHashTable<K,V>::SCIteratorImpl::operator==( const SCIteratorImpl & rhs ) const {
	return &table == &rhs.table && bucket == rhs.bucket && 
		( bucket_iterator == rhs.bucket_iterator || (end && rhs.end) );
}

template <class K, class V>
const pair<K,V> & SCHashTable<K,V>::SCIteratorImpl::operator*() {
    return *bucket_iterator;
}

template <class K, class V>
typename HashTable<K,V>::HTIteratorImpl * SCHashTable<K,V>::SCIteratorImpl::clone() const {
	return new SCIteratorImpl( table, bucket, end );
}
