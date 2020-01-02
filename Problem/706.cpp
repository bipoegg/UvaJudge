#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <set>

enum class BorderLayer
{
    Top = 0,
    UpMiddle,
    Center,
    DownMiddle,
    Bottom
};

int border_shape[5][10] =
{                                     //   0  |  1  |   2   |   3
    { 1, 0, 1, 1, 0, 1, 1, 1, 1, 1 }, //   1  |     |   1   |   1
    { 3, 1, 1, 1, 3, 2, 2, 1, 3, 3 }, //  1 1 |   1 |     1 |     1
    { 0, 0, 1, 1, 1, 1, 1, 0, 1, 1 }, //      |     |   1   |   1
    { 3, 1, 2, 1, 1, 1, 3, 1, 3, 1 }, //  1 1 |   1 | 1     |     1
    { 1, 0, 1, 1, 0, 1, 1, 0, 1, 1 }  //   1  |     |   1   |   1
};

bool is_vertical(BorderLayer layer)
{
    return layer == BorderLayer::UpMiddle || layer == BorderLayer::DownMiddle;
}

std::string get_shape(int index, int boder_length, BorderLayer layer, int number_string_value)
{
    std::string shape = " ";
    if (is_vertical(layer))
    {
        if ((index == 0 && number_string_value & 0x2) ||
            (index == boder_length - 1 && number_string_value & 0x1))
        {
            shape = "|";
        }
    }
    else
    {
        if ((index > 0 && index < boder_length - 1) &&
            (number_string_value == 1))
        {
            shape = "-";
        }
    }

    return shape;
}

std::string get_string_by_number(int size, int number, BorderLayer layer)
{
    int string_format = border_shape[(int)layer][number];
    std::string number_string = "";
    int length = size + 2;
    for (int i = 0; i < length; i++)
    {
        number_string += get_shape(i, length, layer, string_format);
    }
    return number_string;
}

std::string get_line(int size, std::vector<int>& numbers, BorderLayer layer)
{
    std::string line = "";

    for (int i = 0; i < numbers.size(); i++)
    {
        int number = numbers.at(i);
        line += get_string_by_number(size, number, layer);
        if (i != numbers.size() - 1)
        {
            line += " ";
        }
    }

    return line;
}

void generate_number_strings(std::ostream& out, int size, std::vector<int>& numbers)
{
    int height = 2 * size + 3;
    for (int i = 0; i < height; ++i)
    {
        BorderLayer layer = BorderLayer::Top;
        if (i > 0 && i <= size)
        {
            layer = BorderLayer::UpMiddle;
        }
        else if (i == size + 1)
        {
            layer = BorderLayer::Center;
        }
        else if (i > size + 1 && i <= 2 * size + 1)
        {
            layer = BorderLayer::DownMiddle;
        }
        else if (i == height - 1)
        {
            layer = BorderLayer::Bottom;
        }

        out << get_line(size, numbers, layer) << std::endl;
    }
    out << std::endl;
}

std::vector<int> get_number_list(std::string number)
{
    std::vector<int> numbers;
    for (auto num : number)
    {
        numbers.push_back(num - '0');
    }
    return numbers;
}

std::istringstream read_line(std::istream& in)
{
    std::string line;
    std::getline(in, line);
    std::istringstream is(line);
    return is;
}

void number_string_generator(std::istream& in, std::ostream& out)
{
    try
    {
        while (!in.eof())
        {
            int size = 0;
            std::string number;

            std::istringstream is = read_line(in);
            is >> size >> number;

            if (size == 0 && number.compare("0") == 0)
            {
                break;
            }
            std::vector<int> numbers = get_number_list(number);
            generate_number_strings(out, size, numbers);
        }
    }
    catch (std::exception& e)
    {
        out << e.what();
    }

}


int main()
{
    number_string_generator(std::cin, std::cout);

    /*std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    number_string_generator(input, output);*/

    return 0;
}

/*
Sample Input
2 12345
3 67890
0 0
Sample Output
-- -- --
| | | | | |
| | | | | |
-- -- -- --
| | | | |
| | | | |
-- -- --
--- --- --- --- ---
| | | | | | | |
| | | | | | | |
| | | | | | | |
--- --- ---
| | | | | | | |
| | | | | | | |
| | | | | | | |
--- --- --- ---
*/