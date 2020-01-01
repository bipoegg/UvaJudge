#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <set>

enum class BoderLayer
{
	Top = 0,
	TopCenter,
	Center,
	CenterBottom,
	Bottom
}

int border_shape[5][10] =
{                                   //   1   2    3
	{0, 1, 1, 0, 1, 1, 1, 1, 1, 1}, //       1    1
	{1, 1, 1, 3, 2, 2, 1, 3, 3, 3}, //   1     1    1
	{0, 1, 1, 1, 1, 1, 0, 1, 1, 0}, //       1    1
	{1, 2, 1, 1, 1, 3, 1, 3, 1, 3},	//   1 1        1
	{0, 1, 1, 0, 1, 1, 0, 1, 1, 1}  //       1    1
};

class NumberString
{
public 
	NumberString();

    bool is_border_exist(int border_index)
    {
        return borders.count(border_index) > 0;
    }

    void add_scent_point(int border_index)
    {
        borders.insert(border_index);
    }

private:
    std::set<int> borders;
}

void get_string_by_number(int size, int number, BorderLayer layer)
{
	int string_format = border_shape[(int)layer][number];
	std::string number_string = "";
	switch (layer)
	{
		case Top:
		case Center:
		case Bottom:
			number_string += " ";
			for (int i=0; i<size; i++)
			{
				number_string += (string_format == 1) ? "-" : " ";
			}
			number_string += " ";
			break;
		case TopCenter:
		case CenterBottom:
			std::string center_string = "";
			for (int i=0; i<size; i++)
			{
				center_string += " ";
			}
			
			if (string_format == 1)
			{
				number_string = " " + center_string + "|";
			}
			else if (string_format == 2)
			{
				number_string = "|" + center_string + " ";				
			}
			else// (string_format == 3)
			{
				number_string = "|" + center_string + "|";
			}
			break;
	}
	number_string += " ";
	return number_string;
}

std::string get_line(int size, std::vector<int>& numbers, BoderLayer layer)
{
	std::string line = "";
	
	for (auto number: numbers)
	{
		line += get_string_by_number(size, number, layer);
	}
}

void generate_number_strings(std::ostream& out, int size, std::vector<int>& numbers)
{
	int height = 2 * size + 3;
	for (int i = 0; i < height; i++i)
	{
		BoderLayer layer = BoderLayer:top
		if (i > 0 && i <= size)
		{
			layer = BoderLayer::TopCenter;
		}
		else if (i == size + 1)
		{
			layer = BoderLayer::Center;
		}
		else if (i > size + 1 && i <= 2 * size + 1)
		{
			layer = BoderLayer::CenterBottom;
		}
		else if (i == height - 1)
		{
			layer = BoderLayer::Bottom;
		}
		
		out << get_line(size, numbers, layer);
	}
	out << std::endl << std::endl;
}

std::vector<int> get_number_list(string number)
{
	std::vector<int> numbers;
	for(auto num : number)
	{
		numbers.push_back(s[i]-'0');
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
            // get position
            int size = 0;
            std::string number;
			
            is = read_line(in);
            is >> size >> number;
			
			if (size == 0 && number.compare("0") == 0)
			{
				break;
			}
            std::vector<int> numbers = get_number_list(number);
		
			

            // get command
            is = read_line(in);
            while (!is.eof())
            {
                is >> command;
                for (int i = 0; i < command.size(); i++)
                {
                    commands.push_back(parse_command(command[i]));
                }
            }

            // robot execute
            Robot robot(map, location, commands);
            robot.go();

            // output
            Location final_location = robot.get_location();
            out << final_location.point_.x_ << " " << final_location.point_.y_ << " " << get_direction_char(final_location.direction_);
            if (robot.is_robot_failed())
            {
                out << " LOST";
            }
            out << std::endl;
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