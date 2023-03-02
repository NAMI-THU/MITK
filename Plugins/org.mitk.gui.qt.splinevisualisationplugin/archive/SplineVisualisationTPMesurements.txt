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
#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>
#include <berryISelectionListener.h>

// Qmitk
#include "SplineVisualisationTPMeasurments.h"
#include "org_mitk_gui_qt_splinevisualisationplugin_Activator.h"
//#include <rpcnsi.h>
//#include <rpc.h>
#include <tchar.h>
#include <sys/types.h>
#include <errno.h>
#include <filesystem>
#include <vector>

// Qt
#include <QMessageBox>
#include <QTimer>
#include <QPushButton>
#include <QSettings>
#include <QFileDialog> 
#include <QWidget.h> 

// mitk image
#include <mitkImage.h>
// PointSet
#include <mitkPointSet.h>
#include <mitkProperties.h>

// 3d Spline Mapper
// #include <internal/SplineLiveInteractionEditor.h>

// inheriting class...
#include <mitkRenderingManager.h>
#include <mitkPointSetVtkMapper3D.h>
#include <vtkMitkRectangleProp.h>
#include <mitkGL.h>
#include <mitkVector.h>
#include <mitkIFileReader.h>
#include <mitkContourModel.h>
#include <mitkVirtualTrackingDevice.h>
#include <mitkNavigationData.h>
#include <mitkTrackingDeviceSource.h>
#include <mitkNavigationDataObjectVisualizationFilter.h>
#include <mitkPointSet.h>
#include <QmitkRenderWindow.h>
#include <QmitkSliceWidget.h>
#include <mitkPlanarFigure.h>
#include <mitkGeometry3D.h>
#include <mitkVector.h>

// standard imports
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream> 
#include <algorithm>
#include <list>
#include <fstream>
#include <new.h>
#include <dirent_win32.h>
#include <boost/timer/progress_display.hpp>
#include <ctkServiceEvent.h>

// MITK imports
#include <mitkAnnotation.h>
#include <mitkTrackingTool.h>
#include <mitkStandaloneDataStorage.h>
#include <mitkDataNode.h>
#include <mitkRenderWindow.h>
#include <mitkVtkAnnotation3D.h>
#include <mitkTextAnnotation3D.h>
#include <mitkTrackingVolumeGenerator.h>
#include <mitkManualPlacementAnnotationRenderer.h>
#include <mitkBoundingObjectGroup.h>
#include <QmitkAbstractView.h>
#include <mitkVtkMapper.h>
#include <mitkDispatcher.h>
#include <QInputDialog>
#include <mitkBaseRenderer.h>
#include <mitkDataInteractor.h>
#include <mitkVtkPropRenderer.h>
#include <mitkIRenderWindowPart.h>
#include <QmitkSliceNavigationListener.h>
#include <QInputDialog>
#include <mitkNodePredicateAnd.h>
#include <mitkNodePredicateDataType.h>
#include <mitkNodePredicateNot.h>
#include <mitkNodePredicateOr.h>
#include <mitkNodePredicateProperty.h>
#include <mitkRenderWindowBase.h>
#include <mitkGradientBackground.h>
#include <mitkVtkLoggingAdapter.h>
#include <mitkLog.h>
#include <mitkCylinder.h>
#include <mitkPlaneGeometryData.h>
#include <mitkClippingProperty.h>
#include <mitkVectorProperty.h>
#include <mitkNeedleProjectionFilter.h>
#include <mitkNavigationToolStorageDeserializer.h>
#include <mitkIOUtil.h>
#include <mitkIGTException.h>

//  VTK imports
#include <vtkParametricSpline.h>
#include <vtkTreeLayoutStrategy.h>
#include <vtkGraphLayout.h>
#include <vtkSplineFilter.h>
#include <vtkGraphAlgorithm.h>
#include <vtkInfovisCoreModule.h>
#include <vtkGraphHierarchicalBundleEdges.h>
#include <vtkRandomGraphSource.h>
#include <vtkGraphLayoutView.h>
#include <vtkSplineWidget.h>
#include <vtkKochanekSpline.h>            
#include <vtkVolume16Reader.h>
#include <vtkTestUtilities.h>
#include <vtkImagePlaneWidget.h>
#include <vtkOutlineFilter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSplineWidget2.h>
#include <vtkNew.h>
#include <vtkColor.h>
#include <vtkNamedColors.h>
#include <vtkRenderer.h>
#include <vtkWin32RenderWindowInteractor.h>
#include <vtkSplineRepresentation.h>
#include <vtkParametricFunctionSource.h>
#include <vtkGlyph3DMapper.h>
#include <vtkSphereSource.h>
#include <vtkMitkRenderProp.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkMitkRenderProp.h>
#include <vtkCleanPolyData.h>
#include <vtkIdFilter.h>
#include <vtkPolyLine.h>
#include <vtkDataReader.h>
#include <vtkInteractorStyleImage.h>
#include <vtkOpenGLPolyDataMapper.h>
#include <vtkPropAssembly.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkCellArray.h>
#include <vtkDepthSortPolyData.h>
#include <vtkCamera.h>
#include <vtkTubeFilter.h>
#include <vtkRibbonFilter.h>
#include <vtkLookupTable.h>
#include <vtkOpenGLHelper.h>
#include <vtkShaderProgram.h>
#include <vtkPlane.h>
#include <vtkFloatArray.h>
#include "vtkGarbageCollector.h"
#include <vtkTubeFilter.h>
#include "../../../org.mitk.gui.qt.igttracking/src/internal/QmitkMITKIGTTrackingToolboxViewWorker.h"
#include <internal/mitkSplineDataObjectVisualizationFilter.h>

using std::string;
using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;

const std::string SplineVisualisationTPMeasurments::VIEW_ID = "org.mitk.views.splinevisualisationtpmeasurments";

std::vector<double> SplineVisualisationTPMeasurments::TimeStamp_Tool0(82);
std::vector<double> SplineVisualisationTPMeasurments::Valid_Tool0(82);
std::vector<double> SplineVisualisationTPMeasurments::X_Tool0(82);
std::vector<double> SplineVisualisationTPMeasurments::Y_Tool0(82);
std::vector<double> SplineVisualisationTPMeasurments::Z_Tool0(82);
std::vector<double> SplineVisualisationTPMeasurments::QX_Tool0(82);
std::vector<double> SplineVisualisationTPMeasurments::QY_Tool0(82);
std::vector<double> SplineVisualisationTPMeasurments::QZ_Tool0(82);
std::vector<double> SplineVisualisationTPMeasurments::QR_Tool0(82);
std::vector<double> SplineVisualisationTPMeasurments::TimeStamp_Tool1(82);
std::vector<double> SplineVisualisationTPMeasurments::Valid_Tool1(82);
std::vector<double> SplineVisualisationTPMeasurments::X_Tool1(82);
std::vector<double> SplineVisualisationTPMeasurments::Y_Tool1(82);
std::vector<double> SplineVisualisationTPMeasurments::Z_Tool1(82);
std::vector<double> SplineVisualisationTPMeasurments::QX_Tool1(82);
std::vector<double> SplineVisualisationTPMeasurments::QY_Tool1(82);
std::vector<double> SplineVisualisationTPMeasurments::QZ_Tool1(82);
std::vector<double> SplineVisualisationTPMeasurments::QR_Tool1(82);
std::vector<double> SplineVisualisationTPMeasurments::TimeStamp_Tool2(82);
std::vector<double> SplineVisualisationTPMeasurments::Valid_Tool2(82);
std::vector<double> SplineVisualisationTPMeasurments::X_Tool2(82);
std::vector<double> SplineVisualisationTPMeasurments::Y_Tool2(82);
std::vector<double> SplineVisualisationTPMeasurments::Z_Tool2(82);
std::vector<double> SplineVisualisationTPMeasurments::QX_Tool2(82);
std::vector<double> SplineVisualisationTPMeasurments::QY_Tool2(82);
std::vector<double> SplineVisualisationTPMeasurments::QZ_Tool2(82);
std::vector<double> SplineVisualisationTPMeasurments::QR_Tool2(82);

long SplineVisualisationTPMeasurments::couter_coordinates = 0;
SplineVisualisationTPMeasurments::TrackVis_header TrackVis_header;
const char *SplineVisualisationTPMeasurments::FIBER_ID_ARRAY = "Fiber_IDs";

void SplineVisualisationTPMeasurments::SetFocus()
{
  m_Controls.ShowPointSet->setFocus();
}

SplineVisualisationTPMeasurments::SplineVisualisationTPMeasurments()

  : QmitkAbstractView(), 
    m_Controls(),
    m_DeviceTypeCollection(nullptr),
    m_ToolProjectionNode(nullptr),
    m_NumFibers(0)
{

  m_TrackVisHeader.hdr_size = 0;
  m_FiberWeights = vtkSmartPointer<vtkFloatArray>::New();
  m_FiberWeights->SetName("FIBER_WEIGHTS");
  m_FiberPolyData = vtkSmartPointer<vtkPolyData>::New();
  this->UpdateFiberGeometry();
  this->GenerateFiberIds();
  //this->ColorFibersByOrientation();
}

SplineVisualisationTPMeasurments::~SplineVisualisationTPMeasurments() {
// SplineDataObjectVisualizationFilter destructor
 //SplineDataObjectVisualizationFilter *splineDataObjectVisualisationFilter =  new mitk::SplineDataObjectVisualizationFilter();
// delete SplineDataObjectVisualizationFilter;
}

/*
*
* @brief StoreUISettings in MITK (mostly taken from QmitkMITKIGTTrackingToolboxView.cpp)
* 
* 
* * @return  void
*/
//! [StoreUISettings]
void SplineVisualisationTPMeasurments::StoreUISettings()
{
  // persistence service does not directly work in plugins for now
  // -> using QSettings
  QSettings settings;

  settings.beginGroup(QString::fromStdString(SplineVisualisationTPMeasurments::VIEW_ID));
  MITK_DEBUG << "Store UI settings";
  // set the values of some widgets and attrbutes to the QSettings
  settings.setValue("ShowTrackingVolume", QVariant(m_Controls.m_SpShowTrackingVolume->isChecked()));
  settings.setValue("ShowSplineConfigurationSelectionBox", QVariant(m_Controls.m_TrackingPointerSelectionBox->currentIndex()));
  settings.setValue("toolStorageFilename", QVariant(m_ToolStorageFilename));
  settings.setValue("VolumeSelectionBox", QVariant(m_Controls.m_SpVolumeSelectionBox->currentIndex()));
  settings.setValue("SimpleModeEnabled", QVariant(m_SimpleModeEnabled));
  settings.setValue("OpenIGTLinkDataFormat", QVariant(m_Controls.m_SpOpenIGTLinkDataFormat->currentIndex()));
  settings.setValue("EnableOpenIGTLinkMicroService", QVariant(m_Controls.m_SpEnableOpenIGTLinkMicroService->isChecked()));
  settings.endGroup();
}
//! [StoreUISettings]
void SplineVisualisationTPMeasurments::LoadUISettings()
{
  // persistence service does not directly work in plugins for now
  // -> using QSettings
  QSettings settings;
  settings.beginGroup(QString::fromStdString(SplineVisualisationTPMeasurments::VIEW_ID));
  // set some widgets and attributes by the values from the QSettings
  m_Controls.m_SpShowTrackingVolume->setChecked(settings.value("ShowTrackingVolume", true).toBool());
  m_Controls.m_SpVolumeSelectionBox->setCurrentIndex(settings.value("VolumeSelectionBox", 0).toInt());
  m_ToolStorageFilename = settings.value("toolStorageFilename", QVariant("")).toString();
  settings.endGroup();
}

  void SplineVisualisationTPMeasurments::CreateQtPartControl(QWidget *parent)
{
  
  m_Controls.setupUi(parent);

  // try to deserialize the tool storage from the given tool storage file name
  if (!m_ToolStorageFilename.isEmpty())
  {
    // try-catch block for exceptions
    try
    {
      this->GetGeometry();
      mitk::NavigationToolStorageDeserializer::Pointer m_NTSDeserializer =
          mitk::NavigationToolStorageDeserializer::New(this->GetDataStorage());
      m_toolStorage->UnRegisterMicroservice();
      m_toolStorage = m_NTSDeserializer->Deserialize(m_ToolStorageFilename.toStdString());
      m_toolStorage->RegisterAsMicroservice();
      // update label
      //qmitkMITKIGTTrackingToolboxView->UpdateToolStorageLabel(m_ToolStorageFilename);

    }
    catch (const mitk::IGTException& e)
    {
      MITK_WARN("QmitkMITKIGTTrackingToolBoxView") << "Error during restoring tools. Problems with file (" << m_ToolStorageFilename.toStdString() << "), please check the file?";
     // qmitkMITKIGTTrackingToolboxView->OnResetTools(); // qmitkMITKIGTTrackingToolboxView
                                                         // // if there where errors
                                                         // reset
                                                       // the tool storage to avoid problems
                                                       // later on
    }
  }
 /* Create the ComboBox elements list (here we use QString) */
  QList<QString> stringsMethodList;
  QList<QString> stringsEMSensorList;
  stringsMethodList.append("2D Spline");
  stringsMethodList.append("3D Tube");
  // stringsList.append("3d Tube + Third Eye Extention");
  stringsEMSensorList.append("1");
  stringsEMSensorList.append("2");
  stringsEMSensorList.append("3");
  stringsEMSensorList.append("4");
  stringsEMSensorList.append("5");
  stringsEMSensorList.append("6");
  stringsEMSensorList.append("7");
  stringsEMSensorList.append("8");
  /* Populate the comboBoxes */
  m_Controls.m_TrackingPointerSelectionBox->addItems(stringsMethodList);
  m_Controls.m_TrackingPointerSelectionBox->setCurrentIndex(0);
  m_Controls.VisMethodcomboBox->addItems(stringsMethodList);
  m_Controls.VisMethodcomboBox->setCurrentIndex(0);
  m_Controls.NumEMTrackSensorCombobox->addItems(stringsEMSensorList);
  m_Controls.NumEMTrackSensorCombobox->setCurrentIndex(2);

  us::ModuleContext *context = us::GetModuleContext();

   
         
  connect(m_Controls.m_SpConnectDisconnectButton,
          &QPushButton::clicked, //  SIGNAL(clicked()),
          this,
          [this]() { SLOT(QmitkMITKIGTTrackingToolboxView->OnConnectDisconnect());
      }
       );

  /* Search for current index in TrackingPointerSelectionBox*/
  int index = m_Controls.m_TrackingPointerSelectionBox->currentIndex();

  connect(m_Controls.m_TrackingPointerSelectionBox,
          SIGNAL(currentTextChanged(QString)),
          this,
          SLOT(comboboxItemChanged(QString)));

  if (index == 0)
  {
    connect(m_Controls.m_TrackingPointerSelectionBox,
            SIGNAL(currentTextChanged(QString)),
            this,
            SLOT(&SplineVisualisationTPMeasurments::StartReadPointSet()));
  }
  else
  {
    connect(m_Controls.m_TrackingPointerSelectionBox,
            SIGNAL(currentTextChanged(QString)),
            this,
            SLOT(&SplineVisualisationTPMeasurments::StartReadPointSet));
  }
  m_Controls.selectedPointSetWidget->SetDataStorage(this->GetDataStorage());
  m_Controls.selectedPointSetWidget->SetNodePredicate(mitk::NodePredicateAnd::New(
    mitk::TNodePredicateDataType<mitk::PointSet>::New(),
    mitk::NodePredicateNot::New(mitk::NodePredicateOr::New(mitk::NodePredicateProperty::New("helper object"),
                                                           mitk::NodePredicateProperty::New("hidden object")))));
  m_Controls.selectedPointSetWidget->SetSelectionIsOptional(true);
  m_Controls.selectedPointSetWidget->SetAutoSelectNewNodes(true);
  m_Controls.selectedPointSetWidget->SetEmptyInfo(QString("Please select a point set"));
  m_Controls.selectedPointSetWidget->SetPopUpTitel(QString("Select point set"));

  // for the csv data entry
  connect(m_Controls.ShowPointSet, &QPushButton::clicked, this, &SplineVisualisationTPMeasurments::StartReadPointSet);
  connect(m_Controls.StopPointSet, &QPushButton::clicked, this, &SplineVisualisationTPMeasurments::StopReadPointSet);
  // show contour lines for the spline generation
  connect(m_Controls.ShowSplinesButton,
          &QPushButton::clicked,
          this,
          [this]() { SplineVisualisationTPMeasurments::ResampleSpline(1.0); });

  // Example Spline Generator
  connect(m_Controls.ExampleSplinesGen,
          &QPushButton::clicked,
          this,
          [this]()
          {
            mitk::BaseRenderer::Pointer renderer = mitk::BaseRenderer::Pointer();
            int threeDindex = m_Controls.VisMethodcomboBox->currentIndex();
            //MITK_INFO << "!!!!Test Example treeDindex: " << threeDindex << std::endl;
            if (threeDindex == 0)
            {
              MainSplineGenerationFunction(renderer, false, false);
            }
            else
            {
              MainSplineGenerationFunction(renderer, false, true);
            }
            
          });
  connect(m_Controls.m_SpStartStopTrackingButton,
          &QPushButton::clicked,
          this,
          [this]()
          {
            mitk::BaseRenderer::Pointer renderer = mitk::BaseRenderer::Pointer();
            IGTMeasurementRLEMSensor3DSpline(renderer);
          });
  connect(m_Controls.m_SpStartTrackingSimpleMode, &QPushButton::clicked,
      this,
      [this]()
          {
    mitk::BaseRenderer::Pointer renderer = mitk::BaseRenderer::Pointer();
    IGTMeasurementRLEMSensor3DSpline(renderer);
          });


  mitk::VtkLoggingAdapter::Initialize();
  mitk::LoggingBackend *loggingbackend;
  // vtkOutputWindow
  mitk::VtkLoggingAdapter::GetInstance()->DisplayText("Working Logging Connection");
  m_Controls.textEdit->setEnabled(true);
  m_Controls.textEdit->setHtml((QString) "Working Logging Connection");
  m_Controls.textEdit->setHtml((QString) mitk::VtkLoggingAdapter::GetInstance()->GetUseStdErrorForAllMessages());
  m_Controls.textEdit->setHtml((QString) "Working Logging Connection");
  QTextCursor cursor = m_Controls.textEdit->textCursor();
  cursor.setPosition(QTextCursor::MoveOperation::StartOfLine, QTextCursor::MoveAnchor);
  cursor.setPosition(QTextCursor::MoveOperation::EndOfLine, QTextCursor::KeepAnchor);
  m_Controls.textEdit->setTextCursor(cursor);
  if (!(loggingbackend->GetLogFile().empty()))
  {
    m_Controls.textEdit->setHtml(QString::fromStdString(loggingbackend->GetLogFile()));
    // m_Controls.textEdit->setHtml(
    //  QString::fromStdString(loggingbackend->IncrementLogFileNames(loggingbackend->GetLogFile(), 10)));
  }
  
  auto renderWindowPart = this->GetRenderWindowPart();

  if (nullptr != renderWindowPart)
    this->RenderWindowPartActivated(renderWindowPart);

}

void SplineVisualisationTPMeasurments::OnSelectionChanged(berry::IWorkbenchPart::Pointer source,
                                                            const QList<mitk::DataNode::Pointer> &nodes)
{
  // iterate all selected objects, adjust warning visibility
  foreach (mitk::DataNode::Pointer node, nodes)
  {
    if (node.IsNotNull() && dynamic_cast<mitk::Image *>(node->GetData()))
    {
      m_Controls.labelWarning->setVisible(false);
      m_Controls.ShowPointSet->setEnabled(true);
      m_Controls.StopPointSet->setEnabled(true);
      m_Controls.ShowSplinesButton->setEnabled(true);

      return;
    }
  }

}

void SplineVisualisationTPMeasurments::StartReadingPoints()
{
  MITK_INFO << "Data was successfully read and loaded!" << std::endl;
  MITK_INFO << "Start QTimer..." << std::endl;

  mitk::DataNode::Pointer datanode = mitk::DataNode::Pointer();
  datanode->SetName("Tool_0");
  datanode->SetBoolProperty("show contour", true);
  datanode->SetFloatProperty("pointsize", 5);

  this->timer = new QTimer();
  // update position (sequential player)
  connect(this->timer, &QTimer::timeout, this, &SplineVisualisationTPMeasurments::StartReadPointSet);
  connect(this->timer, SIGNAL(timeout()), SLOT(&SplineVisualisationTPMeasurments::StartReadPointSet), Qt::QueuedConnection);
  this->timer->start(1000);
}

void SplineVisualisationTPMeasurments::StopReadPointSet()
{
  this->timer->stop();
  disconnect(this->timer, SIGNAL(timeout()), this, SLOT(&SplineVisualisationTPMeasurments::StartReadPointSet));
  timer = NULL;
  this->timer;
  MITK_INFO << "QMITK Timer was stopped" << std::endl;

}

vector<mitk::Vector3D> SplineVisualisationTPMeasurments::InputMitkVector3DConversion()
{
  vector<mitk::Vector3D> return_vector;
  mitk::Vector3D EM_vector0;
  mitk::Vector3D EM_vector1;
  mitk::Vector3D EM_vector2;

  for (int i = TimeStamp_Tool0.size(); i <= X_Tool0.size(); i++)
  {
    EM_vector0[0] = X_Tool0[i];
    EM_vector0[1] = Y_Tool0[i];
    EM_vector0[2] = Z_Tool0[i];

    EM_vector1[0] = X_Tool1[i];
    EM_vector1[1] = Y_Tool1[i];
    EM_vector1[2] = Z_Tool1[i];

    EM_vector2[0] = X_Tool2[i];
    EM_vector2[1] = Y_Tool2[i];
    EM_vector2[2] = Z_Tool2[i];

  }
  return return_vector;
}

vector<SplineVisualisationTPMeasurments::point> SplineVisualisationTPMeasurments::GetCSVImportData()
{
  unsigned int i = 0;
  vector<point> csvImportall = {};
  if (X_Tool0.size() != 0 || X_Tool1.size() != 0 || X_Tool2.size() != 0)
  {
    for (auto itx = std::begin(X_Tool0); (itx != std::end(X_Tool0)); itx++)
    {
      for (auto ity = std::begin(Y_Tool0); (ity != std::end(Y_Tool0)); ity++)
      {
        for (auto itz = std::begin(Z_Tool0); (itz != std::end(Z_Tool0)); itz++)
        {
          double r_itx = *itx - *((itx + 3 == std::end(X_Tool0)) ? std::begin(X_Tool0) : itx + 3);
          double r_ity = *ity - *((ity + 3 == std::end(Y_Tool0)) ? std::begin(Y_Tool0) : ity + 3);
          double r_itz = *itz - *((itz + 3 == std::end(Z_Tool0)) ? std::begin(Z_Tool0) : itz + 3);

            csvImportall.push_back(point());
            csvImportall[i].x = *itx;
            csvImportall[i].y = *ity;
            csvImportall[i].z = *itz;
            i++;
        }
      }
    }
    return csvImportall;
  }
  else
  {
    csvImportall.push_back(point());
    csvImportall[i].x = 0;
    csvImportall[i].y = 0;
    csvImportall[i].z = 0;
    return csvImportall;
  }
}



SplineVisualisationTPMeasurments::LocalStorage3D::LocalStorage3D()
{
  m_FiberActor = vtkSmartPointer<vtkActor>::New();
  m_FiberMapper = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
  m_FiberAssembly = vtkSmartPointer<vtkPropAssembly>::New();
}

SplineVisualisationTPMeasurments::FiberBundle::FiberBundle(vtkPolyData *fiberPolyData) 
{
  m_TrackVisHeader.hdr_size = 0;
  m_FiberWeights = vtkSmartPointer<vtkFloatArray>::New();
  m_FiberWeights->SetName("FIBER_WEIGHTS");

  m_FiberPolyData = vtkSmartPointer<vtkPolyData>::New();
  if (fiberPolyData != nullptr)
    m_FiberPolyData = fiberPolyData;
  else
  {
    this->m_FiberPolyData->SetPoints(vtkSmartPointer<vtkPoints>::New());
    this->m_FiberPolyData->SetLines(vtkSmartPointer<vtkCellArray>::New());
  }
}

void SplineVisualisationTPMeasurments::OnTrackingVolumeChanged(QString qstr)
{
  if (qstr.isNull())
    return;
  if (qstr.isEmpty())
    return;

  mitk::TrackingVolumeGenerator::Pointer volumeGenerator = mitk::TrackingVolumeGenerator::New();

  std::string str = qstr.toStdString();

  mitk::TrackingDeviceData data = m_DeviceTypeCollection->GetDeviceDataByName(str);
  m_TrackingDeviceData = data;

  volumeGenerator->SetTrackingDeviceData(data);
  volumeGenerator->Update();

  mitk::Surface::Pointer volumeSurface = volumeGenerator->GetOutput();

  m_TrackingVolumeNode->SetData(volumeSurface);

  if (!m_Controls.m_SpShowTrackingVolume->isChecked())
    m_TrackingVolumeNode->SetOpacity(0.0);
  else
    m_TrackingVolumeNode->SetOpacity(0.25);

  GlobalReinit();
}

void SplineVisualisationTPMeasurments::IGTMeasurementRLEMSensor3DSpline(mitk::BaseRenderer *renderer) {
  m_toolStorage = mitk::NavigationToolStorage::New(GetDataStorage());
  m_toolStorage->SetName("TrackingToolbox Default Storage");
  m_toolStorage->RegisterAsMicroservice();
  mitk::StandaloneDataStorage::Pointer dataStorage = mitk::StandaloneDataStorage::New();

  if (!m_connected)
  {
    MITK_WARN << "Can't start tracking if no device is connected. Aborting";
    return;
  }
  if (m_tracking)
  {
    if (!m_tracking)
      return;
    for (unsigned int i = 0; i < m_ToolVisualizationFilter->GetNumberOfIndexedOutputs(); i++)
    {
      mitk::NavigationData::Pointer currentTool = m_ToolVisualizationFilter->GetOutput(i);
      if (currentTool->IsDataValid())
      {
        this->m_toolStorage->GetTool(i)->GetDataNode()->SetColor(mitk::IGTColor_INVALID);
       // this->dataStorage->ComputeBoundingGeometry3D();
      }
    }

    // refresh view and status widget
    mitk::RenderingManager::GetInstance()->RequestUpdateAll();
    // stop tracking
    m_TrackingRenderTimer->stop();
    m_TrackingLoggingTimer->stop();

    //m_Worker->SetWorkerMethod(m_IGTViewWorker->eStopTracking);
    m_WorkerThread->start();
    m_Controls.IGTMeasurementRT->setEnabled(false);
  }
  else
  {
    // show tracking volume start tracking
    this->OnTrackingVolumeChanged(m_Controls.m_SpVolumeSelectionBox->currentText());
    // Reset the view to a defined start. Do it here and not in OnStartTrackingFinished, to give other tracking devices
    // the chance to reset the view to a different direction.
    this->GlobalReinit();
    //m_Worker->SetWorkerMethod(m_IGTViewWorker->eStartTracking);
    m_WorkerThread->start();
    this->m_Controls.IGTMeasurementRT->setEnabled(false);
  }

}

void SplineVisualisationTPMeasurments::GlobalReinit()
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


void SplineVisualisationTPMeasurments::TimerEvent(QTimerEvent *event)
{
  qDebug() << "Update...";
}

void SplineVisualisationTPMeasurments::UpdateTimer(bool stop, QWidget *parent) 
{
  MITK_INFO << "Start Update QTimer..." << std::endl;
  this->m_TrackingRenderTimer = new QTimer();
  QTimerEvent *timeEvent = new QTimerEvent(20);
  int fps = 60; // or 30
  // update position (sequential player)
  this->connect(this->m_TrackingRenderTimer,
                &QTimer::timeout,
                this,
                [&parent]()
                { 
                SplineDataObjectVisualizationFilter *m_splineDataObjectVisualisationfilter =
                    new SplineDataObjectVisualizationFilter();
                  m_splineDataObjectVisualisationfilter->InputTrackingData(parent);
                });
  this->m_TrackingRenderTimer->start(1000);
  this->m_TrackingRenderTimer->setInterval(1000 / fps); // ms
  this->connect(
    this->m_TrackingRenderTimer, SIGNAL(timeout()), this, SLOT(&splineTPMeasurements->updateStandart3dView()));
  this->TimerEvent(timeEvent);
  if (stop)
  {
    this->m_TrackingRenderTimer->stop();
    this->disconnect(
      this->m_TrackingRenderTimer, SIGNAL(timeout()), this, SLOT(&splineTPMeasurements->InputTrackingData));
    m_TrackingRenderTimer = NULL;
    this->m_TrackingRenderTimer;
    MITK_INFO << "QMITK Timer was stopped" << std::endl;
  }
}




void SplineVisualisationTPMeasurments::MainSplineGenerationFunction(mitk::BaseRenderer *renderer,
                                                                    bool csvImport,
                                                                    bool threeDVis)
{
  unsigned int i = 0;
  mitk::PointSet::Pointer splinePointSet = mitk::PointSet::New();
  vtkNew<vtkPoints> vtkSpPoints;
  vtkNew<vtkCellArray> vcells;
  vtkNew<vtkPolyData> polyData;
  vector<point> selectedDataset;
  string windowName;
  //make points
  vector<point> coordinates = {
    {10.90, 10.43, 8.2},
    {12.0, 15.0, 12.0},
    {23.0, 5.0, 9.0}
  };

  vector<point> coor_abitrary = {{57.8, 98, -214},
                                  {52.6, 156.8, -214},
                                  {28.3, 185.4, -214},
                                  {-8.05, 163.9, -214.4},
                                  {-43.3, 117.26, -214.4},
                                  {-84.29, 83.79, -214},
                                  {-129.47, 103.6, -214}};

  vector<point> coor_abitrary_2  = {{6, 16, 1},
                                    { 11, 26, 6 },
                                    { 21, 21, 11 },
                                    { 15, -5, 15 },
                                    { 5.5, 10, 20 },
                                    { 12.5, 15, 25 },
                                    { 15, 0, 30 },
                                    { 5, -10, 35 },
                                    { 10, 15, 40 },
                                    { 5, 15, 30 }};

  vector<point> coor_duodenum =  {{99.7, -103.3, -214}, 
                                           {80.1, -59.1, -214},
                                           {20.13, -160.84, -214},
                                           {5.95, 12.64, -214.4},
                                           {9.45, 84.26, -214.4},
                                           {65.29, 103.79, -214},
                                           {137.47, 93.4, -214}};

  vector<point> coor_esophagus = { {100.4, 146.2, -214},
                                           {106.6, 90.8, -214},
                                           {69.3, 60.4, -214},
                                           {69.01, 60.4, -213.4},
                                           {18.3, 44.26, -214.4},
                                           {50.09, 12.9, -214},
                                           {123.47, 16.6, -214.3},
                                           {156, -53, -213.4}};

 vector<point> csvImportsample = {{X_Tool0[1], Y_Tool0[1], Z_Tool0[1]},
                                   {X_Tool1[1], Y_Tool1[1], Z_Tool1[1]},
                                   {X_Tool2[1], Y_Tool2[1], Z_Tool2[1]},
                                   {X_Tool0[15], Y_Tool0[15], Z_Tool0[15]},
                                   {X_Tool1[15], Y_Tool1[15], Z_Tool1[15]},
                                   {X_Tool2[15], Y_Tool2[15], Z_Tool2[15]},
                                   {X_Tool0[30], Y_Tool0[30], Z_Tool0[30]},
                                   {X_Tool1[30], Y_Tool1[30], Z_Tool1[30]},
                                   {X_Tool2[30], Y_Tool2[30], Z_Tool2[30]},
                                   {X_Tool0[45], Y_Tool0[45], Z_Tool0[45]},
                                   {X_Tool1[45], Y_Tool1[45], Z_Tool1[45]},
                                   {X_Tool2[45], Y_Tool2[45], Z_Tool2[45]}};
  
 if (csvImport)
  {
    selectedDataset = csvImportsample;
    windowName = "CSV import sample Spline ";

  }
  else
  {
    vector<vector<point>> allDatasets = {coordinates, coor_abitrary, coor_abitrary_2, coor_duodenum, coor_esophagus};
    QStringList qAllDatasetsStringList;
    qAllDatasetsStringList.append("arbitrary coordinates");
    qAllDatasetsStringList.append("arbitrary coordinates #2");
    qAllDatasetsStringList.append("arbitrary coordinates #3");
    qAllDatasetsStringList.append("duodenum coordinates");
    qAllDatasetsStringList.append("esophagus + stomach coordinates");

    bool ok = false;
    QString datasetSelectionText = QInputDialog::getItem(QApplication::activeWindow(),
                                                         tr("Select Dataset..."),
                                                         tr("Enter name for the dataset set"),
                                                         qAllDatasetsStringList,
                                                         0 & ok);
    unsigned int index;
    if (datasetSelectionText.toStdString() == "arbitrary coordinates")
      index = 0;
    if (datasetSelectionText.toStdString() == "arbitrary coordinates #2")
      index = 1;
    if (datasetSelectionText.toStdString() == "arbitrary coordinates #3")
      index = 2;
    if (datasetSelectionText.toStdString() == "duodenum coordinates")
      index = 3;
    if (datasetSelectionText.toStdString() == "esophagus + stomach coordinates")
      index = 4;
    windowName = datasetSelectionText.toStdString() + " Spline";

    // MITK_INFO << "LOOK HERE: " << allDatasets[index] << std::endl;
    for (auto it = allDatasets.begin(); it != allDatasets.end(); it++)
    {
      int dIndex = std::distance(allDatasets.begin(), it);
      if (dIndex == index)
      {
        selectedDataset = *it;
      }
    } 
  }

  for (auto sPoint : selectedDataset)
  {
    MITK_INFO << "Vtk Spline Pointset " << sPoint.x << " " << sPoint.y << " " << sPoint.z << " "
              << ";"
              << "\n"
              << std::endl;
    // make pointset and aquire 3d Point
    vtkSpPoints->InsertNextPoint(sPoint.x, sPoint.y, sPoint.z);
    splinePointSet->InsertPoint(sPoint.x, sPoint.y, sPoint.z);
    vcells->InsertNextCell(1);
    vcells->InsertCellPoint(i);
    i++;
  }
  // ask for the name of the point set
  bool ok = false;
  QString pointset_text = QInputDialog::getText(QApplication::activeWindow(),
                                       tr("Add point set..."),
                                       tr("Enter name for the new point set"),
                                       QLineEdit::Normal,
                                       tr("Spline PointSet"),
                                       &ok);
  if (!ok || pointset_text.isEmpty())
  {
    return;
  }
  vtkSmartPointer<vtkActor> m_FiberActor = vtkSmartPointer<vtkActor>::New();
  vtkSmartPointer<vtkOpenGLPolyDataMapper> m_FiberMapper = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
  vtkSmartPointer<vtkPropAssembly> m_FiberAssembly = vtkSmartPointer<vtkPropAssembly>::New();
  vtkSmartPointer<vtkPolyData> m_FiberPolyData = vtkSmartPointer<vtkPolyData>::New();
    
  ResampleSpline(2.5, 1.3, 1.5, 0.3);

  // standalone vtk window
  vtkNew<vtkNamedColors> colors;
  // A renderer and render window
  // #1
  // vtkNew<vtkRenderer> vtkrenderer;
  vtkRenderer *vtkrenderer = vtkRenderer::New();
  // An interactor
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  // parametric spline class
  vtkNew<vtkParametricSpline> parametricspline;
  // parametricspline->SetNumberOfPoints(8);
  parametricspline->SetPoints(vtkSpPoints);
  // the spline representation widget and the parametric function
  vtkNew<vtkSplineWidget2> splineWidget;
  vtkNew<vtkSplineRepresentation> splineRepresentation;
  splineRepresentation->SetParametricSpline(parametricspline);
  splineWidget->SetRepresentation(splineRepresentation);
  splineWidget->SetInteractor(renderWindowInteractor);
  // Parametric function for spline
  vtkNew<vtkParametricFunctionSource> functionSource;
  functionSource->SetParametricFunction(parametricspline);
  functionSource->Update();
  vtkNew<vtkPolyDataMapper> mapper;
  vtkNew<vtkTubeFilter> tubeFilter;
  vtkNew<vtkActor> tubeActor;
  if (threeDVis)
  {
    // Create a tube (cylinder) around the line
    tubeFilter->SetInputConnection(functionSource->GetOutputPort());
    tubeActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
    tubeActor->SetMapper(mapper);
    tubeFilter->SetRadius(.75); // default is .5
    tubeFilter->SetNumberOfSides(50);
    tubeFilter->Update();
    // Setup actor and mapper
    mapper->SetInputConnection(tubeFilter->GetOutputPort());
    tubeActor->GetProperty()->SetOpacity(0.75); // Make the tube have some transparency.
    tubeActor->SetMapper(mapper);
  }
  else
  {
    mapper->SetInputConnection(functionSource->GetOutputPort());
  }
  // Read the data 
  vtkDataReader *vtkDataReader = vtkDataReader::New();
  vtkDataReader->SetInputConnection(functionSource->GetOutputPort());
  // vtkDataReader->SetFileName(functionSource);
  // Actor
  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(colors->GetColor3d("DarkSlateGrey").GetData());
  actor->GetProperty()->SetRepresentationToWireframe();
  actor->GetProperty()->SetLineWidth(3.0);
  actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
  actor->GetMapper()->SetInputConnection(vtkDataReader->GetOutputPort());
  // Create a polydata to store everything in and update
  polyData->SetPoints(vtkSpPoints);
  polyData->UpdatePointGhostArrayCache();
  polyData->SetVerts(vcells);
  vtkNew<vtkPolyDataMapper> polydatamapper;
  polydatamapper->SetInputConnection(functionSource->GetOutputPort());
  polydatamapper->SetInputData(polyData);
  // Setup Anchor Point sphere
  vtkNew<vtkSphereSource> sphereAnchorPoint;
  sphereAnchorPoint->SetPhiResolution(21);
  sphereAnchorPoint->SetThetaResolution(21);
  sphereAnchorPoint->SetRadius(0.1);
  sphereAnchorPoint->GetInput();
  sphereAnchorPoint->SetInputConnection(functionSource->GetOutputPort());
  // get the standard 3D renderer
  renderer->mitk::BaseRenderer::GetInstance(GetRenderWindowPart()->GetQmitkRenderWindow("3d")->GetRenderWindow());
  // #2
  vtkMitkRenderProp *vtkmitkrenderprop = vtkMitkRenderProp::New();
  vtkmitkrenderprop->SetVisibility(TRUE);
  // #3 
  mitk::VtkPropRenderer::Pointer proprenderer;
  proprenderer->mitk::VtkPropRenderer::GetInstance(GetRenderWindowPart()->GetQmitkRenderWindow("3d")->GetRenderWindow());
  if (proprenderer)
  {
    proprenderer->GetInstance(GetRenderWindowPart()->GetQmitkRenderWindow("3d")->GetRenderWindow());
    proprenderer->GetVtkRenderer();
    proprenderer->PrepareRender(); // "proprenderer.m_Pointer" war "nullptr".
    proprenderer->MakeCurrent();
  }
  // now create a new QmitkRenderWindow with this renderingManager as parameter
  vtkNew<vtkGlyph3DMapper> pointMapper;
  pointMapper->SetInputData(polyData);
  pointMapper->SetSourceConnection(sphereAnchorPoint->GetOutputPort());
  pointMapper->SetInputConnection(functionSource->GetOutputPort());
  // insert into 3D View Qmitk RenderWindow
  // make DataNode and PointSet as Data insertion
  mitk::DataNode::Pointer splineDataNode = mitk::DataNode::New();
  splineDataNode->SetName("Spline Example");
  splineDataNode->SetData(splinePointSet);
  splineDataNode->SetProperty("name", mitk::StringProperty::New(pointset_text.toStdString()));
  splineDataNode->SetProperty("opacity", mitk::FloatProperty::New(1));
  splineDataNode->SetBoolProperty("show contour", mitk::BoolProperty::New(true));
  splineDataNode->SetProperty("contoursize", mitk::FloatProperty::New(0.9));
  splineDataNode->SetProperty("pointsize", mitk::FloatProperty::New(1.15));
  splineDataNode->SetColor(1.0, 1.0, 0.0);
  splineDataNode->SetVisibility(true);
  this->GetDataStorage()->Add(splineDataNode);
  // vtkActor
  vtkNew<vtkActor> pointActor;
  pointActor->SetMapper(pointMapper);
  pointActor->GetProperty()->SetColor(colors->GetColor3d("Peacock").GetData());
  // Setup render window, renderer, and interactor
  vtkNew<vtkRenderWindow> vtkrenderWindow;
  vtkrenderWindow->AddRenderer(vtkrenderer);
  vtkrenderWindow->SetWindowName(windowName.c_str());
  // vtkRenderWindow Interactor
  renderWindowInteractor->SetRenderWindow(vtkrenderWindow);
  if (threeDVis)
  {
    vtkrenderer->AddActor(tubeActor);
    vtkrenderer->AddActor(actor);
    vtkrenderer->AddActor(pointActor);
    vtkrenderer->SetBackground(colors->GetColor3d("Black").GetData());
  }
  else
  {
    vtkrenderer->AddActor(actor);
    vtkrenderer->AddActor(pointActor);
    vtkrenderer->SetBackground(colors->GetColor3d("Black").GetData());
  }
  // DataNode add to the Datastorage
  mitk::StandaloneDataStorage::Pointer dataStorage = mitk::StandaloneDataStorage::New(); 
  dataStorage->GetAll();
  dataStorage->Add(splineDataNode);
  // proprenderer->SetDataStorage(dataStorage);
  // A renderer and render window
  QmitkRenderWindow *qmitkrenderWindow =
    new QmitkRenderWindow(this->GetRenderWindowPart()->GetQmitkRenderWindow("3d"), "3d", proprenderer);
  // create a new instance of mitk::RenderingManager
  mitk::RenderingManager::Pointer renderingManager = mitk::RenderingManager::New();
  // add both to the RenderingManager
  renderingManager->SetDataStorage(dataStorage);
  // create new instances of DataStorage and GlobalInteraction
  mitk::DataStorageEditorInput globalInteraction = mitk::DataStorageEditorInput();
  // A interactor pipeline
  vtkSmartPointer<mitk::Dispatcher> dispatcher;
  vtkSmartPointer<mitk::DataInteractor> datainteractor;
  // #3.1
  vtkmitkrenderprop->SetPropRenderer(proprenderer);
  vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
  renderWindowInteractor->SetInteractorStyle(style);
  // Render and start interaction
  // now create a new QmitkRenderWindow with this renderingManager as parameter
  // retrieve a possibly existing IRenderWindowPart
  // #4
  mitk::Mapper *mitkmapper;
  mitkmapper->New();
  //mitkmapper->SetDataNode(splineDataNode);
  // #5
  mitk::VtkMapper *mitkVtkMapper = mitk::VtkMapper::Pointer();
  mitkVtkMapper->New();
  // mitkVtkMapper->SetDataNode(splineDataNode);
  mitk::IRenderWindowPart *renderWindowPart = this->GetRenderWindowPart();
  // let the point set widget know about the render window part (crosshair updates)
  this->RenderWindowPartActivated(renderWindowPart);
  RenderWindowPartActivated(renderWindowPart);
  renderWindowPart->GetQmitkRenderWindow("3d")->GetSliceNavigationController();
  renderWindowPart->GetQmitkRenderWindow("3d")->setAcceptDrops(true);
  splineWidget->SetInteractor(renderWindowInteractor);
  // vtk Render and Window interactor
  vtkrenderWindow->SetSize(800, 800);
  vtkrenderWindow->AddRenderer(vtkrenderer);
  // Render the scene
  vtkrenderWindow->Render();
  renderWindowInteractor->SetRenderWindow(vtkrenderWindow);
  m_Controls.selectedPointSetWidget->SetCurrentSelectedNode(splineDataNode);
  // proprenderer->Render(static_cast<mitk::VtkPropRenderer::RenderType>(2));
  // Create toplevel widget with vertical layout
  // renderWindow = new QmitkRenderWindow(renderWindowPart->GetQmitkRenderWindow("3d"), "3d", proprenderer);
  // renderWindow->InitRenderer();
  // Create a new planar figure composite
  mitk::Geometry3D::Pointer splineGeometry = mitk::Geometry3D::New();
  // Create a spline planar figure and add it to the composite
  // mitk::PlanarFigure::Pointer splineFigure = mitk::PlanarFigure::Pointer();
  // initialize(splineFigure);
  // splineFigure->SetProperty("line width", mitk::FloatProperty::New(2.0));                     // Set the line width
  // splineFigure->SetProperty("planarfigure.drawcontrolpoints", mitk::BoolProperty::New(true)); // Show control points
  // splineFigure->SetProperty("planarfigure.drawname", mitk::BoolProperty::New(false));         // Hide the name
  // splineFigure->SetProperty("planarfigure.drawmeasurements", mitk::BoolProperty::New(false)); // Hide the
  // measurements splineFigure->SetProperty("planarfigure.closed", mitk::BoolProperty::New(false));           // Open
  // spline splineGeometry->SetImageGeometry(splineFigure); actor->SetMapper(splineGeometry); dataStorage->Add(actor);
  // set up the data storage for the renderer
  qmitkrenderWindow->GetRenderer()->SetDataStorage(dataStorage);
  // Use it as a 3D view
  qmitkrenderWindow->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);
  qmitkrenderWindow->GetRenderWindow()->SetSize(800, 800);
  // Reposition the camera to include all visible actors
  qmitkrenderWindow->GetRenderer()->GetVtkRenderer()->ResetCamera();
  //renderWindowPart->GetQmitkRenderWindow("3d")->GetRenderer()->Render(
  //  static_cast<mitk::VtkPropRenderer::RenderType>(2));
  RenderWindowPartActivated(renderWindowPart);
  // Connect datastorage and renderwindow
  qmitkrenderWindow->GetVtkRenderWindow()->Render();
  splineWidget->On();
  // globalInteraction->SetRenderWindow(renderWindow);
  renderWindowInteractor->Initialize();
  renderWindowInteractor->Start();
  renderingManager->AddRenderWindow(qmitkrenderWindow->GetRenderWindow());
  mitk::RenderingManager::GetInstance()->RequestUpdate(
    this->GetRenderWindowPart()->GetQmitkRenderWindow("3d")->GetVtkRenderWindow());
  mitk::RenderingManager::GetInstance()->RequestUpdateAll(); 
  mitk::RenderingManager::GetInstance()->ForceImmediateUpdateAll();

  // Calculate time step of the input data for the specified renderer (integer value)
  // this method is implemented in mitkMapper
  //mitkmapper->CalculateTimeStep(renderer);
  //mitkmapper->InternalGenerateData(renderer);

  vtkSmartPointer<vtkPoints> vtkNewPoints = vtkSmartPointer<vtkPoints>::New();
  vtkNewPoints = vtkSpPoints;
  vtkSmartPointer<vtkCellArray> vtkNewCells = vtkSmartPointer<vtkCellArray>::New();
  vtkNewCells = vcells;
  float error = 0.1;
  MITK_INFO << "Compressing fibers with max. error " << error << "mm";
  unsigned int numRemovedPoints = 0;
  boost::timer::progress_display disp(static_cast<unsigned long>(m_FiberPolyData->GetNumberOfCells()));
  vtkSmartPointer<vtkFloatArray> newFiberWeights = vtkSmartPointer<vtkFloatArray>::New();
  newFiberWeights->SetName("FIBER_WEIGHTS");
  newFiberWeights->SetNumberOfValues(m_NumFibers);

#pragma omp parallel for
  for (int i = 0; i < static_cast<int>(m_FiberPolyData->GetNumberOfCells()); i++)
  {
    std::vector<vnl_vector_fixed<double, 3>> vertices;
    float weight = 1;

#pragma omp critical
    {
      ++disp;
      weight = m_FiberWeights->GetValue(i);
      vtkCell *cell = m_FiberPolyData->GetCell(i);
      auto numPoints = cell->GetNumberOfPoints();
      vtkPoints *points = cell->GetPoints();

      for (int j = 0; j < numPoints; j++)
      {
        double cand[3];
        points->GetPoint(j, cand);
        vnl_vector_fixed<double, 3> candV;
        candV[0] = cand[0];
        candV[1] = cand[1];
        candV[2] = cand[2];
        vertices.push_back(candV);
      }
    }

    // calculate curvatures
    auto numPoints = vertices.size();
    std::vector<int> removedPoints;
    removedPoints.resize(numPoints, 0);
    removedPoints[0] = -1;
    removedPoints[numPoints - 1] = -1;

    vtkSmartPointer<vtkPolyLine> container = vtkSmartPointer<vtkPolyLine>::New();
    unsigned int remCounter = 0;

    bool pointFound = true;
    while (pointFound)
    {
      pointFound = false;
      double minError = static_cast<double>(error);
      unsigned int removeIndex = 0;

      for (unsigned int j = 0; j < vertices.size(); j++)
      {
        if (removedPoints[j] == 0)
        {
          vnl_vector_fixed<double, 3> candV = vertices.at(j);

          int validP = -1;
          vnl_vector_fixed<double, 3> pred;
          for (int k = static_cast<int>(j) - 1; k >= 0; k--)
            if (removedPoints[static_cast<unsigned int>(k)] <= 0)
            {
              pred = vertices.at(static_cast<unsigned int>(k));
              validP = k;
              break;
            }
          int validS = -1;
          vnl_vector_fixed<double, 3> succ;
          for (unsigned int k = j + 1; k < numPoints; k++)
            if (removedPoints[k] <= 0)
            {
              succ = vertices.at(k);
              validS = static_cast<int>(k);
              break;
            }

          if (validP >= 0 && validS >= 0)
          {
            double a = (candV - pred).magnitude();
            double b = (candV - succ).magnitude();
            double c = (pred - succ).magnitude();
            double s = 0.5 * (a + b + c);
            double hc = (2.0 / c) * sqrt(fabs(s * (s - a) * (s - b) * (s - c)));

            if (hc < minError)
            {
              removeIndex = j;
              minError = hc;
              pointFound = true;
            }
          }
        }
      }

      if (pointFound)
      {
        removedPoints[removeIndex] = 1;
        remCounter++;
      }
    }

    for (unsigned int j = 0; j < numPoints; j++)
    {
      if (removedPoints[j] <= 0)
      {
#pragma omp critical
        {
          vtkIdType id = vtkNewPoints->InsertNextPoint(vertices.at(j).data_block());
          container->GetPointIds()->InsertNextId(id);
        }
      }
    }

#pragma omp critical
    {
      newFiberWeights->SetValue(vtkNewCells->GetNumberOfCells(), weight);
      numRemovedPoints += remCounter;
      vtkNewCells->InsertNextCell(container);
    }
  }

  if (vtkNewCells->GetNumberOfCells() > 0)
  {
    MITK_INFO << "Removed points: " << numRemovedPoints;
    SetFiberWeights(newFiberWeights);
    m_FiberPolyData = vtkSmartPointer<vtkPolyData>::New();
    m_FiberPolyData->SetPoints(vtkNewPoints);
    m_FiberPolyData->SetLines(vtkNewCells);
    this->SetFiberPolyData(m_FiberPolyData, true);
  }
}


void SplineVisualisationTPMeasurments::ResampleSpline(float pointDistance,
                                                      double tension,
                                                      double continuity,
                                                      double bias)
{
  if (pointDistance <= 0)
    return;

  vtkSmartPointer<vtkPoints> vtkSmoothPoints =
    vtkSmartPointer<vtkPoints>::New(); // in smoothpoints the interpolated points representing a fiber are stored.

  // in vtkcells all polylines are stored, actually all id's of them are stored
  vtkSmartPointer<vtkCellArray> vtkSmoothCells =
    vtkSmartPointer<vtkCellArray>::New(); // cellcontainer for smoothed lines

  MITK_INFO << "Smoothing fibers";
  vtkSmartPointer<vtkFloatArray> newFiberWeights = vtkSmartPointer<vtkFloatArray>::New();
  newFiberWeights->SetName("FIBER_WEIGHTS");
  newFiberWeights->SetNumberOfValues(m_NumFibers);

  std::vector<vtkSmartPointer<vtkPolyLine>> resampled_streamlines;
  resampled_streamlines.resize(m_NumFibers);

  boost::timer::progress_display disp(m_NumFibers);
#pragma omp parallel for
  for (int i = 0; i < static_cast<int>(m_NumFibers); i++)
  {
    vtkSmartPointer<vtkPoints> newPoints = vtkSmartPointer<vtkPoints>::New();
    float length = 0;
#pragma omp critical
    {
      length = m_FiberLengths.at(static_cast<unsigned int>(i));
      ++disp;
      vtkCell *cell = m_FiberPolyData->GetCell(i);
      auto numPoints = cell->GetNumberOfPoints();
      vtkPoints *points = cell->GetPoints();
      for (int j = 0; j < numPoints; j++)
        newPoints->InsertNextPoint(points->GetPoint(j));
    }

    int sampling = static_cast<int>(std::ceil(length / pointDistance));

    vtkSmartPointer<vtkKochanekSpline> xSpline = vtkSmartPointer<vtkKochanekSpline>::New();
    vtkSmartPointer<vtkKochanekSpline> ySpline = vtkSmartPointer<vtkKochanekSpline>::New();
    vtkSmartPointer<vtkKochanekSpline> zSpline = vtkSmartPointer<vtkKochanekSpline>::New();
    xSpline->SetDefaultBias(bias);
    xSpline->SetDefaultTension(tension);
    xSpline->SetDefaultContinuity(continuity);
    ySpline->SetDefaultBias(bias);
    ySpline->SetDefaultTension(tension);
    ySpline->SetDefaultContinuity(continuity);
    zSpline->SetDefaultBias(bias);
    zSpline->SetDefaultTension(tension);
    zSpline->SetDefaultContinuity(continuity);

    vtkSmartPointer<vtkParametricSpline> spline = vtkSmartPointer<vtkParametricSpline>::New();
    spline->SetXSpline(xSpline);
    spline->SetYSpline(ySpline);
    spline->SetZSpline(zSpline);
    spline->SetPoints(newPoints);

    vtkSmartPointer<vtkParametricFunctionSource> functionSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
    functionSource->SetParametricFunction(spline);
    functionSource->SetUResolution(sampling);
    functionSource->SetVResolution(sampling);
    functionSource->SetWResolution(sampling);
    functionSource->Update();

    vtkPolyData *outputFunction = functionSource->GetOutput();
    vtkPoints *tmpSmoothPnts = outputFunction->GetPoints(); // smoothPoints of current fiber
    vtkSmartPointer<vtkPolyLine> smoothLine = vtkSmartPointer<vtkPolyLine>::New();
    vtkIdList *vtklistids = vtkIdList::New();

#pragma omp critical
    {
      for (int j = 0; j < tmpSmoothPnts->GetNumberOfPoints(); j++)
      {
        vtkIdType id = vtkSmoothPoints->InsertNextPoint(tmpSmoothPnts->GetPoint(j));
        smoothLine->GetPointIds()->InsertNextId(id);

      }
      resampled_streamlines[static_cast<unsigned long>(i)] = smoothLine;
    }
  }

  for (auto container : resampled_streamlines)
  {

    vtkSmoothCells->InsertNextCell(container);
  }

  m_FiberPolyData = vtkSmartPointer<vtkPolyData>::New();
  m_FiberPolyData->SetPoints(vtkSmoothPoints);
  m_FiberPolyData->SetLines(vtkSmoothCells);
  this->SetFiberPolyData(m_FiberPolyData, true);
}

void SplineVisualisationTPMeasurments::ResampleSpline(float pointDistance)
{
  ResampleSpline(pointDistance, 0, 0, 0);
}
	
/*
 * set PolyData (additional flag to recompute fiber geometry, default = true)
 */
void SplineVisualisationTPMeasurments::SetFiberPolyData(vtkSmartPointer<vtkPolyData> fiberPD, bool updateGeometry)
{
  if (fiberPD == nullptr)
    this->m_FiberPolyData = vtkSmartPointer<vtkPolyData>::New();
  else
  {
    m_FiberPolyData->CopyStructure(fiberPD);
    //    m_FiberPolyData->DeepCopy(fiberPD);
  }

  m_NumFibers = static_cast<unsigned int>(m_FiberPolyData->GetNumberOfLines());

  if (updateGeometry)
    UpdateFiberGeometry();
  GenerateFiberIds();
  ColorFibersByOrientation();
}

void SplineVisualisationTPMeasurments::GenerateFiberIds()
{
  if (m_FiberPolyData == nullptr)
    return;

  vtkSmartPointer<vtkIdFilter> idFiberFilter = vtkSmartPointer<vtkIdFilter>::New();
  idFiberFilter->SetInputData(m_FiberPolyData);
  idFiberFilter->CellIdsOn();
  //  idFiberFilter->PointIdsOn(); // point id's are not needed
  idFiberFilter->SetCellIdsArrayName(FIBER_ID_ARRAY);
  idFiberFilter->FieldDataOn();
  idFiberFilter->Update();

  m_FiberIdDataSet = idFiberFilter->GetOutput();
}


void SplineVisualisationTPMeasurments::ColorFibersByOrientation()
{
  //===== FOR WRITING A TEST ========================
  //  colorT size == tupelComponents * tupelElements
  //  compare color results
  //  to cover this code 100% also PolyData needed, where colorarray already exists
  //  + one fiber with exactly 1 point
  //  + one fiber with 0 points
  //=================================================

  vtkPoints *extrPoints = m_FiberPolyData->GetPoints();
  vtkIdType numOfPoints = 0;
  if (extrPoints != nullptr)
    numOfPoints = extrPoints->GetNumberOfPoints();

  // colors and alpha value for each single point, RGBA = 4 components
  unsigned char rgba[4] = {0, 0, 0, 0};
  m_FiberColors = vtkSmartPointer<vtkUnsignedCharArray>::New();
  m_FiberColors->Allocate(numOfPoints * 4);
  m_FiberColors->SetNumberOfComponents(4);
  m_FiberColors->SetName("FIBER_COLORS");

  auto numOfFibers = m_FiberPolyData->GetNumberOfLines();
  if (numOfFibers < 1)
    return;

  /* extract single fibers of fiberBundle */
  vtkCellArray *fiberList = m_FiberPolyData->GetLines();
  fiberList->InitTraversal();
  for (int fi = 0; fi < numOfFibers; ++fi)
  {
    vtkIdType const *idList;  // contains the point id's of the line
    vtkIdType pointsPerFiber; // number of points for current line
    fiberList->GetNextCell(pointsPerFiber, idList);

    /* single fiber checkpoints: is number of points valid */
    if (pointsPerFiber > 1)
    {
      /* operate on points of single fiber */
      for (int i = 0; i < pointsPerFiber; ++i)
      {
        /* process all points elastV[0]ept starting and endpoint for calculating color value take current point,
         * previous point and next point */
        if (i < pointsPerFiber - 1 && i > 0)
        {
          /* The color value of the current point is influenced by the previous point and next point. */
          vnl_vector_fixed<double, 3> currentPntvtk(
            extrPoints->GetPoint(idList[i])[0], extrPoints->GetPoint(idList[i])[1], extrPoints->GetPoint(idList[i])[2]);
          vnl_vector_fixed<double, 3> nextPntvtk(extrPoints->GetPoint(idList[i + 1])[0],
                                                 extrPoints->GetPoint(idList[i + 1])[1],
                                                 extrPoints->GetPoint(idList[i + 1])[2]);
          vnl_vector_fixed<double, 3> prevPntvtk(extrPoints->GetPoint(idList[i - 1])[0],
                                                 extrPoints->GetPoint(idList[i - 1])[1],
                                                 extrPoints->GetPoint(idList[i - 1])[2]);

          vnl_vector_fixed<double, 3> diff1;
          diff1 = currentPntvtk - nextPntvtk;

          vnl_vector_fixed<double, 3> diff2;
          diff2 = currentPntvtk - prevPntvtk;

          vnl_vector_fixed<double, 3> diff;
          diff = (diff1 - diff2) / 2.0;
          diff.normalize();

          rgba[0] = static_cast<unsigned char>(255.0 * std::fabs(diff[0]));
          rgba[1] = static_cast<unsigned char>(255.0 * std::fabs(diff[1]));
          rgba[2] = static_cast<unsigned char>(255.0 * std::fabs(diff[2]));
          rgba[3] = static_cast<unsigned char>(255.0);
        }
        else if (i == 0)
        {
          /* First point has no previous point, therefore only diff1 is taken */

          vnl_vector_fixed<double, 3> currentPntvtk(
            extrPoints->GetPoint(idList[i])[0], extrPoints->GetPoint(idList[i])[1], extrPoints->GetPoint(idList[i])[2]);
          vnl_vector_fixed<double, 3> nextPntvtk(extrPoints->GetPoint(idList[i + 1])[0],
                                                 extrPoints->GetPoint(idList[i + 1])[1],
                                                 extrPoints->GetPoint(idList[i + 1])[2]);

          vnl_vector_fixed<double, 3> diff1;
          diff1 = currentPntvtk - nextPntvtk;
          diff1.normalize();

          rgba[0] = static_cast<unsigned char>(255.0 * std::fabs(diff1[0]));
          rgba[1] = static_cast<unsigned char>(255.0 * std::fabs(diff1[1]));
          rgba[2] = static_cast<unsigned char>(255.0 * std::fabs(diff1[2]));
          rgba[3] = static_cast<unsigned char>(255.0);
        }
        else if (i == pointsPerFiber - 1)
        {
          /* Last point has no next point, therefore only diff2 is taken */
          vnl_vector_fixed<double, 3> currentPntvtk(
            extrPoints->GetPoint(idList[i])[0], extrPoints->GetPoint(idList[i])[1], extrPoints->GetPoint(idList[i])[2]);
          vnl_vector_fixed<double, 3> prevPntvtk(extrPoints->GetPoint(idList[i - 1])[0],
                                                 extrPoints->GetPoint(idList[i - 1])[1],
                                                 extrPoints->GetPoint(idList[i - 1])[2]);

          vnl_vector_fixed<double, 3> diff2;
          diff2 = currentPntvtk - prevPntvtk;
          diff2.normalize();

          rgba[0] = static_cast<unsigned char>(255.0 * std::fabs(diff2[0]));
          rgba[1] = static_cast<unsigned char>(255.0 * std::fabs(diff2[1]));
          rgba[2] = static_cast<unsigned char>(255.0 * std::fabs(diff2[2]));
          rgba[3] = static_cast<unsigned char>(255.0);
        }
        m_FiberColors->InsertTypedTuple(idList[i], rgba);
      }
    }
    else if (pointsPerFiber == 1)
    {
      /* a single point does not define a fiber (use vertex mechanisms instead */
      continue;
    }
    else
    {
      MITK_DEBUG << "Fiber with 0 points detected... please check your tractography algorithm!";
      continue;
    }
  }
  m_UpdateTime3D.Modified();
  m_UpdateTime2D.Modified();
}

void SplineVisualisationTPMeasurments::UpdateFiberGeometry()
{
  vtkSmartPointer<vtkCleanPolyData> cleaner = vtkSmartPointer<vtkCleanPolyData>::New();
  cleaner->SetInputData(m_FiberPolyData);
  cleaner->PointMergingOff();
  cleaner->Update();
  m_FiberPolyData = cleaner->GetOutput();

  m_FiberLengths.clear();
  m_MeanFiberLength = 0;
  m_MedianFiberLength = 0;
  m_LengthStDev = 0;
  m_NumFibers = static_cast<unsigned int>(m_FiberPolyData->GetNumberOfCells());

  if (m_FiberColors == nullptr || m_FiberColors->GetNumberOfTuples() != m_FiberPolyData->GetNumberOfPoints())
    this->ColorFibersByOrientation();

  if (m_FiberWeights->GetNumberOfValues() != m_NumFibers)
  {
    m_FiberWeights = vtkSmartPointer<vtkFloatArray>::New();
    m_FiberWeights->SetName("FIBER_WEIGHTS");
    m_FiberWeights->SetNumberOfValues(m_NumFibers);
    this->SetFiberWeights(1);
  }

  if (m_NumFibers <= 0) // no fibers present; apply default geometry
  {
    m_MinFiberLength = 0;
    m_MaxFiberLength = 0;
    mitk::Geometry3D::Pointer geometry = mitk::Geometry3D::New();
    geometry->SetImageGeometry(false);
    float b[] = {0, 1, 0, 1, 0, 1};
    geometry->SetFloatBounds(b);
    if (geometry.IsNotNull())
    {
      SetGeometry(geometry);
    }
    return;
  }
  double b[6];
  m_FiberPolyData->GetBounds(b);

  // calculate statistics
  for (int i = 0; i < m_FiberPolyData->GetNumberOfCells(); i++)
  {
    vtkCell *cell = m_FiberPolyData->GetCell(i);
    auto p = cell->GetNumberOfPoints();
    vtkPoints *points = cell->GetPoints();
    float length = 0;
    for (int j = 0; j < p - 1; j++)
    {
      double p1[3];
      points->GetPoint(j, p1);
      double p2[3];
      points->GetPoint(j + 1, p2);

      double dist = std::sqrt((p1[0] - p2[0]) * (p1[0] - p2[0]) + (p1[1] - p2[1]) * (p1[1] - p2[1]) +
                              (p1[2] - p2[2]) * (p1[2] - p2[2]));
      length += static_cast<float>(dist);
    }
    m_FiberLengths.push_back(length);
    m_MeanFiberLength += length;
    if (i == 0)
    {
      m_MinFiberLength = length;
      m_MaxFiberLength = length;
    }
    else
    {
      if (length < m_MinFiberLength)
        m_MinFiberLength = length;
      if (length > m_MaxFiberLength)
        m_MaxFiberLength = length;
    }
  }
  m_MeanFiberLength /= m_NumFibers;

  std::vector<float> sortedLengths = m_FiberLengths;
  std::sort(sortedLengths.begin(), sortedLengths.end());
  for (unsigned int i = 0; i < m_NumFibers; i++)
    m_LengthStDev += (m_MeanFiberLength - sortedLengths.at(i)) * (m_MeanFiberLength - sortedLengths.at(i));
  if (m_NumFibers > 1)
    m_LengthStDev /= (m_NumFibers - 1);
  else
    m_LengthStDev = 0;
  m_LengthStDev = std::sqrt(m_LengthStDev);
  m_MedianFiberLength = sortedLengths.at(m_NumFibers / 2);

  mitk::Geometry3D::Pointer geometry = mitk::Geometry3D::New();
  geometry->SetFloatBounds(b);
  this->GetGeometry(geometry);

  GetTrackVisHeader();

  m_UpdateTime3D.Modified();
  m_UpdateTime2D.Modified();
}


void SplineVisualisationTPMeasurments::SetFiberWeights(float newWeight)
{
  for (int i = 0; i < m_FiberWeights->GetNumberOfValues(); i++)
    m_FiberWeights->SetValue(i, newWeight);
}

void SplineVisualisationTPMeasurments::SetFiberWeights(vtkSmartPointer<vtkFloatArray> weights)
{
  if (m_NumFibers != weights->GetNumberOfValues())
  {
    MITK_INFO << "Weights array not equal to number of fibers! " << weights->GetNumberOfValues() << " vs "
              << m_NumFibers;
    return;
  }

  for (int i = 0; i < weights->GetNumberOfValues(); i++)
    m_FiberWeights->SetValue(i, weights->GetValue(i));

  m_FiberWeights->SetName("FIBER_WEIGHTS");
}
	


mitk::BaseGeometry::Pointer SplineVisualisationTPMeasurments::GetGeometry(mitk::Geometry3D::Pointer geom)
{
  mitk::PlaneGeometryData *tempobj = mitk::PlaneGeometryData::New();
  if (geom == nullptr)
  {
    mitk::BaseGeometry::Pointer geometry = tempobj->GetGeometry();
  }
  else
  {
    mitk::BaseGeometry::Pointer geometry = geom;
  }
  return geometry;
}

mitk::BaseGeometry::Pointer SplineVisualisationTPMeasurments::GetGeometry()
{
  mitk::PlaneGeometryData *tempobj = mitk::PlaneGeometryData::New();
  mitk::BaseGeometry::Pointer geometry = tempobj->GetGeometry();
  return geometry;
}

void SplineVisualisationTPMeasurments::SetGeometry(mitk::BaseGeometry *geometry)
{
  // mitk::PlaneGeometryData *tempobj = mitk::PlaneGeometryData::New();
  mitk::PlaneGeometry::Pointer planeGeometry = mitk::PlaneGeometry::New();
  mitk::Geometry3D::Pointer m_aGeometry3D = mitk::Geometry3D::New();
  mitk::SlicedGeometry3D::Pointer slicedGeometry = mitk::SlicedGeometry3D::New();
  planeGeometry->InitializeStandardPlane(m_aGeometry3D);
  slicedGeometry->InitializeEvenlySpaced(planeGeometry, m_aGeometry3D->GetExtent(2));
  //tempobj->mitk::PlaneGeometryData::SetGeometry(geometry);

}

SplineVisualisationTPMeasurments::TrackVis_header SplineVisualisationTPMeasurments::GetTrackVisHeader()
{
  if (m_TrackVisHeader.hdr_size == 0)
  {
    mitk::BaseGeometry *tempGeom = this->GetGeometry();
    mitk::Geometry3D *geom = dynamic_cast<mitk::Geometry3D*>(tempGeom); // dynamic_cast<mitk::Geometry3D*>()
    SetTrackVisHeader(geom);
  }
  return m_TrackVisHeader;
}

void SplineVisualisationTPMeasurments::SetTrackVisHeader(mitk::BaseGeometry *geometry)
{
  vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();
  matrix->Identity();

  if (geometry == nullptr)
    return;

  for (int i = 0; i < 3; i++)
  {
    m_TrackVisHeader.dim[i] = geometry->GetExtent(i);
    m_TrackVisHeader.voxel_size[i] = geometry->GetSpacing()[i];
    m_TrackVisHeader.origin[i] = geometry->GetOrigin()[i];
    matrix = geometry->GetVtkMatrix();
  }

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      m_TrackVisHeader.vox_to_ras[i][j] = matrix->GetElement(i, j);

  m_TrackVisHeader.n_scalars = 0;
  m_TrackVisHeader.n_properties = 0;
  sprintf(m_TrackVisHeader.voxel_order, "RAS");
  m_TrackVisHeader.image_orientation_patient[0] = 1.0;
  m_TrackVisHeader.image_orientation_patient[1] = 0.0;
  m_TrackVisHeader.image_orientation_patient[2] = 0.0;
  m_TrackVisHeader.image_orientation_patient[3] = 0.0;
  m_TrackVisHeader.image_orientation_patient[4] = 1.0;
  m_TrackVisHeader.image_orientation_patient[5] = 0.0;
  m_TrackVisHeader.pad1[0] = 0;
  m_TrackVisHeader.pad1[1] = 0;
  m_TrackVisHeader.pad2[0] = 0;
  m_TrackVisHeader.pad2[1] = 0;
  m_TrackVisHeader.invert_x = 0;
  m_TrackVisHeader.invert_y = 0;
  m_TrackVisHeader.invert_z = 0;
  m_TrackVisHeader.swap_xy = 0;
  m_TrackVisHeader.swap_yz = 0;
  m_TrackVisHeader.swap_zx = 0;
  m_TrackVisHeader.n_count = 0;
  m_TrackVisHeader.version = 2;
  m_TrackVisHeader.hdr_size = 1000;
  std::string id = "TRACK";
  //strcpy(m_TrackVisHeader.id_string, id.c_str());
}

/*
* @brief taken form @QmitkPointSetInteractionView Plug-in
* 
* @ param IRenderwindow Part controlls for interaction with the indiviual views and adds them to the Navigation Controller
* 
*/
void SplineVisualisationTPMeasurments::RenderWindowPartActivated(mitk::IRenderWindowPart *renderWindowPart)
{
  if (m_Controls.pointSplineSetListWidget != nullptr )
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
 * @brief taken form @QmitkPointSetInteractionView Plug-in
 *
 * @ param IRenderwindow Part controlls for interaction with the indiviual views and removes them to the Navigation
 * Controller
 *
 */
void SplineVisualisationTPMeasurments::RenderWindowPartDeactivated(mitk::IRenderWindowPart *renderWindowPart)
{
  if (m_Controls.pointSplineSetListWidget != nullptr)
  {
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


void SplineVisualisationTPMeasurments::StartReadPointSet()
{
  ReadCSVPoints();
  // create a PointSet and add the 3D Point to the Set
  mitk::PointSet::Pointer pointset_xyz = mitk::PointSet::New();
  // create a DataNode and set the PointSet as Data for the entity
  mitk::DataNode::Pointer datanode_xyz = mitk::DataNode::New();

  for (int i = 0; i < this->X_Tool1.size(); i++)
  {
    MITK_INFO << this->X_Tool0[i] << " ; " << this->Y_Tool0[i] << " ; " << this->Z_Tool0[i] << "\n" << std::endl;
    double coordinates[] = {this->X_Tool1[i], this->Y_Tool1[i], this->Z_Tool1[i]};

    mitk::Point3D thisPoint(coordinates);
    pointset_xyz->InsertPoint(thisPoint);
  }

  datanode_xyz->SetName("DataNode_XYZ");
  datanode_xyz->SetData(pointset_xyz);

  // DataNode added to the DataStorage
  this->GetDataStorage()->Add(datanode_xyz);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();

  couter_coordinates = couter_coordinates + 1;
  mitk::BaseRenderer::Pointer renderer = mitk::BaseRenderer::Pointer();
  int threeDindex = m_Controls.VisMethodcomboBox->currentIndex();
  MITK_INFO << "!!!!Test Example treeDindex: " << threeDindex << std::endl;
  if (threeDindex == 0)
  {
    MainSplineGenerationFunction(renderer, true, false);
  }
  else
  {
    MainSplineGenerationFunction(renderer, true, true);
  }
}


void SplineVisualisationTPMeasurments::LogsToOutputWindow() {
  m_Controls.textEdit->update();
  MITK_INFO << m_Controls.textEdit;
}




std::string SplineVisualisationTPMeasurments::GetStringElementFromList(list<std::string> ElementList, int idx)
{
  int pos_i = 0;
  list<std::string>::iterator iter;
  for (iter = ElementList.begin(); iter != ElementList.end(); ++iter)
  {
    pos_i++;
    if (pos_i == idx ) // && *iter == idx
    {
      return *iter;
    }
  }
  return 0;
}




QString SplineVisualisationTPMeasurments::SearchForLogfiles()
{
  const QString dir_path = "../../../../mitk/Plugins/org.mitk.gui.qt.splinevisualisationplugin/resources/";
  std::ifstream ifs("../../../../mitk/Plugins/org.mitk.gui.qt.splinevisualisationplugin/resources/");
  QString logfilePath = QFileDialog::getOpenFileName(m_Controls.AllQTabsWidget, tr("Open File"), dir_path, tr("Text Files (*.csv *.xml *.txt)"));
  MITK_INFO << logfilePath << "\n" << std::endl;
  return logfilePath;
}

void SplineVisualisationTPMeasurments::ReadCSVPoints()
{
  std::ifstream file(SplineVisualisationTPMeasurments::SearchForLogfiles().toStdString());
  int sum = 0; // Position in row
  int k = 0;
  size_t pos = 0; 
  std::string subString;
  vector<std::string> headliner; 

  if (!file)
  {
    std::stringstream message;
    message << "Error: Could not open file!";
    MITK_INFO << message.str();
    return;
  }

  //get first line and remove it from reading process
  std::getline(file, subString);
  std::string delimiter = ";";

  // iterate through the entire file and selct each tool value
  while (std::getline(file, subString, ';'))
  {
    if (k == TimeStamp_Tool0.size())
    {
      break;
    }
      switch (sum)
        {
      case 0:
        this->TimeStamp_Tool0[k] = stold(subString);
        MITK_INFO << "TimeStamp_Tool0 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 1:
        this->Valid_Tool0[k] = stold(subString);
        MITK_INFO << "Valid_Tool0 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 2:
        this->X_Tool0[k] = stold(subString);
        MITK_INFO << "X_Tool0 " << sum << ":" << std::endl << subString << std::endl;
         sum++;
        break;

      case 3:
        this->Y_Tool0[k] = stold(subString);
        MITK_INFO << "Y_Tool0 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 4:
        this->Z_Tool0[k] = stold(subString);
        MITK_INFO << "Z_Tool0 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 5:
        this->QX_Tool0[k] = stold(subString);
        MITK_INFO << "QX_Tool0 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 6:
        this->QY_Tool0[k] = stold(subString);
        MITK_INFO << "QY_Tool0 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 7:
        this->QZ_Tool0[k] = stold(subString);
        MITK_INFO << "QZ_Tool0 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 8:
        this->QR_Tool0[k] = stold(subString);
        MITK_INFO << "QR_Tool0 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;
      case 9:
        this->TimeStamp_Tool1[k] = stold(subString);
        MITK_INFO << "TimeStamp_Tool1 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 10:
        this->TimeStamp_Tool1[k] = stold(subString);
        MITK_INFO << "TimeStamp_Tool1 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 11:
        this->Valid_Tool1[k] = stold(subString);
        MITK_INFO << "Valid_Tool1 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;
      case 12:
        this->X_Tool1[k] = stold(subString);
        MITK_INFO << "X_Tool1 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 13:
        this->Y_Tool1[k] = stold(subString);
        MITK_INFO << "Y_Tool1 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 14:
        this->Z_Tool1[k] = stold(subString);
        MITK_INFO << "Z_Tool1 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 15:
        this->QX_Tool1[k] = stold(subString);
        MITK_INFO << "QX_Tool1 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 16:
        this->QY_Tool1[k] = stold(subString);
        MITK_INFO << "QY_Tool1 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 17:
        this->QZ_Tool1[k] = stold(subString);
        MITK_INFO << "QZ_Tool1 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;
      case 18:
        this->QR_Tool1[k] = stold(subString);
        MITK_INFO << "QR_Tool1 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 19:
        this->TimeStamp_Tool2[k] = stold(subString);
        MITK_INFO << "TimeStamp_Tool2 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 20:
        this->Valid_Tool2[k] = stold(subString);
        MITK_INFO << "Valid_Tool2 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;
      case 21:
        this->X_Tool2[k] = stold(subString);
        MITK_INFO << "X_Tool2 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;
      case 22:
        this->Y_Tool2[k] = stold(subString);
        MITK_INFO << "Y_Tool2 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;
      case 23:
        this->Z_Tool2[k] = stold(subString);
        MITK_INFO << "Z_Tool2 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 24:
        this->QX_Tool2[k] = stold(subString);
        MITK_INFO << "QX_Tool2 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 25:
        this->QY_Tool2[k] = stold(subString);
        MITK_INFO << "QY_Tool2 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 26:
        this->QZ_Tool2[k] = stold(subString);
        MITK_INFO << "QZ_Tool2 " << sum << ":" << std::endl << subString << std::endl;
        sum++;
        break;

      case 27:
        pos = subString.find("\n");
        this->QR_Tool2[k] = stold(subString); // .substr(0, pos)
        MITK_INFO << "QR_Tool2 " << sum << ":" << std::endl << subString << std::endl; // .substr(0, pos)
        // subString.erase(0, pos + 1);

        //if (!subString.empty())
        //{
          //this->TimeStamp_Tool0[k + 1] = stold(subString);
          //MITK_INFO << "TimeStamp_Tool0 " << sum << ":" << std::endl << subString << std::endl;
        //}

        sum = 1;
        k++;
        break;

      default:
          MITK_INFO << "Error: Something went wrong!" << std::endl;
    }
     
    
  }
}
