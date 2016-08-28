/*

  Timer module

*/

//-----------------------------------------------------------------------------------------------
void setTimer(unsigned long *timer, int delayTime)
{
   *timer = (millis() + delayTime); 
}

//-----------------------------------------------------------------------------------------------
boolean checkTimer(unsigned long *timer)
{
  if (*timer >= millis())
    return false;
  else
    return true; 
}   

// End Module
