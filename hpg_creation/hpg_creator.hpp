// ==============================================================================
// ICAP License
// ==============================================================================
// University of Illinois/NCSA
// Open Source License
// 
// Copyright (c) 2014-2016 University of Illinois at Urbana-Champaign.
// All rights reserved.
// 
// Developed by:
// 
//     Nils Oberg
// 	Blake J. Landry, PhD
// 	Arthur R. Schmidt, PhD
// 	Ven Te Chow Hydrosystems Lab
// 
//     University of Illinois at Urbana-Champaign
// 
//     https://vtchl.illinois.edu
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal with
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
// 
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimers.
// 
//     * Redistributions in binary form must reproduce the above copyright notice,
//       this list of conditions and the following disclaimers in the
//       documentation and/or other materials provided with the distribution.
// 
//     * Neither the names of the Ven Te Chow Hydrosystems Lab, University of
// 	  Illinois at Urbana-Champaign, nor the names of its contributors may be
// 	  used to endorse or promote products derived from this Software without
// 	  specific prior written permission.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE
// SOFTWARE.

#ifndef HPG_CREATOR_H_____________________20041020122121__
#define HPG_CREATOR_H_____________________20041020122121__


#include <deque>

#include "../hpg_interp/hpg.hpp"
#include "../hpg/error.hpp"
#include "../xslib/reach.h"


/** @file
* This file is the header file for the HPG Creator library.
*/
//
//
//// Support Win32 and GNU GCC library export.  Win32 must
//// explicitly specify what is exported.
//#if defined(_MSC_VER)
//#if defined(HpgCreator_EXPORTS)
//#define HpgCreator_API __declspec(dllexport)
//#else
//#define HpgCreator_API __declspec(dllimport)
//#endif
//#else
//#define HpgCreator_API
//#endif


class HpgCreator
{
private:
    /**
    * This section describes computation characteristics.
    */
    int numSteps; /**< number of computations in back-water curve computaiton */
    double convergenceTol; /**< convergence factor determines if iterative solution has converged; defaults to 0.000001 */
    int maxIterations; /**< maximum number of iterations when solving; defaults to 100 */
    double maxDepthFrac; /**< maximum depth (% of diameter) before cutting off computations; defaults to 0.80 */
    int numHpc; /**< maximum number of curves in HPG; defaults to 20 */
    int numPoints; /**< maximum number of points on each curve; defaults to 40 */
    double g; /**< gravity constant. changes when changing units; defaults to 9.81 (SI) 32.486 (English) */
    double kn; /**< units constant. changes when changing units; defaults to 1.00 (SI) 1.486 (English) */
    short units; /**< units specifier; defaults to English units */
    int errorCode; /**< error code; 0 == no error, non-0 == error */
    int minCurvePoints; /**< this is the minimum number of points on a curve that are required */
public:
    /**
    * Constructor initializes everything to default values.
    */
    HpgCreator();

    std::shared_ptr<hpg::Hpg> AutoCreateHpg(const xs::Reach& reach);

    // These should be private
    double findMaxFlow(const xs::Reach& reach, bool reverseSlope, double &yCritMax);
    void findFlowIncrements(const xs::Reach& reach, bool reverseSlope, double minDepth, double maxDepth, std::deque<double> &flows);
    void findFlowIncrementsByFlow(const xs::Reach& reach, bool reverseSlope, double minFlow, double maxFlow, std::deque<double> &flows);

public:
    //HPG * AutoCreateHPG2(double diameter, double length, double roughness, double slope, double dsInvert, long nodeID = -1, double dsStation = hpg::error::bad_value);

    //double FindCriticalFlow(const xs::Reach& reach, double depth);
    //double FindMinFlow(const xs::Reach& reach, bool reverseSlope, double starting_discharge, double ending_discharge, double &y_critical_min);

    void computeHpgCurve(const xs::Reach& reach, double flow, double pressurizedHeight, bool reverseSlope, double& yNormal, double& yCritical, hpg::hpgvec &curve);
    bool computeValidHpgCurve(const xs::Reach& reach, double flow, double pressurizedHeight, bool reverseSlope, double& yNormal, double& yCritical, hpg::hpgvec &curve);

public:
    /**
    * getConvergenceTolerance returns the convergence factor for
    * creating HPGs.
    */
    double getConvergenceTolerance();
    /**
    * setConvergenceTolerance sets the convergence factor for
    * creating HPGs.
    */
    void setConvergenceTolerance(double factor);
    /**
    * getNumberOfCurves gets the maximum number of curves to
    * create in an HPG.
    */
    int getNumberOfCurves();
    /**
    * setNumberOfCurves sets the maximum number of curves to
    * create in an HPG.
    */
    void setNumberOfCurves(int curves);
    /**
    * getNumberOfPointsPerCurve gets the maximum number of
    * points to use on a curve when creating HPGs.
    */
    int getNumberOfPointsPerCurve();
    /**
    * setNumberOfPointsPerCurve sets the maximum number of
    * points to use on a curve when creating HPGs.
    */
    void setNumberOfPointsPerCurve(int points);
    /**
    * getMaxDepthFraction gets the depth as a percentage that
    * is considered to be unsteady flow.  Value should be a
    * number between and including 0 and 1.
    */
    double getMaxDepthFraction();
    /**
    * setMaxDepthFraction sets the depth as a percentage that
    * is considered to be unsteady flow.  Value should be a
    * number between and including 0 and 1.
    */
    void setMaxDepthFraction(double depth);
    /**
    * getMaxIterations returns the maximum number of
    * iterations allowed during computations.
    */
    int getMaxIterations();
    /**
    * setMaxIterations sets the maximum number of
    * iterations allowed during computations.
    */
    void setMaxIterations(int iterations);
    /**
    * setUnits sets the units type for this HPG. Valid types are
    * specified in <tarp/units.h> header.
    */
    void setUnits(int u);
    /**
    * getMinCurveSize returns the minimum number of points that constitute
    * a valid curve.
    */
    int getMinCurveSize();
    /**
    * setMinCurveSize sets the minimum number of points that are required 
    * to constitute a valid curve.
    */
    void setMinCurveSize(int size);

    int getNumBackwaterSteps();
    void setNumBackwaterSteps(int numComp);

    /**
    * Return the error code.
    */
    int getError();
    //
    //
    //
};


enum HpgUnits
{
    Hpg_SI,
    Hpg_English,
};

//#if defined(_MSC_VER)
//HPGCREATOR_API long hpgMetricUnits();
//HPGCREATOR_API long hpgEnglishUnits();
//HPGCREATOR_API long hpgAutoCreateHPG(double diameter,
//                                     double length,
//                                     double roughness,
//                                     double slope,
//                                     double dsInvert,
//                                     double unsteadyDepth,
//                                     long units,
//                                     char *outputFile,
//                                     long nodeID,
//                                     double dsStation);
//#endif

#endif//HPG_CREATOR_H_____________________20041020122121__
