#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>

#include "ines.h"
using namespace ines;

struct read_file_result {
   bool Success;
   size_t Length;
   u8* Data;
};

read_file_result ReadEntireFile(char* Path)
{
   read_file_result Result = {0};
   FILE* File = fopen(Path, "rb");
   if (!File)
   {
      return Result;
   }

   fseek(File, 0, SEEK_END);
   u32 Size = ftell(File);
   fseek(File, 0, SEEK_SET);

   Result.Data = (u8*)malloc(Size);
   Result.Length = fread(Result.Data, 1, Size, File);
   fclose(File);

   Result.Success = true;
   return Result;
}

inline char* GetExtension(char* Filename)
{
   char* Str = Filename;
   char* LastDot = 0;
   while (*Str)
   {
      if (*Str == '.')
         LastDot = Str;
      Str++;
   }

   return LastDot + 1;
}

int main(int ArgC, char** Args)
{
   if (ArgC != 2)
   {
      printf("Usage: %s ROM_FILENAME\n", Args[0]);
      exit(1);
   }

   char* Filename = Args[1];
   char* Extension = GetExtension(Filename);
   if (strcmp(Extension, "nes") == 0)
   {
      read_file_result FileResult = ReadEntireFile(Filename);
      if (!FileResult.Success)
      {
         fprintf(stderr, "Failed to open ROM file: %s\n", Filename);
#if _DEBUG
         __debugbreak();
#else
         exit(1);
#endif
      }


      ines_file LoadedRom = LoadInesImage(FileResult.Data, FileResult.Length);
   }


   return 0;
}