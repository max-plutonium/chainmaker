#pragma once

#include <string>
#include <vector>
#include <codecvt>
#include <locale>
#include <set>
#include <unordered_map>

class ChainMaker
{
    struct StringCodec : std::codecvt<char16_t, char, std::mbstate_t> { };
    using ConvType = std::wstring_convert<StringCodec, char16_t>;
    using DictType = std::set<ConvType::wide_string>;
    using DictIterator = DictType::iterator;
    using CharsMap = std::unordered_multimap<char16_t, DictIterator>;
    using CharsMapIterator = CharsMap::iterator;

    /// Конвертер для перевода из многобайтовой кодировки в строку широких символов
    ConvType converter;

    /// Словарь
    DictType dictionary;

    /// Таблица первых символов строк в словаре
    CharsMap firstChars;

    static bool oneLetterDiff(const ConvType::wide_string &lhs, const ConvType::wide_string &rhs);

public:
    static std::vector<std::string>
    readStringsFromFile(const std::string& path);

    static std::pair<std::string, std::string>
    parseInput(const std::vector<std::string> &stringList);

    void setDictionary(const std::vector<std::string> &stringList);
    bool hasDictionary() const;

    std::vector<std::string>
    makeChain(const std::string& srcString, const std::string& destString);
};
