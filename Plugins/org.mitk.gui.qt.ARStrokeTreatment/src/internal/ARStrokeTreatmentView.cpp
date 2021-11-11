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
  m_Controls->m_RegistrationWidget->setDataStorage(this->GetDataStorage());
  m_Controls->m_RegistrationWidget->setDataStorage(this->GetDataStorage());
  m_Controls->m_RegistrationWidget->HideStaticRegistrationRadioButton(true);
  m_Controls->m_RegistrationWidget->HideContinousRegistrationRadioButton(true);
  m_Controls->m_RegistrationWidget->HideUseICPRegistrationCheckbox(true);
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
  connect(m_Controls->m_RegistrationPushButton, SIGNAL(clicked()), this, SLOT(OnRegistrationPushed()));
  connect(m_UpdateTimer, SIGNAL(timeout()), this, SLOT(UpdateLiveData()));
  return;
}

void ARStrokeTreatmentView::OnTrackingGrabberPushed()
{
  if (!m_TrackingActive)
  {
    m_TrackingActive = true;
    m_Controls->m_TrackerGrabbingPushButton->setText("Stop Tracking");
  }
  if (m_TrackingActive)
  {
    m_TrackingActive = false;
    m_Controls->m_TrackerGrabbingPushButton->setText("Start Tracking");
  }
  return;
}

void ARStrokeTreatmentView::OnVideoGrabberPushed()
{
  if (!m_VideoGrabbingActive)
  {
    m_VideoGrabbingActive = true;
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
    mitk::IRenderWindowPart *renderWindow = this->GetRenderWindowPart();
    renderWindow->GetRenderingManager()->InitializeViews(
      m_imageNode->GetData()->GetGeometry(), mitk::RenderingManager::REQUEST_UPDATE_ALL, true);
  }
}

void ARStrokeTreatmentView::OnRegistrationPushed()
{
  return;
}

void ARStrokeTreatmentView::UpdateLiveData()
{
  if (m_TrackingActive)
  {
    m_TrackingData = m_Controls->m_TrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetOutput(
      m_Controls->m_TrackingDeviceSelectionWidget->GetSelectedToolID());
    mitk::NavigationData::Pointer navData =
      m_Controls->m_TrackingDeviceSelectionWidget->GetSelectedNavigationDataSource()->GetOutput(0);
    MITK_INFO << navData->GetPosition();
  }
  if (m_VideoGrabbingActive)
  {
    if (m_VideoCapture == NULL)
    {
      if (true)
      {
        m_VideoCapture = new cv::VideoCapture(0);
      }
      if (false)
      {
        m_VideoCapture = new cv::VideoCapture("C:/7.avi");
      }
    }
    if (!m_VideoCapture->isOpened())
    {
      MITK_WARN << "Video Camera not recognized!";
      return;
    }
    cv::Mat frame;
    if (!m_VideoCapture->read(frame))
    {
      MITK_INFO << "ERROR!";
      return;
    }
    m_VideoCapture->read(frame);
    m_ConversionFilter->SetOpenCVMat(frame);
    m_ConversionFilter->Update();
    m_imageNode->SetData(m_ConversionFilter->GetOutput());
    m_imageNode->Modified();
    mitk::IRenderWindowPart *renderWindow = this->GetRenderWindowPart();
    renderWindow->GetRenderingManager()->InitializeViews(
      m_imageNode->GetData()->GetGeometry(), mitk::RenderingManager::REQUEST_UPDATE_ALL, true);
  }
  return;
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
