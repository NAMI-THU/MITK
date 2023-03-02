/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/
#include "../../../org.mitk.gui.qt.igttracking/src/internal/QmitkMITKIGTTrackingToolboxView.h"
#include <mitkBaseData.h>
#include <mitkBaseRenderer.h>
#include <mitkClippingProperty.h>
#include <mitkDataInteractor.h>
#include <mitkDataNode.h>
#include <mitkMapper.h>
#include <mitkNavigationData.h>
#include <mitkNavigationDataObjectVisualizationFilter.h>
#include <mitkNavigationDataToNavigationDataFilter.h>
#include <mitkNavigationToolStorage.h>
#include <mitkRenderWindow.h>
#include <mitkSplineVtkMapper3D.h>
#include <mitkStandaloneDataStorage.h>
#include <mitkTrackingDeviceSource.h>
#include <mitkTrackingTool.h>
#include <mitkVirtualTrackingDevice.h>
#include <vector>
#include <boost/range/combine.hpp>
#include <internal/SplineVisualisationPlugin.h>
#include <internal/mitkSplineDataObjectVisualizationFilter.h>

//  VTK imports
#include <mitkIOUtil.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCardinalSpline.h>
#include <vtkCellArray.h>
#include <vtkColor.h>
#include <vtkCornerAnnotation.h>
#include <vtkDataReader.h>
#include <vtkFloatArray.h>
#include <vtkGenericDataSet.h>
#include <vtkGlyph3DMapper.h>
#include <vtkKochanekSpline.h>            
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricSpline.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyLine.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSCurveSpline.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkSplineFilter.h>
#include <vtkSplineRepresentation.h>
#include <vtkSplineWidget.h>
#include <vtkSplineWidget2.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkTubeFilter.h>
#include <vtkViewport.h>
#include <vtkWin32RenderWindowInteractor.h>

// b spline specifc imports
#include "mitkSmartPointerProperty.h"
#include <vtkBSplineTransform.h>
#include <vtkContourFilter.h>
#include <vtkDoubleArray.h>
#include <vtkGaussianSplatter.h>
#include <vtkImageBSplineCoefficients.h>
#include <vtkLookupTable.h>
#include <vtkMaskPoints.h>
#include <vtkTexture.h>
#include <vtkThinPlateSplineTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransformToGrid.h>
#include <vtkTupleInterpolator.h>


// Create text
#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

SplineDataObjectVisualizationFilter::SplineDataObjectVisualizationFilter(
mitk::NavigationToolStorage::Pointer m_toolStorage, mitk::StandaloneDataStorage::Pointer dataStorage)
  : NavigationDataToNavigationDataFilter(), m_RepresentationVectorMap(),
    m_TrackingLoggingTimer(), 
    m_RotationMode(mitk::NavigationDataObjectVisualizationFilter::RotationStandard)
{
  this->m_RepresentationVectorMap;
  this->m_ToolStorage;
  this->DataStorage;
  Vtkrenderer = vtkRenderer::New();
  VtkrenderWindow = vtkRenderWindow::New();
  m_ResetRenderer = false;
  m_ShowVTKSubwindow = false;
  m_Debug = false;
  DataInteractor = mitk::DataInteractor::New();
  SplineVtkMapper = mitk::SplineVtkMapper3D::New();
  m_EMRefreshrate = 50;
  m_SplineVisualisationEvalUtil = new SplineVisualisationEvaluationUtil();
  m_SplineVisualPlugin = new SplineVisualisationPlugin();
  // Bounds (within the random numbers are generated) must be set before the tools are added
  Bound = 5.0;
  Bounds[0] = -Bound;
  Bounds[2] = Bounds[4] = Bounds[0];
  Bounds[1] = Bounds[3] = Bounds[5] = Bound;
}

SplineDataObjectVisualizationFilter::~SplineDataObjectVisualizationFilter()
{
  m_RepresentationVectorMap.clear();
  Vtkrenderer->Delete();
  VtkrenderWindow->Delete();
  DataInteractor->Delete();
  m_SplineReferencePointSet->Delete();
  SplineVtkMapper->Delete();
}

void SplineDataObjectVisualizationFilter::InitialiseInputTrackingData(int currentSensoryIndex,
                                                                      int selectiontype,
                                                                      bool resetRenderer,
                                                                      double parametricRange,
    bool debug)
{
  m_CurrentRefDoubleSelection = parametricRange;
  m_CurrentSensoryIndex = currentSensoryIndex;
  DataInteractor->EnableInteraction(true);
  m_ResetRenderer = resetRenderer;
  m_SelectionType = selectiontype;
  m_SplineReferencePointSet = mitk::PointSet::New();
  m_combinedPointSet = mitk::PointSet::New();
  m_lastAnchorPointSet = mitk::PointSet::New();
  OutputDataNode = mitk::DataNode::New();
  Emtracker = mitk::VirtualTrackingDevice::New();
  Emsource = mitk::TrackingDeviceSource::New();
  m_Debug = debug;
  m_Timestep = 0;

  if (m_Debug)
  {
    this->m_SplineVisualisationEvalUtil->SetDebug(true);
  }
  else
  {
    this->m_SplineVisualisationEvalUtil->SetDebug(false);
  }
}

mitk::DataNode::Pointer SplineDataObjectVisualizationFilter::InputTrackingData(
  QmitkNavigationDataSourceSelectionWidget *navidatasourceselection,
  mitk::StandaloneDataStorage::Pointer dataStorage, bool appendPointset)
{
  m_VecEMBaseData = std::vector<mitk::NavigationData::Pointer>();
  m_appendPointset = appendPointset;
  Emtracker->SetBounds(Bounds);
  Emtracker->GetToolByName(navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(0)->GetName());
  Emtracker->SetRefreshRate(m_EMRefreshrate);
  Emsource->SetTrackingDevice(Emtracker);
  EmTool0 = navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(0);
  EmTool0->SetName(navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(0)->GetName());
  if (m_Debug)
  {
    MITK_INFO << "Position EMtool 0: " << EmTool0->GetPosition() << std::endl;
  }
  m_VecEMBaseData.push_back(EmTool0);

  if (m_CurrentSensoryIndex >= 2)
  {
    Emtracker->GetToolByName(navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(1)->GetName());
    EmTool1 = navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(1);
    EmTool1->SetName(navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(1)->GetName());
    if (m_Debug)
    {
      MITK_INFO << "Position EMtool 1: " << EmTool1->GetPosition() << std::endl;
    }
    m_VecEMBaseData.push_back(EmTool1);

    if (m_CurrentSensoryIndex >= 3)
    {
      Emtracker->GetToolByName(navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(2)->GetName());
      EmTool2 = navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(2);
      EmTool2->SetName(navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(2)->GetName());
      if (m_Debug)
      {
        MITK_INFO << "Position EMtool 2: " << EmTool2->GetPosition() << std::endl;
      }
      m_VecEMBaseData.push_back(EmTool2);

      if (m_CurrentSensoryIndex >= 4)
      {
        Emtracker->GetToolByName(
          navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(3)->GetName()); // or AddTool
        EmTool3 = navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(3);
        EmTool3->SetName(navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(3)->GetName());
        if (m_Debug)
        {
          MITK_INFO << "Position EMtool 3: " << EmTool3->GetPosition() << std::endl;
        } 
        m_VecEMBaseData.push_back(EmTool3);

         if (m_CurrentSensoryIndex >= 5)
        {
           Emtracker->GetToolByName(
             navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(4)->GetName());
          EmTool4 = navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(4);
          EmTool4->SetName(navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(4)->GetName());
          if (m_Debug)
          {
            MITK_INFO << "Position EMtool 4: " << EmTool4->GetPosition() << std::endl;
          }
          m_VecEMBaseData.push_back(EmTool3);

           if (m_CurrentSensoryIndex >= 6)
          {
             Emtracker->GetToolByName(
               navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(5)->GetName());
            EmTool5 = navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(5);
            EmTool5->SetName(navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(5)->GetName());
            if (m_Debug)
            {
              MITK_INFO << "Position EMtool 5: " << EmTool5->GetPosition() << std::endl;
            }
            m_VecEMBaseData.push_back(EmTool3);
            if (m_CurrentSensoryIndex >= 7)
            {
              Emtracker->GetToolByName(
                navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(6)->GetName());
              EmTool6 = navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(6);
              EmTool6->SetName(navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(6)->GetName());
              if (m_Debug)
              {
                MITK_INFO << "Position EMtool 6: " << EmTool6->GetPosition() << std::endl;
              }
              m_VecEMBaseData.push_back(EmTool3);

              if (m_CurrentSensoryIndex >= 8)
              {
                Emtracker->GetToolByName(
                  navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(7)->GetName());
                EmTool7 = navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(7);
                EmTool7->SetName(navidatasourceselection->GetSelectedNavigationDataSource()->GetOutput(7)->GetName());
                if (m_Debug)
                {
                  MITK_INFO << "Position EMtool 7: " << EmTool3->GetPosition() << std::endl;
                }
                m_VecEMBaseData.push_back(EmTool3);
              }
            }
          }
        }
      }
    }
  }

  // get datanode for datastorage
  OutputDataNode = GetSplineDataNode(m_VecEMBaseData);

  // Filter for rendering the spline at correct position and orientation
  this->SetInput(0, Emsource->GetOutput());
  this->SetRepresentationObject(0, OutputDataNode->GetData());
  Emsource->Connect();
  this->mitk::NavigationDataToNavigationDataFilter::GenerateData();
  // Update the spline position
  this->Update();

 return OutputDataNode;
}

mitk::DataNode::Pointer SplineDataObjectVisualizationFilter::GetSplineDataNode(std::vector<mitk::NavigationData::Pointer> vecEMToolData)
{
  mitk::DataNode::Pointer outputDataNode = mitk::DataNode::New();
  mitk::PointSet::Pointer splinePointSet = mitk::PointSet::New();

  for (mitk::NavigationData::Pointer sToolData : vecEMToolData)
  {
    if (m_Debug)
    {
      MITK_INFO << "Vtk Spline Pointset :"
                  << "IGT Timestap = " << sToolData->GetIGTTimeStamp() << std::endl
                << "MITK Timestap = " << sToolData->GetTimeStamp() << std::endl
      << "Position = " <<sToolData->GetPosition() << std::endl;   
    } 
    // make pointset and aquire 3d Point
    splinePointSet->InsertPoint(sToolData->GetPosition());
  }

  switch (m_SelectionType)
  {
    case 0:
      outputDataNode = ConvertParametricSplineRepresentation(splinePointSet);
      break;
    case 1:
      outputDataNode = Convert3DTubeRepresentation(splinePointSet);
      break;
    case 2:
      outputDataNode = ConvertSCurveRepresentation(splinePointSet);
      break;
    case 3:
      outputDataNode = ConvertKochanekSplineRepresentation(splinePointSet, false);
      break;
    case 4:
      outputDataNode = ConvertCardinalSplineRepresentation(splinePointSet);
      break;
    case 5:
      outputDataNode = ConvertBSplineRepresentation(splinePointSet);
      break;
    case 6:
      outputDataNode = ConvertKochanekSplineRepresentation(splinePointSet, true);

    default:
      outputDataNode = ConvertParametricSplineRepresentation(splinePointSet);
      break;
  }

  return outputDataNode;
}


mitk::DataNode::Pointer  SplineDataObjectVisualizationFilter::ConvertParametricSplineRepresentation(mitk::PointSet::Pointer AnchorPointSet)
 {
  m_combinedPointSet = GettingCombinedPointset(AnchorPointSet);

  vtkNew<vtkNamedColors> colors;
  vtkSPtrNew(textSource, vtkTextActor);
  // the spline Range with absolute values
  vtkNew<vtkParametricSpline> parametricspline;
  parametricspline->SetPoints(m_SplineVisualisationEvalUtil->ConvertMITKIntoVTKPointset(m_combinedPointSet));
  vtkNew<vtkSplineWidget> splinewidget;
  splinewidget->SetParametricSpline(parametricspline);
  // create a loop to evaluate the parametric coordinate u from [0,1] u[0] , u[1], u[2],
  // Using void Evaluate(double u[3], double Pt[3], double Du[9]) override
  m_SplineReferencePointSet = 
      m_SplineVisualisationEvalUtil->GenerateSplinePoints(
    parametricspline, m_combinedPointSet, m_CurrentRefDoubleSelection);
  TempString = m_SplineVisualisationEvalUtil->VtkQStringTextBlock(m_combinedPointSet, m_SplineReferencePointSet);
  textSource->SetInput(TempString.toLocal8Bit().data());
  textSource->GetTextProperty()->SetFontSize(18);
  textSource->GetTextProperty()->SetJustificationToLeft();
  textSource->GetTextProperty()->SetVerticalJustificationToBottom();
  textSource->GetTextProperty()->BoldOn();
  textSource->GetTextProperty()->SetColor(1, 1, 1);
  // Parametric function for spline
  vtkNew<vtkParametricFunctionSource> functionSource;
  functionSource->SetParametricFunction(parametricspline);
  functionSource->Update();
  // Setup Anchor Point sphere
  vtkNew<vtkSphereSource> sphereAnchorPoint;
  sphereAnchorPoint->SetPhiResolution(19);
  sphereAnchorPoint->SetThetaResolution(19);
  sphereAnchorPoint->SetRadius(3);
  vtkNew<vtkPolyDataMapper> polyDataMapper;
  polyDataMapper->SetInputConnection(functionSource->GetOutputPort());
  // Actor
  vtkNew<vtkActor> splineFuncActor;
  splineFuncActor->SetMapper(polyDataMapper);
  splineFuncActor->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());
  splineFuncActor->GetProperty()->SetRepresentationToWireframe();
  splineFuncActor->GetProperty()->SetLineWidth(3.0);
  splineFuncActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
  splineFuncActor->GetMapper()->SetInputConnection(functionSource->GetOutputPort());
  vtkNew<vtkPolyLine> polyLine;
  polyLine->GetPointIds()->SetNumberOfIds(m_combinedPointSet.GetPointer()->GetSize());
  for (unsigned int i = 0; i < m_combinedPointSet.GetPointer()->GetSize(); i++)
  {
    polyLine->GetPointIds()->SetId(i, i);
  }

  // Create a cell array to store the lines in and add the lines to it
  vtkNew<vtkCellArray> cells;
  cells->InsertNextCell(polyLine);
  // Create a polydata to store everything in and update
  vtkNew<vtkPolyData> polyData;
  polyData->SetPoints(m_SplineVisualisationEvalUtil->ConvertMITKIntoVTKPointset(m_combinedPointSet));
  polyData->SetLines(cells);
  polyData->UpdatePointGhostArrayCache();
  vtkNew<vtkGlyph3DMapper> pointMapper;
  pointMapper->SetInputData(polyData);
  pointMapper->SetSourceConnection(sphereAnchorPoint->GetOutputPort());

  vtkNew<vtkActor> sphereAnchorPointActor;
  sphereAnchorPointActor->SetMapper(pointMapper);
  sphereAnchorPointActor->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());
  sphereAnchorPointActor->GetProperty()->SetColor(1.0, 1.0, 0.0);

  vtkNew<vtkActor> polydataActor;
  polydataActor->SetMapper(polyDataMapper);
  polydataActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
  polydataActor->GetProperty()->SetRepresentationToWireframe();
  polydataActor->GetProperty()->SetLineWidth(5.0);
  polydataActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

  // make DataNode and PointSet as Data insertion
  mitk::DataNode::Pointer Referencepointsdatanode = mitk::DataNode::New();
  Referencepointsdatanode->SetName("Spline Function");
  Referencepointsdatanode->SetData(m_SplineReferencePointSet);
  Referencepointsdatanode->SetProperty("name", mitk::StringProperty::New("Spline Function"));
  Referencepointsdatanode->SetProperty("pointsize", mitk::FloatProperty::New(4.5));
  Referencepointsdatanode->SetProperty("line width", mitk::IntProperty::New(5));
  Referencepointsdatanode->SetColor(1.0, 1.0, 0.0);
  Referencepointsdatanode->SetVisibility(true);

  if (m_ShowVTKSubwindow)
  {
    ExecuteVtkRendering(colors.Get(), polydataActor.Get(), splineFuncActor.Get(), sphereAnchorPointActor.Get(), textSource);
  }
 
  return Referencepointsdatanode;
 }
 

 mitk::DataNode::Pointer SplineDataObjectVisualizationFilter::ConvertBSplineRepresentation(mitk::PointSet::Pointer AnchorPointSet)
 {
   m_combinedPointSet = GettingCombinedPointset(AnchorPointSet);

  //TODO apply vtkImageBSplineInterpolator or vtkBSplineTransform 
  vtkNew<vtkNamedColors> colors;
  vtkSPtrNew(textSource, vtkTextActor);
  vtkSmartPointer<vtkPoints> spoints = vtkSmartPointer<vtkPoints>::New();
  spoints->SetNumberOfPoints(10);
  spoints->SetPoint(0, 0.000, 0.000, 0.500);
  spoints->SetPoint(1, 0.000, 0.000, -0.500);
  spoints->SetPoint(2, 0.433, 0.000, 0.250);
  spoints->SetPoint(3, 0.433, 0.000, -0.250);
  spoints->SetPoint(4, -0.000, 0.433, 0.250);
  spoints->SetPoint(5, -0.000, 0.433, -0.250);
  spoints->SetPoint(6, -0.433, -0.000, 0.250);
  spoints->SetPoint(7, -0.433, -0.000, -0.250);
  spoints->SetPoint(8, 0.000, -0.433, 0.250);
  spoints->SetPoint(9, 0.000, -0.433, -0.250);

   vtkPoints *tempPoints = m_SplineVisualisationEvalUtil->ConvertMITKIntoVTKPointset(AnchorPointSet);

   vtkSmartPointer<vtkThinPlateSplineTransform> thin = vtkSmartPointer<vtkThinPlateSplineTransform>::New();
   thin->SetSourceLandmarks(spoints);
   thin->SetTargetLandmarks(tempPoints);
   thin->SetBasisToR2LogR();

   vtkSmartPointer<vtkTransformToGrid> transformToGrid = vtkSmartPointer<vtkTransformToGrid>::New();
   transformToGrid->SetInput(thin);
   transformToGrid->SetGridOrigin(-1.5, -1.5, -1.5);
   transformToGrid->SetGridExtent(0, 60, 0, 60, 0, 60);
   transformToGrid->SetGridSpacing(0.05, 0.05, 0.05);

   vtkSmartPointer<vtkImageBSplineCoefficients> coeffs = vtkSmartPointer<vtkImageBSplineCoefficients>::New();
   coeffs->SetInputConnection(transformToGrid->GetOutputPort());
   coeffs->UpdateWholeExtent();

   vtkSmartPointer<vtkBSplineTransform> bsplinetransform = vtkSmartPointer<vtkBSplineTransform>::New();
   bsplinetransform->SetCoefficientConnection(coeffs->GetOutputPort());
   bsplinetransform->SetCoefficientData(coeffs->GetOutput());
   bsplinetransform->SetDisplacementScale(0.5);
   bsplinetransform->SetBorderModeToZero();
   // invert the transform before passing it to vtkImageReslice
   bsplinetransform->Inverse();
   vtkNew<vtkParametricSpline> parametricspline;
   parametricspline->SetPoints(m_SplineVisualisationEvalUtil->ConvertMITKIntoVTKPointset(AnchorPointSet));
   
   // get evaluated points
   m_SplineReferencePointSet = m_SplineVisualisationEvalUtil->GenerateSplinePoints(parametricspline, AnchorPointSet, m_CurrentRefDoubleSelection);

   vtkNew<vtkCellArray> lines;
   auto polyData = vtkSmartPointer<vtkPolyData>::New();
   lines->InsertNextCell(tempPoints->GetNumberOfPoints());
   for (unsigned int i = 0; i < tempPoints->GetNumberOfPoints(); ++i)
   {
     lines->InsertCellPoint(i);
   }
   polyData->SetPoints(tempPoints);
   polyData->SetLines(lines);
   polyData->UpdatePointGhostArrayCache();

   vtkSmartPointer<vtkTransformPolyDataFilter> transformpolydatafilter =
   vtkSmartPointer<vtkTransformPolyDataFilter>::New();
   transformpolydatafilter->SetInputData(polyData);
   transformpolydatafilter->SetTransform(bsplinetransform);

   // create a loop to evaluate the parametric coordinate u from [0,1] @ u[0], u[1], u[2],
   // Using void Evaluate(double u[3], double Pt[3], double Du[9]) override
   TempString = m_SplineVisualisationEvalUtil->VtkQStringTextBlock(AnchorPointSet, m_SplineReferencePointSet);
   textSource->SetInput(TempString.toLocal8Bit().data());
   textSource->GetTextProperty()->SetFontSize(18);
   textSource->GetTextProperty()->SetJustificationToLeft();
   textSource->GetTextProperty()->SetVerticalJustificationToBottom();
   textSource->GetTextProperty()->BoldOn();
   textSource->GetTextProperty()->SetColor(1, 1, 1);

   // Setup Anchor Point sphere
   vtkNew<vtkSphereSource> sphereAnchorPoint;
   sphereAnchorPoint->SetPhiResolution(19);
   sphereAnchorPoint->SetThetaResolution(19);
   sphereAnchorPoint->SetRadius(3);
   vtkNew<vtkPolyDataMapper> polyDataMapper;
   polyDataMapper->SetInputConnection(transformpolydatafilter->GetOutputPort());
   // Actor
   vtkNew<vtkActor> splineFuncActor;
   splineFuncActor->SetMapper(polyDataMapper);
   splineFuncActor->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());
   splineFuncActor->GetProperty()->SetRepresentationToWireframe();
   splineFuncActor->GetProperty()->SetLineWidth(3.0);
   splineFuncActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
   splineFuncActor->GetMapper()->SetInputConnection(transformpolydatafilter->GetOutputPort());

   vtkNew<vtkPolyLine> polyLine;
   polyLine->GetPointIds()->SetNumberOfIds(AnchorPointSet.GetPointer()->GetSize());
   for (unsigned int i = 0; i < AnchorPointSet.GetPointer()->GetSize(); i++)
   {
     polyLine->GetPointIds()->SetId(i, i);
   }
   // Create a cell array to store the lines in and add the lines to it
   vtkNew<vtkCellArray> cells;
   cells->InsertNextCell(polyLine);
   // Create a point Glyph3DMapper
   vtkNew<vtkGlyph3DMapper> pointMapper;
   pointMapper->SetInputData(polyData);
   pointMapper->SetSourceConnection(sphereAnchorPoint->GetOutputPort());

   vtkNew<vtkActor> sphereAnchorPointActor;
   sphereAnchorPointActor->SetMapper(pointMapper);
   sphereAnchorPointActor->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());
   sphereAnchorPointActor->GetProperty()->SetColor(1.0, 1.0, 0.0);

   vtkNew<vtkActor> polydataActor;
   polydataActor->SetMapper(polyDataMapper);
   polydataActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
   polydataActor->GetProperty()->SetRepresentationToWireframe();
   polydataActor->GetProperty()->SetLineWidth(5.0);
   polydataActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
   // make DataNode and PointSet as Data insertion
   mitk::DataNode::Pointer convertedDataNode = mitk::DataNode::New();
   convertedDataNode->SetName("Spline Function");
   convertedDataNode->SetData(m_SplineReferencePointSet);
   convertedDataNode->SetProperty("name", mitk::StringProperty::New("Spline Function"));
   convertedDataNode->SetProperty("pointsize", mitk::FloatProperty::New(4.5));
   convertedDataNode->SetProperty("line width", mitk::IntProperty::New(3));
   convertedDataNode->SetColor(1.0, 1.0, 0.0);
   convertedDataNode->SetVisibility(true);
   // convert Datanode
   SplineVtkMapper->SetSplineResolution(1000);
   convertedDataNode->SetMapper(mitk::BaseRenderer::StandardMapperSlot::Standard3D, SplineVtkMapper);
   if (m_ShowVTKSubwindow)
   {
     ExecuteVtkRendering(
       colors.Get(), polydataActor.Get(), splineFuncActor.Get(), sphereAnchorPointActor.Get(), textSource);
   }
   return convertedDataNode;
 }



 mitk::DataNode::Pointer SplineDataObjectVisualizationFilter::ConvertSCurveRepresentation(mitk::PointSet::Pointer AnchorPointSet)
 {
   m_combinedPointSet = GettingCombinedPointset(AnchorPointSet);

   vtkNew<vtkNamedColors> colors;
   vtkSPtrNew(textSource, vtkTextActor);

   // the spline class Range with absolute values
   vtkNew<vtkSCurveSpline> xSpline;
   vtkNew<vtkSCurveSpline> ySpline;
   vtkNew<vtkSCurveSpline> zSpline;

   vtkNew<vtkParametricSpline> parametricspline;
   parametricspline->SetXSpline(xSpline);
   parametricspline->SetYSpline(ySpline);
   parametricspline->SetZSpline(zSpline);
   vtkPoints *tempPoints = m_SplineVisualisationEvalUtil->ConvertMITKIntoVTKPointset(m_combinedPointSet);
   parametricspline->SetPoints(tempPoints);

   vtkNew<vtkParametricFunctionSource> functionSource;
   functionSource->SetParametricFunction(parametricspline);
   functionSource->SetUResolution(500);
   functionSource->SetVResolution(500);
   functionSource->SetWResolution(500);
   functionSource->Update();

   m_SplineReferencePointSet = m_SplineVisualisationEvalUtil->GenerateSplinePoints(
     parametricspline, m_combinedPointSet, m_CurrentRefDoubleSelection);

   vtkNew<vtkCellArray> lines;
   auto polyData = vtkSmartPointer<vtkPolyData>::New();
   lines->InsertNextCell(tempPoints->GetNumberOfPoints());
   for (unsigned int i = 0; i < tempPoints->GetNumberOfPoints(); ++i)
   {
     lines->InsertCellPoint(i);
   }
   polyData->SetPoints(tempPoints);
   polyData->SetLines(lines);
   polyData->UpdatePointGhostArrayCache();
   // create a loop to evaluate the parametric coordinate u from [0,1] @ u[0], u[1], u[2],
   // Using void Evaluate(double u[3], double Pt[3], double Du[9]) override
   TempString = m_SplineVisualisationEvalUtil->VtkQStringTextBlock(m_combinedPointSet, m_SplineReferencePointSet);
   textSource->SetInput(TempString.toLocal8Bit().data());
   textSource->GetTextProperty()->SetFontSize(18);
   textSource->GetTextProperty()->SetJustificationToLeft();
   textSource->GetTextProperty()->SetVerticalJustificationToBottom();
   textSource->GetTextProperty()->BoldOn();
   textSource->GetTextProperty()->SetColor(1, 1, 1);

   // Parametric function for spline
   vtkNew<vtkSplineFilter> splineFilter;
   splineFilter->SetInputData(polyData);
   splineFilter->SetNumberOfSubdivisions(polyData->GetNumberOfPoints() * 10);
   splineFilter->SetSpline(xSpline);
   // Setup Anchor Point sphere
   vtkNew<vtkSphereSource> sphereAnchorPoint;
   sphereAnchorPoint->SetPhiResolution(19);
   sphereAnchorPoint->SetThetaResolution(19);
   sphereAnchorPoint->SetRadius(3);
   vtkNew<vtkPolyDataMapper> polyDataMapper;
   polyDataMapper->SetInputConnection(splineFilter->GetOutputPort());
   // Actor
   vtkNew<vtkActor> splineFuncActor;
   splineFuncActor->SetMapper(polyDataMapper);
   splineFuncActor->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());
   splineFuncActor->GetProperty()->SetRepresentationToWireframe();
   splineFuncActor->GetProperty()->SetLineWidth(3.0);
   splineFuncActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
   splineFuncActor->GetMapper()->SetInputConnection(splineFilter->GetOutputPort());

   vtkNew<vtkPolyLine> polyLine;
   polyLine->GetPointIds()->SetNumberOfIds(m_combinedPointSet.GetPointer()->GetSize());
   for (unsigned int i = 0; i < m_combinedPointSet.GetPointer()->GetSize(); i++)
   {
     polyLine->GetPointIds()->SetId(i, i);
   }
   // Create a cell array to store the lines in and add the lines to it
   vtkNew<vtkCellArray> cells;
   cells->InsertNextCell(polyLine);
   // Create a point Glyph3DMapper
   vtkNew<vtkGlyph3DMapper> pointMapper;
   pointMapper->SetInputData(polyData);
   pointMapper->SetSourceConnection(sphereAnchorPoint->GetOutputPort());

   vtkNew<vtkActor> sphereAnchorPointActor;
   sphereAnchorPointActor->SetMapper(pointMapper);
   sphereAnchorPointActor->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());
   sphereAnchorPointActor->GetProperty()->SetColor(1.0, 1.0, 0.0);

   vtkNew<vtkActor> polydataActor;
   polydataActor->SetMapper(polyDataMapper);
   polydataActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
   polydataActor->GetProperty()->SetRepresentationToWireframe();
   polydataActor->GetProperty()->SetLineWidth(5.0);
   polydataActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
   // make DataNode and PointSet as Data insertion
   mitk::DataNode::Pointer convertedDataNode = mitk::DataNode::New();
   convertedDataNode->SetName("Spline Function");
   convertedDataNode->SetData(m_SplineReferencePointSet);
   convertedDataNode->SetProperty("name", mitk::StringProperty::New("Spline Function"));
   convertedDataNode->SetProperty("pointsize", mitk::FloatProperty::New(4.5));
   convertedDataNode->SetProperty("line width", mitk::IntProperty::New(3));
   convertedDataNode->SetColor(1.0, 1.0, 0.0);
   convertedDataNode->SetVisibility(true);
   if (m_ShowVTKSubwindow)
   {
     ExecuteVtkRendering(
       colors.Get(), polydataActor.Get(), splineFuncActor.Get(), sphereAnchorPointActor.Get(), textSource);
   }
   return convertedDataNode;
 }


 mitk::DataNode::Pointer SplineDataObjectVisualizationFilter::ConvertCardinalSplineRepresentation(mitk::PointSet::Pointer AnchorPointSet)
{
   m_combinedPointSet = GettingCombinedPointset(AnchorPointSet);

   vtkNew<vtkNamedColors> colors;
   vtkSPtrNew(textSource, vtkTextActor);

   // the spline class Range with absolute values
   vtkNew<vtkCardinalSpline> xSpline;
   vtkNew<vtkCardinalSpline> ySpline;
   vtkNew<vtkCardinalSpline> zSpline;

   vtkNew<vtkParametricSpline> parametricspline;
   parametricspline->SetXSpline(xSpline);
   parametricspline->SetYSpline(ySpline);
   parametricspline->SetZSpline(zSpline);
   vtkPoints *tempPoints = m_SplineVisualisationEvalUtil->ConvertMITKIntoVTKPointset(m_combinedPointSet);
   parametricspline->SetPoints(tempPoints);

   vtkNew<vtkParametricFunctionSource> functionSource;
   functionSource->SetParametricFunction(parametricspline);
   functionSource->SetUResolution(500);
   functionSource->SetVResolution(500);
   functionSource->SetWResolution(500);
   functionSource->Update();

   m_SplineReferencePointSet = m_SplineVisualisationEvalUtil->GenerateSplinePoints(
     parametricspline, m_combinedPointSet, m_CurrentRefDoubleSelection);

   vtkNew<vtkCellArray> lines;
   auto polyData = vtkSmartPointer<vtkPolyData>::New();
   lines->InsertNextCell(tempPoints->GetNumberOfPoints());
   for (unsigned int i = 0; i < tempPoints->GetNumberOfPoints(); ++i)
   {
     lines->InsertCellPoint(i);
   }
   polyData->SetPoints(tempPoints);
   polyData->SetLines(lines);
   polyData->UpdatePointGhostArrayCache();
   // create a loop to evaluate the parametric coordinate u from [0,1] @ u[0], u[1], u[2],
   // Using void Evaluate(double u[3], double Pt[3], double Du[9]) override
   TempString = m_SplineVisualisationEvalUtil->VtkQStringTextBlock(m_combinedPointSet, m_SplineReferencePointSet);
   textSource->SetInput(TempString.toLocal8Bit().data());
   textSource->GetTextProperty()->SetFontSize(18);
   textSource->GetTextProperty()->SetJustificationToLeft();
   textSource->GetTextProperty()->SetVerticalJustificationToBottom();
   textSource->GetTextProperty()->BoldOn();
   textSource->GetTextProperty()->SetColor(1, 1, 1);

   // Parametric function for spline
   vtkNew<vtkSplineFilter> splineFilter;
   splineFilter->SetInputData(polyData);
   splineFilter->SetNumberOfSubdivisions(polyData->GetNumberOfPoints() * 10);
   splineFilter->SetSpline(xSpline);
   // Setup Anchor Point sphere
   vtkNew<vtkSphereSource> sphereAnchorPoint;
   sphereAnchorPoint->SetPhiResolution(19);
   sphereAnchorPoint->SetThetaResolution(19);
   sphereAnchorPoint->SetRadius(3);
   vtkNew<vtkPolyDataMapper> polyDataMapper;
   polyDataMapper->SetInputConnection(splineFilter->GetOutputPort());
   // Actor
   vtkNew<vtkActor> splineFuncActor;
   splineFuncActor->SetMapper(polyDataMapper);
   splineFuncActor->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());
   splineFuncActor->GetProperty()->SetRepresentationToWireframe();
   splineFuncActor->GetProperty()->SetLineWidth(3.0);
   splineFuncActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
   splineFuncActor->GetMapper()->SetInputConnection(splineFilter->GetOutputPort());

   vtkNew<vtkPolyLine> polyLine;
   polyLine->GetPointIds()->SetNumberOfIds(m_combinedPointSet.GetPointer()->GetSize());
   for (unsigned int i = 0; i < m_combinedPointSet.GetPointer()->GetSize(); i++)
   {
     polyLine->GetPointIds()->SetId(i, i);
   }
   // Create a cell array to store the lines in and add the lines to it
   vtkNew<vtkCellArray> cells;
   cells->InsertNextCell(polyLine);
   // Create a point Glyph3DMapper
   vtkNew<vtkGlyph3DMapper> pointMapper;
   pointMapper->SetInputData(polyData);
   pointMapper->SetSourceConnection(sphereAnchorPoint->GetOutputPort());

   vtkNew<vtkActor> sphereAnchorPointActor;
   sphereAnchorPointActor->SetMapper(pointMapper);
   sphereAnchorPointActor->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());
   sphereAnchorPointActor->GetProperty()->SetColor(1.0, 1.0, 0.0);

   vtkNew<vtkActor> polydataActor;
   polydataActor->SetMapper(polyDataMapper);
   polydataActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
   polydataActor->GetProperty()->SetRepresentationToWireframe();
   polydataActor->GetProperty()->SetLineWidth(5.0);
   polydataActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
   // make DataNode and PointSet as Data insertion
   mitk::DataNode::Pointer convertedDataNode = mitk::DataNode::New();
   convertedDataNode->SetName("Spline Function");
   convertedDataNode->SetData(m_SplineReferencePointSet);
   convertedDataNode->SetProperty("pointsize", mitk::FloatProperty::New(4.5));
   convertedDataNode->SetProperty("line width", mitk::IntProperty::New(3));
   convertedDataNode->SetColor(1.0, 1.0, 0.0);
   convertedDataNode->SetVisibility(true);
   // convert Datanode
   if (m_ShowVTKSubwindow)
   {
     ExecuteVtkRendering(
       colors.Get(), polydataActor.Get(), splineFuncActor.Get(), sphereAnchorPointActor.Get(), textSource);
   }
   return convertedDataNode;
 }


mitk::DataNode::Pointer SplineDataObjectVisualizationFilter::ConvertKochanekSplineRepresentation(mitk::PointSet::Pointer AnchorPointSet, bool optimizsation)
 {
  m_combinedPointSet = GettingCombinedPointset(AnchorPointSet);

  vtkNew<vtkNamedColors> colors;
  vtkSPtrNew(textSource, vtkTextActor);
  TempString = "";
  for (auto iter = AnchorPointSet->Begin(); iter != AnchorPointSet->End(); iter++)
  {
    TempString.append("Anchor Point" + QString::number(iter.Index()) + " x: " + QString::number(iter.Value()[0]) +
                      " y: " + QString::number(iter.Value()[1]) + " z: " + QString::number(iter.Value()[2]) + "\n");
  }
  TempString.append("\n");
  // parametric spline class
  vtkNew<vtkKochanekSpline> xSpline;
  vtkNew<vtkKochanekSpline> ySpline;
  vtkNew<vtkKochanekSpline> zSpline;
  vtkNew<vtkParametricSpline> parametricspline;
  parametricspline->SetXSpline(xSpline);
  parametricspline->SetYSpline(ySpline);
  parametricspline->SetZSpline(zSpline);
  parametricspline->SetPoints(m_SplineVisualisationEvalUtil->ConvertMITKIntoVTKPointset(m_combinedPointSet));

  vtkNew<vtkParametricFunctionSource> functionSource;
  functionSource->SetParametricFunction(parametricspline);
  functionSource->SetUResolution(500);
  functionSource->SetVResolution(500);
  functionSource->SetWResolution(500);
  functionSource->Update();
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  vtkNew<vtkSplineWidget2> splineWidget;
  vtkNew<vtkSplineRepresentation> splinerepresent;
  splineWidget->SetRepresentation(splinerepresent);
  splineWidget->SetInteractor(renderWindowInteractor);
  splineWidget->SetPriority(1.0);
  splineWidget->KeyPressActivationOff();

  // create a  loop to evaluate the kochanek coordinate u from [0,1] u[0] , u[1], u[2],
  // Using void Evaluate(double t) override;
  if (optimizsation)
  {
    m_SplineReferencePointSet = m_SplineVisualisationEvalUtil->GetKochanekOptimisations( parametricspline, m_combinedPointSet, m_CurrentRefDoubleSelection);
  }
  else
  {
    m_SplineReferencePointSet = m_SplineVisualisationEvalUtil->GenerateSplinePoints(
      parametricspline, m_combinedPointSet, m_CurrentRefDoubleSelection);
  }


  textSource->SetInput(TempString.toLocal8Bit().data());
  textSource->GetTextProperty()->SetFontSize(18);
  textSource->GetTextProperty()->SetJustificationToLeft();
  textSource->GetTextProperty()->SetVerticalJustificationToBottom();
  textSource->GetTextProperty()->BoldOn();
  textSource->GetTextProperty()->SetColor(1, 1, 1);

  // kochanek function for spline
  // Setup Anchor Point sphere
  vtkNew<vtkSphereSource> sphereAnchorPoint;
  sphereAnchorPoint->SetPhiResolution(19);
  sphereAnchorPoint->SetThetaResolution(19);
  sphereAnchorPoint->SetRadius(3);
  vtkNew<vtkPolyDataMapper> polyDataMapper;
  polyDataMapper->SetInputConnection(functionSource->GetOutputPort());
  // Actor
  vtkNew<vtkActor> splineFuncActor;
  splineFuncActor->SetMapper(polyDataMapper);
  splineFuncActor->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());
  splineFuncActor->GetProperty()->SetRepresentationToWireframe();
  splineFuncActor->GetProperty()->SetLineWidth(3.0);
  splineFuncActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
  splineFuncActor->GetMapper()->SetInputConnection(functionSource->GetOutputPort());
  // Create a polydata to store everything in and update
  vtkNew<vtkPolyData> polyData;
  polyData->SetPoints(m_SplineVisualisationEvalUtil->ConvertMITKIntoVTKPointset(m_combinedPointSet));
  polyData->UpdatePointGhostArrayCache();
  vtkNew<vtkGlyph3DMapper> pointMapper;
  pointMapper->SetInputData(polyData);
  pointMapper->SetSourceConnection(sphereAnchorPoint->GetOutputPort());

  vtkNew<vtkActor> sphereAnchorPointActor;
  sphereAnchorPointActor->SetMapper(pointMapper);
  sphereAnchorPointActor->GetProperty()->SetColor(colors->GetColor3d("Yellow").GetData());
  sphereAnchorPointActor->GetProperty()->SetColor(1.0, 1.0, 0.0);

  vtkNew<vtkActor> polydataActor;
  polydataActor->SetMapper(polyDataMapper);
  polydataActor->GetProperty()->SetColor(colors->GetColor3d("Red").GetData());
  polydataActor->GetProperty()->SetRepresentationToWireframe();
  polydataActor->GetProperty()->SetLineWidth(5.0);
  polydataActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

  // make DataNode and PointSet as Data insertion
  mitk::DataNode::Pointer convertedDataNode = mitk::DataNode::New();
  convertedDataNode->SetName("Spline Function");
  convertedDataNode->SetData(m_SplineReferencePointSet);
  convertedDataNode->SetProperty("pointsize", mitk::FloatProperty::New(4.5));
  convertedDataNode->SetProperty("line width", mitk::IntProperty::New(3));
  convertedDataNode->SetColor(1.0, 1.0, 0.0);
  convertedDataNode->SetVisibility(true);

  if (m_ShowVTKSubwindow)
  {
    ExecuteVtkRendering(
      colors.Get(), polydataActor.Get(), splineFuncActor.Get(), sphereAnchorPointActor.Get(), textSource);
  }

  return convertedDataNode;
  }



 mitk::DataNode::Pointer SplineDataObjectVisualizationFilter::Convert3DTubeRepresentation(mitk::PointSet::Pointer AnchorPointSet)
  {
   m_combinedPointSet = GettingCombinedPointset(AnchorPointSet);
   vtkNew<vtkNamedColors> colors;
  vtkNew<vtkParametricSpline> parametricspline;
   parametricspline->SetPoints(m_SplineVisualisationEvalUtil->ConvertMITKIntoVTKPointset(m_combinedPointSet));
  // get evaluated points
  m_SplineReferencePointSet = m_SplineVisualisationEvalUtil->GenerateSplinePoints(
     parametricspline, m_combinedPointSet, m_CurrentRefDoubleSelection);
 
   vtkNew<vtkParametricFunctionSource> functionSource;
  functionSource->SetParametricFunction(parametricspline);
   functionSource->SetUResolution(10 * m_SplineReferencePointSet->GetSize());
   functionSource->Update();

   // Interpolate the scalars
   vtkSmartPointer<vtkTupleInterpolator> interpolatedRadius = vtkSmartPointer<vtkTupleInterpolator>::New();
   interpolatedRadius->SetInterpolationTypeToLinear();
   interpolatedRadius->SetNumberOfComponents(1);
   interpolatedRadius = m_SplineVisualisationEvalUtil->GetInterpolatedRadiusValuesFromTupleInterpolator(
     interpolatedRadius, m_SplineReferencePointSet);

   // Generate the radius scalars
   vtkNew<vtkDoubleArray> tubeRadius;
   unsigned int n = functionSource->GetOutput()->GetNumberOfPoints();
   tubeRadius->SetNumberOfTuples(n);
   tubeRadius->SetName("TubeRadius");
   auto tMin = interpolatedRadius->GetMinimumT();
   auto tMax = interpolatedRadius->GetMaximumT();
   double fixedRadius = .1;
   double t;
   for (int index = 0; index <= n; index++)
   {
     t = (tMax - tMin) / (n - 1) * index + tMin;
     interpolatedRadius->InterpolateTuple(t, &fixedRadius);
     tubeRadius->SetTuple1(index, fixedRadius);
   }
   
   // Add the scalars to the polydata
   vtkSmartPointer tubePolyData = functionSource->GetOutput();
   tubePolyData->GetPointData()->AddArray(tubeRadius);
   tubePolyData->GetPointData()->SetActiveScalars("TubeRadius");

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

   vtkSmartPointer<vtkImageData> tubeImageData = m_SplineVisualisationEvalUtil->convertVTKPolydataToVTKImagedata(
     tubePolyData,
     {static_cast<int>(tubeRadius->GetNumberOfTuples()),
      static_cast<int>(tubeRadius->GetNumberOfTuples()),
      static_cast<int>(tubeRadius->GetNumberOfTuples())},
     1);

   vtkNew<vtkTexture> tubeTexture;
   tubeTexture->SetInputData(tubeImageData);
   tubeTexture->SetColorModeToDirectScalars();
   vtkNew<vtkProperty> tubeColorProp;
   tubeColorProp->SetBaseColorTexture(tubeTexture); //  Bug: The albedoTex texture is not in sRGB color space.

   // generate texture coordinates assuming that surface and texture can be mapped 1 to 1
   vtkSmartPointer<vtkFloatArray> textureCoordinates = vtkSmartPointer<vtkFloatArray>::New();
   textureCoordinates->SetNumberOfComponents(tubeRadius->GetNumberOfComponents());
   textureCoordinates->SetNumberOfTuples(2);
   textureCoordinates->SetName("TextureCoordinates");

   mitk::Image::Pointer textureImage = mitk::Image::New();
   textureImage->Initialize(tubeImageData);

   // generate texture coordinates assuming that surface and texture can be mapped 1 to 1
   int sizeTexImg = sizeof(textureImage->GetDimensions()) / sizeof(textureImage->GetDimensions()[0]);
   std::vector<int> dims(textureImage->GetDimensions(), textureImage->GetDimensions() + sizeTexImg);
   int pixelID;
   float xNorm; 
     for (unsigned int i = 0; i < dims[0]; ++i)
     {
       pixelID = i + dims[0];
       xNorm = (((float)i) / dims[0]);
       textureCoordinates->InsertTuple1(pixelID, xNorm);
     }
   
   // Generate MITK surface
   mitk::Surface::Pointer surfaceToPutTextureOn = mitk::Surface::New();
   surfaceToPutTextureOn->SetVtkPolyData(tuber->GetOutput());
   surfaceToPutTextureOn->GetVtkPolyData()->GetPointData()->SetTCoords(textureCoordinates);
   // surfaceToPutTextureOn->SetProperty("Surface.Texture", textureProperty);
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
   // Generate a node
   mitk::DataNode::Pointer surfaceNode = mitk::DataNode::New();
   surfaceNode->SetData(surfaceToPutTextureOn);
   surfaceNode->SetProperty("point size", mitk::FloatProperty::New(5));
   surfaceNode->SetProperty("color mode", mitk::BoolProperty::New(true));
   surfaceNode->SetProperty("scalar visibility", mitk::BoolProperty::New(true));
   surfaceNode->SetProperty("line width", mitk::FloatProperty::New(4.5));
   surfaceNode->SetVisibility(true);
   surfaceNode->SetName("3D Tube Datanode");
   surfaceNode->Update();

  if (m_ShowVTKSubwindow)
   {
     /*ExecuteVtkRendering(
       colors.Get(), polydataActor.Get(), 
         splineFuncActor.Get(), sphereAnchorPointActor.Get(), NULL);
   */
   }
  

  return surfaceNode;
  }

  mitk::PointSet::Pointer SplineDataObjectVisualizationFilter::GettingCombinedPointset(
    mitk::PointSet::Pointer AnchorPointSet)
  {
    if (m_appendPointset)
    {
      auto iterlast = m_lastAnchorPointSet.GetPointer()->Begin();
      int startlastAnchorpointsize = m_lastAnchorPointSet->GetSize();

      //123123
      for (int i = 0; i != startlastAnchorpointsize;++i)
      {
        if (iterlast != m_lastAnchorPointSet.GetPointer()->End())
        {
          m_lastAnchorPointSet.GetPointer()->SetPoint(i + startlastAnchorpointsize, iterlast.Value());
          iterlast++;
        }
      }
      //123456
       for (auto iter = AnchorPointSet.GetPointer()->Begin(); iter != AnchorPointSet.GetPointer()->End(); ++iter)
      {
        m_lastAnchorPointSet.GetPointer()->SetPoint(iter.Index(), iter.Value());
      }

 
      return m_lastAnchorPointSet;
    }
    else
    {
      m_lastAnchorPointSet->Clear();
     return AnchorPointSet;
      
    }
  }

 void SplineDataObjectVisualizationFilter::ExecuteVtkRendering(vtkSmartPointer<vtkNamedColors> colors,
                                                                vtkActor *polydataActor,
                                                                vtkActor *splineFuncActor,
                                                                vtkActor *sphereAnchorPointActor, 
                                                                vtkSmartPointer<vtkTextActor> textSource)
  {
   // reset window
   if (m_ResetRenderer)
   {
     VtkrenderWindow->RemoveRenderer(Vtkrenderer);
     Vtkrenderer->New();
     Vtkrenderer->SetBackground(colors->GetColor3d("Black").GetData());
     Vtkrenderer->RemoveAllViewProps();
     Vtkrenderer->ResetCamera();
     VtkrenderWindow->AddRenderer(Vtkrenderer);
     VtkrenderWindow->Render();
   }
   // initialise window renderer
   Vtkrenderer->New();
   VtkrenderWindow->SetDesiredUpdateRate(60.0);
   VtkrenderWindow->SetWindowName("Spline Visualisation");
   Vtkrenderer->AddActor(polydataActor);
   Vtkrenderer->AddActor(splineFuncActor);
   Vtkrenderer->AddActor(sphereAnchorPointActor);
   Vtkrenderer->AddActor(textSource);
   Vtkrenderer->SetBackground(colors->GetColor3d("Black").GetData());
   VtkrenderWindow->SetSize(1000, 1000);
   VtkrenderWindow->AddRenderer(Vtkrenderer);
   // vtkRenderWindow Interactor
   // Render the scene
   int *winSize = VtkrenderWindow->GetSize();
   double size[2] = {0};
   textSource->GetSize(Vtkrenderer, size);
   textSource->SetPosition(winSize[0] - size[0] - 30, winSize[1] - size[1] - 10);
   VtkrenderWindow->Render();
   m_ResetRenderer = true;
 }


mitk::BaseData::Pointer SplineDataObjectVisualizationFilter::GetRepresentationObject(unsigned int idx) const
{
  auto iter = m_RepresentationVectorMap.find(idx);
  if (iter != m_RepresentationVectorMap.end())
    return iter->second.at(0);

  return nullptr;
}


 void SplineDataObjectVisualizationFilter::SetRepresentationObject(unsigned int idx, mitk::BaseData::Pointer data)
{
  std::vector<mitk::BaseData::Pointer> dataVector;
  dataVector.push_back(data);
  this->SetRepresentationObjects(idx, dataVector);
}

 void SplineDataObjectVisualizationFilter::SetRepresentationObjects(unsigned int idx, const std::vector<mitk::BaseData::Pointer> &data)
{
  m_RepresentationVectorMap[idx] = data;
}

void SplineDataObjectVisualizationFilter::FreezeUnFreezeStandartView(bool freeze, int currentSensoryIndex)
{
  m_Worker = new QmitkMITKIGTTrackingToolboxViewWorker;
  Emtracker = mitk::VirtualTrackingDevice::New();
  m_Worker->SetWorkerMethod(QmitkMITKIGTTrackingToolboxViewWorker::eConnectDevice);
  m_Worker->SetTrackingDevice(Emtracker);
  m_ToolStorage = mitk::NavigationToolStorage::New();
  m_ToolStorage->SetName("TrackingToolbox Default Storage");
  m_ToolStorage->RegisterAsMicroservice();
  m_Worker->SetNavigationToolStorage(m_ToolStorage);
  if (freeze)
  {
    m_Worker->GetTrackingDeviceSource()->Freeze();
    Emsource->Disconnect();
    EmTool0.GetPointer()->Delete();
    EmTool0.GetPointer()->SetDataValid(false);
    if (currentSensoryIndex >= 2)
    {
    // EmTool1.GetPointer()->Delete();
      EmTool1.GetPointer()->SetHasOrientation(false);
      EmTool1.GetPointer()->SetDataValid(false);
    }
    if (currentSensoryIndex >= 3)
    {
      EmTool2.GetPointer()->SetHasOrientation(false);
      EmTool2.GetPointer()->SetDataValid(false);
    }
    if (currentSensoryIndex >= 4)
    {
      EmTool3.GetPointer()->SetHasOrientation(false);
      EmTool3.GetPointer()->SetDataValid(false);
    }
  }
  else
  {
    m_Worker->GetTrackingDeviceSource()->UnFreeze();
    Emsource->Connect();
    EmTool0.GetPointer()->SetHasOrientation(true);
    EmTool0.GetPointer()->SetDataValid(true);
    if (currentSensoryIndex >= 2)
    {
      EmTool1.GetPointer()->SetHasOrientation(true);
      EmTool1.GetPointer()->SetDataValid(true);
    }
    if (currentSensoryIndex >= 3)
    {
      EmTool2.GetPointer()->SetHasOrientation(true);
      EmTool2.GetPointer()->SetDataValid(true);
    }
    if (currentSensoryIndex >= 4)
    {
      EmTool3.GetPointer()->SetHasOrientation(true);
      EmTool3.GetPointer()->SetDataValid(true);
    }
  }
}