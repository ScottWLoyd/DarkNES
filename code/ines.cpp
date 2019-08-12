
#include "ines.h"

namespace ines {

   ines_file LoadInesImage(u8* Data, size_t DataLen)
   {
      ines_file Result = { 0 };
      Result.Header = *(ines_header*)Data;
      
      Result.Loaded = true;
      return Result;
   }

}