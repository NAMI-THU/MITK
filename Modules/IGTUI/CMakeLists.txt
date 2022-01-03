MITK_CREATE_MODULE(
  INCLUDE_DIRS Qmitk
  DEPENDS MitkIGT MitkQtWidgetsExt MitkPersistence ITKLabelMap
)

add_subdirectory(autoload/ConfigurationWidgetRegistry)

## create IGTUI config
configure_file(mitkIGTUIConfig.h.in ${PROJECT_BINARY_DIR}/mitkIGTUIConfig.h @ONLY)


