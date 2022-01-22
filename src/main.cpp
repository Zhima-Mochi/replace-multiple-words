#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
using std::ifstream;
using std::ofstream;
using std::pair;
using std::string;
using std::u32string;
using std::unordered_map;
using std::vector;

class TrieNode
{
public:
    using Node = unordered_map<int, TrieNode *>;
    Node children;
    u32string val;
    TrieNode() = default;
    TrieNode(u32string n) : val(n), children() {}
};

class Trie
{
    pair<u32string, int> find_replace_word(const u32string &origin, int width, int idx)
    {
        auto cur = root;
        pair<u32string, int> res = {u32string{origin[idx]}, idx + 1};
        while (idx < width && cur->children.count(origin[idx]))
        {
            cur = cur->children[origin[idx]];
            if (!cur->val.empty())
            {
                res = {cur->val == del ? u32string() : cur->val, idx + 1};
            }
            ++idx;
        }
        return res;
    }

public:
    u32string del;
    TrieNode *root;
    void insert(const u32string &key, const u32string &val)
    {
        auto cur = root;
        for (auto c : key)
        {
            if (!cur->children.count(c))
            {
                cur->children[c] = new TrieNode();
            }
            cur = cur->children[c];
        }
        cur->val = val;
    }
    u32string replace_string(const u32string &origin)
    {
        u32string res;
        int idx = 0;
        int w = origin.size();
        while (idx < w)
        {
            auto [next_words, next_idx] = find_replace_word(origin, w, idx);
            res += next_words;
            idx = next_idx;
        }
        return res;
    }
};

string code_point_to_utf8(int code_point);
u32string utf8_string_to_code_point_string(const string &s);

int main(int argc, char **argv)
{
    const char *map_filename("map.txt");
    const char *input_filename("input.txt");
    const char *output_filename("output.txt");
    switch (argc)
    {
    case 4:
        output_filename = argv[3];
    case 3:
        input_filename = argv[2];
    case 2:
        map_filename = argv[1];
    case 1:
        std::cout << "Usage: transword <map file = " << map_filename << "> <input file = " << input_filename << "> <output file = " << output_filename << ">\n";
        break;
    default:
        return 1;
    }

    ifstream mapFile(map_filename);
    ifstream inputFile(input_filename);
    ofstream outputFile(output_filename);

    if (!mapFile)
    {
        std::cerr << "Failed to read map file";
        return 1;
    }
    else if (!inputFile)
    {
        std::cerr << "Failed to read input file";
        return 1;
    }

    Trie trie;
    trie.root = new TrieNode();
    trie.del = utf8_string_to_code_point_string("[del]");

    u32string break_words = utf8_string_to_code_point_string(":：");
    string textLine;
    while (getline(mapFile, textLine))
    {
        u32string u32textLine = utf8_string_to_code_point_string(textLine);
        int break_point = u32textLine.find_first_of(break_words);
        u32string key = u32textLine.substr(0, break_point);
        u32string value = u32textLine.substr(break_point + 1);
        if (key.size() == 0 || value.size() == 0)
        {
            std::cerr << "Error: Key or value in map file can not be blank";
            return 1;
        }
        trie.insert(key, value);
    }
    mapFile.close();

    while (getline(inputFile, textLine))
    {
        u32string u32textLine = utf8_string_to_code_point_string(textLine);
        u32string u32replacedStr = trie.replace_string(u32textLine);
        for (auto c : u32replacedStr)
        {
            outputFile << code_point_to_utf8(c);
        }
        outputFile << '\n';
    }
    inputFile.close();
    outputFile.close();
    return 0;
}

string code_point_to_utf8(int code_point)
{
    string res = "";
    res.reserve(4);
    if (code_point <= 0x7F)
    {
        res.push_back(code_point);
    }
    else if (code_point <= 0x7FF)
    {
        res.push_back((code_point >> 6) + 192);
        res.push_back((code_point & 63) + 128);
    }
    else if (code_point >= 0xD800 && code_point <= 0xDFFF)
    {
    }
    else if (code_point <= 0xFFFF)
    {
        res.push_back((code_point >> 12) + 224);
        res.push_back(((code_point >> 6) & 63) + 128);
        res.push_back((code_point & 63) + 128);
    }
    else if (code_point <= 0x10FFFF)
    {
        res.push_back((code_point >> 18) + 240);
        res.push_back(((code_point >> 12) & 63) + 128);
        res.push_back(((code_point >> 6) & 63) + 128);
        res.push_back((code_point & 63) + 128);
    }
    return res;
}

u32string utf8_string_to_code_point_string(const string &s)
{
    u32string res;
    int w = s.size();
    if (w == 0)
    {
        return res;
    }
    vector<int> str_int;
    for (auto c : s)
    {
        str_int.push_back(static_cast<unsigned char>(c));
    }
    try
    {
        int idx = 0;
        while (idx < w)
        {
            if (str_int[idx] >= 0 && str_int[idx] <= 127)
            {
                res.push_back(str_int[idx]);
                ++idx;
            }
            else if (str_int[idx] >= 192 && str_int[idx] <= 223)
            {
                res.push_back((str_int[idx] - 192) * 64 + (str_int[idx + 1] - 128));
                idx += 2;
            }
            else if (str_int[idx] >= 224 && str_int[idx] <= 239)
            {
                res.push_back((str_int[idx] - 224) * 4096 + (str_int[idx + 1] - 128) * 64 + (str_int[idx + 2] - 128));
                idx += 3;
            }
            else if (str_int[idx] >= 240 && str_int[idx] <= 247)
            {
                res.push_back((str_int[idx] - 240) * 262144 + (str_int[idx + 1] - 128) * 4096 + (str_int[idx + 2] - 128) * 64 + (str_int[idx + 3] - 128));
                idx += 4;
            }
        }
    }
    catch (const std::out_of_range &oor)
    {
        std::cerr << "Out of Range error: " << oor.what() << '\n';
    }
    return res;
}