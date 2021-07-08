#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <codecvt>

const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()); //https://stackoverflow.com/questions/9900596/reading-chinese-character-from-csv-file-in-c-c
class myIfstream : public std::wifstream
{
    static const std::locale format;

public:
    myIfstream(const char *filename)
        : std::wifstream(filename)
    {
        this->imbue(format);
    }
};
const std::locale myIfstream::format = utf8_locale;

class myOfstream : public std::wofstream
{
    static const std::locale format;

public:
    myOfstream(const char *filename)
        : std::wofstream(filename)
    {
        this->imbue(format);
    }
};
const std::locale myOfstream::format = utf8_locale;

class LinkMap
{
public:
    using mapType = std::unordered_map<wchar_t, LinkMap *>;
    std::wstring val;
    mapType next_map;
    LinkMap() : val(L""), next_map(mapType()) {}
    LinkMap(std::wstring x) : val(x), next_map(mapType()) {}
};

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
    myIfstream mapFile(map_filename);
    myIfstream inputFile(input_filename);
    myOfstream outputFile(output_filename);
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

    auto head = new LinkMap();
    std::wstring textLine;
    while (std::getline(mapFile, textLine))
    {
        auto break_pos = textLine.find_first_of(L":：");
        auto key = textLine.substr(0, break_pos);
        auto value = textLine.substr(break_pos + 1);
        auto len = key.size();
        if (len == 0 || value.size() == 0)
        {
            std::cerr << "Error: Key or value in map file can not be blank";
            return 1;
        }
        auto cur = head;
        for (int i = 0; i != len; ++i)
        {
            if (cur->next_map.count(key[i]))
            {
                cur = cur->next_map[key[i]];
            }
            else
            {
                cur = cur->next_map[key[i]] = new LinkMap();
            }
        }
        cur->val = value;
    }
    mapFile.close();

    while (getline(inputFile, textLine))
    {
        auto len = textLine.size();
        for (int cur = 0; cur != len; ++cur)
        {
            if (head->next_map.count(textLine[cur]))
            {
                auto map_cur = head->next_map[textLine[cur]];
                auto is_match = false;
                int match_cur;
                for (match_cur = cur + 1; match_cur != len; ++match_cur)
                {
                    if (map_cur->next_map.count(textLine[match_cur]))
                    {
                        map_cur = map_cur->next_map[textLine[match_cur]];
                        if (map_cur->next_map.empty())
                        {
                            auto value = map_cur->val;
                            if (value != L"[del]")
                            {
                                outputFile << value;
                            }
                            is_match = true;
                            break;
                        }
                    }
                    else
                    {
                        auto value = map_cur->val;
                        if (value == L"[del]")
                        {
                            is_match = true;
                        }
                        else if (value != L"")
                        {
                            outputFile << value;
                            is_match = true;
                        }
                        else
                        {
                            outputFile << textLine[cur];
                            is_match = false;
                        }
                        --match_cur;
                        break;
                    }
                }
                if (is_match)
                {
                    cur = match_cur;
                }
                else if (match_cur == len)
                {
                    auto value = map_cur->val;
                    if (value == L"[del]")
                    {
                        cur = match_cur - 1;
                    }
                    else if (value != L"")
                    {
                        outputFile << value;
                        cur = match_cur - 1;
                    }
                }
            }
            else
            {
                outputFile << textLine[cur];
            }
        }
        outputFile << '\n';
    }
    inputFile.close();
    outputFile.close();
    return 0;
}
