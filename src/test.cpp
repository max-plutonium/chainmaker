#include <gtest/gtest.h>
#include "ChainMaker.h"

TEST(ChainMaker, ReadStringsFromInput)
{
    const std::string path = "../data/input.txt";

    const auto stringList = ChainMaker::readStringsFromFile(path);

    EXPECT_EQ(2, stringList.size());
    EXPECT_EQ("КОТ", stringList.at(0));
    EXPECT_EQ("ТОН", stringList.at(1));
}

TEST(ChainMaker, ReadStringsFromWrongFile)
{
    const std::string path = "../data/notexist.txt";

    EXPECT_THROW(ChainMaker::readStringsFromFile(path), std::runtime_error);
}

TEST(ChainMaker, ParseInputFromStringList)
{
    const std::vector<std::string> stringList{"КОН", "ДОР"};

    auto pair = ChainMaker::parseInput(stringList);
    EXPECT_EQ("КОН", pair.first);
    EXPECT_EQ("ДОР", pair.second);
}

TEST(ChainMaker, ParseInputFromWrongStringList)
{
    std::vector<std::string> stringList{"КО", "ДОР", "ДОР"};
    EXPECT_THROW(ChainMaker::parseInput(stringList), std::runtime_error);

    stringList.pop_back();
    EXPECT_THROW(ChainMaker::parseInput(stringList), std::runtime_error);
}

TEST(ChainMaker, SetDictionaryTest)
{
    std::vector<std::string> stringList{"КО", "ДОР", "ДО"};

    ChainMaker maker;
    EXPECT_FALSE(maker.hasDictionary());
    maker.setDictionary(stringList);
    EXPECT_TRUE(maker.hasDictionary());
}

TEST(ChainMaker, MakeOneChainTest)
{
    ChainMaker maker;

    // Словарь не нужен
    auto res = maker.makeChain("КОТ", "КОТ");
    EXPECT_EQ(1, res.size());
    EXPECT_EQ("КОТ", res.front());
}

TEST(ChainMaker, MakeTwoChainTest)
{
    std::vector<std::string> dict {
        "КОТ", "ТОН", "НОТА", "КОТЫ",
        "РОТ", "РОТА", "ТОТ"
    };

    ChainMaker maker;
    maker.setDictionary(dict);

    auto res = maker.makeChain("КОТ", "ТОТ");
    EXPECT_EQ(2, res.size());
    EXPECT_EQ("КОТ", res[0]);
    EXPECT_EQ("ТОТ", res[1]);
}

TEST(ChainMaker, MakeTwoChainWithoutDictTest)
{
    ChainMaker maker;

    auto res = maker.makeChain("КОТ", "ТОТ");
    EXPECT_EQ(0, res.size());
}

TEST(ChainMaker, MakeThreeChainTest)
{
    std::vector<std::string> dict {
        "КОТ", "ТОН", "НОТА", "КОТЫ",
        "РОТ", "РОТА", "ТОТ"
    };

    ChainMaker maker;
    maker.setDictionary(dict);

    auto res = maker.makeChain("КОТ", "ТОН");
    EXPECT_EQ(3, res.size());
    EXPECT_EQ("КОТ", res[0]);
    EXPECT_EQ("ТОТ", res[1]);
    EXPECT_EQ("ТОН", res[2]);
}

TEST(ChainMaker, MakeChainWithoutWordTest)
{
    std::vector<std::string> dict {
        "КОТ", "ТОН", "НОТА", "КОТЫ",
        "РОТ", "РОТА", "ТОТ", "ДОН"
    };

    ChainMaker maker;
    maker.setDictionary(dict);

    auto res = maker.makeChain("КОТ", "ДОН");
    EXPECT_EQ(0, res.size());
}

TEST(ChainMaker, MakeComplexChainTest)
{
    std::vector<std::string> dict {
        "КОТ", "ТОН", "НОТА", "КОТЫ",
        "РОТ", "РОТА", "ТОТ", "КОЖА",
        "АОТА", "ЫОТЫ", "ЫОТН", "НОТН",
        "АОТР", "РОТР"
    };

    ChainMaker maker;
    maker.setDictionary(dict);

    auto res = maker.makeChain("КОТЫ", "РОТА");
    EXPECT_EQ(9, res.size());
    EXPECT_EQ("КОТЫ", res[0]);
    EXPECT_EQ("ЫОТЫ", res[1]);
    EXPECT_EQ("ЫОТН", res[2]);
    EXPECT_EQ("НОТН", res[3]);
    EXPECT_EQ("НОТА", res[4]);
    EXPECT_EQ("АОТА", res[5]);
    EXPECT_EQ("АОТР", res[6]);
    EXPECT_EQ("РОТР", res[7]);
    EXPECT_EQ("РОТА", res[8]);
}
