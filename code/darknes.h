
#ifndef _DARKNES_H_
#define _DARKNES_H_

#define DEBUG

#ifdef DEBUG
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
#define Assert(Expression)
#endif

#define global static
#define internal static

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef float f32;

#define KB(X) ((X)*1024)

// Platform dependant unfortunately
internal void
Print(char* fmt, ...);

//
// Cartridge
//

#pragma pack(push, 1)
struct nes2_flags
{
   u8 Mirroring         : 1;
   u8 BatteryBacked     : 1;
   u8 Trainer           : 1;
   u8 FourScreen        : 1;
   u8 LowerNibbleMapper : 4;
   u8 VsGame            : 1;
   u8 PC10Game          : 1;
   u8 Nes2Signature     : 2;
   u8 UpperNibbleMapper : 4;
};
#pragma pack(pop)

struct nes2_header
{
   u32 FormatId;
   u8 PrgPages;
   u8 ChrPages;
   nes2_flags Flags;
   u8 RamBanks;
   u8 Spare[7];
};

struct rom_spec
{
   u8 MapperNumber;
   u32 PrgSize;
   u32 ChrSize;
   u32 PrgRamSize;
};

struct cart
{
   rom_spec RomSpec;

   u32 RomSize;
   u8* Rom;
};

struct input
{
   u8 Keys[128];
};

//
// CPU
//

enum interrupt_type
{
   Interrupt_Nmi,
   Interrupt_Reset,
   Interrupt_IrqBrk,

   Interrupt_TypeCount,
};

#pragma pack(push, 1)
union status_register
{
   struct 
   {
      u8 Carry             : 1;
      u8 Zero              : 1;
      u8 InterruptDisable  : 1;
      u8 DecimalMode       : 1;
      u8 BreakCommand      : 1;
      u8 Unused            : 1;
      u8 Overflow          : 1;
      u8 Negative          : 1;
   };
   u8 Value;
};
#pragma pack(pop)

struct cpu
{
   u32 CyclesRemaining;

   u8 A;
   u8 X;
   u8 Y;

   u16 PC;
   u8 S;    // Stack Pointer
   status_register P;    // Status Register
};


//
// PPU
//

struct pixel_array
{
   u32* Buffer;
   u32 Width;
   u32 Height;
};

u32 nesRgb[] =
{  0x7C7C7C, 0x0000FC, 0x0000BC, 0x4428BC, 0x940084, 0xA80020, 0xA81000, 0x881400,
   0x503000, 0x007800, 0x006800, 0x005800, 0x004058, 0x000000, 0x000000, 0x000000,
   0xBCBCBC, 0x0078F8, 0x0058F8, 0x6844FC, 0xD800CC, 0xE40058, 0xF83800, 0xE45C10,
   0xAC7C00, 0x00B800, 0x00A800, 0x00A844, 0x008888, 0x000000, 0x000000, 0x000000,
   0xF8F8F8, 0x3CBCFC, 0x6888FC, 0x9878F8, 0xF878F8, 0xF85898, 0xF87858, 0xFCA044,
   0xF8B800, 0xB8F818, 0x58D854, 0x58F898, 0x00E8D8, 0x787878, 0x000000, 0x000000,
   0xFCFCFC, 0xA4E4FC, 0xB8B8F8, 0xD8B8F8, 0xF8B8F8, 0xF8A4C0, 0xF0D0B0, 0xFCE0A8,
   0xF8D878, 0xD8F878, 0xB8F8B8, 0xB8F8D8, 0x00FCFC, 0xF8D8F8, 0x000000, 0x000000 };

struct ppu_control
{
   u8 NametableSelect   : 2;
   u8 IncrementMode     : 1;
   u8 SpriteTileSelect  : 1;
   u8 BgTileSelect      : 1;
   u8 SpriteHeight      : 1;
   u8 PpuMaster         : 1;
   u8 NmiEnable         : 1;
};

struct ppu_mask
{
   u8 Greyscale        : 1;
   u8 BackgroundLeft   : 1;
   u8 SpriteLeft       : 1;
   u8 BackgroundEnable : 1;
   u8 SpriteEnable     : 1;
   u8 ColorEmphasis    : 3;
};

struct ppu_status
{
   u8 Spare          : 5;
   u8 SpriteOverflow : 1;
   u8 Sprite0Hit     : 1;
   u8 VertBlank      : 1;   
};

struct ppu
{
   pixel_array Display;

   ppu_control PpuControl;
   ppu_mask PpuMask;
   ppu_status PpuStatus;
   u8 OamAddr;
   u8 OamData;
   u8 PpuScroll;
   u8 PpuAddr;
   u8 PpuData;
   u8 OamDma;
};

//
// APU
//

struct apu
{

};


struct nes
{
   char* RomName;

   cpu Cpu;
   ppu Ppu;
   apu Apu;

   u32 RamSize;
   u8* Ram;

   cart Cart;
};

#endif