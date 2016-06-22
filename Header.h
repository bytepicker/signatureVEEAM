//
//  Header.h
//  signatureVEEAM
//
//  Created by shura on 20.06.16.
//  Copyright © 2016 shura. All rights reserved.
//

#ifndef Header_h
#define Header_h

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/crc.hpp>

void writeResults(const std::string&, const std::vector<std::vector<unsigned short> >&);
void singleTask(const std::string&, size_t&, size_t&, unsigned long&, std::vector<unsigned short>&);

#endif /* Header_h */