/****************************************************************************
**
** Multipli Machine Study Application
** Copyright (C) 1994, 2018-2022 Multipli Machinery, Corp.
** Contact: http://multiplimachine.com/
**
** This file is part of the Multipli Machine Study Application,
** written and developed for Multipli Machinery by Donald Whisnant.
**
** GNU General Public License Usage
** This file may be used under the terms of the GNU General Public License
** version 3.0 as published by the Free Software Foundation and appearing
** in the file gpl-3.0.txt included in the packaging of this file. Please
** review the following information to ensure the GNU General Public License
** version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and
** Multipli Machinery Corp.
**
****************************************************************************/

//
// Math Conversion Factors
//	Imperial -> Metric
//	Metric -> Imperial
//

#ifndef _CONVERSION_H_
#define _CONVERSION_H_

#include <math.h>

//
// Conversion Constants:
//          Imperial -> Metric     Multiply by conversion factor
//          Metric -> Imperial     Divide by conversion factor
//

inline double toMetric(double nValue, double nScaleFactor)
{
	return (nValue * nScaleFactor);
}

inline double toImperial(double nValue, double nScaleFactor)
{
	return (nValue / nScaleFactor);
}

inline double convert(bool bToMetric, double nValue, double nScaleFactor)
{
	return bToMetric ? toMetric(nValue, nScaleFactor) : toImperial(nValue, nScaleFactor);
}

const double kg_per_lb			= 0.4536;				// Kilograms per pound
const double kg_per_ton			= kg_per_lb*2000.0;		// Kilograms per ton
const double lbs_per_ton		= 2000.0;				// pounds per ton
const double grams_per_lb		= 453.6;				// grams per pound
const double sqm_per_tsqft		= 92.903;				// Square Meters per Thousand Sq Ft
const double mm_per_in			= 25.4;					// Millimeters per inch
const double liters_per_gallon	= 3.785;				// liters per gallon
const double m_per_ft			= 0.3048;				// meters per foot
const double sqmm_per_sqin		= 645.2;				// square mm per square in.
const double gallons_per_cubft	= 7.4805194805195;		// gallons per cubic ft.
const double sqin_per_sqft		= 144.0;				// square in per square ft.
const double sec_per_min		= 60.0;					// seconds per minute
const double min_per_hour		= 60.0;					// minutes per hour
const double hours_per_day		= 24.0;					// hours per day
const double in_per_ft			= 12.0;					// Inches per foot

const double BasisWeightConv	= grams_per_lb/sqm_per_tsqft;	// lbs/1K Sq. Ft -> Grams/Sq. Meter
const double SheetWidthConv		= mm_per_in;					// in -> mm
const double YieldPerDayConv	= kg_per_ton;					// Tons/Day -> kg/Day
const double WaterNeededConv	= liters_per_gallon;			// Gallons/Min -> Liters/Min
const double MachineSpeedConv	= m_per_ft;						// Feet/Min -> Meters/Min
const double DensityConv		= grams_per_lb;					// lbs/point -> Grams/point
const double CaliperConv		= 1.0;							// ??? (none)
const double VelocityConv		= m_per_ft;						// Feet/Sec -> Meters/Sec
const double AreaConv			= sqmm_per_sqin;				// Sq In -> Sq mm
const double PipeConv			= mm_per_in;					// In -> mm
const double FormConv			= kg_per_ton/mm_per_in;			// ton/in of vat -> kg/mm of vat

#endif	// _CONVERSION_H_
