/*
 * This is just a demo for memory representation of floating point integers
 */

#include <fstream>
#include <vector>
#include <string>
#include <iostream>

using t = float;

int main (int argc, char ** argv)
{
	const std::string FN = "./file.raw";
	const std::vector<t> vf = {1.1, 2.2, 3.3, 4.4, 5.5};
	const size_t NUMEL =  vf.size();

	std::ofstream ofs(FN, std::ios::binary);
	ofs.write((const char*) vf.data(), NUMEL * sizeof(t));
	ofs.close();

	std::ifstream ifs(FN, std::ios::binary);
	auto pf = new t[NUMEL];

	ifs.read((char*) pf, NUMEL * sizeof(t));
	for (int i=0; i<NUMEL; i++)
	{
		std::cout << pf[i] << "\t";
	}
	std::cout << std::endl;

	ifs.close();
	delete [] pf;

	const std::string CMD = "shed " + FN;
	system(CMD.c_str());
	return 0;
}


struct CommandReq
{
	char    sMark[6];  //start flag\r\n*KW        6s
	short   nPackLen;  //packet length            h
	short   nFlag;     //command ID 0x0002        h
	int     nGisIp;    //GIS port                 i
	short   nPort;     //GIS Port                 h
	char    sData[50]; //command string           50s
	char    sEnd[2];   //end flag "\r\n"          2s
};

format_ = "6s h h i h 50s 2s"
