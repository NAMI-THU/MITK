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
#ifndef SplineVisualisationTPMeasurments_h
#define SplineVisualisationTPMeasurments_h


#include <QmitkAbstractView.h>
#include "SplineOptimisationCalc.h"

// Blueberry
#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>
#include <berryISelectionListener.h>

// Qt
#include <QMessageBox>
#include <QTimer>
#include <QPushButton>
// #include<Qt3DAnimation/>
// mitk image
#include <mitkImage.h>
// PointSet
#include <mitkPointSet.h>
#include <mitkProperties.h>
#include <mitkCommon.h>
// inheriting class...
#include <mitkRenderingManager.h>
#include <mitkPointSetVtkMapper3D.h>
#include <vtkMitkRectangleProp.h>
#include <mitkVtkPropRenderer.h>
#include "mitkGL.h"
#include <mitkVector.h>
#include <vector>
#include <mitkIFileReader.h>
#include <mitkCone.h>
#include <mitkContourModel.h>
#include <mitkVirtualTrackingDevice.h>
#include <mitkNavigationData.h>
#include <mitkTrackingDeviceSource.h>
#include <mitkNavigationDataObjectVisualizationFilter.h>

// standard imports
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream> 
#include <algorithm>
#include <list>
#include <ep/include/ITK-5.2/itksys/SystemTools.hxx>
#include <fstream>
#include <new.h>
#include <ctkServiceEvent.h>

// MITK and VTK imports
#include <mitkAnnotation.h>
#include <mitkTrackingTool.h>
#include <mitkStandaloneDataStorage.h>
#include <mitkDataNode.h>
#include <mitkRenderWindow.h>
#include <mitkVtkAnnotation3D.h>
#include <mitkTextAnnotation3D.h>
#include <mitkManualPlacementAnnotationRenderer.h>
#include <mitkBoundingObjectGroup.h>
#include <QmitkAbstractView.h>
#include <mitkNavigationToolStorage.h>
#include <mitkNavigationDataObjectVisualizationFilter.h>
#include <mitkNavigationDataRecorder.h>
#include <mitkNavigationDataToIGTLMessageFilter.h>
#include <mitkIGTLServer.h>
#include <mitkIGTLMessageProvider.h>
#include <mitkVtkPropRenderer.h>
#include <mitkNeedleProjectionFilter.h>

#include <vtkTreeLayoutStrategy.h>
#include <vtkGraphLayout.h>
#include <vtkSplineFilter.h>
#include "vtkGraphAlgorithm.h" // vtkGraphAlgorithm.h" // ../ep/src/VTK/Common/ExecutionModel/vtkGraphAlgorithm.h
#include "vtkInfovisCoreModule.h"
#include <vtkGraphHierarchicalBundleEdges.h>
#include <vtkRandomGraphSource.h>
#include <vtkGraphLayoutView.h>
#include <vtkSplineWidget.h>
#include <vtkKochanekSpline.h>
#include <vtkVolume16Reader.h>
#include <vtkTestUtilities.h>
#include <vtkImagePlaneWidget.h>
#include <vtkOutlineFilter.h>
#include <vtkParametricSpline.h>
#include <vtkRenderWindowInteractor.h>
#include <ui_SplineVisualisationTPMeasurmentsControls.h>
#include <mitkIRenderWindowPartListener.h>
#include <ctkServiceReference.h>
#include <mitkPlaneGeometryData.h>
#include <vtkOpenGLPolyDataMapper.h>
#include <mitkPlanarFigure.h>
#include <vtkPolyData.h>
#include "../../../org.mitk.gui.qt.igttracking/src/internal/QmitkMITKIGTTrackingToolboxViewWorker.h"
#include <internal/mitkSplineDataObjectVisualizationFilter.h>


  /*
  \brief SplineVisualisationTPMeasurments

  \warning  This class is not yet documented. Use "git blame" and ask the author to provide basic documentation.

  \sa QmitkAbstractView
  \ingroup ${plugin_target}_internal
*/
class SplineDataObjectVisualizationFilter;

  class SplineVisualisationTPMeasurments
  : public QmitkAbstractView,
    public mitk::IRenderWindowPartListener                                             
  {
  // this is needed for all Qt objects that should have a Qt meta-object
  // (everything that derives from QObject and wants to have signal/slots)
  Q_OBJECT

public:

  // typedef SplineVisualisationTPMeasurments::Pointer splineTPMeasurements;

  static const std::string VIEW_ID;
  QTimer *timer;
  QmitkAbstractView *qmitkabtractview;
  static mitk::VirtualTrackingDevice::Pointer tracker;
  static mitk::NavigationDataObjectVisualizationFilter::Pointer visualizer;
  static vtkKochanekSpline *xspline;
  static vtkKochanekSpline *yspline;
  static vtkKochanekSpline *zspline;
  static vtkTreeLayoutStrategy treeStrategy;
  static vtkParametricSpline para;
  vtkGraphLayout *treeLayout;
  vtkVolume16Reader *v16;
  static mitk::TrackingDeviceSource source;
  static mitk::NavigationData fixedNavigationData;
  Ui::SplineVisualisationTPMeasurmentsControls m_Controls;
  SplineVisualisationTPMeasurments();
  ~SplineVisualisationTPMeasurments();

  struct point
  {
    double x;
    double y;
    double z;
  };
  // Structure to hold metadata of a TrackVis file
  struct TrackVis_header
  {
    char id_string[6];
    short int dim[3];
    float voxel_size[3];
    float origin[3];
    short int n_scalars;
    char scalar_name[10][20];
    short int n_properties;
    char property_name[10][20];
    float vox_to_ras[4][4];
    char reserved[444];
    char voxel_order[4];
    char pad2[4];
    float image_orientation_patient[6];
    char pad1[2];
    unsigned char invert_x;
    unsigned char invert_y;
    unsigned char invert_z;
    unsigned char swap_xy;
    unsigned char swap_yz;
    unsigned char swap_zx;
    int n_count;
    int version;
    int hdr_size;
  };


  void StoreUISettings();

  void LoadUISettings();

  virtual void CreateQtPartControl(QWidget *parent);

  virtual void SetFocus() override;

  /** @brief This timer updates the IGT pipline and also the logging filter if logging is activated.*/
  static const char *FIBER_ID_ARRAY;
  QTimer *m_TrackingRenderTimer;
  QTimer *m_TrackingLoggingTimer;
  QTimer *m_TimeoutTimer;
  unsigned int m_NumFibers;
  unsigned int m_MeanFiberLength;
  unsigned int m_MedianFiberLength;
  unsigned int m_LengthStDev;
  vector<float> m_FiberLengths;
  unsigned int m_MinFiberLength;
  unsigned int m_MaxFiberLength;
  bool threeDVis;
  vtkSmartPointer<vtkFloatArray> m_FiberWeights;
  vtkSmartPointer<vtkPolyData> m_FiberPolyData;
  vtkSmartPointer<vtkUnsignedCharArray> m_FiberColors;
  vtkSmartPointer<vtkDataSet> m_FiberIdDataSet;
  TrackVis_header m_TrackVisHeader;
  itk::TimeStamp m_UpdateTime2D;
  itk::TimeStamp m_UpdateTime3D;
  mitk::BaseGeometry *geometry;
  QThread *m_WorkerThread;
  QmitkMITKIGTTrackingToolboxViewWorker *m_Worker;
  SplineDataObjectVisualizationFilter *m_splineDataObjectVisualisationfilter;
  mitk::NavigationDataObjectVisualizationFilter::Pointer m_ToolVisualizationFilter; ///> holds the tool visualization filter (second filter of the IGT pipeline)
  bool m_tracking;         ///> bool which is true if tracking is running, false if not
  bool m_connected;        ///> bool that is true when a tracking device is connected
  bool m_logging;          ///> bool which is true if logging is running, false if not
  bool m_ShowHideToolAxis; ///> bool, which will be true, if the tool axis is visible during tracking
  int m_loggedFrames;      ///> stores the current number of logged frames if logging is on
  QString m_ToolStorageFilename; ///>stores the filename of the current tool storage
  QString m_AutoSaveFilename;    ///>a filename for auto saving tools if no m_ToolStorageFilename was given by the user
  bool m_SimpleModeEnabled = false;
  float m_TubeRadius;
  int m_TubeSides;
  int m_LineWidth;
  float m_RibbonWidth;
  bool m_Lighting;
  vtkSmartPointer<vtkLookupTable> m_lut;

   class LocalStorage3D : public mitk::Mapper::BaseLocalStorage
  {
  public:
    vtkSmartPointer<vtkActor> m_FiberActor;
    vtkSmartPointer<vtkOpenGLPolyDataMapper> m_FiberMapper;
    vtkSmartPointer<vtkPropAssembly> m_FiberAssembly;
    itk::TimeStamp m_LastUpdateTime;
    LocalStorage3D();
    ~LocalStorage3D() override {}
  };

   class FiberBundle : public mitk::BaseData
  {
  public:
    TrackVis_header m_TrackVisHeader;
    vtkSmartPointer<vtkFloatArray> m_FiberWeights;
    vtkSmartPointer<vtkPolyData> m_FiberPolyData;
    FiberBundle(vtkPolyData *fiberPolyData);
  };

  mitk::LocalStorageHandler<LocalStorage3D> m_LocalStorageHandler;
  LocalStorage3D *localStorage;
  mitk::NavigationToolStorage::Pointer m_toolStorage; 
  mitk::TrackingDeviceData m_TrackingDeviceData; 
  mitk::DataNode::Pointer m_TrackingVolumeNode; 
  /// \brief called by QmitkFunctionality when DataManager's selection has changed
  virtual void OnSelectionChanged(berry::IWorkbenchPart::Pointer source, const QList<mitk::DataNode::Pointer> &nodes) override;
  void IGTMeasurementRLEMSensor3DSpline(mitk::BaseRenderer *renderer);
  void OnTrackingVolumeChanged(QString qstr);
  void GlobalReinit();
  void TimerEvent(QTimerEvent *event);
  void UpdateTimer(bool stop, QWidget *parent);
  /// \brief Called when the user clicks the GUI button
  void StartReadingPoints();
  void StopReadPointSet();
  // for spline Generation
  void RenderWindowPartDeactivated(mitk::IRenderWindowPart *renderWindowPart);
  vector<mitk::Vector3D> InputMitkVector3DConversion();
  std::vector<point> GetCSVImportData();
  void MainSplineGenerationFunction(mitk::BaseRenderer *renderer, bool csvImport, bool threeDVis);
  void ResampleSpline(float pointDistance, double tension, double continuity, double bias);
  void ResampleSpline(float pointDistance);
  void SetFiberPolyData(vtkSmartPointer<vtkPolyData> fiberPD, bool updateGeometry);
  void SetFiberWeights(float newWeight);
  void SetFiberWeights(vtkSmartPointer<vtkFloatArray> weights);
  void GenerateFiberIds();
  void ColorFibersByOrientation();
  void UpdateFiberGeometry();
  TrackVis_header GetTrackVisHeader();
  void SetTrackVisHeader(mitk::BaseGeometry *geometry);
  void RenderWindowPartActivated(mitk::IRenderWindowPart *renderWindowPart);
  void StartReadPointSet();
  void LogsToOutputWindow();
  std::string GetStringElementFromList(list<std::string> ElementList, int idx);
  QString SearchForLogfiles();
  virtual void ReadCSVPoints();

  mitk::BaseGeometry::Pointer GetGeometry(mitk::Geometry3D::Pointer t);
  mitk::BaseGeometry::Pointer GetGeometry();
  void SetGeometry(mitk::BaseGeometry *geometry);

  public slots:
  virtual void updateStandart3dView() { 
      // this->UpdateWindow();
  }

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
  static long couter_coordinates;


 private:
  ctkServiceReference m_DeviceTypeServiceReference;
  mitk::TrackingDeviceTypeCollection *m_DeviceTypeCollection;
  mitk::DataNode::Pointer m_ToolProjectionNode;
  };

#endif //  SplineVisualisationTPMeasurments_h
