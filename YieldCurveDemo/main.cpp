// =================================================================================
// main.cpp
//
// Entry point for the Yield Curve Interpolation project.
// This file includes the YieldCurveBuilder class and executes its analysis.
// =================================================================================

#include "YieldCurveBuilder.h" // Include the header file containing our class

int main() {
    try {
        // Define the evaluation date for the analysis.
        // All calculations will be relative to this date.
        QuantLib::Date evalDate(24, QuantLib::August, 2025);

        // Create an instance of our builder class.
        YieldCurveBuilder builder(evalDate);

        // Run the full analysis.
        // This will build the curves and print the comparison table to the console.
        builder.runAnalysis();

    }
    catch (const std::exception& e) {
        // Catch any standard exceptions (including those from QuantLib)
        // and print an error message.
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1; // Exit with an error code
    }
    catch (...) {
        // Catch any other unknown exceptions.
        std::cerr << "An unknown error occurred." << std::endl;
        return 1; // Exit with an error code
    }

    // If everything ran successfully, exit with a success code.
    return 0;
}
