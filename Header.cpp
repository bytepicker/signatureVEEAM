//
//  Header.cpp
//  signatureVEEAM
//
//  Created by shura on 20.06.16.
//  Copyright © 2016 shura. All rights reserved.
//

#include "Header.h"

void writeResults(const std::string& outputFileName, const std::vector<std::vector<unsigned short> >& result)
{
    std::ofstream outputFileHandler;
    outputFileHandler.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    try {
        outputFileHandler.open(outputFileName.c_str());
        for (const std::vector<unsigned short> &v : result)
            for (const auto& x : v)
                outputFileHandler << std::hex <<  x <<  ' ';
        outputFileHandler.close();
    }
    
    catch (std::ofstream::failure& e) {
        std::cerr << "Error making output file." << e.what() << std::endl;
    }
}

void singleTask(const std::string& inputFileName, size_t& threadNumber, size_t& nThreads, unsigned long& chunkSize, std::vector<unsigned short>& resultVector)
{
    std::ifstream inputFile(inputFileName.c_str(), std::ios::in | std::ios::binary);
    auto buffer = std::vector<char>(chunkSize);
    boost::crc_32_type crcSum;
    
    size_t iteration = 0;
    
    while (!inputFile.eof())
    {
        inputFile.seekg((threadNumber + iteration * nThreads) * chunkSize, std::ios::beg);
        inputFile.read(&buffer[0], chunkSize);
        
        const size_t readBytes = inputFile.gcount();
        
        if (readBytes < chunkSize)
            buffer.resize(readBytes);
        
        if (!buffer.empty()) {
            // void  process_bytes( void const *buffer, std::size_t byte_count );
            crcSum.process_bytes(&buffer[0], buffer.size());
            resultVector.push_back(crcSum.checksum());
            //crcSum.reset();
            ++iteration;
        }
    }
    inputFile.close();
}