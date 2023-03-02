/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

// Blueberry
#include <berryISelectionListener.h>
#include <berryISelectionService.h>

// Qmitk
#include <internal/SplineVisualisationPlugin.h>
#include <filesystem>
#include <ui_SplineVisualisationPluginControls.h>
#include <map>
#include <tchar.h>
#include <vector>
// Qt
#include <QFileDialog>
#include <QMessageBox>
#include <QWidget.h>
#include <qthread.h>

// mitk image
#include <mitkImage.h>
// PointSet
#include <mitkPointSet.h>
#include <mitkProperties.h>

// inheriting MITK classes...
#include <QmitkRenderWindow.h>
#include <QmitkSliceWidget.h>
#include <mitkContourModel.h>
#include <mitkRenderingManager.h>
#include <mitkVector.h>
#include <mitkVirtualTrackingDevice.h>

// standard imports
#include <algorithm>
#include <chrono>
#include <dirent_win32.h>
#include <fstream>
#include <iostream>
#include <new.h>
#include <sstream>
#include <string>
#include <vector>

// MITK imports
#include "mitkTubeGraph.h"
#include <QInputDialog>
#include <QmitkAbstractView.h>
#include <QmitkSliceNavigationListener.h>
#include <mitkBaseRenderer.h>
#include <mitkClippingProperty.h>
#include <mitkDataNode.h>
#include <mitkGradientBackground.h>
#include <mitkLog.h>
#include <mitkStandaloneDataStorage.h>
#include <mitkVtkMapper.h>
#include "mitkSplineVisualisationEvaluationUtil.h"

//  VTK imports
#include <mitkSurfaceVtkMapper3D.h>
#include <vtkDataReader.h>
#include <vtkImageStencil.h>
#include <vtkKdTreePointLocator.h>
#include <vtkKochanekSpline.h>
#include <vtkNew.h>
#include <vtkOpenGLPolyDataMapper.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkParametricSpline.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTupleInterpolator.h>
#include <vtkSCurveSpline.h>
#include <opencv2/core.hpp>
#include <vtkPiecewiseFunction.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointLocator.h>
#include <vtkPointInterpolator.h>
#include <windows.h>
#include "vtkBoxMuellerRandomSequence.h"


 

std::vector<double> SplineVisualisationEvaluationUtil::TimeStamp_Tool0(82);
std::vector<double> SplineVisualisationEvaluationUtil::Valid_Tool0(82);
std::vector<double> SplineVisualisationEvaluationUtil::X_Tool0(82);
std::vector<double> SplineVisualisationEvaluationUtil::Y_Tool0(82);
std::vector<double> SplineVisualisationEvaluationUtil::Z_Tool0(82);
mitk::PointSet::Pointer XYZ_Tool0 = mitk::PointSet::New();
mitk::Point3D temp_Point_Tool0;
std::vector<double> SplineVisualisationEvaluationUtil::QX_Tool0(82);
std::vector<double> SplineVisualisationEvaluationUtil::QY_Tool0(82);
std::vector<double> SplineVisualisationEvaluationUtil::QZ_Tool0(82);
std::vector<double> SplineVisualisationEvaluationUtil::QR_Tool0(82);
std::vector<double> SplineVisualisationEvaluationUtil::TimeStamp_Tool1(82);
std::vector<double> SplineVisualisationEvaluationUtil::Valid_Tool1(82);
std::vector<double> SplineVisualisationEvaluationUtil::X_Tool1(82);
std::vector<double> SplineVisualisationEvaluationUtil::Y_Tool1(82);
std::vector<double> SplineVisualisationEvaluationUtil::Z_Tool1(82);
std::vector<double> SplineVisualisationEvaluationUtil::QX_Tool1(82);
std::vector<double> SplineVisualisationEvaluationUtil::QY_Tool1(82);
std::vector<double> SplineVisualisationEvaluationUtil::QZ_Tool1(82);
std::vector<double> SplineVisualisationEvaluationUtil::QR_Tool1(82);
std::vector<double> SplineVisualisationEvaluationUtil::TimeStamp_Tool2(82);
std::vector<double> SplineVisualisationEvaluationUtil::Valid_Tool2(82);
std::vector<double> SplineVisualisationEvaluationUtil::X_Tool2(82);
std::vector<double> SplineVisualisationEvaluationUtil::Y_Tool2(82);
std::vector<double> SplineVisualisationEvaluationUtil::Z_Tool2(82);
std::vector<double> SplineVisualisationEvaluationUtil::QX_Tool2(82);
std::vector<double> SplineVisualisationEvaluationUtil::QY_Tool2(82);
std::vector<double> SplineVisualisationEvaluationUtil::QZ_Tool2(82);
std::vector<double> SplineVisualisationEvaluationUtil::QR_Tool2(82);
std::vector<double> SplineVisualisationEvaluationUtil::TimeStamp_Tool3(82);
std::vector<double> SplineVisualisationEvaluationUtil::Valid_Tool3(82);
std::vector<double> SplineVisualisationEvaluationUtil::X_Tool3(82);
std::vector<double> SplineVisualisationEvaluationUtil::Y_Tool3(82);
std::vector<double> SplineVisualisationEvaluationUtil::Z_Tool3(82);
std::vector<double> SplineVisualisationEvaluationUtil::QX_Tool3(82);
std::vector<double> SplineVisualisationEvaluationUtil::QY_Tool3(82);
std::vector<double> SplineVisualisationEvaluationUtil::QZ_Tool3(82);
std::vector<double> SplineVisualisationEvaluationUtil::QR_Tool3(82);

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

// Specifc Spline Parametric Range Values
double u[3];
double Pt[3];
double lastPt[3];
double Du[9];
// generate Spline member variables
mitk::PointSet::Pointer tempReferencePoints;
int referencePointCounter;
double referencePointStep;

long counterCoordinates = 0;

SplineVisualisationEvaluationUtil::SplineVisualisationEvaluationUtil() : 
    m_DataStorage() {
  referencePointCounter = 0;
  tempReferencePoints = mitk::PointSet::New();
  m_Debug = false;
  m_SplineVisualisationPlugin = new SplineVisualisationPlugin();

}

SplineVisualisationEvaluationUtil::~SplineVisualisationEvaluationUtil() {}


void SplineVisualisationEvaluationUtil::SetDebug(bool setting)
{
    m_Debug = setting;
}

std::vector<double> SplineVisualisationEvaluationUtil::CalculateAVGStandartDeviation(
  mitk::PointSet::Pointer groundtruth,
  mitk::PointSet::Pointer measuredvalues)
{

  std::vector<double> vecMeanXGT, vecMeanYGT, vecMeanZGT;
  std::vector<double> vecMeanXMV, vecMeanYMV, vecMeanZMV;

  // first do the mean of each x,y,z, of the groundtruth
  for (auto iterMV = measuredvalues.GetPointer()->Begin(); iterMV != measuredvalues.GetPointer()->End(); ++iterMV)
  {
    vecMeanXMV.push_back(iterMV.Value()[0]);
    vecMeanYMV.push_back(iterMV.Value()[1]);
    vecMeanZMV.push_back(iterMV.Value()[2]);
  }
  // first do the mean of each x,y,z, of the groundtruth
 for (auto iterGT = groundtruth.GetPointer()->Begin(); iterGT != groundtruth.GetPointer()->End(); ++iterGT)
  {
   vecMeanXGT.push_back(iterGT.Value()[0]);
   vecMeanYGT.push_back(iterGT.Value()[1]);
   vecMeanZGT.push_back(iterGT.Value()[2]);
  }
 
  // Calculate the mean of xyz GT values
  double meanXGT = std::accumulate(vecMeanXGT.begin(), vecMeanXGT.end(), 0.0) / vecMeanXGT.size();
  double meanYGT = std::accumulate(vecMeanYGT.begin(), vecMeanYGT.end(), 0.0) / vecMeanYGT.size();
  double meanZGT = std::accumulate(vecMeanZGT.begin(), vecMeanZGT.end(), 0.0) / vecMeanZGT.size();
  // Calculate the mean of xyz MV values
  double meanXMV = std::accumulate(vecMeanXMV.begin(), vecMeanXMV.end(), 0.0) / vecMeanXMV.size();
  double meanYMV = std::accumulate(vecMeanYMV.begin(), vecMeanYMV.end(), 0.0) / vecMeanYMV.size();
  double meanZMV = std::accumulate(vecMeanZMV.begin(), vecMeanZMV.end(), 0.0) / vecMeanZMV.size();
  // Calculate the distance from each data point to the mean in GT DS, take absolute values
  std::for_each(vecMeanXGT.begin(), vecMeanXGT.end(), [&meanXGT](auto &x){x -= meanXGT;x = std::abs(x);});
  std::for_each(vecMeanYGT.begin(), vecMeanYGT.end(), [&meanYGT](auto &y){y -= meanYGT;y = std::abs(y);});
  std::for_each(vecMeanZGT.begin(),vecMeanZGT.end(),[&meanZGT](auto &z){z -= meanZGT;z = std::abs(z);});
  // Calculate the distance from each data point to the mean in MV DS, take absolute values
  std::for_each(vecMeanXMV.begin(),vecMeanXMV.end(),[&meanXMV](auto &x){x -= meanXMV;x = std::abs(x);});
  std::for_each(vecMeanYMV.begin(),vecMeanYMV.end(),[&meanYMV](auto &y){y -= meanYMV;y = std::abs(y);});
  std::for_each(vecMeanZMV.begin(),vecMeanZMV.end(),[&meanZMV](auto &z){z -= meanZMV;z = std::abs(z);});
  // Calculate standard deviation for each x,y,z axis in GT DS
  double stdDeviationGTX = sqrt(pow(std::accumulate(vecMeanXGT.begin(), vecMeanXGT.end(), 0.0), 2) / vecMeanXGT.size());
  double stdDeviationGTY = sqrt(pow(std::accumulate(vecMeanYGT.begin(), vecMeanYGT.end(), 0.0), 2) / vecMeanYGT.size());
  double stdDeviationGTZ = sqrt(pow(std::accumulate(vecMeanZGT.begin(), vecMeanZGT.end(), 0.0), 2) / vecMeanZGT.size());
  // Calculate standard deviation for each x,y,z axis in MV DS
  double stdDeviationMVX = sqrt(pow(std::accumulate(vecMeanXMV.begin(), vecMeanXMV.end(), 0.0), 2) / vecMeanXMV.size());
  double stdDeviationMVY = sqrt(pow(std::accumulate(vecMeanYMV.begin(), vecMeanYMV.end(), 0.0), 2) / vecMeanYMV.size());
  double stdDeviationMVZ = sqrt(pow(std::accumulate(vecMeanZMV.begin(), vecMeanZMV.end(), 0.0), 2) / vecMeanZMV.size());

 
  // calculate the variance with
  double stdDeviationResultGT = sqrt(pow(stdDeviationGTX, 2) + pow(stdDeviationGTY, 2) + pow(stdDeviationGTZ, 2));
  double stdDeviationResultMV = sqrt(pow(stdDeviationMVX, 2) + pow(stdDeviationMVY, 2) + pow(stdDeviationMVZ, 2));

  return {stdDeviationResultGT, stdDeviationResultMV}; 
}

double SplineVisualisationEvaluationUtil::CalculateEuclideanDistance(mitk::PointSet::Pointer groundtruth, mitk::PointSet::Pointer measuredvalues)
{
  std::vector<double> allEuclidanDistances;
  int numberofValues = groundtruth.GetPointer()->GetSize();
  // Calculating the euclidian distance between two points
  // d(P1, P2) = (x2-x1)^2 + (y2-y1)^2 + (z2-z1)^2.
  auto iterMV = measuredvalues.GetPointer()->Begin();
  auto iterGT = groundtruth.GetPointer()->Begin();
  for (int numIter = 0; numIter != numberofValues; numIter++)
  {
      allEuclidanDistances.push_back(std::sqrt(pow(iterGT.Value()[0] - iterMV.Value()[0], 2) +
                                     pow(iterGT.Value()[1] - iterMV.Value()[1], 2) +
                                     pow(iterGT.Value()[2] - iterMV.Value()[2], 2)));
    ++iterMV;
    ++iterGT;
  }
  return ((std::accumulate(allEuclidanDistances.begin(), allEuclidanDistances.end(), 0.0)) / numberofValues);
}

// \brief Calculate the AVG Mean of the diffrence (Error)
mitk::Point3D SplineVisualisationEvaluationUtil::CalculateMSE(mitk::PointSet::Pointer groundtruth,
                                                      mitk::PointSet::Pointer measuredvalues)
{
  int numberOfEntries = groundtruth->GetSize();

  std::vector<double> xSquareDiffrence;
  std::vector<double> ySquareDiffrence;
  std::vector<double> zSquareDiffrence;
  mitk::Point3D outputAVG;
  if (numberOfEntries != measuredvalues->GetSize())
  {
    MITK_INFO << "ERROR: Groundtruth Size and Measured Size are not the same!" << std::endl;
    return outputAVG;
  }
  auto iterMV = measuredvalues.GetPointer()->Begin();
  auto iterGT = groundtruth.GetPointer()->Begin();
  for (int numIter = 0; numIter != numberOfEntries; numIter++)
  {
      xSquareDiffrence.push_back(pow(iterGT.Value()[0] - iterMV.Value()[0], 2));
      ySquareDiffrence.push_back(pow(iterGT.Value()[1] - iterMV.Value()[1], 2));
      zSquareDiffrence.push_back(pow(iterGT.Value()[2] - iterMV.Value()[2], 2));
      ++iterMV; ++iterGT;
  }
  if (m_Debug)
  {
    for (int i =0; i != numberOfEntries; i++)
    {
      MITK_INFO << "xSquareDifference @" << i << " : " << xSquareDiffrence.at(i) << std::endl;
      MITK_INFO << "ySquareDifference: @" << i << " : " << ySquareDiffrence.at(i) << std::endl;
      MITK_INFO << "zSquareDifference: @" << i << " : " << zSquareDiffrence.at(i) << std::endl;
    }

    MITK_INFO << "Accumulated x: "
              << (std::accumulate(xSquareDiffrence.begin(), xSquareDiffrence.end(), 0.0)) / numberOfEntries << std::endl;
    MITK_INFO << "Accumulated y: " <<(std::accumulate(ySquareDiffrence.begin(), ySquareDiffrence.end(), 0.0)) / numberOfEntries
              << std::endl;
    MITK_INFO << "Accumulated z: " << (std::accumulate(zSquareDiffrence.begin(), zSquareDiffrence.end(), 0.0)) / numberOfEntries
              << std::endl;
  }
  outputAVG[0] = (std::accumulate(xSquareDiffrence.begin(), xSquareDiffrence.end(), 0.0)) / numberOfEntries;
  outputAVG[1] = (std::accumulate(ySquareDiffrence.begin(), ySquareDiffrence.end(), 0.0)) / numberOfEntries;
  outputAVG[2] = (std::accumulate(zSquareDiffrence.begin(), zSquareDiffrence.end(), 0.0)) / numberOfEntries;

  return outputAVG;
}

mitk::Point3D SplineVisualisationEvaluationUtil::CalculateRMSE(mitk::Point3D AVGMSE)
{
  mitk::Point3D outputPoint;
  FillVector3D(outputPoint, sqrt(AVGMSE[0]), sqrt(AVGMSE[1]), sqrt(AVGMSE[2]));
  return outputPoint;
}

mitk::PointSet::Pointer SplineVisualisationEvaluationUtil::KdTreeLocator(mitk::PointSet::Pointer groundtruth,
                                                                          mitk::PointSet::Pointer measuredvalues,
                                                                         bool writeToFile,
                                                                         std::string windowInput)
{
  mitk::PointSet::Pointer outputPointSet = mitk::PointSet::New();

  vtkNew<vtkPolyData> polyGTData;
  polyGTData->SetPoints(ConvertMITKIntoVTKPointset(groundtruth));

  vtkNew<vtkKdTreePointLocator> locatorGT; 
  locatorGT->SetDataSet(polyGTData);

  double x[3];
  mitk::Point3D previousTempPoint;
  mitk::Point3D tempPoint;
  vtkSmartPointer<vtkIdList> nPoints = vtkSmartPointer<vtkIdList>::New();
  vtkIdType cPlocator;
  double rad = 0.5;
  for (auto iterMV = measuredvalues.GetPointer()->Begin(); iterMV != measuredvalues.GetPointer()->End(); iterMV++)
  {
    x[0] = iterMV.Value()[0];
    x[1] = iterMV.Value()[1];
    x[2] = iterMV.Value()[2];
    cPlocator = locatorGT->FindClosestPoint(x); // mitk::PointLocator::IdType
    tempPoint = mitk::Point3D(polyGTData->GetPoint(cPlocator));
    if (tempPoint[0] != previousTempPoint[0] && tempPoint[1] != previousTempPoint[1] &&
        tempPoint[2] != previousTempPoint[2])
    {
      outputPointSet->InsertPoint(tempPoint, 0);
    } else {

      locatorGT->FindClosestNPoints(2, x, nPoints); // get next nearest point 
      tempPoint = mitk::Point3D(polyGTData->GetPoint(nPoints->GetId(1)));
      outputPointSet->InsertPoint(tempPoint, 0);
    }
    if (m_Debug)
    {
      MITK_INFO << "New Closest Points values @ " << iterMV.Index() << " : "
                << " x: " << tempPoint[0] << " y: " << tempPoint[1] << " z: " << tempPoint[2] << std::endl;
    }
    previousTempPoint = tempPoint;
  }
  outputPointSet->SetObjectName(windowInput);
  if (writeToFile)
  {
    WriteToCSVFile(outputPointSet);
  }

  return outputPointSet;
}

double SplineVisualisationEvaluationUtil::RoundUp(double value)
{
  const double multiplier = std::pow(10.0, 2);
  return std::round(value * multiplier) / multiplier;
}

void SplineVisualisationEvaluationUtil::CalculateParametricErrorMeasure(mitk::PointSet::Pointer groundtruth,
                                                                mitk::PointSet::Pointer measuredvalues, 
                                                                std::string windowInput, bool writeGTtoFile, bool writeAllToFile, bool printheader)
{ // calculate euclidian distance between ground truth and measured spline then make countour between each point
  // take only the points that are nearest to each other
  std::stringstream tempString;
  MITK_INFO << "-----------------------------------(AVG MSE & Euclidean Distance)-----------------------------------"
             << std::endl;
  MITK_INFO << "Size of Groundtruth Values: " << groundtruth->GetSize() << " ; "
            << " Size of Measured Values : " << measuredvalues->GetSize() << std::endl;

  // if unequal nearest neighbour searches was a k - d - tree.
  if (groundtruth->GetSize() != measuredvalues->GetSize())
  {
    groundtruth = KdTreeLocator(groundtruth, measuredvalues, writeGTtoFile, windowInput);
  }
  MITK_INFO << "New Size of Groundtruth Values : " << groundtruth->GetSize() << std::endl;
  mitk::Point3D mse = CalculateMSE(groundtruth, measuredvalues);
  MITK_INFO << "AVG Mean Square Error : "
            << RoundUp(mse[0]) << " mm"
            << " ; " << RoundUp(mse[1]) << " mm"
            << " ; " << RoundUp(mse[2]) << " mm" << std::endl;
  tempString << RoundUp(mse[0]) 
             << " ; " << RoundUp(mse[1]) 
             << " ; " << RoundUp(mse[2]);
  std::string MSEInputs = tempString.str();
  tempString.str(std::string());

  MITK_INFO << "AVG Euclidean distance for all Elements: " << CalculateEuclideanDistance(groundtruth, measuredvalues)
            << " mm"
             << std::endl;
  std::string EuclidInputs = std::to_string(CalculateEuclideanDistance(groundtruth, measuredvalues));

  MITK_INFO << "--------------------------------------------(AVG RMSE)----------------------------------------------"
            << std::endl;
  mitk::Point3D rmse = CalculateRMSE(CalculateMSE(groundtruth, measuredvalues));
  MITK_INFO << "AVG Root Mean Square Error: "
            << RoundUp(rmse[0]) << " mm"
            << " ; " << RoundUp(rmse[1]) << " mm"
            << " ; " << RoundUp(rmse[2]) << " mm" << std::endl;
  tempString << RoundUp(rmse[0]) << " ; " << RoundUp(rmse[1]) << " ; " << RoundUp(rmse[2]);
  std::string RMSEInputs = tempString.str();
  tempString.str(std::string());
  // see https://math.stackexchange.com/questions/850228/finding-how-spreaded-a-point-cloud-in-3d
  MITK_INFO << "---------------------------------------------(Standart Deviation of the axial, sagital and coronal plane)----------------------------------------------"
            << std::endl;
  std::vector<double> standartError = CalculateAVGStandartDeviation(groundtruth, measuredvalues);
  MITK_INFO << "The AVG standard deviation (Variance) of the groundtruth Pointset is: " << RoundUp(standartError.at(0))
            << " and for the measured Pointset: " << RoundUp(standartError.at(1))
            << " therfor, a difference (percentage) " << abs(((RoundUp(standartError.at(1)) / RoundUp(standartError.at(0)))*100)-100)
            << "%" << std::endl;
  std::string VarianceInputs = std::to_string(abs(((RoundUp(standartError.at(1)) / RoundUp(standartError.at(0)))*100)-100));
  tempString.str(std::string());

   MITK_INFO
    << "---------------------------------------------( AVG Directional Derivative Deviation )----------------------------------------------"
    << std::endl; 
  double directionalDerivativeDeviationError = CalculateDirectionalDerivativeDeviation(groundtruth, measuredvalues);
   

  // TODO research for Point Wise Interpolation Error

  if (writeAllToFile)
  {
    WriteToErrorDataFile(MSEInputs, RMSEInputs, EuclidInputs, VarianceInputs, printheader);
  }
}

double SplineVisualisationEvaluationUtil::CalculateAVGVTKPoints(vtkSmartPointer<vtkPoints> inputPoints, int xyz)
{
  double result =0;
  for (auto iter = 0; iter != inputPoints.GetPointer()->GetNumberOfPoints(); ++iter)
  {
    result += inputPoints.GetPointer()->GetPoint(iter)[xyz];
  }

  return result / inputPoints.GetPointer()->GetNumberOfPoints();
  }

/**
 * \brief 
 * \param groundtruth 
 * \param measuredvalues 
 * \return 
 */
  double SplineVisualisationEvaluationUtil::CalculateDirectionalDerivativeDeviation(
    mitk::PointSet::Pointer groundtruth, mitk::PointSet::Pointer measuredvalues)
  {
    // f(x,y,z) is not known
    // TODO:VTK wont allow Paramters set to DerivativesReturn=1 for the New function also no
    // SetDerivativesAvailable is possible anymore, a own 
    /* vtkSmartPointer<vtkPoints> mVPartialDu, mVPartialDv, mVPartialDw;
    vtkSmartPointer<vtkPoints> gTPartialDu, gTPartialDv, gTPartialDw;
    vtkSmartPointer<vtkParametricSpline> groundTruthFunction = vtkSmartPointer<vtkParametricSpline>::New();
    groundTruthFunction->SetPoints(ConvertMITKIntoVTKPointset(groundtruth));
    groundTruthFunction->SetDerivativesAvailable(true);
    groundTruthFunction->DerivativesAvailableOn();
    vtkSmartPointer<vtkParametricSpline> measuredValuesFunction = vtkSmartPointer<vtkParametricSpline>::New();
    measuredValuesFunction->SetPoints(ConvertMITKIntoVTKPointset(measuredvalues));
    measuredValuesFunction->SetDerivativesAvailable(true);
    measuredValuesFunction->DerivativesAvailableOn();
    double uvw[3];
    double Pt[3];
    double Duvw[9];
    std::fill_n(Pt, 3, 0.0);
    std::fill_n(uvw, 3, 0.0);
    std::fill_n(Duvw, 9, 0.0);
    // first do the mean of each x,y,z, of the groundtruth
    auto iterMV = measuredvalues.GetPointer()->Begin();
    int mvSize = measuredvalues.GetPointer()->GetPointSetSeriesSize();
    int i = 0;
    do
    {
      mitk::Point3D first = iterMV.Value();
      if (iterMV != measuredvalues.GetPointer()->End())
      {
        ++iterMV;
      }
      else
      {
        i = mvSize;
        break;
      }
      mitk::Point3D second = iterMV.Value();
      uvw[0] = first[0] - second[0];
      uvw[1] = first[1] - second[1];
      uvw[2] = first[2] - second[2];
      measuredValuesFunction->Evaluate(uvw, Pt, Duvw);
      // getting partial derivatives
      mVPartialDu->InsertNextPoint(Duvw[0], Duvw[1], Duvw[2]);
      mVPartialDv->InsertNextPoint(Duvw[3], Duvw[4], Duvw[5]);
      mVPartialDw->InsertNextPoint(Duvw[6], Duvw[7], Duvw[8]);
    }while (i != mvSize);

    i = 0;
    auto iterGT = groundtruth.GetPointer()->Begin();
    int gtSize = groundtruth.GetPointer()->GetPointSetSeriesSize();
    std::fill_n(Pt, 3, 0.0);
    std::fill_n(uvw, 3, 0.0);
    std::fill_n(Duvw, 9, 0.0);
    // first do the mean of each x,y,z, of the groundtruth
    do
    {
      mitk::Point3D first = iterGT.Value();
      if (iterGT != groundtruth.GetPointer()->End())
      {
        ++iterGT;
      }
      else
      {
        break;
      }
      mitk::Point3D second = iterGT.Value();
      uvw[0] = first[0] - second[0];
      uvw[1] = first[1] - second[1];
      uvw[2] = first[2] - second[2];
      groundTruthFunction->Evaluate(uvw, Pt, Duvw);
      // getting partial derivatives
      gTPartialDu->InsertNextPoint(Duvw[0], Duvw[1], Duvw[2]);
      gTPartialDv->InsertNextPoint(Duvw[3], Duvw[4], Duvw[5]);
      gTPartialDw->InsertNextPoint(Duvw[6], Duvw[7], Duvw[8]);
    } while (i != gtSize);

    MITK_INFO << "The AVG GT partial derivatives : "
              << "u:" << RoundUp(CalculateAVGVTKPoints(gTPartialDu,0))
              << " v:" << RoundUp(CalculateAVGVTKPoints(gTPartialDv,1))
              << " w:" << RoundUp(CalculateAVGVTKPoints(gTPartialDw,2)) << std::endl;
    MITK_INFO << "The AVG MV partial derivatives : "
              << "u:" << RoundUp(CalculateAVGVTKPoints(mVPartialDu,0))
              << " v:" << RoundUp(CalculateAVGVTKPoints(mVPartialDv,1))
              << " w:" << RoundUp(CalculateAVGVTKPoints(mVPartialDw,2)) << std::endl;

    // (Du_mV - Du_gT)/ DU_gt
    MITK_INFO << "The Directional Derivative Deviation Error for the polynomial Interpolant : " << "u:"
              << abs(RoundUp(CalculateAVGVTKPoints(mVPartialDu,0))) -
                   abs(RoundUp(CalculateAVGVTKPoints(gTPartialDu,0))) /
  abs(RoundUp(CalculateAVGVTKPoints(gTPartialDu,0)))
              << " v:"   // (Dv_mV - Dv_gT)/ Dv_gt
              << abs(RoundUp(CalculateAVGVTKPoints(mVPartialDu,1))) -
                   abs(RoundUp(CalculateAVGVTKPoints(gTPartialDu,1))) /
  abs(RoundUp(CalculateAVGVTKPoints(gTPartialDu,1)))
              << " w:"   // (Dw_mV - Dw_gT)/ Dw_gt
              << abs(RoundUp(CalculateAVGVTKPoints(mVPartialDu,2))) -
                   abs(RoundUp(CalculateAVGVTKPoints(gTPartialDu,2))) /
  abs(RoundUp(CalculateAVGVTKPoints(gTPartialDu,2)))
              << std::endl;

    // calculate length of directional derivitive

    double lengthDirecitonDerivitive =
      std::sqrt(std::pow(abs(RoundUp(CalculateAVGVTKPoints(mVPartialDu, 0))) -
                abs(RoundUp(CalculateAVGVTKPoints(gTPartialDu, 0))) /
                  abs(RoundUp(CalculateAVGVTKPoints(gTPartialDu, 0))),2) +
                std::pow(abs(RoundUp(CalculateAVGVTKPoints(mVPartialDu, 1))) -
        abs(RoundUp(CalculateAVGVTKPoints(gTPartialDu, 1))) / abs(RoundUp(CalculateAVGVTKPoints(gTPartialDu, 1))),2) +
        std::pow(abs(RoundUp(CalculateAVGVTKPoints(mVPartialDu, 2))) -
        abs(RoundUp(CalculateAVGVTKPoints(gTPartialDu, 2))) / abs(RoundUp(CalculateAVGVTKPoints(gTPartialDu, 2))), 2));

    MITK_INFO << "Directional derivitive length " << lengthDirecitonDerivitive << std::endl;
  }*/
    return 0.0;

  }

mitk::DataNode::Pointer SplineVisualisationEvaluationUtil::StartReadPointSet()
{
  ReadCSVPoints();
  // create a PointSet and add the 3D Point to the Set
  mitk::PointSet::Pointer AllCSVpointset = mitk::PointSet::New();
  // create a DataNode and set the PointSet as Data for the entity
  mitk::DataNode::Pointer AllCSVpointsdatanode = mitk::DataNode::New();

  for (int i = 0; i < this->TimeStamp_Tool0.size(); i++)
  {
    MITK_INFO << " CSV point Tool0 " << i << " : " << this->X_Tool0[i] << " ; " << this->Y_Tool0[i] << " ; "
              << this->Z_Tool0[i] << std::endl;
    double coordinates0[] = {this->X_Tool0[i], this->Y_Tool0[i], this->Z_Tool0[i]};
    mitk::Point3D firstPoint3D(coordinates0);
    AllCSVpointset->InsertPoint(firstPoint3D);

    if (X_Tool1[i] <= 800.0 && X_Tool1[i] > 0.1 || Y_Tool1[i] >= 800.0 && Y_Tool1[i] > 0.1 ||
        Z_Tool1[i] >= 800.0 && Z_Tool1[i] > 0.1)
    {
      MITK_INFO << " CSV point Tool1 " << i << " : " << this->X_Tool1[i] << " ; " << this->Y_Tool1[i] << " ; "
                << this->Z_Tool1[i] << std::endl;
      double coordinates1[] = {this->X_Tool1[i], this->Y_Tool1[i], this->Z_Tool1[i]};
      mitk::Point3D secPoint3D(coordinates1);
      AllCSVpointset->InsertPoint(secPoint3D);
    }
    if (X_Tool2[i] <= 800.0 && X_Tool2[i] > 0.1 || Y_Tool2[i] >= 800.0 && Y_Tool2[i] > 0.1 ||
        Z_Tool2[i] >= 800.0 && Z_Tool2[i] > 0.1)
    {
      MITK_INFO << " CSV point Tool2 " << i << " : " << this->X_Tool2[i] << " ; " << this->Y_Tool2[i] << " ; "
                << this->Z_Tool2[i] << std::endl;
      double coordinates2[] = {this->X_Tool2[i], this->Y_Tool2[i], this->Z_Tool2[i]};
      mitk::Point3D thirdPoint3D(coordinates2);
      AllCSVpointset->InsertPoint(thirdPoint3D);
    }
  }
  SetCSVPointset(AllCSVpointset);
  AllCSVpointsdatanode->SetName("CSV Points Datanode");
  AllCSVpointsdatanode->SetData(AllCSVpointset);
  counterCoordinates = counterCoordinates + 1;
  // DataNode added to the DataStorage
  m_SplineVisualisationPlugin->GetPluginDataStorage()->Add(AllCSVpointsdatanode);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  return AllCSVpointsdatanode;
}

mitk::PointSet::Pointer SplineVisualisationEvaluationUtil::GetCSVPointset()
{
  return m_CSVPointset;
}

void SplineVisualisationEvaluationUtil::SetCSVPointset(mitk::PointSet::Pointer input)
{
  m_CSVPointset = input;
}

QString SplineVisualisationEvaluationUtil::SearchForLogfiles()
{

  std::ifstream ifs(MITK_ROOT);
  QString logfilePath = QFileDialog::getOpenFileName(m_SplineVisualisationPlugin->GetMControls().AllQTabsWidget,
                                                     m_SplineVisualisationPlugin->tr("Open File"),
                                                     MITK_ROOT
                                                    ,
                                                     m_SplineVisualisationPlugin->tr("Text Files (*.csv *.xml *.txt)"));
  MITK_INFO << logfilePath << "\n" << std::endl;
  return logfilePath;
}

void SplineVisualisationEvaluationUtil::ReadCSVPoints()
{
  std::ifstream file(SplineVisualisationEvaluationUtil::SearchForLogfiles().toStdString());
  int sum = 0; // Position in row
  int k = 0;
  size_t pos = 0;
  std::string subString;
  std::vector<std::string> headliner;

  if (!file)
  {
    std::stringstream message;
    message << "Error: Could not open file!";
    MITK_INFO << message.str();
    return;
  }
  // get first line and remove it from reading process
  std::getline(file, subString);
  const char *delimiter = ";";
  int countOfEntries = std::count(subString.begin(), subString.end(), *delimiter);
  MITK_INFO << "Test:"
            << " k : " << k << " Line : " << subString << " count of entries : " << countOfEntries << std::endl;
  // iterate through the entire file and select each tool value
  while (std::getline(file, subString, *delimiter))
  {
    MITK_INFO << "current Line:"
              << " k : " << k << " sum: " << sum << " Line: " << subString << std::endl;
    if (k == TimeStamp_Tool0.size())
    {
      break;
    }
    switch (sum)
    {
      case 0:
        this->TimeStamp_Tool0[k] = stold(subString);
        MITK_INFO << "TimeStamp_Tool0 " << sum << ": " << subString << std::endl;
        sum++;
        break;

      case 1:
        this->Valid_Tool0[k] = stold(subString);
        MITK_INFO << "Valid_Tool0 " << sum << ": " << subString << std::endl;
        sum++;
        break;

      case 2:
        this->X_Tool0[k] = stold(subString);
        temp_Point_Tool0[0] = stold(subString);
        MITK_INFO << "X_Tool0 " << sum << ": " << subString << std::endl;
        sum++;
        break;

      case 3:
        this->Y_Tool0[k] = stold(subString);
        temp_Point_Tool0[1] = stold(subString);
        MITK_INFO << "Y_Tool0 " << sum << ":" << subString << std::endl;
        sum++;
        break;

      case 4:
        this->Z_Tool0[k] = stold(subString);
        temp_Point_Tool0[2] = stold(subString);
        MITK_INFO << "Z_Tool0 " << sum << ":" << subString << std::endl;
        sum++;
        break;

      case 5:
        this->QX_Tool0[k] = stold(subString);
        MITK_INFO << "QX_Tool0 " << sum << ":" << subString << std::endl;
        sum++;
        break;

      case 6:
        this->QY_Tool0[k] = stold(subString);
        MITK_INFO << "QY_Tool0 " << sum << ":" << subString << std::endl;
        sum++;
        break;

      case 7:
        this->QZ_Tool0[k] = stold(subString);
        MITK_INFO << "QZ_Tool0 " << sum << ":" << subString << std::endl;
        sum++;
        break;

      case 8:
        this->QR_Tool0[k] = stold(subString);
        MITK_INFO << "QR_Tool0 " << sum << ":" << subString << std::endl;
        if (countOfEntries == 9)
        {
          sum = 0;
          k++;
        }
        else
        {
          sum++;
        }
        break;
      case 9:
        this->TimeStamp_Tool1[k] = stold(subString);
        MITK_INFO << "TimeStamp_Tool1 " << sum << ":" << subString << std::endl;
        sum++;
        break;

      case 10:
        this->Valid_Tool1[k] = stold(subString);
        MITK_INFO << "Valid_Tool1 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 11:
        this->X_Tool1[k] = stold(subString);
        MITK_INFO << "X_Tool1 " << sum << ":" << subString << std::endl;
        sum++;
        break;

      case 12:
        this->Y_Tool1[k] = stold(subString);
        MITK_INFO << "Y_Tool1 " << sum << ":" << subString << std::endl;
        sum++;
        break;

      case 13:
        this->Z_Tool1[k] = stold(subString);
        MITK_INFO << "Z_Tool1 " << sum << ":" << subString << std::endl;
        sum++;
        break;

      case 14:
        this->QX_Tool1[k] = stold(subString);
        MITK_INFO << "QX_Tool1 " << sum << ":" << subString << std::endl;
        sum++;
        break;

      case 15:
        this->QY_Tool1[k] = stold(subString);
        MITK_INFO << "QY_Tool1 " << sum << ":" << subString << std::endl;
        sum++;
        break;

      case 16:
        this->QZ_Tool1[k] = stold(subString);
        MITK_INFO << "QZ_Tool1 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 17:
        this->QR_Tool1[k] = stold(subString);
        MITK_INFO << "QR_Tool1 " << sum << ":" << subString << std::endl;
        if (countOfEntries == 18)
        {
          sum = 0;
          k++;
        }
        else
        {
          sum++;
        }
        break;

      case 18:
        this->TimeStamp_Tool2[k] = stold(subString);
        MITK_INFO << "TimeStamp_Tool2 " << sum << ":" << subString << std::endl;
        sum++;
        break;

      case 19:
        this->Valid_Tool2[k] = stold(subString);
        MITK_INFO << "Valid_Tool2 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 20:
        this->X_Tool2[k] = stold(subString);
        MITK_INFO << "X_Tool2 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 21:
        this->Y_Tool2[k] = stold(subString);
        MITK_INFO << "Y_Tool2 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 22:
        this->Z_Tool2[k] = stold(subString);
        MITK_INFO << "Z_Tool2 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 23:
        this->QX_Tool2[k] = stold(subString);
        MITK_INFO << "QX_Tool2 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 24:
        this->QY_Tool2[k] = stold(subString);
        MITK_INFO << "QY_Tool2 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 25:
        this->QZ_Tool2[k] = stold(subString);
        MITK_INFO << "QZ_Tool2 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 26:
        pos = subString.find("\n");
        this->QR_Tool2[k] = stold(subString);
        MITK_INFO << "QR_Tool2 " << sum << ":" << subString << std::endl;
        if (countOfEntries == 27)
        {
          sum = 0;
          k++;
        }
        else
        {
          sum++;
        }
        break;
      case 27:
        pos = subString.find("\n");
        this->TimeStamp_Tool3[k] = stold(subString);
        MITK_INFO << "TimeStamp_Tool3 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 28:
        pos = subString.find("\n");
        this->Valid_Tool3[k] = stold(subString);
        MITK_INFO << "Valid_Tool3 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 29:
        pos = subString.find("\n");
        this->X_Tool3[k] = stold(subString);
        MITK_INFO << "X_Tool3 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 30:
        pos = subString.find("\n");
        this->Y_Tool3[k] = stold(subString);
        MITK_INFO << "Y_Tool3 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 31:
        pos = subString.find("\n");
        this->Z_Tool3[k] = stold(subString);
        MITK_INFO << "Z_Tool3 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 32:
        pos = subString.find("\n");
        this->QX_Tool3[k] = stold(subString);
        MITK_INFO << "QX_Tool3 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 33:
        pos = subString.find("\n");
        this->QY_Tool3[k] = stold(subString);
        MITK_INFO << "QY_Tool3 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 34:
        pos = subString.find("\n");
        this->QZ_Tool3[k] = stold(subString);
        MITK_INFO << "QZ_Tool3 " << sum << ":" << subString << std::endl;
        sum++;
        break;
      case 35:
        pos = subString.find("\n");
        this->QR_Tool3[k] = stold(subString);
        MITK_INFO << "QR_Tool3 " << sum << ":" << subString << std::endl;
        sum = 0; // sum=1;
        k++;
        break;
      default:
        MITK_INFO << "Error: Something went wrong!" << std::endl;
    }
  }
}


mitk::PointSet::Pointer SplineVisualisationEvaluationUtil::GenerateSplinePoints(vtkParametricSpline *parametricspline,
                                                                        mitk::PointSet::Pointer Anchorpoints,
                                                                        double iteration_range)
{
  auto timestamp1 = high_resolution_clock::now();
  referencePointCounter = 0;
  std::fill_n(u, 3, 0.0);
  std::fill_n(Pt, 3, 0.0);
  std::fill_n(lastPt, 3, 0.0);
  std::fill_n(Du, 9, 0.0);
  // the spline Range with absolute values
  tempReferencePoints = mitk::PointSet::New();
  // create a loop to evaluate the parametric coordinate u from [0,1] u[0] , u[1], u[2],
  // Using void Evaluate(double u[3], double Pt[3], double Du[9]) override
  // 1 loop does the same kind of points evaluation results see test results
  for (double u0 = 0; u0 <= 1.0; u0 = u0 + iteration_range)
  {
    u[0] = u0;
    u[1] = u0;
    u[2] = u0;
    // Evaluate the spline at the parameter t

    parametricspline->Evaluate(u, Pt, Du);

    // check for anchor points and remove them from the reference points
    for (auto iter = Anchorpoints->Begin(); iter != Anchorpoints->End(); iter++)
    {
      if (((round(iter.Value()[0] * 100.0) / 100.0) != (round(Pt[0] * 100.0) / 100.0) ||
           (round(iter.Value()[1] * 100.0) / 100.0) != (round(Pt[1] * 100.0) / 100.0) ||
           (round(iter.Value()[2] * 100.0) / 100.0) != (round(Pt[2] * 100.0) / 100.0)) &&
          ((round(lastPt[0] * 100.0) / 100.0) != (round(Pt[0] * 100.0) / 100.0) ||
           (round(lastPt[1] * 100.0) / 100.0) != (round(Pt[1] * 100.0) / 100.0) ||
           (round(lastPt[2] * 100.0) / 100.0) != (round(Pt[2] * 100.0) / 100.0)))
      {
        referencePointCounter = referencePointCounter + 1;
        lastPt[0] = Pt[0];
        lastPt[1] = Pt[1];
        lastPt[2] = Pt[2];
        if (m_Debug)
        {
          MITK_INFO << "PT values (Ref Points) @ " << referencePointCounter << " : "
                    << " x: " << Pt[0] << " y: " << Pt[1] << " z: " << Pt[2] << std::endl;
        }
        tempReferencePoints->InsertPoint(referencePointCounter, Pt, 0);
      }
    }
  }

  auto timestamp2 = high_resolution_clock::now();
  auto ms_int = std::chrono::duration_cast<milliseconds>(timestamp2 - timestamp1);
  if (m_Debug)
  {
    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = timestamp2 - timestamp1;
    double sec_double = ms_double.count() / 1000;
    double min_int = sec_double / 60;
    MITK_INFO << "Evaluate Performance Measurment: " << std::endl;
    MITK_INFO << "ms-Double: " << ms_double.count() << " ms" << std::endl;
    MITK_INFO << "ms-Integer: " << ms_int.count() << "ms" << std::endl;
    MITK_INFO << "sec-Double: " << sec_double << " sec" << std::endl;
    MITK_INFO << "sec-Integer: " << int(sec_double) << " sec" << std::endl;
    MITK_INFO << "minutes-Integer: " << min_int << " min" << std::endl;
  }

  return tempReferencePoints;
}


mitk::PointSet::Pointer SplineVisualisationEvaluationUtil::GetKochanekOptimisations(
  vtkSmartPointer<vtkParametricSpline> parametricspline, mitk::PointSet::Pointer splinePointSet, double currentRefDoubleSelection) {

  // first initialisation
  double selected_bais, selected_continuity, selected_tension;
  std::map<double, double> baisAvgEucli, continuityAvgEucli, tensionAvgEucli;
  double AVGEuclidian;
  vtkSmartPointer<vtkKochanekSpline> xSpline = vtkSmartPointer<vtkKochanekSpline>::New();
  vtkSmartPointer<vtkKochanekSpline> ySpline = vtkSmartPointer<vtkKochanekSpline>::New();
  vtkSmartPointer<vtkKochanekSpline> zSpline = vtkSmartPointer<vtkKochanekSpline>::New();

  parametricspline->SetXSpline(xSpline);
  parametricspline->SetYSpline(ySpline);
  parametricspline->SetZSpline(zSpline);
  parametricspline->SetPoints(ConvertMITKIntoVTKPointset(splinePointSet));
  m_SplineReferencePointSet =
    GenerateSplinePoints(parametricspline,
                         splinePointSet,
                         currentRefDoubleSelection );

  mitk::PointSet::Pointer groundtruth;
  std::vector<vtkSmartPointer<vtkKochanekSpline>> kochVec;
  kochVec.push_back(xSpline);
  kochVec.push_back(ySpline);
  kochVec.push_back(zSpline);

  m_SplineVisualisationPlugin->SetGroundTruthInPointSetListWidget();
  // best in class bais, continuity, tension calc
  //TODO make Bidirectional search for best results , fastest search time
  auto iter = kochVec.begin();
  for (double i = -0.3; i <= 0.3; i = i + 0.1)
  {
    for (double j = -1.0; j <= 1.0; j = j + 0.1)
    {
      for (double k = -1.0; k <= 1.0; k = k + 0.1)
      {
        iter->GetPointer()->SetDefaultBias(i);
        iter->GetPointer()->SetDefaultContinuity(j);
        iter->GetPointer()->SetDefaultTension(k);
        parametricspline->SetXSpline(iter->GetPointer());
        auto nxt = std::next(iter, 1);
        parametricspline->SetYSpline(nxt->GetPointer());
        auto nxtnxt = std::next(nxt, 1);
        parametricspline->SetZSpline(nxtnxt->GetPointer());
        parametricspline->SetPoints(ConvertMITKIntoVTKPointset(splinePointSet));
        m_SplineReferencePointSet = GenerateSplinePoints(parametricspline, splinePointSet, currentRefDoubleSelection);
        groundtruth = KdTreeLocator(
          m_SplineVisualisationPlugin->GetCurrentPointsetInListWidget(), m_SplineReferencePointSet, false, "Closest GT Points");
        AVGEuclidian = CalculateEuclideanDistance(groundtruth, m_SplineReferencePointSet);
        baisAvgEucli.insert({i, AVGEuclidian});
        continuityAvgEucli.insert({j, AVGEuclidian});
        tensionAvgEucli.insert({k, AVGEuclidian});
        MITK_INFO << "----------------------------------------------------------"
                 << "\n";
        MITK_INFO << "Current Bais : " << i << "\n";
        MITK_INFO << "Current Continuity : " << j << "\n";
        MITK_INFO << "Current Tension : " << k << "\n";
      }
    }
  }

  selected_bais = GetMinimumOfMap(baisAvgEucli);
  selected_continuity = GetMinimumOfMap(continuityAvgEucli);
  selected_tension = GetMinimumOfMap(tensionAvgEucli);
  MITK_INFO << "Best-in-Class Bais Value for current Kochanek Spline: " << selected_bais << std::endl;
  MITK_INFO << "Best-in-Class Tension Value for current Kochanek Spline: " << selected_tension << std::endl;
  MITK_INFO << "Best-in-Class Continuity Value for current Kochanek Spline: " << selected_continuity << std::endl;

  xSpline->SetDefaultBias(selected_bais);
  xSpline->SetDefaultContinuity(selected_continuity);
  xSpline->SetDefaultTension(selected_tension);
  parametricspline->SetXSpline(xSpline);
  ySpline->SetDefaultBias(selected_bais);
  ySpline->SetDefaultContinuity(selected_continuity);
  ySpline->SetDefaultTension(selected_tension);
  parametricspline->SetYSpline(ySpline);
  zSpline->SetDefaultBias(selected_bais);
  zSpline->SetDefaultContinuity(selected_continuity);
  zSpline->SetDefaultTension(selected_tension);
  parametricspline->SetZSpline(zSpline);
  parametricspline->SetPoints(ConvertMITKIntoVTKPointset(splinePointSet));
  m_SplineReferencePointSet = GenerateSplinePoints(parametricspline, splinePointSet, currentRefDoubleSelection);
  return m_SplineReferencePointSet;
}



mitk::SurfaceVtkMapper3D::Pointer SplineVisualisationEvaluationUtil::ClearTubeVtkMapperProps(
  mitk::BaseRenderer *renderer, mitk::SurfaceVtkMapper3D::Pointer tubeVtkMapper)
{
  // Update Mapper with null Pointer
  mitk::Point3D zp, zp1, zp2;
  zp[0] = zp[1] = zp[2] = 0;
  zp2 = zp1 = zp;
  mitk::PointSet::Pointer emptyPointset = mitk::PointSet::New();
  emptyPointset.GetPointer()->SetPoint(0, zp, 0);
  emptyPointset.GetPointer()->SetPoint(1, zp1, 0);
  emptyPointset.GetPointer()->SetPoint(2, zp2, 0);
  mitk::DataNode::Pointer emptyDatanode = mitk::DataNode::New();
  emptyDatanode->SetData(emptyPointset);
  emptyDatanode->SetName("EmptyDN");
  emptyDatanode->SetProperty("show contour", mitk::BoolProperty::New(false));
  tubeVtkMapper->SetDataNode(emptyDatanode);
  emptyDatanode->SetMapper(mitk::BaseRenderer::StandardMapperSlot::Standard3D, tubeVtkMapper);
  tubeVtkMapper->Update(renderer);
  m_SplineVisualisationPlugin->m_DataStorage->Remove(
      m_SplineVisualisationPlugin->GetPluginDataStorage()->GetNamedNode(tubeVtkMapper->GetDataNode()->GetName()));
  m_SplineVisualisationPlugin->m_DataStorage->Add(tubeVtkMapper->GetDataNode());
  InitClearProps(renderer);
  return tubeVtkMapper;
}


vtkSmartPointer<vtkImageData> SplineVisualisationEvaluationUtil::convertVTKPolydataToVTKImagedata(
  vtkSmartPointer<vtkPolyData> polydata,
  std::vector<int> dimensions,
  int padding = 1)
{
  double xmin = polydata->GetBounds()[0];
  double xmax = polydata->GetBounds()[1];
  double ymin = polydata->GetBounds()[2];
  double ymax = polydata->GetBounds()[3];
  double zmin = polydata->GetBounds()[4];
  double zmax = polydata->GetBounds()[5];
  int dx = dimensions[0];
  int dy = dimensions[1];
  int dz = dimensions[2];

 // Calculating spacing
  double sx = (xmax - xmin) / dx;
  double sy = (ymax - ymin) / dy;
  double sz = (zmax - zmin) / dz;

 // Calculating Origin
  double ox = xmin + sx / 2.0;
  double oy = ymin + sy / 2.0;
  double oz = zmin + sz / 2.0;

  if (padding == 1)
  {
    ox -= sx;
    oy -= sy;
    oz -= sz;

    dx += 2 * padding;
    dy += 2 * padding;
    dz += 2 * padding;
  }
  vtkNew<vtkImageData> image;
  image->SetSpacing(sx, sy, sz);
  image->SetDimensions(dx, dy, dz);
  image->SetExtent(0, dx - 1, 0, dy - 1, 0, dz - 1);
  image->SetOrigin(ox, oy, oz);
  image->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
  const int inval = 255;
  const int outval = 0;

  for (int i = 0; i != image->GetNumberOfPoints(); ++i){
    image->GetPointData()->GetScalars()->SetTuple1(i, inval);
  }

  vtkNew<vtkPolyDataToImageStencil> pol2stenc;
  pol2stenc->SetInputData(polydata);
  pol2stenc->SetOutputOrigin(ox, oy, oz);
  pol2stenc->SetOutputSpacing(sx, sy, sz);
  pol2stenc->SetOutputWholeExtent(image->GetExtent());
  pol2stenc->Update();

  vtkNew<vtkImageStencil> imgstenc;
  imgstenc->SetInputData(image);
  imgstenc->SetStencilConnection(pol2stenc->GetOutputPort());
  imgstenc->ReverseStencilOff();
  imgstenc->SetBackgroundValue(outval);
  imgstenc->Update();

  return imgstenc->GetOutput();
}

vtkSmartPointer<vtkTupleInterpolator>
  SplineVisualisationEvaluationUtil::GetInterpolatedRadiusValuesFromTupleInterpolator( vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius, mitk::PointSet::Pointer refPointSet)
{
  double rad;  //1-5
  double xDiffrence;
  double yDiffrence;
  double zDiffrence;
  auto iter = refPointSet->Begin();
  mitk::Point3D firstpoint;
  mitk::Point3D secondpoint;
    for (auto iter = refPointSet->Begin(); iter != refPointSet->End(); ++iter)
    {
      firstpoint = iter.Value();
      ++std::move(iter);
      if (iter != refPointSet->End())
      {
        secondpoint = iter.Value();
      }
      else
      {
        --std::move(iter);
        secondpoint = iter.Value();
      }

      xDiffrence = abs(firstpoint[0]) - abs(secondpoint[0]);
      yDiffrence = abs(firstpoint[1]) - abs(secondpoint[1]);
      zDiffrence = abs(firstpoint[2]) - abs(secondpoint[2]);
      if (m_Debug)
      {
        MITK_INFO << " xfirst : " << firstpoint[0] << " yfirst : " << firstpoint[1] << " zfirst : " << firstpoint[2] << std::endl;
        MITK_INFO << " xsec : " << secondpoint[0] << " ysec : " << secondpoint[1] << " zsec : " << secondpoint[2] << std::endl;
        MITK_INFO << " xDif : " << abs(xDiffrence) << " yDif : " << abs(yDiffrence) << " zDif : " << abs(zDiffrence) << std::endl;
      }

      if (45.0 < abs(xDiffrence) || 45.0 < abs(yDiffrence) || 45.0 < abs(zDiffrence) )
      {
        rad = 1.0;
      }
      else if (35 < abs(xDiffrence) && abs(xDiffrence) < 45 || 35 < abs(yDiffrence) && abs(yDiffrence) < 45.0 ||
               35 < abs(zDiffrence) && abs(zDiffrence) < 45.0)
      {
        rad = 1.5;
      }
      else if (30 < abs(xDiffrence) && abs(xDiffrence) < 35 || 30 < abs(yDiffrence) && abs(yDiffrence) < 35 ||
               30 < abs(zDiffrence) && abs(zDiffrence) < 35)
      {
        rad = 2.0;
      }
      else if (25 < abs(xDiffrence) && abs(xDiffrence) < 30 || 25 < abs(yDiffrence) && abs(yDiffrence) < 30.0 ||
          25 < abs(zDiffrence) && abs(zDiffrence) < 30.0)
      {
        rad = 2.5;
      }
      else if (20 < abs(xDiffrence) && abs(xDiffrence) < 25 || 20 < abs(yDiffrence) && abs(yDiffrence) < 25 ||
               20 < abs(zDiffrence) && abs(zDiffrence) < 25)
      {
        rad = 3.0;
      }
      else if (15 < abs(xDiffrence) && abs(xDiffrence) < 20 || 15 < abs(yDiffrence) && abs(yDiffrence) < 20 ||
               15 < abs(zDiffrence) && abs(zDiffrence) < 20)
      {
        rad = 3.5;
      }
      else if (10 < abs(xDiffrence) && abs(xDiffrence) < 15 || 10 < abs(yDiffrence) && abs(yDiffrence) < 15 ||
               10 < abs(zDiffrence) && abs(zDiffrence) < 15)
      {
        rad = 4.0;
      }
      else if (5 < abs(xDiffrence) && abs(xDiffrence) < 10 || 5 < abs(yDiffrence) && abs(yDiffrence) < 10 ||
               5 < abs(zDiffrence) && abs(zDiffrence) < 10)
      {
        rad = 4.5;
      }
      else if (0.01 < abs(xDiffrence) && abs(xDiffrence) < 2.5 || 0.01 < abs(yDiffrence) && abs(yDiffrence) < 2.5 ||
               0.01 < abs(zDiffrence) && abs(zDiffrence) < 2.5)
      {
        rad = 5.0;
      }
      else if (0.01 < abs(xDiffrence) && abs(xDiffrence) < 2.5 || 0.01 < abs(yDiffrence) && abs(yDiffrence) < 2.5 ||
          0.01 < abs(zDiffrence) && abs(zDiffrence) < 2.5)
      {
        rad = 5.5;
      }
      if (m_Debug)
      {
        MITK_INFO << "index: " << iter.Index() << " selected rad: " << rad << std::endl;
      }
      interpolatedRadius->AddTuple(iter.Index(), &rad);
  }
  return interpolatedRadius;
}

vtkSmartPointer<vtkTupleInterpolator>
  SplineVisualisationEvaluationUtil::GetInterpolatedRadiusValuesFromTupleInterpolatorTime(
    vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius, mitk::PointSet::Pointer refPointSet)
{
  double rad = 5;
  for (auto iter = refPointSet->Begin(); iter != refPointSet->End(); ++iter)
  {
    rad = rad - 0.15;
    if (m_Debug)
    {
      MITK_INFO << "index: " << iter.Index() << " selected rad: " << rad << std::endl;
    }
    interpolatedRadius->AddTuple(iter.Index(), &rad);
  }
  return interpolatedRadius;
}

mitk::SplineVtkMapper3D::Pointer SplineVisualisationEvaluationUtil::ClearSplineVtkMapperProps(
  mitk::BaseRenderer *renderer,
  mitk::SplineVtkMapper3D::Pointer splineVtkMapper)
{
  // Update Mapper with null Pointer
  mitk::Point3D zp, zp1, zp2;
  zp[0] = zp[1] = zp[2] = 0;
  zp2 = zp1 = zp;
  mitk::PointSet::Pointer emptyPointset = mitk::PointSet::New();
  emptyPointset.GetPointer()->SetPoint(0, zp, 0);
  emptyPointset.GetPointer()->SetPoint(1, zp1, 0);
  emptyPointset.GetPointer()->SetPoint(2, zp2, 0);
  mitk::DataNode::Pointer emptyDatanode = mitk::DataNode::New();
  emptyDatanode->SetData(emptyPointset);
  emptyDatanode->SetName("EmptyDN");
  emptyDatanode->SetProperty("show contour", mitk::BoolProperty::New(false));
  splineVtkMapper->SetDataNode(emptyDatanode);
  emptyDatanode->SetMapper(mitk::BaseRenderer::StandardMapperSlot::Standard3D, splineVtkMapper);
  splineVtkMapper->UpdateSpline();
  m_SplineVisualisationPlugin->m_DataStorage->Remove(
      m_SplineVisualisationPlugin->GetPluginDataStorage()->GetNamedNode(splineVtkMapper->GetDataNode()->GetName()));
  m_SplineVisualisationPlugin->m_DataStorage->Add(splineVtkMapper->GetDataNode());
  InitClearProps(renderer);
  return splineVtkMapper;
}


void SplineVisualisationEvaluationUtil::InitClearProps(
  mitk::BaseRenderer *renderer)
{
  renderer->GetVtkRenderer()->RemoveAllViewProps();
  renderer->ForceImmediateUpdate();
  mitk::RenderingManager::GetInstance()->AbortRendering();
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();

}

double SplineVisualisationEvaluationUtil::GetMinimumOfMap(std::map<double, double> inputMap)
{
  auto minIter = std::min_element(
    std::begin(inputMap), std::end(inputMap), [](const auto &l, const auto &r) { return l.second < r.second; });
  return minIter == inputMap.end() ? -1 : minIter->first;
}





mitk::PointSet::Pointer SplineVisualisationEvaluationUtil::GenerateAdditionalAnchorpoints(
  mitk::PointSet::Pointer Anchorpoints, int anchorpointsToGen)
{
  int i = 0;
  vtkSmartPointer<vtkBoxMuellerRandomSequence> randomSequence = vtkSmartPointer<vtkBoxMuellerRandomSequence>::New();
  mitk::PointSet::Pointer tempAnchorPoints = mitk::PointSet::New();
  mitk::BaseGeometry::Pointer baseGeometry = Anchorpoints->GetGeometry();
  mitk::Vector3D vectorSpacing = baseGeometry->GetSpacing();
  mitk::Point3D prevPoint;
  mitk::Point3D currentPoint;
  mitk::Point3D genPoint;
  auto iter = Anchorpoints->Begin();
  do {
    currentPoint = iter.Value();
    MITK_INFO << "Anchor Point (selected) values @ " << i << " : "
              << " x: " << iter.Value()[0] << " y: " << iter.Value()[1] << " z: " << iter.Value()[2] << std::endl;
    tempAnchorPoints->InsertPoint(i, currentPoint, 0);
    double randNum = exp(randomSequence.New()->GetScaledValue(0.0, 0.1));
    randomSequence->Next();
    if (iter != Anchorpoints->End())
    {
      ++iter;
    } 
    if (i == anchorpointsToGen - 1 || iter == Anchorpoints->End())
    {
      i = anchorpointsToGen - 1;
      // add random Box Mueller Gaussian distribution sequence 
      genPoint[0] = (currentPoint[0] + randNum); 
      genPoint[1] = (currentPoint[1] + randNum);
      genPoint[2] = (currentPoint[2] + randNum);
      tempAnchorPoints->InsertPoint(i, genPoint, 0);
      MITK_INFO << "Anchor Point (generated) values @ " << i << " : "
                << " x: " << genPoint[0] << " y: " << genPoint[1] << " z: " << genPoint[2] << std::endl;
      break;
    }
    
    FillVector3D(vectorSpacing, iter.Value()[0], iter.Value()[1], iter.Value()[2]);
    genPoint[0] = (currentPoint[0] + vectorSpacing[0]) / 2;
    genPoint[1] = (currentPoint[1] + vectorSpacing[1]) / 2;
    genPoint[2] = (currentPoint[2] + vectorSpacing[2]) / 2;
    i++;
   
    tempAnchorPoints->InsertPoint(i, genPoint, 0);
    MITK_INFO << "Anchor Point (generated) values @ " << i << " : "
              << " x: " << genPoint[0] << " y: " << genPoint[1] << " z: " << genPoint[2] << std::endl;
    i++;
        
  }
  while (i < anchorpointsToGen);

  return tempAnchorPoints;
}


vtkPoints *SplineVisualisationEvaluationUtil::ConvertMITKIntoVTKPointset(mitk::PointSet::Pointer pointSet)
{
  vtkPoints *vtkpoints = vtkPoints::New();
  for (auto iter = pointSet->Begin(); iter != pointSet->End(); iter++)
  {
    vtkpoints->InsertNextPoint(iter.Value()[0], iter.Value()[1], iter.Value()[2]);
  }

  return vtkpoints;
}

QString SplineVisualisationEvaluationUtil::VtkQStringTextBlock(mitk::PointSet::Pointer Anchorpoints,
                                                       mitk::PointSet::Pointer ReferenceSplinepoints)
{
  QString tempQString = "";
  // append the anchor points
  for (auto iter = Anchorpoints->Begin(); iter != Anchorpoints->End(); iter++)
  {
    tempQString.append("Anchor Point" + QString::number(iter.Index()) + " x: " + QString::number(iter.Value()[0]) +
                       " y: " + QString::number(iter.Value()[1]) + " z: " + QString::number(iter.Value()[2]) + "\n");
  }
  tempQString.append("\n");

  // append the reference points
  for (auto iter = ReferenceSplinepoints->Begin(); iter != ReferenceSplinepoints->End(); iter++)
  {
    tempQString.append("Ref. Point " + QString::number(iter.Index()) + " x: " + QString::number(iter.Value()[0]) +
                       " y: " + QString::number(iter.Value()[1]) + " z: " + QString::number(iter.Value()[2]) + "\n");
  }

  return tempQString;
}

void SplineVisualisationEvaluationUtil::WriteToErrorDataFile(std::string MSEInputs,
                                                            std::string RMSEInputs,
                                                            std::string EuclidInputs,
                                                            std::string VarianceInputs, bool printheader)
{
  

  std::ofstream outputfile;
  std::filesystem::path p = std::filesystem::current_path();
  auto dir_path = p.generic_string() + "/" + string("All Error Values (Combined Evaluation).csv");
               

  outputfile.open(dir_path, std::ios_base::app); 
  if (printheader)
  {
    outputfile << "MSE(x)"
               << ";"
               << "MSE(y)"
               << ";"
               << "MSE(z)"
               << ";"
               << "RMSE(x)"
               << ";"
               << "RMSE(y)"
               << ";"
               << "RMSE(z)"
               << ";"
               << "Euclidian Distance(AVG)"
               << ";"
               << "StandardDeviation(AVG)" << std::endl;
  }
  std::replace(MSEInputs.begin(), MSEInputs.end(), '.', ',');
  std::replace(RMSEInputs.begin(), RMSEInputs.end(), '.', ',');
  std::replace(EuclidInputs.begin(), EuclidInputs.end(), '.', ',');
  std::replace(VarianceInputs.begin(), VarianceInputs.end(), '.', ',');

  outputfile << MSEInputs << ";" << RMSEInputs << ";" << EuclidInputs << ";" << VarianceInputs << std::endl;
  outputfile.close();

}


void SplineVisualisationEvaluationUtil::WriteToCSVFile(mitk::PointSet::Pointer input)
{
  std::ofstream outputfile;
  bool ok;
  // Enter the name of the point set
  std::string outputname;
  if (input->GetObjectName() == "")
  {
    outputname = "CSV Reference Points"; // Spline Points 
  }
  else
  {
    outputname = input->GetObjectName();
  }

  QString csvPointsText = QInputDialog::getText(QApplication::activeWindow(),
                                                m_SplineVisualisationPlugin->tr("Add point set..."),
                                                m_SplineVisualisationPlugin->tr("Enter name for the CSV save file"),
                                                QLineEdit::Normal,
                                                outputname.c_str(),
                                                &ok);
  if (!ok || csvPointsText.isEmpty())
  {
    QMessageBox msgWarning;
    msgWarning.setText("Warning message:\n Point set name was empty. Stopping operation.");
    msgWarning.setIcon(QMessageBox::Warning);
    msgWarning.setWindowTitle("Warning: no point set name ");
    msgWarning.exec();
    return;
  }
  std::filesystem::path p = std::filesystem::current_path();
  string dir_path = p.generic_string() + "/" + csvPointsText.toStdString() + ".csv";

  outputfile.open(dir_path);
  for (auto iter = input.GetPointer()->Begin(); iter != input.GetPointer()->End(); iter++)
  {
    string val0 = std::to_string(iter.Value()[0]);
    string val1 = std::to_string(iter.Value()[1]);
    string val2 = std::to_string(iter.Value()[2]);
    std::replace(val0.begin(), val0.end(), '.', ',');
    std::replace(val1.begin(), val1.end(), '.', ',');
    std::replace(val2.begin(), val2.end(), '.', ',');
    outputfile << val0 << ";" << val1 << ";" << val2;
    outputfile << "\n";
  }
  outputfile.close();
  MITK_INFO << "Done writing to CSV File to: " << dir_path << std::endl;
}


