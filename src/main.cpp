/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Arquivo principal
 *
 *        Version:  1.0
 *        Created:  10-10-2013 16:57:17
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Psycho Mantys, Jonathas
 *
 * =====================================================================================
 */
#include	<hurricane/config.hpp>

//g++ -lboost_program_options -lboost_system -std=c++0x -Wall (Maybe...)
#include	<iostream>
#include	<fstream>
#include	<vector>
#include	<string>

// Boost...
#include	<boost/program_options.hpp>

using std::vector;
using std::string;
using std::unique_ptr;
using std::ofstream;
using std::ifstream;
using std::istream;
using std::ostream;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;

namespace po=boost::program_options;


int main( int argc, char* argv[]){
	const char *config_file_name="config.cfg";
	po::variables_map vm;

	unique_ptr<ofstream> proxy_output_file;
	unique_ptr<ifstream> proxy_input_file;

	po::options_description parameters_cmd_line;
	try{
		po::options_description parameters_config_file;
		po::options_description parameters_environment;

		po::options_description parameters_input("Opções de entrada");

		// Começa a tratar opções passadas ao programa
		parameters_input.add_options()
			("help,h", "produce help message")
			("input,i", po::value< vector<string> >(), "Arquivo de entrada para o programa")
			("output,o", po::value< vector<string> >(), "Arquivo de saída")
			;

		parameters_cmd_line.add(parameters_input);

		po::positional_options_description positional_opt;
		positional_opt.add("input", -1);

		po::store( po::parse_environment(parameters_environment,"HURRICANEC_"), vm);
		po::store( po::command_line_parser(argc, argv).options(parameters_cmd_line).positional(positional_opt).run(), vm);
		ifstream config_file(config_file_name);

		if( config_file ){
			po::store( po::parse_config_file(config_file, parameters_cmd_line), vm);
		}else{
			// clog << "Configuration file \""<<config_file_name<<"\" not found!"<<endl;
		}

		po::notify(vm);

		// Caso seja solicitado ajuda
		if( vm.count("help") ){
			cout << parameters_cmd_line << "\n";
			return 0;
		}
		if( vm.count("output") ){
			proxy_output_file.reset( new ofstream(vm["output"].as<vector<string> >()[0].c_str()) );
		}
		if( vm.count("input") ){
			proxy_input_file.reset( new ifstream(vm["input"].as<vector<string> >()[0].c_str()) );
		}
		// Final do tratamento de opções
	}catch( po::error &e ){
		cerr<< argv[0]<<": "<<e.what()<<endl<<endl;
		cerr<< parameters_cmd_line <<endl;
		return 1;
	}

	istream &input_file=( proxy_input_file.get() )?(*proxy_input_file):cin;
	ostream &output_file=( proxy_output_file.get() )?(*proxy_output_file):cout;

}


