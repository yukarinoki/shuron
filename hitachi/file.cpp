#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ofstream ofs("./seq.fa");
    
    ofs << ">seq1" << std::endl;
    ofs << "ACAGTGTGACTTAGCTGTAC" + str(20) << std::endl;

    ofs << ">seq2" << std::endl;
    ofs << "CCCGGCTTTTATGAG" << std::endl;

    return 0;
}