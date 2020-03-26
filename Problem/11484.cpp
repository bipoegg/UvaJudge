#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <iterator>
#include <memory>

template <class Container>
void split_value(const std::string& str, Container& cont)
{
	std::istringstream iss(str);
	std::copy(std::istream_iterator<std::string>(iss),
		std::istream_iterator<std::string>(),
		std::back_inserter(cont));
}

#define START "<n "
#define END "</n>"

enum class Instruction
{
	None = -1,
	FirstChild = 0,
	Next,
	Previous,
	Parent,
	Count
};

struct Node {
	Node(std::string value)
		: value_(value)
		, parent_(nullptr)
		, first_child_(nullptr)
		, next_(nullptr)
		, previous_(nullptr)
		, got_close_symbol(false)
	{

	}
	bool got_close_symbol;
	std::string value_;
	std::shared_ptr<Node> parent_;
	std::shared_ptr<Node> first_child_;
	std::shared_ptr<Node> next_;
	std::shared_ptr<Node> previous_;
}; 

class DocumentModel
{
public:
	DocumentModel(int case_index) 
		: case_index_(case_index)
		, current_node_(nullptr)
		, root_(nullptr)
	{
	}

	virtual ~DocumentModel() {}

	bool find_node_symbol(const std::string& document, const std::string& symbol)
	{
		bool success = false;

		std::size_t symbol_found = document.find(symbol);
		if (symbol_found != std::string::npos)
		{
			success = true;
		}

		return success;
	}

	void handle_sibling_node(std::shared_ptr<Node> current_node, std::shared_ptr<Node> new_node)
	{
		new_node->parent_ = current_node != nullptr ? current_node_->parent_ : nullptr;
		new_node->previous_ = current_node_;
		if (current_node_ != nullptr)
		{
			current_node_->next_ = new_node;
			current_node_->got_close_symbol = true;
		}
	}

	bool handle_is_child_node(std::shared_ptr<Node> current_node, std::shared_ptr<Node> new_node)
	{
		bool is_child_node = false;
		if (current_node && !current_node->got_close_symbol)
		{
			new_node->parent_ = current_node;
			if (current_node->first_child_ == nullptr)
			{
				current_node->first_child_ = new_node;
			}
			is_child_node = true;
		}
		return is_child_node;
	}

	void add_node(const std::string& document)
	{
		// handle start
		if (find_node_symbol(document, START))
		{
			std::string value = parse_node(document);
			std::shared_ptr<Node> new_node = std::make_shared<Node>(value);

			if (current_node_ == nullptr)
			{
				// keep root only first time
				root_ = new_node;
			}

			if (!handle_is_child_node(current_node_, new_node))
			{
				handle_sibling_node(current_node_, new_node);
			}
			current_node_ = new_node;
		}
		// handle end
		else if (find_node_symbol(document, END))
		{
			std::shared_ptr<Node> node = current_node_;
			// mark first parent which have no end yet
			while (node->got_close_symbol)
			{
				node = node->parent_;
				if (node == nullptr)
				{
					break;
				}
			}

			if (node != nullptr)
			{
				node->got_close_symbol = true;
			}
		}
	}

	void add_instruction(const std::string& instruction)
	{
		Instruction operation = Instruction::None;
		if ("first_child" == instruction)
		{
			operation = Instruction::FirstChild;
		}
		else if ("next_sibling" == instruction)
		{
			operation = Instruction::Next;
		}
		else if ("previous_sibling" == instruction)
		{
			operation = Instruction::Previous;
		}
		else if ("parent" == instruction)
		{
			operation = Instruction::Parent;
		}

		if (operation > Instruction::None && operation < Instruction::Count)
		{
			operations_.push_back(operation);
		}
	}

	void operate_instruction(std::ostream& out)
	{
		out << "Case " << case_index_ << ":" << std::endl;

		std::shared_ptr<Node> node = root_;
		std::shared_ptr<Node> next_node = root_;
		for (auto operation : operations_)
		{
			switch (operation)
			{
			case Instruction::FirstChild:
				next_node = node->first_child_;
				break;
			case Instruction::Next:
				next_node = node->next_;
				break;
			case Instruction::Previous:
				next_node = node->previous_;
				break;
			case Instruction::Parent:
				next_node = node->parent_;
				break;
			}

			if (next_node != nullptr)
			{
				node = next_node;
			}

			out << node->value_ << std::endl;
		}
	}

private:
	std::string parse_node(const std::string& data)
	{
		std::vector<std::string> words;
		split_value(data, words);
		for (auto value : words)
		{
			std::string delimiter = "value='";
			std::size_t pos = value.find(delimiter);
			if (pos != std::string::npos)
			{
				value.erase(0, pos + delimiter.length());
				int length = value.size();
				value.erase(length - 2, length - 1);
				return value;
			}
		}
		return "";
	}
	int case_index_;
	std::shared_ptr<Node> root_;
	std::shared_ptr<Node> current_node_;
	std::vector<Instruction> operations_;
	int depth_;
};

std::istringstream read_line(std::istream& in)
{
	std::string line;
	std::getline(in, line);
	std::istringstream is(line);
	return is;
}

void document_object_model(std::istream& in, std::ostream& out)
{
	try
	{
		int case_index = 0;
		while (!in.eof())
		{
			int line_count = 0;
			std::istringstream is = read_line(in);
			is >> line_count;
			++case_index;

			if (line_count == 0)
			{
				break;
			}
			else
			{
				DocumentModel document_model(case_index);
				for (int i = 0; i < line_count; ++i)
				{
					std::string line;
					std::getline(in, line);
					document_model.add_node(line);
				}

				int instruction_count = 0;
				std::istringstream is = read_line(in);
				is >> instruction_count;

				for (int i = 0; i < instruction_count; ++i)
				{
					std::string line;
					std::getline(in, line);
					document_model.add_instruction(line);
				}

				document_model.operate_instruction(out);
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
	document_object_model(std::cin, std::cout);

	/*std::ifstream input("input.txt");
	std::ofstream output("output.txt");

	document_object_model(input, output);*/
	return 0;
}

/*
Sample Input
4
<n value='parent'>
<n value='child'>
</n>
</n>
2
next_sibling
first_child
0
Sample Output
Case 1:
parent
child
*/

