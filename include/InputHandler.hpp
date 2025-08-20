#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <string_view>
#include <ranges>
#include <optional>
#include <sstream>

class InputHandler
{
public:

	InputHandler();
	void cleanscr() const;
	bool check_args(const std::vector<std::string>& command, int size, std::string_view command_name);
	std::optional<std::vector<std::string>> parse_input(const std::string& input);


};

