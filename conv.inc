{

           Machine Study V1.1

   Copyright(c)1994 Multipli Machinery

       Written by Donald Whisnant

  English<->Metric Conversion Values
          Header/Include File

}

{ Conversion:
          English -> Metric     Multiply by conversion factor
          Metric -> English     Divide by conversion factor
}

const
  kg_per_lb     =       0.4536;         { Kilograms per pound }
  kg_per_ton    =       kg_per_lb*2000; { Kilograms per ton }
  grams_per_lb  =       453.6;          { grams per pound }
  sqm_per_tsqft =       0.0929*1000;    { Square Meters per Th. Sq Ft }
  mm_per_in     =       25.4;           { Millimeters per inch }
  liters_per_gallon =   3.785;          { liters per gallon }
  m_per_ft      =       0.3048;         { meters per foot }
  sqmm_per_sqin =       645.2;          { square mm per square in. }

  BWConv = grams_per_lb/sqm_per_tsqft;  { lbs/1K Sq. Ft -> Grams/Sq. Meter }
  SWConv = mm_per_in;                   { in -> mm }
  TPDConv = kg_per_ton;                 { Tons/Day -> kg/Day }
  GPMConv = liters_per_gallon;          { Gallons/Min -> Liters/Min }
  MSConv = m_per_ft;                    { Feet/Min -> Meters/Min }
  DensityConv = grams_per_lb;           { lbs/point -> Grams/point }
  CaliperConv = 1;                      { ??? }
  VelocityConv = m_per_ft;              { Feet/Sec -> Meters/Sec }
  AreaConv = sqmm_per_sqin;             { Sq In -> Sq MM }
  PipeConv = mm_per_in;                 { In -> MM }
  FormConv = kg_per_ton/mm_per_in;      { ton/in of vat -> kg/mm of vat }
