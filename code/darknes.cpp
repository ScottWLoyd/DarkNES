
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

internal u32
InitNes(nes* Nes)
{   
   u32 RamSize = 2048;
   Nes->Ram = (u8*)malloc(RamSize);
   memset(Nes->Ram, 0, RamSize);

   Nes->Ppu.Display.Height = 240;
   Nes->Ppu.Display.Width = 256;
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

   Nes->Cart.Rom = (u8*)malloc(fsize + 1);
   Nes->Cart.RomSize = fread(Nes->Cart.Rom, 1, fsize, RomFile);
   fclose(RomFile);

   nes2_header* NesHeader = (nes2_header*)Nes->Cart.Rom;
   // TODO(scott): check for "NES<eof>" format here

   // TODO(scott): actually implement NES2.0 format handling

   return 1;
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


internal void
RunFrame(nes* Nes, input* Input)
{
   //RenderTestGradient(&Nes->Ppu.Display);
   Tick(Nes);
}
