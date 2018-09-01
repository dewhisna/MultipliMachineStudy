#############################################################################
##
## Multipli Machine Study Application
## Copyright (C) 2018 Multipli Machinery, Corp.
## Contact: http://multiplimachine.com/
##
## This file is part of the Multipli Machine Study Application,
## written and developed for Multipli Machinery by Donald Whisnant.
##
## GNU General Public License Usage
## This file may be used under the terms of the GNU General Public License
## version 3.0 as published by the Free Software Foundation and appearing
## in the file gpl-3.0.txt included in the packaging of this file. Please
## review the following information to ensure the GNU General Public License
## version 3.0 requirements will be met:
## http://www.gnu.org/copyleft/gpl.html.
##
## Other Usage
## Alternatively, this file may be used in accordance with the terms and
## conditions contained in a signed written agreement between you and
## Multipli Machinery Corp.
##
#############################################################################

cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)

if(WIN32)
	set(LANGUAGES CXX RC)
else()
	set(LANGUAGES CXX)
endif()
project(MachineStudy LANGUAGES ${LANGUAGES} VERSION 2.0.0.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# The following copies the msvc runtime dlls to the install folder on Windows:
include(InstallRequiredSystemLibraries)

# ---------------------------------------------------------

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets PrintSupport Help Sql)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ---------------------------------------------------------

add_definitions(
	# The following define makes your compiler emit warnings if you use
	# any feature of Qt which has been marked as deprecated (the exact warnings
	# depend on your compiler). Please consult the documentation of the
	# deprecated API in order to know how to port your code away from it.
	-DQT_DEPRECATED_WARNINGS
	# You can also make your code fail to compile if you use deprecated APIs.
	# In order to do so, uncomment the following line.
	# You can also select to disable deprecated APIs only up to a certain version of Qt.
	-DQT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
)

if(WIN32)
	add_definitions(
		-DWIN32_LEAN_AND_MEAN
		-D_USE_MATH_DEFINES
	)
endif()

if(SPECIALVERSION)
	add_definitions(
		-DVER_SPECIALVERSION_STR=\"${SPECIALVERSION}\"
	)
endif()

# ---------------------------------------------------------

set(SOURCES
	"main.cpp"
	"MachineStudy.qrc"
	"conversion.h"
	"KeyPressFilter.h"
	"version.h"
)

SET(rc_file "MachineStudy.rc")
SET(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} -D_RUNNING_RC_COMPILER_ -I\"${CMAKE_CURRENT_BINARY_DIR}\" -I\"${CMAKE_CURRENT_SOURCE_DIR}\"")
if(WIN32)
	LIST(APPEND SOURCES ${rc_file})
endif()

set(MainWindow_SOURCES
	"MainWindow.cpp"
	"MainWindow.h"
	"MainWindow.ui"
)

set(CustomerDialog_SOURCES
	"CustomerDialog.cpp"
	"CustomerDialog.h"
	"CustomerDialog.ui"
)

set(FormationCalcDialog_SOURCES
	"FormationCalcDialog.cpp"
	"FormationCalcDialog.h"
	"FormationCalcDialog.ui"
)

set(ProductionCalcDialog_SOURCES
	"ProductionCalcDialog.cpp"
	"ProductionCalcDialog.h"
	"ProductionCalcDialog.ui"
)

set(GradeCalcDialog_SOURCES
	"GradeCalcDialog.cpp"
	"GradeCalcDialog.h"
	"GradeCalcDialog.ui"
)

set(HeadBoxDilutionDialog_SOURCES
	"HeadBoxDilutionDialog.cpp"
	"HeadBoxDilutionDialog.h"
	"HeadBoxDilutionDialog.ui"
)

set(FormConstDialog_SOURCES
	"FormConstDialog.cpp"
	"FormConstDialog.h"
	"FormConstDialog.ui"
)

set(PipeFlowCalcDialog_SOURCES
	"PipeFlowCalcDialog.cpp"
	"PipeFlowCalcDialog.h"
	"PipeFlowCalcDialog.ui"
)

set(AreaFlowCalcDialog_SOURCES
	"AreaFlowCalcDialog.cpp"
	"AreaFlowCalcDialog.h"
	"AreaFlowCalcDialog.ui"
)

set(YieldPerDayDialog_SOURCES
	"YieldPerDayDialog.cpp"
	"YieldPerDayDialog.h"
	"YieldPerDayDialog.ui"
)

set(AboutDialog_SOURCES
	"AboutDialog.cpp"
	"AboutDialog.h"
	"AboutDialog.ui"
)

set(HelpDialog_SOURCES
	"HelpDialog.cpp"
	"HelpDialog.h"
	"HelpDialog.ui"
	"HelpBrowser.cpp"
	"HelpBrowser.h"
)

# Add QtFindReplaceDialog, but exclude from 'all' build so it doesn't get
#	"installed" with this application binary "make install":
add_subdirectory(../QtFindReplaceDialog/dialogs QtFindReplaceDialog EXCLUDE_FROM_ALL)

if(WIN32)
	set(MY_COMPILE_OPTIONS "/W3")
else()
	set(MY_COMPILE_OPTIONS "-Wall")
endif()

add_library(MainWindow ${MainWindow_SOURCES})
target_compile_options(MainWindow PUBLIC ${MY_COMPILE_OPTIONS})
qt5_use_modules(MainWindow Core Gui Widgets Help Sql PrintSupport)
target_link_libraries(MainWindow QtFindReplaceDialog)

add_library(CustomerDialog ${CustomerDialog_SOURCES})
target_compile_options(CustomerDialog PUBLIC ${MY_COMPILE_OPTIONS})
qt5_use_modules(CustomerDialog Core Gui Widgets)

add_library(FormationCalcDialog ${FormationCalcDialog_SOURCES})
target_compile_options(FormationCalcDialog PUBLIC ${MY_COMPILE_OPTIONS})
qt5_use_modules(FormationCalcDialog Core Gui Widgets)

add_library(ProductionCalcDialog ${ProductionCalcDialog_SOURCES})
target_compile_options(ProductionCalcDialog PUBLIC ${MY_COMPILE_OPTIONS})
qt5_use_modules(ProductionCalcDialog Core Gui Widgets)

add_library(GradeCalcDialog ${GradeCalcDialog_SOURCES})
target_compile_options(GradeCalcDialog PUBLIC ${MY_COMPILE_OPTIONS})
qt5_use_modules(GradeCalcDialog Core Gui Widgets)

add_library(HeadBoxDilutionDialog ${HeadBoxDilutionDialog_SOURCES})
target_compile_options(HeadBoxDilutionDialog PUBLIC ${MY_COMPILE_OPTIONS})
qt5_use_modules(HeadBoxDilutionDialog Core Gui Widgets)

add_library(FormConstDialog ${FormConstDialog_SOURCES})
target_compile_options(FormConstDialog PUBLIC ${MY_COMPILE_OPTIONS})
qt5_use_modules(FormConstDialog Core Gui Widgets)

add_library(PipeFlowCalcDialog ${PipeFlowCalcDialog_SOURCES})
target_compile_options(PipeFlowCalcDialog PUBLIC ${MY_COMPILE_OPTIONS})
qt5_use_modules(PipeFlowCalcDialog Core Gui Widgets)

add_library(AreaFlowCalcDialog ${AreaFlowCalcDialog_SOURCES})
target_compile_options(AreaFlowCalcDialog PUBLIC ${MY_COMPILE_OPTIONS})
qt5_use_modules(AreaFlowCalcDialog Core Gui Widgets)

add_library(YieldPerDayDialog ${YieldPerDayDialog_SOURCES})
target_compile_options(YieldPerDayDialog PUBLIC ${MY_COMPILE_OPTIONS})
qt5_use_modules(YieldPerDayDialog Core Gui Widgets)

add_library(AboutDialog ${AboutDialog_SOURCES})
target_compile_options(AboutDialog PUBLIC ${MY_COMPILE_OPTIONS})
qt5_use_modules(AboutDialog Core Gui Widgets)

add_library(HelpDialog ${HelpDialog_SOURCES})
target_compile_options(HelpDialog PUBLIC ${MY_COMPILE_OPTIONS})
qt5_use_modules(HelpDialog Core Gui Widgets Help Sql)

if(WIN32)
	set(GUI_TYPE WIN32)
elseif(APPLE)
	set(GUI_TYPE MACOSX_BUNDLE)
endif()

add_executable(${PROJECT_NAME} ${GUI_TYPE} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
		MainWindow
		CustomerDialog
		FormationCalcDialog
		ProductionCalcDialog
		GradeCalcDialog
		HeadBoxDilutionDialog
		FormConstDialog
		PipeFlowCalcDialog
		AreaFlowCalcDialog
		YieldPerDayDialog
		AboutDialog
		HelpDialog
)
target_compile_options(${PROJECT_NAME} PUBLIC ${MY_COMPILE_OPTIONS})

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install CACHE PATH "Installation Prefix Folder" FORCE)

install(TARGETS
		${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION bin
)

# Deploy Help Files
add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/../../doc/qthelp/machstdy.qch $<TARGET_FILE_DIR:${PROJECT_NAME}>
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/../../doc/qthelp/machstdy.qhc $<TARGET_FILE_DIR:${PROJECT_NAME}>
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/../../doc/MachineStudy.pdf $<TARGET_FILE_DIR:${PROJECT_NAME}>
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/../../doc/MachineStudy-Examples.pdf $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

install(FILES
		${CMAKE_CURRENT_SOURCE_DIR}/../../doc/qthelp/machstdy.qch
		${CMAKE_CURRENT_SOURCE_DIR}/../../doc/qthelp/machstdy.qhc
		${CMAKE_CURRENT_SOURCE_DIR}/../../doc/MachineStudy.pdf
		${CMAKE_CURRENT_SOURCE_DIR}/../../doc/MachineStudy-Examples.pdf
		DESTINATION bin
)

if(WIN32)
	# Include DLLs needed on Windows:
	#	This logic doesn't work for Linux since the rpath
	#	won't be set, plus Qt needs to be compiled with
	#	the other system dependencies too...
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:${PROJECT_NAME}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:${PROJECT_NAME}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:${PROJECT_NAME}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::PrintSupport> $<TARGET_FILE_DIR:${PROJECT_NAME}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Help> $<TARGET_FILE_DIR:${PROJECT_NAME}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Sql> $<TARGET_FILE_DIR:${PROJECT_NAME}>
		COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QMinimalIntegrationPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms
		COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/platformthemes
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QFlatpakThemePlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/platformthemes
		COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/generic
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QTuioTouchPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/generic
		COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/imageformats
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QGifPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/imageformats
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QICNSPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/imageformats
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QICOPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/imageformats
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QJpegPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/imageformats
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QWbmpPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/imageformats
		COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/styles
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QWindowsVistaStylePlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/styles
		COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/sqldrivers
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QSQLiteDriverPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/sqldrivers
		COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/printsupport
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QWindowsPrinterSupportPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/printsupport
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE $<TARGET_FILE_DIR:${PROJECT_NAME}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/gpl-3.0.txt $<TARGET_FILE_DIR:${PROJECT_NAME}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/MachineStudy.iss $<TARGET_FILE_DIR:${PROJECT_NAME}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/version.iss $<TARGET_FILE_DIR:${PROJECT_NAME}>
	)

#	message(status "Core: ${Qt5Core_PLUGINS}  Gui: ${Qt5Gui_PLUGINS}  Widgets: ${Qt5Widgets_PLUGINS}  Help: ${Qt5Help_PLUGINS}  PrintSupport: ${Qt5PrintSupport_PLUGINS}  SQL: ${Qt5Sql_PLUGINS}")

	install(FILES
			$<TARGET_FILE:Qt5::Core>
			$<TARGET_FILE:Qt5::Gui>
			$<TARGET_FILE:Qt5::Widgets>
			$<TARGET_FILE:Qt5::PrintSupport>
			$<TARGET_FILE:Qt5::Help>
			$<TARGET_FILE:Qt5::Sql>
			LICENSE
			gpl-3.0.txt
			MachineStudy.iss
			version.iss
			DESTINATION bin
	)

	install(FILES
			$<TARGET_FILE:Qt5::QWindowsIntegrationPlugin>
			$<TARGET_FILE:Qt5::QMinimalIntegrationPlugin>
			DESTINATION bin/platforms
	)

	install(FILES
			$<TARGET_FILE:Qt5::QFlatpakThemePlugin>
			DESTINATION bin/platformthemes
	)

	install(FILES
			$<TARGET_FILE:Qt5::QTuioTouchPlugin>
			DESTINATION bin/generic
	)

	install(FILES
			$<TARGET_FILE:Qt5::QGifPlugin>
			$<TARGET_FILE:Qt5::QICNSPlugin>
			$<TARGET_FILE:Qt5::QICOPlugin>
			$<TARGET_FILE:Qt5::QJpegPlugin>
			$<TARGET_FILE:Qt5::QWbmpPlugin>
			DESTINATION bin/imageformats
	)

	install(FILES
			$<TARGET_FILE:Qt5::QWindowsVistaStylePlugin>
			DESTINATION bin/styles
	)

	install(FILES
			$<TARGET_FILE:Qt5::QSQLiteDriverPlugin>
			DESTINATION bin/sqldrivers
	)

	install(FILES
			$<TARGET_FILE:Qt5::QWindowsPrinterSupportPlugin>
			DESTINATION bin/printsupport
	)
endif()

