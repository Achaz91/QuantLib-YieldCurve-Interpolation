#pragma once

// =================================================================================
// YieldCurveBuilder.h
//
// Author: [Your Name]
// Date: August 24, 2025
//
// Description:
// A header-only C++ class to build and analyze a zero-coupon yield curve
// from discrete market data points using the QuantLib library.
// It demonstrates and compares two interpolation methods:
// 1. Piecewise Linear Interpolation
// 2. Natural Cubic Spline Interpolation
//
// This version uses specific QuantLib headers for clarity and to avoid
// precompiled header issues.
// =================================================================================


// --- Standard Library Includes ---
#include <iostream>
#include <vector>
#include <iomanip>

// --- Specific QuantLib Header Includes ---
#include <ql/settings.hpp>
#include <ql/time/calendars/target.hpp>
#include <ql/time/date.hpp>
#include <ql/time/daycounters/actual365fixed.hpp>
#include <ql/termstructures/yield/zerocurve.hpp>
#include <ql/math/interpolations/linearinterpolation.hpp>
#include <ql/math/interpolations/cubicinterpolation.hpp>
// linear and cubic interpolations are included via zerocurve

#include <ql/handle.hpp>


// To simplify calls to QuantLib functions and classes
using namespace QuantLib;

/**
 * @class YieldCurveBuilder
 * @brief Encapsulates the logic for building, interpolating, and analyzing yield curves.
 */
class YieldCurveBuilder {
public:
    /**
     * @brief Constructor for the YieldCurveBuilder.
     * @param evaluationDate The reference date for all calculations.
     */
    YieldCurveBuilder(const Date& evaluationDate)
        : evaluationDate_(evaluationDate),
        calendar_(TARGET()),
        dayCounter_(Actual365Fixed())
    {
        // Set the global evaluation date for the QuantLib environment
        Settings::instance().evaluationDate() = evaluationDate_;

        // Populate the market data vectors upon construction
        setupMarketData();
    }

    /**
     * @brief Runs the full analysis: builds curves and prints a comparison table.
     */
    void runAnalysis() {
        std::cout << "Evaluation Date: " << evaluationDate_ << std::endl;
        std::cout << "----------------------------------------------------" << std::endl;

        // 1. Build the Linear Interpolation Curve
        Handle<YieldTermStructure> linearCurveHandle(
            ext::make_shared<InterpolatedZeroCurve<Linear>>(dates_, rates_, dayCounter_, calendar_, Linear())
        );

        // 2. Build the Natural Cubic Spline Interpolation Curve
        Handle<YieldTermStructure> cubicSplineCurveHandle(
            ext::make_shared<InterpolatedZeroCurve<Cubic>>(dates_, rates_, dayCounter_, calendar_, Cubic())
        );

        // 3. Enable extrapolation on both curves
        linearCurveHandle->enableExtrapolation();
        cubicSplineCurveHandle->enableExtrapolation();

        // 4. Perform and display the analysis
        performAnalysis(linearCurveHandle, cubicSplineCurveHandle);
    }


private:
    // --- Member Variables ---
    Date evaluationDate_;
    Calendar calendar_;
    DayCounter dayCounter_;
    std::vector<Date> dates_;
    std::vector<Rate> rates_;

    /**
     * @brief Initializes the market data vectors (dates and rates).
     */
    void setupMarketData() {
        dates_.push_back(calendar_.advance(evaluationDate_, 6, Months));
        rates_.push_back(0.0300); // 3.00%

        dates_.push_back(calendar_.advance(evaluationDate_, 1, Years));
        rates_.push_back(0.0350); // 3.50%

        dates_.push_back(calendar_.advance(evaluationDate_, 2, Years));
        rates_.push_back(0.0375); // 3.75%

        dates_.push_back(calendar_.advance(evaluationDate_, 5, Years));
        rates_.push_back(0.0400); // 4.00%

        dates_.push_back(calendar_.advance(evaluationDate_, 10, Years));
        rates_.push_back(0.0425); // 4.25%

        dates_.push_back(calendar_.advance(evaluationDate_, 30, Years));
        rates_.push_back(0.0450); // 4.50%
    }

    /**
     * @brief Calculates and prints the comparison table for given curves.
     * @param linearCurve A handle to the linearly interpolated curve.
     * @param cubicCurve A handle to the cubic spline interpolated curve.
     */
    void performAnalysis(const Handle<YieldTermStructure>& linearCurve,
        const Handle<YieldTermStructure>& cubicCurve) {

        std::cout << std::fixed << std::setprecision(5);
        std::cout << "Maturity    | Linear Rate | Cubic Spline Rate" << std::endl;
        std::cout << "----------------------------------------------------" << std::endl;

        std::vector<Period> testPeriods = { Period(3, Months), Period(7, Years), Period(40, Years) };

        for (const auto& period : testPeriods) {
            Date targetDate = calendar_.advance(evaluationDate_, period);

            Time t = dayCounter_.yearFraction(evaluationDate_, targetDate);

            Rate linearRate = linearCurve->zeroRate(t, Continuous);
            Rate cubicRate = cubicCurve->zeroRate(t, Continuous);

            std::cout << std::left << std::setw(11) << period
                << " |  " << std::setw(10) << linearRate
                << " |  " << std::setw(10) << cubicRate << std::endl;
        }
        std::cout << "----------------------------------------------------" << std::endl;
    }
};
