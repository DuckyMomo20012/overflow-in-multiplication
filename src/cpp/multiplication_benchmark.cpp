#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

template<typename T> T linearMultiply(T x, long long n) {
    T result = 0;
    for (long long i = 0; i < n; i++) {
        result += x;
    }
    return result;
}

template<typename T> T fastMultiply(T x, long long n) {
    T result = 0;
    T curr = x;
    while (n > 0) {
        if (n % 2 == 1) {
            result += curr;
        }
        curr += curr;
        n /= 2;
    }
    return result;
}

template<typename T> struct BenchmarkResult {
    T linearResult;
    T fastMultiplyResult;
    T expectedResult;
    long double linearError;
    long double fastMultiplyError;
    long double linearTime;
    long double fastMultiplyTime;
};

template<typename T> BenchmarkResult<T> benchmarkPrecision(T x, long long n, T expected) {
    BenchmarkResult<T> result;
    
    // Measure linear multiplication time
    auto start = std::chrono::high_resolution_clock::now();
    result.linearResult = linearMultiply(x, n);
    auto end = std::chrono::high_resolution_clock::now();
    result.linearTime = std::chrono::duration<double>(end - start).count();
    
    // Measure fast multiplication time
    start = std::chrono::high_resolution_clock::now();
    result.fastMultiplyResult = fastMultiply(x, n);
    end = std::chrono::high_resolution_clock::now();
    result.fastMultiplyTime = std::chrono::duration<double>(end - start).count();
    
    result.expectedResult = expected;
    
    // Calculate errors
    result.linearError = std::abs((result.linearResult - result.expectedResult) / result.expectedResult);
    result.fastMultiplyError = std::abs((result.fastMultiplyResult - result.expectedResult) / result.expectedResult);
    
    return result;
}

template<typename T> struct TestCase {
    T x;
    long long n;
    T expectedResult;
};

template<typename T> std::vector< BenchmarkResult<T> > runBenchmark(const std::string& typeName, std::ofstream& csvFile) {
    // For header printing
    std::cout << std::fixed << std::setprecision(8);
    std::cout << "\nBenchmarking " << typeName << " multiplication methods\n";
    std::cout << "--------------------------------------------------\n";
    
    const T x = 0.1;
    std::vector< TestCase<T> > testCases;
    long long curr = 1;
    // Create test cases for powers of 10 from 10 to 10^11
    for (long long exp = 1; exp <= 11; exp++) {
        TestCase<T> tc;
        tc.expectedResult = curr;
        curr = curr * 10;
        tc.x = x;
        tc.n = static_cast<long long>(curr);
        testCases.push_back(tc);
    }
    
    // Initialize counters for summary
    int linearBetterCount = 0;
    int fastMultiplyBetterCount = 0;
    int equalCount = 0;
    double totalLinearTime = 0.0;
    double totalFastMultiplyTime = 0.0;
    
    // Results for categorization
    std::vector< BenchmarkResult<T> > results;
    
    // Print header
    std::cout << std::left << std::setw(15) << "n" 
              << std::setw(20) << "Linear Result" 
              << std::setw(20) << "Fast Result" 
              << std::setw(20) << "Expected" 
              << std::setw(15) << "Linear Error" 
              << std::setw(15) << "Fast Error" 
              << std::setw(15) << "Linear Time" 
              << std::setw(15) << "Fast Time" 
              << "Winner" << std::endl;
    std::cout << std::string(130, '-') << std::endl;
    
    // Run benchmarks for each test case
    for (const auto& tc : testCases) {
        auto result = benchmarkPrecision(tc.x, tc.n, tc.expectedResult);
        results.push_back(result);
        
        std::string winner;
        if (result.linearError < result.fastMultiplyError) {
            winner = "Linear";
            linearBetterCount++;
        } else if (result.fastMultiplyError < result.linearError) {
            winner = "Fast";
            fastMultiplyBetterCount++;
        } else {
            winner = "Equal";
            equalCount++;
        }
        
        // Print results
        std::cout << std::fixed << std::setprecision(10) 
                << std::left << std::setw(15) << tc.n 
                << std::setw(20) << result.linearResult 
                << std::setw(20) << result.fastMultiplyResult 
                << std::setw(20) << result.expectedResult 
                << std::setprecision(10)  // Higher precision for error values
                << std::setw(15) << result.linearError 
                << std::setw(15) << result.fastMultiplyError 
                << std::setprecision(6)   // Different precision for time values
                << std::setw(15) << result.linearTime 
                << std::setw(15) << result.fastMultiplyTime 
                << winner << std::endl;
        // Write to CSV
        csvFile << typeName << "," << tc.n << "," 
                << result.linearResult << "," 
                << result.fastMultiplyResult << "," 
                << result.expectedResult << "," 
                << result.linearError << "," 
                << result.fastMultiplyError << "," 
                << result.linearTime << "," 
                << result.fastMultiplyTime << "," 
                << winner << "\n";
        
        totalLinearTime += result.linearTime;
        totalFastMultiplyTime += result.fastMultiplyTime;
    }
    
    // Print summary
    int totalCases = testCases.size();
    double linearPercentage = (linearBetterCount / static_cast<double>(totalCases)) * 100;
    double fastMultiplyPercentage = (fastMultiplyBetterCount / static_cast<double>(totalCases)) * 100;
    double equalPercentage = (equalCount / static_cast<double>(totalCases)) * 100;
    
    std::cout << "\n" << std::string(100, '=') << std::endl;
    std::cout << "SUMMARY OF ACCURACY COMPARISON FOR " << typeName << std::endl;
    std::cout << std::string(100, '=') << std::endl;
    
    std::cout << "\nPRECISION COMPARISON:" << std::endl;
    std::cout << "Linear provides better accuracy: " << linearBetterCount << " times" << std::endl;
    std::cout << "Fast provides better accuracy: " << fastMultiplyBetterCount << " times" << std::endl;
    std::cout << "Equal accuracy between methods: " << equalCount << " times" << std::endl;
    
    std::cout << "\nPercentage breakdown:" << std::endl;
    std::cout << "Linear better: " << linearPercentage << "%" << std::endl;
    std::cout << "Fast better: " << fastMultiplyPercentage << "%" << std::endl;
    std::cout << "Equal accuracy: " << equalPercentage << "%" << std::endl;
    
    // Performance comparison
    std::cout << "\nPERFORMANCE COMPARISON:" << std::endl;
    std::cout << "Total Linear execution time: " << totalLinearTime << " seconds" << std::endl;
    std::cout << "Total Fast execution time: " << totalFastMultiplyTime << " seconds" << std::endl;
    
    double speedRatio = totalLinearTime / totalFastMultiplyTime;
    std::string speedDescription;
    if (speedRatio > 1.05) {
        speedDescription = "slower than";
    } else if (speedRatio < 0.95) {
        speedDescription = "faster than";
    } else {
        speedDescription = "approximately the same speed as";
    }
    
    std::cout << "Speed ratio: Linear is " << speedRatio << "x " << speedDescription << " Fast Multiply" << std::endl;
    
    // Calculate average times
    double avgLinearTime = totalLinearTime / totalCases;
    double avgFastTime = totalFastMultiplyTime / totalCases;
    std::cout << "Average Linear execution time: " << avgLinearTime << " seconds per operation" << std::endl;
    std::cout << "Average Fast execution time: " << avgFastTime << " seconds per operation" << std::endl;
    
    // Conclusion
    std::cout << "\nCONCLUSION:" << std::endl;
    if (linearBetterCount > fastMultiplyBetterCount) {
        std::cout << "1. Linear method generally provides better accuracy than Fast method for " << typeName << " multiplication" << std::endl;
        std::cout << "   - Linear was more accurate in " << linearBetterCount << " out of " << totalCases 
                  << " test cases (" << linearPercentage << "%)" << std::endl;
    } else if (fastMultiplyBetterCount > linearBetterCount) {
        std::cout << "1. Fast method generally provides better accuracy than Linear method for " << typeName << " multiplication" << std::endl;
        std::cout << "   - Fast was more accurate in " << fastMultiplyBetterCount << " out of " << totalCases 
                  << " test cases (" << fastMultiplyPercentage << "%)" << std::endl;
    } else {
        std::cout << "1. Both methods provided similar levels of accuracy overall" << std::endl;
    }
    
    std::cout << "\n3. Performance Cost:" << std::endl;
    std::cout << "   - Linear operations are " << speedRatio << "x " << speedDescription << " Fast operations" << std::endl;
    
    return results;
}

void compareFloatAndDouble(const std::vector< BenchmarkResult<float> >& floatResults, 
                           const std::vector< BenchmarkResult<double> >& doubleResults) {
    
    std::cout << "\n\n" << std::string(100, '=') << std::endl;
    std::cout << "PRECISION COMPARISON BETWEEN FLOAT AND DOUBLE" << std::endl;
    std::cout << std::string(100, '=') << std::endl;
    
    std::cout << "\nComparing linear multiplication method:" << std::endl;
    std::cout << std::left << std::setw(15) << "n" 
              << std::setw(20) << "Float Error" 
              << std::setw(20) << "Double Error" 
              << std::setw(20) << "Improvement Factor" 
              << std::endl;
    std::cout << std::string(75, '-') << std::endl;
    
    for (size_t i = 0; i < floatResults.size(); i++) {
        double improvementFactor = 0;
        if (floatResults[i].linearError > 0) {
            improvementFactor = floatResults[i].linearError / doubleResults[i].linearError;
        }
        
        std::cout << std::left << std::setw(15) << i+1
                  << std::setw(20) << floatResults[i].linearError
                  << std::setw(20) << doubleResults[i].linearError
                  << std::setw(20) << improvementFactor
                  << std::endl;
    }
    
    std::cout << "\nComparing fast multiplication method:" << std::endl;
    std::cout << std::left << std::setw(15) << "n" 
              << std::setw(20) << "Float Error" 
              << std::setw(20) << "Double Error" 
              << std::setw(20) << "Improvement Factor" 
              << std::endl;
    std::cout << std::string(75, '-') << std::endl;
    
    for (size_t i = 0; i < floatResults.size(); i++) {
        double improvementFactor = 0;
        if (floatResults[i].fastMultiplyError > 0) {
            improvementFactor = floatResults[i].fastMultiplyError / doubleResults[i].fastMultiplyError;
        }
        
        std::cout << std::left << std::setw(15) << i+1
                  << std::setw(20) << floatResults[i].fastMultiplyError
                  << std::setw(20) << doubleResults[i].fastMultiplyError
                  << std::setw(20) << improvementFactor
                  << std::endl;
    }
    
    // Calculate average improvement factors
    double avgLinearImprovement = 0;
    double avgFastImprovement = 0;
    int validLinearCount = 0;
    int validFastCount = 0;
    
    for (size_t i = 0; i < floatResults.size(); i++) {
        if (floatResults[i].linearError > 0 && doubleResults[i].linearError > 0) {
            avgLinearImprovement += (floatResults[i].linearError / doubleResults[i].linearError);
            validLinearCount++;
        }
        
        if (floatResults[i].fastMultiplyError > 0 && doubleResults[i].fastMultiplyError > 0) {
            avgFastImprovement += (floatResults[i].fastMultiplyError / doubleResults[i].fastMultiplyError);
            validFastCount++;
        }
    }
    
    if (validLinearCount > 0) {
        avgLinearImprovement /= validLinearCount;
    }
    
    if (validFastCount > 0) {
        avgFastImprovement /= validFastCount;
    }
    
    std::cout << "\nSUMMARY:" << std::endl;
    std::cout << "- Average precision improvement with double vs float (linear method): " 
              << avgLinearImprovement << "x" << std::endl;
    std::cout << "- Average precision improvement with double vs float (fast method): " 
              << avgFastImprovement << "x" << std::endl;
    
    std::cout << "\nCONCLUSION:" << std::endl;
    if (avgLinearImprovement > 1.1) {
        std::cout << "- For the linear method, double precision provides significantly better accuracy than float"
                  << " (average " << avgLinearImprovement << "x improvement)" << std::endl;
    } else {
        std::cout << "- For the linear method, double precision and float provide similar levels of accuracy" << std::endl;
    }
    
    if (avgFastImprovement > 1.1) {
        std::cout << "- For the fast method, double precision provides significantly better accuracy than float"
                  << " (average " << avgFastImprovement << "x improvement)" << std::endl;
    } else {
        std::cout << "- For the fast method, double precision and float provide similar levels of accuracy" << std::endl;
    }
}

int main() {
    // Open CSV file for writing
    std::ofstream csvFile("benchmark_results.csv");
    // Set precision and fixed notation for the CSV file
    csvFile.setf(std::ios::fixed, std::ios::floatfield);
    csvFile.precision(10);
    // Write CSV header
    csvFile << "Type,n,Linear_Result,Fast_Result,Expected_Result,Linear_Error,Fast_Error,Linear_Time,Fast_Time,Winner\n";
    
    // Run benchmark for float (32-bit)
    auto floatResults = runBenchmark<float>("float", csvFile);
    
    // Run benchmark for double (64-bit)
    auto doubleResults = runBenchmark<double>("double", csvFile);
    
    // Compare precision between float and double
    compareFloatAndDouble(floatResults, doubleResults);
    
    // Close CSV file
    csvFile.close();
    
    std::cout << "Results have been written to benchmark_results.csv" << std::endl;
    
    return 0;
}
