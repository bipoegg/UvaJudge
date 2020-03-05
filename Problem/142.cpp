#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <iomanip>

static char start_region_id = 'A';
static int start_icon_id = 1;
#define MAX_NUM 2147483647

struct Point
{
	int x;
	int y;
};

struct Rect
{
	Point top_left;
	Point bottom_right;
};

struct Region
{
	Region() : id(start_region_id++) {}

	bool cover_point(Point pt)
	{
		if (pt.x >= area.top_left.x && pt.x <= area.bottom_right.x &&
			pt.y >= area.top_left.y && pt.y <= area.bottom_right.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	char id;
	Rect area;
};

struct Icon
{
	Icon() : id(start_icon_id++), is_visible(true) {}

	int id;
	Point position;
	bool is_visible;
};

std::vector<Region> regions_;
std::vector<Icon> icons_;
std::vector<Point> clicks_;

void check_icon_visible()
{
	for (auto& icon : icons_)
	{
		for (auto region : regions_)
		{
			if (region.cover_point(icon.position))
			{
				icon.is_visible = false;
				break;
			}
		}
	}
}

int get_distance(Point p1, Point p2)
{
	int d_x = p1.x - p2.x;
	int d_y = p1.y - p2.y;
	return d_x * d_x + d_y * d_y;
}

bool check_mouse_in_region(const Point& click, char& region_id)
{
	bool click_in_region = false;
	for (int i = regions_.size() - 1; i >= 0; i--)
	{
		Region region = regions_.at(i);
		if (region.cover_point(click))
		{
			region_id = region.id;
			click_in_region = true;
			break;
		}
	}

	return click_in_region;
}

std::vector<int> get_nearlest_icons(const Point click)
{
	int min_distance = MAX_NUM;
	std::vector<int> nearlest_icons;
	for (auto icon : icons_)
	{
		int distance = get_distance(icon.position, click);
		if (distance < min_distance)
		{
			min_distance = distance;
			nearlest_icons.clear();
			nearlest_icons.push_back(icon.id);
		}
		else if (distance == min_distance)
		{
			nearlest_icons.push_back(icon.id);
		}
	}
	return nearlest_icons;
}

void handle_mouse_click(std::ostream& out)
{
	for (auto click : clicks_)
	{

		char region_id = 'A';
		if (check_mouse_in_region(click, region_id))
		{
			out << static_cast<char>(region_id) << std::endl;
			continue;
		}

		std::vector<int> nearlest_icons = get_nearlest_icons(click);
		for (auto icon_id : nearlest_icons)
		{
			out << std::setw(3) << icon_id;
		}
		out << std::endl;
	}
}

void check_mouse_click(std::istream& in, std::ostream& out)
{
	try
	{
		char data_type = '#';
		while (!in.eof())
		{
			in >> data_type;
			switch (data_type)
			{
				case 'I':
				{
					Icon icon;
					in >> icon.position.x >> icon.position.y;
					icons_.push_back(icon);
					break;
				}
				case 'R':
				{
					Region region;
					in >> region.area.top_left.x >> region.area.top_left.y >> region.area.bottom_right.x >> region.area.bottom_right.y;
					regions_.push_back(region);
					break;
				}
				case 'M':
				{
					Point point;
					in >> point.x >> point.y;
					clicks_.push_back(point);
					break;
				}
				case '#':
					break;
				default:
					break;
			}
		}

		check_icon_visible();
		handle_mouse_click(out);

	}
	catch (std::exception& e)
	{
		out << e.what();
	}
}


int main()
{
	//check_mouse_click(std::cin, std::cout);

	std::ifstream input("input.txt");
	std::ofstream output("output.txt");

	check_mouse_click(input, output);

	return 0;
}

/*
Sample Input
I 216 28
R 22 19 170 102
I 40 150
I 96 138
I 36 193
R 305 13 425 103
I 191 184
I 387 200
R 266 63 370 140
I 419 134
I 170 102
M 50 50
M 236 30
M 403 167
M 330 83
#
Sample Output
A
  1
  6  7
C
*/