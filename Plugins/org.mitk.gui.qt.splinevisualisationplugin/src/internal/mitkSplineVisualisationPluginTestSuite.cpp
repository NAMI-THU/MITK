/*============================================================================
 
The Medical Imaging Interaction Toolkit (MITK)
 
Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.
 
Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.
 
============================================================================*/
 
// Testing
#include "mitkTestingMacros.h"
#include <mitkTestFixture.h>
#include <mitkTestNotRunException.h>
#include <mitkIOUtil.h>
#include <mitkNodePredicateDataType.h>
#include <mitkRenderingTestHelper.h>
#include <mitkSplineVtkMapper3D.h>
#include <vtkFloatArray.h>
// std includes
#include <string>

// MITK includes
#include "internal/SplineVisualisationPlugin.h"
#include "internal/mitkSplineDataObjectVisualizationFilter.h"
#include "internal/mitkSplineVisualisationEvaluationUtil.h"

// VTK includes
#include <cppunit/extensions/HelperMacros.h>

class mitkSplineVisualisationStructureTestSuite : public mitk::TestFixture
{

  CPPUNIT_TEST_SUITE(mitkSplineVisualisationStructureTestSuite);
  // Test the append method
  MITK_TEST(Equal_CloneAndOriginal_GetPointsetData_From_getPointsetQmitkWidget);
  MITK_TEST(Equal_CloneAndOriginal_GetPointsetData_From_GetCurrentPointsetInListWidget);
  MITK_TEST(Render_CommonSplineRenderFunction_StandardViewOutput);
  MITK_TEST(Render_CommonTubeRenderFunction_StandardViewOutput);
  MITK_TEST(Equal_CloneAndOriginal_ConvertParametricSplineRepresentation);
  MITK_TEST(Equal_CloneAndOriginal_ConvertTubeSplineRepresentation);
  MITK_TEST(Return_BaseData_GetRepresentationObject);
  MITK_TEST(Equal_CloneAndOriginal_GetSplineDataNode);
  MITK_TEST(Equal_CloneAndOriginal_GenerateSplinePoints);
  //MITK_TEST(Equal_InputIsNull_ReturnsFalse);
  //MITK_TEST(Equal_DifferentImageGeometry_ReturnsFalse);
  // MITK_TEST(Equal_DifferentPixelTypes_ReturnsFalse);
  CPPUNIT_TEST_SUITE_END();

  

public:
  SplineVisualisationPlugin* m_SplineVisualisationPlugin;
  SplineVisualisationEvaluationUtil* m_SplineVisualisationEvaluationUtil;
  SplineDataObjectVisualizationFilter* m_SplineDataObjectVisualizationFilter;
  itk::SmartPointer<mitk::NavigationToolStorage::Self> m_ToolStorage;
  mitk::StandaloneDataStorage::Pointer m_DataStorage;
  std::string m_DefaultDataString;
  std::string m_PathToPointSet;

  void setUp() override
  {
    m_SplineVisualisationPlugin = new SplineVisualisationPlugin;
    m_SplineVisualisationEvaluationUtil = new SplineVisualisationEvaluationUtil();
    m_ToolStorage = mitk::NavigationToolStorage::New(m_SplineVisualisationPlugin->GetPluginDataStorage());
    m_DataStorage = m_SplineVisualisationPlugin->m_DataStorage;
    m_SplineDataObjectVisualizationFilter = new SplineDataObjectVisualizationFilter(m_ToolStorage, m_DataStorage);
    m_DefaultDataString = "This is the example data content\nAnd a second line\n";
    m_PathToPointSet = GetTestDataFilePath("InteractionTestData/ReferenceData/PointSetDataInteractor_PointsAdd2d3d.mps");

  }

  void tearDown() override
  {
    m_DefaultDataString = "";
    m_SplineVisualisationPlugin = nullptr;
    m_SplineVisualisationEvaluationUtil = nullptr;
    m_ToolStorage = nullptr;
    m_DataStorage = nullptr;
    m_SplineDataObjectVisualizationFilter = nullptr; 
  }
  // SplineVisualizationPlugin Test Methods

  void Equal_CloneAndOriginal_GetPointsetData_From_getPointsetQmitkWidget()
  {

    itk::SmartPointer<mitk::BaseData::Self> testingPointset = mitk::IOUtil::Load(m_PathToPointSet)[0];
    mitk::PointSet::Pointer returnedPointset = m_SplineVisualisationPlugin->getPointsetQmitkWidget(
      m_SplineVisualisationPlugin->m_Controls.EmtrackerPointSetWidget_1->GetSelectedNode());
    ;
    CPPUNIT_ASSERT_MESSAGE("Checking whether the Pointset has been returned correctly.", returnedPointset == testingPointset);
    return;
  }

   void Equal_InputIsNull_GetPointsetData_From_getPointsetQmitkWidget()
  {
     mitk::PointSet::Pointer returnedPointset = m_SplineVisualisationPlugin->getPointsetQmitkWidget(
       m_SplineVisualisationPlugin->m_Controls.EmtrackerPointSetWidget_1->GetSelectedNode());
    
    CPPUNIT_ASSERT_MESSAGE("Checking whether the Pointset is null.", returnedPointset == nullptr);
  }

   void Equal_CloneAndOriginal_GetPointsetData_From_GetCurrentPointsetInListWidget()
  {
    itk::SmartPointer<mitk::BaseData::Self> testingPointset = mitk::IOUtil::Load(m_PathToPointSet)[0];

    mitk::PointSet::Pointer returnedPointset = m_SplineVisualisationPlugin->GetCurrentPointsetInListWidget();
    ;
    CPPUNIT_ASSERT_MESSAGE("Checking whether the Pointset has been returned correctly.", returnedPointset == testingPointset);
  }


  void Render_CommonSplineRenderFunction_StandardViewOutput()
  {
    // modified from https://docs.mitk.org/2016.11/mitkSplineVtkMapper3DTest_8cpp_source.html 
    mitk::DataNode::Pointer testNode = mitk::DataNode::New();
    testNode->SetData(mitk::IOUtil::Load(m_PathToPointSet)[0]);
    mitk::SplineVtkMapper3D::Pointer testMapper = mitk::SplineVtkMapper3D::New();
    testNode->SetMapper(mitk::BaseRenderer::StandardMapperSlot::Standard3D, testMapper);
    char *argv[] = {"parm0", "param1", "param2"};
    int argc = 0;
    std::vector<std::string> m_CommandlineArgs;
    mitk::RenderingTestHelper renderingTestHelper = mitk::RenderingTestHelper(640, 480, argc, argv);
    // disables anti-aliasing which is enabled on several graphics cards and
    // causes problems when doing a pixel-wise comparison to a reference image
    renderingTestHelper.GetVtkRenderWindow()->SetMultiSamples(0);
    // Build a command line for the vtkTesting::Test method.
    // See VTK documentation and RenderingTestHelper for more information.
    // Use the following command line option to save the difference image
    // and the test image in some tmp folder
    // m_CommandlineArgs.push_back("-T");
    // m_CommandlineArgs.push_back("/path/to/save/tmp/difference/images/");
    m_CommandlineArgs.push_back("-V");
    renderingTestHelper.AddNodeToStorage(testNode);
    renderingTestHelper.SetMapperIDToRender3D();

    m_SplineVisualisationPlugin->CommonSplineRenderFunction(m_SplineVisualisationPlugin->renderer);
    mitk::SplineVtkMapper3D::Pointer returnedMapper = m_SplineVisualisationPlugin->m_SplineVtkMapper;

    m_CommandlineArgs.push_back(m_SplineVisualisationPlugin->m_SplineVtkMapper->GetClassHierarchy()[0]);

    // Convert vector of strings to argc/argv
    mitk::RenderingTestHelper::ArgcHelperClass arg(m_CommandlineArgs);
    CPPUNIT_ASSERT_MESSAGE("Checking whether mitk SplineMapper output was correctly initialised.",
                           testMapper == returnedMapper);
    CPPUNIT_ASSERT(renderingTestHelper.CompareRenderWindowAgainstReference(arg.GetArgc(), arg.GetArgv()) == true);
  }

   void Render_CommonTubeRenderFunction_StandardViewOutput() {
   // modified from  https://docs.mitk.org/2016.11/mitkSurfaceVtkMapper3DTest_8cpp_source.html
   char *argv[] = {"parm0", "param1", "param2"};
   int argc = 0;
   mitk::RenderingTestHelper renderingTestHelper = mitk::RenderingTestHelper(640, 480, argc, argv);
   renderingTestHelper.GetVtkRenderWindow()->SetMultiSamples(0);
   std::vector<std::string> m_CommandlineArgs;
   m_CommandlineArgs.push_back(m_SplineVisualisationPlugin->m_SplineVtkMapper->GetClassHierarchy()[0]);

   renderingTestHelper.SetMapperID(mitk::BaseRenderer::Standard3D);
    vtkSmartPointer<vtkFloatArray> textureCoordinates = vtkSmartPointer<vtkFloatArray>::New();
    textureCoordinates->SetNumberOfComponents(1);
    textureCoordinates->SetName("TestTubeTextureCoordinates");
    mitk::Image::Pointer textureImage = static_cast<mitk::Image *>(
      renderingTestHelper.GetDataStorage()->GetNode(mitk::NodePredicateDataType::New("Image"))->GetData());

    mitk::Surface::Pointer testSurfaceToPutTextureOn = static_cast<mitk::Surface *>(
      renderingTestHelper.GetDataStorage()->GetNode(mitk::NodePredicateDataType::New("Surface"))->GetData());

    m_SplineVisualisationPlugin->CommonTubeRenderFunction(m_SplineVisualisationPlugin->renderer,
                                                          m_SplineVisualisationPlugin->m_Parametricspline);
      // Convert vector of strings to argc/argv
      mitk::RenderingTestHelper::ArgcHelperClass arg(m_CommandlineArgs);
    CPPUNIT_ASSERT_MESSAGE(
      "Checking whether mitk SplineMapper output was correctly initialised.",
      testSurfaceToPutTextureOn == m_SplineVisualisationPlugin->m_TubeSurfaceVTKmapper->GetInput());
      CPPUNIT_ASSERT(renderingTestHelper.CompareRenderWindowAgainstReference(arg.GetArgc(), arg.GetArgv()) == true);
    MITK_TEST_CONDITION(renderingTestHelper.CompareRenderWindowAgainstReference(argc, argv) == true,
                        "CompareRenderWindowAgainstReference test result positive?");
  }

  // SplineDataObjectVisualizationFilter

  void  Equal_CloneAndOriginal_ConvertParametricSplineRepresentation()
  {
    mitk::DataNode::Pointer testNode = mitk::DataNode::New();
    testNode->SetData(mitk::IOUtil::Load(m_PathToPointSet)[0]);
    mitk::PointSet::Pointer testAnchorPointSet = m_SplineVisualisationPlugin->GetCurrentPointsetInListWidget();
    char *argv[] = {"parm0", "param1", "param2"};
    int argc = 0;
    mitk::RenderingTestHelper renderingTestHelper = mitk::RenderingTestHelper(640, 480, argc, argv);
    renderingTestHelper.GetVtkRenderWindow()->SetMultiSamples(0);

    mitk::DataNode::Pointer Referencepointsdatanode =
      m_SplineDataObjectVisualizationFilter->ConvertParametricSplineRepresentation(testAnchorPointSet);

    CPPUNIT_ASSERT_MESSAGE("Checking whether the Datanode was successfully initialised and assigned.",
                           testNode == Referencepointsdatanode);
    m_SplineVisualisationPlugin->m_DataStorage->Add(Referencepointsdatanode);
    renderingTestHelper.GetDataStorage()->Add(Referencepointsdatanode);
    MITK_TEST_CONDITION(renderingTestHelper.GetDataStorage() == m_SplineVisualisationPlugin->m_DataStorage,
                        "CompareDataObjectAgainstReference test result positive?");
  }

  void Equal_CloneAndOriginal_ConvertTubeSplineRepresentation()
  {
    mitk::DataNode::Pointer testNode = mitk::DataNode::New();
    testNode->SetData(mitk::IOUtil::Load(m_PathToPointSet)[0]);
    mitk::PointSet::Pointer testAnchorPointSet = m_SplineVisualisationPlugin->GetCurrentPointsetInListWidget();
    char *argv[] = {"parm0", "param1", "param2"};
    int argc = 0;
    mitk::RenderingTestHelper renderingTestHelper = mitk::RenderingTestHelper(640, 480, argc, argv);

    mitk::DataNode::Pointer Referencepointsdatanode =
      m_SplineDataObjectVisualizationFilter->ConvertParametricSplineRepresentation(testAnchorPointSet);

    CPPUNIT_ASSERT_MESSAGE("Checking whether the Datanode was successfully initialised and assigned.",
                           testNode == Referencepointsdatanode);
    m_SplineVisualisationPlugin->m_DataStorage->Add(Referencepointsdatanode);
    renderingTestHelper.GetDataStorage()->Add(Referencepointsdatanode);
    MITK_TEST_CONDITION(renderingTestHelper.GetDataStorage() == m_SplineVisualisationPlugin->m_DataStorage,
                        "CompareDataObjectAgainstReference test result positive?");
  }

    void Return_BaseData_GetRepresentationObject()
  {
    unsigned int idx =  1+ (std::rand() % (12 - 1 + 1));
    mitk::BaseData::Pointer ReferenceBaseData = m_SplineDataObjectVisualizationFilter->GetRepresentationObject(idx);
    mitk::BaseData::Pointer testBaseData = mitk::IOUtil::Load(m_PathToPointSet)[0];

     CPPUNIT_ASSERT_MESSAGE("Checking whether the Object was successfully initialised and assigned.",
                           testBaseData == ReferenceBaseData);
  }

   void Return_nullptr_GetRepresentationObject()
  {
    unsigned int idx = -10;
    mitk::BaseData::Pointer ReferenceBaseData = m_SplineDataObjectVisualizationFilter->GetRepresentationObject(idx);
    CPPUNIT_ASSERT_MESSAGE("Checking whether the Object was successfully initialised and assigned.",
                           ReferenceBaseData == nullptr);
  }

  void Equal_CloneAndOriginal_GetSplineDataNode()
  {
    mitk::DataNode::Pointer testNode = mitk::DataNode::New();
    testNode->SetData(mitk::IOUtil::Load(m_PathToPointSet)[0]);
    mitk::PointSet::Pointer testAnchorPointSet = m_SplineVisualisationPlugin->GetCurrentPointsetInListWidget();
    char *argv[] = {"parm0", "param1", "param2"};
    int argc = 0;
    mitk::RenderingTestHelper renderingTestHelper = mitk::RenderingTestHelper(640, 480, argc, argv);

    std::vector<mitk::NavigationData::Pointer> referenceNavigationDataVector = m_SplineDataObjectVisualizationFilter->m_VecEMBaseData;
    mitk::DataNode::Pointer referenceDataNode = m_SplineDataObjectVisualizationFilter->GetSplineDataNode(m_SplineDataObjectVisualizationFilter->m_VecEMBaseData);


    CPPUNIT_ASSERT_MESSAGE("Checking whether the Datanode was successfully initialised and assigned.",
                           testNode == referenceDataNode);
    m_SplineVisualisationPlugin->m_DataStorage->Add(referenceDataNode);
    renderingTestHelper.GetDataStorage()->Add(testNode);
    MITK_TEST_CONDITION(renderingTestHelper.GetDataStorage() == m_SplineVisualisationPlugin->m_DataStorage,
                        "CompareDataObjectAgainstReference test result positive?");
  }


  // mitkSplineVisualisationEvaluationUtil Test Methods 

  void Equal_CloneAndOriginal_GenerateSplinePoints()
  {
    mitk::PointSet::Pointer testPointSet = mitk::PointSet::New();
    mitk::Point3D new0, new1, new2, new3;
    new0.Fill(0);
    new1.Fill(1);
    new2.Fill(2);
    testPointSet->InsertPoint(5, new0, mitk::PTCORNER, 0);
    testPointSet->InsertPoint(112, new1, 0);
    testPointSet->InsertPoint(2, new2, 0);
    testPointSet->InsertPoint(2, new0, 1);
    FillVector3D(new3, 2.2, 3.3, -4.4);
    testPointSet->InsertPoint(17, new3);
    testPointSet->SetPoint(4, new3);
    testPointSet->SetPoint(7, new3);
    testPointSet->SetPoint(2, new3);
    vtkParametricSpline *parametricspline = m_SplineVisualisationPlugin->m_Parametricspline;
    mitk::PointSet::Pointer refAnchorpoints = m_SplineVisualisationPlugin->GetCurrentPointsetInListWidget();
    double iteration_range = 0.025;
    mitk::PointSet::Pointer refPointSet =
      m_SplineVisualisationEvaluationUtil->GenerateSplinePoints(parametricspline, refAnchorpoints, iteration_range);
    char *argv[] = {"parm0", "param1", "param2"};
    int argc = 0;
    mitk::RenderingTestHelper renderingTestHelper = mitk::RenderingTestHelper(640, 480, argc, argv);


    CPPUNIT_ASSERT_MESSAGE("Checking whether the refernece points have been successfully initialised and assigned.",
                           refAnchorpoints != refPointSet);
    MITK_TEST_CONDITION(renderingTestHelper.GetDataStorage() == m_SplineVisualisationPlugin->m_DataStorage,
                        "CompareDataObjectAgainstReference test result positive?");
  }


};
MITK_TEST_SUITE_REGISTRATION(mitkSplineVisualisationStructure)