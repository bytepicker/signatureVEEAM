//
//  main.cpp
//  signatureVEEAM
//
//  Created by shura on 15.06.16.
//  Copyright © 2016 shura. All rights reserved.
//

#include <exception>
#include <boost/filesystem.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>

#include "Header.h"

using namespace std;

int main(int argc, char** argv)
{
    try
    {
        if (argc < 3 || argc > 5)
            throw invalid_argument("Usage: input_file output_file [chunk_size]");
        
        const string inputFileName(argv[1]);
        if (!boost::filesystem::exists(inputFileName))
                throw runtime_error("Input file not found");
            
        const string outputFileName(argv[2]);
            
        // default chunk size = 1Mb
        unsigned long chunkSize = 1024 * 1024;
        const unsigned long totalSize = boost::filesystem::file_size(argv[1]);
            
        if (argc == 4) {
            unsigned long cSize = stoi(argv[3]);
            if (cSize <= 0)
                throw invalid_argument("Chunk size must be > 0");
            if (cSize > totalSize)
                throw invalid_argument("Chunk size is too big");
            chunkSize = cSize;
        }
            
        const size_t nThreads = boost::thread::hardware_concurrency();
        
        vector<vector<unsigned int> > outputResult(nThreads);
        boost::asio::io_service ioService;
        
        for (size_t i = 0; i < nThreads; ++i) {
            ioService.post(boost::bind(&singleTask, inputFileName, i, nThreads, chunkSize, boost::ref(outputResult[i])));
        }

        boost::thread_group threadPool;

        // add threads to the thread pool
        for (size_t i=0; i<nThreads; ++i) {
            threadPool.create_thread(boost::bind(&boost::asio::io_service::run, &ioService));
        }
        
        threadPool.join_all();
        ioService.stop();
        writeResults(outputFileName, outputResult);
        }
    
    catch (const invalid_argument& errArg) {
        cerr << "Wrong argument. " << errArg.what() << endl;
    }
    catch (const runtime_error& errRun) {
        cerr << "Runtime error. " << errRun.what() << endl;
    }
    catch (const exception& errAny) {
        cerr << errAny.what() << endl;
    }
    return 0;
}