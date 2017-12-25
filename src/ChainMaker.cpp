#include "ChainMaker.h"

#include <exception>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <deque>
#include <algorithm>

/**
 * Читает из файла список строк
 * @param path Путь к файлу
 * @return Список строк
 */
//static
std::vector<std::string> ChainMaker::readStringsFromFile(const std::string& path)
{
    std::ifstream is(path);

    if(!is.is_open())
        throw std::runtime_error("Could not open file " + path);

    std::stringstream ss({
        std::istreambuf_iterator<char>(is.rdbuf()),
        std::istreambuf_iterator<char>()
    });

    std::string to;

    std::vector<std::string> stringList;

    while(std::getline(ss, to, '\n'))
        stringList.push_back(std::move(to));

    is.close();

    return stringList;
}

/**
 * Выделает из списка строк начальное и конечное слово
 */
//static
std::pair<std::string, std::string>
ChainMaker::parseInput(const std::vector<std::string> &stringList)
{
    if(2 != stringList.size())
        throw std::runtime_error("List must be size of 2");

    if(stringList.at(0).size() != stringList.at(1).size())
        throw std::runtime_error("Words in list must be same sizes");

    return std::make_pair(stringList.at(0), stringList.at(1));
}

/**
 * Возвращает true, если строки одинаковы либо различаются на один символ.
 * Не делает проверок на длину строк - они должны быть одинаковы.
 */
//static
bool ChainMaker::oneLetterDiff(const ConvType::wide_string &lhs, const ConvType::wide_string &rhs)
{
    bool hasDifference = false;
    for(auto i = 0; i < lhs.size(); ++i)
        if(lhs[i] != rhs[i])
            if(!hasDifference)
                hasDifference = true;
            else
                return false;
    return true;
}

/**
 * Устанавливает словарь для поиска слов
 * @param stringList Список слов словаря
 */
void ChainMaker::setDictionary(const std::vector<std::string> &stringList)
{
    for (const auto &s : stringList)
    {
        const auto iterableString = converter.from_bytes(s);
        auto it = dictionary.insert(iterableString).first;
        firstChars.emplace(iterableString.front(), it);
    }
}

/**
 * @return true, если словарь установлен
 */
bool ChainMaker::hasDictionary() const
{
    return !dictionary.empty();
}

/**
 * Собирает цепочку слов из внутреннего словаря, от слова @a srcString до слова @a destString.
 * Каждое следующее слово в цепочке отличается от предыдущего только одной буквой.
 * Исходное, конечное и все промежуточные слова состоят из одинакового количества букв.
 * Все используемые слова содержатся в словаре.
 *
 * @param srcString Начальная строка
 * @param destString Конечная строка
 * @return Список строк - цепочка от начальной к конечной
 */
std::vector<std::string>
ChainMaker::makeChain(const std::string &srcString, const std::string &destString)
{
    const auto wideSrcString = converter.from_bytes(srcString);
    const auto wideDestString = converter.from_bytes(destString);

    std::vector<std::string> chain;
    chain.push_back(srcString);

    ConvType::wide_string nextWord;

    // Так как следующее слово в цепочке должно отличаться только на одну букву,
    // то мы сразу можем понять какое слово мы должны искать - вначале ищем в словаре
    // только то слово, в котором первая и последня буквы те же, что и последняя буква первого слова
    for(auto curWord = wideSrcString; curWord != wideDestString;)
    {
        nextWord = curWord.back() + curWord.substr(1);

        // Если сконструированное таким способом слово найдено в словаре,
        // продолжаем цепочку, иначе мы сразу можем выдать ошибку и указать,
        // какое слово требуется добавить в словарь для построения цепочки
        if (nextWord != curWord) {
            if (dictionary.cend() != dictionary.find(nextWord)) {
                chain.push_back(converter.to_bytes(nextWord));
                curWord = nextWord;
            } else {
                std::cerr << "Need word: " << converter.to_bytes(nextWord) << std::endl;
                return {};
            }
        }
        else
            break;
    }

    if(destString == chain.back())
        return chain;

    // Если получилось такое же слово, как и предыдущее, значит необходимо
    // перебирать варианты в словаре методов поиска в глубину

    // Стек обрабатываемых слов
    std::deque<ConvType::wide_string> wordStack;

    // Обрабатываемые диапазоны слов для данных начальных букв
    std::unordered_map<ConvType::wide_string,
            std::pair<CharsMapIterator, CharsMapIterator>> wordsCharRange;

    // Посещенные слова
    std::unordered_set<ConvType::wide_string> visitedWords;

    wordStack.push_back(nextWord);
    wordsCharRange.emplace(nextWord, firstChars.equal_range(nextWord.back()));

    while(!wordStack.empty())
    {
        const auto topWord = wordStack.back();
        auto &charRange = wordsCharRange.at(topWord);

        // Проверим, не нашли ли мы уже конечное слово
        if(topWord == wideDestString)
            break;

        // Пробегаемся по всем словам, начинающимся с конечной буквы topWord
        for(; charRange.first != charRange.second; ++charRange.first)
        {
            const auto word = *charRange.first->second;

            if(visitedWords.cend() != visitedWords.find(word))
                continue;

            const bool sameSize = word.size() == topWord.size();
            const bool onlyOneCharDiff = oneLetterDiff(word, topWord);
            const bool notInWork = wordsCharRange.cend() == wordsCharRange.find(word);

            // Хороший вариант для дальнейшей проверки
            if (sameSize && onlyOneCharDiff && notInWork) {
                wordStack.push_back(word);
                wordsCharRange.emplace(word, firstChars.equal_range(word.back()));
                break;
            }
        }

        // Ничего не найдено для последующих слов от topWord
        if(charRange.first == charRange.second) {
            visitedWords.insert(topWord);
            wordsCharRange.erase(topWord);
            wordStack.pop_back();
        }
    }

    // Если мы прошли весь словарь и не нашли цепочку слов, сообщим об этом
    if(wordStack.empty()) {
        std::cerr << "No way from " << srcString << " to " << destString << " in dictionary" << std::endl;
        return {};
    }

    // Начальное для поиска слово уже содержится в chain
    wordStack.pop_front();

    // Добавляем найденную цепочку слов из стека
    std::transform(wordStack.cbegin(), wordStack.cend(), std::back_inserter(chain),
        [&](const auto &wideString) {
            return converter.to_bytes(wideString);
        });

    return chain;
}
