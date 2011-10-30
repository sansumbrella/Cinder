//
//  Dictionary.cpp
//  VisualDictionary
//
//  Created by Andrew Bell on 10/29/11.
//  Copyright (c) 2011. All rights reserved.
//

#include "cinder/Buffer.h"
#include "cinder/Utilities.h"
#include "cinder/Stream.h"

#include <iostream>
#include <algorithm>
#include <set>
#include <string.h>

#include "Dictionary.h"

using namespace std;
using namespace ci;

#ifndef strcasecmp // Windows has strnicmp instead of strncasecmp
	#define strcasecmp strnicmp
#endif

Dictionary::Dictionary( DataSourceRef dataSource )
{
	Buffer decompressed = decompressBuffer( Buffer( dataSource ), false, true );
	
	// read out all the words, which are separated by lines
	IStreamMemRef stream = IStreamMem::create( decompressed.getData(), decompressed.getDataSize() );
	while( ! stream->isEof() )
		mWords.push_back( stream->readLine() );

	std::cout << "Dictionary has: " << mWords.size() << " words" << std::endl;
}

struct CompareStringPrefix {
	bool operator()( string test1, string test2 ) const {
		return strncasecmp( test1.c_str(), test2.c_str(), std::min( test1.size(), test2.size() ) ) < 0;
	}
};

vector<string> Dictionary::getDescendants( const string &word ) const
{
	vector<string> result;
	set<char> foundLetters;

	// we want to figure out what letters follow 'word'
	// so for "victor" we want 'i' (victories), 's' (victors), 'y' (victory)
	pair<vector<string>::const_iterator, vector<string>::const_iterator> range;
	range = std::equal_range( mWords.begin(), mWords.end(), word, CompareStringPrefix() );
	
	for( vector<string>::const_iterator wordIt = range.first; wordIt != range.second; ++wordIt ) {
		if( wordIt->size() > word.size() )
			foundLetters.insert( (*wordIt)[word.size()] );
	}
	
	for( set<char>::const_iterator letIt = foundLetters.begin(); letIt != foundLetters.end(); ++letIt )
		result.push_back( word + *letIt );
	
	return result;
}