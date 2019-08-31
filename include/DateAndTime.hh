#ifndef DATEANDTIME_HH
#define DATEANDTIME_HH
class DateAndTime 
{
public:
 //constructor destructor
  DateAndTime();
  DateAndTime(int y, int mo, int day);
  DateAndTime(int y, int mo, int day, int h, int mi, int s);
  DateAndTime(int y, int mo, int day, int h, int mi, int s, int msec);
  DateAndTime(double julian_date);
  ~DateAndTime();
	
//public methods
	
  double DifferenceInDays(const DateAndTime) const;
  double DifferenceInHours(const DateAndTime) const;
  double DifferenceInMinutes(const DateAndTime) const;
  double DifferenceInSeconds(const DateAndTime) const;
  double DifferenceInMilliseconds(const DateAndTime)const;
  long   JulianDay() const;
  double JulianDate() const;
  int   DayOfYear() const;
  void ConvertToJulianDate(double aJulianDate);
	
//operators
  void operator=(const DateAndTime);

private:

//private methods


//attributes
public: //public attributes
  int year,month,day,hour,min,sec,msec;
};
#endif
