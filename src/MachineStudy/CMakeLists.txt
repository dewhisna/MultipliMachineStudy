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

# ---------------------------------------------------------

find_package(Qt5Widgets)

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

add_subdirectory(../QtFindReplaceDialog/dialogs QtFindReplaceDialog)

add_library(MainWindow ${MainWindow_SOURCES})
target_compile_options(MainWindow PUBLIC "-Wall")
qt5_use_modules(MainWindow Core Widgets PrintSupport)
target_link_libraries(MainWindow QtFindReplaceDialog)

add_library(CustomerDialog ${CustomerDialog_SOURCES})
target_compile_options(CustomerDialog PUBLIC "-Wall")
qt5_use_modules(CustomerDialog Core Widgets)

add_library(FormationCalcDialog ${FormationCalcDialog_SOURCES})
target_compile_options(FormationCalcDialog PUBLIC "-Wall")
qt5_use_modules(FormationCalcDialog Core Widgets)

add_library(ProductionCalcDialog ${ProductionCalcDialog_SOURCES})
target_compile_options(ProductionCalcDialog PUBLIC "-Wall")
qt5_use_modules(ProductionCalcDialog Core Widgets)

add_library(GradeCalcDialog ${GradeCalcDialog_SOURCES})
target_compile_options(GradeCalcDialog PUBLIC "-Wall")
qt5_use_modules(GradeCalcDialog Core Widgets)

add_library(HeadBoxDilutionDialog ${HeadBoxDilutionDialog_SOURCES})
target_compile_options(HeadBoxDilutionDialog PUBLIC "-Wall")
qt5_use_modules(HeadBoxDilutionDialog Core Widgets)

add_library(FormConstDialog ${FormConstDialog_SOURCES})
target_compile_options(FormConstDialog PUBLIC "-Wall")
qt5_use_modules(FormConstDialog Core Widgets)

add_library(PipeFlowCalcDialog ${PipeFlowCalcDialog_SOURCES})
target_compile_options(PipeFlowCalcDialog PUBLIC "-Wall")
qt5_use_modules(PipeFlowCalcDialog Core Widgets)

add_library(AreaFlowCalcDialog ${AreaFlowCalcDialog_SOURCES})
target_compile_options(AreaFlowCalcDialog PUBLIC "-Wall")
qt5_use_modules(AreaFlowCalcDialog Core Widgets)

add_library(YieldPerDayDialog ${YieldPerDayDialog_SOURCES})
target_compile_options(YieldPerDayDialog PUBLIC "-Wall")
qt5_use_modules(YieldPerDayDialog Core Widgets)

add_library(AboutDialog ${AboutDialog_SOURCES})
target_compile_options(AboutDialog PUBLIC "-Wall")
qt5_use_modules(AboutDialog Core Widgets)

add_library(HelpDialog ${HelpDialog_SOURCES})
target_compile_options(HelpDialog PUBLIC "-Wall")
qt5_use_modules(HelpDialog Core Widgets Help)

add_executable(${PROJECT_NAME} ${SOURCES})
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
target_compile_options(${PROJECT_NAME} PUBLIC "-Wall")

