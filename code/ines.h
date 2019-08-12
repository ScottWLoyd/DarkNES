#ifndef _DARKNES_INES_FILE_H_
#define _DARKNES_INES_FILE_H_

#include "common.h"

namespace ines {

#pragma pack(push, 1)

struct ines_header
{
   char Signature[4];   // "NES" + 0x1A
   u8 PrgRomSize;       // size in 16 KB units
   u8 ChrRomSize;       // size in 8 KB units
   union {
      struct {
         u8 Mirroring         : 1;
         u8 BatteryBackedRam  : 1;
         u8 TrainerPresent    : 1;  // 512 bytes at $7000-71FF
         u8 FourScreenVram    : 1;
         u8 MapperNumberLo    : 4;
      };
      u8 Flags6;
   };
   union {
      struct {
         u8 VsUnisystem    : 1;
         u8 PlayChoice     : 1;
         u8 Nes2Format     : 2; // if == 2, flags 8-15 are NES 2.0
         u8 MapperNumberHi : 4;
      };
      u8 Flags7;
   };
   u8 PrgRamSize;
   union {
      struct {
         u8 TvSystem1 : 1;
         u8 Reserved  : 7;
      };
      u8 Flags9;
   };
   union {
      struct {
         u8 TvSystem2 : 2;
         u8 Spare1 : 2;
         u8 PrgRamMissing : 1;
         u8 BusConflicts : 1;
         u8 Spare2 : 2;
      };
      u8 Flags10;
   };
   u8 Byte11;
   u8 Byte12;
   u8 Byte13;
   u8 Byte14;
   u8 Byte15;
};

#pragma pack(pop)

struct ines_file {
   bool Loaded;
   ines_header Header;
};

ines_file LoadInesImage(u8* Data, size_t DataLen);

};
#endif
