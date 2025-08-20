#include "../include/ShellManager.hpp"
#include <ranges>
using OPR = FileFunctions::OpResult;
using command_handler = std::function<FileFunctions::OpResult(const std::vector<std::string>&)>;


ShellManager::ShellManager() {
    auto register_cmd = [this](std::string_view name, std::string_view aliases,
        std::string_view description, int args_num, command_handler handler) {
            //_COMMANDS cmd{ name, aliases, description, args_num, handler };
            //commandMap.insert({ std::string(aliases), cmd });
            commandMap.emplace(std::string(aliases), _COMMANDS(name, aliases, description, args_num, handler));
     };


    //maybe i need to move those register_cmd function calls to hpp file
    register_cmd("list", "-l",
        "List files in current folder: -l", 1, [this](const std::vector<std::string>& cmd) {
            //it can be way shorter
            if (!input_handler.check_args(cmd, 1, cmd[0]))
                return OPR::fail({ std::make_error_code(std::errc::invalid_argument) });
            return file_handler.display_files(path);
        });

    register_cmd("list_dir", "-ld",
        "List directories in current folder: -ld", 1, [this](const std::vector<std::string>& cmd) {
            if (!input_handler.check_args(cmd, 1, cmd[0]))
                return OPR::fail({ std::make_error_code(std::errc::invalid_argument) });
            return file_handler.display_dirs(path);
        });

    register_cmd("enter", "-e",
        "Enter a file in the current folder: -e file_name", 2, [this](const std::vector<std::string>& cmd) {
            if (!input_handler.check_args(cmd, 2, cmd[0]))
                return OPR::fail({ std::make_error_code(std::errc::invalid_argument) });
            return file_handler.enter_directory(path, cmd[1]);
        });

    register_cmd("up", "-u",
        "Go to parent directory: -u", 1, [this](const std::vector<std::string>& cmd) {
            if (!input_handler.check_args(cmd, 1, cmd[0]))
                return OPR::fail({ std::make_error_code(std::errc::invalid_argument) });
            return file_handler.parent_directory(path);
        });

    register_cmd("create", "-cr",
        "Create a directory in the current folder: -cr folder_name", 2, [this](const std::vector<std::string>& cmd) {
            if (!input_handler.check_args(cmd, 2, cmd[0]))
                return OPR::fail({ std::make_error_code(std::errc::invalid_argument) });
            return file_handler.create_directory(path, cmd[1]);
        });

    register_cmd("delete_dir", "-dr",
        "Delete directory in the current folder: -dr directory_name", 2, [this](const std::vector<std::string>& cmd) {
            if (!input_handler.check_args(cmd, 2, cmd[0]))
                return OPR::fail({ std::make_error_code(std::errc::invalid_argument) });
            return file_handler.delete_folder(path, cmd[1]);
        });

    register_cmd("delete", "-d",
        "Delete file in the current folder: -dr file_name", 2, [this](const std::vector<std::string>& cmd) {
            if (!input_handler.check_args(cmd, 2, cmd[0]))
                return OPR::fail({ std::make_error_code(std::errc::invalid_argument) });
            return file_handler.delete_file(path, cmd[1]);
        });

    register_cmd("rename", "-rn",
        "Rename a file: -rn file_name", 3, [this](const std::vector<std::string>& cmd) {   
            if (!input_handler.check_args(cmd, 3, cmd[0]))
                return OPR::fail({ std::make_error_code(std::errc::invalid_argument) });
            return file_handler.rename_file(path, cmd[1], cmd[2]);
        });

    register_cmd("copy", "-cp",
        "Copy a file: -cp file_name", 2, [this](const std::vector<std::string>& cmd) {   
            if (!input_handler.check_args(cmd, 2, cmd[0]))
                return OPR::fail({ std::make_error_code(std::errc::invalid_argument) });
            return file_handler.copy_file(path, copy_path, cmd[1]);
        });

    register_cmd("paste", "-pt",
        "Paste a copied file: -rn file_name", 1, [this](const std::vector<std::string>& cmd) {
            if (!input_handler.check_args(cmd, 1, cmd[0]))
                return OPR::fail({ std::make_error_code(std::errc::invalid_argument) });
            return file_handler.paste_file(path, copy_path);
        });

    register_cmd("file_info", "-info",
        "Info of a file: -rn file_name", 2, [this](const std::vector<std::string>& cmd) {
            if (!input_handler.check_args(cmd, 2, cmd[0]))
                return OPR::fail({ std::make_error_code(std::errc::invalid_argument) });
            return file_handler.info_file(path, cmd[1]);
        });
             
    register_cmd("help", "-help",
        "Show help: -help", 1, [this](const std::vector<std::string>&) {
            //return file_handler.help();
            std::cout << std::format("{:<10} # {:<10} # {}", "Function", "Command", "Description") << std::endl;
            std::cout << std::format("#####################################") << std::endl;
            for (auto it : commandMap) {
                std::cout << std::format("{:<10} # {:<10} # {} \n", it.second.name, it.second.aliases, it.second.description);

            }
            return OPR::scs();
        });
        
}



void ShellManager::run_shell() {
	std::string command;
	while (true) {
		std::cout << path << " $ ";
		std::getline(std::cin, command);
		if (command == "exit") {
			break;
		}
		handle_command(command);
	}
}

void ShellManager::handle_command(const std::string& command) {
    auto commands = input_handler.parse_input(command);
    //if empty command entered, just 1 line;
    if (!commands.has_value()) {
        std::cerr << std::endl;
        return;
    }

    std::string cmd = commands.value()[0];
    auto it = commandMap.find(cmd);
    if (it != commandMap.end()) {
        OPR result = it->second.handler(commands.value());
        if (!result.success) {
            std::cerr << result.err_c.message() << std::endl;
            return;
        }
    }
    else {
        std::cerr << "Invalid command: " << cmd << std::endl;
    }

}



