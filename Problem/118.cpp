#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <set>
using namespace std;

enum class Direction
{
    None = -1,
    North = 0,
    East,
    South,
    West
};

enum class Command
{
    None = -1,
    Right = 0,
    Left,
    Forward
};

struct Point
{
    Point(int x, int y)
        : x_(x)
        , y_(y)
    {
    }

    int x_;
    int y_;
};

struct Location
{
    Location(Point point, Direction direction)
        : point_(point)
        , direction_(direction)
    {
    }

    Point point_;
    Direction direction_;
};

Direction parse_location(char* direction)
{
    switch (direction[0])
    {
    case 'N':
        return Direction::North;
    case 'E':
        return Direction::East;
    case 'S':
        return Direction::South;
    case 'W':
        return Direction::West;
    }

    return Direction::None;
}

char get_direction_char(Direction direction)
{
    switch (direction)
    {
    case Direction::North:
        return 'N';
    case Direction::East:
        return 'E';
    case Direction::South:
        return 'S';
    case Direction::West:
        return 'W';
    default:
        return '?';
    }
}

Command parse_command(char* command)
{
    switch (command[0])
    {
    case 'R':
        return Command::Right;
    case 'L':
        return Command::Left;
    case 'F':
        return Command::Forward;
    }

    return Command::None;
}

class Map
{
public:
    Map(int width, int height)
        : width_(width)
        , height_(height)
    {
    }

    bool is_out_of_bound(Point point)
    {
        if (point.x_ < 0 || point.x_ > width_ ||
            point.y_ < 0 || point.y_ > height_)
            return true;
        else
            return false;
    }

    bool is_current_point_has_scent(const Point& point)
    {
        return scent_points_.count(point) > 0;
    }

    void add_scent_point(const Point& point)
    {
        scent_points_.insert(point);
    }

private:
    int width_;
    int height_;
    set<Point> scent_points_;
};

class Robot
{
public:
    Robot(Map& map, Location location, vector<Command>& commands)
        : map_(map)
        , location_(location)
        , commands_(commands)
        , is_failed(false)
    {
    }

    void go()
    {
        for (auto& command : commands_)
        {
            Location go_back_location = location_;
            if (!operation(command))
            {
                location_ = go_back_location;
                if (!map_.is_current_point_has_scent(location_.point_))
                {
                    map_.add_scent_point(location_.point_);
                    is_failed = true;
                    break;
                }
            }
        }
    }

    Location get_location()
    {
        return location_;
    }

    bool is_robot_failed()
    {
        return is_failed;
    }

private:
    void rotate(Command command)
    {
        int is_clockwise = Command::Right == command;
        int offset = (int)Direction::West + 1;
        int shift = is_clockwise ? 1 : offset - 1;

        location_.direction_ = (Direction)(((int)location_.direction_ + shift) % offset);
    }

    void go_foward()
    {
        switch (location_.direction_)
        {
        case Direction::North:
            location_.point_.y_++;
            break;
        case Direction::East:
            location_.point_.x_++;
            break;
        case Direction::South:
            location_.point_.y_--;
            break;
        case Direction::West:
            location_.point_.x_--;
            break;
        default:
            break;
        }
    }

    bool operation(Command command)
    {
        switch (command)
        {
        case Command::Right:
        case Command::Left:
            rotate(command);
            return true;
        case Command::Forward:
            go_foward();
            return map_.is_out_of_bound(location_.point_);
        default:
            break;
        }

        return false;
    }

    Map map_;
    Location location_;
    vector<Command> commands_;
    bool is_failed;
};


istringstream read_line(istream& in)
{
    string line;
    getline(in, line);
    istringstream is(line);
    return is;
}

void robot_walking(istream& in, ostream& out)
{
    int width = 0;
    int height = 0;

    int x = 0;
    int y = 0;
    char direction[1];
    char command[1];

    istringstream is = read_line(in);
    is >> width >> height;

    Map map(width, height);
    while (!in.eof())
    {
        // get position
        is = read_line(in);
        is >> x >> y >> direction;
        Location location(Point(x, y), parse_location(direction));

        // get command
        is = read_line(in);
        vector<Command> commands;
        while (!is.eof())
        {
            is >> command;
            commands.push_back(parse_command(command));
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
        out << endl;
    }
}


int main()
{
    robot_walking(cin, cout);

    /*std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    robot_walking(input, output);*/

    return 0;
}

/*
Sample Input
5 3
1 1 E
RFRFRFRF
3 2 N
FRRFLLFFRRFLL
0 3 W
LLFFFLFLFL
Sample Output
1 1 E
3 3 N LOST
2 3 S
*/