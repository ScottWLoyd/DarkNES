
internal void
RenderTestGradient(pixel_array* pa)
{
   u32* Pixel = pa->Buffer;
   for(u32 Y=0; Y<pa->Height; Y++)
   {
      for(u32 X=0; X<pa->Width; X++)
      {
         u32 Blue = (u32)(((f32)Y / pa->Height) * 255.0f);
         u32 Green = (u32)(((f32)X / pa->Width) * 255.0f);
         u32 Color =  (Green << 8) |  Blue;
         *Pixel++ = Color;
      }
   }
}


inline u8
Read(nes* Nes, u16 Addr)
{
   u8 Value = Nes->Ram[Addr];
   return Value;
}

internal u32
InitNes(nes* Nes)
{   
   Nes->RamSize = 0x800;

   if (Nes->Ram)
   {
      free(Nes->Ram);
   }
   Nes->Ram = (u8*)malloc(Nes->RamSize);

   Nes->Ppu.Display.Height = 240;
   Nes->Ppu.Display.Width = 256;

   if (Nes->Ppu.Display.Buffer)
   {
      free(Nes->Ppu.Display.Buffer);
   }
   Nes->Ppu.Display.Buffer = (u32*)malloc(Nes->Ppu.Display.Height * Nes->Ppu.Display.Width * sizeof(u32));

   // Load the ROM
   FILE* RomFile = fopen(Nes->RomName, "rb");
   if (!RomFile)
   {
      return 0;
   }

   fseek(RomFile, 0, SEEK_END);
   u64 fsize = ftell(RomFile);
   fseek(RomFile, 0, SEEK_SET);  //same as rewind(f);

   if (Nes->Cart.Rom)
   {
      free(Nes->Cart.Rom);
   }
   Nes->Cart.Rom = (u8*)malloc(fsize + 1);
   Nes->Cart.RomSize = fread(Nes->Cart.Rom, 1, fsize, RomFile);
   fclose(RomFile);

   nes2_header* NesHeader = (nes2_header*)Nes->Cart.Rom;
   
   char Signature[4] = {'N', 'E', 'S', 0x1A};
   Assert(NesHeader->FormatId == *(u32*)Signature);

   // TODO(scott): actually implement NES2.0 format handling

   if (NesHeader->RamBanks == 0)
   {
      NesHeader->RamBanks = 1;
   }

   Nes->Cart.RomSpec.MapperNumber = (NesHeader->Flags.UpperNibbleMapper << 4) |
                                    (NesHeader->Flags.LowerNibbleMapper);
   

   Nes->Cart.RomSpec.PrgSize = NesHeader->PrgPages * 0x4000;
   Nes->Cart.RomSpec.ChrSize = NesHeader->ChrPages * 0x2000;
   Nes->Cart.RomSpec.PrgRamSize = NesHeader->RamBanks * 0x2000;

   return 1;
}

internal void
LoadMapper(nes* Nes)
{
   if (Nes->Cart.RomSpec.MapperNumber != 0)
   {
      Assert(!"Unimplemented mapper number:");
   }

   memcpy(Nes->Ram + 0x8000, Nes->Cart.Rom + 16, KB(16));
   //memcpy(Nes->Ram + 0xC000, Nes->Cart.Rom + 16 + KB(16), KB(16));
   memcpy(Nes->Ram + 0xC000, Nes->Cart.Rom + 16, KB(16));
}

inline void
StepPpu(ppu* Ppu)
{
   // TODO(scott): render a single pixel here
}

inline void
Tick(nes* Nes)
{
   StepPpu(&Nes->Ppu);
   StepPpu(&Nes->Ppu);
   StepPpu(&Nes->Ppu);
   Nes->Cpu.CyclesRemaining--;
}


internal u16
InterruptVector(nes* Nes, interrupt_type Type)
{
   switch (Type)
   {
      case Interrupt_Nmi: {
         u16 Vector = (Read(Nes, 0xFFFA) | Read(Nes, 0xFFFB) << 8);
         return Vector;
      } break;

      case Interrupt_Reset: {
         u16 Vector = ((u16)Read(Nes, 0xFFFC) | (u16)Read(Nes, 0xFFFD) << 8);
         return Vector;
      } break;

      case Interrupt_IrqBrk: {
         u16 Vector = (Read(Nes, 0xFFFE) | Read(Nes, 0xFFFF) << 8);
         return Vector;
      } break;

      default: {
         Assert(!"Invalid interrupt type!");
         return 0;
      }      
   }
}

internal void
PowerOn(nes* Nes)
{   
   Nes->Cpu.A = 0;
   Nes->Cpu.X = 0;
   Nes->Cpu.Y = 0;
   Nes->Cpu.S = 0;
   Nes->Cpu.P.Value = 0x04;
   Nes->Cpu.CyclesRemaining = 0;

   memset(Nes->Ram, 0, Nes->RamSize);

   LoadMapper(Nes);

   Nes->Cpu.PC = InterruptVector(Nes, Interrupt_Reset);
}

internal void
RunFrame(nes* Nes, input* Input)
{
   Nes->Cpu.CyclesRemaining += 29781;

   while (Nes->Cpu.CyclesRemaining > 0)
   {
      // TODO(scott): handle interrupts

      u8 Opcode = Read(Nes, Nes->Cpu.PC++);

      switch(Opcode)
      {

         default: {
            printf("Unknown opcode: %02X\n", Opcode);
            exit(1);
         } break;
      }
   }   
}
