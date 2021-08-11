project(org_mitk_gui_qt_ARStrokeTreatment)

mitk_create_plugin(
  EXPORT_DIRECTIVE ARSTROKETREATMENT_EXPORT
  EXPORTED_INCLUDE_SUFFIXES src
  MODULE_DEPENDS MitkQtWidgetsExt MitkIGT MitkIGTUI
)
