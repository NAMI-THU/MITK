/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/


#ifndef MITKSPLINEDATAOBJECTVISUALIZATIONFILTER_H_HEADER_INCLUDED_
#define MITKSPLINEDATAOBJECTVISUALIZATIONFILTER_H_HEADER_INCLUDED_

#include "mitkNavigationDataToNavigationDataFilter.h"
#include "mitkNavigationDataObjectVisualizationFilter.h"
#include <internal/mitkSplineVisualisationEvaluationUtil.h>
#include "mitkNavigationData.h"
#include "mitkBaseData.h"
#include <mitkGeometry3D.h>


#include <mitkVirtualTrackingDevice.h>
#include <mitkTrackingDeviceSource.h>
#include <mitkDataNode.h>
#include <vtkNamedColors.h>

#include <mitkNavigationToolStorage.h>
#include <mitkBaseRenderer.h>
#include <mitkStandaloneDataStorage.h>
#include <mitkSplineVtkMapper3D.h>
#include <..\..\..\..\mitk\Plugins\org.mitk.gui.qt.igttracking\src\internal\QmitkMITKIGTTrackingToolboxView.h>
#include <..\..\..\..\mitk\Plugins\org.mitk.gui.qt.igttracking\src\internal\QmitkMITKIGTTrackingToolboxViewWorker.h>
#include <internal\QmitkMITKIGTTrackingToolboxViewWorker.h>

class SplineVisualisationPlugin;
class SplineVisualisationEvaluationUtil;
  /**
   * \brief Class that reads NavigationData from input and transfers the information to the geometry of the associated
   * BaseData for the Spline Output Filter 
   * Similary, the NavigationDataToNavigationDataFilter is the base class of all filters that receive
  * NavigationDatas as input and produce NavigationDatas as output
  *
  * Base class that for the spline navigation filters that receive NavigationData objects as input
  * and produce NavigationData objects as output.
  * This class defines the input-interface for NavigationDataFilters.
   *
   * Derived from NavigationDataToNavigationDataFilter and NavigationDataObjectVisualizationFilter ( public mitk::NavigationDataObjectVisualizationFilter)
   *
   * 
   * \sa mitk::NavigationDataToNavigationDataFilter
   * \ingroup IGT //MITKIGT_EXPORT

   */
class SplineDataObjectVisualizationFilter:  public mitk::NavigationDataToNavigationDataFilter
{

public:

  bool m_ResetRenderer; // resets the Rendering mechanism 
  bool m_ShowVTKSubwindow; // optional VTK Renderwindow with spline   
  bool m_Debug; // if output is needed
  QString TempString = " "; // a temporare QString for point output in the VTK Renderwindow
  QTimer *m_TrackingLoggingTimer; // the Tracking logging Timer for aborting operations
  unsigned int m_MouseMotion; //  mouse motion count for the picker intergration
  int NumberOfHandles; // 
  int m_EMRefreshrate; // Refresh rate Settings
  double Bound;  // Bound of tracking range edge
  double m_CurrentRefDoubleSelection;
  int m_CurrentSensoryIndex;
  int m_SelectionType;
  bool m_appendPointset;
  int m_Timestep;
  std::vector<mitk::NavigationData::Pointer> m_VecEMBaseData;

  mitk::NavigationToolStorage::Pointer m_ToolStorage; // class wide instantiation for the tool storage
  mitk::StandaloneDataStorage::Pointer DataStorage;// class wide instantiation for the standalone data storage
  mitk::TrackingDeviceType::pointer m_TrackingDeviceType; // class wide instantiation for tracking device type
  mitk::Geometry3D::Pointer m_ToolIconVisualization;
  mitk::VirtualTrackingDevice::Pointer Emtracker;
  mitk::NavigationData::Pointer EmTool0, EmTool1, EmTool2, EmTool3, EmTool4, EmTool5, EmTool6, EmTool7; // // class wide instantiation of all possible tools (currently #4 available)
  mitk::ScalarType Bounds[6];
  mitk::TrackingDeviceSource::Pointer Emsource;
  mitk::DataInteractor::Pointer DataInteractor;
  mitk::PointSet::Pointer m_SplineReferencePointSet;
  mitk::PointSet::Pointer m_combinedPointSet;
  mitk::PointSet::Pointer m_lastAnchorPointSet;


  mitk::DataNode::Pointer OutputDataNode;
  mitk::DataNode::Pointer SplineNode;
  mitk::SplineVtkMapper3D::Pointer SplineVtkMapper;
  vtkRenderWindow *VtkrenderWindow;
  vtkRenderer *Vtkrenderer;
  QmitkMITKIGTTrackingToolboxViewWorker *m_Worker;
  SplineVisualisationPlugin *m_SplineVisualPlugin;
  SplineVisualisationEvaluationUtil *m_SplineVisualisationEvalUtil;

  mitk::NavigationDataObjectVisualizationFilter::RepresentationVectorPointerMap m_RepresentationVectorMap;

  mitk::NavigationDataObjectVisualizationFilter::RotationMode m_RotationMode; ///< defines the rotation mode Standard or Transposed, Standard is default

  /**
   * \brief Constructor
   **/
  SplineDataObjectVisualizationFilter(mitk::NavigationToolStorage::Pointer m_toolStorage,
                                      mitk::StandaloneDataStorage::Pointer dataStorage);
  /**
   * \brief Destructor
   **/
  ~SplineDataObjectVisualizationFilter() override;

  /*
  * \brief Get the Spline Datanode from the selected converted VTK Spline Reprensentation Visualisation kind
  */
 mitk::DataNode::Pointer GetSplineDataNode(std::vector<mitk::NavigationData::Pointer> vecEMToolData);

 void FreezeUnFreezeStandartView(bool freeze, int currentSensoryIndex);

 /*
 * \brief Get the converted parametric spline representation for the Standard 3D View 
 */
  mitk::DataNode::Pointer ConvertParametricSplineRepresentation(mitk::PointSet::Pointer AnchorPointSet);

  mitk::DataNode::Pointer Convert3DTubeRepresentation(mitk::PointSet::Pointer AnchorPointSet);

  mitk::DataNode::Pointer ConvertKochanekSplineRepresentation(mitk::PointSet::Pointer AnchorPointSet,
                                                              bool optimizsation);

  mitk::DataNode::Pointer ConvertCardinalSplineRepresentation(mitk::PointSet::Pointer AnchorPointSet);

  mitk::DataNode::Pointer ConvertBSplineRepresentation(mitk::PointSet::Pointer AnchorPointSet);

  mitk::DataNode::Pointer ConvertSCurveRepresentation(mitk::PointSet::Pointer AnchorPointSet);

  mitk::DataNode::Pointer InputTrackingData(
    QmitkNavigationDataSourceSelectionWidget *navidatasourceselection,
    mitk::StandaloneDataStorage::Pointer dataStorage,
    bool appendPointset);

 void  InitialiseInputTrackingData(
   int currentSensoryIndex, int selectiontype, bool resetRenderer, double parametricRange, bool debug);

  void ExecuteVtkRendering(vtkSmartPointer<vtkNamedColors> colors,
                          vtkActor *polydataActor,
                          vtkActor *splineFuncActor,
                          vtkActor *sphereAnchorPointActor,
                          vtkSmartPointer<vtkTextActor> textSource);

  mitk::PointSet::Pointer GettingCombinedPointset(mitk::PointSet::Pointer AnchorPointSet);

  mitk::BaseData::Pointer GetRepresentationObject(unsigned int idx) const;

  void SetRepresentationObject(unsigned int idx, mitk::BaseData::Pointer data);

  void SetRepresentationObjects(unsigned int idx, const std::vector<mitk::BaseData::Pointer> &data);

double randomSplineValue(double startoffset, double splineRangeval);

 protected:
  typedef std::map<itk::ProcessObject::DataObjectPointerArraySizeType, bool> BooleanInputMap;
  typedef std::map<unsigned int, mitk::AffineTransform3D::Pointer> OffsetPointerMap;

 
};


#endif /* MITKSPLINEDATAOBJECTVISUALIZATIONFILTER_H_HEADER_INCLUDED_ */
