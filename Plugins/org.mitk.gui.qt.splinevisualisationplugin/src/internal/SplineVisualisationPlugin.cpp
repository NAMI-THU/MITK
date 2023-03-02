
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
#include "SplineVisualisationPlugin.h"

#include "ui_SplineVisualisationPluginControls.h"
#include <filesystem>
#include <tchar.h>
#include <vector>
#include <internal/mitkSplineDataObjectVisualizationFilter.h>

// Qt
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
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
#include <mitkNavigationData.h>
#include <mitkNavigationDataObjectVisualizationFilter.h>
#include <mitkRenderingManager.h>

// standard imports
#include <chrono>
#include <ctkServiceEvent.h>
#include <dirent_win32.h>
#include <iostream>
#include <new.h>
#include <string>
#include <vector>

// MITK imports
#include "QmitkStdMultiWidget.h"
#include "mitkLookupTableProperty.h"
#include "mitkTubeGraph.h"
#include <QInputDialog>
#include <QmitkAbstractView.h>
#include <QmitkSliceNavigationListener.h>
#include <mitkBaseRenderer.h>
#include <mitkClippingProperty.h>
#include <mitkDataNode.h>
#include <mitkDispatcher.h>
#include <mitkGradientBackground.h>
#include <mitkIGTException.h>
#include <mitkIOUtil.h>
#include <mitkIRenderWindowPart.h>
#include <mitkLog.h>
#include <mitkManualPlacementAnnotationRenderer.h>
#include <mitkNavigationToolStorageDeserializer.h>
#include <mitkNodePredicateAnd.h>
#include <mitkNodePredicateDataType.h>
#include <mitkNodePredicateNot.h>
#include <mitkNodePredicateOr.h>
#include <mitkNodePredicateProperty.h>
#include <mitkRenderWindowBase.h>
#include <mitkStandaloneDataStorage.h>
#include <mitkSurfaceVtkMapper3D.h>
#include <mitkTrackingVolumeGenerator.h>
#include <mitkTubeGraphObjectFactory.h>
#include <mitkTubeGraphVtkMapper3D.h>
#include <mitkVtkMapper.h>
#include <mitkVtkPropRenderer.h>
#include <..\..\..\..\mitk\Plugins\org.mitk.gui.qt.igttracking\src\internal\QmitkMITKIGTTrackingToolboxView.h>
#include <internal\QmitkMITKIGTTrackingToolboxViewWorker.h>

//  VTK imports
#include "vtkPoints.h"
#include "vtkRenderWindow.h"
#include <vtkBSplineTransform.h>
#include <vtkCamera.h>
#include <vtkCardinalSpline.h>
#include <vtkCellArray.h>
#include <vtkCleanPolyData.h>
#include <vtkColor.h>
#include <vtkContourFilter.h>
#include <vtkDataReader.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkGaussianSplatter.h>
#include <vtkGenericDataSet.h>
#include <vtkGlyph3DMapper.h>
#include <vtkGraphAlgorithm.h>
#include <vtkImageBSplineCoefficients.h>
#include <vtkKochanekSpline.h>
#include <vtkLookupTable.h>
#include <vtkMaskPoints.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkOpenGLPolyDataMapper.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricSpline.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSCurveSpline.h>
#include <vtkScalarBarActor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkSplineRepresentation.h>
#include <vtkSplineWidget2.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkTexture.h>
#include <vtkThinPlateSplineTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransformToGrid.h>
#include <vtkTubeFilter.h>
#include <vtkTupleInterpolator.h>


#define USE_SHADER_PROPERTIES 1
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

const std::string SplineVisualisationPlugin::VIEW_ID = "org.mitk.views.splinevisualisationplugin";


SplineVisualisationPlugin::SplineVisualisationPlugin()
  : QmitkAbstractView(),
    m_Controls(),
    m_DeviceTypeCollection(nullptr),
    m_ToolProjectionNode(nullptr),
    m_Trackingdevicedata(),
    m_Worker(),
    MultiWidget()
{
  m_WorkerThread = new QThread();
  m_TrackingLoggingTimer = new QTimer();
  m_TimeoutTimer = new QTimer();
  m_reConnect = true;
  NumUpdates = 0;
  m_Initialise = true;
  m_reExecuteSpline = false;
  m_reExecuteTube = false;
  QtempString = "";
  m_Debug = false;
  m_SplineReferencePointSet = mitk::PointSet::New();
  m_DataStorage = mitk::StandaloneDataStorage::New();
  m_Splineresolution = 1000;
  this->m_PointSetTimer = new QTimer(this);
  this->m_TrackingRenderTimer = new QTimer(this);
}

SplineVisualisationPlugin::~SplineVisualisationPlugin()
{
  m_TrackingLoggingTimer->stop();
  m_TrackingRenderTimer->stop(); // Error
  m_TimeoutTimer->stop();
  delete m_TrackingRenderTimer;
  delete m_TimeoutTimer;
  delete m_DeviceTypeCollection;
  // wait for thread to finish
  m_WorkerThread->terminate();
  m_WorkerThread->wait();
  // clean up worker thread
  if (m_WorkerThread){
    delete m_WorkerThread;
  }
  if (m_Worker){
    delete m_Worker;
  }
}

mitk::DataStorage::Pointer SplineVisualisationPlugin::GetPluginDataStorage(){
  return this->GetDataStorage();
}

void SplineVisualisationPlugin::SetPluginDataStorage(mitk::StandaloneDataStorage::Pointer m_DataStorage) {
  this->GetDataStorage() = m_DataStorage;
}


void SplineVisualisationPlugin::SetFocus()
{
  m_Controls.IGTMeasurementRT->setFocus();
}

/*
 *
 * @brief StoreUISettings in MITK (mostly taken from QmitkMITKIGTTrackingToolboxView.cpp)
 *
 *
 * * @return  void
 */
//! [StoreUISettings]
void SplineVisualisationPlugin::StoreUISettings()
{
  // persistence service does not directly work in plugins for now -> using QSettings
  QSettings settings;
  settings.beginGroup(QString::fromStdString(SplineVisualisationPlugin::VIEW_ID));
  MITK_DEBUG << "Store UI settings";
  // set the values of some widgets and attrbutes to the QSettings
  settings.setValue("ShowTrackingVolume", QVariant(m_Controls.m_SpShowTrackingVolume->isChecked()));
  settings.setValue("ShowSplineConfigurationSelectionBox",
                    QVariant(m_Controls.m_TrackingPointerSelectionBox->currentIndex()));
  settings.setValue("toolStorageFilename", QVariant(m_ToolStorageFilename));
  settings.setValue("VolumeSelectionBox", QVariant(m_Controls.m_SpVolumeSelectionBox->currentIndex()));
  settings.endGroup();
}
//! [StoreUISettings]
void SplineVisualisationPlugin::LoadUISettings()
{
  // persistence service does not directly work in plugins for now
  // -> using QSettings
  QSettings settings;
  settings.beginGroup(QString::fromStdString(SplineVisualisationPlugin::VIEW_ID));
  // set some widgets and attributes by the values from the QSettings
  m_Controls.m_SpShowTrackingVolume->setChecked(settings.value("ShowTrackingVolume", true).toBool());
  m_Controls.m_SpVolumeSelectionBox->setCurrentIndex(settings.value("VolumeSelectionBox", 0).toInt());
  m_ToolStorageFilename = settings.value("toolStorageFilename", QVariant("")).toString();
  settings.endGroup();
}

void SplineVisualisationPlugin::CreateQtPartControl(QWidget *parent)
{

  m_Controls.setupUi(parent);
  m_SplineVisualisationEvaluationUtil = new SplineVisualisationEvaluationUtil();

  // initialize time
  // Update Timer @ timeout event
  QTimerEvent *timeEvent = new QTimerEvent(20);
  this->m_TrackingRenderTimer->setInterval(10 / m_Fps);
  QWidget *viewParent = new QWidget();
  // Create toplevel widget with vertical layout
  QVBoxLayout *vlayout = new QVBoxLayout(viewParent);
  vlayout->setMargin(0);
  vlayout->setSpacing(2);
  // Create viewParent widget with horizontal layout
  vlayout->addWidget(viewParent);
  QHBoxLayout *hlayout = new QHBoxLayout(viewParent);
  hlayout->setMargin(0);

  MultiWidget = new QmitkStdMultiWidget(viewParent);
  MultiWidget->setEnabled(true);
  vlayout->addWidget(MultiWidget, 1, 0);
  hlayout->addWidget(MultiWidget);
  mitk::VtkPropRenderer *vtkproprenderer = mitk::VtkPropRenderer::Pointer();
  QmitkRenderWindow qmitkrenderWindow = QmitkRenderWindow(parent, "3D renderwindow", vtkproprenderer);
  // Tell the renderwindow which (part of) the datastorage to render
  qmitkrenderWindow.GetRenderer()->SetDataStorage(m_DataStorage);
  // Use it as a 3D view
  qmitkrenderWindow.GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);
  // Reposition the camera to include all visible actors
  qmitkrenderWindow.GetRenderer()->GetVtkRenderer()->ResetCamera();
  // Tell the multiWidget which DataStorage to render
  this->renderer = qmitkrenderWindow.GetRenderer();
  MultiWidget->SetDataStorage(m_DataStorage);
  // Initialize views as axial, sagittal, coronar (from top-left to bottom)
  m_TimeGeometry = m_DataStorage->ComputeBoundingGeometry3D(m_DataStorage->GetAll());
  mitk::RenderingManager::GetInstance()->InitializeViews(m_TimeGeometry);
  // Add the displayed views to the DataStorage to see their positions in 2D and 3D
  MultiWidget->AddPlanesToDataStorage();
  MultiWidget->SetWidgetPlanesVisibility(true);
  // set optional parameters for Anchorpoint selection and Reference Point distance
  m_Controls.checkBoxEnableAnchorselection->setEnabled(true);
  m_Controls.checkBoxEnableAnchorselection->setCheckable(true);
  m_Controls.checkBoxEnableAnchorselection->setChecked(false);
  m_Controls.PointsetReaderLabel_5->setEnabled(false);
  m_Controls.totalAnchorpointsSpinBox->setEnabled(false);
  // AnchorPoint Point Selection static
  connect(m_Controls.checkBoxEnableAnchorselection, SIGNAL(clicked(bool)), this, SLOT(SetAnchorPointSelection(bool)));

  m_Controls.checkBoxEnableReferencePointDistance->setEnabled(true);
  m_Controls.checkBoxEnableReferencePointDistance->setCheckable(true);
  m_Controls.checkBoxEnableReferencePointDistance->setChecked(false);
  m_Controls.ReferencePointDistancestatic->setEnabled(false);
  m_Controls.m_PSReferenceSplineComboBox->setEnabled(false);
  m_Controls.NumberOfReferencePointsLabel->setEnabled(false);
  // Reference Point Distance static
  connect(m_Controls.checkBoxEnableReferencePointDistance,
          SIGNAL(clicked(bool)),
          this,
          SLOT(SetReferencePointSetDistance(bool)));

  m_Controls.checkBoxEnableTipEMTracking->setEnabled(true);
  m_Controls.checkBoxEnableTipEMTracking->setCheckable(true);
  m_Controls.checkBoxEnableTipEMTracking->setChecked(false);
  m_Controls.EmtrackingSensorLayout->setEnabled(false);
  m_Controls.NumEMTrackSensorCombobox->setEnabled(false);
  // enable seperate tip visualisation 
  connect(
    m_Controls.checkBoxEnableTipEMTracking, SIGNAL(clicked(bool)), this, SLOT(SetEMTrackingSensorDisplaying(bool)));

  // Combined Recordings Evaluation
  m_Controls.ProcessLabel->setVisible(false);

 connect(m_Controls.startProcessing, SIGNAL(clicked(bool)), this, SLOT(StartProcessingRecordings()));


  // try to deserialize the tool storage from the given tool storage file name
  if (!m_ToolStorageFilename.isEmpty())
  {
    // try-catch block for exceptions
    try
    {
      mitk::NavigationToolStorageDeserializer::Pointer m_NTSDeserializer =
        mitk::NavigationToolStorageDeserializer::New(this->GetDataStorage());
      m_ToolStorage->UnRegisterMicroservice();
      m_ToolStorage = m_NTSDeserializer->Deserialize(m_ToolStorageFilename.toStdString());
      m_ToolStorage->RegisterAsMicroservice();
    }
    catch (const mitk::IGTException &e)
    {
      MITK_WARN("QmitkMITKIGTTrackingToolBoxView")
        << "Error during restoring tools. Problems with file (" << m_ToolStorageFilename.toStdString()
        << "), please check the file?";
      // if there where errors reset  the tool storage to avoid problems later on
     //  qmitkMITKIGTTrackingToolboxView->OnResetTools(); 

    }
  }
  /* Create the ComboBox elements list (here we use QString) */
  QList<QString> stringsMethodList;
  QList<QString> stringsEMSensorList;
  QList<QString> stringsGlobalROIList;
  QList<QString> stringsReferencePointSteps;
  stringsMethodList.append("Parametric Spline");       // 0
  stringsMethodList.append("3D Tube");                 // 1
  stringsMethodList.append("S Curve Spline");          // 2
  stringsMethodList.append("Kochanek Spline");         // 3
  stringsMethodList.append("Cardinal Spline");         // 4
  stringsMethodList.append("B Spline Transformation"); // 5
  stringsMethodList.append("Kochanek Spline (optimised)"); // 6
  // stringsList.append("3d Tube + Third Eye Extention"); // 7

  stringsGlobalROIList.append("duodenum coordinates");
  stringsGlobalROIList.append("duodenum coordinates (#2)");
  stringsGlobalROIList.append("duodenum coordinates (#3)");
  stringsGlobalROIList.append("esophagus and stomach coordinates");
  stringsGlobalROIList.append("esophagus and stomach coordinates (#2)");
  stringsGlobalROIList.append("esophagus and stomach coordinates (#3)");
  stringsEMSensorList.append("3");
  stringsEMSensorList.append("4");
  stringsEMSensorList.append("5");
  stringsEMSensorList.append("6");
  stringsEMSensorList.append("7");
  stringsEMSensorList.append("8");
  stringsEMSensorList.append("9");
  stringsEMSensorList.append("10");
  stringsEMSensorList.append("11");
  stringsEMSensorList.append("12");
  stringsReferencePointSteps.append("0.1"); //10
  stringsReferencePointSteps.append("0.05"); //20
  stringsReferencePointSteps.append("0.025"); //40
  stringsReferencePointSteps.append("0.005"); // 200
  stringsReferencePointSteps.append("0.0025"); // 400

  /* Populate the comboBoxes */
  m_Controls.m_TrackingPointerSelectionBox->addItems(stringsMethodList);
  m_Controls.m_TrackingPointerSelectionBox->setCurrentIndex(0);
  m_Controls.m_CombinedTrackingPointerSelectionBox->addItems(stringsMethodList);
  m_Controls.m_CombinedTrackingPointerSelectionBox->setCurrentIndex(0);
  m_Controls.VisMethodcomboBox->addItems(stringsMethodList);
  m_Controls.VisMethodcomboBox->setCurrentIndex(0);
  m_Controls.NumEMTrackSensorCombobox->addItems(stringsEMSensorList);
  m_Controls.NumEMTrackSensorCombobox->setCurrentIndex(0);
  m_Controls.m_globalROISelectionBox->addItems(stringsGlobalROIList);
  m_Controls.m_globalROISelectionBox->setCurrentIndex(0);
  m_Controls.m_PSReferenceSplineComboBox->addItems(stringsReferencePointSteps);
  m_Controls.m_PSReferenceSplineComboBox->setCurrentIndex(2);
  // RT Combobox
  m_Controls.m_PSRTReferenceSplineComboBox->addItems(stringsReferencePointSteps);
  m_Controls.m_PSRTReferenceSplineComboBox->setCurrentIndex(2);
  // Comb Combobox
  m_Controls.m_PSRTReferenceSplineComboBox_Combined->addItems(stringsReferencePointSteps);
  m_Controls.m_PSRTReferenceSplineComboBox_Combined->setCurrentIndex(2);

  us::ModuleContext *context = us::GetModuleContext();
  m_Controls.selectedPointSetWidget->SetDataStorage(this->GetDataStorage());
  m_Controls.selectedPointSetWidget->SetNodePredicate(mitk::NodePredicateAnd::New(
    mitk::TNodePredicateDataType<mitk::PointSet>::New(),
    mitk::NodePredicateNot::New(mitk::NodePredicateOr::New(mitk::NodePredicateProperty::New("helper object"),
                                                           mitk::NodePredicateProperty::New("hidden object")))));
  m_Controls.selectedPointSetWidget->SetSelectionIsOptional(true);
  m_Controls.selectedPointSetWidget->SetAutoSelectNewNodes(true);
  m_Controls.selectedPointSetWidget->SetEmptyInfo(QString("Please select a point set"));
  m_Controls.selectedPointSetWidget->SetPopUpTitel(QString("Select point set"));


   // show contour lines for the spline generation
  connect(m_Controls.m_DebuggingCheckBox,
          &QPushButton::clicked,
          this,
          [this]()
          {
            if (m_Controls.m_DebuggingCheckBox->isEnabled() && 
                m_Controls.m_DebuggingCheckBox->isChecked())
            {
              m_Debug = true;
              m_SplineVisualisationEvaluationUtil->SetDebug(true);

            }
            else
            {
              m_Debug = false;
              m_SplineVisualisationEvaluationUtil->SetDebug(false);
            }
      });
 

  // show contour lines for the spline generation
  connect(m_Controls.ToogleSplinesButton,
          &QPushButton::clicked,
          this,
          [this]()
          {
            if (m_Controls.ToogleSplinesButton->text() == "Deactivate Spline")
            {
              m_Controls.m_TrackingControlLabel->setText("Status: connected");

              m_Controls.ToogleSplinesButton->setText("Activate Spline");
              SLOT(&SplineVisualisationPlugin::RenderWindowPartDeactivated("THREE_D"));
            }
            else
            {
              m_Controls.m_TrackingControlLabel->setText("Status: disconnected");

              m_Controls.ToogleSplinesButton->setText("Deactivate Spline");
              SLOT(&SplineVisualisationPlugin::RenderWindowPartActivated("THREE_D"));
            }
          });

  // Reference Distance Evaluation Step Combobox 
  connect(m_Controls.EvaluatePointsSplinesGen,
          &QPushButton::clicked,
          this,
          [this]() { 
            // check if Reference Points Combobox has changed and change global variable
            if (m_Controls.checkBoxEnableReferencePointDistance->isChecked())
          {
            SetReferenceDistanceEvaluationStep(m_Controls.m_PSReferenceSplineComboBox->currentIndex());
          }
          else
          {
            SetReferenceDistanceEvaluationStep(2);
          }
          CommonSplineRenderFunction(this->renderer);
      });

  connect(m_Controls.m_SpFreezeUnfreezeTrackingButton,
          &QPushButton::clicked,
          this,
          [this, parent]() { SplinePluginOnFreezeUnfreezeDisplay(parent, m_DataStorage); });

  connect(m_Controls.SavePointsetasCSV,
          &QPushButton::clicked,
          this,
          [this]()
          {
            m_SplineVisualisationEvaluationUtil->WriteToCSVFile(m_Controls.pointSplineSetListWidget->GetPointSet());
          });

  connect(m_Controls.ToogleSplinesButtonIGTRTT,
          &QPushButton::clicked,
          this,
          [this]()
          {
            InitalRTEMSensorTracking(this->renderer, m_Controls.m_SplineTrackingDeviceSelectionWidget);
            if (m_Initialise)
            {
              InitialiseTimerSlot();
              m_Initialise = false;
            }
            if (m_reConnect)
            {
              this->connect(m_TrackingRenderTimer, SIGNAL(timeout()), this, SLOT(UpdateTimerSlot()));
              this->m_TrackingRenderTimer->start(1000);
              m_reConnect = false;
            }
          });

    connect(m_Controls.m_CombinedEvaluateButton,
          &QPushButton::clicked,
          this,
          [this]()
          {
            if (m_Controls.m_CombinedEvaluateButton->text() == "Evaluate Spline")
            {
              m_Controls.m_CombinedEvaluateButton->setText("Stop Evaluation");
              m_reConnect = true;
            }
            else
            {
              m_Controls.m_CombinedEvaluateButton->setText("Evaluate Spline");
              StopTimer();
              m_reConnect = false;
              return;
            }
            if (m_Controls.checkBoxEnableReferencePointDistance->isChecked())
            {
              SetReferenceDistanceEvaluationStep(m_Controls.m_PSReferenceSplineComboBox->currentIndex());
            }
            else
            {
              SetReferenceDistanceEvaluationStep(2);
            }
            if (m_Initialise)
            {
              InitialiseTimerSlot();
              m_Initialise = false;
            }
            if (m_reConnect)
            {
              this->connect(m_TrackingRenderTimer, SIGNAL(timeout()), this, SLOT(UpdateCombinedTimerSlot()));
              this->m_TrackingRenderTimer->start(1000);
              m_reConnect = false;
            }
          });

  auto renderWindowPart = this->GetRenderWindowPart();

  if (nullptr != renderWindowPart)
    this->RenderWindowPartActivated(renderWindowPart);

  connect(m_Controls.selectedPointSetWidget,
          &QmitkSingleNodeSelectionWidget::CurrentSelectionChanged,
          this,
          &SplineVisualisationPlugin::OnCurrentSelectionChanged);

  m_Controls.EmtrackerPointSetWidget_1->SetDataStorage(this->GetDataStorage());
  m_Controls.EmtrackerPointSetWidget_1->SetNodePredicate(mitk::NodePredicateAnd::New(
    mitk::TNodePredicateDataType<mitk::PointSet>::New(),
    mitk::NodePredicateNot::New(mitk::NodePredicateOr::New(mitk::NodePredicateProperty::New("helper object"),
                                                           mitk::NodePredicateProperty::New("hidden object")))));
  m_Controls.EmtrackerPointSetWidget_1->SetSelectionIsOptional(true);
  m_Controls.EmtrackerPointSetWidget_1->SetAutoSelectNewNodes(true);
  m_Controls.EmtrackerPointSetWidget_1->SetEmptyInfo(QString("Please select a point set"));
  m_Controls.EmtrackerPointSetWidget_1->SetPopUpTitel(QString("Select point set"));

  connect(m_Controls.EmtrackerPointSetWidget_1,
          &QmitkSingleNodeSelectionWidget::CurrentSelectionChanged,
          this,
          &SplineVisualisationPlugin::OnCurrentSelectionChanged);

  m_Controls.EmtrackerPointSetWidget_2->SetDataStorage(this->GetDataStorage());
  m_Controls.EmtrackerPointSetWidget_2->SetNodePredicate(mitk::NodePredicateAnd::New(
    mitk::TNodePredicateDataType<mitk::PointSet>::New(),
    mitk::NodePredicateNot::New(mitk::NodePredicateOr::New(mitk::NodePredicateProperty::New("helper object"),
                                                           mitk::NodePredicateProperty::New("hidden object")))));
  m_Controls.EmtrackerPointSetWidget_2->SetSelectionIsOptional(true);
  m_Controls.EmtrackerPointSetWidget_2->SetAutoSelectNewNodes(true);
  m_Controls.EmtrackerPointSetWidget_2->SetEmptyInfo(QString("Please select a point set"));
  m_Controls.EmtrackerPointSetWidget_2->SetPopUpTitel(QString("Select point set"));
  connect(m_Controls.EmtrackerPointSetWidget_2,
          &QmitkSingleNodeSelectionWidget::CurrentSelectionChanged,
          this,
          &SplineVisualisationPlugin::OnCurrentSelectionChanged);

  m_Controls.EmtrackerPointSetWidget_3->SetDataStorage(this->GetDataStorage());
  m_Controls.EmtrackerPointSetWidget_3->SetNodePredicate(mitk::NodePredicateAnd::New(
    mitk::TNodePredicateDataType<mitk::PointSet>::New(),
    mitk::NodePredicateNot::New(mitk::NodePredicateOr::New(mitk::NodePredicateProperty::New("helper object"),
                                                           mitk::NodePredicateProperty::New("hidden object")))));
  m_Controls.EmtrackerPointSetWidget_3->SetSelectionIsOptional(true);
  m_Controls.EmtrackerPointSetWidget_3->SetAutoSelectNewNodes(true);
  m_Controls.EmtrackerPointSetWidget_3->SetEmptyInfo(QString("Please select a point set"));
  m_Controls.EmtrackerPointSetWidget_3->SetPopUpTitel(QString("Select point set"));

  connect(m_Controls.EmtrackerPointSetWidget_3,
          &QmitkSingleNodeSelectionWidget::CurrentSelectionChanged,
          this,
          &SplineVisualisationPlugin::OnCurrentSelectionChanged);

  m_Controls.EmtrackerPointSetWidget_4->SetDataStorage(this->GetDataStorage());
  m_Controls.EmtrackerPointSetWidget_4->SetNodePredicate(mitk::NodePredicateAnd::New(
    mitk::TNodePredicateDataType<mitk::PointSet>::New(),
    mitk::NodePredicateNot::New(mitk::NodePredicateOr::New(mitk::NodePredicateProperty::New("helper object"),
                                                           mitk::NodePredicateProperty::New("hidden object")))));
  m_Controls.EmtrackerPointSetWidget_4->SetSelectionIsOptional(true);
  m_Controls.EmtrackerPointSetWidget_4->SetAutoSelectNewNodes(false);
  m_Controls.EmtrackerPointSetWidget_4->SetEmptyInfo(QString("Please select a point set"));
  m_Controls.EmtrackerPointSetWidget_4->SetPopUpTitel(QString("Select point set"));
  connect(m_Controls.EmtrackerPointSetWidget_4,
          &QmitkSingleNodeSelectionWidget::CurrentSelectionChanged,
          this,
          &SplineVisualisationPlugin::OnCurrentSelectionChanged);
 
  
  m_Controls.EmtrackerPointSetWidget_5->SetDataStorage(this->GetDataStorage());
  m_Controls.EmtrackerPointSetWidget_5->SetNodePredicate(mitk::NodePredicateAnd::New(
    mitk::TNodePredicateDataType<mitk::PointSet>::New(),
    mitk::NodePredicateNot::New(mitk::NodePredicateOr::New(mitk::NodePredicateProperty::New("helper object"),
                                                           mitk::NodePredicateProperty::New("hidden object")))));
  m_Controls.EmtrackerPointSetWidget_5->SetSelectionIsOptional(true);
  m_Controls.EmtrackerPointSetWidget_5->SetAutoSelectNewNodes(false);
  m_Controls.EmtrackerPointSetWidget_5->SetEmptyInfo(QString("Please select a point set"));
  m_Controls.EmtrackerPointSetWidget_5->SetPopUpTitel(QString("Select point set"));
  connect(m_Controls.EmtrackerPointSetWidget_5,
          &QmitkSingleNodeSelectionWidget::CurrentSelectionChanged,
          this,
          &SplineVisualisationPlugin::OnCurrentSelectionChanged);

  m_Controls.EmtrackerPointSetWidget_6->SetDataStorage(this->GetDataStorage());
  m_Controls.EmtrackerPointSetWidget_6->SetNodePredicate(mitk::NodePredicateAnd::New(
    mitk::TNodePredicateDataType<mitk::PointSet>::New(),
    mitk::NodePredicateNot::New(mitk::NodePredicateOr::New(mitk::NodePredicateProperty::New("helper object"),
                                                           mitk::NodePredicateProperty::New("hidden object")))));
  m_Controls.EmtrackerPointSetWidget_6->SetSelectionIsOptional(true);
  m_Controls.EmtrackerPointSetWidget_6->SetAutoSelectNewNodes(false);
  m_Controls.EmtrackerPointSetWidget_6->SetEmptyInfo(QString("Please select a point set"));
  m_Controls.EmtrackerPointSetWidget_6->SetPopUpTitel(QString("Select point set"));
  connect(m_Controls.EmtrackerPointSetWidget_6,
          &QmitkSingleNodeSelectionWidget::CurrentSelectionChanged,
          this,
          &SplineVisualisationPlugin::OnCurrentSelectionChanged);
  
  m_Controls.EmtrackerPointSetWidget_7->SetDataStorage(this->GetDataStorage());
  m_Controls.EmtrackerPointSetWidget_7->SetNodePredicate(mitk::NodePredicateAnd::New(
    mitk::TNodePredicateDataType<mitk::PointSet>::New(),
    mitk::NodePredicateNot::New(mitk::NodePredicateOr::New(mitk::NodePredicateProperty::New("helper object"),
                                                           mitk::NodePredicateProperty::New("hidden object")))));
  m_Controls.EmtrackerPointSetWidget_7->SetSelectionIsOptional(true);
  m_Controls.EmtrackerPointSetWidget_7->SetAutoSelectNewNodes(false);
  m_Controls.EmtrackerPointSetWidget_7->SetEmptyInfo(QString("Please select a point set"));
  m_Controls.EmtrackerPointSetWidget_7->SetPopUpTitel(QString("Select point set"));
  connect(m_Controls.EmtrackerPointSetWidget_7,
          &QmitkSingleNodeSelectionWidget::CurrentSelectionChanged,
          this,
          &SplineVisualisationPlugin::OnCurrentSelectionChanged);

  m_Controls.EmtrackerPointSetWidget_8->SetDataStorage(this->GetDataStorage());
  m_Controls.EmtrackerPointSetWidget_8->SetNodePredicate(mitk::NodePredicateAnd::New(
    mitk::TNodePredicateDataType<mitk::PointSet>::New(),
    mitk::NodePredicateNot::New(mitk::NodePredicateOr::New(mitk::NodePredicateProperty::New("helper object"),
                                                           mitk::NodePredicateProperty::New("hidden object")))));
  m_Controls.EmtrackerPointSetWidget_8->SetSelectionIsOptional(true);
  m_Controls.EmtrackerPointSetWidget_8->SetAutoSelectNewNodes(false);
  m_Controls.EmtrackerPointSetWidget_8->SetEmptyInfo(QString("Please select a point set"));
  m_Controls.EmtrackerPointSetWidget_8->SetPopUpTitel(QString("Select point set"));

  connect(m_Controls.EmtrackerPointSetWidget_8,
          &QmitkSingleNodeSelectionWidget::CurrentSelectionChanged,
          this,
          &SplineVisualisationPlugin::OnCurrentSelectionChanged);

  this->OnCurrentSelectionChanged(m_Controls.selectedPointSetWidget->GetSelectedNodes());
}

mitk::PointSet::Pointer SplineVisualisationPlugin::getPointsetQmitkWidget(mitk::DataNode::Pointer currentDatanode)
{
  m_Controls.pointSplineSetListWidget->SetPointSetNode(currentDatanode);
  mitk::PointSet::Pointer currentPointSet = m_Controls.pointSplineSetListWidget->GetPointSet();
  return currentPointSet;
}

void SplineVisualisationPlugin::StartProcessingRecordings()
{
    //check if minimum of 3 EM Tracking Recordings are set
  if (m_Controls.EmtrackerPointSetWidget_1->GetSelectedNode() == nullptr ||
        m_Controls.EmtrackerPointSetWidget_2->GetSelectedNode() == nullptr ||
        m_Controls.EmtrackerPointSetWidget_3->GetSelectedNode() == nullptr)
  {
    QMessageBox msgError;
    msgError.setText("Error Message: \n A minimum of three pointset datanodes need to be selected! \n Select more Datasets "
                     "or restart application.");
    msgError.setIcon(QMessageBox::Critical);
    msgError.setWindowTitle("Error");
    msgError.exec();
    return;
  }
  mitk::DataNode::Pointer combinedDatanode = mitk::DataNode::New();
  m_CombinedPointset = mitk::PointSet::New();
  mitk::PointSet::Pointer selectedPointset = mitk::PointSet::New();
  m_CounterofDN = 3;
  mitk::PointSet::Pointer EMPointSet1 = getPointsetQmitkWidget(m_Controls.EmtrackerPointSetWidget_1->GetSelectedNode());
  int NumPS1 = EMPointSet1->GetSize();
  mitk::PointSet::Pointer EMPointSet2 = getPointsetQmitkWidget(m_Controls.EmtrackerPointSetWidget_2->GetSelectedNode());
  int NumPS2 = EMPointSet2->GetSize();
  mitk::PointSet::Pointer EMPointSet3 = getPointsetQmitkWidget(m_Controls.EmtrackerPointSetWidget_3->GetSelectedNode());
  int NumPS3 = EMPointSet3->GetSize();
  std::vector<mitk::PointSet::Pointer> allPointsets;
  allPointsets.push_back(EMPointSet1);
  allPointsets.push_back(EMPointSet2);
  allPointsets.push_back(EMPointSet3);
  int minPoints = std::min({NumPS1, NumPS2, NumPS3});
  if (m_Controls.EmtrackerPointSetWidget_4->GetSelectedNode() != nullptr)
  {
    m_CounterofDN = 4;
    mitk::PointSet::Pointer EMPointSet4 = getPointsetQmitkWidget(m_Controls.EmtrackerPointSetWidget_4->GetSelectedNode());
    allPointsets.push_back(EMPointSet4);
    int NumPS4 = EMPointSet4->GetSize();
    minPoints = std::min({NumPS1, NumPS2, NumPS3, NumPS4});
  }
  if (m_Controls.EmtrackerPointSetWidget_5->GetSelectedNode() != nullptr)
  {
    m_CounterofDN = 5;
    mitk::PointSet::Pointer EMPointSet5 = getPointsetQmitkWidget(m_Controls.EmtrackerPointSetWidget_5->GetSelectedNode());
    allPointsets.push_back(EMPointSet5);
    int NumPS5 = EMPointSet5->GetSize();
    minPoints = std::min({minPoints, NumPS5});
  }
  if (m_Controls.EmtrackerPointSetWidget_6->GetSelectedNode() != nullptr)
  {
    m_CounterofDN = 6;
    mitk::PointSet::Pointer EMPointSet6 = getPointsetQmitkWidget(m_Controls.EmtrackerPointSetWidget_6->GetSelectedNode());
    allPointsets.push_back(EMPointSet6);
    int NumPS6 = EMPointSet6->GetSize();
    minPoints = std::min({minPoints, NumPS6});
  }
  if (m_Controls.EmtrackerPointSetWidget_7->GetSelectedNode() != nullptr)
  {
    m_CounterofDN = 7;
    mitk::PointSet::Pointer EMPointSet7 = getPointsetQmitkWidget(m_Controls.EmtrackerPointSetWidget_7->GetSelectedNode());
    allPointsets.push_back(EMPointSet7);
    int NumPS7 = EMPointSet7->GetSize();
    minPoints = std::min({minPoints, NumPS7});
  }
  if (m_Controls.EmtrackerPointSetWidget_8->GetSelectedNode() != nullptr)
  {
    m_CounterofDN = 8;
    mitk::PointSet::Pointer EMPointSet8 = getPointsetQmitkWidget(m_Controls.EmtrackerPointSetWidget_8->GetSelectedNode());
    allPointsets.push_back(EMPointSet8);
    int NumPS8 = EMPointSet8->GetSize();
    minPoints = std::min({minPoints, NumPS8});
  }
  // Combine Datasets to one single datanode to analyse
  mitk::Point3D tempPoint;
  int pointCount = 0;
  do
  {
    for (auto iter = allPointsets.begin(); iter != allPointsets.end(); iter++)
    { 
      selectedPointset = iter->GetPointer();// select a Dataset
      m_CombinedPointset->InsertPoint(selectedPointset->GetPoint(pointCount));
    }
    pointCount++;
  } while (pointCount <= minPoints);

  combinedDatanode->SetData(m_CombinedPointset);
  combinedDatanode->SetName("Combined Datanode");
  this->m_DataStorage->Add(combinedDatanode);
  if (m_Debug)
  {
    MITK_INFO << "Completed Combined DataNode!" << std::endl;
  }
  m_Controls.ProcessLabel->setVisible(true);
  m_Controls.m_CombinedEvaluateButton->setEnabled(true);
  m_Controls.m_CombinedEvaluateButton->setVisible(true);

}


void SplineVisualisationPlugin::SetEMTrackingSensorDisplaying(bool checked)
{
  if (m_Controls.checkBoxEnableTipEMTracking->isChecked())
  {
    m_Controls.EmtrackingSensorLayout->setEnabled(true);
    m_Controls.EMTrackinDisplaySelectionLayout->setEnabled(true);
    m_Controls.NumEMTrackSensorCombobox->setEnabled(true);
  }
  else
  {
    m_Controls.EmtrackingSensorLayout->setEnabled(false);
    m_Controls.EMTrackinDisplaySelectionLayout->setEnabled(false);
    m_Controls.NumEMTrackSensorCombobox->setEnabled(false);
  }
}

void SplineVisualisationPlugin::SetAnchorPointSelection(bool checked)
{
  if (m_Controls.checkBoxEnableAnchorselection->isChecked())
  {
    m_Controls.AnchorpointSelectionLayout->setEnabled(true);
    m_Controls.PointsetReaderLabel_5->setEnabled(true);
    m_Controls.totalAnchorpointsSpinBox->setEnabled(true);
  }
  else
  {
    m_Controls.AnchorpointSelectionLayout->setEnabled(false);
    m_Controls.PointsetReaderLabel_5->setEnabled(false);
    m_Controls.totalAnchorpointsSpinBox->setEnabled(false);
  }
}

void SplineVisualisationPlugin::SetReferencePointSetDistance(bool checked)
{
  if (m_Controls.checkBoxEnableReferencePointDistance->isChecked())
  {
    m_Controls.ReferencePointLayout->setEnabled(true);
    m_Controls.ReferencePointDistancestatic->setEnabled(true);
    m_Controls.m_PSReferenceSplineComboBox->setEnabled(true);
    m_Controls.NumberOfReferencePointsLabel->setEnabled(true);
  }
  else
  {
    m_Controls.ReferencePointLayout->setEnabled(false);
    m_Controls.ReferencePointDistancestatic->setEnabled(false);
    m_Controls.m_PSReferenceSplineComboBox->setEnabled(false);
    m_Controls.NumberOfReferencePointsLabel->setEnabled(false);
  }

 
}

void SplineVisualisationPlugin::SetReferenceDistanceEvaluationStep(int currentIndex) {
  // double step = .025; // changes the paramteric coordinate step for the evaluation distance between points
  // .1 = 10 | .05 = 20 | .025 = 40 | .005 = 200  | .0025 = 400
  string currentRefStringSelection;
  if (m_Controls.m_PSReferenceSplineComboBox->isVisible())
  {
    currentRefStringSelection = m_Controls.m_PSReferenceSplineComboBox->currentText().toStdString();
  }
  else if (m_Controls.m_PSRTReferenceSplineComboBox->isVisible())
  {
    currentRefStringSelection = m_Controls.m_PSRTReferenceSplineComboBox->currentText().toStdString();
  }
  else if (m_Controls.m_PSRTReferenceSplineComboBox_Combined->isVisible())
  {
    currentRefStringSelection = m_Controls.m_PSRTReferenceSplineComboBox_Combined->currentText().toStdString();
  }
  m_CurrentRefDoubleSelection = std::stod(currentRefStringSelection);

  if (m_Debug)
  {
    MITK_INFO << "Index : " << currentIndex << " Current Reference Selection " << currentRefStringSelection
              << std::endl; 
  }
  if (m_CurrentRefDoubleSelection == .1)
  {
    m_Controls.NumberOfReferencePointsLabel->setText("Count of Ref Points : 10");
    m_Controls.NumberOfReferencePointsLabelRT->setText("Count of Ref Points : 10");
    m_Controls.NumberOfReferencePointsLabelRT_Comb->setText("Count of Ref Points : 10");
  }
  else if (m_CurrentRefDoubleSelection == .05)
  {
    m_Controls.NumberOfReferencePointsLabel->setText("Count of Ref Points : 20");
    m_Controls.NumberOfReferencePointsLabelRT->setText("Count of Ref Points : 20");
    m_Controls.NumberOfReferencePointsLabelRT_Comb->setText("Count of Ref Points : 20");

  }
  else if (m_CurrentRefDoubleSelection == .025)
  {
    m_Controls.NumberOfReferencePointsLabel->setText("Count of Ref Points : 40");
    m_Controls.NumberOfReferencePointsLabelRT->setText("Count of Ref Points : 40");
    m_Controls.NumberOfReferencePointsLabelRT_Comb->setText("Count of Ref Points : 40");

  }
  else if (m_CurrentRefDoubleSelection == .005)
  {
    m_Controls.NumberOfReferencePointsLabel->setText("Count of Ref Points : 200");
    m_Controls.NumberOfReferencePointsLabelRT->setText("Count of Ref Points : 200");
    m_Controls.NumberOfReferencePointsLabelRT_Comb->setText("Count of Ref Points : 200");

  }
  else if (m_CurrentRefDoubleSelection == .0025)
  {
    m_Controls.NumberOfReferencePointsLabel->setText("Count of Ref Points : 400");
    m_Controls.NumberOfReferencePointsLabelRT->setText("Count of Ref Points : 400");
    m_Controls.NumberOfReferencePointsLabelRT_Comb->setText("Count of Ref Points : 400");

  }
}

void SplineVisualisationPlugin::OnCurrentSelectionChanged(QmitkSingleNodeSelectionWidget::NodeList /*nodes*/)
{
  m_Controls.pointSplineSetListWidget->SetPointSetNode(m_Controls.selectedPointSetWidget->GetSelectedNode());
}

void SplineVisualisationPlugin::SplinePluginOnFreezeUnfreezeDisplay(QWidget *parent, mitk::StandaloneDataStorage *m_DataStorage)
{
  m_SplineDataObjectVisualisationfilter = new SplineDataObjectVisualizationFilter(this->m_ToolStorage, m_DataStorage);

  if (m_Controls.m_SpFreezeUnfreezeTrackingButton->text() == "Freeze Standard View")
  {
    StopTimer();
    // clear data storage
    // first remove old datanode from general datastorage
    if (m_Selectiontype == 0)
    {
      this->GetDataStorage()->Remove(this->m_DataStorage->GetNamedNode("Spline Datanode"));

    }
    if (m_Selectiontype == 1)
    {
      this->GetDataStorage()->Remove(this->m_DataStorage->GetNamedNode("3D Tube Datanode"));
    }
    if (m_Selectiontype == 2)
    {
      this->GetDataStorage()->Remove(this->m_DataStorage->GetNamedNode("sCurve Datanode"));

    }
    if (m_Selectiontype == 3)
    {
      this->GetDataStorage()->Remove(this->m_DataStorage->GetNamedNode("Kochanek Datanode"));

    }
    if (m_Selectiontype == 4)
    {
      this->GetDataStorage()->Remove(this->m_DataStorage->GetNamedNode("Cardinal Datanode"));

    }
    if (m_Selectiontype == 5)
    {
      this->GetDataStorage()->Remove(this->m_DataStorage->GetNamedNode("BSpline Datanode"));

    }
 
    m_Initialise = true;
    m_SplineDataObjectVisualisationfilter->FreezeUnFreezeStandartView(true, m_Toolcount);
    m_Controls.m_SpFreezeUnfreezeTrackingButton->setText("Unfreeze Standard View");
    m_Controls.m_SpTrackingFrozenLabel->setVisible(true);
    m_Controls.m_SpTrackingFrozenLabel->show();
  }
  else if (m_Controls.m_SpFreezeUnfreezeTrackingButton->text() == "Unfreeze Standard View")
  {
    m_SplineDataObjectVisualisationfilter->FreezeUnFreezeStandartView(false, m_Toolcount);
    m_Controls.m_SpFreezeUnfreezeTrackingButton->setText("Freeze Standard View");
    m_Controls.m_SpTrackingFrozenLabel->setVisible(false);
    m_Controls.m_SpTrackingFrozenLabel->hide();
    this->m_TrackingRenderTimer->start(1000);
  }
}

void SplineVisualisationPlugin::OnSelectionChanged(berry::IWorkbenchPart::Pointer source,
                                                          const QList<mitk::DataNode::Pointer> &nodes)
{
  // iterate all selected objects, adjust warning visibility
  foreach (mitk::DataNode::Pointer node, nodes)
  {
    if (node.IsNotNull() && dynamic_cast<mitk::Image *>(node->GetData()))
    {
      m_Controls.labelWarning->setVisible(false);
      m_Controls.ToogleSplinesButton->setEnabled(true);

      return;
    }
  }
}

void SplineVisualisationPlugin::StartReadingCSVFile()
{
  MITK_INFO << "Start QTimer..." << std::endl;
  // update position (sequential player)
  connect(this->m_PointSetTimer,
          SIGNAL(timeout()),
          SLOT(&SplineVisualisationEvaluationUtil::StartReadPointSet),
          Qt::QueuedConnection);
  this->m_PointSetTimer->start(1000);
}

void SplineVisualisationPlugin::StopReadPointSet()
{
  this->m_PointSetTimer->stop();
  disconnect(
    this->m_PointSetTimer, SIGNAL(timeout()), this, SLOT(&SplineVisualisationPlugin::StartReadPointSet));
  MITK_INFO << "QMITK Timer was stopped" << std::endl;
}


void SplineVisualisationPlugin::InitalRTEMSensorTracking(mitk::BaseRenderer *renderer, QWidget *parent)
{
  if (m_Controls.ToogleSplinesButtonIGTRTT->text() == "Activate Visualisation")
  {
    m_Controls.m_TrackingControlLabel->setText("Status: connected");
    m_Controls.m_SpTrackingFrozenLabel->hide();
    m_Controls.ToogleSplinesButtonIGTRTT->setText("Deactivate Visualisation");
    m_reConnect = true;
  }
  else
  {
    m_Controls.m_TrackingControlLabel->setText("Status: disconnected");
    m_Controls.ToogleSplinesButtonIGTRTT->setText("Activate Visualisation");
    StopTimer();
    m_reConnect = false;
    return;
  }
  if (m_Controls.m_SplineTrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetNumberOfOutputs() == NULL)
  {
    MITK_INFO << "A Problem occured: no selected data source detected" << std::endl;
    return;
  }

  // get TrackingToolboxAutoSave.IGTToolStorage
  SplineDataObjectVisualizationFilter *splineDataObjectVisFilter;
  QmitkMITKIGTTrackingToolboxView::Pointer qmitkMITKIGTtrackingtoolboxView;
  QmitkMITKIGTTrackingToolboxViewWorker qmitkMITKIGTtrackingtoolboxViewWorker = QmitkMITKIGTTrackingToolboxViewWorker();

  // Initialize views as axial, sagittal, coronar (from top-left to bottom)
  m_TimeGeometry = m_DataStorage->ComputeBoundingGeometry3D(m_DataStorage->GetAll());
  mitk::RenderingManager::GetInstance()->InitializeViews(m_TimeGeometry);

  m_ToolStorage = mitk::NavigationToolStorage::New(GetDataStorage());
  m_ToolStorage->SetName("TrackingToolbox Default Storage");
  m_ToolStorage->RegisterAsMicroservice();
  m_Trackingdevicedata = new mitk::TrackingDeviceData();
  // initialize worker thread
  m_ToolVisualizationFilter = mitk::NavigationDataObjectVisualizationFilter::New();
  m_ToolVisualizationFilter->Update();

  MITK_INFO << "Number of outputs ToolVisualizationFilter: " << m_ToolVisualizationFilter->GetNumberOfIndexedOutputs()
            << std::endl;
  MITK_INFO << "Number of inputs ToolVisualizationFilter: " << m_ToolVisualizationFilter->GetNumberOfIndexedInputs()
            << std::endl;

  MITK_INFO << "Number of Navigation Data Source Outputs : "
            << m_Controls.m_SplineTrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetNumberOfOutputs()
            << std::endl;
  MITK_INFO << "selected (internal) tool name : "
            << m_Controls.m_SplineTrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()
                 ->GetOutputNames()
                 .begin()
                 ->c_str()
            << std::endl;
  if (m_Controls.m_SplineTrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetNumberOfOutputs() >= 1)
    MITK_INFO
      << " #0 tool name : "
      << m_Controls.m_SplineTrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetOutput(0)->GetName()
      << std::endl;
  if (m_Controls.m_SplineTrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetNumberOfOutputs() >= 2)
    MITK_INFO
      << " #1 tool name : "
      << m_Controls.m_SplineTrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetOutput(1)->GetName()
      << std::endl;
  if (m_Controls.m_SplineTrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetNumberOfOutputs() >= 3)
    MITK_INFO
      << " #2 tool name : "
      << m_Controls.m_SplineTrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetOutput(2)->GetName()
      << std::endl;
  if (m_Controls.m_SplineTrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetNumberOfOutputs() >= 4)
  {
    MITK_INFO
      << " #3 tool name : "
      << m_Controls.m_SplineTrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetOutput(3)->GetName()
      << std::endl;
  }
  if (m_Controls.m_SplineTrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetNumberOfOutputs() >= 5)
  {
    MITK_INFO
      << " #4 tool name : "
      << m_Controls.m_SplineTrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetOutput(4)->GetName()
      << std::endl;
  }
  MITK_INFO << " tool outputs : "
            << m_Controls.m_SplineTrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()
                 ->GetOutputs()
                 .begin()
                 ->GetPointer()
            << std::endl;

  m_Controls.m_SpTrackingFrozenLabel->setVisible(false);
  m_Controls.m_TrackingControlLabel->setText("Status: connected");
  qDebug() << "Start Update QTimer...";

  // keep view and status widget
    renderer->SetKeepDisplayedRegion(true);
}
void SplineVisualisationPlugin::StopTrackingMechanism()
{
  // stop tracking
  m_TrackingRenderTimer->stop();
  m_TrackingLoggingTimer->stop();

  m_Worker->StopTrackingFinished(true, "Worker has stopped!");
  m_WorkerThread->start();
  m_Controls.IGTMeasurementRT->setEnabled(false);
}

void SplineVisualisationPlugin::GlobalReinit()
{
  // get all nodes that have not set "includeInBoundingBox" to false
  mitk::NodePredicateNot::Pointer pred = mitk::NodePredicateNot::New(
    mitk::NodePredicateProperty::New("includeInBoundingBox", mitk::BoolProperty::New(false)));

  mitk::DataStorage::SetOfObjects::ConstPointer rs = this->GetDataStorage()->GetSubset(pred);
  // calculate bounding geometry of these nodes
  auto bounds = this->GetDataStorage()->ComputeBoundingGeometry3D(rs, "visible");

  // initialize the views to the bounding geometry
  mitk::RenderingManager::GetInstance()->InitializeViews(bounds);
}

void SplineVisualisationPlugin::InitialiseTimerSlot()
{
  renderer->SetKeepDisplayedRegion(true);
  // set reference distance evaluation step
 
  if (m_Controls.AllQTabsWidget->currentIndex() == 1)
  {
    SetReferenceDistanceEvaluationStep(m_Controls.m_PSRTReferenceSplineComboBox->currentIndex());
    m_SplineDataObjectVisualisationfilter = new SplineDataObjectVisualizationFilter(this->m_ToolStorage, m_DataStorage);
    m_Toolcount = m_Controls.m_SplineTrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetNumberOfOutputs();
    m_NaviDataSourceSelection = m_Controls.m_SplineTrackingDeviceSelectionWidget;
    m_Selectiontype = m_Controls.m_TrackingPointerSelectionBox->currentIndex();
  }
  if (m_Controls.AllQTabsWidget->currentIndex() == 2)
  {
    SetReferenceDistanceEvaluationStep(m_Controls.m_PSRTReferenceSplineComboBox_Combined->currentIndex());
    m_Selectiontype = m_Controls.m_CombinedTrackingPointerSelectionBox->currentIndex();
    m_StepCounter = 0;
  }

  mitk::PointSet::Pointer realtimePointset = mitk::PointSet::New();
  m_Parametricspline = vtkSmartPointer<vtkParametricSpline>::New();
  m_SplineReturnDataNote = mitk::DataNode::New();
  m_SplineVtkMapper = mitk::SplineVtkMapper3D::New();
  m_SplineVtkMapper->SetSplineResolution(m_Splineresolution);
  m_TubeSurfaceVTKmapper = mitk::SurfaceVtkMapper3D::New();
  if (m_Selectiontype == 0)
  {
    mitk::DataNode::Pointer splineDataNode = mitk::DataNode::New();
    splineDataNode->SetName("Spline Datanode");
    splineDataNode->SetData(realtimePointset);
    splineDataNode->SetProperty("color", mitk::ColorProperty::New(0.0f, 0.0f, 1.0f)); // set to blue
    splineDataNode->SetProperty("pointsize", mitk::FloatProperty::New(3));
    splineDataNode->SetProperty("line width", mitk::IntProperty::New(3));
    splineDataNode->SetVisibility(true, this->renderer, "visable");
    this->m_DataStorage->Add(splineDataNode);
  }
  if (m_Selectiontype == 1)
  {
    mitk::DataNode::Pointer tubeDataNode = mitk::DataNode::New();
    tubeDataNode->SetName("3D Tube Datanode");
    tubeDataNode->SetData(realtimePointset);
    tubeDataNode->SetProperty("pointsize", mitk::FloatProperty::New(3));
    tubeDataNode->SetProperty("line width", mitk::IntProperty::New(3));
    tubeDataNode->SetVisibility(true, this->renderer, "visable");
    this->m_DataStorage->Add(tubeDataNode);
  }
  if (m_Selectiontype == 2)
  {
    // set the spline class
    vtkNew<vtkSCurveSpline> xSpline;
    vtkNew<vtkSCurveSpline> ySpline;
    vtkNew<vtkSCurveSpline> zSpline;
    m_Parametricspline->SetXSpline(xSpline);
    m_Parametricspline->SetYSpline(ySpline);
    m_Parametricspline->SetZSpline(zSpline);
    mitk::DataNode::Pointer sCurveDataNode = mitk::DataNode::New();
    sCurveDataNode->SetName("sCurve Datanode");
    sCurveDataNode->SetData(realtimePointset);
    sCurveDataNode->SetProperty("opacity", mitk::FloatProperty::New(1));
    sCurveDataNode->SetProperty("pointsize", mitk::FloatProperty::New(4.5));
    sCurveDataNode->SetBoolProperty("show contour", mitk::BoolProperty::New(true));
    sCurveDataNode->SetProperty("contoursize", mitk::FloatProperty::New(2.0));
    sCurveDataNode->SetColor(0.0, 1.0, 1.0);
    sCurveDataNode->SetVisibility(true, this->renderer, "visable");
    this->m_DataStorage->Add(sCurveDataNode);
  }
  if (m_Selectiontype == 3)
  {
    // set the spline class 
    vtkNew<vtkKochanekSpline> xSpline;
    vtkNew<vtkKochanekSpline> ySpline;
    vtkNew<vtkKochanekSpline> zSpline;
    m_Parametricspline->SetXSpline(xSpline);
    m_Parametricspline->SetYSpline(ySpline);
    m_Parametricspline->SetZSpline(zSpline);
    mitk::DataNode::Pointer kochanekDataNode = mitk::DataNode::New();
    kochanekDataNode->SetName("Kochanek Datanode");
    kochanekDataNode->SetData(realtimePointset);
    kochanekDataNode->SetProperty("pointsize", mitk::FloatProperty::New(3));
    kochanekDataNode->SetProperty("line width", mitk::IntProperty::New(3));
    kochanekDataNode->SetVisibility(true, this->renderer, "visable");
    this->m_DataStorage->Add(kochanekDataNode);
  }
  if (m_Selectiontype == 4)
  {
    mitk::DataNode::Pointer cardinalDataNode = mitk::DataNode::New();
    // set the spline class
    vtkNew<vtkCardinalSpline> xSpline;
    vtkNew<vtkCardinalSpline> ySpline;
    vtkNew<vtkCardinalSpline> zSpline;
    m_Parametricspline->SetXSpline(xSpline);
    m_Parametricspline->SetYSpline(ySpline);
    m_Parametricspline->SetZSpline(zSpline);
    cardinalDataNode->SetName("Cardinal Datanode");
    cardinalDataNode->SetData(realtimePointset);
    cardinalDataNode->SetProperty("color", mitk::ColorProperty::New(0.0f, 0.0f, 1.0f)); // set to blue
    cardinalDataNode->SetProperty("pointsize", mitk::FloatProperty::New(3));
    cardinalDataNode->SetProperty("line width", mitk::IntProperty::New(3));
    cardinalDataNode->SetVisibility(true, this->renderer, "visable");
    this->m_DataStorage->Add(cardinalDataNode);
  }
  if (m_Selectiontype == 5)
  {
    mitk::DataNode::Pointer bSplineDataNode = mitk::DataNode::New();
    bSplineDataNode->SetName("BSpline Datanode");
    bSplineDataNode->SetData(realtimePointset);
    bSplineDataNode->SetProperty("opacity", mitk::FloatProperty::New(1));
    bSplineDataNode->SetProperty("pointsize", mitk::FloatProperty::New(4.5));
    bSplineDataNode->SetBoolProperty("show contour", mitk::BoolProperty::New(true));
    bSplineDataNode->SetProperty("contoursize", mitk::FloatProperty::New(2.0));
    bSplineDataNode->SetColor(0.0, 1.0, 1.0);
    bSplineDataNode->SetVisibility(true, this->renderer, "visable");
    this->m_DataStorage->Add(bSplineDataNode);
  }
  if (m_Selectiontype == 6)
  {
    // set the spline class
    vtkNew<vtkKochanekSpline> xSpline;
    vtkNew<vtkKochanekSpline> ySpline;
    vtkNew<vtkKochanekSpline> zSpline;
    m_Parametricspline->SetXSpline(xSpline);
    m_Parametricspline->SetYSpline(ySpline);
    m_Parametricspline->SetZSpline(zSpline);
    mitk::DataNode::Pointer kochanekDataNode = mitk::DataNode::New();
    kochanekDataNode->SetName("optimized Kochanek Datanode");
    kochanekDataNode->SetData(realtimePointset);
    kochanekDataNode->SetProperty("pointsize", mitk::FloatProperty::New(3));
    kochanekDataNode->SetProperty("line width", mitk::IntProperty::New(3));
    kochanekDataNode->SetVisibility(true, this->renderer, "visable");
    this->m_DataStorage->Add(kochanekDataNode);
  }
  if (m_Controls.IGTMeasurementRT->isVisible())
  {
    m_SplineDataObjectVisualisationfilter->InitialiseInputTrackingData(
      m_Toolcount, m_Selectiontype, m_Controls.checkBoxVTKVisRT->isChecked(), m_CurrentRefDoubleSelection, m_Debug);
  }
  if (m_Controls.m_selectedAnchorpointscheckBox->isChecked() == false)
  {
    m_CombinedTempPointset = mitk::PointSet::New();
  }
  MITK_INFO << "Initialise..." << std::endl;
}

mitk::Surface::Pointer SplineVisualisationPlugin::getUpdateSurfaceTubeData()
{
  vtkNew<vtkNamedColors> colors;
  vtkNew<vtkParametricFunctionSource> functionSource;
  functionSource->SetParametricFunction(m_Parametricspline);
  functionSource->SetUResolution(10 * m_SplineReferencePointSet->GetSize());
  functionSource->Update();

  // changes to the name so that the datastorage can copy the datanode
  mitk::DataNode::Pointer Referencepointsdatanode = mitk::DataNode::New();
  Referencepointsdatanode->SetData(m_SplineReferencePointSet);
  Referencepointsdatanode->SetProperty("color", mitk::ColorProperty::New(0.0f, 0.0f, 1.0f)); // set to blue
  Referencepointsdatanode->SetProperty("line width", mitk::IntProperty::New(3));
  Referencepointsdatanode->SetProperty("pointsize", mitk::FloatProperty::New(5));
  Referencepointsdatanode->SetName("Selected Measured Points");

  // Interpolate the scalars
  vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius = vtkSmartPointer<vtkTupleInterpolator>::New();
  interpolatedRadius->SetInterpolationTypeToLinear();
  interpolatedRadius->SetNumberOfComponents(1);
  interpolatedRadius = m_SplineVisualisationEvaluationUtil->GetInterpolatedRadiusValuesFromTupleInterpolator(
    interpolatedRadius, m_SplineReferencePointSet);

  // Generate the radius scalars
  vtkNew<vtkDoubleArray> tubeRadius;
  unsigned int n = functionSource->GetOutput()->GetNumberOfPoints();
  tubeRadius->SetNumberOfTuples(n);
  tubeRadius->SetName("TubeRadius");
  auto tMin = interpolatedRadius->GetMinimumT();
  auto tMax = interpolatedRadius->GetMaximumT();
  double r = .1;
  for (int index = 0; index <= n; index++)
  {
    auto t = (tMax - tMin) / (n - 1) * index + tMin;
    interpolatedRadius->InterpolateTuple(t, &r);
    tubeRadius->SetTuple1(index, r);
  }

  // Add the scalars to the polydata
  auto tubePolyData = functionSource->GetOutput();
  tubePolyData->GetPointData()->AddArray(tubeRadius);
  tubePolyData->GetPointData()->SetActiveScalars("TubeRadius");

  // tubePolyData->GetPointData()->AddArray(colors);
  // Create the tubes
  vtkNew<vtkTubeFilter> tuber;
  tuber->SetInputData(tubePolyData);
  tuber->SetNumberOfSides(m_SplineReferencePointSet->GetSize());
  tuber->SetVaryRadiusToVaryRadiusOff();
  tuber->SetGenerateTCoords(tMax);
  tuber->SetRadius(7.0);
  tuber->SetProgressText("3D Spline Tube");
  tuber->SetGenerateTCoordsToUseScalars();
  tuber->UpdateDataObject();

  vtkSmartPointer<vtkImageData> tubeImageData = m_SplineVisualisationEvaluationUtil->convertVTKPolydataToVTKImagedata(
    tubePolyData,
    {static_cast<int>(tubeRadius->GetNumberOfTuples()),
     static_cast<int>(tubeRadius->GetNumberOfTuples()),
     static_cast<int>(tubeRadius->GetNumberOfTuples())}, 1);

  vtkNew<vtkTexture> tubeTexture;
  tubeTexture->SetInputData(tubeImageData);

  vtkNew<vtkProperty> tubeColorProp;
  tubeColorProp->SetBaseColorTexture(tubeTexture);

  // Generate MITK surface
  mitk::Surface::Pointer surfaceToPutTextureOn = mitk::Surface::New();
  surfaceToPutTextureOn->SetVtkPolyData(tuber->GetOutput());
  surfaceToPutTextureOn->SetProperty(
    "back color", mitk::ColorProperty::New(reinterpret_cast<const float *>(tubeColorProp->GetColor())));
  surfaceToPutTextureOn->SetProperty(
    "material.ambientCoefficient",
    mitk::FloatProperty::New(tubeColorProp->GetAmbient())); // set to yellow 1.0f, 1.0f, 0.0f
  surfaceToPutTextureOn->SetProperty(
    "material.diffuseCoefficient",
    mitk::FloatProperty::New(tubeColorProp->GetDiffuse())); // material.diffuseCoefficient
  surfaceToPutTextureOn->SetProperty("material.specularCoefficient",
                                     mitk::FloatProperty::New(tubeColorProp->GetSpecular()));
  surfaceToPutTextureOn->SetProperty("material.specularPower",
                                     mitk::FloatProperty::New(tubeColorProp->GetSpecularPower()));
  return surfaceToPutTextureOn;
}

void SplineVisualisationPlugin::UpdateCombinedTimerSlot() {

  // update position (sequential player)
  m_StepCounter = m_StepCounter + m_CounterofDN;
  if (NumUpdates ==0)
  {
    m_Printheader = true;
  } else
  {
    m_Printheader = false;
  }
  if (m_Debug)
  {
    MITK_INFO << "m_stepCounter:  " << m_StepCounter << std::endl;
  }
  if (m_Controls.m_selectedAnchorpointscheckBox->isChecked())
  {
    m_CombinedTempPointset = mitk::PointSet::New();
  }
  for (unsigned int i = m_StepCounter; i < m_StepCounter+m_CounterofDN; i++)
  {
    if (m_Controls.m_selectedAnchorpointscheckBox->isChecked())
    {
      m_CombinedTempPointset->InsertPoint(i, m_CombinedPointset.GetPointer()->GetPoint(i));

    }
    else
    {
      if (i % m_CounterofDN == 0 && m_CombinedPointset.GetPointer()->GetPoint(i)[0] != 0.0 &&
          m_CombinedPointset.GetPointer()->GetPoint(i)[1] != 0.0 &&
          m_CombinedPointset.GetPointer()->GetPoint(i)[2] != 0.0)
      {
       // TODO get a triangle with vectors, two sides of the triangle get prepedicular bisectors ,
       // compute intersection of lines for center (Planeintersectionvisitor)
        m_CombinedTempPointset->InsertPoint(i, m_CombinedPointset.GetPointer()->GetPoint(i)); 
      }
      
    }
    if (m_Debug)
      {
        MITK_INFO << "index : " << i << std::endl;
        MITK_INFO << "x : " << m_CombinedPointset.GetPointer()->GetPoint(i)[0]
                  << "y : " << m_CombinedPointset.GetPointer()->GetPoint(i)[1]
                  << "z: " << m_CombinedPointset.GetPointer()->GetPoint(i)[2] << std::endl;
      }
  }

    m_SplineReturnDataNote->SetData(m_CombinedTempPointset);

    if (m_Selectiontype == 0)
    {
      m_Parametricspline.GetPointer()->SetPoints(m_SplineVisualisationEvaluationUtil->ConvertMITKIntoVTKPointset(m_CombinedTempPointset));
      m_SplineReferencePointSet = m_SplineVisualisationEvaluationUtil->GenerateSplinePoints(m_Parametricspline, m_CombinedTempPointset, m_CurrentRefDoubleSelection);
      this->m_DataStorage->GetNamedNode("Spline Datanode")->SetData(m_SplineReferencePointSet);
      CombinedSplineMapperUpdate(this->m_DataStorage->GetNamedNode("Spline Datanode"));
      m_SplineVisualisationEvaluationUtil->CalculateParametricErrorMeasure(
        m_CombinedTempPointset, m_SplineReferencePointSet, "", false, true, m_Printheader);
    }
    if (m_Selectiontype == 1)
    {
      m_Parametricspline.GetPointer()->SetPoints(
        m_SplineVisualisationEvaluationUtil->ConvertMITKIntoVTKPointset(m_CombinedTempPointset));
      m_SplineReferencePointSet = m_SplineVisualisationEvaluationUtil->GenerateSplinePoints(
        m_Parametricspline, m_CombinedTempPointset, m_CurrentRefDoubleSelection);
      this->m_DataStorage->GetNamedNode("3D Tube Datanode")->SetData(m_SplineReferencePointSet);
      Combined3DTubeMapperUpdate(this->m_DataStorage->GetNamedNode("3D Tube Datanode"));
      m_SplineVisualisationEvaluationUtil->CalculateParametricErrorMeasure(
        m_CombinedTempPointset, m_SplineReferencePointSet, "", false, true, m_Printheader);
      m_reExecuteTube = true;
    }
    if (m_Selectiontype == 2)
    {
      m_Parametricspline.GetPointer()->SetPoints(
        m_SplineVisualisationEvaluationUtil->ConvertMITKIntoVTKPointset(m_CombinedTempPointset));
      m_SplineReferencePointSet = m_SplineVisualisationEvaluationUtil->GenerateSplinePoints(
        m_Parametricspline, m_CombinedTempPointset, m_CurrentRefDoubleSelection);
      this->m_DataStorage->GetNamedNode("sCurve Datanode")->SetData(m_SplineReferencePointSet);
      CombinedSplineMapperUpdate(this->m_DataStorage->GetNamedNode("sCurve Datanode"));
      m_SplineVisualisationEvaluationUtil->CalculateParametricErrorMeasure(
        m_CombinedTempPointset, m_SplineReferencePointSet, "", false, true, m_Printheader);
    }
    if (m_Selectiontype == 3)
    {
      m_Parametricspline.GetPointer()->SetPoints(
        m_SplineVisualisationEvaluationUtil->ConvertMITKIntoVTKPointset(m_CombinedTempPointset));
      m_SplineReferencePointSet = m_SplineVisualisationEvaluationUtil->GenerateSplinePoints(
        m_Parametricspline, m_CombinedTempPointset, m_CurrentRefDoubleSelection);
      this->m_DataStorage->GetNamedNode("Kochanek Datanode")->SetData(m_SplineReferencePointSet);
      CombinedSplineMapperUpdate(this->m_DataStorage->GetNamedNode("Kochanek Datanode"));
      m_SplineVisualisationEvaluationUtil->CalculateParametricErrorMeasure(
        m_CombinedTempPointset, m_SplineReferencePointSet, "", false, true, m_Printheader);
    }
    if (m_Selectiontype == 4)
    {
      m_Parametricspline.GetPointer()->SetPoints(
        m_SplineVisualisationEvaluationUtil->ConvertMITKIntoVTKPointset(m_CombinedTempPointset));
      m_SplineReferencePointSet = m_SplineVisualisationEvaluationUtil->GenerateSplinePoints(
        m_Parametricspline, m_CombinedTempPointset, m_CurrentRefDoubleSelection);
      this->m_DataStorage->GetNamedNode("Cardinal Datanode")->SetData(m_SplineReferencePointSet);
      CombinedSplineMapperUpdate(this->m_DataStorage->GetNamedNode("Cardinal Datanode"));
      m_SplineVisualisationEvaluationUtil->CalculateParametricErrorMeasure(
        m_CombinedTempPointset, m_SplineReferencePointSet, "", false, true, m_Printheader);
    }
    if (m_Selectiontype == 5)
    {
      this->m_DataStorage->GetNamedNode("BSpline Datanode")->SetData(m_SplineReturnDataNote->GetData());
      CombinedSplineMapperUpdate(this->m_DataStorage->GetNamedNode("BSpline Datanode"));
    }
    if (m_Selectiontype == 6)
    {
      m_Parametricspline.GetPointer()->SetPoints(
        m_SplineVisualisationEvaluationUtil->ConvertMITKIntoVTKPointset(m_CombinedTempPointset));
      m_SplineReferencePointSet = m_SplineVisualisationEvaluationUtil->GenerateSplinePoints(
        m_Parametricspline, m_CombinedTempPointset, m_CurrentRefDoubleSelection);
      this->m_DataStorage->GetNamedNode("optimized Kochanek Datanode")->SetData(m_SplineReferencePointSet);
      CombinedSplineMapperUpdate(this->m_DataStorage->GetNamedNode("optimized Kochanek Datanode"));
      m_SplineVisualisationEvaluationUtil->CalculateParametricErrorMeasure(
        m_CombinedTempPointset, m_SplineReferencePointSet, "", false, true, m_Printheader);
    }
  
  m_reExecuteSpline = true;
  qDebug() << "Update ... " << NumUpdates << "";
  NumUpdates++;
}

  void SplineVisualisationPlugin::UpdateTimerSlot(){

  // update position (sequential player)
  renderer->SetKeepDisplayedRegion(true);
  m_SplineReturnDataNote = m_SplineDataObjectVisualisationfilter->InputTrackingData(
    m_NaviDataSourceSelection, m_DataStorage, m_Controls.m_checkboxAllPointsSpline->isChecked()); 

  if (m_Selectiontype == 0)
  {
    this->m_DataStorage->GetNamedNode("Spline Datanode")->SetData(m_SplineReturnDataNote->GetData());
    CombinedSplineMapperUpdate(this->m_DataStorage->GetNamedNode("Spline Datanode"));
  }
  if (m_Selectiontype == 1)
  {
    this->m_DataStorage->GetNamedNode("3D Tube Datanode")->SetData(m_SplineReturnDataNote->GetData());
    Combined3DTubeMapperUpdate(this->m_DataStorage->GetNamedNode("3D Tube Datanode"));
    m_reExecuteTube = true;
  }
  if (m_Selectiontype == 2)
  {
    this->m_DataStorage->GetNamedNode("sCurve Datanode")->SetData(m_SplineReturnDataNote->GetData());
    CombinedSplineMapperUpdate(this->m_DataStorage->GetNamedNode("sCurve Datanode"));
  }
  if (m_Selectiontype == 3)
  {
    this->m_DataStorage->GetNamedNode("Kochanek Datanode")->SetData(m_SplineReturnDataNote->GetData());
    CombinedSplineMapperUpdate(this->m_DataStorage->GetNamedNode("Kochanek Datanode"));
  }
  if (m_Selectiontype == 4)
  {
    this->m_DataStorage->GetNamedNode("Cardinal Datanode")->SetData(m_SplineReturnDataNote->GetData());
    CombinedSplineMapperUpdate(this->m_DataStorage->GetNamedNode("Cardinal Datanode"));
  }
  if (m_Selectiontype == 5)
  {
    this->m_DataStorage->GetNamedNode("BSpline Datanode")->SetData(m_SplineReturnDataNote->GetData());
    Combined3DTubeMapperUpdate(this->m_DataStorage->GetNamedNode("BSpline Datanode"));
    m_reExecuteTube = true;
  }
  if (m_Selectiontype == 6)
  {
    this->m_DataStorage->GetNamedNode("optimized Kochanek Datanode")->SetData(m_SplineReturnDataNote->GetData());
    CombinedSplineMapperUpdate(this->m_DataStorage->GetNamedNode("optimized Kochanek Datanode"));
  }
  m_reExecuteSpline = true;
  qDebug() << "Update ... " << NumUpdates << "";
  NumUpdates++;
}

void SplineVisualisationPlugin::StopTimer()
{
  this->m_TrackingRenderTimer->stop();
  this->disconnect(m_TrackingRenderTimer, SIGNAL(timeout()), this, SLOT(UpdateTimerSlot()));
  this->disconnect(m_TrackingRenderTimer, SIGNAL(timeout()), this, SLOT(UpdateCombinedTimerSlot()));
  m_TrackingRenderTimer = new QTimer();
  this->m_TrackingRenderTimer->setInterval(10 / m_Fps); // 100/60
  this->m_TrackingRenderTimer->stop();
  MITK_INFO << "QMITK Timer was stopped" << std::endl;
}

void SplineVisualisationPlugin::Combined3DTubeMapperUpdate(mitk::DataNode::Pointer surfaceNode)
{
  //(only the surface geometry should be used for rendering)
  renderer->GetDataStorage()->Remove(renderer->GetDataStorage()->GetNamedNode("ImageDatanode"));
  // execute combined Update for Tube Mapper
  m_TubeSurfaceVTKmapper =  mitk::SurfaceVtkMapper3D::New();
  m_TubeSurfaceVTKmapper->SetDataNode(surfaceNode);
  m_TubeSurfaceVTKmapper->SetDefaultProperties(surfaceNode, renderer);
  surfaceNode->SetMapper(mitk::BaseRenderer::StandardMapperSlot::Standard3D, m_TubeSurfaceVTKmapper);
  // add all datanodes to the standalone datastorage

 
    if (!(m_reExecuteSpline))
  {
    for (int i = 0; i < this->GetDataStorage()->GetAll()->Size(); i++)
    {
      this->m_DataStorage->Add(this->GetDataStorage()->GetAll()->GetElement(i).GetPointer());
    }
    this->GetDataStorage()->Add(m_TubeSurfaceVTKmapper->GetDataNode());
  }
  else
  {
    // first remove old datanode from general datastorage
    this->GetDataStorage()->Remove(
      this->GetDataStorage()->GetNamedNode(m_TubeSurfaceVTKmapper->GetDataNode()->GetName()));
    // second, add converted datanode to standalone datastorage
    this->GetDataStorage()->Add(m_TubeSurfaceVTKmapper->GetDataNode());
  }

  renderer->SetDataStorage(this->m_DataStorage);
  renderer->SetMapperID(mitk::BaseRenderer::StandardMapperSlot::Standard3D);
  // add standalone datastorage to the RenderingManager
  mitk::RenderingManager::GetInstance()->SetDataStorage(this->m_DataStorage);
}

void SplineVisualisationPlugin::CombinedSplineMapperUpdate(mitk::DataNode::Pointer splinePointsDatanode)
{
  // get the standard baserenderer the 3D part of the QmitkRenderWindow
  renderer->mitk::BaseRenderer::GetInstance(GetRenderWindowPart()->GetQmitkRenderWindow("3d")->GetRenderWindow());
  renderer->SetKeepDisplayedRegion(true);
  // set the datanode to the mapper 
  m_SplineVtkMapper->SetDataNode(splinePointsDatanode);
  splinePointsDatanode->SetMapper(mitk::BaseRenderer::StandardMapperSlot::Standard3D, m_SplineVtkMapper);
  m_SplineVtkMapper->UpdateSpline();
    
    // add all dataNodes to the standalone datastorage

    if (!(m_reExecuteSpline))
    {
      for (int i = 0; i < this->GetDataStorage()->GetAll()->Size(); i++)
      {
        this->m_DataStorage->Add(this->GetDataStorage()->GetAll()->GetElement(i).GetPointer());
      }
      this->GetDataStorage()->Add(m_SplineVtkMapper->GetDataNode());
    }
    else
    {
      // remove and add datanode again for update to occure
      this->GetDataStorage()->Remove(this->GetDataStorage()->GetNamedNode(m_SplineVtkMapper->GetDataNode()->GetName()));
      this->GetDataStorage()->Add(m_SplineVtkMapper->GetDataNode());
      
    }
      // set the current standalone datastorage to the plugin Baserenderer
      renderer->SetDataStorage(this->m_DataStorage);
      // set the mapper id to identify standart mapper slot, where the update should occure
      renderer->SetMapperID(mitk::BaseRenderer::StandardMapperSlot::Standard3D);
      // add standalone datastorage to the RenderingManager
      mitk::RenderingManager::GetInstance()->SetDataStorage(this->m_DataStorage);
    
}


void SplineVisualisationPlugin::CommonTubeRenderFunction(mitk::BaseRenderer::Pointer renderer,
                                                         vtkParametricSpline *spline)
{
  vtkSPtrNew(const textPointInfo, vtkTextActor);
  vtkNew<vtkNamedColors> colors;
  vtkNew<vtkParametricFunctionSource> functionSource;
  functionSource->SetParametricFunction(spline);
  functionSource->SetUResolution(10 * m_SplineReferencePointSet->GetSize());
  functionSource->Update();
  
  // changes to the name so that the datastorage can copy the datanode
  mitk::DataNode::Pointer Referencepointsdatanode = mitk::DataNode::New();
  Referencepointsdatanode->SetData(m_SplineReferencePointSet);
  Referencepointsdatanode->SetProperty("color", mitk::ColorProperty::New(0.0f, 0.0f, 1.0f)); // set to blue
  Referencepointsdatanode->SetProperty("line width", mitk::IntProperty::New(5));
  Referencepointsdatanode->SetProperty("pointsize", mitk::FloatProperty::New(7));
  Referencepointsdatanode->SetName("Selected Measured Points");

  // Interpolate the scalars
  vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius = vtkSmartPointer<vtkTupleInterpolator>::New();
  interpolatedRadius->SetInterpolationTypeToLinear();
  interpolatedRadius->SetNumberOfComponents(2);
  interpolatedRadius = m_SplineVisualisationEvaluationUtil->GetInterpolatedRadiusValuesFromTupleInterpolatorTime(interpolatedRadius, m_SplineReferencePointSet);
 
  // Generate the radius scalars
  vtkNew<vtkDoubleArray> tubeRadius;
  unsigned int n = functionSource->GetOutput()->GetNumberOfPoints();
  tubeRadius->SetNumberOfTuples(n);
  tubeRadius->SetName("TubeRadius");
  auto tMin = interpolatedRadius->GetMinimumT();
  auto tMax = interpolatedRadius->GetMaximumT();
  double r =.1;
  double t;
  for (int index = 0; index <= n; index++)
  {
    t = (tMax - tMin) / (n - 1) * index + tMin;
    interpolatedRadius->InterpolateTuple(t, &r);
    tubeRadius->SetTuple1(index, r);
  }

  // Add the scalars to the polydata
  auto tubePolyData = functionSource->GetOutput();
  tubePolyData->GetPointData()->AddArray(tubeRadius);
  tubePolyData->GetPointData()->SetActiveScalars("TubeRadius");

  //tubePolyData->GetPointData()->AddArray(colors);
  // Create the tubes
  vtkNew<vtkTubeFilter> tuber;
  tuber->SetInputData(tubePolyData);
  tuber->SetNumberOfSides(m_SplineReferencePointSet->GetSize());
  tuber->SetVaryRadiusToVaryRadiusOff();
  tuber->SetGenerateTCoords(tMax);
  tuber->SetRadius(5.0); // 7.0
  tuber->SetProgressText("3D Spline Tube");
  tuber->SetGenerateTCoordsToUseScalars();
  tuber->UpdateDataObject();

  // Create a lookup table that consists of the full hue circle
  // (from HSV).
  auto belowRangeColor = colors->GetColor4d("Black").GetData();
  belowRangeColor[3] = 0.2;
  auto aboveRangeColor = colors->GetColor4d("White").GetData();
  aboveRangeColor[3] = 0.2;
  vtkNew<vtkLookupTable> hueLut;
  hueLut->SetTableRange(-1.0, tMax);
  hueLut->SetHueRange(0.667, 0);
  hueLut->SetSaturationRange(1, 1);
  hueLut->SetValueRange(0, 1);
  hueLut->UseBelowRangeColorOn();
  hueLut->SetBelowRangeColor(belowRangeColor);
  hueLut->UseAboveRangeColorOn();
  hueLut->SetAboveRangeColor(aboveRangeColor);
  hueLut->SetNumberOfColors(5);
  double *last = hueLut->GetTableValue(4);
  hueLut->SetAboveRangeColor(last[0], last[1], last[2], 0);
  hueLut->Build();

  vtkNew<vtkPolyDataMapper> tubeMapper;
  tubeMapper->SetInputConnection(tuber->GetOutputPort());
  tubeMapper->SetScalarRange(tubePolyData->GetScalarRange());

  // Actor for the Tube
  vtkNew<vtkActor> tubeActor;
  tubeActor->SetMapper(tubeMapper);
  // TubeActor Propertys
  tubeActor->GetProperty()->SetOpacity(0.9);
  tubeActor->GetProperty()->SetLineWidth(5.5f);

  vtkSmartPointer<vtkImageData> tubeImageData = m_SplineVisualisationEvaluationUtil->convertVTKPolydataToVTKImagedata(
    tubePolyData,
    {static_cast<int>(tubeRadius->GetNumberOfTuples()),
     static_cast<int>(tubeRadius->GetNumberOfTuples()),
     static_cast<int>(tubeRadius->GetNumberOfTuples())},
    1);

   vtkNew<vtkTexture> tubeTexture;
  tubeTexture->SetInputData(tubeImageData);

  vtkNew<vtkProperty> tubeColorProp;
  tubeColorProp->SetRepresentationToSurface();
  tubeColorProp->SetBaseColorTexture(tubeTexture); //  Bug: The albedoTex texture is not in sRGB color space.

  vtkNew<vtkPolyDataNormals> normals;
  normals->SetInputConnection(tuber->GetOutputPort());

  vtkNew<vtkMaskPoints> mask;
  mask->SetInputConnection(normals->GetOutputPort());
  mask->SetOnRatio(8);
  mask->RandomModeOn();

  vtkNew<vtkGaussianSplatter> splatter;
  splatter->SetInputConnection(mask->GetOutputPort());
  splatter->SetSampleDimensions(100, 100, 100);
  splatter->SetEccentricity(2.5);
  splatter->NormalWarpingOn();
  splatter->SetScaleFactor(1.0);
  splatter->SetRadius(0.025);

  // clang-format on
  vtkNew<vtkContourFilter> contour;
  contour->SetInputConnection(splatter->GetOutputPort());
  contour->SetValue(0, 0.25);

  vtkNew<vtkPolyDataMapper> splatMapper;
  splatMapper->SetInputConnection(contour->GetOutputPort());
  splatMapper->SetLookupTable(hueLut);
  splatMapper->Update();

  vtkNew<vtkActor> splatActor;
  splatActor->SetMapper(splatMapper);
  // splatActor->SetProperty(tubeColorProp);  
 
  // Create a scalar bar
  vtkNew<vtkScalarBarActor> scalarBar;
  scalarBar->SetLookupTable(hueLut);
  scalarBar->SetTitle("Time Measure");
  scalarBar->SetNumberOfLabels(5);

  // get the standard baserenderer, specifically the 3D part of the QmitkRenderWindow
  renderer->mitk::BaseRenderer::GetInstance(GetRenderWindowPart()->GetQmitkRenderWindow("3d")->GetRenderWindow());

  // generate texture coordinates assuming that surface and texture can be mapped 1 to 1
  vtkSmartPointer<vtkFloatArray> textureCoordinates = vtkSmartPointer<vtkFloatArray>::New();
  textureCoordinates->SetNumberOfComponents(tubeRadius->GetNumberOfComponents()); 
  textureCoordinates->SetNumberOfTuples(tubeRadius->GetNumberOfTuples()*20); // 403*20 = 8060
  textureCoordinates->SetName("TextureCoordinates");
  MITK_INFO << "Number of Tuples: " << tubeRadius->GetNumberOfTuples() << std::endl;
  MITK_INFO << "Number of Components: " << tubeRadius->GetNumberOfComponents() << std::endl;
  // generate texture coordinates assuming that surface and texture can be mapped 1 to 1
  int *dims = tubeImageData->GetDimensions();

  for (unsigned int i = 0; i != sizeof(&dims[0]); ++i) //TODO find out what i do wrong with the array
    {
      int pixelID = i + sizeof(&dims[0]);
      float xNorm = (i / dims[0]); 
      textureCoordinates->InsertTuple1(pixelID, xNorm);
    }

  // Generate MITK surface
  mitk::Surface::Pointer surfaceToPutTextureOn = mitk::Surface::New();
  surfaceToPutTextureOn->SetVtkPolyData(tuber->GetOutput());
  surfaceToPutTextureOn->GetVtkPolyData()->GetPointData()->SetTCoords(textureCoordinates);
  surfaceToPutTextureOn->SetProperty("back color",
                                     mitk::ColorProperty::New(reinterpret_cast<const float *>(tubeColorProp->GetColor())));
  surfaceToPutTextureOn->SetProperty("material.ambientCoefficient", mitk::FloatProperty::New(tubeColorProp->GetAmbient())); // set to yellow 1.0f, 1.0f, 0.0f
  surfaceToPutTextureOn->SetProperty("material.diffuseCoefficient",
                           mitk::FloatProperty::New(tubeColorProp->GetDiffuse())); // material.diffuseCoefficient
  surfaceToPutTextureOn->SetProperty("material.specularCoefficient",
                                     mitk::FloatProperty::New(tubeColorProp->GetSpecular()));
  surfaceToPutTextureOn->SetProperty("material.specularPower",
                                     mitk::FloatProperty::New(tubeColorProp->GetSpecularPower())); 
  // Generate a node
  mitk::DataNode::Pointer surfaceNode = mitk::DataNode::New();
  surfaceNode->SetData(surfaceToPutTextureOn);
  surfaceNode->SetProperty("pointsize", mitk::IntProperty::New(5));
  surfaceNode->SetProperty("color mode", mitk::BoolProperty::New(true));
  surfaceNode->SetProperty("scalar visibility", mitk::BoolProperty::New(true));
  surfaceNode->SetName("Surface Node (Tube)");
  surfaceNode->Update();
  //(only the surface geometry should be used for rendering)
  renderer->GetDataStorage()->Remove(renderer->GetDataStorage()->GetNamedNode("ImageDatanode"));
  // execute combined Update for Tube Mapper
  mitk::SurfaceVtkMapper3D::Pointer TubeSurfaceVTKmapper = mitk::SurfaceVtkMapper3D::New();
  // execute combined Update for Tube Tip Mapper 
  mitk::SurfaceVtkMapper3D::Pointer TipSurfaceVtkMapper = mitk::SurfaceVtkMapper3D::New();
  TubeSurfaceVTKmapper->SetDataNode(surfaceNode);
  TubeSurfaceVTKmapper->SetDefaultProperties(surfaceNode);
  TubeSurfaceVTKmapper->SetObjectName("3d Tube Visualisation");
  surfaceNode->SetMapper(mitk::BaseRenderer::StandardMapperSlot::Standard3D, TubeSurfaceVTKmapper);
  // add all datanodes to the standalone datastorage
  if (!(m_reExecuteTube))
  {
    this->GetDataStorage()->Add(Referencepointsdatanode);
    this->GetDataStorage()->Add(TubeSurfaceVTKmapper->GetDataNode());
    m_TubeSurfaceVTKmapper = TubeSurfaceVTKmapper; // m_TubeGraphVTKmapper
    if (m_Controls.NumEMTrackSensorCombobox->isEnabled() && m_Controls.checkBoxEnableTipEMTracking->isChecked())
    {
      this->GetDataStorage()->Add(TubeSurfaceVTKmapper->GetDataNode());
    }
    for (int i = 0; i < this->GetDataStorage()->GetAll()->Size(); i++)
    {
      this->m_DataStorage->Add(this->GetDataStorage()->GetAll()->GetElement(i).GetPointer());
    }
    this->m_reExecuteTube = true;
  }
  else
  {
    // first remove old datanode from general datastorage
    this->GetDataStorage()->Remove(
      this->GetDataStorage()->GetNamedNode(m_TubeSurfaceVTKmapper->GetDataNode()->GetName()));
    // second adjust spline vtk mapper with new empty datanode
    m_TubeSurfaceVTKmapper =
      m_SplineVisualisationEvaluationUtil->ClearTubeVtkMapperProps(renderer, m_TubeSurfaceVTKmapper);
    // third, add converted datanode to standalone datastorage

    if (TubeSurfaceVTKmapper->GetDataNode() != m_TubeSurfaceVTKmapper->GetDataNode())
    {
      this->m_DataStorage->Add(m_TubeSurfaceVTKmapper->GetDataNode());
      this->m_DataStorage->Add(Referencepointsdatanode);
    }
    if (m_Controls.NumEMTrackSensorCombobox->isEnabled() &&
        m_Controls.checkBoxEnableTipEMTracking->isChecked()) // checkBoxEnableTipEMTracking
    {
      this->GetDataStorage()->Remove(
        this->GetDataStorage()->GetNamedNode(m_TubeSurfaceVTKmapper->GetDataNode()->GetName()));
      this->GetDataStorage()->Add(TipSurfaceVtkMapper->GetDataNode());
    }
  }

  renderer->SetDataStorage(this->m_DataStorage);
  renderer->SetMapperID(mitk::BaseRenderer::StandardMapperSlot::Standard3D);
  // add standalone datastorage to the RenderingManager
  mitk::RenderingManager::GetInstance()->SetDataStorage(this->m_DataStorage);


  if (m_Controls.checkBoxVTKVisualisation->isChecked())
  {
    // Setup render window, renderer, and interactor
    vtkNew<vtkRenderer> vtkrenderer;
    vtkNew<vtkRenderer> vtkrenderer2;

    vtkrenderer->UseHiddenLineRemovalOn();
    vtkNew<vtkRenderWindow> renderWindow;
    vtkNew<vtkRenderWindow> renderWindow2;
    renderWindow->AddRenderer(vtkrenderer);
    renderWindow2->AddRenderer(vtkrenderer2);
    renderWindow->SetSize(1000, 900);
    renderWindow2->SetSize(1000, 900);
    renderWindow->SetWindowName("TubesFromSplines");
    renderWindow2->SetWindowName("SplatFromSplines");

    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor2;
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindowInteractor2->SetRenderWindow(renderWindow2);
    vtkrenderer->AddActor(tubeActor);
    vtkrenderer->AddActor(scalarBar);
    vtkrenderer2->AddActor(splatActor);
    vtkrenderer2->AddActor(scalarBar);
    vtkrenderer->SetBackground(colors->GetColor3d("SlateGray").GetData());
    vtkrenderer2->SetBackground(colors->GetColor3d("SlateGray").GetData());
    // Render the scene
    renderWindowInteractor->Initialize();
    renderWindowInteractor2->Initialize();
    renderWindow->Render();
    renderWindow2->Render();
    renderWindowInteractor->Start(); 
    renderWindowInteractor2->Start(); 
  }
  tubeImageData = NULL;
   // delete[] &dims;
}

QString SplineVisualisationPlugin::SetPointSetText()
{
  bool ok;
  // Enter the name of the point set
  QString pointsetText = QInputDialog::getText(QApplication::activeWindow(),
                                               tr("Add point set..."),
                                               tr("Enter name for the point set"),
                                               QLineEdit::Normal,
                                               tr("Spline Anchor PointSet"),
                                               &ok);
  if (!ok || pointsetText.isEmpty())
  {
    QMessageBox msgWarning;
    msgWarning.setText("Warning message:\n Point set name was empty. Stopping operation.");
    msgWarning.setIcon(QMessageBox::Warning);
    msgWarning.setWindowTitle("Warning: no point set name ");
    msgWarning.exec();
    return NULL;
  }

  return pointsetText;
}

void SplineVisualisationPlugin::SetGroundTruthInPointSetListWidget()
{
  if (m_Controls.pointSplineSetListWidget == NULL || m_Controls.selectedPointSetWidget == NULL)
  {
   QWidget *parent = new QWidget(); 
   m_Controls.setupUi(parent);
  }
  this->m_Controls.pointSplineSetListWidget->SetPointSetNode(this->GetDataStorage()->GetNamedNode("GroundTruthPoints"));
  this->m_Controls.selectedPointSetWidget->SetCurrentSelectedNode(this->GetDataStorage()->GetNamedNode("GroundTruthPoints"));
}

mitk::PointSet::Pointer SplineVisualisationPlugin::GetCurrentPointsetInListWidget()
{
    return m_Controls.pointSplineSetListWidget->GetPointSet();
}

Ui::SplineVisualisationPluginControls SplineVisualisationPlugin::GetMControls()
{
  return this->m_Controls;
}

  void SplineVisualisationPlugin::CommonBsplineRenderFunction(mitk::BaseRenderer::Pointer renderer,
                                                            vtkParametricSpline *spline)
{

    vtkNew<vtkNamedColors> colors;
    vtkSPtrNew(textSource, vtkTextActor);
    vtkNew<vtkRenderWindow> vtkrenderWindow;
    vtkrenderWindow->SetSize(1000, 900);
    vtkrenderWindow->SetWindowName("BSplineTube");
    
    vtkNew<vtkParametricFunctionSource> functionSource;
    functionSource->SetParametricFunction(spline);
    functionSource->SetUResolution(10 * m_SplineReferencePointSet->GetSize());
    functionSource->Update();

    // Interpolate the scalars
    vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius = vtkSmartPointer<vtkTupleInterpolator>::New();
    interpolatedRadius->SetInterpolationTypeToLinear();
    interpolatedRadius->SetNumberOfComponents(1);
    interpolatedRadius = m_SplineVisualisationEvaluationUtil->GetInterpolatedRadiusValuesFromTupleInterpolator(interpolatedRadius, m_SplineReferencePointSet);

    // Generate the radius scalars
    vtkNew<vtkDoubleArray> tubeRadius;
    unsigned int n = functionSource->GetOutput()->GetNumberOfPoints();
    tubeRadius->SetNumberOfTuples(n);
    tubeRadius->SetName("BSplineTubeRadius");
    auto tMin = interpolatedRadius->GetMinimumT();
    auto tMax = interpolatedRadius->GetMaximumT();
    double r = .1;
    for (int index = 0; index <= n; index++)
    {
      auto t = (tMax - tMin) / (n - 1) * index + tMin;
      interpolatedRadius->InterpolateTuple(t, &r);
      tubeRadius->SetTuple1(index, r);
    }

    // Add the scalars to the polydata
    auto tubePolyData = functionSource->GetOutput();
    tubePolyData->GetPointData()->AddArray(tubeRadius);
    tubePolyData->GetPointData()->SetActiveScalars("TubeRadius");

    // Make a tube (filter)
    vtkSmartPointer<vtkTubeFilter> tuber = vtkSmartPointer<vtkTubeFilter>::New();
    tuber->SetInputData(tubePolyData);
    tuber->SetNumberOfSides(m_SplineReferencePointSet->GetSize());
    tuber->SetVaryRadiusToVaryRadiusByAbsoluteScalar();
    tuber->SetProgressText("3D BSpline Tube");
    tuber->UpdateDataObject();

   vtkSmartPointer<vtkPoints> spoints = vtkSmartPointer<vtkPoints>::New();
    spoints->SetNumberOfPoints(5);
   spoints->SetPoint(0,m_SplineReferencePointSet->GetPoint(0)[0],
                     m_SplineReferencePointSet->GetPoint(0)[1],
                     m_SplineReferencePointSet->GetPoint(0)[2]);
    spoints->SetPoint(1,
                      m_SplineReferencePointSet->GetPoint(0)[0],
                      m_SplineReferencePointSet->GetPoint(0)[1],
                      m_SplineReferencePointSet->GetPoint(0)[2]);
   spoints->SetPoint(2,
                     m_SplineReferencePointSet->GetPoint(0)[0],
                     m_SplineReferencePointSet->GetPoint(0)[1],
                     m_SplineReferencePointSet->GetPoint(0)[2]);
    spoints->SetPoint(3,
                      m_SplineReferencePointSet->GetPoint(0)[0],
                      m_SplineReferencePointSet->GetPoint(0)[1],
                      m_SplineReferencePointSet->GetPoint(0)[2]);
   spoints->SetPoint(4,
                     m_SplineReferencePointSet->GetPoint(0)[0],
                     m_SplineReferencePointSet->GetPoint(0)[1],
                     m_SplineReferencePointSet->GetPoint(0)[2]);


    vtkSmartPointer<vtkPoints> tpoints = vtkSmartPointer<vtkPoints>::New();
    tpoints->SetNumberOfPoints(5);
    tpoints->SetPoint(0, 0.000, 0.000, 0.800);
    tpoints->SetPoint(1, 0.000, 0.000, -0.200);
    tpoints->SetPoint(2, 0.433, 0.000, 0.350);
    tpoints->SetPoint(3, 0.433, 0.000, -0.150);
    tpoints->SetPoint(4, -0.000, 0.233, 0.350);
    tpoints->SetPoint(5, -0.000, 0.433, -0.150);
    tpoints->SetPoint(6, -0.433, -0.000, 0.350);
    tpoints->SetPoint(7, -0.433, -0.000, -0.150);
    tpoints->SetPoint(8, 0.000, -0.233, 0.350);
    tpoints->SetPoint(9, 0.000, -0.433, -0.150);


    vtkSmartPointer<vtkThinPlateSplineTransform> thin = vtkSmartPointer<vtkThinPlateSplineTransform>::New();
    thin->SetSourceLandmarks(spoints);
    thin->SetTargetLandmarks(tpoints);
    thin->SetBasisToR2LogR();

    // b-spline transform, no normals
    vtkSmartPointer<vtkTransformToGrid> transformToGrid = vtkSmartPointer<vtkTransformToGrid>::New();
    transformToGrid->SetInput(thin);
    transformToGrid->SetGridOrigin(-1.5, -1.5, -1.5);
    transformToGrid->SetGridExtent(0, 60, 0, 60, 0, 60);
    transformToGrid->SetGridSpacing(0.05, 0.05, 0.05);

    vtkSmartPointer<vtkImageBSplineCoefficients> bsplinecoeffs = vtkSmartPointer<vtkImageBSplineCoefficients>::New();
    bsplinecoeffs->SetInputConnection(transformToGrid->GetOutputPort());

    // Third pane: b-spline, normals
    vtkSmartPointer<vtkBSplineTransform> bsplinetransform = vtkSmartPointer<vtkBSplineTransform>::New();
    bsplinetransform->SetCoefficientConnection(bsplinecoeffs->GetOutputPort());

    vtkSmartPointer<vtkTransformPolyDataFilter> transformpolydfilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformpolydfilter->SetInputConnection(tuber->GetOutputPort());
    transformpolydfilter->SetTransform(bsplinetransform);

    vtkSmartPointer<vtkPolyDataMapper> polymapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    polymapper->SetInputConnection(transformpolydfilter->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(polymapper);
    actor->RotateY(90);
    actor->GetProperty()->SetColor(1, 0, 0);

    vtkSmartPointer<vtkRenderer> vtkrenderer = vtkSmartPointer<vtkRenderer>::New();
    vtkrenderer->SetViewport(0.25, 0.25, 0.75, 1.0);
    vtkrenderer->ResetCamera(-0.5, 0.5, -0.5, 0.5, -1, 1);
    vtkrenderer->AddActor(actor);

    vtkrenderWindow->AddRenderer(vtkrenderer);

    vtkSmartPointer<vtkRenderWindowInteractor> vtkrenderwindowinteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkrenderwindowinteractor->SetRenderWindow(vtkrenderWindow);

    // render and interact
    vtkrenderwindowinteractor->Render();
    vtkrenderWindow->Render();
    vtkrenderwindowinteractor->Start();
}


void SplineVisualisationPlugin::CommonSplineRenderFunction(mitk::BaseRenderer *renderer)
{
  if (m_reExecuteSpline)
  {
    this->GetDataStorage()->Remove(this->GetDataStorage()->GetNamedNode("Spline Anchor PointSet")); // Referencepointsdatanode3D
    this->GetDataStorage()->Remove(this->GetDataStorage()->GetNamedNode("Selected Measured Points"));
    this->GetDataStorage()->Remove(this->GetDataStorage()->GetNamedNode("CSV Points Datanode"));
    renderer->InitRenderer(renderer->GetVtkRenderer()->GetRenderWindow());
  }
  int i = 0;
  bool ok_method;
  string windowName = "";
  mitk::PointSet::Pointer splinePointSet = mitk::PointSet::New();
  mitk::PointSet::Pointer tipPointSet = mitk::PointSet::New();
  vector<mitk::Point3D> selectedDataset;
  vtkSPtrNew(textPointInfo, vtkTextActor);
  QString pointsetText;
  QStringList qAllDatasetsStringList;
  qAllDatasetsStringList.append("Select a CSV File...");
  qAllDatasetsStringList.append("Current selected pointset dataset (Widget)");

  QString datasetSelectionText = QInputDialog::getItem(QApplication::activeWindow(),
                                                       tr("Select a Method..."),
                                                       tr("Select the Method of the dataset retrival:"),
                                                       qAllDatasetsStringList,
                                                       1,
                                                       false,
                                                       &ok_method);

  if (!ok_method)
  {
    return;
  }
  if (datasetSelectionText.toStdString() == "Select a CSV File..." && ok_method)
  {
    mitk::PointSet::Pointer tempPointSet = m_SplineVisualisationEvaluationUtil->GetCSVPointset();

    for (auto iter = tempPointSet->Begin(); iter != tempPointSet->End(); iter++)
    {
      // aquire the mitk 3d Point
      if (iter.Index() % 5 == 0)
      {
        MITK_INFO << "Selected Subset CSV Pointset " << iter.Value()[0] << " " << iter.Value()[1] << " " << iter.Value()[2] << std::endl;
        splinePointSet->InsertPoint(i, iter.Value(), 0);
        i++;
      }
    }
    pointsetText = SetPointSetText();
    windowName = "CSV import spline";
  }
  if (datasetSelectionText.toStdString() == "Current selected pointset dataset (Widget)" && ok_method)
  {
    if (m_Controls.pointSplineSetListWidget->GetPointSetNode() == nullptr || 
        m_Controls.pointSplineSetListWidget->GetPointSet() == nullptr ||
        m_Controls.selectedPointSetWidget->GetSelectedNode()->GetData() == nullptr)
    {
      QMessageBox msgError;
      msgError.setText("Error Message: \n No currently selected pointset datanode available! \n Select a Dataset "
                       "or restart application.");
      msgError.setIcon(QMessageBox::Critical);
      msgError.setWindowTitle("Error");
      msgError.exec();
      return;
    }
    mitk::DataNode::Pointer tempSelectedDN = mitk::DataNode::New();
    tempSelectedDN->SetData(m_Controls.selectedPointSetWidget->GetSelectedNode()->GetData());
    splinePointSet = m_Controls.pointSplineSetListWidget->GetPointSet();
    pointsetText = SetPointSetText();
    windowName = "Current selected pointset dataset (Widget) Spline";
  }
  if (pointsetText == NULL)
  {
    return;
  }
  
  // standalone vtk window
  vtkNew<vtkNamedColors> colors;
  vtkRenderer *vtkrenderer = vtkRenderer::New();
  // Create a polydata to store everything in and update
  vtkNew<vtkPolyData> polyData;
  polyData->SetPoints(m_SplineVisualisationEvaluationUtil->ConvertMITKIntoVTKPointset(splinePointSet));
  polyData->UpdatePointGhostArrayCache();
  // adjust anchorpoint set size if needed
  if (m_Controls.totalAnchorpointsSpinBox->isEnabled() && m_Controls.checkBoxEnableAnchorselection->isChecked())
  {
    int anchorpointsToGen = m_Controls.totalAnchorpointsSpinBox->value();
    splinePointSet = m_SplineVisualisationEvaluationUtil->GenerateAdditionalAnchorpoints(splinePointSet, anchorpointsToGen);
  }

  // DataNode added to the DataStorage
  mitk::DataNode::Pointer Anchorpointsdatanode = mitk::DataNode::New();
  Anchorpointsdatanode->SetName(pointsetText.toStdString());
  Anchorpointsdatanode->SetColor(1.0f, 1.0f, 0.0f); // red
  Anchorpointsdatanode->SetProperty("pointsize", mitk::FloatProperty::New(5.0));
  Anchorpointsdatanode->SetProperty("contoursize", mitk::FloatProperty::New(2.0));
  Anchorpointsdatanode->SetProperty("show contour", mitk::BoolProperty::New(true));
  if (m_Controls.NumEMTrackSensorCombobox->isEnabled() &&
      m_Controls.checkBoxEnableTipEMTracking->isChecked()) 
  {
    // Get the Pointset for the tip of the endoscope only
    double iterCounter = (m_Controls.NumEMTrackSensorCombobox->currentIndex() + 3);
    auto iterSp = splinePointSet.GetPointer()->End();
    while (iterSp != splinePointSet.GetPointer()->Begin())
    {
      --iterSp;
      if (iterCounter != 0)
      {
        iterCounter--;
        mitk::Point3D temppoint;
        FillVector3D(temppoint, iterSp.Value()[0], iterSp.Value()[1], iterSp.Value()[2]);
        MITK_INFO << "index: " << iterSp.Index() << " temppoint: " << temppoint[0] << " " << temppoint[1] << " "
                  << temppoint[2] << std::endl;
        tipPointSet.GetPointer()->InsertPoint(iterSp.Index(), temppoint);
      }
    }
    bool flag = true;
    for (auto iter = tipPointSet.GetPointer()->Begin(); iter != tipPointSet.GetPointer()->End(); iter++)
    {
      if (flag)
      {
        iter++;
        flag = false;
      }
      splinePointSet.GetPointer()->RemovePointIfExists(iter.Index(), 0);
    }
  }
  else
  {
    Anchorpointsdatanode->SetData(splinePointSet);
  }
  this->GetDataStorage()->Add(Anchorpointsdatanode);

  vtkNew<vtkSplineWidget2> splineWidget;
  vtkNew<vtkSplineRepresentation> splineRepresentation;
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  vtkNew<vtkParametricFunctionSource> functionSource;
  vtkNew<vtkParametricSpline> parametricspline;
  if (m_Controls.VisMethodcomboBox->currentIndex() == 2)
  {
    vtkNew<vtkSCurveSpline> xSpline;
    vtkNew<vtkSCurveSpline> ySpline;
    vtkNew<vtkSCurveSpline> zSpline;
    parametricspline->SetXSpline(xSpline);
    parametricspline->SetYSpline(ySpline);
    parametricspline->SetZSpline(zSpline);
    parametricspline->SetPoints(m_SplineVisualisationEvaluationUtil->ConvertMITKIntoVTKPointset(splinePointSet));
    m_SplineReferencePointSet = m_SplineVisualisationEvaluationUtil->GenerateSplinePoints(
      parametricspline, splinePointSet, m_CurrentRefDoubleSelection);
    if (m_Controls.NumEMTrackSensorCombobox->isEnabled() &&
      m_Controls.checkBoxEnableTipEMTracking->isChecked())
    {
      m_TipSplineReferencePointSet = m_SplineVisualisationEvaluationUtil->GenerateSplinePoints(
        parametricspline, tipPointSet, m_CurrentRefDoubleSelection);

    }
  }
  if (m_Controls.VisMethodcomboBox->currentIndex() == 3 || m_Controls.VisMethodcomboBox->currentIndex() == 6)
  {
    QMessageBox infoBox;

    if (m_Controls.VisMethodcomboBox->currentIndex() == 6)
    {
      goto optimiseKochnek;
    }
    infoBox.setText("Info Message: \n Kochanek Bais, Tention, Confidence values are not optimal. Should the optimisation procedure be applied to the current Kochanek Spline?");
    infoBox.setIcon(QMessageBox::Information);
    infoBox.setWindowTitle("Kochanek Optimisation");
    infoBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    infoBox.setDefaultButton(QMessageBox::Yes);
    auto returnValue = infoBox.exec();

    if (returnValue == QMessageBox::Yes)
    {
      optimiseKochnek:
      m_SplineReferencePointSet =
        m_SplineVisualisationEvaluationUtil->GetKochanekOptimisations(parametricspline, splinePointSet, m_CurrentRefDoubleSelection);
      if (m_Controls.NumEMTrackSensorCombobox->isEnabled() && m_Controls.checkBoxEnableTipEMTracking->isChecked())
      {
        m_TipSplineReferencePointSet = m_SplineVisualisationEvaluationUtil->GetKochanekOptimisations(
          parametricspline, tipPointSet, m_CurrentRefDoubleSelection);
      }
    }
    else {
    // first initialisation
    vtkSmartPointer<vtkKochanekSpline> xSpline = vtkSmartPointer<vtkKochanekSpline>::New();
    vtkSmartPointer<vtkKochanekSpline> ySpline = vtkSmartPointer<vtkKochanekSpline>::New();
    vtkSmartPointer<vtkKochanekSpline> zSpline = vtkSmartPointer<vtkKochanekSpline>::New();

    parametricspline->SetXSpline(xSpline);
    parametricspline->SetYSpline(ySpline);
    parametricspline->SetZSpline(zSpline);
    parametricspline->SetPoints(m_SplineVisualisationEvaluationUtil->ConvertMITKIntoVTKPointset(splinePointSet));
    m_SplineReferencePointSet = m_SplineVisualisationEvaluationUtil->GenerateSplinePoints(
      parametricspline, splinePointSet, m_CurrentRefDoubleSelection);
    }
    if (m_Controls.NumEMTrackSensorCombobox->isEnabled() && m_Controls.checkBoxEnableTipEMTracking->isChecked())
    {
      parametricspline->SetPoints(m_SplineVisualisationEvaluationUtil->ConvertMITKIntoVTKPointset(tipPointSet));
      m_TipSplineReferencePointSet = m_SplineVisualisationEvaluationUtil->GenerateSplinePoints(
        parametricspline, tipPointSet, m_CurrentRefDoubleSelection);
    }
  }
  else if (m_Controls.VisMethodcomboBox->currentIndex() == 4)
  {
    // the spline class Range with absolute values
    vtkNew<vtkCardinalSpline> xSpline;
    vtkNew<vtkCardinalSpline> ySpline;
    vtkNew<vtkCardinalSpline> zSpline;
   
    parametricspline->SetXSpline(xSpline);
    parametricspline->SetYSpline(ySpline);
    parametricspline->SetZSpline(zSpline);
    parametricspline->SetPoints(m_SplineVisualisationEvaluationUtil->ConvertMITKIntoVTKPointset(splinePointSet));

    m_SplineReferencePointSet = m_SplineVisualisationEvaluationUtil->GenerateSplinePoints(parametricspline, splinePointSet, m_CurrentRefDoubleSelection);
    if (m_Controls.NumEMTrackSensorCombobox->isEnabled() && m_Controls.checkBoxEnableTipEMTracking->isChecked())
    {
      parametricspline->SetPoints(m_SplineVisualisationEvaluationUtil->ConvertMITKIntoVTKPointset(tipPointSet));
      m_TipSplineReferencePointSet = m_SplineVisualisationEvaluationUtil->GenerateSplinePoints(
        parametricspline, tipPointSet, m_CurrentRefDoubleSelection);
    }
  }
  else
  {
    // parametric spline class
    parametricspline->SetPoints(m_SplineVisualisationEvaluationUtil->ConvertMITKIntoVTKPointset(splinePointSet));
    // set the  interactor
    splineRepresentation->SetParametricSpline(parametricspline);
    splineWidget->SetRepresentation(splineRepresentation);
    splineWidget->SetInteractor(renderWindowInteractor);
    // get anchor and reference Points from the spline
    m_SplineReferencePointSet = m_SplineVisualisationEvaluationUtil->GenerateSplinePoints(parametricspline, splinePointSet, m_CurrentRefDoubleSelection);
    if (m_Controls.NumEMTrackSensorCombobox->isEnabled() && m_Controls.checkBoxEnableTipEMTracking->isChecked())
    {
      parametricspline->SetPoints(m_SplineVisualisationEvaluationUtil->ConvertMITKIntoVTKPointset(tipPointSet));
      m_TipSplineReferencePointSet = m_SplineVisualisationEvaluationUtil->GenerateSplinePoints(
        parametricspline, tipPointSet, m_CurrentRefDoubleSelection);
    }
    // Parametric function for spline
    functionSource->SetParametricFunction(parametricspline);
    functionSource->Update();

    if (m_Controls.VisMethodcomboBox->currentIndex() == 5)
    {
      CommonBsplineRenderFunction( renderer, parametricspline);
      return;
    }
    else if (m_Controls.VisMethodcomboBox->currentIndex() == 1)
    {
      CommonTubeRenderFunction(renderer, parametricspline);
      return;
    }
  }

  splineWidget->SetRepresentation(splineRepresentation);
  splineWidget->SetInteractor(renderWindowInteractor);
  splineWidget->SetPriority(1.0);
  splineWidget->KeyPressActivationOff();

  functionSource->SetParametricFunction(parametricspline);
  functionSource->SetUResolution(500);
  functionSource->SetVResolution(500);
  functionSource->SetWResolution(500);
  functionSource->Update();

  // Reference Points Q string
  QtempString = m_SplineVisualisationEvaluationUtil->VtkQStringTextBlock(splinePointSet, m_SplineReferencePointSet);
  textPointInfo->SetInput(QtempString.toLocal8Bit().data());
  textPointInfo->GetTextProperty()->SetFontSize(18);
  textPointInfo->GetTextProperty()->SetJustificationToLeft();
  textPointInfo->GetTextProperty()->SetVerticalJustificationToBottom();
  textPointInfo->GetTextProperty()->BoldOn();
  textPointInfo->GetTextProperty()->SetColor(1, 1, 1);

  // Poly data Mapper
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(functionSource->GetOutputPort());
  // Actor
  vtkNew<vtkActor> splineActor;
  splineActor->SetMapper(mapper);
  splineActor->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());
  splineActor->GetProperty()->SetRepresentationToWireframe();
  splineActor->GetProperty()->SetLineWidth(3.0);
  splineActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
  splineActor->GetMapper()->SetInputConnection(functionSource->GetOutputPort());
  vtkNew<vtkPolyDataMapper> polydatamapper;
  polydatamapper->SetInputConnection(functionSource->GetOutputPort());
  polydatamapper->SetInputData(polyData);
  vtkNew<vtkGlyph3DMapper> pointMapper;
  pointMapper->SetInputData(polyData);
  pointMapper->SetInputConnection(functionSource->GetOutputPort());
  // vtkActor
  vtkNew<vtkActor> pointActor;
  pointActor->SetMapper(pointMapper);
  pointActor->GetProperty()->SetColor(colors->GetColor3d("White").GetData());
  vtkrenderer->AddActor(splineActor);
  vtkrenderer->AddActor(pointActor);

  // Setup render window, renderer, and interactor
  vtkNew<vtkRenderWindow> vtkrenderWindow;
  vtkrenderWindow->AddRenderer(vtkrenderer);
  vtkrenderWindow->SetWindowName(windowName.c_str());
  // vtkRenderWindow Interactor
  renderWindowInteractor->SetRenderWindow(vtkrenderWindow);
  vtkrenderer->AddActor(textPointInfo);
  vtkrenderer->SetBackground(colors->GetColor3d("Black").GetData());
  // get the standard baserenderer the 3D partof the QmitkRenderWindow
  renderer->mitk::BaseRenderer::GetInstance(GetRenderWindowPart()->GetQmitkRenderWindow("3d")->GetRenderWindow());
  mitk::DataNode::Pointer EndTipPointsdatanode = mitk::DataNode::New();
  mitk::DataNode::Pointer TipRefpointsdatanode = mitk::DataNode::New();
  if (m_Controls.NumEMTrackSensorCombobox->isEnabled() &&
      m_Controls.checkBoxEnableTipEMTracking->isChecked()) 
  {
    // changes to the name so that the datastorage can copy the datanode
    EndTipPointsdatanode->SetData(tipPointSet);
    EndTipPointsdatanode->SetProperty("color", mitk::ColorProperty::New(0.0f, 0.5f, 0.0f)); // set to green
    EndTipPointsdatanode->SetProperty("line width", mitk::IntProperty::New(3));
    EndTipPointsdatanode->SetProperty("pointsize", mitk::FloatProperty::New(5));
    EndTipPointsdatanode->SetProperty("show contour", mitk::BoolProperty::New(true));
    EndTipPointsdatanode->SetName("Tip Points");
    // changes to the name so that the datastorage can copy the datanode
    TipRefpointsdatanode->SetData(m_TipSplineReferencePointSet);
    TipRefpointsdatanode->SetProperty("color", mitk::ColorProperty::New(0.0f, 0.5f, 0.0f)); // set to blue
    TipRefpointsdatanode->SetProperty("line width", mitk::IntProperty::New(3));
    TipRefpointsdatanode->SetProperty("pointsize", mitk::FloatProperty::New(5));
    TipRefpointsdatanode->SetName("Tip Measured Points");
  }
  // changes to the name so that the datastorage can copy the datanode
  mitk::DataNode::Pointer Referencepointsdatanode = mitk::DataNode::New();
  Referencepointsdatanode->SetData(m_SplineReferencePointSet); // default color 
  Referencepointsdatanode->SetProperty("color", mitk::ColorProperty::New(0.0f, 0.0f, 1.0f)); // set to blue
  if (m_Controls.VisMethodcomboBox->currentIndex() == 2)  // S Curve Spline
    Referencepointsdatanode->SetProperty("color", mitk::ColorProperty::New(1.00f, 1.00f, 0.0f)); // set to yellow
  else if (m_Controls.VisMethodcomboBox->currentIndex() == 3 ||
           m_Controls.VisMethodcomboBox->currentIndex() == 6)   //  Kochanek Spline
    Referencepointsdatanode->SetProperty("color", mitk::ColorProperty::New(0.75f, 0.35f, 0.0f));   // set to orange
  else if (m_Controls.VisMethodcomboBox->currentIndex() == 4) // Cardinal Spline
    Referencepointsdatanode->SetProperty("color", mitk::ColorProperty::New(0.0f, 1.0f, 0.0f)); // set to green
 
  Referencepointsdatanode->SetProperty("line width", mitk::IntProperty::New(3));
  Referencepointsdatanode->SetProperty("pointsize", mitk::FloatProperty::New(5));
  Referencepointsdatanode->SetName("Reference Measured Points");

  // execute combined Update for Spline Mapper
  mitk::SplineVtkMapper3D::Pointer SplineVtkMapper = mitk::SplineVtkMapper3D::New();
  mitk::SplineVtkMapper3D::Pointer TipSplineVtkMapper = mitk::SplineVtkMapper3D::New();

  // convert Reference Datanode
  SplineVtkMapper->SetDataNode(Referencepointsdatanode);
  SplineVtkMapper->SetSplineResolution(m_Splineresolution);
  Referencepointsdatanode->SetMapper(mitk::BaseRenderer::StandardMapperSlot::Standard3D, SplineVtkMapper);
  SplineVtkMapper->UpdateSpline();

  if (m_Controls.NumEMTrackSensorCombobox->isEnabled() && m_Controls.checkBoxEnableTipEMTracking->isChecked()) 
  {
    // convert Reference Datanode
    TipSplineVtkMapper->SetDataNode(TipRefpointsdatanode);
    TipSplineVtkMapper->SetSplineResolution(m_Splineresolution);
    TipRefpointsdatanode->SetMapper(mitk::BaseRenderer::StandardMapperSlot::Standard3D, TipSplineVtkMapper);
    TipSplineVtkMapper->UpdateSpline();
  }
  // add all datanodes to the standalone datastorage
  if (!(m_reExecuteSpline))
  {
    this->GetDataStorage()->Add(SplineVtkMapper->GetDataNode());
    m_SplineVtkMapper = SplineVtkMapper;
    if (m_Controls.NumEMTrackSensorCombobox->isEnabled() && m_Controls.checkBoxEnableTipEMTracking->isChecked()) 
    {
      this->GetDataStorage()->Add(TipSplineVtkMapper->GetDataNode());
      this->GetDataStorage()->Add(EndTipPointsdatanode);
      m_TipSplineVtkMapper = TipSplineVtkMapper;
    }

    for (int i = 0; i < this->GetDataStorage()->GetAll()->Size(); i++)
    {
      this->m_DataStorage->Add(this->GetDataStorage()->GetAll()->GetElement(i).GetPointer());
    }
    this->m_reExecuteSpline = true;

    
  }
  else
  {
      // first remove old datanode from general datastorage
      this->GetDataStorage()->Remove(this->GetDataStorage()->GetNamedNode(m_SplineVtkMapper->GetDataNode()->GetName()));
      // second adjust spline vtk mapper with new empty datanode
      m_SplineVtkMapper = m_SplineVisualisationEvaluationUtil->ClearSplineVtkMapperProps(renderer, m_SplineVtkMapper);
      // third, add converted datanode to standalone datastorage
      m_SplineVtkMapper = SplineVtkMapper;
      Referencepointsdatanode->SetMapper(mitk::BaseRenderer::StandardMapperSlot::Standard3D, m_SplineVtkMapper);
      m_SplineVtkMapper->UpdateSpline();
      this->m_DataStorage->Add(m_SplineVtkMapper->GetDataNode());
      this->GetDataStorage()->Add(m_SplineVtkMapper->GetDataNode());

    if (m_Controls.NumEMTrackSensorCombobox->isEnabled() && m_Controls.checkBoxEnableTipEMTracking->isChecked())
    {
      // first remove old datanode from general datastorage
      this->GetDataStorage()->Remove(
        this->GetDataStorage()->GetNamedNode(TipSplineVtkMapper->GetDataNode()->GetName()));
      this->GetDataStorage()->Remove(
        this->GetDataStorage()->GetNamedNode(EndTipPointsdatanode->GetName()));
      if (m_TipSplineVtkMapper.GetPointer() != nullptr)
      {
        // second adjust spline vtk mapper with new empty datanode
        m_TipSplineVtkMapper =
          m_SplineVisualisationEvaluationUtil->ClearSplineVtkMapperProps(renderer, m_TipSplineVtkMapper);
      }
      else
      {
          this->GetDataStorage()->Add(TipSplineVtkMapper->GetDataNode());
          this->GetDataStorage()->Add(EndTipPointsdatanode);
          m_TipSplineVtkMapper = TipSplineVtkMapper;
      }
      // third, add converted datanode to standalone datastorage
      if (TipSplineVtkMapper->GetDataNode() != m_TipSplineVtkMapper->GetDataNode())
      {
        this->m_DataStorage->Add(TipSplineVtkMapper->GetDataNode());
        this->GetDataStorage()->Add(TipSplineVtkMapper->GetDataNode());

      }

    }
    
  }

  renderer->SetDataStorage(this->m_DataStorage);
  renderer->SetMapperID(mitk::BaseRenderer::StandardMapperSlot::Standard3D);
  // add standalone datastorage to the RenderingManager
  mitk::RenderingManager::GetInstance()->SetDataStorage(this->m_DataStorage);

  if (m_Controls.checkBoxVTKVisualisation->isChecked())
  {
    // A interactor pipeline
    splineWidget->SetInteractor(renderWindowInteractor);
    // vtk Render and Window interactor
    vtkrenderer->ResetCamera();
    vtkrenderWindow->SetSize(1000, 900);
    vtkrenderWindow->AddRenderer(vtkrenderer);
    // Render the scene
    renderWindowInteractor->SetRenderWindow(vtkrenderWindow);
    // Correct the vtkWindow positioning of the textPointInfo
    int *winSize = vtkrenderWindow->GetSize();
    double size[2] = {0};
    textPointInfo->GetSize(vtkrenderer, size);
    textPointInfo->SetPosition(winSize[0] - size[0] - 30, winSize[1] - size[1] - 10);
    // Render the scene
    vtkrenderWindow->Render();
    splineWidget->On();
    renderWindowInteractor->Initialize();
    renderWindowInteractor->Start();
  }
  // Error Calculation: RMSE, MSE, Euclidian Distance, Jitter
  if (this->GetDataStorage()->GetNamedNode("GroundTruthPoints") != nullptr)
  {
    m_Controls.pointSplineSetListWidget->SetPointSetNode(this->GetDataStorage()->GetNamedNode("GroundTruthPoints"));
    m_Controls.selectedPointSetWidget->SetCurrentSelectedNode(this->GetDataStorage()->GetNamedNode("GroundTruthPoints"));
    m_SplineVisualisationEvaluationUtil->CalculateParametricErrorMeasure(
      m_Controls.pointSplineSetListWidget->GetPointSet(),
      m_SplineReferencePointSet,
      "Closest GT Point values",
      true,
      false,
      m_Printheader);
    if (m_Controls.NumEMTrackSensorCombobox->isEnabled() && m_Controls.checkBoxEnableTipEMTracking->isChecked())
    {
      m_SplineVisualisationEvaluationUtil->CalculateParametricErrorMeasure(
        m_Controls.pointSplineSetListWidget->GetPointSet(),
        m_TipSplineReferencePointSet,
        "Closest GT Tip Point values",
        true,
        false,
        m_Printheader);
    }

  }
  else
  {
    QMessageBox msgError;
    msgError.setText("Error Message: \n No Ground Truth Points datanode available! \n Open the Dataset "
                     "or restart application.");
    msgError.setIcon(QMessageBox::Critical);
    msgError.setWindowTitle("Error");
    msgError.exec();
  }
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

/*
 * @brief taken and modified form @QmitkPointSetInteractionView Plug-in
 *
 * @ param IRenderwindow Part controls for interaction with the individual views and adds them to the Navigation
 * Controller
 *
 */
void SplineVisualisationPlugin::RenderWindowPartActivated(mitk::IRenderWindowPart *renderWindowPart)
{
  if (nullptr != m_Controls.pointSplineSetListWidget)
  {
    m_Controls.pointSplineSetListWidget->AddSliceNavigationController(
      renderWindowPart->GetQmitkRenderWindow("axial")->GetSliceNavigationController());
    m_Controls.pointSplineSetListWidget->AddSliceNavigationController(
      renderWindowPart->GetQmitkRenderWindow("sagittal")->GetSliceNavigationController());
    m_Controls.pointSplineSetListWidget->AddSliceNavigationController(
      renderWindowPart->GetQmitkRenderWindow("coronal")->GetSliceNavigationController());
    m_Controls.pointSplineSetListWidget->AddSliceNavigationController(
      renderWindowPart->GetQmitkRenderWindow("3d")->GetSliceNavigationController());
  }
}

/*
 * @brief taken and modified form @QmitkPointSetInteractionView Plug-in
 *
 * @ param IRenderwindow Part controls for interaction with the individual views and removes them to the Navigation
 * Controller
 *
 */
void SplineVisualisationPlugin::RenderWindowPartDeactivated(mitk::IRenderWindowPart *renderWindowPart)
{
  if (m_Controls.pointSplineSetListWidget != nullptr)
  {
    m_SplineVisualisationEvaluationUtil->InitClearProps(this->renderer);
    mitk::RenderingManager::GetInstance()->RemoveRenderWindow(this->renderer->GetRenderWindow());
    mitk::RenderingManager::GetInstance()->RequestUpdateAll();
    if (m_SplineVtkMapper != nullptr)
    {
     m_SplineVisualisationEvaluationUtil->ClearSplineVtkMapperProps(
      renderer, m_SplineVtkMapper);
    }
    if (m_TubeSurfaceVTKmapper != nullptr)
    {
      m_SplineVisualisationEvaluationUtil->ClearTubeVtkMapperProps(renderer, m_TubeSurfaceVTKmapper);
    }
      m_Controls.pointSplineSetListWidget->RemoveSliceNavigationController(
      renderWindowPart->GetQmitkRenderWindow("axial")->GetSliceNavigationController());
    m_Controls.pointSplineSetListWidget->RemoveSliceNavigationController(
      renderWindowPart->GetQmitkRenderWindow("sagittal")->GetSliceNavigationController());
    m_Controls.pointSplineSetListWidget->RemoveSliceNavigationController(
      renderWindowPart->GetQmitkRenderWindow("coronal")->GetSliceNavigationController());
    m_Controls.pointSplineSetListWidget->RemoveSliceNavigationController(
      renderWindowPart->GetQmitkRenderWindow("3d")->GetSliceNavigationController());
  }
}
