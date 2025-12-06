#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>


class NBT_Reader
{
    private:
    char tables = 0;
    char*& letter;
    unsigned int& readen;
    public:

    NBT_Reader(char*& letter, unsigned int& readen) : letter(letter), readen(readen)
    {
        0;
    }

    std::string TAGS[13] = {
        "End,"
        "Byte,"
        "Short,"
        "Int,"
        "Long,"
        "Float,"
        "Double,"
        "Byte_Array,"
        "String,"
        "List,"
        "Dict,"
        "Int_Array,"
        "Long_Array"
    };

    void add_(char x)
    {
        letter += x;
        readen += x;
    }

    std::string read_tag()
    {
        unsigned char tag = *letter;

        unsigned short number_of_chars = (*(letter + 1) << 8) | (*(letter + 2));

        add_(3);

        char name[number_of_chars + 1] = "\0";
        for (char i = 0; i < number_of_chars; i++, letter++, readen++)
        {
            name[i] = *letter;
        }

        std::string tables_str = "\0";
        for (char i = 0; i < tables; i++)
        {
            tables_str.append("\t");
        }

        std::string result = "\0";

        switch (tag)
        {
        case 3:
            {
                unsigned int dat = (*letter << 24) | (*(letter + 1) << 16) | (*(letter + 2) << 8) | *(letter + 3);
                result += ("INT " + std::string(name) + " = " + std::to_string(dat) + ";\n" + tables_str);
                add_(4);
            }
            break;
        case 5:
            {
                float dat = (*letter << 24) | (*(letter + 1) << 16) | (*(letter + 2) << 8) | *(letter + 3);
                result += ("Float " + std::string(name) + " = " + std::to_string(dat) + ";\n" + tables_str);
            }
        case 10:
            tables++;
            result += ("Dict " + std::string(name) + " = {\n" + tables_str + "\t");
            break;
        default:
            result = "None\n" + tables_str;
        }
        return result;
    }

    static std::string dec_to_hex(char& x)
    {
        char base[17] = "0123456789ABCDEF";
        char g = x;
        std::string str;
        while (g)
        {
            if ((g + 1) % 16)
            {
                str.push_back(base[g % 16]);
                g = int(g / 16);
            }
            else
            {
                str.push_back(base[g]);
                g = 0;
            }
        }
        std::reverse(str.begin(), str.end());
        if (!str.size())
            return "00";
        if (str.size() < 2)
            return "0" + str;
        return str;
    }
};

int main() {
    std::ifstream file("test", std::ios::binary);

    const unsigned int NUM_CHARS = 1000;
    char str[NUM_CHARS + 1];

    file.read(str, NUM_CHARS);

    str[NUM_CHARS] = '\0';

    char* letter = str;
    unsigned int readen = 0;

    NBT_Reader reader(letter, readen);

    // while (readen < 40)
    // {
    //     std::cout << reader.read_tag();
    // }

    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << int(*letter) << " " << readen << "\n";


    for (int i = 0; i < NUM_CHARS; ++i) {
        if (str[i] == ' ')
        continue;

        if (str[i] > 47)
        std::cout << (str[i]);

        else
        std::cout << "\\x" << reader.dec_to_hex(str[i]) << " ";
    }
    
    while (true)
    {
        /* code */
    }
    

    return 0;
}