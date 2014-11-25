//extern "C"
//{


float __declspec(naked) _CIsin()
{
   _asm {
      fsin
      ret
   };
};

//};

