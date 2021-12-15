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

// Qmitk
#include "ARStrokeTreatmentRegistration.h"
#include "ARStrokeTreatmentView.h"

// Qt
#include <QMessageBox>
#include <QmitkDataStorageComboBox.h>

// mitk image
#include <QTimer>
#include <mitkIRenderingManager.h>
#include <mitkImage.h>
#include <mitkImageGenerator.h>
#include <mitkNodePredicateDataType.h>
#include <mitkNodePredicateNot.h>
#include <mitkNodePredicateOr.h>
#include <mitkNodePredicateProperty.h>
#include <mitkOpenCVToMitkImageFilter.h>

const std::string ARStrokeTreatmentView::VIEW_ID = "org.mitk.views.arstroketreatment";

void ARStrokeTreatmentView::SetFocus()
{
  // m_Controls.buttonPerformImageProcessing->setFocus();
}

void ARStrokeTreatmentView::CreateQtPartControl(QWidget *parent)
{
  // create GUI widgets from the Qt Designer's .ui file
  m_Controls = new Ui::ARStrokeTreatmentControls;
  m_Controls->setupUi(parent);
  // connect(m_Controls.buttonPerformImageProcessing, &QPushButton::clicked, this,
  // &ARStrokeTreatmentView::DoImageProcessing);
  // create timer for tracking and video grabbing
  MITK_INFO << "timer started!";
  m_UpdateTimer = new QTimer(this);
  m_UpdateTimer->setInterval(100);
  m_UpdateTimer->start();

  CreateConnections();

  // m_Controls->m_DataStorageComboBox->SetPredicate(
  //  mitk::NodePredicateOr::New(mitk::NodePredicateDataType::New("Surface"),
  //  mitk::NodePredicateDataType::New("Image")));

  m_Controls->m_RegistrationWidget->setDataStorage(this->GetDataStorage());
  m_Controls->m_RegistrationWidget->HideStaticRegistrationRadioButton(true);
  m_Controls->m_RegistrationWidget->HideContinousRegistrationRadioButton(true);
  m_Controls->m_RegistrationWidget->HideUseICPRegistrationCheckbox(true);

  m_Controls->m_DataStorageComboBox->SetDataStorage(this->GetDataStorage());
  m_Controls->m_DataStorageComboBox->SetAutoSelectNewItems(false);

  m_Controls->m_VideoPausePushButton->setDisabled(true);
}

void ARStrokeTreatmentView::OnSelectionChanged(berry::IWorkbenchPart::Pointer /*source*/,
                                               const QList<mitk::DataNode::Pointer> &nodes)
{
  // iterate all selected objects, adjust warning visibility
  foreach (mitk::DataNode::Pointer node, nodes)
  {
    if (node.IsNotNull() && dynamic_cast<mitk::Image *>(node->GetData()))
    {
      // m_Controls.labelWarning->setVisible(false);
      return;
    }
  }
  // m_Controls.labelWarning->setVisible(true);
}

void ARStrokeTreatmentView::CreateConnections()
{
  // connect(m_Controls.m_TrackingDeviceSelectionWidget,
  //        SIGNAL(NavigationDataSourceSelected(mitk::NavigationDataSource::Pointer)),
  //        this,
  //        SLOT(OnSetupNavigation()));
  connect(m_Controls->m_TrackerGrabbingPushButton, SIGNAL(clicked()), this, SLOT(OnTrackingGrabberPushed()));
  connect(m_Controls->m_VideoGrabbingPushButton, SIGNAL(clicked()), this, SLOT(OnVideoGrabberPushed()));
  connect(m_Controls->m_VideoPausePushButton, SIGNAL(clicked()), this, SLOT(OnVideoPausePushButton()));
  connect(m_UpdateTimer, SIGNAL(timeout()), this, SLOT(UpdateLiveData()));

  // create connections for Registration Widget
  connect(m_Controls->m_ChooseSelectedPointer, SIGNAL(clicked()), this, SLOT(PointerSelectionChanged()));
  connect(m_Controls->m_ChooseSelectedImage, SIGNAL(clicked()), this, SLOT(ImageSelectionChanged()));
  return;
}

void ARStrokeTreatmentView::OnTrackingGrabberPushed()
{
  if (m_TrackingActive == false)
  {
    if (m_Controls->m_TrackingDeviceSelectionWidget->GetSelectedToolID() == -1)
    {
      m_TrackingActive = false;
      return;
    }
    m_Controls->m_TrackerGrabbingPushButton->setText("Stop Tracking");
    m_TrackingActive = true;
    this->InitializeConeView();
  }
  else if (m_TrackingActive == true)
  {
    m_Controls->m_TrackerGrabbingPushButton->setText("Start Tracking");
    m_TrackingActive = false;
  }
  return;
}

void ARStrokeTreatmentView::InitializeConeView()
{
  m_Cone = mitk::Cone::New(); // instantiate a new cone
  double scale[] = {20.0, 20.0, 20.0};
  m_Cone->GetGeometry()->SetSpacing(scale);
  m_ConeNode = mitk::DataNode::New();
  m_ConeNode->SetColor(1.0, 0.0, 0.0);
  m_ConeNode->SetName("My tracked object");

  this->GetDataStorage()->Add(m_ConeNode);
}

void ARStrokeTreatmentView::OnVideoGrabberPushed()
{
  if (!m_VideoGrabbingActive)
  {
    m_Controls->m_VideoGrabbingPushButton->setText("Stop Video");
    // Initialize new video grabber
    m_imageNode = mitk::DataNode::New();
    std::stringstream nodeName;
    nodeName << "Live Image Stream";
    m_imageNode->SetName(nodeName.str());
    // create a dummy image (gray values 0..255) for correct initialization of level window, etc.
    mitk::Image::Pointer dummyImage = mitk::ImageGenerator::GenerateRandomImage<float>(100, 100, 1, 1, 1, 1, 1, 255, 0);
    m_imageNode->SetData(dummyImage);
    this->GetDataStorage()->Add(m_imageNode);

    // select video source
    // m_VideoCapture = new cv::VideoCapture("C:/Tools/7.avi");
    m_VideoCapture = new cv::VideoCapture(0);
    mitk::IRenderWindowPart *renderWindow = this->GetRenderWindowPart();
    renderWindow->GetRenderingManager()->InitializeViews(
      m_imageNode->GetData()->GetGeometry(), mitk::RenderingManager::REQUEST_UPDATE_ALL, true);
    m_Controls->m_VideoPausePushButton->setDisabled(false);
  }
  if (m_VideoGrabbingActive)
  {
    ARStrokeTreatmentView::DisableVideoData();
  }
  m_VideoGrabbingActive = !m_VideoGrabbingActive;
}

void ARStrokeTreatmentView::PointerSelectionChanged()
{
  ARStrokeTreatmentView::InitializeRegistration();
  int toolID = m_Controls->m_TrackingDeviceSelectionWidget->GetSelectedToolID();
  m_TrackingData = m_Controls->m_TrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetOutput(toolID);
  m_Controls->m_RegistrationWidget->setTrackerNavigationData(m_TrackingData);
  m_Controls->m_PointerLabel->setText(
    m_Controls->m_TrackingDeviceSelectionWidget->GetSelectedNavigationTool()->GetToolName().c_str());
}

void ARStrokeTreatmentView::ImageSelectionChanged()
{
  ARStrokeTreatmentView::InitializeRegistration();
  m_Controls->m_ImageLabel->setText(m_Controls->m_DataStorageComboBox->GetSelectedNode()->GetName().c_str());
  m_Controls->m_RegistrationWidget->setImageNode(m_Controls->m_DataStorageComboBox->GetSelectedNode());
}

void ARStrokeTreatmentView::InitializeRegistration()
{
  foreach (QmitkRenderWindow *renderWindow, this->GetRenderWindowPart()->GetQmitkRenderWindows().values())
  {
    this->m_Controls->m_RegistrationWidget->AddSliceNavigationController(renderWindow->GetSliceNavigationController());
  }
}

void ARStrokeTreatmentView::UpdateTrackingData()
{
  m_TrackingData = m_Controls->m_TrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetOutput(
    m_Controls->m_TrackingDeviceSelectionWidget->GetSelectedToolID());
  m_ConeNode = m_Controls->m_TrackingDeviceSelectionWidget->GetSelectedNavigationTool()->GetDataNode();

  m_Controls->m_TrackingDeviceSelectionWidget->GetSelectedNavigationDataSource();

  mitk::TrackingVolumeGenerator::Pointer volumeGenerator = mitk::TrackingVolumeGenerator::New();
  // volumeGenerator->SetTrackingDeviceData(m_TrackingData);

  m_ConeNode->SetData(m_Cone);
  m_ConeNode->Modified();

  // m_TrackingData->GetPosition();
  // m_TrackingData->GetOrientation();
}

void ARStrokeTreatmentView::UpdateLiveData()
{
  m_Controls->m_TrackingDeviceSelectionWidget->update();
  if (m_TrackingActive)
  {
    UpdateTrackingData();
  }
  if (m_VideoGrabbingActive && m_UpdateVideoData)
  {
    cv::Mat frame;
    if (m_VideoCapture->read(frame))
    {
      // m_VideoCapture->read(frame);
      m_ConversionFilter->SetOpenCVMat(frame);
      m_ConversionFilter->Update();
      m_imageNode->SetData(m_ConversionFilter->GetOutput());
      // m_imageNode->GetData()->GetGeometry()->SetIndexToWorldTransform();
      mitk::Vector3D setSpacing;
      setSpacing[0] = 0.1; // left-right
      setSpacing[1] = 0.1; // up
      setSpacing[2] = 0.1; // height
      m_imageNode->GetData()->GetGeometry()->SetSpacing(setSpacing);
      m_imageNode->Modified();
      // mitk::IRenderWindowPart *renderWindow = this->GetRenderWindowPart();
      // renderWindow->GetRenderingManager()->InitializeViews(
      //  m_imageNode->GetData()->GetGeometry(), mitk::RenderingManager::REQUEST_UPDATE_ALL, false);
      this->RequestRenderWindowUpdate(mitk::RenderingManager::REQUEST_UPDATE_ALL);
    }
    else
    {
      MITK_ERROR << "No Image could be read. Video Source not found or finished!";
      ARStrokeTreatmentView::DisableVideoData();
    }
  }
  return;
}

void ARStrokeTreatmentView::DisableVideoData()
{
  m_VideoGrabbingActive = false;
  m_VideoCapture = new cv::VideoCapture;
  m_Controls->m_VideoGrabbingPushButton->setText("Start Video");
  m_Controls->m_VideoPausePushButton->setDisabled(true);
  this->GetDataStorage()->Remove(m_imageNode);
  return;
}

void ARStrokeTreatmentView::OnVideoPausePushButton()
{
  m_UpdateVideoData = !m_UpdateVideoData;
  if (m_UpdateVideoData)
  {
    m_Controls->m_VideoPausePushButton->setText("Pause Video");
  }
  if (!m_UpdateVideoData)
  {
    m_Controls->m_VideoPausePushButton->setText("Continue Video");
  }
}

void ARStrokeTreatmentView::DoImageProcessing()
{
  QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
  if (nodes.empty())
    return;

  mitk::DataNode *node = nodes.front();

  if (!node)
  {
    // Nothing selected. Inform the user and return
    QMessageBox::information(nullptr, "Template", "Please load and select an image before starting image processing.");
    return;
  }

  // here we have a valid mitk::DataNode

  // a node itself is not very useful, we need its data item (the image)
  mitk::BaseData *data = node->GetData();
  if (data)
  {
    // test if this data item is an image or not (could also be a surface or something totally different)
    mitk::Image *image = dynamic_cast<mitk::Image *>(data);
    if (image)
    {
      std::stringstream message;
      std::string name;
      message << "Performing image processing for image ";
      if (node->GetName(name))
      {
        // a property called "name" was found for this DataNode
        message << "'" << name << "'";
      }
      message << ".";
      MITK_INFO << message.str();

      // actually do something here...
    }
  }
}

void ARStrokeTreatmentView::TestText()
{
  MITK_INFO << "TestText succesfully printed! Yay!";
  return;
}
