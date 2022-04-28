#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <math.h>
using namespace std;

//void processFile(ifstream&);
int Reg_to_int(string); //turns register string into an int. ex: R1 -> 1
int binaryToint(int);
void Rtype_execute(string, int (&regs)[32], int (&mems)[249],int& b, int& instruction_count, int& cycle_count, ofstream&); //executes  add, sub or slt instructions
void Itype_execute(string, int (&regs)[32], int(&mems)[249],int& b,int& s, int& instruction_count, int& cycle_count, ofstream&); //executes  addi, beq, sw or lw instructions

struct Rtype { //Rtype structure
	int op, rs, rt, rd, shamt, function;
};

struct Itype { //Itype structure
	int op, rs, rt, address;
};

struct Jtype {// Jtype structure
	int op, address;
};

int main() {
	//Open input file:
	int run = 1;
	while (run==1) {
		int instruction_count = 1;
		int cycle_count = 1;
		cout << "Please enter the input file name: ";
		string in;
		cin >> in;
		cout << "Please enter the output file name: ";
		string out;
		cin >> out;
		ifstream input;
		input.open(in);

		//Check if input file is opened:
		if (!input.is_open()) {
			cout << "File not found! Exiting!" << endl;
			exit(EXIT_FAILURE);
		}

    ofstream output;
		output.open(out);

		//Check if output file is opened:
		if (!output.is_open()) {
			cout << "File not found! Exiting!" << endl;
			exit(EXIT_FAILURE);
		}


		string temp;
		vector<string> str; //string vector to store input file data
		int REG[32]; // Registers
		int MEM[249]; //Memory every 4 bytes

		for (int k = 0; k < 32; k++) {
			REG[k] = 0;
		}
		for (int k = 0; k < 249; k++) {
			MEM[k] = 0;
		}

		int i = 0;
		while (input >> temp) { // reads from file and stores all text into string str
			str.push_back(temp);
			i++;
		}
		int j;
		for (i = 0; i < str.size(); i++) {// loops through string str;
			if (str[i] == "REGISTERS") {// stores Register value into REG
				cout << "REGISTERS:" << endl;
				j = i + 1;
				while (str[j] != "MEMORY") {
					REG[Reg_to_int(str[j])] = stoi(str[j + 1]);
					cout << "R" << j << " " << REG[Reg_to_int(str[j])] << endl;
					j = j + 2;
				}
			}
			else if (str[i] == "MEMORY") {// stores value into MEM
				cout << "MEMORY:" << endl;
				j = i + 1;
				while (str[j] != "CODE") {
					MEM[stoi(str[j]) / 4] = stoi(str[j + 1]);
					cout << str[j] << " " << MEM[stoi(str[j]) / 4] << endl;
					j = j + 2;
				}
			}
			else if (str[i] == "CODE") { // gets the position where code starts in string str
				cout << "CODE:" << endl;
				j = i + 1;
			}
		}


		//   for (int k = 0; k<32 ; k++){
		// 	  cout << "REG[" << k << "] = " << REG[k] << endl;
		//   }
			// Go through the code and execute the appropriate type of function.
		int s = j;
		int b = 0;
		int next = 0;
    // int cycle_count=1;
    // int instruction_count = 1;
		for (; j < str.size(); j++) {
			b = s - j;
			string opcode = str[j].substr(0, 6);
			if (s == j && next == 1) {
				s++;
				next = 0;
				cout << "NEXT:";
			}
			if (s == j)
				s++;
			else
				next = 1;
			cout << "\t";
			if (opcode == "000000") { //opcode 0 -> Add, Sub, Slt
				//execute Rtype function
         //output << "C#" << cycle_count++ << "  I" << instruction_count << "-IF" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-ID" << endl;
				 Rtype_execute(str[j], REG, MEM, b, instruction_count, cycle_count, output);
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-EX" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-MEM" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-WB" << endl;
			}
			else if (opcode == "100011") {//opcode 35 -> lw
				//execute Itype function
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-IF" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-ID" << endl;
				 Itype_execute(str[j], REG, MEM, b, s, instruction_count, cycle_count, output);
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-EX" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-MEM" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-WB" << endl;

			}
			else if (opcode == "101011") {//opcode 43 -> sw
				//execute Itype function
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-IF" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-ID" << endl;
				 Itype_execute(str[j], REG, MEM, b, s, instruction_count, cycle_count, output);
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-EX" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-MEM" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-WB" << endl;

			}
			else if (opcode == "000101") {//opcode 5 -> BNE
				//execute Itype function
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-IF" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-ID" << endl;
				 Itype_execute(str[j], REG, MEM, b, s, instruction_count, cycle_count, output);
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-EX" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-MEM" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-WB" << endl;

			}
			else if (opcode == "001000") {//opcode 8 -> addi
				//execute Jtype function
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-IF" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-ID" << endl;
				 Itype_execute(str[j], REG, MEM, b, s, instruction_count, cycle_count, output);
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-EX" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-MEM" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-WB" << endl;

			}
			else if (opcode == "000100") {//opcode 4 -> BEQ
				//execute Itype function
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-IF" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-ID" << endl;
				 Itype_execute(str[j], REG, MEM, b, s, instruction_count, cycle_count, output);
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-EX" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-MEM" << endl;
        // output << "C#" << cycle_count++ << "  I" << instruction_count << "-WB" << endl;

			}
			else {
				cout << "There is an instruction that is not supported! Exiting Application!" << endl;
				exit(EXIT_FAILURE);
			}
      //instruction_count++;
		}

		input.close();
		cout <<endl<< "Would you like to run again? (1 for yes, 0 for no)"<<endl;
		cin >> run;

		// ofstream output;
		// output.open(out);

		// //Check if input file is opened:
		// if (!output.is_open()) {
		// 	cout << "File not found! Exiting!" << endl;
		// 	exit(EXIT_FAILURE);
		// }
		output << "REGISTERS" << endl;
		for (int i = 0; i < 32; i++) {
			if (REG[i] != 0)
				output << "R" << i << " " << REG[i]<<endl;
		}
		output << "MEMORY" << endl;
		for (int i = 0; i < 249; i++) {
			if (MEM[i] != 0)
				output <<  i*4 << " " << MEM[i]<<endl;
		}
		output.close();
	}
}


void Itype_execute(string machine_code, int(&regs)[32], int(&mems)[249],int& b,int& s, int& instruction_count, int& cycle_count, ofstream& output) {
	struct Itype instruction;
	int address = 0;
	instruction.op = stoi(machine_code.substr(0, 6));
	instruction.rs = stoi(machine_code.substr(6, 5));
	instruction.rt = stoi(machine_code.substr(11, 5));
	if (b == 0) output << "C#" << cycle_count++ << "  I" << instruction_count << "-IF" << endl;
	if (machine_code.compare(16, 1,"1")==0 ){
		
		for (int i = 15; i >= 0; i--) {
			if (machine_code.compare(16 + i, 1, "1") == 0)
				machine_code[16 + i] = '0';
			else
				machine_code[16 + i] = '1';
		}
		if (machine_code.compare(31,1,"1")==0) {
			machine_code[31] = '2';
			for (int i = 15; i >= 0; i--) {
				if (machine_code.compare(16 + i, 1, "2") == 0) {
					machine_code[16 + i] = '0';
					if (machine_code.compare(16 + i + 1, 1, "1") == 0) {
						machine_code[16 + i+1] = '2';
					}
					else
						machine_code[16 + i + 1] = '1';
				}
				else
					machine_code[16 + i] = '1';
			}
		}
		instruction.address = stoi(machine_code.substr(16, 16));
		address = (binaryToint(instruction.address) + 1) * -1;
	}
	else {
		instruction.address = stoi(machine_code.substr(16, 16));
		address = binaryToint(instruction.address);
	}

	

	if (instruction.op == 100011) { // lw function
		if (b == 0) output << "C#" << cycle_count++ << "  I" << instruction_count << "-ID" << endl;
		cout << "LW ";
		cout << "R" << binaryToint(instruction.rt) << ", " << address <<"(R"<< binaryToint(instruction.rs)<<")"<<endl;
		if (b == 0) {
			regs[binaryToint(instruction.rt)] = mems[(regs[binaryToint(instruction.rs)] + address)/4];
			output << "C#" << cycle_count++ << "  I" << instruction_count << "-EX" << endl; // EX, MEM, and WB are performed at the same time
        	output << "C#" << cycle_count++ << "  I" << instruction_count << "-MEM" << endl;
        	output << "C#" << cycle_count++ << "  I" << instruction_count << "-WB" << endl;
			output << "LW" << endl;
		}
	}
	else if (instruction.op == 101011) { // sw function
		if (b == 0) output << "C#" << cycle_count++ << "  I" << instruction_count << "-ID" << endl;
		cout << "SW ";
		cout << "R" << binaryToint(instruction.rt) << ", " << address << "(R" << binaryToint(instruction.rs) << ")" << endl;
		if (b == 0) {
			mems[(regs[binaryToint(instruction.rs)] + address) / 4] = regs[binaryToint(instruction.rt)];
			output << "C#" << cycle_count++ << "  I" << instruction_count << "-EX" << endl; // EX and MEM are performed at the same time
        	output << "C#" << cycle_count++ << "  I" << instruction_count << "-MEM" << endl;
			output << "SW" << endl;
		}
	}
	else if (instruction.op == 1000) { // addi function
		if (b == 0) output << "C#" << cycle_count++ << "  I" << instruction_count << "-ID" << endl;
		//output << "addi" << endl;
		cout << "ADDI ";
		cout << "R" << binaryToint(instruction.rt) << ", " << "R" << binaryToint(instruction.rs) << ", "<< address << endl;
		if (b == 0) {
			regs[binaryToint(instruction.rt)] = regs[binaryToint(instruction.rs)] + address;
			 output << "C#" << cycle_count++ << "  I" << instruction_count << "-EX" << endl; //Ex, and WB are performed at the same time ^^
        	 output << "C#" << cycle_count++ << "  I" << instruction_count << "-WB" << endl;
			 output << "ADDI" << endl;
		}
	}
	else if (instruction.op == 101) { // bne function
		if (b == 0) output << "C#" << cycle_count++ << "  I" << instruction_count << "-ID" << endl;
		cout << "BNE ";
		cout << "R" << binaryToint(instruction.rs) << ", " << "R" << binaryToint(instruction.rt) << ", NEXT"  << endl;
		if (b == 0) {
			if (binaryToint(instruction.rs) - binaryToint(instruction.rt))
				s += address;
				output << "C#" << cycle_count++ << "  I" << instruction_count << "-EX" << endl;
				output << "BNE" << endl;
		}
	}
	else if (instruction.op == 100) { // beq function
		if (b == 0) output << "C#" << cycle_count++ << "  I" << instruction_count << "-ID" << endl;
		cout << "BEQ " ;
		cout << "R" << binaryToint(instruction.rs) << ", " << "R" << binaryToint(instruction.rt) << ", NEXT" << endl;
		if (b == 0) {
			if (binaryToint(instruction.rs) - binaryToint(instruction.rt))
				s += address;
				output << "C#" << cycle_count++ << "  I" << instruction_count << "-EX" << endl;
				output << "BEQ" << endl;
		}
	}
	else {
		cout << "There is an instruction that is not supported! Exiting Application!" << endl;
		exit(EXIT_FAILURE);
	}
	if (b == 0) instruction_count++;
}

void Rtype_execute(string machine_code, int(&regs)[32], int(&mems)[249],int& b, int& instruction_count, int& cycle_count, ofstream& output) {
	struct Rtype instruction;
	instruction.op = stoi(machine_code.substr(0, 6));
	instruction.rs = stoi(machine_code.substr(6, 5));
	instruction.rt = stoi(machine_code.substr(11, 5));
	instruction.rd = stoi(machine_code.substr(16, 5));
	instruction.shamt = stoi(machine_code.substr(21, 5));
	instruction.function = stoi(machine_code.substr(26, 6));
	if (b == 0) output << "C#" << cycle_count++ << "  I" << instruction_count << "-IF" << endl;
	if (instruction.function == 100000) { // add function
		if (b == 0) output << "C#" << cycle_count++ << "  I" << instruction_count << "-ID" << endl;
		//output << "add" << endl;
		cout << "ADD ";
		cout << "R" << binaryToint(instruction.rd) << ", " << "R" << binaryToint(instruction.rs) << ", "<<"R"  << binaryToint(instruction.rt) << endl;
		if (b == 0) {
			regs[binaryToint(instruction.rd)] = regs[binaryToint(instruction.rt)] + regs[binaryToint(instruction.rs)];
			 output << "C#" << cycle_count++ << "  I" << instruction_count << "-EX" << endl; //Ex, and WB are performed at the same time ^^
        	 output << "C#" << cycle_count++ << "  I" << instruction_count << "-WB" << endl;
			 output << "ADD" << endl;
		}
	}
	else if (instruction.function == 100010) { // sub function
		if (b == 0) output << "C#" << cycle_count++ << "  I" << instruction_count << "-ID" << endl;
		cout << "SUB " ;
		cout << "R" << binaryToint(instruction.rd) << ", " << "R" << binaryToint(instruction.rs) << ", " << "R" << binaryToint(instruction.rt) << endl;
		if (b == 0) {
			regs[binaryToint(instruction.rd)] = regs[binaryToint(instruction.rs)] - regs[binaryToint(instruction.rt)];
			 output << "C#" << cycle_count++ << "  I" << instruction_count << "-EX" << endl; //Ex, and WB are performed at the same time ^^
        	 output << "C#" << cycle_count++ << "  I" << instruction_count << "-WB" << endl;
			 output << "SUB" << endl;
		}
		
	}
	else if (instruction.function == 101010) { // slt function
		if (b == 0) output << "C#" << cycle_count++ << "  I" << instruction_count << "-ID" << endl;
		cout << "SLT " ;
		cout << "R" << binaryToint(instruction.rd) << ", " << "R" << binaryToint(instruction.rs) << ", " << "R" << binaryToint(instruction.rt) << endl;
		if (b == 0) {
			if (regs[binaryToint(instruction.rs)] < regs[binaryToint(instruction.rt)]) {
				regs[binaryToint(instruction.rd)] = 1;
				output << "C#" << cycle_count++ << "  I" << instruction_count << "-EX" << endl; //Ex, and WB are performed at the same time ^^
        	 	output << "C#" << cycle_count++ << "  I" << instruction_count << "-WB" << endl;
				 output << "SLT" << endl;
			}
			else{
				regs[binaryToint(instruction.rd)] = 0;
				output << "C#" << cycle_count++ << "  I" << instruction_count << "-EX" << endl; //Ex, and WB are performed at the same time ^^
        	 	output << "C#" << cycle_count++ << "  I" << instruction_count << "-WB" << endl;
				 output << "SLT" << endl;
			}
		}
		
	}
	else {
		cout << "There is an instruction that is not supported! Exiting Application!" << endl;
		exit(EXIT_FAILURE);
	}
	if (b == 0) instruction_count++;
}


int binaryToint(int n) // this function was taken from geeksforgeeks.org written by GeekforGeeks
{
	int num = n;
	int dec_value = 0;

	// Initializing base value to 1, i.e 2^0
	int base = 1;

	int temp = num;
	while (temp) {
		int last_digit = temp % 10;
		temp = temp / 10;

		dec_value += last_digit * base;

		base = base * 2;
	}

	return dec_value;
}

int Reg_to_int(string reg) {
	int r;
	if (reg == "R0") {
		r = 0;
	}
	else if (reg == "R1") {
		r = 1;
	}
	else if (reg == "R2") {
		r = 2;
	}
	else if (reg == "R3") {
		r = 3;
	}
	else if (reg == "R4") {
		r = 4;
	}
	else if (reg == "R5") {
		r = 5;
	}
	else if (reg == "R6") {
		r = 6;
	}
	else if (reg == "R7") {
		r = 7;
	}
	else if (reg == "R8") {
		r = 8;
	}
	else if (reg == "R9") {
		r = 9;
	}
	else if (reg == "R10") {
		r = 10;
	}
	else if (reg == "R11") {
		r = 11;
	}
	else if (reg == "R12") {
		r = 12;
	}
	else if (reg == "R13") {
		r = 13;
	}
	else if (reg == "R14") {
		r = 14;
	}
	else if (reg == "R15") {
		r = 15;
	}
	else if (reg == "R16") {
		r = 16;
	}
	else if (reg == "R17") {
		r = 17;
	}
	else if (reg == "R18") {
		r = 18;
	}
	else if (reg == "R19") {
		r = 19;
	}
	else if (reg == "R20") {
		r = 20;
	}
	else if (reg == "R21") {
		r = 21;
	}
	else if (reg == "R22") {
		r = 22;
	}
	else if (reg == "R23") {
		r = 23;
	}
	else if (reg == "R24") {
		r = 24;
	}
	else if (reg == "R25") {
		r = 25;
	}
	else if (reg == "R26") {
		r = 26;
	}
	else if (reg == "R27") {
		r = 27;
	}
	else if (reg == "R28") {
		r = 28;
	}
	else if (reg == "R29") {
		r = 29;
	}
	else if (reg == "R30") {
		r = 30;
	}
	else if (reg == "R31") {
		r = 31;
	}
	return r;
}
// void processFile(ifstream& inFile){
// 	string line, word;
// 	istringstream iss;


// 	while (!inFile.eof()) {
// 		// read line by line from the file
// 		getline(inFile, line);
// 		if (inFile.good()) {
// 			//cout << line << endl;
// 			// read word by word from the line
// 			iss.clear(); // clear out state
// 			iss.str(line);
// 			while (iss.good()) {
// 				iss >> word;
// 				cout << word << endl;
// 			}	
// 		}
// 	}

// }