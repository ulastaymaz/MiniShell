#include "../include/InputHandler.hpp"



InputHandler::InputHandler() {};

void InputHandler::cleanscr() const {
#if defined _WIN32
    system("cls");

#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	std::cout << "\033[2J\033[1;1H";
#elif defined (__APPLE__)
	std::cout << "\033[2J\033[1;1H";
#endif
}



std::optional<std::vector<std::string>> InputHandler::parse_input(const std::string& input) {
	std::vector<std::string> commands;
	std::string command;
	std::istringstream stream(input);
	while (std::getline(stream, command, ' ')) {
		if (!command.empty()) {
			commands.push_back(command);
		}
	}
	if (!commands.empty()) return commands;
	else return {};
}


bool InputHandler::check_args(const std::vector<std::string>& command, int size, std::string_view command_name) {
	return command.size() == size;

}