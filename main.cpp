#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>


class NBT_Reader
{
    private:
    char tables = 0;
    char*& letter;
    unsigned int& readen;
    std::vector<unsigned int> stocks{};
    std::vector<unsigned int> number{};
    bool is_name = true;
    unsigned char cls_in_matrix = 0;

    std::string get_res(unsigned char tag, std::string name)
    {
        std::string result = "\0";

        std::string tables_str = "\0";
        for (char i = 0; i < tables; i++)
        {
            tables_str.append("\t");
        }

        if (is_name)
        {
            result += (tables_str + TAGS[tag] + " " + name + " = ");
            return result;
        }
        return (tables_str + TAGS[tag] + " : ");
    }

    public:

    NBT_Reader(char*& letter, unsigned int& readen) : letter(letter), readen(readen)
    {
        0;
    }
    std::string TAGS[13] = {
        "End",
        "Byte",
        "Short",
        "Int",
        "Long",
        "Float",
        "Double",
        "Byte_Array",
        "String",
        "List",
        "Dict",
        "Int_Array",
        "Long_Array"
    };

    void add_(char x)
    {
        letter += x;
        readen += x;
    }

    std::string get_res_is_name()
    {
        std::string result = "\0";
        unsigned char tag;
        if (is_name)
        {
            tag = *letter;
        }
        else
        {
            tag = cls_in_matrix;
            number.back()++;
        }
        
        if (!tag && is_name)
        {
            tables--;
            std::string tables_str = "\0";
            for (char i = 0; i < tables; i++)
            {
                tables_str.append("\t");
            }
            add_(1);
            if (stocks.back() > number.back())
            {
                tables++;
                if (!number.empty())
                {
                    if (number.back() < stocks.back())
                    {
                        number.back()++;
                    }
                }
                return tables_str + "},\n" + tables_str + "{\n";
            }
            stocks.pop_back();
            number.pop_back();
            is_name = true;
            return tables_str + "};\n";
        }

        unsigned short number_of_chars = 1;
        if (is_name)
        {
            number_of_chars = (*(letter + 1) << 8) | (*(letter + 2));
        }

        add_(3);

        char name[number_of_chars + 1] = "\0";
        if (is_name)
        {
            for (char i = 0; i < number_of_chars; i++, add_(1))
            {
                name[i] = *letter;
            }
        }
        
        std::string tables_str = "\0";
        for (char i = 0; i < tables; i++)
        {
            tables_str.append("\t");
        }

        if (tag == 1)
            {
                bool dat = *letter;
                std::string res = (dat) ? "true" : "false";
                result += (tables_str + "Bool " + std::string(name) + " = " + res + ";\n");
                add_(1);
            }
        else if (tag == 3)
            {
                unsigned int dat = (*letter << 24) | (*(letter + 1) << 16) | (*(letter + 2) << 8) | *(letter + 3);
                result += get_res(tag, std::string(name)) + std::to_string(dat) + ";\n";
                add_(4);
            }
        else if (tag == 8)
            {
                unsigned short number_of_chars_in_data = (*(letter) << 8) | (*(letter + 1));
                add_(2);
                char dat[number_of_chars_in_data + 1] = "\0";
                for (char i = 0; i < number_of_chars_in_data; i++, add_(1))
                {
                    dat[i] = *letter;
                }
                result += get_res(tag, std::string(name)) + std::string(dat) + ";\n";
            }
        else if (tag == 5)
            {
                float dat = (*letter << 24) | (*(letter + 1) << 16) | (*(letter + 2) << 8) | *(letter + 3);
                result += get_res(tag, std::string(name)) + std::to_string(dat) + ";\n";
                add_(4);
            }
        else if (tag == 6)
            {
                double dat = (*(letter) << 56) | (*(letter + 1) << 48) | (*(letter + 2) << 40) | (*(letter + 3) << 32) | (*(letter + 4) << 24) | (*(letter + 5) << 16) | (*(letter + 6) << 8) | *(letter + 7);
                result += get_res(tag, std::string(name)) + std::to_string(dat) + ";\n";
                add_(8);
            }
        else if (tag == 9)
            {
                tables++;
                unsigned int num_dat = ((*(letter + 1) << 24) | (*(letter + 2) << 16) | (*(letter + 3) << 8) | *(letter + 4));
                stocks.push_back(num_dat - 1);
                number.push_back(0);
                if (TAGS[*(letter)] != "Dict")
                {
                    is_name = false;
                    cls_in_matrix = *letter;
                }
                result += tables_str + TAGS[*(letter)] + " " + std::string(name) + "[" + std::to_string(num_dat) + "] = {\n";
                add_(5);
            }
        else if (tag == 10)
            {
                tables++;
                result += (tables_str + "Dict " + std::string(name) + " = {\n");
            }
        else
            result = "None\n" + tables_str;
        return result;
    }

    std::string read_tag()
    {
        return get_res_is_name();
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
    std::ifstream file("player1.dat", std::ios::binary);

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
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();
    std::cout << reader.read_tag();

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