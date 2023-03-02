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
#ifndef SplineVisualisationPlugin_h
#define SplineVisualisationPlugin_h

#include <QmitkAbstractView.h>
#include <ui_SplineVisualisationPluginControls.h>

// Blueberry

// Qt
// #include<Qt3DAnimation/>

// standard imports
#include <ctkServiceEvent.h>
#include <ctkServiceReference.h>
#include <new.h>
#include <string>

// MITK imports
// mitk image
#include <mitkImage.h>
// PointSet
#include <mitkPointSet.h>
// inheriting class...
#include "mitkGL.h"
#include <DataManagement/mitkTrackingDeviceTypeCollection.h>
#include <internal/mitkSplineDataObjectVisualizationFilter.h>
#include <mitkCameraVisualization.h>
#include <mitkDataNode.h>
#include <mitkIRenderWindowPartListener.h>
#include <mitkNavigationData.h>
#include <mitkNavigationDataObjectVisualizationFilter.h>
#include <mitkNavigationToolStorage.h>
#include <mitkSplineVtkMapper3D.h>
#include <mitkStandaloneDataStorage.h>
#include <mitkTrackingDeviceSource.h>
#include <mitkVirtualTrackingDevice.h>
#include <mitkVtkAnnotation3D.h>
#include "QmitkStdMultiWidget.h"
#include <mitkSurfaceVtkMapper3D.h>
#include <..\..\..\..\mitk\Plugins\org.mitk.gui.qt.igttracking\src\internal\QmitkMITKIGTTrackingToolboxViewWorker.h>
#include <internal\QmitkMITKIGTTrackingToolboxViewWorker.h>
// VTK imports
#include <vtkParametricSpline.h>
#include <vtkPolyData.h>
#include <vtkTestUtilities.h>
#include <internal/mitkSplineVisualisationEvaluationUtil.h>

/*
\brief SplineVisualisationPlugin - The Spline Visualisation Plugin consists of both
static and dynamic tracking mechanisms of (EM) Sensors, to deliver a coherent continuous representation of vtk inherited
spline methods for path visualisation of possible endoscopic equipment while traveling through a patients abdomen.
Several diffrent predefined options for targeting the correct region of interest, sensor count and visualisation method
(2D and 3D) exist.


\sa QmitkAbstractView
\sa itk::IRenderWindowPartListener
\ingroup ${plugin_target}_internal
*/

class SplineDataObjectVisualizationFilter;
class SplineVisualisationEvaluationUtil;

class SplineVisualisationPlugin : public QmitkAbstractView,
                                  public mitk::IRenderWindowPartListener
{
  // this is needed for all Qt objects that should have a Qt meta-object
  // (everything that derives from QObject and wants to have signal/slots)
  Q_OBJECT

public:

  
  static const char *ID_ARRAY; // Signature unique identifier
  QTimer *m_TrackingRenderTimer;
  /** @brief This timer updates the IGT pipline and also the logging filter if logging is activated.*/
  QTimer *m_TrackingLoggingTimer;
  QTimer *m_TimeoutTimer;
  QTimer *m_PointSetTimer;
  bool m_Logging;          ///> bool which is true if logging is running, false if not
  bool m_ShowHideToolAxis; ///> bool, which will be true, if the tool axis is visible during tracking
  int m_LoggedFrames;      ///> stores the current number of logged frames if logging is on
  int m_Selectiontype;
  bool m_reConnect;
  bool m_reExecuteSpline; // check if spline mapper already executed otherwise reset mapper
  bool m_reExecuteTube; // check if tube mapper already executed otherwise reset mapper
  int m_Fps = 30;                // or 20 or /10/60
  QString m_ToolStorageFilename; ///>stores the filename of the current tool storage
  QString m_AutoSaveFilename;    ///>a filename for auto saving tools if no m_ToolStorageFilename was given by the user
  QString QtempString;
  bool m_SimpleModeEnabled = false;
  bool m_Tracking; // bool which is true if tracking is running, false if not tool visualization filter second filter of
                   // the IGT pipeline)
  bool m_Printheader; //  flag for combined Error measurment list 
  float m_TubeRadius; // 3D Tube radius for the 3d tube splatting
  int m_TubeSides;    // 3d Tube Sides
  int m_LineWidth;    // Line Width of the Spline Visualisation
  bool m_Lighting;    // boolean Lighting of the 3d ViewVisualisation
  int NumUpdates;
  int m_Splineresolution; // sets the resolution for cardinal and kochanek splines
  bool m_Initialise;
  unsigned int m_Toolcount;
  bool m_Debug; // enable/disable debug output 
  double m_CurrentRefDoubleSelection;
  mitk::PointSet::Pointer m_CombinedPointset; // combined from input of all EM Sensors
  int m_CounterofDN; // Datanode Counter for combined Measurments
  int m_StepCounter; // for UpdateCombinedMethod counts the currently selected Points
  mitk::PointSet::Pointer m_CombinedTempPointset; // 
  static const std::string VIEW_ID; // for m_controls to identify in mitk modules
  QmitkAbstractView::Pointer Qmitkabtractview; // abstract view qmitk object 
  static mitk::VirtualTrackingDevice::Pointer Tracker;
  Ui::SplineVisualisationPluginControls m_Controls; // Plug-In UI Controls
  QmitkStdMultiWidget *MultiWidget;

  vtkSmartPointer<vtkFloatArray> m_SplineWeights; 
  vtkSmartPointer<vtkPolyData> m_SplinePolyData;
  vtkSmartPointer<vtkUnsignedCharArray> m_SplineColors;
  vtkSmartPointer<vtkDataSet> m_SplineIdDataSet;

  QThread *m_WorkerThread;
  SplineDataObjectVisualizationFilter *m_SplineDataObjectVisualisationfilter; // inheriting Filter object class for the updating mechanism based on the NavigationDataToNavigationDataFilter Class
  SplineVisualisationEvaluationUtil *m_SplineVisualisationEvaluationUtil; // Utiloity class for various supporting methods in spline object instancing 

  mitk::NavigationDataObjectVisualizationFilter::Pointer m_ToolVisualizationFilter; //> holds the IGT Tracker Tool data

  mitk::NavigationToolStorage::Pointer m_ToolStorage; // Navigation tool storage
  mitk::TrackingDeviceData m_TrackingDeviceData; // 
  mitk::DataNode::Pointer m_TrackingVolumeNode; //current selected tracking volumn node 
  mitk::BaseRenderer::Pointer renderer; 
  mitk::IRenderWindowPart *iRenderWindowPart;
  QmitkNavigationDataSourceSelectionWidget *m_NaviDataSourceSelection; // object for a selectable NavigationDataSource 

  static vtkParametricSpline ParametricSpline; // class 
  static mitk::TrackingDeviceSource Source;
  static mitk::NavigationData FixedNavigationData;
  mitk::CameraVisualization::Pointer m_VirtualView;  ///< filter to update
  mitk::PointSet::Pointer m_SplineReferencePointSet; // all selected evaluated reference Points on the calculated spline
  mitk::PointSet::Pointer m_TipSplineReferencePointSet; // only evaluated tip points, seperate from normal reference points of the spline
  mitk::NavigationData::Pointer m_PointSetRecordingNavigationData;
  mitk::NavigationData::Pointer m_NavigationData; ///< navigation data
  mitk::NavigationData::Pointer m_NavigationDataLastUpdate; ///< this is the position of the tracker from the last call of update(); it is used to
                                ///< check the difference and decide if the visualization must be updated
  vtkSmartPointer<vtkParametricSpline> m_Parametricspline; // 
  mitk::DataNode::Pointer m_SplineReferencePointsDataNode;
  mitk::SplineVtkMapper3D::Pointer m_SplineVtkMapper;
  mitk::SurfaceVtkMapper3D::Pointer m_TubeSurfaceVTKmapper;
  mitk::SplineVtkMapper3D::Pointer m_TipSplineVtkMapper;
  mitk::SurfaceVtkMapper3D::Pointer m_TipTubeVtkMapper;
  QmitkMITKIGTTrackingToolboxViewWorker *m_Worker;
  mitk::StandaloneDataStorage::Pointer m_DataStorage;

  /*
   *  \brief Constructor
   */
  SplineVisualisationPlugin();
  /*
   *   \brief Deconstructor
   */
  ~SplineVisualisationPlugin();
  /**
   * \brief 
   * \return 
   */
  /**
   * \brief initialise the timer slot for combined and real time spline evaluation
   */
  void InitialiseTimerSlot();
  /**
   * \brief store the ui settings for later updates 
   */
  void StoreUISettings();
  /**
   * \brief load ui setting during start up
   */
  void LoadUISettings();
  /**
   * \brief Craeats the Controlls for the plugin
   * \param parent Plugin QWidget for UI execution 
   */
  virtual void CreateQtPartControl(QWidget *parent);
  /**
   * \brief gets the currently selected Pointset datanode form the QmitkPointsetSelectionWidget 
   * \param currentDatanode Datanode 
   * \return  
   */
  mitk::PointSet::Pointer getPointsetQmitkWidget(mitk::DataNode::Pointer currentDatanode);
  /**
   * \brief set focus on the lastly executed current tab
   */
  virtual void SetFocus() override;

  void StopTrackingMechanism();
  /**
   * \brief update mmethod for the combined spline mapper for all refernce points 
   * \param splinePointsDatanode (evaluated spline points)
   */
  void CombinedSplineMapperUpdate(mitk::DataNode::Pointer splinePointsDatanode);
  /**
   * \brief combined tube mapper update for the surface node 
   * \param datanode surface datanode 
   */
  void Combined3DTubeMapperUpdate(mitk::DataNode::Pointer datanode);
  /**
   * \brief combined spline render Update function
   * \param combinedPointset 
   */
  void CombinedSplineRenderFunction(mitk::PointSet::Pointer combinedPointset);
  /**
   * \brief On Selection Changed 
   * \param source 
   * \param nodes 
   */
  virtual void OnSelectionChanged(berry::IWorkbenchPart::Pointer source,
                                  const QList<mitk::DataNode::Pointer> &nodes) override;
  /**
   * \brief Setting  current datanode  with the QmitkSingleNodeSelectionWidget 
   * \param nodes 
   */
  void OnCurrentSelectionChanged(QmitkSingleNodeSelectionWidget::NodeList nodes);
  /**
   * \brief changes for changing the tracking device volumn
   * \param qstr 
   */
  void OnTrackingVolumeChanged(QString qstr);
  /**
   * \brief Initialization Real Time EM Sensor Tracking
   * \param renderer 
   * \param parent 
   */
  void InitalRTEMSensorTracking(mitk::BaseRenderer *renderer, QWidget *parent);
  /**
   * \brief Resetting the global standard view  and set all nodes "includeInBoundingBox" to false
   */
  void GlobalReinit();
  /**
   * \brief Set GroundTruthPoints in PointSetListWidget  
   */
  void SetGroundTruthInPointSetListWidget();
  /**
   * \brief m_Controls Getter
   * \return 
   */
  Ui::SplineVisualisationPluginControls GetMControls();
  /**
   * \brief 
   * \return 
   */
  mitk::PointSet::Pointer GetCurrentPointsetInListWidget();
  /*
   * \brief CSV Loading Functions
   */
  void StartReadingCSVFile();
  /**
   * \brief stop reading the pointset
   */
  void StopReadPointSet();

  /**
   * \brief Activating the RenderWindow Part 
   * \param renderWindowPart 
   */
  void RenderWindowPartActivated(mitk::IRenderWindowPart *renderWindowPart) override;
  /**
   * \brief Deactivates the IRenderWindow Part
   * \param renderWindowPart 
   */
  void RenderWindowPartDeactivated(mitk::IRenderWindowPart *renderWindowPart) override;
  /**
   * \brief Renders the B Spline Transformation (for 3d Tube ) 
   * \param renderer 
   * \param spline 
   */
  void CommonBsplineRenderFunction(mitk::BaseRenderer::Pointer renderer, 
                                   vtkParametricSpline* spline);
  /**
   * \brief Common Spline (for parametric, (optimised) kochanek , cardinal and scurve ) render function
   * \param renderer 
   */
  void CommonSplineRenderFunction(mitk::BaseRenderer *renderer);
  /**
   * \brief Common Tube (for 3DTube and BSplineTransformation ) render function
   * \param renderer 
   * \param spline vtkObject for surface reconstruction with BSplineTransformation
   */
  void CommonTubeRenderFunction(mitk::BaseRenderer::Pointer renderer,
                                vtkParametricSpline *spline);
  /**
   * \brief Support: set the Pointest text UI window  
   * \return 
   */
  QString SetPointSetText();
  /**
   * \brief stops the render timer 
   */
  void StopTimer();
  /**
   * \brief Spline Plugin On Freeze Unfreeze Display Function
   * \param parent 
   * \param dataStorage 
   */
  void SplinePluginOnFreezeUnfreezeDisplay(QWidget *parent, mitk::StandaloneDataStorage *dataStorage);
  /**
   * \brief Getter for Standalone class data storage
   * \return 
   */
  mitk::DataStorage::Pointer GetPluginDataStorage();
  /**
   * \brief Setter Plugin StandaloneDataStorage 
   * \param m_DataStorage 
   */
  void SetPluginDataStorage(mitk::StandaloneDataStorage::Pointer m_DataStorage);
  /**
   * \brief Setter for spline point generation distance 
   * \param currentIndex 
   */
  void SetReferenceDistanceEvaluationStep(int currentIndex);
  /**
   * \brief 
   * \return surface node
   */
  mitk::Surface::Pointer getUpdateSurfaceTubeData();

public slots:
  /**
   * \brief Update the timer slot for real time evaluation
   */
  void UpdateTimerSlot();
  /**
   * \brief setter anchor point selection
   * \param checked 
   */
  void SetAnchorPointSelection(bool checked);
  /**
   * \brief setter spline point selection
   * \param checked 
   */
  void SetReferencePointSetDistance(bool checked);
  /**
   * \brief setter EM Tracking sensor display
   * \param checked 
   */
  void SetEMTrackingSensorDisplaying(bool checked);
  /**
   * \brief Processing Recordings
   */
  void StartProcessingRecordings();
  /**
   * \brief Update timer slot for combined recordings 
   */
  void UpdateCombinedTimerSlot();

 protected:
  static long couterCoordinates;
  mitk::TrackingDeviceData *m_Trackingdevicedata;
  mitk::TimeGeometry::ConstPointer  m_TimeGeometry; // the time geometry is responsible for the layout inside the QmitkStdMultiWidget *multiwidget layout
  mitk::DataNode::Pointer m_SplineReturnDataNote; // the return datanode of the SplienVisualisationFilter class

private:
  ctkServiceReference m_DeviceTypeServiceReference;
  mitk::TrackingDeviceTypeCollection *m_DeviceTypeCollection;
  mitk::DataNode::Pointer m_ToolProjectionNode;
};

#endif //  SplineVisualisationPlugin_h
