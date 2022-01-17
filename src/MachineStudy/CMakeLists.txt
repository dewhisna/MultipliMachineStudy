#############################################################################
##
## Multipli Machine Study Application
## Copyright (C) 1994, 2018-2022 Multipli Machinery, Corp.
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

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets PrintSupport Help Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets PrintSupport Help Sql)

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
target_link_libraries(MainWindow
	QtFindReplaceDialog
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Gui
	Qt${QT_VERSION_MAJOR}::Widgets
	Qt${QT_VERSION_MAJOR}::Help
	Qt${QT_VERSION_MAJOR}::Sql
	Qt${QT_VERSION_MAJOR}::PrintSupport
)

add_library(CustomerDialog ${CustomerDialog_SOURCES})
target_compile_options(CustomerDialog PUBLIC ${MY_COMPILE_OPTIONS})
target_link_libraries(CustomerDialog
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Gui
	Qt${QT_VERSION_MAJOR}::Widgets
)

add_library(FormationCalcDialog ${FormationCalcDialog_SOURCES})
target_compile_options(FormationCalcDialog PUBLIC ${MY_COMPILE_OPTIONS})
target_link_libraries(FormationCalcDialog
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Gui
	Qt${QT_VERSION_MAJOR}::Widgets
)

add_library(ProductionCalcDialog ${ProductionCalcDialog_SOURCES})
target_compile_options(ProductionCalcDialog PUBLIC ${MY_COMPILE_OPTIONS})
target_link_libraries(ProductionCalcDialog
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Gui
	Qt${QT_VERSION_MAJOR}::Widgets
)

add_library(GradeCalcDialog ${GradeCalcDialog_SOURCES})
target_compile_options(GradeCalcDialog PUBLIC ${MY_COMPILE_OPTIONS})
target_link_libraries(GradeCalcDialog
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Gui
	Qt${QT_VERSION_MAJOR}::Widgets
)

add_library(HeadBoxDilutionDialog ${HeadBoxDilutionDialog_SOURCES})
target_compile_options(HeadBoxDilutionDialog PUBLIC ${MY_COMPILE_OPTIONS})
target_link_libraries(HeadBoxDilutionDialog
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Gui
	Qt${QT_VERSION_MAJOR}::Widgets
)

add_library(FormConstDialog ${FormConstDialog_SOURCES})
target_compile_options(FormConstDialog PUBLIC ${MY_COMPILE_OPTIONS})
target_link_libraries(FormConstDialog
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Gui
	Qt${QT_VERSION_MAJOR}::Widgets
)

add_library(PipeFlowCalcDialog ${PipeFlowCalcDialog_SOURCES})
target_compile_options(PipeFlowCalcDialog PUBLIC ${MY_COMPILE_OPTIONS})
target_link_libraries(PipeFlowCalcDialog
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Gui
	Qt${QT_VERSION_MAJOR}::Widgets
)

add_library(AreaFlowCalcDialog ${AreaFlowCalcDialog_SOURCES})
target_compile_options(AreaFlowCalcDialog PUBLIC ${MY_COMPILE_OPTIONS})
target_link_libraries(AreaFlowCalcDialog
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Gui
	Qt${QT_VERSION_MAJOR}::Widgets
)

add_library(YieldPerDayDialog ${YieldPerDayDialog_SOURCES})
target_compile_options(YieldPerDayDialog PUBLIC ${MY_COMPILE_OPTIONS})
target_link_libraries(YieldPerDayDialog
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Gui
	Qt${QT_VERSION_MAJOR}::Widgets
)

add_library(AboutDialog ${AboutDialog_SOURCES})
target_compile_options(AboutDialog PUBLIC ${MY_COMPILE_OPTIONS})
target_link_libraries(AboutDialog
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Gui
	Qt${QT_VERSION_MAJOR}::Widgets
)

add_library(HelpDialog ${HelpDialog_SOURCES})
target_compile_options(HelpDialog PUBLIC ${MY_COMPILE_OPTIONS})
target_link_libraries(HelpDialog
	Qt${QT_VERSION_MAJOR}::Core
	Qt${QT_VERSION_MAJOR}::Gui
	Qt${QT_VERSION_MAJOR}::Widgets
	Qt${QT_VERSION_MAJOR}::Help
	Qt${QT_VERSION_MAJOR}::Sql
)

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
	# Locate windeployqt utility:
	if (Qt${QT_VERSION_MAJOR}_FOUND AND TARGET Qt${QT_VERSION_MAJOR}::qmake AND NOT TARGET Qt${QT_VERSION_MAJOR}::windeployqt)
		get_target_property(_Qt${QT_VERSION_MAJOR}_qmake_location Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)

		execute_process(
			COMMAND "${_Qt${QT_VERSION_MAJOR}_qmake_location}" -query QT_INSTALL_PREFIX
			RESULT_VARIABLE return_code
			OUTPUT_VARIABLE qt${QT_VERSION_MAJOR}_install_prefix
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)

		set(imported_location "${qt${QT_VERSION_MAJOR}_install_prefix}/bin/windeployqt.exe")

		if (EXISTS ${imported_location})
			add_executable(Qt${QT_VERSION_MAJOR}::windeployqt IMPORTED)

			set_target_properties(Qt${QT_VERSION_MAJOR}::windeployqt PROPERTIES
				IMPORTED_LOCATION ${imported_location}
			)
		endif()
	endif()

	# Include DLLs needed on Windows by calling windeployqt:
	#	This logic doesn't work for Linux since the rpath
	#	won't be set, plus Qt needs to be compiled with
	#	the other system dependencies too...
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}/windeployqt"
		COMMAND set PATH=%PATH%$<SEMICOLON>${qt${QT_VERSION_MAJOR}_install_prefix}/bin
		COMMAND Qt${QT_VERSION_MAJOR}::windeployqt --no-translations --dir "${CMAKE_CURRENT_BINARY_DIR}/windeployqt" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
	)

	install(FILES
			"$<TARGET_FILE_DIR:${PROJECT_NAME}>/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
			LICENSE
			gpl-3.0.txt
			DESTINATION bin
	)

	install(FILES
		MachineStudy.iss
		version.iss
		DESTINATION .
	)

	install(DIRECTORY
		"${CMAKE_CURRENT_BINARY_DIR}/windeployqt/"
		DESTINATION bin
	)
endif()

