#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[])
{
    string target_executable_file = argv[1];

    cout << "\n////////////////////////////////////////////////////////////////////////////////\n#################"
            "##############################################################\n\nExecuting "
            "target "
            "program: \n"
         << target_executable_file << " ... "
         << "\n";
    cout << "\n-------------------------------------------------------------------------------\n\n";

    auto start_time = chrono::high_resolution_clock::now();

    int target_return_code = system(("\"" + target_executable_file + "\"").c_str());

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end_time - start_time;

    cout << "\n\n-------------------------------------------------------------------------------\n";
    cout << "\nProcess returned " << target_return_code << " (0x" << hex << target_return_code << ")";
    cout << "    Execution time: " << fixed << setprecision(3) << duration.count() << " s" << "\n";

    system("pause");

    cout << "\n###############################################################################"
            "\n\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"
            "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";

    return 0;
}
