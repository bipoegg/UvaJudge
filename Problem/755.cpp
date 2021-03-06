#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <map>

const int AlphaToNumber[] = {
	2, 2, 2, 3, 3, 3,
	4, 4, 4, 5, 5, 5,
	6, 6, 6, 7, -1, 7, 7,
	8, 8, 8, 9, 9, 9, -1 };

int get_number(char tel)
{
	if (isdigit(tel))
	{
		return tel - '0';
	}
	else if (isalpha(tel))
	{
		return AlphaToNumber[tel - 'A'];
	}
	else
	{
		return -1;
	}
}

class TelephoneDirectory
{
public:
	TelephoneDirectory() {}
	virtual ~TelephoneDirectory() {}

	void add_telephone(const std::string& telephone_text)
	{
		int telephone_number = 0;
		for (auto tel : telephone_text)
		{
			int number = get_number(tel);
			if (number != -1)
			{
				telephone_number = 10 * telephone_number + number;
			}
		}

		if (directory_.find(telephone_number) == directory_.end())
		{
			directory_[telephone_number] = 1;
		}
		else
		{
			++directory_[telephone_number];
		}
	}

	void output_duplicate(std::ostream& out)
	{
		bool has_duplicate_phone = false;
		for (auto & telephone : directory_)
		{
			if (telephone.second > 1)
			{
				has_duplicate_phone = true;

				char output_telephone[9];
				snprintf(output_telephone, 9, "%.3d-%.4d", (telephone.first / 10000), (telephone.first % 10000));
				out << output_telephone << " " << telephone.second << std::endl;
			}
		}

		if (!has_duplicate_phone)
		{
			out << "No duplicates." << std::endl;
		}
	}

private:
	std::map<int, int> directory_;
};

void telephone_parser(std::istream& in, std::ostream& out)
{
	try
	{
		int dataset_count = 0;
		in >> dataset_count;

		for (int i = 0; i < dataset_count; ++i)
		{
			TelephoneDirectory tel_directory;
			int telephone_count = 0;
			in >> telephone_count;
			for (int j = 0; j < telephone_count; ++j)
			{
				std::string memerable_telephone;
				
				in >> memerable_telephone;
				tel_directory.add_telephone(memerable_telephone);
			}

			tel_directory.output_duplicate(out);
			if (i != dataset_count - 1)
			{
				out << std::endl;
			}
		}

	}
	catch (std::exception& e)
	{
		out << e.what();
	}
}


int main()
{
	telephone_parser(std::cin, std::cout);

	/*std::ifstream input("input.txt");
	std::ofstream output("output.txt");

	telephone_parser(input, output);*/

	return 0;
}

/*
Sample Input
1

12
4873279
ITS-EASY
888-4567
3-10-10-10
888-GLOP
TUT-GLOP
967-11-11
310-GINO
F101010
888-1200
-4-8-7-3-2-7-9-
487-3279
Sample Output
310-1010 2
487-3279 4
888-4567 3
--- --- --- ---
*/
