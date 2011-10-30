#pragma once

#include "cinder/DataSource.h"

#include <vector>

class Dictionary {
  public:
	Dictionary( ci::DataSourceRef dictionaryFile );
	
	std::vector<std::string>	getDescendants( const std::string &word ) const;
	
	std::vector<std::string>	mWords;
};