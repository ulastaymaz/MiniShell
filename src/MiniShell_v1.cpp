#include "../include/FileFunctions.hpp"
#include "../include/ShellManager.hpp"



int main() {


try {
	ShellManager run_program;
	run_program.run_shell();
	return 0;
}
catch (const std::exception& e){
	std::cout << "ERROR: " << e.what();
	return 0;
}
	

}