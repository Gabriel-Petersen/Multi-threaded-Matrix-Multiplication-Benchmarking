#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

#include <time_measurer.h>
#include <big_matrix.h>

using namespace std;
namespace fs = std::filesystem;

void saveResults(const string& filename, const string& method, int size, double time) 
{
    fs::create_directories("benchmarks");
    
    bool exists = fs::exists(filename);
    ofstream file(filename, ios::app);
    
    if (!exists)
        file << "Method,Size,Time_ms\n";
    
    file << method << "," << size << "," << time << "\n";
    file.close();
}

int main() 
{
    TimeMeasurer tm;
    string csvPath = "benchmarks/results.csv";
    
    vector<int> sizes = {1000, 2500, 5000, 7500};
    
    struct TestMode {
        bigmatrices::MultiplicationMode mode;
        string name;
    };

    vector<TestMode> modes = {
        {bigmatrices::MultiplicationMode::Trivial, "Trivial"},
        {bigmatrices::MultiplicationMode::TransposedTrivial, "TransposedTrivial"},
        {bigmatrices::MultiplicationMode::Blocking, "Blocking"},
        {bigmatrices::MultiplicationMode::TransposedBlocking, "TransposedBlocking"},
        {bigmatrices::MultiplicationMode::Parallel, "Parallel"},
        {bigmatrices::MultiplicationMode::TransposedParallel, "TransposedParallel"}
    };

    for (int s : sizes) 
    {
        cout << "\n--- Testing Size: " << s << "x" << s << " ---\n";
        
        bigmatrices::BigSquareMatrix matA(s);
        bigmatrices::BigSquareMatrix matB(s);
        bigmatrices::BigSquareMatrix matC(s);
        
        matA.fillWithRand();
        matB.fillWithRand();

        for (auto& m : modes) 
        {
            matC.zero();
            double time = tm.measure(false, bigmatrices::multiplyMatrices, ref(matA), ref(matB), ref(matC), m.mode);
            
            cout << m.name << ": " << time << " ms\n";
            saveResults(csvPath, m.name, s, time);
        }
    }

    cout << "\nBenchmark complete! Results saved to " << csvPath << endl;
    return 0;
}