/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef MITKSPLINEVISUALISATIONEVALUATIONUTIL_H_HEADER_INCLUDED_
#define MITKSPLINEVISUALISATIONEVALUATIONUTIL_H_HEADER_INCLUDED_

#include <itksys/SystemTools.hxx>
#include <mitkDataNode.h>
#include <mitkRenderWindow.h>
#include <mitkSplineVtkMapper3D.h>
#include <mitkStandaloneDataStorage.h>
#include <mitkTrackingTool.h>
#include <mitkSurfaceVtkMapper3D.h>

// VTK imports
#include <vtkParametricSpline.h>
#include <vtkTestUtilities.h>
#include <vtkTupleInterpolator.h>

class SplineVisualisationPlugin;

/**
 * \brief SplineVisualisationEvaluationUtil - A support class to fullfill the nessasary needs and methods for the main spline visualisationplugin class 
 * and deliver the output of the RSME calculation and other methods to evaluate the reults of the spline output method. 
 *
 *
 * \sa mitk::SplineVisualisationPlugin
 * \ingroup IGT //MITKIGT_EXPORT

 */
class SplineVisualisationEvaluationUtil
{
public:
	
  mitk::PointSet::Pointer m_SplineReferencePointSet; // all selected evaluated reference Points on the calculated spline
  SplineVisualisationPlugin *m_SplineVisualisationPlugin; // SplineVisualisationPlugin Object
  mitk::StandaloneDataStorage::Pointer m_DataStorage; // 
  mitk::PointSet::Pointer m_CSVPointset;
  bool m_Debug;
  /**
   * \brief Constructor
   */
  SplineVisualisationEvaluationUtil();
  /**
   * \brief Destructor
   */
  ~SplineVisualisationEvaluationUtil();
  /**
   * \brief setting debug mode
   * \param setting : boolean 
   */
  void SetDebug(bool setting);
  /**
   * \brief Calculates the average standard deviation for the closest pointset each point perpendicular to the spline points
   * \param groundtruth (em sensor points)
   * \param measuredvalues (spline points)
   * \return vector of double values of the aggregated data
   */
  std::vector<double> CalculateAVGStandartDeviation(mitk::PointSet::Pointer groundtruth,
                                                    mitk::PointSet::Pointer measuredvalues);
  /**
   * \brief Calculates the euclidain distance values of the closest groundtruth pointset  to the spline pointset
   * \param groundtruth (em sensor points)
   * \param measuredvalues (spline points)
   * \return the double value of the euclidiean distance function. 
   */
  double CalculateEuclideanDistance(mitk::PointSet::Pointer groundtruth, mitk::PointSet::Pointer measuredvalues);
  /**
   * \brief Calculates the mean square error of the closest groundtruth pointset  to the spline pointset
   * \param groundtruth (em sensor points)
   * \param measuredvalues  (spline points)
   * \return returns the MSE for each axis
   */
  mitk::Point3D CalculateMSE(mitk::PointSet::Pointer groundtruth, mitk::PointSet::Pointer measuredvalues);
  /**
   * \brief Calculates the Root mean sqare error based on the average mean sqare error values for each axis
   * \param AVGMSE from the CalculateMSE method
   * \return the RMSE for each axis
   */
  mitk::Point3D CalculateRMSE(mitk::Point3D AVGMSE);
  /**
   * \brief Uses the vtkDKTreeLocator for locating the closest groundtruth (em sensor) point perpendicular to the measured spline point 
    * \param groundtruth (em sensor points)
   * \param measuredvalues  (spline points)
   * \param writeToFile 
   * \param windowInput distinction of input file for writing to csv
   * \return the pointset
   */
  mitk::PointSet::Pointer KdTreeLocator(mitk::PointSet::Pointer groundtruth,
                                        mitk::PointSet::Pointer measuredvalues,
                                        bool writeToFile,
                                        std::string windowInput);
  /**
   * \brief  clears the vtk rendering of viewports, which are objects that controll the rendering process for objects. Additionally, *cleaing the rendering removes geometry, lights, camera view. The coordinate transformation between world coordinates, view coordinates will be reset
   * \param renderer 
   */
  void InitClearProps(
    mitk::BaseRenderer *renderer);
  /**
   * \brief Resets the vtkViewports and splinevtkmapper3d data node for updating the spline once visualised on the screen 
   * \param renderer 
   * \param splineVtkMapper , includes the datanode for the spline points 
   * \return  the updated reset-ed spline datanode
   */
  mitk::SplineVtkMapper3D::Pointer ClearSplineVtkMapperProps(
    mitk::BaseRenderer *renderer,
    mitk::SplineVtkMapper3D::Pointer splineVtkMapper);
  /**
   * \brief Resets the vtkViewports and surfacevtkmapper for reseting data node once the surface has been visualised on the screen
   * \param renderer
   * \param surfaceVtkMapper , includes the datanode for the surface points
   * \return the updated reset-ed surface datanode
   *
   */
  mitk::SurfaceVtkMapper3D::Pointer ClearTubeVtkMapperProps(mitk::BaseRenderer *renderer,
                                                            mitk::SurfaceVtkMapper3D::Pointer tubeVtkMapper);
  /**
   * \brief Support method for the optimsied kochanek spline method
   * \param inputMap std::map with the index of the calculation and either the  bais, tension or continuity values 
   * \return the minimum value of the map
   */
  double GetMinimumOfMap(std::map<double, double> inputMap);
  /**
   * \brief Generate additional spline points feature
   * \param Anchorpoints 
   * \param anchorpointsToGen 
   * \return spline pointset
   */
  mitk::PointSet::Pointer GenerateAdditionalAnchorpoints(mitk::PointSet::Pointer Anchorpoints, int anchorpointsToGen);

  /**
   * \brief 
   * \param pointSet 
   * \return 
   */
  vtkPoints *ConvertMITKIntoVTKPointset(mitk::PointSet::Pointer pointSet);

  QString VtkQStringTextBlock(mitk::PointSet::Pointer Anchorpoints, mitk::PointSet::Pointer ReferenceSplinepoints);

  mitk::DataNode::Pointer StartReadPointSet();

  QString SearchForLogfiles();

  vtkSmartPointer<vtkTupleInterpolator> GetInterpolatedRadiusValuesFromTupleInterpolator(
    vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius, mitk::PointSet::Pointer refPointSet);

   /*
   * \brief CSV Loading Functions
   */
  void ReadCSVPoints();

  mitk::PointSet::Pointer GenerateSplinePoints(vtkParametricSpline *parametricspline,
                                               mitk::PointSet::Pointer Anchorpoints,
                                               double iteration_range);

  void WriteToCSVFile(mitk::PointSet::Pointer input);

  mitk::PointSet::Pointer GetCSVPointset();

  void SetCSVPointset(mitk::PointSet::Pointer input);


  /*
   * \brief dynamically adjusts the bais, tention and continuity for the current spline
   */
  mitk::PointSet::Pointer GetKochanekOptimisations(
    vtkSmartPointer<vtkParametricSpline> parametricspline,
    mitk::PointSet::Pointer splinePointSet,
    double currentRefDoubleSelection);

  double CalculateDirectionalDerivativeDeviation(mitk::PointSet::Pointer groundtruth,
                                                 mitk::PointSet::Pointer measuredvalues);

  void CalculateParametricErrorMeasure(mitk::PointSet::Pointer  groundtruth,
                                        mitk::PointSet::Pointer measuredvalues,
                                       std::string windowInput,
                                       bool writeToFile,
                                       bool writeAllToFile,
                                       bool printheader);
  double CalculateAVGVTKPoints(vtkSmartPointer<vtkPoints> inputPoints, int xyz);
  

  void WriteToErrorDataFile(std::string MSEInputs,
                            std::string RMSEInputs,
                            std::string EuclidInputs,
                            std::string VarianceInputs, bool printheader);


  double GetconvertedPixelSizeInMM(mitk::BaseRenderer *renderer, const mitk::Point3D &pos);

  double RoundUp(double value);

 vtkSmartPointer<vtkImageData> convertVTKPolydataToVTKImagedata(vtkSmartPointer<vtkPolyData> polydata,
                                                                 std::vector<int> dimensions,
                                                                 int padding);
  
  vtkSmartPointer<vtkTupleInterpolator> GetInterpolatedRadiusValuesFromTupleInterpolatorTime(
    vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius, mitk::PointSet::Pointer refPointSet);

protected:
  // PointSet
  static std::vector<double> TimeStamp_Tool0;
  static std::vector<double> Valid_Tool0;
  static std::vector<double> X_Tool0;
  static std::vector<double> Y_Tool0;
  static std::vector<double> Z_Tool0;
  static std::vector<double> QX_Tool0;
  static std::vector<double> QY_Tool0;
  static std::vector<double> QZ_Tool0;
  static std::vector<double> QR_Tool0;
  static std::vector<double> TimeStamp_Tool1;
  static std::vector<double> Valid_Tool1;
  static std::vector<double> X_Tool1;
  static std::vector<double> Y_Tool1;
  static std::vector<double> Z_Tool1;
  static std::vector<double> QX_Tool1;
  static std::vector<double> QY_Tool1;
  static std::vector<double> QZ_Tool1;
  static std::vector<double> QR_Tool1;
  static std::vector<double> TimeStamp_Tool2;
  static std::vector<double> Valid_Tool2;
  static std::vector<double> X_Tool2;
  static std::vector<double> Y_Tool2;
  static std::vector<double> Z_Tool2;
  static std::vector<double> QX_Tool2;
  static std::vector<double> QY_Tool2;
  static std::vector<double> QZ_Tool2;
  static std::vector<double> QR_Tool2;
  static std::vector<double> TimeStamp_Tool3;
  static std::vector<double> Valid_Tool3;
  static std::vector<double> X_Tool3;
  static std::vector<double> Y_Tool3;
  static std::vector<double> Z_Tool3;
  static std::vector<double> QX_Tool3;
  static std::vector<double> QY_Tool3;
  static std::vector<double> QZ_Tool3;
  static std::vector<double> QR_Tool3;

  long CounterCoordinates;
};

#endif /* MITKSPLINEVISUALISATIONEVALUATIONUTIL_H_HEADER_INCLUDED_ */
