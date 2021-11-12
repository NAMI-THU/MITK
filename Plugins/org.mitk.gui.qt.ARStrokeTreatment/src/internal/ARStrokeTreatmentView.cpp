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
  //connect(m_Controls->m_RegistrationPushButton, SIGNAL(clicked()), this, SLOT(OnRegistrationPushed()));
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
  if (m_VideoGrabbingActive == false)
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
    m_VideoCapture = new cv::VideoCapture("C:/Tools/7.avi");
    mitk::IRenderWindowPart *renderWindow = this->GetRenderWindowPart();
    renderWindow->GetRenderingManager()->InitializeViews(
      m_imageNode->GetData()->GetGeometry(), mitk::RenderingManager::REQUEST_UPDATE_ALL, true);
  }
  if (m_VideoGrabbingActive == true)
  {
    m_Controls->m_VideoGrabbingPushButton->setText("Start Video");
  }
  m_VideoGrabbingActive = !m_VideoGrabbingActive;
}

//void ARStrokeTreatmentView::OnAddRegistrationTrackingFiducial()
//{
//  mitk::NavigationData::Pointer nd = m_InstrumentNavigationData;
//
//  if (nd.IsNull() || !nd->IsDataValid())
//  {
//    QMessageBox::warning(
//      nullptr, "Invalid tracking data", "Navigation data is not available or invalid!", QMessageBox::Ok);
//    return;
//  }
//
//  if (m_TrackerFiducialsDataNode.IsNotNull() && m_TrackerFiducialsDataNode->GetData() != nullptr)
//  {
//    mitk::PointSet::Pointer ps = dynamic_cast<mitk::PointSet *>(m_TrackerFiducialsDataNode->GetData());
//    ps->InsertPoint(ps->GetSize(), nd->GetPosition());
//  }
//  else
//    QMessageBox::warning(
//      nullptr, "IGTSurfaceTracker: Error", "Can not access Tracker Fiducials. Adding fiducial not possible!");
//}

//void ARStrokeTreatmentView::OnRegistrationPushed()
//{
//
//  mitk::PointSet::Pointer imageFiducials = dynamic_cast<mitk::PointSet *>(m_ImageFiducialsDataNode->GetData());
//  mitk::PointSet::Pointer trackerFiducials = dynamic_cast<mitk::PointSet *>(m_TrackerFiducialsDataNode->GetData());
//
//  //############### conversion to vtk data types (we will use the vtk landmark based transform)
//  //########################## convert point sets to vtk poly data
//  vtkSmartPointer<vtkPoints> sourcePoints = vtkSmartPointer<vtkPoints>::New();
//  vtkSmartPointer<vtkPoints> targetPoints = vtkSmartPointer<vtkPoints>::New();
//  for (int i = 0; i < imageFiducials->GetSize(); i++)
//  {
//    double point[3] = {imageFiducials->GetPoint(i)[0], imageFiducials->GetPoint(i)[1], imageFiducials->GetPoint(i)[2]};
//    sourcePoints->InsertNextPoint(point);
//    double point_targets[3] = {
//      trackerFiducials->GetPoint(i)[0], trackerFiducials->GetPoint(i)[1], trackerFiducials->GetPoint(i)[2]};
//    targetPoints->InsertNextPoint(point_targets);
//  }
//
//  //########################### here, the actual transform is computed ##########################
//  // compute transform
//  vtkSmartPointer<vtkLandmarkTransform> transform = vtkSmartPointer<vtkLandmarkTransform>::New();
//  transform->SetSourceLandmarks(sourcePoints);
//  transform->SetTargetLandmarks(targetPoints);
//  transform->SetModeToRigidBody();
//  transform->Modified();
//  transform->Update();
//  // compute FRE of transform
//  double FRE = mitk::StaticIGTHelperFunctions::ComputeFRE(imageFiducials, trackerFiducials, transform);
//  m_Controls.m_RegistrationWidget->SetQualityDisplayText("FRE: " + QString::number(FRE) + " mm");
//  //#############################################################################################
//
//  //############### conversion back to itk/mitk data types ##########################
//  // convert from vtk to itk data types
//  itk::Matrix<float, 3, 3> rotationFloat = itk::Matrix<float, 3, 3>();
//  itk::Vector<float, 3> translationFloat = itk::Vector<float, 3>();
//  itk::Matrix<double, 3, 3> rotationDouble = itk::Matrix<double, 3, 3>();
//  itk::Vector<double, 3> translationDouble = itk::Vector<double, 3>();
//
//  vtkSmartPointer<vtkMatrix4x4> m = transform->GetMatrix();
//  for (int k = 0; k < 3; k++)
//    for (int l = 0; l < 3; l++)
//    {
//      rotationFloat[k][l] = m->GetElement(k, l);
//      rotationDouble[k][l] = m->GetElement(k, l);
//    }
//  for (int k = 0; k < 3; k++)
//  {
//    translationFloat[k] = m->GetElement(k, 3);
//    translationDouble[k] = m->GetElement(k, 3);
//  }
//  // create affine transform 3D surface
//  mitk::AffineTransform3D::Pointer mitkTransform = mitk::AffineTransform3D::New();
//  mitkTransform->SetMatrix(rotationDouble);
//  mitkTransform->SetOffset(translationDouble);
//  //#############################################################################################
//
//  //############### object is transformed ##########################
//  // save transform
//  m_T_ObjectReg = mitk::NavigationData::New(
//    mitkTransform); // this is stored in a member because it is needed for permanent registration later on
//
//  // transform surface
//  if (m_Controls.m_SurfaceActive->isChecked() && m_Controls.m_ObjectComboBox->GetSelectedNode().IsNotNull())
//  {
//    m_Controls.m_ObjectComboBox->GetSelectedNode()->GetData()->GetGeometry()->SetIndexToWorldTransform(mitkTransform);
//  }
//  //################################################################
//
//  //############### if activated: ct image is also transformed ##########################
//  // transform ct image
//  // todo: Explain that AffineTransform3D is used, because NavigationData does not support spacing!
//  if (m_Controls.m_ImageActive->isChecked() && m_Controls.m_ImageComboBox->GetSelectedNode().IsNotNull())
//  {
//    // first we have to store the original ct image transform to compose it with the new transform later
//    mitk::AffineTransform3D::Pointer imageTransform =
//      m_Controls.m_ImageComboBox->GetSelectedNode()->GetData()->GetGeometry()->GetIndexToWorldTransform();
//    m_T_ImageGeo = mitk::AffineTransform3D::New(); // this is also stored in a member because it is needed for
//                                                   // permanent registration later on
//    // now the new transform of the ct image is computed
//    m_T_ImageGeo->Compose(imageTransform);
//    imageTransform->Compose(mitkTransform);
//    mitk::AffineTransform3D::Pointer newImageTransform =
//      mitk::AffineTransform3D::New(); // create new image transform... setting the composed directly leads to an error
//    itk::Matrix<mitk::ScalarType, 3, 3> rotationFloatNew = imageTransform->GetMatrix();
//    itk::Vector<mitk::ScalarType, 3> translationFloatNew = imageTransform->GetOffset();
//    newImageTransform->SetMatrix(rotationFloatNew);
//    newImageTransform->SetOffset(translationFloatNew);
//    m_Controls.m_ImageComboBox->GetSelectedNode()->GetData()->GetGeometry()->SetIndexToWorldTransform(
//      newImageTransform);
//    m_T_ImageReg = m_Controls.m_ImageComboBox->GetSelectedNode()->GetData()->GetGeometry()->GetIndexToWorldTransform();
//  }
//  //################################################################
//}

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
    cv::Mat frame;
    m_VideoCapture->read(frame);
    m_ConversionFilter->SetOpenCVMat(frame);
    m_ConversionFilter->Update();
    m_imageNode->SetData(m_ConversionFilter->GetOutput());
    //m_imageNode->GetData()->GetGeometry()->SetIndexToWorldTransform();
    mitk::Vector3D setSpacing;
    //setSpacing[0] = 1; //left-right
    //setSpacing[1] = 1; //up
    //setSpacing[2] = 1; //height
    //m_imageNode->GetData()->GetGeometry()->SetSpacing(setSpacing);
    m_imageNode->Modified();
    //mitk::IRenderWindowPart *renderWindow = this->GetRenderWindowPart();
    //renderWindow->GetRenderingManager()->InitializeViews(
    //  m_imageNode->GetData()->GetGeometry(), mitk::RenderingManager::REQUEST_UPDATE_ALL, false);
    this->RequestRenderWindowUpdate(mitk::RenderingManager::REQUEST_UPDATE_ALL);
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
