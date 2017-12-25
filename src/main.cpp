#include "ChainMaker.h"
#include <iostream>

int main(int argc, const char **argv)
{
    if(argc != 3) {
        std::cerr << "Usage: chainmaker <path to input words file> <path to dictionary file>" <<std::endl;
        return 1;
    }

    const auto inputWords = ChainMaker::parseInput(ChainMaker::readStringsFromFile(std::string(argv[1])));
    const auto dictionary = ChainMaker::readStringsFromFile(std::string(argv[2]));

    ChainMaker maker;
    maker.setDictionary(dictionary);

    for(const auto &s : maker.makeChain(inputWords.first, inputWords.second))
        std::cout << s << std::endl;

    return 0;
}
